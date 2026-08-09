                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__roman_α:
proc_LBL__roman_α_body:
#=======================================================================================================================
# roman   s = ''
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:
                                                                                        jmp   n1_lit_string_α
n0_statement_begin_β:
                                                                                        jmp   n4_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n1_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx248_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n2_assign_α
.Lx248_0:
                        .quad            .Lx248_0_s
.Lx248_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n2_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052320], rax                    # s
                        mov              qword ptr [1879052328], rdx
                                                                                        jmp   n3_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n3_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n4_statement_begin_α
#=======================================================================================================================
#         v = ARRAY(13)
#-----------------------------------------------------------------------------------------------------------------------
n4_statement_begin_α:
                                                                                        jmp   n5_lit_integer_α
n4_statement_begin_β:
                                                                                        jmp   n9_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n5_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx254_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n6_call_α
.Lx254_0:
                        .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n6_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd256:            .string          "ARRAY"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd256]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx255_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n4_statement_begin_β
.Lx255_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n7_assign_α
n6_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n4_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n7_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052336], rax                    # v
                        mov              qword ptr [1879052344], rdx
                                                                                        jmp   n8_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n8_statement_end_α:
                        add              rsp, 32
                                                                                        jmp   n9_statement_begin_α
#=======================================================================================================================
#         v<1> = 1000
#-----------------------------------------------------------------------------------------------------------------------
n9_statement_begin_α:
                                                                                        jmp   n10_var_α
n9_statement_begin_β:
                                                                                        jmp   n16_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n10_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n11_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n11_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx263_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n12_subscript_α
n11_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n9_statement_begin_β
.Lx263_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n12_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx264_240
                        add              rsp, 16
                                                                                        jmp   n11_lit_integer_β
.Lx264_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n13_lit_integer_α
n12_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n11_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n13_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx265_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n14_assign_var_α
n13_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n12_subscript_β
.Lx265_0:
                        .quad            1000
#-----------------------------------------------------------------------------------------------------------------------
n14_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx266_240
                        add              rsp, 16
                                                                                        jmp   n13_lit_integer_β
.Lx266_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n15_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n15_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n16_statement_begin_α
#=======================================================================================================================
#         v<2> = 900
#-----------------------------------------------------------------------------------------------------------------------
n16_statement_begin_α:
                                                                                        jmp   n17_var_α
n16_statement_begin_β:
                                                                                        jmp   n23_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n17_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n18_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n18_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx272_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n19_subscript_α
n18_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n16_statement_begin_β
.Lx272_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n19_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx273_240
                        add              rsp, 16
                                                                                        jmp   n18_lit_integer_β
.Lx273_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n20_lit_integer_α
n19_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n18_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n20_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx274_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n21_assign_var_α
n20_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n19_subscript_β
.Lx274_0:
                        .quad            900
#-----------------------------------------------------------------------------------------------------------------------
n21_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx275_240
                        add              rsp, 16
                                                                                        jmp   n20_lit_integer_β
.Lx275_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n22_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n22_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n23_statement_begin_α
#=======================================================================================================================
#         v<3> = 500
#-----------------------------------------------------------------------------------------------------------------------
n23_statement_begin_α:
                                                                                        jmp   n24_var_α
n23_statement_begin_β:
                                                                                        jmp   n30_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n24_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n25_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n25_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx281_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n26_subscript_α
n25_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n23_statement_begin_β
.Lx281_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n26_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx282_240
                        add              rsp, 16
                                                                                        jmp   n25_lit_integer_β
.Lx282_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n27_lit_integer_α
n26_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n25_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n27_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx283_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n28_assign_var_α
n27_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n26_subscript_β
.Lx283_0:
                        .quad            500
#-----------------------------------------------------------------------------------------------------------------------
n28_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx284_240
                        add              rsp, 16
                                                                                        jmp   n27_lit_integer_β
.Lx284_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n29_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n29_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n30_statement_begin_α
#=======================================================================================================================
#         v<4> = 400
#-----------------------------------------------------------------------------------------------------------------------
n30_statement_begin_α:
                                                                                        jmp   n31_var_α
n30_statement_begin_β:
                                                                                        jmp   n37_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n31_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n32_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n32_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx290_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n33_subscript_α
n32_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n30_statement_begin_β
.Lx290_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n33_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx291_240
                        add              rsp, 16
                                                                                        jmp   n32_lit_integer_β
.Lx291_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n34_lit_integer_α
n33_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n32_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n34_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx292_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n35_assign_var_α
n34_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n33_subscript_β
.Lx292_0:
                        .quad            400
#-----------------------------------------------------------------------------------------------------------------------
n35_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx293_240
                        add              rsp, 16
                                                                                        jmp   n34_lit_integer_β
.Lx293_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n36_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n36_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n37_statement_begin_α
#=======================================================================================================================
#         v<5> = 100
#-----------------------------------------------------------------------------------------------------------------------
n37_statement_begin_α:
                                                                                        jmp   n38_var_α
n37_statement_begin_β:
                                                                                        jmp   n44_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n38_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n39_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n39_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx299_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n40_subscript_α
n39_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n37_statement_begin_β
.Lx299_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n40_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx300_240
                        add              rsp, 16
                                                                                        jmp   n39_lit_integer_β
.Lx300_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n41_lit_integer_α
n40_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n39_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n41_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx301_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n42_assign_var_α
n41_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n40_subscript_β
.Lx301_0:
                        .quad            100
#-----------------------------------------------------------------------------------------------------------------------
n42_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx302_240
                        add              rsp, 16
                                                                                        jmp   n41_lit_integer_β
.Lx302_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n43_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n43_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n44_statement_begin_α
#=======================================================================================================================
#         v<6> = 90
#-----------------------------------------------------------------------------------------------------------------------
n44_statement_begin_α:
                                                                                        jmp   n45_var_α
n44_statement_begin_β:
                                                                                        jmp   n51_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n45_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n46_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n46_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx308_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n47_subscript_α
n46_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n44_statement_begin_β
.Lx308_0:
                        .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n47_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx309_240
                        add              rsp, 16
                                                                                        jmp   n46_lit_integer_β
.Lx309_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n48_lit_integer_α
n47_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n46_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n48_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx310_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n49_assign_var_α
n48_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n47_subscript_β
.Lx310_0:
                        .quad            90
#-----------------------------------------------------------------------------------------------------------------------
n49_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx311_240
                        add              rsp, 16
                                                                                        jmp   n48_lit_integer_β
.Lx311_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n50_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n50_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n51_statement_begin_α
#=======================================================================================================================
#         v<7> = 50
#-----------------------------------------------------------------------------------------------------------------------
n51_statement_begin_α:
                                                                                        jmp   n52_var_α
n51_statement_begin_β:
                                                                                        jmp   n58_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n52_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n53_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n53_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx317_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n54_subscript_α
n53_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n51_statement_begin_β
.Lx317_0:
                        .quad            7
#-----------------------------------------------------------------------------------------------------------------------
n54_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx318_240
                        add              rsp, 16
                                                                                        jmp   n53_lit_integer_β
.Lx318_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n55_lit_integer_α
n54_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n53_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n55_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx319_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n56_assign_var_α
n55_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n54_subscript_β
.Lx319_0:
                        .quad            50
#-----------------------------------------------------------------------------------------------------------------------
n56_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx320_240
                        add              rsp, 16
                                                                                        jmp   n55_lit_integer_β
.Lx320_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n57_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n57_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n58_statement_begin_α
#=======================================================================================================================
#         v<8> = 40
#-----------------------------------------------------------------------------------------------------------------------
n58_statement_begin_α:
                                                                                        jmp   n59_var_α
n58_statement_begin_β:
                                                                                        jmp   n65_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n59_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n60_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n60_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx326_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n61_subscript_α
n60_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n58_statement_begin_β
.Lx326_0:
                        .quad            8
#-----------------------------------------------------------------------------------------------------------------------
n61_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx327_240
                        add              rsp, 16
                                                                                        jmp   n60_lit_integer_β
.Lx327_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n62_lit_integer_α
n61_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n60_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n62_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx328_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n63_assign_var_α
n62_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n61_subscript_β
.Lx328_0:
                        .quad            40
#-----------------------------------------------------------------------------------------------------------------------
n63_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx329_240
                        add              rsp, 16
                                                                                        jmp   n62_lit_integer_β
.Lx329_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n64_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n64_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n65_statement_begin_α
#=======================================================================================================================
#         v<9> = 10
#-----------------------------------------------------------------------------------------------------------------------
n65_statement_begin_α:
                                                                                        jmp   n66_var_α
n65_statement_begin_β:
                                                                                        jmp   n72_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n66_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n67_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n67_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx335_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n68_subscript_α
n67_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n65_statement_begin_β
.Lx335_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n68_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx336_240
                        add              rsp, 16
                                                                                        jmp   n67_lit_integer_β
.Lx336_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n69_lit_integer_α
n68_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n67_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n69_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx337_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n70_assign_var_α
n69_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n68_subscript_β
.Lx337_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n70_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx338_240
                        add              rsp, 16
                                                                                        jmp   n69_lit_integer_β
.Lx338_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n71_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n71_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n72_statement_begin_α
#=======================================================================================================================
#         v<10> = 9
#-----------------------------------------------------------------------------------------------------------------------
n72_statement_begin_α:
                                                                                        jmp   n73_var_α
n72_statement_begin_β:
                                                                                        jmp   n79_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n73_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n74_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n74_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx344_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n75_subscript_α
n74_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n72_statement_begin_β
.Lx344_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n75_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx345_240
                        add              rsp, 16
                                                                                        jmp   n74_lit_integer_β
.Lx345_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n76_lit_integer_α
n75_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n74_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n76_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx346_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n77_assign_var_α
n76_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n75_subscript_β
.Lx346_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n77_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx347_240
                        add              rsp, 16
                                                                                        jmp   n76_lit_integer_β
.Lx347_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n78_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n78_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n79_statement_begin_α
#=======================================================================================================================
#         v<11> = 5
#-----------------------------------------------------------------------------------------------------------------------
n79_statement_begin_α:
                                                                                        jmp   n80_var_α
n79_statement_begin_β:
                                                                                        jmp   n86_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n80_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n81_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n81_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx353_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n82_subscript_α
n81_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n79_statement_begin_β
.Lx353_0:
                        .quad            11
#-----------------------------------------------------------------------------------------------------------------------
n82_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx354_240
                        add              rsp, 16
                                                                                        jmp   n81_lit_integer_β
.Lx354_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n83_lit_integer_α
n82_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n81_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n83_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx355_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n84_assign_var_α
n83_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n82_subscript_β
.Lx355_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n84_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx356_240
                        add              rsp, 16
                                                                                        jmp   n83_lit_integer_β
.Lx356_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n85_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n85_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n86_statement_begin_α
#=======================================================================================================================
#         v<12> = 4
#-----------------------------------------------------------------------------------------------------------------------
n86_statement_begin_α:
                                                                                        jmp   n87_var_α
n86_statement_begin_β:
                                                                                        jmp   n93_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n87_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n88_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n88_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx362_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n89_subscript_α
n88_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n86_statement_begin_β
.Lx362_0:
                        .quad            12
#-----------------------------------------------------------------------------------------------------------------------
n89_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx363_240
                        add              rsp, 16
                                                                                        jmp   n88_lit_integer_β
.Lx363_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n90_lit_integer_α
n89_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n88_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n90_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx364_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n91_assign_var_α
n90_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n89_subscript_β
.Lx364_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n91_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx365_240
                        add              rsp, 16
                                                                                        jmp   n90_lit_integer_β
.Lx365_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n92_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n92_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n93_statement_begin_α
#=======================================================================================================================
#         v<13> = 1
#-----------------------------------------------------------------------------------------------------------------------
n93_statement_begin_α:
                                                                                        jmp   n94_var_α
n93_statement_begin_β:
                                                                                        jmp   n100_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n94_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n95_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n95_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx371_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n96_subscript_α
n95_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n93_statement_begin_β
.Lx371_0:
                        .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n96_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx372_240
                        add              rsp, 16
                                                                                        jmp   n95_lit_integer_β
.Lx372_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n97_lit_integer_α
n96_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n95_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n97_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx373_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n98_assign_var_α
n97_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n96_subscript_β
.Lx373_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n98_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx374_240
                        add              rsp, 16
                                                                                        jmp   n97_lit_integer_β
.Lx374_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n99_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n99_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n100_statement_begin_α
#=======================================================================================================================
#         r = ARRAY(13)
#-----------------------------------------------------------------------------------------------------------------------
n100_statement_begin_α:
                                                                                        jmp   n101_lit_integer_α
n100_statement_begin_β:
                                                                                        jmp   n105_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n101_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx379_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n102_call_α
.Lx379_0:
                        .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n102_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd381:            .string          "ARRAY"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd381]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx380_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n100_statement_begin_β
.Lx380_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n103_assign_α
n102_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n100_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n103_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052352], rax                    # r
                        mov              qword ptr [1879052360], rdx
                                                                                        jmp   n104_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n104_statement_end_α:
                        add              rsp, 32
                                                                                        jmp   n105_statement_begin_α
#=======================================================================================================================
#         r<1> = 'M'
#-----------------------------------------------------------------------------------------------------------------------
n105_statement_begin_α:
                                                                                        jmp   n106_var_α
n105_statement_begin_β:
                                                                                        jmp   n112_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n106_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n107_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n107_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx388_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n108_subscript_α
n107_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n105_statement_begin_β
.Lx388_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n108_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx389_240
                        add              rsp, 16
                                                                                        jmp   n107_lit_integer_β
.Lx389_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n109_lit_string_α
n108_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n107_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n109_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx390_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n110_assign_var_α
n109_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n108_subscript_β
.Lx390_0:
                        .quad            .Lx390_0_s
.Lx390_0_s:
                        .string          "M"
#-----------------------------------------------------------------------------------------------------------------------
n110_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx391_240
                        add              rsp, 16
                                                                                        jmp   n109_lit_string_β
.Lx391_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n111_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n111_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n112_statement_begin_α
#=======================================================================================================================
#         r<2> = 'CM'
#-----------------------------------------------------------------------------------------------------------------------
n112_statement_begin_α:
                                                                                        jmp   n113_var_α
n112_statement_begin_β:
                                                                                        jmp   n119_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n113_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n114_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n114_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx397_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n115_subscript_α
n114_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n112_statement_begin_β
.Lx397_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n115_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx398_240
                        add              rsp, 16
                                                                                        jmp   n114_lit_integer_β
.Lx398_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n116_lit_string_α
n115_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n114_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n116_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx399_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n117_assign_var_α
n116_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n115_subscript_β
.Lx399_0:
                        .quad            .Lx399_0_s
.Lx399_0_s:
                        .string          "CM"
#-----------------------------------------------------------------------------------------------------------------------
n117_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx400_240
                        add              rsp, 16
                                                                                        jmp   n116_lit_string_β
.Lx400_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n118_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n118_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n119_statement_begin_α
#=======================================================================================================================
#         r<3> = 'D'
#-----------------------------------------------------------------------------------------------------------------------
n119_statement_begin_α:
                                                                                        jmp   n120_var_α
n119_statement_begin_β:
                                                                                        jmp   n126_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n120_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n121_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n121_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx406_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n122_subscript_α
n121_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n119_statement_begin_β
.Lx406_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n122_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx407_240
                        add              rsp, 16
                                                                                        jmp   n121_lit_integer_β
.Lx407_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n123_lit_string_α
n122_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n121_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n123_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx408_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n124_assign_var_α
n123_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n122_subscript_β
.Lx408_0:
                        .quad            .Lx408_0_s
.Lx408_0_s:
                        .string          "D"
#-----------------------------------------------------------------------------------------------------------------------
n124_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx409_240
                        add              rsp, 16
                                                                                        jmp   n123_lit_string_β
.Lx409_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n125_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n125_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n126_statement_begin_α
#=======================================================================================================================
#         r<4> = 'CD'
#-----------------------------------------------------------------------------------------------------------------------
n126_statement_begin_α:
                                                                                        jmp   n127_var_α
n126_statement_begin_β:
                                                                                        jmp   n133_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n127_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n128_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n128_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx415_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n129_subscript_α
n128_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n126_statement_begin_β
.Lx415_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n129_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx416_240
                        add              rsp, 16
                                                                                        jmp   n128_lit_integer_β
.Lx416_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n130_lit_string_α
n129_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n128_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n130_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx417_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n131_assign_var_α
n130_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n129_subscript_β
.Lx417_0:
                        .quad            .Lx417_0_s
.Lx417_0_s:
                        .string          "CD"
#-----------------------------------------------------------------------------------------------------------------------
n131_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx418_240
                        add              rsp, 16
                                                                                        jmp   n130_lit_string_β
.Lx418_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n132_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n132_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n133_statement_begin_α
#=======================================================================================================================
#         r<5> = 'C'
#-----------------------------------------------------------------------------------------------------------------------
n133_statement_begin_α:
                                                                                        jmp   n134_var_α
n133_statement_begin_β:
                                                                                        jmp   n140_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n134_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n135_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n135_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx424_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n136_subscript_α
n135_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n133_statement_begin_β
.Lx424_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n136_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx425_240
                        add              rsp, 16
                                                                                        jmp   n135_lit_integer_β
.Lx425_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n137_lit_string_α
n136_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n135_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n137_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx426_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n138_assign_var_α
n137_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n136_subscript_β
.Lx426_0:
                        .quad            .Lx426_0_s
.Lx426_0_s:
                        .string          "C"
#-----------------------------------------------------------------------------------------------------------------------
n138_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx427_240
                        add              rsp, 16
                                                                                        jmp   n137_lit_string_β
.Lx427_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n139_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n139_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n140_statement_begin_α
#=======================================================================================================================
#         r<6> = 'XC'
#-----------------------------------------------------------------------------------------------------------------------
n140_statement_begin_α:
                                                                                        jmp   n141_var_α
n140_statement_begin_β:
                                                                                        jmp   n147_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n141_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n142_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n142_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx433_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n143_subscript_α
n142_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n140_statement_begin_β
.Lx433_0:
                        .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n143_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx434_240
                        add              rsp, 16
                                                                                        jmp   n142_lit_integer_β
.Lx434_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n144_lit_string_α
n143_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n142_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n144_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx435_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n145_assign_var_α
n144_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n143_subscript_β
.Lx435_0:
                        .quad            .Lx435_0_s
.Lx435_0_s:
                        .string          "XC"
#-----------------------------------------------------------------------------------------------------------------------
n145_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx436_240
                        add              rsp, 16
                                                                                        jmp   n144_lit_string_β
.Lx436_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n146_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n146_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n147_statement_begin_α
#=======================================================================================================================
#         r<7> = 'L'
#-----------------------------------------------------------------------------------------------------------------------
n147_statement_begin_α:
                                                                                        jmp   n148_var_α
n147_statement_begin_β:
                                                                                        jmp   n154_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n148_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n149_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n149_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx442_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n150_subscript_α
n149_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n147_statement_begin_β
.Lx442_0:
                        .quad            7
#-----------------------------------------------------------------------------------------------------------------------
n150_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx443_240
                        add              rsp, 16
                                                                                        jmp   n149_lit_integer_β
.Lx443_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n151_lit_string_α
n150_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n149_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n151_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx444_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n152_assign_var_α
n151_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n150_subscript_β
.Lx444_0:
                        .quad            .Lx444_0_s
.Lx444_0_s:
                        .string          "L"
#-----------------------------------------------------------------------------------------------------------------------
n152_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx445_240
                        add              rsp, 16
                                                                                        jmp   n151_lit_string_β
.Lx445_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n153_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n153_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n154_statement_begin_α
#=======================================================================================================================
#         r<8> = 'XL'
#-----------------------------------------------------------------------------------------------------------------------
n154_statement_begin_α:
                                                                                        jmp   n155_var_α
n154_statement_begin_β:
                                                                                        jmp   n161_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n155_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n156_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n156_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx451_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n157_subscript_α
n156_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n154_statement_begin_β
.Lx451_0:
                        .quad            8
#-----------------------------------------------------------------------------------------------------------------------
n157_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx452_240
                        add              rsp, 16
                                                                                        jmp   n156_lit_integer_β
.Lx452_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n158_lit_string_α
n157_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n156_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n158_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx453_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n159_assign_var_α
n158_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n157_subscript_β
.Lx453_0:
                        .quad            .Lx453_0_s
.Lx453_0_s:
                        .string          "XL"
#-----------------------------------------------------------------------------------------------------------------------
n159_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx454_240
                        add              rsp, 16
                                                                                        jmp   n158_lit_string_β
.Lx454_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n160_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n160_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n161_statement_begin_α
#=======================================================================================================================
#         r<9> = 'X'
#-----------------------------------------------------------------------------------------------------------------------
n161_statement_begin_α:
                                                                                        jmp   n162_var_α
n161_statement_begin_β:
                                                                                        jmp   n168_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n162_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n163_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n163_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx460_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n164_subscript_α
n163_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n161_statement_begin_β
.Lx460_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n164_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx461_240
                        add              rsp, 16
                                                                                        jmp   n163_lit_integer_β
.Lx461_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n165_lit_string_α
n164_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n163_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n165_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx462_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n166_assign_var_α
n165_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n164_subscript_β
.Lx462_0:
                        .quad            .Lx462_0_s
.Lx462_0_s:
                        .string          "X"
#-----------------------------------------------------------------------------------------------------------------------
n166_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx463_240
                        add              rsp, 16
                                                                                        jmp   n165_lit_string_β
.Lx463_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n167_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n167_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n168_statement_begin_α
#=======================================================================================================================
#         r<10> = 'IX'
#-----------------------------------------------------------------------------------------------------------------------
n168_statement_begin_α:
                                                                                        jmp   n169_var_α
n168_statement_begin_β:
                                                                                        jmp   n175_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n169_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n170_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n170_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx469_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n171_subscript_α
n170_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n168_statement_begin_β
.Lx469_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n171_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx470_240
                        add              rsp, 16
                                                                                        jmp   n170_lit_integer_β
.Lx470_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n172_lit_string_α
n171_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n170_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n172_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx471_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n173_assign_var_α
n172_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n171_subscript_β
.Lx471_0:
                        .quad            .Lx471_0_s
.Lx471_0_s:
                        .string          "IX"
#-----------------------------------------------------------------------------------------------------------------------
n173_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx472_240
                        add              rsp, 16
                                                                                        jmp   n172_lit_string_β
.Lx472_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n174_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n174_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n175_statement_begin_α
#=======================================================================================================================
#         r<11> = 'V'
#-----------------------------------------------------------------------------------------------------------------------
n175_statement_begin_α:
                                                                                        jmp   n176_var_α
n175_statement_begin_β:
                                                                                        jmp   n182_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n176_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n177_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n177_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx478_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n178_subscript_α
n177_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n175_statement_begin_β
.Lx478_0:
                        .quad            11
#-----------------------------------------------------------------------------------------------------------------------
n178_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx479_240
                        add              rsp, 16
                                                                                        jmp   n177_lit_integer_β
.Lx479_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n179_lit_string_α
n178_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n177_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n179_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx480_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n180_assign_var_α
n179_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n178_subscript_β
.Lx480_0:
                        .quad            .Lx480_0_s
.Lx480_0_s:
                        .string          "V"
#-----------------------------------------------------------------------------------------------------------------------
n180_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx481_240
                        add              rsp, 16
                                                                                        jmp   n179_lit_string_β
.Lx481_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n181_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n181_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n182_statement_begin_α
#=======================================================================================================================
#         r<12> = 'IV'
#-----------------------------------------------------------------------------------------------------------------------
n182_statement_begin_α:
                                                                                        jmp   n183_var_α
n182_statement_begin_β:
                                                                                        jmp   n189_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n183_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n184_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n184_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx487_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n185_subscript_α
n184_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n182_statement_begin_β
.Lx487_0:
                        .quad            12
#-----------------------------------------------------------------------------------------------------------------------
n185_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx488_240
                        add              rsp, 16
                                                                                        jmp   n184_lit_integer_β
.Lx488_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n186_lit_string_α
n185_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n184_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n186_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx489_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n187_assign_var_α
n186_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n185_subscript_β
.Lx489_0:
                        .quad            .Lx489_0_s
.Lx489_0_s:
                        .string          "IV"
#-----------------------------------------------------------------------------------------------------------------------
n187_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx490_240
                        add              rsp, 16
                                                                                        jmp   n186_lit_string_β
.Lx490_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n188_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n188_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n189_statement_begin_α
#=======================================================================================================================
#         r<13> = 'I'
#-----------------------------------------------------------------------------------------------------------------------
n189_statement_begin_α:
                                                                                        jmp   n190_var_α
n189_statement_begin_β:
                                                                                        jmp   n196_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n190_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n191_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n191_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx496_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n192_subscript_α
n191_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n189_statement_begin_β
.Lx496_0:
                        .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n192_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx497_240
                        add              rsp, 16
                                                                                        jmp   n191_lit_integer_β
.Lx497_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n193_lit_string_α
n192_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n191_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n193_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx498_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n194_assign_var_α
n193_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n192_subscript_β
.Lx498_0:
                        .quad            .Lx498_0_s
.Lx498_0_s:
                        .string          "I"
#-----------------------------------------------------------------------------------------------------------------------
n194_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx499_240
                        add              rsp, 16
                                                                                        jmp   n193_lit_string_β
.Lx499_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n195_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n195_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n196_statement_begin_α
#=======================================================================================================================
#         i = 1
#-----------------------------------------------------------------------------------------------------------------------
n196_statement_begin_α:
                                                                                        jmp   n197_lit_integer_α
n196_statement_begin_β:
                                                                                        jmp   n200_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n197_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx504_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n198_assign_α
.Lx504_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n198_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_integer
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052368], rax                    # i
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n199_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n199_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n200_statement_begin_α
#=======================================================================================================================
# RLOOP   GT(n, 0)                                                       :F(RDONE)
#-----------------------------------------------------------------------------------------------------------------------
n200_statement_begin_α:
                                                                                        jmp   n201_var_α
n200_statement_begin_β:
                                                                                        jmp   n241_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n201_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # n
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n202_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n202_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx511_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n203_coerce_numeric_α
n202_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n200_statement_begin_β
.Lx511_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n203_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # var
                        cmp              eax, 5
                                                                                        je    .Lx513_1
                        cmp              eax, 3
                                                                                        jne   .Lx513_0
                        mov              eax, dword ptr [rsp + 16]                      # lit_integer
                        cmp              eax, 3
                                                                                        jne   .Lx513_0
.Lx513_1:
                        mov              rax, qword ptr [rsp + 32]                      # var
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # var
                        mov              qword ptr [rsp + 8], rax                       # result
                                                                                        jmp   n204_coerce_numeric_α
.Lx513_0:
                        lea              rdi, [rsp + 32]                                # var
                        lea              rsi, [rsp + 16]                                # lit_integer
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 111                                       # codes
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n204_coerce_numeric_α
n203_coerce_numeric_β:
                        add              rsp, 16
                                                                                        jmp   n202_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n204_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # lit_integer
                        cmp              eax, 5
                                                                                        je    .Lx515_1
                        cmp              eax, 3
                                                                                        jne   .Lx515_0
                        mov              eax, dword ptr [rsp + 48]                      # var
                        cmp              eax, 3
                                                                                        jne   .Lx515_0
.Lx515_1:
                        mov              rax, qword ptr [rsp + 32]                      # lit_integer
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # lit_integer
                        mov              qword ptr [rsp + 8], rax                       # result
                                                                                        jmp   n205_cmp_test_α
.Lx515_0:
                        lea              rdi, [rsp + 32]                                # lit_integer
                        lea              rsi, [rsp + 48]                                # var
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 112                                       # codes
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n205_cmp_test_α
n204_coerce_numeric_β:
                        add              rsp, 16
                                                                                        jmp   n203_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n205_cmp_test_α:
                        sub              rsp, 16
                        lea              rdi, [rsp + 32]                                # coerce_numeric
                        lea              rsi, [rsp + 16]                                # b
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jg    .Lx517_240
                        add              rsp, 16
                                                                                        jmp   n204_coerce_numeric_β
.Lx517_240:
                                                                                        jmp   n206_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n206_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n207_statement_begin_α
#=======================================================================================================================
#         GE(n, v<i>)                                                    :F(RNEXT)
#-----------------------------------------------------------------------------------------------------------------------
n207_statement_begin_α:
                                                                                        jmp   n208_var_α
n207_statement_begin_β:
                                                                                        jmp   n235_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n208_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # n
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n209_var_α
#-----------------------------------------------------------------------------------------------------------------------
n209_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n210_var_α
n209_var_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n207_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n210_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052368]                    # i
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n211_subscript_α
n210_var_β:
                        add              rsp, 16
                                                                                        jmp   n209_var_β
#-----------------------------------------------------------------------------------------------------------------------
n211_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # idx
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx525_240
                        add              rsp, 16
                                                                                        jmp   n210_var_β
.Lx525_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n212_deref_α
n211_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n210_var_β
#-----------------------------------------------------------------------------------------------------------------------
n212_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # subscript
                        mov              rsi, qword ptr [rsp + 24]                      # d
                        call             rt_deref@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx526_240
                        add              rsp, 16
                                                                                        jmp   n211_subscript_β
.Lx526_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n213_coerce_numeric_α
n212_deref_β:
                        add              rsp, 16
                                                                                        jmp   n211_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n213_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 80]                      # var
                        cmp              eax, 5
                                                                                        je    .Lx528_1
                        cmp              eax, 3
                                                                                        jne   .Lx528_0
                        mov              eax, dword ptr [rsp + 16]                      # deref
                        cmp              eax, 3
                                                                                        jne   .Lx528_0
.Lx528_1:
                        mov              rax, qword ptr [rsp + 80]                      # var
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 88]                      # var
                        mov              qword ptr [rsp + 8], rax                       # result
                                                                                        jmp   n214_coerce_numeric_α
.Lx528_0:
                        lea              rdi, [rsp + 80]                                # var
                        lea              rsi, [rsp + 16]                                # deref
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 109                                       # codes
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n214_coerce_numeric_α
n213_coerce_numeric_β:
                        add              rsp, 16
                                                                                        jmp   n212_deref_β
#-----------------------------------------------------------------------------------------------------------------------
n214_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # deref
                        cmp              eax, 5
                                                                                        je    .Lx530_1
                        cmp              eax, 3
                                                                                        jne   .Lx530_0
                        mov              eax, dword ptr [rsp + 96]                      # var
                        cmp              eax, 3
                                                                                        jne   .Lx530_0
.Lx530_1:
                        mov              rax, qword ptr [rsp + 32]                      # deref
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # deref
                        mov              qword ptr [rsp + 8], rax                       # result
                                                                                        jmp   n215_cmp_test_α
.Lx530_0:
                        lea              rdi, [rsp + 32]                                # deref
                        lea              rsi, [rsp + 96]                                # var
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 110                                       # codes
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n215_cmp_test_α
n214_coerce_numeric_β:
                        add              rsp, 16
                                                                                        jmp   n213_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n215_cmp_test_α:
                        sub              rsp, 16
                        lea              rdi, [rsp + 32]                                # coerce_numeric
                        lea              rsi, [rsp + 16]                                # b
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jns   .Lx532_240
                        add              rsp, 16
                                                                                        jmp   n214_coerce_numeric_β
.Lx532_240:
                                                                                        jmp   n216_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n216_statement_end_α:
                        add              rsp, 128
                                                                                        jmp   n217_statement_begin_α
#=======================================================================================================================
#         s = s r<i>
#-----------------------------------------------------------------------------------------------------------------------
n217_statement_begin_α:
                                                                                        jmp   n218_var_α
n217_statement_begin_β:
                                                                                        jmp   n226_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n218_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # s
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n219_var_α
#-----------------------------------------------------------------------------------------------------------------------
n219_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n220_var_α
n219_var_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n217_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n220_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052368]                    # i
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n221_subscript_α
n220_var_β:
                        add              rsp, 16
                                                                                        jmp   n219_var_β
#-----------------------------------------------------------------------------------------------------------------------
n221_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # idx
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx540_240
                        add              rsp, 16
                                                                                        jmp   n220_var_β
.Lx540_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n222_deref_α
n221_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n220_var_β
#-----------------------------------------------------------------------------------------------------------------------
n222_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # subscript
                        mov              rsi, qword ptr [rsp + 24]                      # d
                        call             rt_deref@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx541_240
                        add              rsp, 16
                                                                                        jmp   n221_subscript_β
.Lx541_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n223_binop_α
n222_deref_β:
                        add              rsp, 16
                                                                                        jmp   n221_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n223_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 80]                      # var
                        mov              rsi, qword ptr [rsp + 88]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # deref
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n224_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n224_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052320], rax                    # s
                        mov              qword ptr [1879052328], rdx
                                                                                        jmp   n225_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n225_statement_end_α:
                        add              rsp, 96
                                                                                        jmp   n226_statement_begin_α
#=======================================================================================================================
#         n = n - v<i>                                                   :(RLOOP)
#-----------------------------------------------------------------------------------------------------------------------
n226_statement_begin_α:
                                                                                        jmp   n227_var_α
n226_statement_begin_β:
                                                                                        jmp   n200_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n227_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # n
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n228_var_α
#-----------------------------------------------------------------------------------------------------------------------
n228_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n229_var_α
n228_var_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n226_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n229_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052368]                    # i
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n230_subscript_α
n229_var_β:
                        add              rsp, 16
                                                                                        jmp   n228_var_β
#-----------------------------------------------------------------------------------------------------------------------
n230_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # idx
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx551_240
                        add              rsp, 16
                                                                                        jmp   n229_var_β
.Lx551_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n231_deref_α
n230_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n229_var_β
#-----------------------------------------------------------------------------------------------------------------------
n231_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # subscript
                        mov              rsi, qword ptr [rsp + 24]                      # d
                        call             rt_deref@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx552_240
                        add              rsp, 16
                                                                                        jmp   n230_subscript_β
.Lx552_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n232_binop_α
n231_deref_β:
                        add              rsp, 16
                                                                                        jmp   n230_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n232_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 80]                      # var
                        mov              rsi, qword ptr [rsp + 88]
                        mov              rdx, qword ptr [rsp + 16]                      # deref
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_sub@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx553_240
                        add              rsp, 16
                                                                                        jmp   n231_deref_β
.Lx553_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n233_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n233_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax                    # n
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   n234_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n234_statement_end_α:
                        add              rsp, 96
                                                                                        jmp   n200_statement_begin_α
#=======================================================================================================================
# RNEXT   i = i + 1                                                      :(RLOOP)
#-----------------------------------------------------------------------------------------------------------------------
n235_statement_begin_α:
                                                                                        jmp   n236_var_α
n235_statement_begin_β:
                                                                                        jmp   n200_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n236_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052368]                    # i
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n237_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n237_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx560_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n238_binop_α
n237_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n235_statement_begin_β
.Lx560_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n238_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_add@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx561_240
                        add              rsp, 16
                                                                                        jmp   n237_lit_integer_β
.Lx561_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n239_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n239_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052368], rax                    # i
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n240_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n240_statement_end_α:
                        add              rsp, 48
                                                                                        jmp   n200_statement_begin_α
#=======================================================================================================================
# RDONE   roman = s                                                      :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n241_statement_begin_α:
                                                                                        jmp   n242_var_α
n241_statement_begin_β:
                                                                                        jmp   n245_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n242_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # s
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n243_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n243_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax                    # roman
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n244_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n244_statement_end_α:
                                                                                        jmp   n245_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n245_save_restore_α:
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx572_0
                        mov              cl, 0
                        mov              rax, qword ptr [rcx + -64]
                                                                                        jmp   rax
.Lx572_0:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__roman_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__roman_β:
                                                                                        jmp   proc_LBL__roman_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__roman_γ:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__roman_ω:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_roman_α
proc_roman_α:
proc_roman_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n573_save_restore_α:
                        mov              rdi, rcx                                       # gw
                        mov              rsi, rdx                                       # ww
                        lea              rdx, [rsp + 0]                                 # rsp
                        mov              rcx, rbp                                       # rbp
                        call             rt_flat_wire_adopt@PLT
                                                                                        jmp   n574_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n574_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx578_0]                # name
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx578_1
.Lx578_0:
                        .quad            .Lx578_0_s
.Lx578_0_s:
                        .string          "roman"
.Lx578_1:
                                                                                        jmp   proc_roman_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_roman_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_roman_β:
                                                                                        jmp   proc_roman_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_roman_γ:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_roman_ω:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lstartup_pname0:       .string          "LBL__roman"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_LBL__roman_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 3104
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname1:       .string          "roman"
.Lstartup_pp1_0:        .string          "n"
.Lstartup_pp1_1:        .string          "s"
.Lstartup_pp1_2:        .string          "v"
.Lstartup_pp1_3:        .string          "r"
.Lstartup_pp1_4:        .string          "i"
                        .align           8
.Lstartup_pnames1:
                        .quad            .Lstartup_pp1_0
                        .quad            .Lstartup_pp1_1
                        .quad            .Lstartup_pp1_2
                        .quad            .Lstartup_pp1_3
                        .quad            .Lstartup_pp1_4
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname1]
                        lea              rsi, [rip + .Lstartup_pnames1]
                        mov              edx, 5
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        lea              rsi, [rip + proc_roman_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 5
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 1
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 16
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        add              rsp, 8
                        ret
                        .section         .rodata
.Lgvan0:                .string          "roman"
.Lgvan1:                .string          "n"
.Lgvan2:                .string          "s"
.Lgvan3:                .string          "v"
.Lgvan4:                .string          "r"
.Lgvan5:                .string          "i"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .quad            .Lgvan1
                        .quad            .Lgvan2
                        .quad            .Lgvan3
                        .quad            .Lgvan4
                        .quad            .Lgvan5
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             proc_startup
                        mov              edi, 6
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 6
                        call             gva_register@PLT
                        mov              r12, qword ptr [0x70000000]
                        xor              esi, esi
                                                                                        jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
#=======================================================================================================================
#         DEFINE('roman(n)s,v,r,i')                                     :(roman_end)
#-----------------------------------------------------------------------------------------------------------------------
n579_statement_begin_α:
                                                                                        jmp   n580_func_activate_α
n579_statement_begin_β:
                                                                                        jmp   n582_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n580_func_activate_α:
                        lea              rax, [rip + roman_act_α]
                        mov              r11, qword ptr [rip + fn_cell$roman@GOTPCREL]
                        mov              qword ptr [r11 + 0], rax
                                                                                        jmp   n581_statement_end_α
n580_func_activate_β:
                                                                                        jmp   n579_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n581_statement_end_α:
                                                                                        jmp   n582_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n582_statement_begin_α:
                                                                                        jmp   n583_statement_end_α
n582_statement_begin_β:
                                                                                        jmp   n584_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n583_statement_end_α:
                                                                                        jmp   n584_statement_begin_α
#=======================================================================================================================
#         OUTPUT = roman(1)
#-----------------------------------------------------------------------------------------------------------------------
n584_statement_begin_α:
                                                                                        jmp   n585_lit_integer_α
n584_statement_begin_β:
                                                                                        jmp   n589_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n585_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx877_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n586_call_α
.Lx877_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n586_call_α:
                        sub              rsp, 16
                        mov              edi, 0                                         # idx
                        mov              rsi, qword ptr [rsp + 16]                      # lit_integer
                        mov              rdx, qword ptr [rsp + 24]                      # v
                        call             rt_arg_stage@PLT
                        sub              rsp, 96
                        mov              rax, qword ptr [1879052304]                    # n
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052320]                    # s
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 56], rax
                        mov              rax, qword ptr [1879052368]                    # i
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [1879052376]
                        mov              qword ptr [rsp + 72], rax
                        mov              rax, qword ptr [1879052288]                    # roman
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 88], rax
                        mov              rdi, qword ptr [rip + .Lx879_0]                # name
                        mov              esi, 5                                         # np
                        mov              edx, 1                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx879_5
                        lea              r10, [rip + g_call_args]
                        mov              rax, qword ptr [r10 + 0]
                        mov              qword ptr [1879052304], rax                    # n
                        mov              rax, qword ptr [r10 + 8]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx879_6]
                        lea              rdx, [rip + .Lx879_7]
                                                                                        jmp   rax
.Lx879_6:
                        mov              rdi, qword ptr [1879052288]                    # roman
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax                    # i
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax                    # r
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax                    # v
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax                    # s
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax                    # n
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx879_2
.Lx879_7:
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax                    # roman
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax                    # i
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax                    # r
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax                    # v
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax                    # s
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax                    # n
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx879_2
.Lx879_5:
                        add              rsp, 96
.Lx879_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx879_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n584_statement_begin_β
.Lx879_240:
                                                                                        jmp   n587_assign_α
n586_call_β:
                                                                                        jmp   n584_statement_begin_β
.Lx879_0:
                        .quad            .Lx879_0_s
.Lx879_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n587_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx880_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n588_statement_end_α
.Lx880_0:
                        .quad            .Lx880_0_s
.Lx880_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n588_statement_end_α:
                        add              rsp, 32
                                                                                        jmp   n589_statement_begin_α
#=======================================================================================================================
#         OUTPUT = roman(4)
#-----------------------------------------------------------------------------------------------------------------------
n589_statement_begin_α:
                                                                                        jmp   n590_lit_integer_α
n589_statement_begin_β:
                                                                                        jmp   n594_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n590_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx885_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n591_call_α
.Lx885_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n591_call_α:
                        sub              rsp, 16
                        mov              edi, 0                                         # idx
                        mov              rsi, qword ptr [rsp + 16]                      # lit_integer
                        mov              rdx, qword ptr [rsp + 24]                      # v
                        call             rt_arg_stage@PLT
                        sub              rsp, 96
                        mov              rax, qword ptr [1879052304]                    # n
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052320]                    # s
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 56], rax
                        mov              rax, qword ptr [1879052368]                    # i
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [1879052376]
                        mov              qword ptr [rsp + 72], rax
                        mov              rax, qword ptr [1879052288]                    # roman
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 88], rax
                        mov              rdi, qword ptr [rip + .Lx887_0]                # name
                        mov              esi, 5                                         # np
                        mov              edx, 1                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx887_5
                        lea              r10, [rip + g_call_args]
                        mov              rax, qword ptr [r10 + 0]
                        mov              qword ptr [1879052304], rax                    # n
                        mov              rax, qword ptr [r10 + 8]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx887_6]
                        lea              rdx, [rip + .Lx887_7]
                                                                                        jmp   rax
.Lx887_6:
                        mov              rdi, qword ptr [1879052288]                    # roman
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax                    # i
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax                    # r
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax                    # v
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax                    # s
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax                    # n
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx887_2
.Lx887_7:
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax                    # roman
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax                    # i
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax                    # r
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax                    # v
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax                    # s
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax                    # n
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx887_2
.Lx887_5:
                        add              rsp, 96
.Lx887_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx887_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n589_statement_begin_β
.Lx887_240:
                                                                                        jmp   n592_assign_α
n591_call_β:
                                                                                        jmp   n589_statement_begin_β
.Lx887_0:
                        .quad            .Lx887_0_s
.Lx887_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n592_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx888_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n593_statement_end_α
.Lx888_0:
                        .quad            .Lx888_0_s
.Lx888_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n593_statement_end_α:
                        add              rsp, 32
                                                                                        jmp   n594_statement_begin_α
#=======================================================================================================================
#         OUTPUT = roman(9)
#-----------------------------------------------------------------------------------------------------------------------
n594_statement_begin_α:
                                                                                        jmp   n595_lit_integer_α
n594_statement_begin_β:
                                                                                        jmp   n599_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n595_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx893_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n596_call_α
.Lx893_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n596_call_α:
                        sub              rsp, 16
                        mov              edi, 0                                         # idx
                        mov              rsi, qword ptr [rsp + 16]                      # lit_integer
                        mov              rdx, qword ptr [rsp + 24]                      # v
                        call             rt_arg_stage@PLT
                        sub              rsp, 96
                        mov              rax, qword ptr [1879052304]                    # n
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052320]                    # s
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 56], rax
                        mov              rax, qword ptr [1879052368]                    # i
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [1879052376]
                        mov              qword ptr [rsp + 72], rax
                        mov              rax, qword ptr [1879052288]                    # roman
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 88], rax
                        mov              rdi, qword ptr [rip + .Lx895_0]                # name
                        mov              esi, 5                                         # np
                        mov              edx, 1                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx895_5
                        lea              r10, [rip + g_call_args]
                        mov              rax, qword ptr [r10 + 0]
                        mov              qword ptr [1879052304], rax                    # n
                        mov              rax, qword ptr [r10 + 8]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx895_6]
                        lea              rdx, [rip + .Lx895_7]
                                                                                        jmp   rax
.Lx895_6:
                        mov              rdi, qword ptr [1879052288]                    # roman
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax                    # i
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax                    # r
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax                    # v
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax                    # s
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax                    # n
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx895_2
.Lx895_7:
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax                    # roman
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax                    # i
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax                    # r
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax                    # v
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax                    # s
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax                    # n
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx895_2
.Lx895_5:
                        add              rsp, 96
.Lx895_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx895_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n594_statement_begin_β
.Lx895_240:
                                                                                        jmp   n597_assign_α
n596_call_β:
                                                                                        jmp   n594_statement_begin_β
.Lx895_0:
                        .quad            .Lx895_0_s
.Lx895_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n597_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx896_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n598_statement_end_α
.Lx896_0:
                        .quad            .Lx896_0_s
.Lx896_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n598_statement_end_α:
                        add              rsp, 32
                                                                                        jmp   n599_statement_begin_α
#=======================================================================================================================
#         OUTPUT = roman(42)
#-----------------------------------------------------------------------------------------------------------------------
n599_statement_begin_α:
                                                                                        jmp   n600_lit_integer_α
n599_statement_begin_β:
                                                                                        jmp   n604_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n600_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx901_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n601_call_α
.Lx901_0:
                        .quad            42
#-----------------------------------------------------------------------------------------------------------------------
n601_call_α:
                        sub              rsp, 16
                        mov              edi, 0                                         # idx
                        mov              rsi, qword ptr [rsp + 16]                      # lit_integer
                        mov              rdx, qword ptr [rsp + 24]                      # v
                        call             rt_arg_stage@PLT
                        sub              rsp, 96
                        mov              rax, qword ptr [1879052304]                    # n
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052320]                    # s
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 56], rax
                        mov              rax, qword ptr [1879052368]                    # i
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [1879052376]
                        mov              qword ptr [rsp + 72], rax
                        mov              rax, qword ptr [1879052288]                    # roman
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 88], rax
                        mov              rdi, qword ptr [rip + .Lx903_0]                # name
                        mov              esi, 5                                         # np
                        mov              edx, 1                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx903_5
                        lea              r10, [rip + g_call_args]
                        mov              rax, qword ptr [r10 + 0]
                        mov              qword ptr [1879052304], rax                    # n
                        mov              rax, qword ptr [r10 + 8]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx903_6]
                        lea              rdx, [rip + .Lx903_7]
                                                                                        jmp   rax
.Lx903_6:
                        mov              rdi, qword ptr [1879052288]                    # roman
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax                    # i
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax                    # r
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax                    # v
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax                    # s
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax                    # n
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx903_2
.Lx903_7:
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax                    # roman
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax                    # i
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax                    # r
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax                    # v
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax                    # s
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax                    # n
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx903_2
.Lx903_5:
                        add              rsp, 96
.Lx903_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx903_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n599_statement_begin_β
.Lx903_240:
                                                                                        jmp   n602_assign_α
n601_call_β:
                                                                                        jmp   n599_statement_begin_β
.Lx903_0:
                        .quad            .Lx903_0_s
.Lx903_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n602_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx904_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n603_statement_end_α
.Lx904_0:
                        .quad            .Lx904_0_s
.Lx904_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n603_statement_end_α:
                        add              rsp, 32
                                                                                        jmp   n604_statement_begin_α
#=======================================================================================================================
#         OUTPUT = roman(1999)
#-----------------------------------------------------------------------------------------------------------------------
n604_statement_begin_α:
                                                                                        jmp   n605_lit_integer_α
n604_statement_begin_β:
                                                                                        jmp   n609_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n605_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx909_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n606_call_α
.Lx909_0:
                        .quad            1999
#-----------------------------------------------------------------------------------------------------------------------
n606_call_α:
                        sub              rsp, 16
                        mov              edi, 0                                         # idx
                        mov              rsi, qword ptr [rsp + 16]                      # lit_integer
                        mov              rdx, qword ptr [rsp + 24]                      # v
                        call             rt_arg_stage@PLT
                        sub              rsp, 96
                        mov              rax, qword ptr [1879052304]                    # n
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052320]                    # s
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 56], rax
                        mov              rax, qword ptr [1879052368]                    # i
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [1879052376]
                        mov              qword ptr [rsp + 72], rax
                        mov              rax, qword ptr [1879052288]                    # roman
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 88], rax
                        mov              rdi, qword ptr [rip + .Lx911_0]                # name
                        mov              esi, 5                                         # np
                        mov              edx, 1                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx911_5
                        lea              r10, [rip + g_call_args]
                        mov              rax, qword ptr [r10 + 0]
                        mov              qword ptr [1879052304], rax                    # n
                        mov              rax, qword ptr [r10 + 8]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx911_6]
                        lea              rdx, [rip + .Lx911_7]
                                                                                        jmp   rax
.Lx911_6:
                        mov              rdi, qword ptr [1879052288]                    # roman
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax                    # i
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax                    # r
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax                    # v
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax                    # s
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax                    # n
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx911_2
.Lx911_7:
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax                    # roman
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax                    # i
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax                    # r
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax                    # v
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax                    # s
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax                    # n
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx911_2
.Lx911_5:
                        add              rsp, 96
.Lx911_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx911_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n604_statement_begin_β
.Lx911_240:
                                                                                        jmp   n607_assign_α
n606_call_β:
                                                                                        jmp   n604_statement_begin_β
.Lx911_0:
                        .quad            .Lx911_0_s
.Lx911_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n607_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx912_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n608_statement_end_α
.Lx912_0:
                        .quad            .Lx912_0_s
.Lx912_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n608_statement_end_α:
                        add              rsp, 32
                                                                                        jmp   n609_statement_begin_α
#=======================================================================================================================
#         OUTPUT = roman(2024)
#-----------------------------------------------------------------------------------------------------------------------
n609_statement_begin_α:
                                                                                        jmp   n610_lit_integer_α
n609_statement_begin_β:
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n610_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx917_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n611_call_α
.Lx917_0:
                        .quad            2024
#-----------------------------------------------------------------------------------------------------------------------
n611_call_α:
                        sub              rsp, 16
                        mov              edi, 0                                         # idx
                        mov              rsi, qword ptr [rsp + 16]                      # lit_integer
                        mov              rdx, qword ptr [rsp + 24]                      # v
                        call             rt_arg_stage@PLT
                        sub              rsp, 96
                        mov              rax, qword ptr [1879052304]                    # n
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052320]                    # s
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 56], rax
                        mov              rax, qword ptr [1879052368]                    # i
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [1879052376]
                        mov              qword ptr [rsp + 72], rax
                        mov              rax, qword ptr [1879052288]                    # roman
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 88], rax
                        mov              rdi, qword ptr [rip + .Lx919_0]                # name
                        mov              esi, 5                                         # np
                        mov              edx, 1                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx919_5
                        lea              r10, [rip + g_call_args]
                        mov              rax, qword ptr [r10 + 0]
                        mov              qword ptr [1879052304], rax                    # n
                        mov              rax, qword ptr [r10 + 8]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx919_6]
                        lea              rdx, [rip + .Lx919_7]
                                                                                        jmp   rax
.Lx919_6:
                        mov              rdi, qword ptr [1879052288]                    # roman
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax                    # i
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax                    # r
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax                    # v
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax                    # s
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax                    # n
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx919_2
.Lx919_7:
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax                    # roman
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax                    # i
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax                    # r
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax                    # v
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax                    # s
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax                    # n
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx919_2
.Lx919_5:
                        add              rsp, 96
.Lx919_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx919_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n609_statement_begin_β
.Lx919_240:
                                                                                        jmp   n612_assign_α
n611_call_β:
                                                                                        jmp   n609_statement_begin_β
.Lx919_0:
                        .quad            .Lx919_0_s
.Lx919_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n612_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx920_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n613_statement_end_α
.Lx920_0:
                        .quad            .Lx920_0_s
.Lx920_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n613_statement_end_α:
                        add              rsp, 32
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n614_goto_α:
                                                                                        jmp   n615_statement_begin_α
n614_goto_β:
                                                                                        jmp   main_ω
#=======================================================================================================================
# roman   s = ''
#-----------------------------------------------------------------------------------------------------------------------
n615_statement_begin_α:
                                                                                        jmp   n616_lit_string_α
n615_statement_begin_β:
                                                                                        jmp   n619_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n616_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx926_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n617_assign_α
.Lx926_0:
                        .quad            .Lx926_0_s
.Lx926_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n617_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052320], rax                    # s
                        mov              qword ptr [1879052328], rdx
                                                                                        jmp   n618_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n618_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n619_statement_begin_α
#=======================================================================================================================
#         v = ARRAY(13)
#-----------------------------------------------------------------------------------------------------------------------
n619_statement_begin_α:
                                                                                        jmp   n620_lit_integer_α
n619_statement_begin_β:
                                                                                        jmp   n624_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n620_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx932_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n621_call_α
.Lx932_0:
                        .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n621_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd934:            .string          "ARRAY"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd934]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx933_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n619_statement_begin_β
.Lx933_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n622_assign_α
n621_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n619_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n622_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052336], rax                    # v
                        mov              qword ptr [1879052344], rdx
                                                                                        jmp   n623_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n623_statement_end_α:
                        add              rsp, 32
                                                                                        jmp   n624_statement_begin_α
#=======================================================================================================================
#         v<1> = 1000
#-----------------------------------------------------------------------------------------------------------------------
n624_statement_begin_α:
                                                                                        jmp   n625_var_α
n624_statement_begin_β:
                                                                                        jmp   n631_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n625_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n626_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n626_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx941_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n627_subscript_α
n626_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n624_statement_begin_β
.Lx941_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n627_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx942_240
                        add              rsp, 16
                                                                                        jmp   n626_lit_integer_β
.Lx942_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n628_lit_integer_α
n627_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n626_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n628_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx943_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n629_assign_var_α
n628_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n627_subscript_β
.Lx943_0:
                        .quad            1000
#-----------------------------------------------------------------------------------------------------------------------
n629_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx944_240
                        add              rsp, 16
                                                                                        jmp   n628_lit_integer_β
.Lx944_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n630_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n630_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n631_statement_begin_α
#=======================================================================================================================
#         v<2> = 900
#-----------------------------------------------------------------------------------------------------------------------
n631_statement_begin_α:
                                                                                        jmp   n632_var_α
n631_statement_begin_β:
                                                                                        jmp   n638_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n632_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n633_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n633_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx950_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n634_subscript_α
n633_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n631_statement_begin_β
.Lx950_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n634_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx951_240
                        add              rsp, 16
                                                                                        jmp   n633_lit_integer_β
.Lx951_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n635_lit_integer_α
n634_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n633_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n635_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx952_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n636_assign_var_α
n635_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n634_subscript_β
.Lx952_0:
                        .quad            900
#-----------------------------------------------------------------------------------------------------------------------
n636_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx953_240
                        add              rsp, 16
                                                                                        jmp   n635_lit_integer_β
.Lx953_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n637_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n637_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n638_statement_begin_α
#=======================================================================================================================
#         v<3> = 500
#-----------------------------------------------------------------------------------------------------------------------
n638_statement_begin_α:
                                                                                        jmp   n639_var_α
n638_statement_begin_β:
                                                                                        jmp   n645_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n639_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n640_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n640_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx959_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n641_subscript_α
n640_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n638_statement_begin_β
.Lx959_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n641_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx960_240
                        add              rsp, 16
                                                                                        jmp   n640_lit_integer_β
.Lx960_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n642_lit_integer_α
n641_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n640_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n642_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx961_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n643_assign_var_α
n642_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n641_subscript_β
.Lx961_0:
                        .quad            500
#-----------------------------------------------------------------------------------------------------------------------
n643_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx962_240
                        add              rsp, 16
                                                                                        jmp   n642_lit_integer_β
.Lx962_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n644_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n644_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n645_statement_begin_α
#=======================================================================================================================
#         v<4> = 400
#-----------------------------------------------------------------------------------------------------------------------
n645_statement_begin_α:
                                                                                        jmp   n646_var_α
n645_statement_begin_β:
                                                                                        jmp   n652_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n646_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n647_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n647_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx968_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n648_subscript_α
n647_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n645_statement_begin_β
.Lx968_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n648_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx969_240
                        add              rsp, 16
                                                                                        jmp   n647_lit_integer_β
.Lx969_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n649_lit_integer_α
n648_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n647_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n649_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx970_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n650_assign_var_α
n649_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n648_subscript_β
.Lx970_0:
                        .quad            400
#-----------------------------------------------------------------------------------------------------------------------
n650_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx971_240
                        add              rsp, 16
                                                                                        jmp   n649_lit_integer_β
.Lx971_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n651_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n651_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n652_statement_begin_α
#=======================================================================================================================
#         v<5> = 100
#-----------------------------------------------------------------------------------------------------------------------
n652_statement_begin_α:
                                                                                        jmp   n653_var_α
n652_statement_begin_β:
                                                                                        jmp   n659_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n653_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n654_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n654_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx977_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n655_subscript_α
n654_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n652_statement_begin_β
.Lx977_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n655_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx978_240
                        add              rsp, 16
                                                                                        jmp   n654_lit_integer_β
.Lx978_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n656_lit_integer_α
n655_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n654_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n656_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx979_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n657_assign_var_α
n656_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n655_subscript_β
.Lx979_0:
                        .quad            100
#-----------------------------------------------------------------------------------------------------------------------
n657_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx980_240
                        add              rsp, 16
                                                                                        jmp   n656_lit_integer_β
.Lx980_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n658_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n658_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n659_statement_begin_α
#=======================================================================================================================
#         v<6> = 90
#-----------------------------------------------------------------------------------------------------------------------
n659_statement_begin_α:
                                                                                        jmp   n660_var_α
n659_statement_begin_β:
                                                                                        jmp   n666_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n660_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n661_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n661_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx986_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n662_subscript_α
n661_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n659_statement_begin_β
.Lx986_0:
                        .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n662_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx987_240
                        add              rsp, 16
                                                                                        jmp   n661_lit_integer_β
.Lx987_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n663_lit_integer_α
n662_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n661_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n663_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx988_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n664_assign_var_α
n663_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n662_subscript_β
.Lx988_0:
                        .quad            90
#-----------------------------------------------------------------------------------------------------------------------
n664_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx989_240
                        add              rsp, 16
                                                                                        jmp   n663_lit_integer_β
.Lx989_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n665_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n665_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n666_statement_begin_α
#=======================================================================================================================
#         v<7> = 50
#-----------------------------------------------------------------------------------------------------------------------
n666_statement_begin_α:
                                                                                        jmp   n667_var_α
n666_statement_begin_β:
                                                                                        jmp   n673_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n667_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n668_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n668_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx995_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n669_subscript_α
n668_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n666_statement_begin_β
.Lx995_0:
                        .quad            7
#-----------------------------------------------------------------------------------------------------------------------
n669_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx996_240
                        add              rsp, 16
                                                                                        jmp   n668_lit_integer_β
.Lx996_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n670_lit_integer_α
n669_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n668_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n670_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx997_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n671_assign_var_α
n670_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n669_subscript_β
.Lx997_0:
                        .quad            50
#-----------------------------------------------------------------------------------------------------------------------
n671_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx998_240
                        add              rsp, 16
                                                                                        jmp   n670_lit_integer_β
.Lx998_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n672_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n672_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n673_statement_begin_α
#=======================================================================================================================
#         v<8> = 40
#-----------------------------------------------------------------------------------------------------------------------
n673_statement_begin_α:
                                                                                        jmp   n674_var_α
n673_statement_begin_β:
                                                                                        jmp   n680_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n674_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n675_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n675_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1004_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n676_subscript_α
n675_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n673_statement_begin_β
.Lx1004_0:
                        .quad            8
#-----------------------------------------------------------------------------------------------------------------------
n676_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1005_240
                        add              rsp, 16
                                                                                        jmp   n675_lit_integer_β
.Lx1005_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n677_lit_integer_α
n676_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n675_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n677_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1006_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n678_assign_var_α
n677_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n676_subscript_β
.Lx1006_0:
                        .quad            40
#-----------------------------------------------------------------------------------------------------------------------
n678_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1007_240
                        add              rsp, 16
                                                                                        jmp   n677_lit_integer_β
.Lx1007_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n679_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n679_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n680_statement_begin_α
#=======================================================================================================================
#         v<9> = 10
#-----------------------------------------------------------------------------------------------------------------------
n680_statement_begin_α:
                                                                                        jmp   n681_var_α
n680_statement_begin_β:
                                                                                        jmp   n687_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n681_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n682_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n682_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1013_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n683_subscript_α
n682_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n680_statement_begin_β
.Lx1013_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n683_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1014_240
                        add              rsp, 16
                                                                                        jmp   n682_lit_integer_β
.Lx1014_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n684_lit_integer_α
n683_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n682_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n684_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1015_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n685_assign_var_α
n684_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n683_subscript_β
.Lx1015_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n685_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1016_240
                        add              rsp, 16
                                                                                        jmp   n684_lit_integer_β
.Lx1016_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n686_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n686_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n687_statement_begin_α
#=======================================================================================================================
#         v<10> = 9
#-----------------------------------------------------------------------------------------------------------------------
n687_statement_begin_α:
                                                                                        jmp   n688_var_α
n687_statement_begin_β:
                                                                                        jmp   n694_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n688_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n689_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n689_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1022_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n690_subscript_α
n689_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n687_statement_begin_β
.Lx1022_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n690_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1023_240
                        add              rsp, 16
                                                                                        jmp   n689_lit_integer_β
.Lx1023_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n691_lit_integer_α
n690_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n689_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n691_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1024_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n692_assign_var_α
n691_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n690_subscript_β
.Lx1024_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n692_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1025_240
                        add              rsp, 16
                                                                                        jmp   n691_lit_integer_β
.Lx1025_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n693_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n693_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n694_statement_begin_α
#=======================================================================================================================
#         v<11> = 5
#-----------------------------------------------------------------------------------------------------------------------
n694_statement_begin_α:
                                                                                        jmp   n695_var_α
n694_statement_begin_β:
                                                                                        jmp   n701_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n695_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n696_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n696_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1031_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n697_subscript_α
n696_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n694_statement_begin_β
.Lx1031_0:
                        .quad            11
#-----------------------------------------------------------------------------------------------------------------------
n697_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1032_240
                        add              rsp, 16
                                                                                        jmp   n696_lit_integer_β
.Lx1032_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n698_lit_integer_α
n697_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n696_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n698_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1033_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n699_assign_var_α
n698_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n697_subscript_β
.Lx1033_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n699_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1034_240
                        add              rsp, 16
                                                                                        jmp   n698_lit_integer_β
.Lx1034_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n700_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n700_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n701_statement_begin_α
#=======================================================================================================================
#         v<12> = 4
#-----------------------------------------------------------------------------------------------------------------------
n701_statement_begin_α:
                                                                                        jmp   n702_var_α
n701_statement_begin_β:
                                                                                        jmp   n708_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n702_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n703_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n703_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1040_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n704_subscript_α
n703_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n701_statement_begin_β
.Lx1040_0:
                        .quad            12
#-----------------------------------------------------------------------------------------------------------------------
n704_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1041_240
                        add              rsp, 16
                                                                                        jmp   n703_lit_integer_β
.Lx1041_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n705_lit_integer_α
n704_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n703_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n705_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1042_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n706_assign_var_α
n705_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n704_subscript_β
.Lx1042_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n706_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1043_240
                        add              rsp, 16
                                                                                        jmp   n705_lit_integer_β
.Lx1043_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n707_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n707_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n708_statement_begin_α
#=======================================================================================================================
#         v<13> = 1
#-----------------------------------------------------------------------------------------------------------------------
n708_statement_begin_α:
                                                                                        jmp   n709_var_α
n708_statement_begin_β:
                                                                                        jmp   n715_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n709_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n710_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n710_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1049_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n711_subscript_α
n710_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n708_statement_begin_β
.Lx1049_0:
                        .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n711_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1050_240
                        add              rsp, 16
                                                                                        jmp   n710_lit_integer_β
.Lx1050_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n712_lit_integer_α
n711_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n710_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n712_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1051_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n713_assign_var_α
n712_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n711_subscript_β
.Lx1051_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n713_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1052_240
                        add              rsp, 16
                                                                                        jmp   n712_lit_integer_β
.Lx1052_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n714_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n714_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n715_statement_begin_α
#=======================================================================================================================
#         r = ARRAY(13)
#-----------------------------------------------------------------------------------------------------------------------
n715_statement_begin_α:
                                                                                        jmp   n716_lit_integer_α
n715_statement_begin_β:
                                                                                        jmp   n720_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n716_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1057_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n717_call_α
.Lx1057_0:
                        .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n717_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd1059:           .string          "ARRAY"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd1059]                      # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx1058_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n715_statement_begin_β
.Lx1058_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n718_assign_α
n717_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n715_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n718_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052352], rax                    # r
                        mov              qword ptr [1879052360], rdx
                                                                                        jmp   n719_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n719_statement_end_α:
                        add              rsp, 32
                                                                                        jmp   n720_statement_begin_α
#=======================================================================================================================
#         r<1> = 'M'
#-----------------------------------------------------------------------------------------------------------------------
n720_statement_begin_α:
                                                                                        jmp   n721_var_α
n720_statement_begin_β:
                                                                                        jmp   n727_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n721_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n722_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n722_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1066_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n723_subscript_α
n722_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n720_statement_begin_β
.Lx1066_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n723_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1067_240
                        add              rsp, 16
                                                                                        jmp   n722_lit_integer_β
.Lx1067_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n724_lit_string_α
n723_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n722_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n724_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx1068_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n725_assign_var_α
n724_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n723_subscript_β
.Lx1068_0:
                        .quad            .Lx1068_0_s
.Lx1068_0_s:
                        .string          "M"
#-----------------------------------------------------------------------------------------------------------------------
n725_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1069_240
                        add              rsp, 16
                                                                                        jmp   n724_lit_string_β
.Lx1069_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n726_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n726_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n727_statement_begin_α
#=======================================================================================================================
#         r<2> = 'CM'
#-----------------------------------------------------------------------------------------------------------------------
n727_statement_begin_α:
                                                                                        jmp   n728_var_α
n727_statement_begin_β:
                                                                                        jmp   n734_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n728_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n729_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n729_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1075_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n730_subscript_α
n729_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n727_statement_begin_β
.Lx1075_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n730_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1076_240
                        add              rsp, 16
                                                                                        jmp   n729_lit_integer_β
.Lx1076_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n731_lit_string_α
n730_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n729_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n731_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx1077_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n732_assign_var_α
n731_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n730_subscript_β
.Lx1077_0:
                        .quad            .Lx1077_0_s
.Lx1077_0_s:
                        .string          "CM"
#-----------------------------------------------------------------------------------------------------------------------
n732_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1078_240
                        add              rsp, 16
                                                                                        jmp   n731_lit_string_β
.Lx1078_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n733_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n733_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n734_statement_begin_α
#=======================================================================================================================
#         r<3> = 'D'
#-----------------------------------------------------------------------------------------------------------------------
n734_statement_begin_α:
                                                                                        jmp   n735_var_α
n734_statement_begin_β:
                                                                                        jmp   n741_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n735_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n736_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n736_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1084_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n737_subscript_α
n736_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n734_statement_begin_β
.Lx1084_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n737_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1085_240
                        add              rsp, 16
                                                                                        jmp   n736_lit_integer_β
.Lx1085_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n738_lit_string_α
n737_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n736_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n738_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx1086_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n739_assign_var_α
n738_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n737_subscript_β
.Lx1086_0:
                        .quad            .Lx1086_0_s
.Lx1086_0_s:
                        .string          "D"
#-----------------------------------------------------------------------------------------------------------------------
n739_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1087_240
                        add              rsp, 16
                                                                                        jmp   n738_lit_string_β
.Lx1087_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n740_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n740_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n741_statement_begin_α
#=======================================================================================================================
#         r<4> = 'CD'
#-----------------------------------------------------------------------------------------------------------------------
n741_statement_begin_α:
                                                                                        jmp   n742_var_α
n741_statement_begin_β:
                                                                                        jmp   n748_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n742_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n743_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n743_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1093_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n744_subscript_α
n743_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n741_statement_begin_β
.Lx1093_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n744_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1094_240
                        add              rsp, 16
                                                                                        jmp   n743_lit_integer_β
.Lx1094_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n745_lit_string_α
n744_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n743_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n745_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx1095_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n746_assign_var_α
n745_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n744_subscript_β
.Lx1095_0:
                        .quad            .Lx1095_0_s
.Lx1095_0_s:
                        .string          "CD"
#-----------------------------------------------------------------------------------------------------------------------
n746_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1096_240
                        add              rsp, 16
                                                                                        jmp   n745_lit_string_β
.Lx1096_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n747_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n747_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n748_statement_begin_α
#=======================================================================================================================
#         r<5> = 'C'
#-----------------------------------------------------------------------------------------------------------------------
n748_statement_begin_α:
                                                                                        jmp   n749_var_α
n748_statement_begin_β:
                                                                                        jmp   n755_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n749_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n750_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n750_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1102_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n751_subscript_α
n750_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n748_statement_begin_β
.Lx1102_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n751_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1103_240
                        add              rsp, 16
                                                                                        jmp   n750_lit_integer_β
.Lx1103_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n752_lit_string_α
n751_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n750_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n752_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx1104_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n753_assign_var_α
n752_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n751_subscript_β
.Lx1104_0:
                        .quad            .Lx1104_0_s
.Lx1104_0_s:
                        .string          "C"
#-----------------------------------------------------------------------------------------------------------------------
n753_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1105_240
                        add              rsp, 16
                                                                                        jmp   n752_lit_string_β
.Lx1105_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n754_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n754_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n755_statement_begin_α
#=======================================================================================================================
#         r<6> = 'XC'
#-----------------------------------------------------------------------------------------------------------------------
n755_statement_begin_α:
                                                                                        jmp   n756_var_α
n755_statement_begin_β:
                                                                                        jmp   n762_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n756_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n757_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n757_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1111_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n758_subscript_α
n757_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n755_statement_begin_β
.Lx1111_0:
                        .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n758_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1112_240
                        add              rsp, 16
                                                                                        jmp   n757_lit_integer_β
.Lx1112_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n759_lit_string_α
n758_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n757_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n759_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx1113_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n760_assign_var_α
n759_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n758_subscript_β
.Lx1113_0:
                        .quad            .Lx1113_0_s
.Lx1113_0_s:
                        .string          "XC"
#-----------------------------------------------------------------------------------------------------------------------
n760_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1114_240
                        add              rsp, 16
                                                                                        jmp   n759_lit_string_β
.Lx1114_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n761_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n761_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n762_statement_begin_α
#=======================================================================================================================
#         r<7> = 'L'
#-----------------------------------------------------------------------------------------------------------------------
n762_statement_begin_α:
                                                                                        jmp   n763_var_α
n762_statement_begin_β:
                                                                                        jmp   n769_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n763_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n764_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n764_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1120_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n765_subscript_α
n764_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n762_statement_begin_β
.Lx1120_0:
                        .quad            7
#-----------------------------------------------------------------------------------------------------------------------
n765_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1121_240
                        add              rsp, 16
                                                                                        jmp   n764_lit_integer_β
.Lx1121_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n766_lit_string_α
n765_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n764_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n766_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx1122_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n767_assign_var_α
n766_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n765_subscript_β
.Lx1122_0:
                        .quad            .Lx1122_0_s
.Lx1122_0_s:
                        .string          "L"
#-----------------------------------------------------------------------------------------------------------------------
n767_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1123_240
                        add              rsp, 16
                                                                                        jmp   n766_lit_string_β
.Lx1123_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n768_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n768_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n769_statement_begin_α
#=======================================================================================================================
#         r<8> = 'XL'
#-----------------------------------------------------------------------------------------------------------------------
n769_statement_begin_α:
                                                                                        jmp   n770_var_α
n769_statement_begin_β:
                                                                                        jmp   n776_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n770_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n771_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n771_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1129_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n772_subscript_α
n771_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n769_statement_begin_β
.Lx1129_0:
                        .quad            8
#-----------------------------------------------------------------------------------------------------------------------
n772_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1130_240
                        add              rsp, 16
                                                                                        jmp   n771_lit_integer_β
.Lx1130_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n773_lit_string_α
n772_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n771_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n773_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx1131_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n774_assign_var_α
n773_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n772_subscript_β
.Lx1131_0:
                        .quad            .Lx1131_0_s
.Lx1131_0_s:
                        .string          "XL"
#-----------------------------------------------------------------------------------------------------------------------
n774_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1132_240
                        add              rsp, 16
                                                                                        jmp   n773_lit_string_β
.Lx1132_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n775_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n775_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n776_statement_begin_α
#=======================================================================================================================
#         r<9> = 'X'
#-----------------------------------------------------------------------------------------------------------------------
n776_statement_begin_α:
                                                                                        jmp   n777_var_α
n776_statement_begin_β:
                                                                                        jmp   n783_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n777_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n778_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n778_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1138_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n779_subscript_α
n778_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n776_statement_begin_β
.Lx1138_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n779_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1139_240
                        add              rsp, 16
                                                                                        jmp   n778_lit_integer_β
.Lx1139_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n780_lit_string_α
n779_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n778_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n780_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx1140_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n781_assign_var_α
n780_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n779_subscript_β
.Lx1140_0:
                        .quad            .Lx1140_0_s
.Lx1140_0_s:
                        .string          "X"
#-----------------------------------------------------------------------------------------------------------------------
n781_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1141_240
                        add              rsp, 16
                                                                                        jmp   n780_lit_string_β
.Lx1141_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n782_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n782_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n783_statement_begin_α
#=======================================================================================================================
#         r<10> = 'IX'
#-----------------------------------------------------------------------------------------------------------------------
n783_statement_begin_α:
                                                                                        jmp   n784_var_α
n783_statement_begin_β:
                                                                                        jmp   n790_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n784_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n785_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n785_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1147_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n786_subscript_α
n785_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n783_statement_begin_β
.Lx1147_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n786_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1148_240
                        add              rsp, 16
                                                                                        jmp   n785_lit_integer_β
.Lx1148_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n787_lit_string_α
n786_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n785_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n787_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx1149_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n788_assign_var_α
n787_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n786_subscript_β
.Lx1149_0:
                        .quad            .Lx1149_0_s
.Lx1149_0_s:
                        .string          "IX"
#-----------------------------------------------------------------------------------------------------------------------
n788_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1150_240
                        add              rsp, 16
                                                                                        jmp   n787_lit_string_β
.Lx1150_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n789_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n789_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n790_statement_begin_α
#=======================================================================================================================
#         r<11> = 'V'
#-----------------------------------------------------------------------------------------------------------------------
n790_statement_begin_α:
                                                                                        jmp   n791_var_α
n790_statement_begin_β:
                                                                                        jmp   n797_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n791_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n792_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n792_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1156_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n793_subscript_α
n792_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n790_statement_begin_β
.Lx1156_0:
                        .quad            11
#-----------------------------------------------------------------------------------------------------------------------
n793_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1157_240
                        add              rsp, 16
                                                                                        jmp   n792_lit_integer_β
.Lx1157_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n794_lit_string_α
n793_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n792_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n794_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx1158_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n795_assign_var_α
n794_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n793_subscript_β
.Lx1158_0:
                        .quad            .Lx1158_0_s
.Lx1158_0_s:
                        .string          "V"
#-----------------------------------------------------------------------------------------------------------------------
n795_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1159_240
                        add              rsp, 16
                                                                                        jmp   n794_lit_string_β
.Lx1159_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n796_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n796_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n797_statement_begin_α
#=======================================================================================================================
#         r<12> = 'IV'
#-----------------------------------------------------------------------------------------------------------------------
n797_statement_begin_α:
                                                                                        jmp   n798_var_α
n797_statement_begin_β:
                                                                                        jmp   n804_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n798_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n799_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n799_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1165_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n800_subscript_α
n799_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n797_statement_begin_β
.Lx1165_0:
                        .quad            12
#-----------------------------------------------------------------------------------------------------------------------
n800_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1166_240
                        add              rsp, 16
                                                                                        jmp   n799_lit_integer_β
.Lx1166_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n801_lit_string_α
n800_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n799_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n801_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx1167_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n802_assign_var_α
n801_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n800_subscript_β
.Lx1167_0:
                        .quad            .Lx1167_0_s
.Lx1167_0_s:
                        .string          "IV"
#-----------------------------------------------------------------------------------------------------------------------
n802_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1168_240
                        add              rsp, 16
                                                                                        jmp   n801_lit_string_β
.Lx1168_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n803_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n803_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n804_statement_begin_α
#=======================================================================================================================
#         r<13> = 'I'
#-----------------------------------------------------------------------------------------------------------------------
n804_statement_begin_α:
                                                                                        jmp   n805_var_α
n804_statement_begin_β:
                                                                                        jmp   n811_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n805_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n806_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n806_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1174_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n807_subscript_α
n806_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n804_statement_begin_β
.Lx1174_0:
                        .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n807_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1175_240
                        add              rsp, 16
                                                                                        jmp   n806_lit_integer_β
.Lx1175_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n808_lit_string_α
n807_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n806_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n808_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx1176_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n809_assign_var_α
n808_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n807_subscript_β
.Lx1176_0:
                        .quad            .Lx1176_0_s
.Lx1176_0_s:
                        .string          "I"
#-----------------------------------------------------------------------------------------------------------------------
n809_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1177_240
                        add              rsp, 16
                                                                                        jmp   n808_lit_string_β
.Lx1177_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n810_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n810_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n811_statement_begin_α
#=======================================================================================================================
#         i = 1
#-----------------------------------------------------------------------------------------------------------------------
n811_statement_begin_α:
                                                                                        jmp   n812_lit_integer_α
n811_statement_begin_β:
                                                                                        jmp   n815_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n812_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1182_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n813_assign_α
.Lx1182_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n813_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_integer
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052368], rax                    # i
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n814_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n814_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n815_statement_begin_α
#=======================================================================================================================
# RLOOP   GT(n, 0)                                                       :F(RDONE)
#-----------------------------------------------------------------------------------------------------------------------
n815_statement_begin_α:
                                                                                        jmp   n816_var_α
n815_statement_begin_β:
                                                                                        jmp   n856_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n816_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # n
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n817_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n817_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1189_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n818_coerce_numeric_α
n817_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n815_statement_begin_β
.Lx1189_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n818_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # var
                        cmp              eax, 5
                                                                                        je    .Lx1191_1
                        cmp              eax, 3
                                                                                        jne   .Lx1191_0
                        mov              eax, dword ptr [rsp + 16]                      # lit_integer
                        cmp              eax, 3
                                                                                        jne   .Lx1191_0
.Lx1191_1:
                        mov              rax, qword ptr [rsp + 32]                      # var
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # var
                        mov              qword ptr [rsp + 8], rax                       # result
                                                                                        jmp   n819_coerce_numeric_α
.Lx1191_0:
                        lea              rdi, [rsp + 32]                                # var
                        lea              rsi, [rsp + 16]                                # lit_integer
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 111                                       # codes
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n819_coerce_numeric_α
n818_coerce_numeric_β:
                        add              rsp, 16
                                                                                        jmp   n817_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n819_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # lit_integer
                        cmp              eax, 5
                                                                                        je    .Lx1193_1
                        cmp              eax, 3
                                                                                        jne   .Lx1193_0
                        mov              eax, dword ptr [rsp + 48]                      # var
                        cmp              eax, 3
                                                                                        jne   .Lx1193_0
.Lx1193_1:
                        mov              rax, qword ptr [rsp + 32]                      # lit_integer
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # lit_integer
                        mov              qword ptr [rsp + 8], rax                       # result
                                                                                        jmp   n820_cmp_test_α
.Lx1193_0:
                        lea              rdi, [rsp + 32]                                # lit_integer
                        lea              rsi, [rsp + 48]                                # var
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 112                                       # codes
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n820_cmp_test_α
n819_coerce_numeric_β:
                        add              rsp, 16
                                                                                        jmp   n818_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n820_cmp_test_α:
                        sub              rsp, 16
                        lea              rdi, [rsp + 32]                                # coerce_numeric
                        lea              rsi, [rsp + 16]                                # b
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jg    .Lx1195_240
                        add              rsp, 16
                                                                                        jmp   n819_coerce_numeric_β
.Lx1195_240:
                                                                                        jmp   n821_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n821_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n822_statement_begin_α
#=======================================================================================================================
#         GE(n, v<i>)                                                    :F(RNEXT)
#-----------------------------------------------------------------------------------------------------------------------
n822_statement_begin_α:
                                                                                        jmp   n823_var_α
n822_statement_begin_β:
                                                                                        jmp   n850_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n823_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # n
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n824_var_α
#-----------------------------------------------------------------------------------------------------------------------
n824_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n825_var_α
n824_var_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n822_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n825_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052368]                    # i
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n826_subscript_α
n825_var_β:
                        add              rsp, 16
                                                                                        jmp   n824_var_β
#-----------------------------------------------------------------------------------------------------------------------
n826_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # idx
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1203_240
                        add              rsp, 16
                                                                                        jmp   n825_var_β
.Lx1203_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n827_deref_α
n826_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n825_var_β
#-----------------------------------------------------------------------------------------------------------------------
n827_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # subscript
                        mov              rsi, qword ptr [rsp + 24]                      # d
                        call             rt_deref@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1204_240
                        add              rsp, 16
                                                                                        jmp   n826_subscript_β
.Lx1204_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n828_coerce_numeric_α
n827_deref_β:
                        add              rsp, 16
                                                                                        jmp   n826_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n828_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 80]                      # var
                        cmp              eax, 5
                                                                                        je    .Lx1206_1
                        cmp              eax, 3
                                                                                        jne   .Lx1206_0
                        mov              eax, dword ptr [rsp + 16]                      # deref
                        cmp              eax, 3
                                                                                        jne   .Lx1206_0
.Lx1206_1:
                        mov              rax, qword ptr [rsp + 80]                      # var
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 88]                      # var
                        mov              qword ptr [rsp + 8], rax                       # result
                                                                                        jmp   n829_coerce_numeric_α
.Lx1206_0:
                        lea              rdi, [rsp + 80]                                # var
                        lea              rsi, [rsp + 16]                                # deref
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 109                                       # codes
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n829_coerce_numeric_α
n828_coerce_numeric_β:
                        add              rsp, 16
                                                                                        jmp   n827_deref_β
#-----------------------------------------------------------------------------------------------------------------------
n829_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # deref
                        cmp              eax, 5
                                                                                        je    .Lx1208_1
                        cmp              eax, 3
                                                                                        jne   .Lx1208_0
                        mov              eax, dword ptr [rsp + 96]                      # var
                        cmp              eax, 3
                                                                                        jne   .Lx1208_0
.Lx1208_1:
                        mov              rax, qword ptr [rsp + 32]                      # deref
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # deref
                        mov              qword ptr [rsp + 8], rax                       # result
                                                                                        jmp   n830_cmp_test_α
.Lx1208_0:
                        lea              rdi, [rsp + 32]                                # deref
                        lea              rsi, [rsp + 96]                                # var
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 110                                       # codes
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n830_cmp_test_α
n829_coerce_numeric_β:
                        add              rsp, 16
                                                                                        jmp   n828_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n830_cmp_test_α:
                        sub              rsp, 16
                        lea              rdi, [rsp + 32]                                # coerce_numeric
                        lea              rsi, [rsp + 16]                                # b
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jns   .Lx1210_240
                        add              rsp, 16
                                                                                        jmp   n829_coerce_numeric_β
.Lx1210_240:
                                                                                        jmp   n831_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n831_statement_end_α:
                        add              rsp, 128
                                                                                        jmp   n832_statement_begin_α
#=======================================================================================================================
#         s = s r<i>
#-----------------------------------------------------------------------------------------------------------------------
n832_statement_begin_α:
                                                                                        jmp   n833_var_α
n832_statement_begin_β:
                                                                                        jmp   n841_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n833_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # s
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n834_var_α
#-----------------------------------------------------------------------------------------------------------------------
n834_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n835_var_α
n834_var_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n832_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n835_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052368]                    # i
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n836_subscript_α
n835_var_β:
                        add              rsp, 16
                                                                                        jmp   n834_var_β
#-----------------------------------------------------------------------------------------------------------------------
n836_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # idx
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1218_240
                        add              rsp, 16
                                                                                        jmp   n835_var_β
.Lx1218_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n837_deref_α
n836_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n835_var_β
#-----------------------------------------------------------------------------------------------------------------------
n837_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # subscript
                        mov              rsi, qword ptr [rsp + 24]                      # d
                        call             rt_deref@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1219_240
                        add              rsp, 16
                                                                                        jmp   n836_subscript_β
.Lx1219_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n838_binop_α
n837_deref_β:
                        add              rsp, 16
                                                                                        jmp   n836_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n838_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 80]                      # var
                        mov              rsi, qword ptr [rsp + 88]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # deref
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n839_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n839_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052320], rax                    # s
                        mov              qword ptr [1879052328], rdx
                                                                                        jmp   n840_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n840_statement_end_α:
                        add              rsp, 96
                                                                                        jmp   n841_statement_begin_α
#=======================================================================================================================
#         n = n - v<i>                                                   :(RLOOP)
#-----------------------------------------------------------------------------------------------------------------------
n841_statement_begin_α:
                                                                                        jmp   n842_var_α
n841_statement_begin_β:
                                                                                        jmp   n815_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n842_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # n
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n843_var_α
#-----------------------------------------------------------------------------------------------------------------------
n843_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n844_var_α
n843_var_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n841_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n844_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052368]                    # i
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n845_subscript_α
n844_var_β:
                        add              rsp, 16
                                                                                        jmp   n843_var_β
#-----------------------------------------------------------------------------------------------------------------------
n845_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # idx
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1229_240
                        add              rsp, 16
                                                                                        jmp   n844_var_β
.Lx1229_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n846_deref_α
n845_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n844_var_β
#-----------------------------------------------------------------------------------------------------------------------
n846_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # subscript
                        mov              rsi, qword ptr [rsp + 24]                      # d
                        call             rt_deref@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1230_240
                        add              rsp, 16
                                                                                        jmp   n845_subscript_β
.Lx1230_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n847_binop_α
n846_deref_β:
                        add              rsp, 16
                                                                                        jmp   n845_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n847_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 80]                      # var
                        mov              rsi, qword ptr [rsp + 88]
                        mov              rdx, qword ptr [rsp + 16]                      # deref
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_sub@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1231_240
                        add              rsp, 16
                                                                                        jmp   n846_deref_β
.Lx1231_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n848_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n848_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax                    # n
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   n849_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n849_statement_end_α:
                        add              rsp, 96
                                                                                        jmp   n815_statement_begin_α
#=======================================================================================================================
# RNEXT   i = i + 1                                                      :(RLOOP)
#-----------------------------------------------------------------------------------------------------------------------
n850_statement_begin_α:
                                                                                        jmp   n851_var_α
n850_statement_begin_β:
                                                                                        jmp   n815_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n851_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052368]                    # i
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n852_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n852_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1238_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n853_binop_α
n852_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n850_statement_begin_β
.Lx1238_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n853_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_add@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1239_240
                        add              rsp, 16
                                                                                        jmp   n852_lit_integer_β
.Lx1239_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n854_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n854_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052368], rax                    # i
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n855_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n855_statement_end_α:
                        add              rsp, 48
                                                                                        jmp   n815_statement_begin_α
#=======================================================================================================================
# RDONE   roman = s                                                      :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n856_statement_begin_α:
                                                                                        jmp   n857_var_α
n856_statement_begin_β:
                                                                                        jmp   n860_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n857_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # s
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n858_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n858_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax                    # roman
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n859_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n859_statement_end_α:
                                                                                        jmp   n860_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n860_save_restore_α:
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx1250_0
                        mov              cl, 0
                        mov              rax, qword ptr [rcx + -64]
                                                                                        jmp   rax
.Lx1250_0:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n861_goto_α:
                                                                                        jmp   n815_statement_begin_α
n861_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n862_goto_α:
                                                                                        jmp   n850_statement_begin_α
n862_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n863_goto_α:
                                                                                        jmp   n856_statement_begin_α
n863_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n864_goto_α:
                                                                                        jmp   n582_statement_begin_α
n864_goto_β:
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
                        .section         .data
                        .align           8
fn_cell$roman:
                        .quad            rt_ab_undef_fn_stub
                        .section         .text
                        .intel_syntax    noprefix
roman_act_α:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 200
                        mov              qword ptr [rbp + -16], rcx
                        mov              qword ptr [rbp + -24], rdx
                        lea              rax, [rbp + 8]
                        mov              qword ptr [rbp + -32], rax
                        mov              rax, qword ptr [1879048216]
                        mov              qword ptr [rbp + -40], rax
                        mov              qword ptr [1879048216], rbp
                        lea              rax, [rip + .Lx1255_1]
                        mov              qword ptr [rbp + -64], rax
                        mov              rdi, rbp
                        call             rt_ab_enter_env@PLT
                        mov              rax, qword ptr [1879052288]                    # roman
                        mov              qword ptr [rbp + -112], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rbp + -104], rax
                        mov              rax, qword ptr [1879052304]                    # n
                        mov              qword ptr [rbp + -128], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rbp + -120], rax
                        mov              qword ptr [rbp + -144], 0
                        mov              qword ptr [rbp + -136], 0
                        mov              qword ptr [rbp + -160], 0
                        mov              qword ptr [rbp + -152], 0
                        mov              qword ptr [rbp + -176], 0
                        mov              qword ptr [rbp + -168], 0
                        mov              qword ptr [rbp + -192], 0
                        mov              qword ptr [rbp + -184], 0
                        xor              eax, eax                                       # roman
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rax
                        xor              eax, eax                                       # n
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rax
                        movabs           rax, 139711988612492
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx1255_2
                        mov              rdi, qword ptr [rip + .Lx1255_0]
                        call             mon_emit_call_bin@PLT
.Lx1255_2:
                        mov              rax, qword ptr [rip + fn_cell$roman@GOTPCREL]
                                                                                        jmp   rax
                                                                                        jmp   roman_act_γ
.Lx1255_1:
                        movzx            r9, cl
                        cmp              r9d, 2
                                                                                        je    .Lx1255_3
                        mov              qword ptr [rbp + -88], rax
                        mov              qword ptr [rbp + -96], rdx
                        mov              rdi, rbp
                        mov              rsi, qword ptr [rbp + -88]
                        mov              rdx, qword ptr [rbp + -96]
                        xor              ecx, ecx
                        call             rt_ab_leave_env@PLT
                        mov              qword ptr [rbp + -88], rax
                        mov              qword ptr [rbp + -96], rdx
                                                                                        jmp   .Lx1255_4
.Lx1255_3:
                        mov              rdi, rbp
                        xor              esi, esi
                        xor              edx, edx
                        mov              ecx, 1
                        call             rt_ab_leave_env@PLT
                        mov              qword ptr [rbp + -88], rax
                        mov              qword ptr [rbp + -96], rdx
.Lx1255_4:
                        mov              rcx, qword ptr [rbp + -112]                    # roman
                        mov              qword ptr [1879052288], rcx
                        mov              rcx, qword ptr [rbp + -104]
                        mov              qword ptr [1879052296], rcx
                        mov              rcx, qword ptr [rbp + -128]                    # n
                        mov              qword ptr [1879052304], rcx
                        mov              rcx, qword ptr [rbp + -120]
                        mov              qword ptr [1879052312], rcx
                        mov              r10, qword ptr [rbp + -16]
                        mov              r11, qword ptr [rbp + -24]
                        mov              rcx, qword ptr [rbp + -40]
                        movabs           rax, 139711988612492
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx1255_5
                        mov              rdi, qword ptr [rip + .Lx1255_0]
                        mov              rsi, qword ptr [rbp + -88]
                        mov              rdx, qword ptr [rbp + -96]
                        call             mon_emit_return_bin@PLT
.Lx1255_5:
                        mov              rax, qword ptr [rbp + -88]
                        mov              rdx, qword ptr [rbp + -96]
                        mov              qword ptr [1879048216], rcx
                        cmp              r9d, 2
                                                                                        je    .Lx1255_6
                                                                                        jmp   r10
.Lx1255_6:
                                                                                        jmp   r11
roman_act_γ:
roman_act_ω:
.Lx1255_0:
                        .quad            .Lx1255_0_s
.Lx1255_0_s:
                        .string          "roman"
                        .section         .note.GNU-stack,"",@progbits

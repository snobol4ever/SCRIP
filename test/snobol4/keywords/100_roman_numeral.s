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
                                                                                        jmp   n8_statement_end_α
.Lx255_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n7_assign_α
n6_call_β:
                                                                                        jmp   n8_statement_end_α
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
                                                                                        jmp   n16_statement_begin_α
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
                                                                                        jmp   n23_statement_begin_α
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
                                                                                        jmp   n30_statement_begin_α
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
                                                                                        jmp   n37_statement_begin_α
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
                                                                                        jmp   n44_statement_begin_α
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
                                                                                        jmp   n51_statement_begin_α
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
                                                                                        jmp   n58_statement_begin_α
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
                                                                                        jmp   n65_statement_begin_α
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
                                                                                        jmp   n72_statement_begin_α
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
                                                                                        jmp   n79_statement_begin_α
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
                                                                                        jmp   n86_statement_begin_α
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
                                                                                        jmp   n93_statement_begin_α
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
                                                                                        jmp   n100_statement_begin_α
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
                                                                                        jmp   n104_statement_end_α
.Lx380_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n103_assign_α
n102_call_β:
                                                                                        jmp   n104_statement_end_α
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
                                                                                        jmp   n112_statement_begin_α
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
                                                                                        jmp   n119_statement_begin_α
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
                                                                                        jmp   n126_statement_begin_α
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
                                                                                        jmp   n133_statement_begin_α
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
                                                                                        jmp   n140_statement_begin_α
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
                                                                                        jmp   n147_statement_begin_α
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
                                                                                        jmp   n154_statement_begin_α
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
                                                                                        jmp   n161_statement_begin_α
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
                                                                                        jmp   n168_statement_begin_α
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
                                                                                        jmp   n175_statement_begin_α
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
                                                                                        jmp   n182_statement_begin_α
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
                                                                                        jmp   n189_statement_begin_α
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
                                                                                        jmp   n196_statement_begin_α
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
                                                                                        jmp   n241_statement_begin_α
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
                                                                                        jmp   n235_statement_begin_α
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
                                                                                        jmp   n226_statement_begin_α
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
n223_binop_β:
                        add              rsp, 16
                                                                                        jmp   n222_deref_β
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
                                                                                        jmp   n200_statement_begin_α
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
n232_binop_β:
                        add              rsp, 16
                                                                                        jmp   n231_deref_β
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
                                                                                        jmp   n200_statement_begin_α
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
n238_binop_β:
                        add              rsp, 16
                                                                                        jmp   n237_lit_integer_β
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
                        mov              rsp, rbp
                        pop              rbp
                        xor              edi, edi
                        call             exit@PLT
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__roman_ω:
                        mov              rsp, rbp
                        pop              rbp
                        mov              edi, 1
                        call             exit@PLT
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
                        mov              esi, 3088
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
                                                                                        jmp   n580_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n580_statement_end_α:
                                                                                        jmp   n581_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n581_statement_begin_α:
                                                                                        jmp   n582_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n582_statement_end_α:
                                                                                        jmp   n583_statement_begin_α
#=======================================================================================================================
#         OUTPUT = roman(1)
#-----------------------------------------------------------------------------------------------------------------------
n583_statement_begin_α:
                                                                                        jmp   n584_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n584_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx874_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n585_call_α
.Lx874_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n585_call_α:
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
                        mov              rdi, qword ptr [rip + .Lx876_0]                # name
                        mov              esi, 5                                         # np
                        mov              edx, 1                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx876_5
                        lea              r10, [rip + g_call_args]
                        mov              rax, qword ptr [r10 + 0]
                        mov              qword ptr [1879052304], rax                    # n
                        mov              rax, qword ptr [r10 + 8]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx876_6]
                        lea              rdx, [rip + .Lx876_7]
                                                                                        jmp   rax
.Lx876_6:
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
                                                                                        jmp   .Lx876_2
.Lx876_7:
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
                                                                                        jmp   .Lx876_2
.Lx876_5:
                        add              rsp, 96
.Lx876_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx876_240
                                                                                        jmp   n587_statement_end_α
.Lx876_240:
                                                                                        jmp   n586_assign_α
n585_call_β:
                                                                                        jmp   n587_statement_end_α
.Lx876_0:
                        .quad            .Lx876_0_s
.Lx876_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n586_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx877_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n587_statement_end_α
.Lx877_0:
                        .quad            .Lx877_0_s
.Lx877_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n587_statement_end_α:
                        add              rsp, 32
                                                                                        jmp   n588_statement_begin_α
#=======================================================================================================================
#         OUTPUT = roman(4)
#-----------------------------------------------------------------------------------------------------------------------
n588_statement_begin_α:
                                                                                        jmp   n589_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n589_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx882_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n590_call_α
.Lx882_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n590_call_α:
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
                        mov              rdi, qword ptr [rip + .Lx884_0]                # name
                        mov              esi, 5                                         # np
                        mov              edx, 1                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx884_5
                        lea              r10, [rip + g_call_args]
                        mov              rax, qword ptr [r10 + 0]
                        mov              qword ptr [1879052304], rax                    # n
                        mov              rax, qword ptr [r10 + 8]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx884_6]
                        lea              rdx, [rip + .Lx884_7]
                                                                                        jmp   rax
.Lx884_6:
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
                                                                                        jmp   .Lx884_2
.Lx884_7:
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
                                                                                        jmp   .Lx884_2
.Lx884_5:
                        add              rsp, 96
.Lx884_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx884_240
                                                                                        jmp   n592_statement_end_α
.Lx884_240:
                                                                                        jmp   n591_assign_α
n590_call_β:
                                                                                        jmp   n592_statement_end_α
.Lx884_0:
                        .quad            .Lx884_0_s
.Lx884_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n591_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx885_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n592_statement_end_α
.Lx885_0:
                        .quad            .Lx885_0_s
.Lx885_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n592_statement_end_α:
                        add              rsp, 32
                                                                                        jmp   n593_statement_begin_α
#=======================================================================================================================
#         OUTPUT = roman(9)
#-----------------------------------------------------------------------------------------------------------------------
n593_statement_begin_α:
                                                                                        jmp   n594_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n594_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx890_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n595_call_α
.Lx890_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n595_call_α:
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
                        mov              rdi, qword ptr [rip + .Lx892_0]                # name
                        mov              esi, 5                                         # np
                        mov              edx, 1                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx892_5
                        lea              r10, [rip + g_call_args]
                        mov              rax, qword ptr [r10 + 0]
                        mov              qword ptr [1879052304], rax                    # n
                        mov              rax, qword ptr [r10 + 8]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx892_6]
                        lea              rdx, [rip + .Lx892_7]
                                                                                        jmp   rax
.Lx892_6:
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
                                                                                        jmp   .Lx892_2
.Lx892_7:
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
                                                                                        jmp   .Lx892_2
.Lx892_5:
                        add              rsp, 96
.Lx892_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx892_240
                                                                                        jmp   n597_statement_end_α
.Lx892_240:
                                                                                        jmp   n596_assign_α
n595_call_β:
                                                                                        jmp   n597_statement_end_α
.Lx892_0:
                        .quad            .Lx892_0_s
.Lx892_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n596_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx893_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n597_statement_end_α
.Lx893_0:
                        .quad            .Lx893_0_s
.Lx893_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n597_statement_end_α:
                        add              rsp, 32
                                                                                        jmp   n598_statement_begin_α
#=======================================================================================================================
#         OUTPUT = roman(42)
#-----------------------------------------------------------------------------------------------------------------------
n598_statement_begin_α:
                                                                                        jmp   n599_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n599_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx898_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n600_call_α
.Lx898_0:
                        .quad            42
#-----------------------------------------------------------------------------------------------------------------------
n600_call_α:
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
                        mov              rdi, qword ptr [rip + .Lx900_0]                # name
                        mov              esi, 5                                         # np
                        mov              edx, 1                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx900_5
                        lea              r10, [rip + g_call_args]
                        mov              rax, qword ptr [r10 + 0]
                        mov              qword ptr [1879052304], rax                    # n
                        mov              rax, qword ptr [r10 + 8]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx900_6]
                        lea              rdx, [rip + .Lx900_7]
                                                                                        jmp   rax
.Lx900_6:
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
                                                                                        jmp   .Lx900_2
.Lx900_7:
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
                                                                                        jmp   .Lx900_2
.Lx900_5:
                        add              rsp, 96
.Lx900_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx900_240
                                                                                        jmp   n602_statement_end_α
.Lx900_240:
                                                                                        jmp   n601_assign_α
n600_call_β:
                                                                                        jmp   n602_statement_end_α
.Lx900_0:
                        .quad            .Lx900_0_s
.Lx900_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n601_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx901_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n602_statement_end_α
.Lx901_0:
                        .quad            .Lx901_0_s
.Lx901_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n602_statement_end_α:
                        add              rsp, 32
                                                                                        jmp   n603_statement_begin_α
#=======================================================================================================================
#         OUTPUT = roman(1999)
#-----------------------------------------------------------------------------------------------------------------------
n603_statement_begin_α:
                                                                                        jmp   n604_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n604_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx906_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n605_call_α
.Lx906_0:
                        .quad            1999
#-----------------------------------------------------------------------------------------------------------------------
n605_call_α:
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
                        mov              rdi, qword ptr [rip + .Lx908_0]                # name
                        mov              esi, 5                                         # np
                        mov              edx, 1                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx908_5
                        lea              r10, [rip + g_call_args]
                        mov              rax, qword ptr [r10 + 0]
                        mov              qword ptr [1879052304], rax                    # n
                        mov              rax, qword ptr [r10 + 8]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx908_6]
                        lea              rdx, [rip + .Lx908_7]
                                                                                        jmp   rax
.Lx908_6:
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
                                                                                        jmp   .Lx908_2
.Lx908_7:
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
                                                                                        jmp   .Lx908_2
.Lx908_5:
                        add              rsp, 96
.Lx908_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx908_240
                                                                                        jmp   n607_statement_end_α
.Lx908_240:
                                                                                        jmp   n606_assign_α
n605_call_β:
                                                                                        jmp   n607_statement_end_α
.Lx908_0:
                        .quad            .Lx908_0_s
.Lx908_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n606_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx909_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n607_statement_end_α
.Lx909_0:
                        .quad            .Lx909_0_s
.Lx909_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n607_statement_end_α:
                        add              rsp, 32
                                                                                        jmp   n608_statement_begin_α
#=======================================================================================================================
#         OUTPUT = roman(2024)
#-----------------------------------------------------------------------------------------------------------------------
n608_statement_begin_α:
                                                                                        jmp   n609_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n609_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx914_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n610_call_α
.Lx914_0:
                        .quad            2024
#-----------------------------------------------------------------------------------------------------------------------
n610_call_α:
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
                        mov              rdi, qword ptr [rip + .Lx916_0]                # name
                        mov              esi, 5                                         # np
                        mov              edx, 1                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx916_5
                        lea              r10, [rip + g_call_args]
                        mov              rax, qword ptr [r10 + 0]
                        mov              qword ptr [1879052304], rax                    # n
                        mov              rax, qword ptr [r10 + 8]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx916_6]
                        lea              rdx, [rip + .Lx916_7]
                                                                                        jmp   rax
.Lx916_6:
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
                                                                                        jmp   .Lx916_2
.Lx916_7:
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
                                                                                        jmp   .Lx916_2
.Lx916_5:
                        add              rsp, 96
.Lx916_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx916_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   main_γ
.Lx916_240:
                                                                                        jmp   n611_assign_α
n610_call_β:
                                                                                        jmp   main_γ
.Lx916_0:
                        .quad            .Lx916_0_s
.Lx916_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n611_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx917_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n612_statement_end_α
.Lx917_0:
                        .quad            .Lx917_0_s
.Lx917_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n612_statement_end_α:
                        add              rsp, 32
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n613_goto_α:
                                                                                        jmp   n614_statement_begin_α
n613_goto_β:
                                                                                        jmp   main_ω
#=======================================================================================================================
# roman   s = ''
#-----------------------------------------------------------------------------------------------------------------------
n614_statement_begin_α:
                                                                                        jmp   n615_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n615_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx923_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n616_assign_α
.Lx923_0:
                        .quad            .Lx923_0_s
.Lx923_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n616_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052320], rax                    # s
                        mov              qword ptr [1879052328], rdx
                                                                                        jmp   n617_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n617_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n618_statement_begin_α
#=======================================================================================================================
#         v = ARRAY(13)
#-----------------------------------------------------------------------------------------------------------------------
n618_statement_begin_α:
                                                                                        jmp   n619_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n619_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx929_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n620_call_α
.Lx929_0:
                        .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n620_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd931:            .string          "ARRAY"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd931]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx930_240
                                                                                        jmp   n622_statement_end_α
.Lx930_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n621_assign_α
n620_call_β:
                                                                                        jmp   n622_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n621_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052336], rax                    # v
                        mov              qword ptr [1879052344], rdx
                                                                                        jmp   n622_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n622_statement_end_α:
                        add              rsp, 32
                                                                                        jmp   n623_statement_begin_α
#=======================================================================================================================
#         v<1> = 1000
#-----------------------------------------------------------------------------------------------------------------------
n623_statement_begin_α:
                                                                                        jmp   n624_var_α
#-----------------------------------------------------------------------------------------------------------------------
n624_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n625_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n625_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx938_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n626_subscript_α
n625_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n630_statement_begin_α
.Lx938_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n626_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx939_240
                        add              rsp, 16
                                                                                        jmp   n625_lit_integer_β
.Lx939_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n627_lit_integer_α
n626_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n625_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n627_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx940_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n628_assign_var_α
n627_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n626_subscript_β
.Lx940_0:
                        .quad            1000
#-----------------------------------------------------------------------------------------------------------------------
n628_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx941_240
                        add              rsp, 16
                                                                                        jmp   n627_lit_integer_β
.Lx941_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n629_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n629_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n630_statement_begin_α
#=======================================================================================================================
#         v<2> = 900
#-----------------------------------------------------------------------------------------------------------------------
n630_statement_begin_α:
                                                                                        jmp   n631_var_α
#-----------------------------------------------------------------------------------------------------------------------
n631_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n632_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n632_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx947_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n633_subscript_α
n632_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n637_statement_begin_α
.Lx947_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n633_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx948_240
                        add              rsp, 16
                                                                                        jmp   n632_lit_integer_β
.Lx948_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n634_lit_integer_α
n633_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n632_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n634_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx949_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n635_assign_var_α
n634_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n633_subscript_β
.Lx949_0:
                        .quad            900
#-----------------------------------------------------------------------------------------------------------------------
n635_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx950_240
                        add              rsp, 16
                                                                                        jmp   n634_lit_integer_β
.Lx950_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n636_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n636_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n637_statement_begin_α
#=======================================================================================================================
#         v<3> = 500
#-----------------------------------------------------------------------------------------------------------------------
n637_statement_begin_α:
                                                                                        jmp   n638_var_α
#-----------------------------------------------------------------------------------------------------------------------
n638_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n639_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n639_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx956_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n640_subscript_α
n639_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n644_statement_begin_α
.Lx956_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n640_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx957_240
                        add              rsp, 16
                                                                                        jmp   n639_lit_integer_β
.Lx957_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n641_lit_integer_α
n640_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n639_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n641_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx958_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n642_assign_var_α
n641_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n640_subscript_β
.Lx958_0:
                        .quad            500
#-----------------------------------------------------------------------------------------------------------------------
n642_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx959_240
                        add              rsp, 16
                                                                                        jmp   n641_lit_integer_β
.Lx959_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n643_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n643_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n644_statement_begin_α
#=======================================================================================================================
#         v<4> = 400
#-----------------------------------------------------------------------------------------------------------------------
n644_statement_begin_α:
                                                                                        jmp   n645_var_α
#-----------------------------------------------------------------------------------------------------------------------
n645_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n646_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n646_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx965_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n647_subscript_α
n646_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n651_statement_begin_α
.Lx965_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n647_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx966_240
                        add              rsp, 16
                                                                                        jmp   n646_lit_integer_β
.Lx966_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n648_lit_integer_α
n647_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n646_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n648_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx967_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n649_assign_var_α
n648_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n647_subscript_β
.Lx967_0:
                        .quad            400
#-----------------------------------------------------------------------------------------------------------------------
n649_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx968_240
                        add              rsp, 16
                                                                                        jmp   n648_lit_integer_β
.Lx968_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n650_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n650_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n651_statement_begin_α
#=======================================================================================================================
#         v<5> = 100
#-----------------------------------------------------------------------------------------------------------------------
n651_statement_begin_α:
                                                                                        jmp   n652_var_α
#-----------------------------------------------------------------------------------------------------------------------
n652_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n653_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n653_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx974_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n654_subscript_α
n653_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n658_statement_begin_α
.Lx974_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n654_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx975_240
                        add              rsp, 16
                                                                                        jmp   n653_lit_integer_β
.Lx975_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n655_lit_integer_α
n654_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n653_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n655_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx976_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n656_assign_var_α
n655_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n654_subscript_β
.Lx976_0:
                        .quad            100
#-----------------------------------------------------------------------------------------------------------------------
n656_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx977_240
                        add              rsp, 16
                                                                                        jmp   n655_lit_integer_β
.Lx977_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n657_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n657_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n658_statement_begin_α
#=======================================================================================================================
#         v<6> = 90
#-----------------------------------------------------------------------------------------------------------------------
n658_statement_begin_α:
                                                                                        jmp   n659_var_α
#-----------------------------------------------------------------------------------------------------------------------
n659_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n660_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n660_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx983_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n661_subscript_α
n660_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n665_statement_begin_α
.Lx983_0:
                        .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n661_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx984_240
                        add              rsp, 16
                                                                                        jmp   n660_lit_integer_β
.Lx984_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n662_lit_integer_α
n661_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n660_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n662_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx985_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n663_assign_var_α
n662_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n661_subscript_β
.Lx985_0:
                        .quad            90
#-----------------------------------------------------------------------------------------------------------------------
n663_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx986_240
                        add              rsp, 16
                                                                                        jmp   n662_lit_integer_β
.Lx986_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n664_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n664_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n665_statement_begin_α
#=======================================================================================================================
#         v<7> = 50
#-----------------------------------------------------------------------------------------------------------------------
n665_statement_begin_α:
                                                                                        jmp   n666_var_α
#-----------------------------------------------------------------------------------------------------------------------
n666_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n667_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n667_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx992_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n668_subscript_α
n667_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n672_statement_begin_α
.Lx992_0:
                        .quad            7
#-----------------------------------------------------------------------------------------------------------------------
n668_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx993_240
                        add              rsp, 16
                                                                                        jmp   n667_lit_integer_β
.Lx993_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n669_lit_integer_α
n668_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n667_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n669_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx994_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n670_assign_var_α
n669_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n668_subscript_β
.Lx994_0:
                        .quad            50
#-----------------------------------------------------------------------------------------------------------------------
n670_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx995_240
                        add              rsp, 16
                                                                                        jmp   n669_lit_integer_β
.Lx995_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n671_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n671_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n672_statement_begin_α
#=======================================================================================================================
#         v<8> = 40
#-----------------------------------------------------------------------------------------------------------------------
n672_statement_begin_α:
                                                                                        jmp   n673_var_α
#-----------------------------------------------------------------------------------------------------------------------
n673_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n674_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n674_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1001_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n675_subscript_α
n674_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n679_statement_begin_α
.Lx1001_0:
                        .quad            8
#-----------------------------------------------------------------------------------------------------------------------
n675_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1002_240
                        add              rsp, 16
                                                                                        jmp   n674_lit_integer_β
.Lx1002_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n676_lit_integer_α
n675_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n674_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n676_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1003_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n677_assign_var_α
n676_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n675_subscript_β
.Lx1003_0:
                        .quad            40
#-----------------------------------------------------------------------------------------------------------------------
n677_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1004_240
                        add              rsp, 16
                                                                                        jmp   n676_lit_integer_β
.Lx1004_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n678_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n678_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n679_statement_begin_α
#=======================================================================================================================
#         v<9> = 10
#-----------------------------------------------------------------------------------------------------------------------
n679_statement_begin_α:
                                                                                        jmp   n680_var_α
#-----------------------------------------------------------------------------------------------------------------------
n680_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n681_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n681_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1010_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n682_subscript_α
n681_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n686_statement_begin_α
.Lx1010_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n682_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1011_240
                        add              rsp, 16
                                                                                        jmp   n681_lit_integer_β
.Lx1011_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n683_lit_integer_α
n682_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n681_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n683_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1012_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n684_assign_var_α
n683_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n682_subscript_β
.Lx1012_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n684_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1013_240
                        add              rsp, 16
                                                                                        jmp   n683_lit_integer_β
.Lx1013_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n685_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n685_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n686_statement_begin_α
#=======================================================================================================================
#         v<10> = 9
#-----------------------------------------------------------------------------------------------------------------------
n686_statement_begin_α:
                                                                                        jmp   n687_var_α
#-----------------------------------------------------------------------------------------------------------------------
n687_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n688_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n688_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1019_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n689_subscript_α
n688_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n693_statement_begin_α
.Lx1019_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n689_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1020_240
                        add              rsp, 16
                                                                                        jmp   n688_lit_integer_β
.Lx1020_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n690_lit_integer_α
n689_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n688_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n690_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1021_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n691_assign_var_α
n690_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n689_subscript_β
.Lx1021_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n691_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1022_240
                        add              rsp, 16
                                                                                        jmp   n690_lit_integer_β
.Lx1022_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n692_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n692_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n693_statement_begin_α
#=======================================================================================================================
#         v<11> = 5
#-----------------------------------------------------------------------------------------------------------------------
n693_statement_begin_α:
                                                                                        jmp   n694_var_α
#-----------------------------------------------------------------------------------------------------------------------
n694_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n695_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n695_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1028_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n696_subscript_α
n695_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n700_statement_begin_α
.Lx1028_0:
                        .quad            11
#-----------------------------------------------------------------------------------------------------------------------
n696_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1029_240
                        add              rsp, 16
                                                                                        jmp   n695_lit_integer_β
.Lx1029_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n697_lit_integer_α
n696_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n695_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n697_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1030_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n698_assign_var_α
n697_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n696_subscript_β
.Lx1030_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n698_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1031_240
                        add              rsp, 16
                                                                                        jmp   n697_lit_integer_β
.Lx1031_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n699_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n699_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n700_statement_begin_α
#=======================================================================================================================
#         v<12> = 4
#-----------------------------------------------------------------------------------------------------------------------
n700_statement_begin_α:
                                                                                        jmp   n701_var_α
#-----------------------------------------------------------------------------------------------------------------------
n701_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n702_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n702_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1037_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n703_subscript_α
n702_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n707_statement_begin_α
.Lx1037_0:
                        .quad            12
#-----------------------------------------------------------------------------------------------------------------------
n703_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1038_240
                        add              rsp, 16
                                                                                        jmp   n702_lit_integer_β
.Lx1038_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n704_lit_integer_α
n703_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n702_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n704_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1039_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n705_assign_var_α
n704_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n703_subscript_β
.Lx1039_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n705_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1040_240
                        add              rsp, 16
                                                                                        jmp   n704_lit_integer_β
.Lx1040_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n706_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n706_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n707_statement_begin_α
#=======================================================================================================================
#         v<13> = 1
#-----------------------------------------------------------------------------------------------------------------------
n707_statement_begin_α:
                                                                                        jmp   n708_var_α
#-----------------------------------------------------------------------------------------------------------------------
n708_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n709_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n709_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1046_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n710_subscript_α
n709_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n714_statement_begin_α
.Lx1046_0:
                        .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n710_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1047_240
                        add              rsp, 16
                                                                                        jmp   n709_lit_integer_β
.Lx1047_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n711_lit_integer_α
n710_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n709_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n711_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1048_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n712_assign_var_α
n711_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n710_subscript_β
.Lx1048_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n712_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1049_240
                        add              rsp, 16
                                                                                        jmp   n711_lit_integer_β
.Lx1049_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n713_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n713_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n714_statement_begin_α
#=======================================================================================================================
#         r = ARRAY(13)
#-----------------------------------------------------------------------------------------------------------------------
n714_statement_begin_α:
                                                                                        jmp   n715_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n715_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1054_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n716_call_α
.Lx1054_0:
                        .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n716_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd1056:           .string          "ARRAY"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd1056]                      # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx1055_240
                                                                                        jmp   n718_statement_end_α
.Lx1055_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n717_assign_α
n716_call_β:
                                                                                        jmp   n718_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n717_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052352], rax                    # r
                        mov              qword ptr [1879052360], rdx
                                                                                        jmp   n718_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n718_statement_end_α:
                        add              rsp, 32
                                                                                        jmp   n719_statement_begin_α
#=======================================================================================================================
#         r<1> = 'M'
#-----------------------------------------------------------------------------------------------------------------------
n719_statement_begin_α:
                                                                                        jmp   n720_var_α
#-----------------------------------------------------------------------------------------------------------------------
n720_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n721_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n721_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1063_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n722_subscript_α
n721_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n726_statement_begin_α
.Lx1063_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n722_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1064_240
                        add              rsp, 16
                                                                                        jmp   n721_lit_integer_β
.Lx1064_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n723_lit_string_α
n722_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n721_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n723_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx1065_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n724_assign_var_α
n723_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n722_subscript_β
.Lx1065_0:
                        .quad            .Lx1065_0_s
.Lx1065_0_s:
                        .string          "M"
#-----------------------------------------------------------------------------------------------------------------------
n724_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1066_240
                        add              rsp, 16
                                                                                        jmp   n723_lit_string_β
.Lx1066_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n725_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n725_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n726_statement_begin_α
#=======================================================================================================================
#         r<2> = 'CM'
#-----------------------------------------------------------------------------------------------------------------------
n726_statement_begin_α:
                                                                                        jmp   n727_var_α
#-----------------------------------------------------------------------------------------------------------------------
n727_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n728_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n728_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1072_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n729_subscript_α
n728_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n733_statement_begin_α
.Lx1072_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n729_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1073_240
                        add              rsp, 16
                                                                                        jmp   n728_lit_integer_β
.Lx1073_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n730_lit_string_α
n729_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n728_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n730_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx1074_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n731_assign_var_α
n730_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n729_subscript_β
.Lx1074_0:
                        .quad            .Lx1074_0_s
.Lx1074_0_s:
                        .string          "CM"
#-----------------------------------------------------------------------------------------------------------------------
n731_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1075_240
                        add              rsp, 16
                                                                                        jmp   n730_lit_string_β
.Lx1075_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n732_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n732_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n733_statement_begin_α
#=======================================================================================================================
#         r<3> = 'D'
#-----------------------------------------------------------------------------------------------------------------------
n733_statement_begin_α:
                                                                                        jmp   n734_var_α
#-----------------------------------------------------------------------------------------------------------------------
n734_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n735_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n735_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1081_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n736_subscript_α
n735_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n740_statement_begin_α
.Lx1081_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n736_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1082_240
                        add              rsp, 16
                                                                                        jmp   n735_lit_integer_β
.Lx1082_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n737_lit_string_α
n736_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n735_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n737_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx1083_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n738_assign_var_α
n737_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n736_subscript_β
.Lx1083_0:
                        .quad            .Lx1083_0_s
.Lx1083_0_s:
                        .string          "D"
#-----------------------------------------------------------------------------------------------------------------------
n738_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1084_240
                        add              rsp, 16
                                                                                        jmp   n737_lit_string_β
.Lx1084_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n739_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n739_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n740_statement_begin_α
#=======================================================================================================================
#         r<4> = 'CD'
#-----------------------------------------------------------------------------------------------------------------------
n740_statement_begin_α:
                                                                                        jmp   n741_var_α
#-----------------------------------------------------------------------------------------------------------------------
n741_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n742_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n742_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1090_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n743_subscript_α
n742_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n747_statement_begin_α
.Lx1090_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n743_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1091_240
                        add              rsp, 16
                                                                                        jmp   n742_lit_integer_β
.Lx1091_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n744_lit_string_α
n743_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n742_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n744_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx1092_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n745_assign_var_α
n744_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n743_subscript_β
.Lx1092_0:
                        .quad            .Lx1092_0_s
.Lx1092_0_s:
                        .string          "CD"
#-----------------------------------------------------------------------------------------------------------------------
n745_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1093_240
                        add              rsp, 16
                                                                                        jmp   n744_lit_string_β
.Lx1093_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n746_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n746_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n747_statement_begin_α
#=======================================================================================================================
#         r<5> = 'C'
#-----------------------------------------------------------------------------------------------------------------------
n747_statement_begin_α:
                                                                                        jmp   n748_var_α
#-----------------------------------------------------------------------------------------------------------------------
n748_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n749_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n749_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1099_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n750_subscript_α
n749_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n754_statement_begin_α
.Lx1099_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n750_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1100_240
                        add              rsp, 16
                                                                                        jmp   n749_lit_integer_β
.Lx1100_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n751_lit_string_α
n750_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n749_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n751_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx1101_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n752_assign_var_α
n751_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n750_subscript_β
.Lx1101_0:
                        .quad            .Lx1101_0_s
.Lx1101_0_s:
                        .string          "C"
#-----------------------------------------------------------------------------------------------------------------------
n752_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1102_240
                        add              rsp, 16
                                                                                        jmp   n751_lit_string_β
.Lx1102_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n753_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n753_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n754_statement_begin_α
#=======================================================================================================================
#         r<6> = 'XC'
#-----------------------------------------------------------------------------------------------------------------------
n754_statement_begin_α:
                                                                                        jmp   n755_var_α
#-----------------------------------------------------------------------------------------------------------------------
n755_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n756_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n756_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1108_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n757_subscript_α
n756_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n761_statement_begin_α
.Lx1108_0:
                        .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n757_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1109_240
                        add              rsp, 16
                                                                                        jmp   n756_lit_integer_β
.Lx1109_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n758_lit_string_α
n757_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n756_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n758_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx1110_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n759_assign_var_α
n758_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n757_subscript_β
.Lx1110_0:
                        .quad            .Lx1110_0_s
.Lx1110_0_s:
                        .string          "XC"
#-----------------------------------------------------------------------------------------------------------------------
n759_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1111_240
                        add              rsp, 16
                                                                                        jmp   n758_lit_string_β
.Lx1111_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n760_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n760_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n761_statement_begin_α
#=======================================================================================================================
#         r<7> = 'L'
#-----------------------------------------------------------------------------------------------------------------------
n761_statement_begin_α:
                                                                                        jmp   n762_var_α
#-----------------------------------------------------------------------------------------------------------------------
n762_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n763_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n763_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1117_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n764_subscript_α
n763_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n768_statement_begin_α
.Lx1117_0:
                        .quad            7
#-----------------------------------------------------------------------------------------------------------------------
n764_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1118_240
                        add              rsp, 16
                                                                                        jmp   n763_lit_integer_β
.Lx1118_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n765_lit_string_α
n764_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n763_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n765_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx1119_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n766_assign_var_α
n765_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n764_subscript_β
.Lx1119_0:
                        .quad            .Lx1119_0_s
.Lx1119_0_s:
                        .string          "L"
#-----------------------------------------------------------------------------------------------------------------------
n766_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1120_240
                        add              rsp, 16
                                                                                        jmp   n765_lit_string_β
.Lx1120_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n767_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n767_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n768_statement_begin_α
#=======================================================================================================================
#         r<8> = 'XL'
#-----------------------------------------------------------------------------------------------------------------------
n768_statement_begin_α:
                                                                                        jmp   n769_var_α
#-----------------------------------------------------------------------------------------------------------------------
n769_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n770_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n770_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1126_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n771_subscript_α
n770_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n775_statement_begin_α
.Lx1126_0:
                        .quad            8
#-----------------------------------------------------------------------------------------------------------------------
n771_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1127_240
                        add              rsp, 16
                                                                                        jmp   n770_lit_integer_β
.Lx1127_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n772_lit_string_α
n771_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n770_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n772_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx1128_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n773_assign_var_α
n772_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n771_subscript_β
.Lx1128_0:
                        .quad            .Lx1128_0_s
.Lx1128_0_s:
                        .string          "XL"
#-----------------------------------------------------------------------------------------------------------------------
n773_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1129_240
                        add              rsp, 16
                                                                                        jmp   n772_lit_string_β
.Lx1129_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n774_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n774_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n775_statement_begin_α
#=======================================================================================================================
#         r<9> = 'X'
#-----------------------------------------------------------------------------------------------------------------------
n775_statement_begin_α:
                                                                                        jmp   n776_var_α
#-----------------------------------------------------------------------------------------------------------------------
n776_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n777_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n777_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1135_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n778_subscript_α
n777_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n782_statement_begin_α
.Lx1135_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n778_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1136_240
                        add              rsp, 16
                                                                                        jmp   n777_lit_integer_β
.Lx1136_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n779_lit_string_α
n778_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n777_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n779_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx1137_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n780_assign_var_α
n779_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n778_subscript_β
.Lx1137_0:
                        .quad            .Lx1137_0_s
.Lx1137_0_s:
                        .string          "X"
#-----------------------------------------------------------------------------------------------------------------------
n780_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1138_240
                        add              rsp, 16
                                                                                        jmp   n779_lit_string_β
.Lx1138_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n781_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n781_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n782_statement_begin_α
#=======================================================================================================================
#         r<10> = 'IX'
#-----------------------------------------------------------------------------------------------------------------------
n782_statement_begin_α:
                                                                                        jmp   n783_var_α
#-----------------------------------------------------------------------------------------------------------------------
n783_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n784_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n784_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1144_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n785_subscript_α
n784_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n789_statement_begin_α
.Lx1144_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n785_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1145_240
                        add              rsp, 16
                                                                                        jmp   n784_lit_integer_β
.Lx1145_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n786_lit_string_α
n785_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n784_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n786_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx1146_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n787_assign_var_α
n786_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n785_subscript_β
.Lx1146_0:
                        .quad            .Lx1146_0_s
.Lx1146_0_s:
                        .string          "IX"
#-----------------------------------------------------------------------------------------------------------------------
n787_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1147_240
                        add              rsp, 16
                                                                                        jmp   n786_lit_string_β
.Lx1147_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n788_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n788_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n789_statement_begin_α
#=======================================================================================================================
#         r<11> = 'V'
#-----------------------------------------------------------------------------------------------------------------------
n789_statement_begin_α:
                                                                                        jmp   n790_var_α
#-----------------------------------------------------------------------------------------------------------------------
n790_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n791_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n791_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1153_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n792_subscript_α
n791_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n796_statement_begin_α
.Lx1153_0:
                        .quad            11
#-----------------------------------------------------------------------------------------------------------------------
n792_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1154_240
                        add              rsp, 16
                                                                                        jmp   n791_lit_integer_β
.Lx1154_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n793_lit_string_α
n792_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n791_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n793_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx1155_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n794_assign_var_α
n793_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n792_subscript_β
.Lx1155_0:
                        .quad            .Lx1155_0_s
.Lx1155_0_s:
                        .string          "V"
#-----------------------------------------------------------------------------------------------------------------------
n794_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1156_240
                        add              rsp, 16
                                                                                        jmp   n793_lit_string_β
.Lx1156_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n795_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n795_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n796_statement_begin_α
#=======================================================================================================================
#         r<12> = 'IV'
#-----------------------------------------------------------------------------------------------------------------------
n796_statement_begin_α:
                                                                                        jmp   n797_var_α
#-----------------------------------------------------------------------------------------------------------------------
n797_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n798_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n798_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1162_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n799_subscript_α
n798_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n803_statement_begin_α
.Lx1162_0:
                        .quad            12
#-----------------------------------------------------------------------------------------------------------------------
n799_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1163_240
                        add              rsp, 16
                                                                                        jmp   n798_lit_integer_β
.Lx1163_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n800_lit_string_α
n799_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n798_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n800_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx1164_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n801_assign_var_α
n800_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n799_subscript_β
.Lx1164_0:
                        .quad            .Lx1164_0_s
.Lx1164_0_s:
                        .string          "IV"
#-----------------------------------------------------------------------------------------------------------------------
n801_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1165_240
                        add              rsp, 16
                                                                                        jmp   n800_lit_string_β
.Lx1165_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n802_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n802_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n803_statement_begin_α
#=======================================================================================================================
#         r<13> = 'I'
#-----------------------------------------------------------------------------------------------------------------------
n803_statement_begin_α:
                                                                                        jmp   n804_var_α
#-----------------------------------------------------------------------------------------------------------------------
n804_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n805_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n805_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1171_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n806_subscript_α
n805_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n810_statement_begin_α
.Lx1171_0:
                        .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n806_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1172_240
                        add              rsp, 16
                                                                                        jmp   n805_lit_integer_β
.Lx1172_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n807_lit_string_α
n806_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n805_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n807_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx1173_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n808_assign_var_α
n807_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n806_subscript_β
.Lx1173_0:
                        .quad            .Lx1173_0_s
.Lx1173_0_s:
                        .string          "I"
#-----------------------------------------------------------------------------------------------------------------------
n808_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1174_240
                        add              rsp, 16
                                                                                        jmp   n807_lit_string_β
.Lx1174_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n809_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n809_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n810_statement_begin_α
#=======================================================================================================================
#         i = 1
#-----------------------------------------------------------------------------------------------------------------------
n810_statement_begin_α:
                                                                                        jmp   n811_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n811_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1179_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n812_assign_α
.Lx1179_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n812_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_integer
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052368], rax                    # i
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n813_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n813_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n814_statement_begin_α
#=======================================================================================================================
# RLOOP   GT(n, 0)                                                       :F(RDONE)
#-----------------------------------------------------------------------------------------------------------------------
n814_statement_begin_α:
                                                                                        jmp   n815_var_α
#-----------------------------------------------------------------------------------------------------------------------
n815_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # n
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n816_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n816_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1186_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n817_coerce_numeric_α
n816_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n855_statement_begin_α
.Lx1186_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n817_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # var
                        cmp              eax, 5
                                                                                        je    .Lx1188_1
                        cmp              eax, 3
                                                                                        jne   .Lx1188_0
                        mov              eax, dword ptr [rsp + 16]                      # lit_integer
                        cmp              eax, 3
                                                                                        jne   .Lx1188_0
.Lx1188_1:
                        mov              rax, qword ptr [rsp + 32]                      # var
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # var
                        mov              qword ptr [rsp + 8], rax                       # result
                                                                                        jmp   n818_coerce_numeric_α
.Lx1188_0:
                        lea              rdi, [rsp + 32]                                # var
                        lea              rsi, [rsp + 16]                                # lit_integer
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 111                                       # codes
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n818_coerce_numeric_α
n817_coerce_numeric_β:
                        add              rsp, 16
                                                                                        jmp   n816_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n818_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # lit_integer
                        cmp              eax, 5
                                                                                        je    .Lx1190_1
                        cmp              eax, 3
                                                                                        jne   .Lx1190_0
                        mov              eax, dword ptr [rsp + 48]                      # var
                        cmp              eax, 3
                                                                                        jne   .Lx1190_0
.Lx1190_1:
                        mov              rax, qword ptr [rsp + 32]                      # lit_integer
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # lit_integer
                        mov              qword ptr [rsp + 8], rax                       # result
                                                                                        jmp   n819_cmp_test_α
.Lx1190_0:
                        lea              rdi, [rsp + 32]                                # lit_integer
                        lea              rsi, [rsp + 48]                                # var
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 112                                       # codes
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n819_cmp_test_α
n818_coerce_numeric_β:
                        add              rsp, 16
                                                                                        jmp   n817_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n819_cmp_test_α:
                        sub              rsp, 16
                        lea              rdi, [rsp + 32]                                # coerce_numeric
                        lea              rsi, [rsp + 16]                                # b
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jg    .Lx1192_240
                        add              rsp, 16
                                                                                        jmp   n818_coerce_numeric_β
.Lx1192_240:
                                                                                        jmp   n820_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n820_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n821_statement_begin_α
#=======================================================================================================================
#         GE(n, v<i>)                                                    :F(RNEXT)
#-----------------------------------------------------------------------------------------------------------------------
n821_statement_begin_α:
                                                                                        jmp   n822_var_α
#-----------------------------------------------------------------------------------------------------------------------
n822_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # n
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n823_var_α
#-----------------------------------------------------------------------------------------------------------------------
n823_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n824_var_α
n823_var_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n849_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n824_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052368]                    # i
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n825_subscript_α
n824_var_β:
                        add              rsp, 16
                                                                                        jmp   n823_var_β
#-----------------------------------------------------------------------------------------------------------------------
n825_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # idx
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1200_240
                        add              rsp, 16
                                                                                        jmp   n824_var_β
.Lx1200_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n826_deref_α
n825_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n824_var_β
#-----------------------------------------------------------------------------------------------------------------------
n826_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # subscript
                        mov              rsi, qword ptr [rsp + 24]                      # d
                        call             rt_deref@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1201_240
                        add              rsp, 16
                                                                                        jmp   n825_subscript_β
.Lx1201_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n827_coerce_numeric_α
n826_deref_β:
                        add              rsp, 16
                                                                                        jmp   n825_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n827_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 80]                      # var
                        cmp              eax, 5
                                                                                        je    .Lx1203_1
                        cmp              eax, 3
                                                                                        jne   .Lx1203_0
                        mov              eax, dword ptr [rsp + 16]                      # deref
                        cmp              eax, 3
                                                                                        jne   .Lx1203_0
.Lx1203_1:
                        mov              rax, qword ptr [rsp + 80]                      # var
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 88]                      # var
                        mov              qword ptr [rsp + 8], rax                       # result
                                                                                        jmp   n828_coerce_numeric_α
.Lx1203_0:
                        lea              rdi, [rsp + 80]                                # var
                        lea              rsi, [rsp + 16]                                # deref
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 109                                       # codes
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n828_coerce_numeric_α
n827_coerce_numeric_β:
                        add              rsp, 16
                                                                                        jmp   n826_deref_β
#-----------------------------------------------------------------------------------------------------------------------
n828_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # deref
                        cmp              eax, 5
                                                                                        je    .Lx1205_1
                        cmp              eax, 3
                                                                                        jne   .Lx1205_0
                        mov              eax, dword ptr [rsp + 96]                      # var
                        cmp              eax, 3
                                                                                        jne   .Lx1205_0
.Lx1205_1:
                        mov              rax, qword ptr [rsp + 32]                      # deref
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # deref
                        mov              qword ptr [rsp + 8], rax                       # result
                                                                                        jmp   n829_cmp_test_α
.Lx1205_0:
                        lea              rdi, [rsp + 32]                                # deref
                        lea              rsi, [rsp + 96]                                # var
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 110                                       # codes
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n829_cmp_test_α
n828_coerce_numeric_β:
                        add              rsp, 16
                                                                                        jmp   n827_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n829_cmp_test_α:
                        sub              rsp, 16
                        lea              rdi, [rsp + 32]                                # coerce_numeric
                        lea              rsi, [rsp + 16]                                # b
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jns   .Lx1207_240
                        add              rsp, 16
                                                                                        jmp   n828_coerce_numeric_β
.Lx1207_240:
                                                                                        jmp   n830_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n830_statement_end_α:
                        add              rsp, 128
                                                                                        jmp   n831_statement_begin_α
#=======================================================================================================================
#         s = s r<i>
#-----------------------------------------------------------------------------------------------------------------------
n831_statement_begin_α:
                                                                                        jmp   n832_var_α
#-----------------------------------------------------------------------------------------------------------------------
n832_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # s
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n833_var_α
#-----------------------------------------------------------------------------------------------------------------------
n833_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n834_var_α
n833_var_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n840_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n834_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052368]                    # i
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n835_subscript_α
n834_var_β:
                        add              rsp, 16
                                                                                        jmp   n833_var_β
#-----------------------------------------------------------------------------------------------------------------------
n835_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # idx
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1215_240
                        add              rsp, 16
                                                                                        jmp   n834_var_β
.Lx1215_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n836_deref_α
n835_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n834_var_β
#-----------------------------------------------------------------------------------------------------------------------
n836_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # subscript
                        mov              rsi, qword ptr [rsp + 24]                      # d
                        call             rt_deref@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1216_240
                        add              rsp, 16
                                                                                        jmp   n835_subscript_β
.Lx1216_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n837_binop_α
n836_deref_β:
                        add              rsp, 16
                                                                                        jmp   n835_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n837_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 80]                      # var
                        mov              rsi, qword ptr [rsp + 88]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # deref
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n838_assign_α
n837_binop_β:
                        add              rsp, 16
                                                                                        jmp   n836_deref_β
#-----------------------------------------------------------------------------------------------------------------------
n838_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052320], rax                    # s
                        mov              qword ptr [1879052328], rdx
                                                                                        jmp   n839_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n839_statement_end_α:
                        add              rsp, 96
                                                                                        jmp   n840_statement_begin_α
#=======================================================================================================================
#         n = n - v<i>                                                   :(RLOOP)
#-----------------------------------------------------------------------------------------------------------------------
n840_statement_begin_α:
                                                                                        jmp   n841_var_α
#-----------------------------------------------------------------------------------------------------------------------
n841_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # n
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n842_var_α
#-----------------------------------------------------------------------------------------------------------------------
n842_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n843_var_α
n842_var_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n814_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n843_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052368]                    # i
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n844_subscript_α
n843_var_β:
                        add              rsp, 16
                                                                                        jmp   n842_var_β
#-----------------------------------------------------------------------------------------------------------------------
n844_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # idx
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1226_240
                        add              rsp, 16
                                                                                        jmp   n843_var_β
.Lx1226_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n845_deref_α
n844_subscript_β:
                        add              rsp, 16
                                                                                        jmp   n843_var_β
#-----------------------------------------------------------------------------------------------------------------------
n845_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # subscript
                        mov              rsi, qword ptr [rsp + 24]                      # d
                        call             rt_deref@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1227_240
                        add              rsp, 16
                                                                                        jmp   n844_subscript_β
.Lx1227_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n846_binop_α
n845_deref_β:
                        add              rsp, 16
                                                                                        jmp   n844_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n846_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 80]                      # var
                        mov              rsi, qword ptr [rsp + 88]
                        mov              rdx, qword ptr [rsp + 16]                      # deref
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_sub@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1228_240
                        add              rsp, 16
                                                                                        jmp   n845_deref_β
.Lx1228_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n847_assign_α
n846_binop_β:
                        add              rsp, 16
                                                                                        jmp   n845_deref_β
#-----------------------------------------------------------------------------------------------------------------------
n847_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax                    # n
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   n848_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n848_statement_end_α:
                        add              rsp, 96
                                                                                        jmp   n814_statement_begin_α
#=======================================================================================================================
# RNEXT   i = i + 1                                                      :(RLOOP)
#-----------------------------------------------------------------------------------------------------------------------
n849_statement_begin_α:
                                                                                        jmp   n850_var_α
#-----------------------------------------------------------------------------------------------------------------------
n850_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052368]                    # i
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n851_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n851_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx1235_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n852_binop_α
n851_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n814_statement_begin_α
.Lx1235_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n852_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_add@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1236_240
                        add              rsp, 16
                                                                                        jmp   n851_lit_integer_β
.Lx1236_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n853_assign_α
n852_binop_β:
                        add              rsp, 16
                                                                                        jmp   n851_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n853_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052368], rax                    # i
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n854_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n854_statement_end_α:
                        add              rsp, 48
                                                                                        jmp   n814_statement_begin_α
#=======================================================================================================================
# RDONE   roman = s                                                      :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n855_statement_begin_α:
                                                                                        jmp   n856_var_α
#-----------------------------------------------------------------------------------------------------------------------
n856_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # s
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n857_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n857_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax                    # roman
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n858_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n858_statement_end_α:
                                                                                        jmp   n859_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n859_save_restore_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n860_goto_α:
                                                                                        jmp   n814_statement_begin_α
n860_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n861_goto_α:
                                                                                        jmp   n849_statement_begin_α
n861_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n862_goto_α:
                                                                                        jmp   n855_statement_begin_α
n862_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n863_goto_α:
                                                                                        jmp   n581_statement_begin_α
n863_goto_β:
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

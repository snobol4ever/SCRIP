                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_LBL__roman_α
proc_LBL__roman_α:
proc_LBL__roman_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n0_goto_α:
                                                                                        jmp   n1_lit_string_α
n0_goto_β:
                                                                                        jmp   proc_LBL__roman_ω
#=======================================================================================================================
# roman   s = ''
#-----------------------------------------------------------------------------------------------------------------------
n1_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx212_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n2_assign_α
.Lx212_0:
                        .quad            .Lx212_0_s
.Lx212_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n2_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052320], rax                    # s
                        mov              qword ptr [1879052328], rdx
                                                                                        jmp   n3_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n3_statement_α:
                        add              rsp, 16
                                                                                        jmp   n4_lit_integer_α
proc_LBL__roman_zw5s2_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n4_lit_integer_α
#=======================================================================================================================
#         v = ARRAY(13)
#-----------------------------------------------------------------------------------------------------------------------
n4_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx216_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n5_call_α
.Lx216_0:
                        .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n5_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd218:            .string          "ARRAY"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd218]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx217_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s3_ω_d16
.Lx217_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n6_assign_α
n5_call_β:
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s3_ω_d16
#-----------------------------------------------------------------------------------------------------------------------
n6_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052336], rax                    # v
                        mov              qword ptr [1879052344], rdx
                                                                                        jmp   n7_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n7_statement_α:
                        add              rsp, 32
                                                                                        jmp   n8_var_α
proc_LBL__roman_zw5s3_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n8_var_α
proc_LBL__roman_zw5s3_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n8_var_α
#=======================================================================================================================
#         v<1> = 1000
#-----------------------------------------------------------------------------------------------------------------------
n8_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n9_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n9_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx223_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n10_subscript_α
.Lx223_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n10_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx224_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s4_ω_d32
.Lx224_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n11_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n11_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx225_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n12_assign_var_α
.Lx225_0:
                        .quad            1000
#-----------------------------------------------------------------------------------------------------------------------
n12_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx226_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s4_ω_d64
.Lx226_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n13_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n13_statement_α:
                        add              rsp, 80
                                                                                        jmp   n14_var_α
proc_LBL__roman_zw5s4_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n14_var_α
proc_LBL__roman_zw5s4_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n14_var_α
proc_LBL__roman_zw5s4_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n14_var_α
proc_LBL__roman_zw5s4_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n14_var_α
#=======================================================================================================================
#         v<2> = 900
#-----------------------------------------------------------------------------------------------------------------------
n14_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n15_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n15_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx230_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n16_subscript_α
.Lx230_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n16_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx231_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s5_ω_d32
.Lx231_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n17_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n17_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx232_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n18_assign_var_α
.Lx232_0:
                        .quad            900
#-----------------------------------------------------------------------------------------------------------------------
n18_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx233_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s5_ω_d64
.Lx233_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n19_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n19_statement_α:
                        add              rsp, 80
                                                                                        jmp   n20_var_α
proc_LBL__roman_zw5s5_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n20_var_α
proc_LBL__roman_zw5s5_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n20_var_α
proc_LBL__roman_zw5s5_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n20_var_α
proc_LBL__roman_zw5s5_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n20_var_α
proc_LBL__roman_zw5s5_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n20_var_α
#=======================================================================================================================
#         v<3> = 500
#-----------------------------------------------------------------------------------------------------------------------
n20_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n21_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n21_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx237_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n22_subscript_α
.Lx237_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n22_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx238_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s6_ω_d32
.Lx238_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n23_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n23_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx239_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n24_assign_var_α
.Lx239_0:
                        .quad            500
#-----------------------------------------------------------------------------------------------------------------------
n24_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx240_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s6_ω_d64
.Lx240_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n25_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n25_statement_α:
                        add              rsp, 80
                                                                                        jmp   n26_var_α
proc_LBL__roman_zw5s6_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n26_var_α
proc_LBL__roman_zw5s6_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n26_var_α
proc_LBL__roman_zw5s6_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n26_var_α
proc_LBL__roman_zw5s6_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n26_var_α
proc_LBL__roman_zw5s6_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n26_var_α
#=======================================================================================================================
#         v<4> = 400
#-----------------------------------------------------------------------------------------------------------------------
n26_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n27_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n27_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx244_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n28_subscript_α
.Lx244_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n28_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx245_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s7_ω_d32
.Lx245_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n29_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n29_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx246_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n30_assign_var_α
.Lx246_0:
                        .quad            400
#-----------------------------------------------------------------------------------------------------------------------
n30_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx247_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s7_ω_d64
.Lx247_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n31_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n31_statement_α:
                        add              rsp, 80
                                                                                        jmp   n32_var_α
proc_LBL__roman_zw5s7_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n32_var_α
proc_LBL__roman_zw5s7_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n32_var_α
proc_LBL__roman_zw5s7_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n32_var_α
proc_LBL__roman_zw5s7_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n32_var_α
proc_LBL__roman_zw5s7_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n32_var_α
#=======================================================================================================================
#         v<5> = 100
#-----------------------------------------------------------------------------------------------------------------------
n32_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n33_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n33_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx251_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n34_subscript_α
.Lx251_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n34_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx252_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s8_ω_d32
.Lx252_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n35_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n35_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx253_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n36_assign_var_α
.Lx253_0:
                        .quad            100
#-----------------------------------------------------------------------------------------------------------------------
n36_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx254_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s8_ω_d64
.Lx254_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n37_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n37_statement_α:
                        add              rsp, 80
                                                                                        jmp   n38_var_α
proc_LBL__roman_zw5s8_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n38_var_α
proc_LBL__roman_zw5s8_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n38_var_α
proc_LBL__roman_zw5s8_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n38_var_α
proc_LBL__roman_zw5s8_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n38_var_α
proc_LBL__roman_zw5s8_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n38_var_α
#=======================================================================================================================
#         v<6> = 90
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
                        mov              rax, qword ptr [rip + .Lx258_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n40_subscript_α
.Lx258_0:
                        .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n40_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx259_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s9_ω_d32
.Lx259_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n41_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n41_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx260_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n42_assign_var_α
.Lx260_0:
                        .quad            90
#-----------------------------------------------------------------------------------------------------------------------
n42_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx261_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s9_ω_d64
.Lx261_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n43_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n43_statement_α:
                        add              rsp, 80
                                                                                        jmp   n44_var_α
proc_LBL__roman_zw5s9_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n44_var_α
proc_LBL__roman_zw5s9_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n44_var_α
proc_LBL__roman_zw5s9_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n44_var_α
proc_LBL__roman_zw5s9_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n44_var_α
proc_LBL__roman_zw5s9_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n44_var_α
#=======================================================================================================================
#         v<7> = 50
#-----------------------------------------------------------------------------------------------------------------------
n44_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n45_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n45_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx265_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n46_subscript_α
.Lx265_0:
                        .quad            7
#-----------------------------------------------------------------------------------------------------------------------
n46_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx266_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s10_ω_d32
.Lx266_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n47_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n47_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx267_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n48_assign_var_α
.Lx267_0:
                        .quad            50
#-----------------------------------------------------------------------------------------------------------------------
n48_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx268_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s10_ω_d64
.Lx268_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n49_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n49_statement_α:
                        add              rsp, 80
                                                                                        jmp   n50_var_α
proc_LBL__roman_zw5s10_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n50_var_α
proc_LBL__roman_zw5s10_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n50_var_α
proc_LBL__roman_zw5s10_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n50_var_α
proc_LBL__roman_zw5s10_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n50_var_α
proc_LBL__roman_zw5s10_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n50_var_α
#=======================================================================================================================
#         v<8> = 40
#-----------------------------------------------------------------------------------------------------------------------
n50_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n51_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n51_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx272_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n52_subscript_α
.Lx272_0:
                        .quad            8
#-----------------------------------------------------------------------------------------------------------------------
n52_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx273_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s11_ω_d32
.Lx273_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n53_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n53_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx274_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n54_assign_var_α
.Lx274_0:
                        .quad            40
#-----------------------------------------------------------------------------------------------------------------------
n54_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx275_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s11_ω_d64
.Lx275_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n55_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n55_statement_α:
                        add              rsp, 80
                                                                                        jmp   n56_var_α
proc_LBL__roman_zw5s11_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n56_var_α
proc_LBL__roman_zw5s11_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n56_var_α
proc_LBL__roman_zw5s11_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n56_var_α
proc_LBL__roman_zw5s11_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n56_var_α
proc_LBL__roman_zw5s11_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n56_var_α
#=======================================================================================================================
#         v<9> = 10
#-----------------------------------------------------------------------------------------------------------------------
n56_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n57_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n57_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx279_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n58_subscript_α
.Lx279_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n58_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx280_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s12_ω_d32
.Lx280_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n59_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n59_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx281_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n60_assign_var_α
.Lx281_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n60_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx282_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s12_ω_d64
.Lx282_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n61_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n61_statement_α:
                        add              rsp, 80
                                                                                        jmp   n62_var_α
proc_LBL__roman_zw5s12_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n62_var_α
proc_LBL__roman_zw5s12_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n62_var_α
proc_LBL__roman_zw5s12_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n62_var_α
proc_LBL__roman_zw5s12_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n62_var_α
proc_LBL__roman_zw5s12_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n62_var_α
#=======================================================================================================================
#         v<10> = 9
#-----------------------------------------------------------------------------------------------------------------------
n62_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n63_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n63_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx286_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n64_subscript_α
.Lx286_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n64_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx287_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s13_ω_d32
.Lx287_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n65_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n65_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx288_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n66_assign_var_α
.Lx288_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n66_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx289_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s13_ω_d64
.Lx289_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n67_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n67_statement_α:
                        add              rsp, 80
                                                                                        jmp   n68_var_α
proc_LBL__roman_zw5s13_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n68_var_α
proc_LBL__roman_zw5s13_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n68_var_α
proc_LBL__roman_zw5s13_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n68_var_α
proc_LBL__roman_zw5s13_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n68_var_α
proc_LBL__roman_zw5s13_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n68_var_α
#=======================================================================================================================
#         v<11> = 5
#-----------------------------------------------------------------------------------------------------------------------
n68_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n69_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n69_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx293_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n70_subscript_α
.Lx293_0:
                        .quad            11
#-----------------------------------------------------------------------------------------------------------------------
n70_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx294_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s14_ω_d32
.Lx294_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n71_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n71_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx295_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n72_assign_var_α
.Lx295_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n72_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx296_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s14_ω_d64
.Lx296_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n73_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n73_statement_α:
                        add              rsp, 80
                                                                                        jmp   n74_var_α
proc_LBL__roman_zw5s14_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n74_var_α
proc_LBL__roman_zw5s14_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n74_var_α
proc_LBL__roman_zw5s14_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n74_var_α
proc_LBL__roman_zw5s14_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n74_var_α
proc_LBL__roman_zw5s14_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n74_var_α
#=======================================================================================================================
#         v<12> = 4
#-----------------------------------------------------------------------------------------------------------------------
n74_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n75_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n75_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx300_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n76_subscript_α
.Lx300_0:
                        .quad            12
#-----------------------------------------------------------------------------------------------------------------------
n76_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx301_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s15_ω_d32
.Lx301_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n77_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n77_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx302_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n78_assign_var_α
.Lx302_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n78_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx303_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s15_ω_d64
.Lx303_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n79_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n79_statement_α:
                        add              rsp, 80
                                                                                        jmp   n80_var_α
proc_LBL__roman_zw5s15_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n80_var_α
proc_LBL__roman_zw5s15_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n80_var_α
proc_LBL__roman_zw5s15_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n80_var_α
proc_LBL__roman_zw5s15_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n80_var_α
proc_LBL__roman_zw5s15_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n80_var_α
#=======================================================================================================================
#         v<13> = 1
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
                        mov              rax, qword ptr [rip + .Lx307_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n82_subscript_α
.Lx307_0:
                        .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n82_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx308_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s16_ω_d32
.Lx308_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n83_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n83_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx309_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n84_assign_var_α
.Lx309_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n84_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx310_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s16_ω_d64
.Lx310_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n85_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n85_statement_α:
                        add              rsp, 80
                                                                                        jmp   n86_lit_integer_α
proc_LBL__roman_zw5s16_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n86_lit_integer_α
proc_LBL__roman_zw5s16_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n86_lit_integer_α
proc_LBL__roman_zw5s16_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n86_lit_integer_α
proc_LBL__roman_zw5s16_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n86_lit_integer_α
proc_LBL__roman_zw5s16_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n86_lit_integer_α
#=======================================================================================================================
#         r = ARRAY(13)
#-----------------------------------------------------------------------------------------------------------------------
n86_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx313_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n87_call_α
.Lx313_0:
                        .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n87_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd315:            .string          "ARRAY"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd315]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx314_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s17_ω_d16
.Lx314_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n88_assign_α
n87_call_β:
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s17_ω_d16
#-----------------------------------------------------------------------------------------------------------------------
n88_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052352], rax                    # r
                        mov              qword ptr [1879052360], rdx
                                                                                        jmp   n89_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n89_statement_α:
                        add              rsp, 32
                                                                                        jmp   n90_var_α
proc_LBL__roman_zw5s17_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n90_var_α
proc_LBL__roman_zw5s17_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n90_var_α
proc_LBL__roman_zw5s17_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n90_var_α
#=======================================================================================================================
#         r<1> = 'M'
#-----------------------------------------------------------------------------------------------------------------------
n90_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n91_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n91_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx320_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n92_subscript_α
.Lx320_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n92_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx321_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s18_ω_d32
.Lx321_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n93_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n93_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx322_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n94_assign_var_α
.Lx322_0:
                        .quad            .Lx322_0_s
.Lx322_0_s:
                        .string          "M"
#-----------------------------------------------------------------------------------------------------------------------
n94_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx323_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s18_ω_d64
.Lx323_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n95_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n95_statement_α:
                        add              rsp, 80
                                                                                        jmp   n96_var_α
proc_LBL__roman_zw5s18_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n96_var_α
proc_LBL__roman_zw5s18_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n96_var_α
proc_LBL__roman_zw5s18_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n96_var_α
proc_LBL__roman_zw5s18_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n96_var_α
#=======================================================================================================================
#         r<2> = 'CM'
#-----------------------------------------------------------------------------------------------------------------------
n96_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n97_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n97_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx327_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n98_subscript_α
.Lx327_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n98_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx328_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s19_ω_d32
.Lx328_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n99_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n99_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx329_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n100_assign_var_α
.Lx329_0:
                        .quad            .Lx329_0_s
.Lx329_0_s:
                        .string          "CM"
#-----------------------------------------------------------------------------------------------------------------------
n100_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx330_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s19_ω_d64
.Lx330_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n101_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n101_statement_α:
                        add              rsp, 80
                                                                                        jmp   n102_var_α
proc_LBL__roman_zw5s19_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n102_var_α
proc_LBL__roman_zw5s19_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n102_var_α
proc_LBL__roman_zw5s19_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n102_var_α
proc_LBL__roman_zw5s19_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n102_var_α
proc_LBL__roman_zw5s19_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n102_var_α
#=======================================================================================================================
#         r<3> = 'D'
#-----------------------------------------------------------------------------------------------------------------------
n102_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n103_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n103_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx334_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n104_subscript_α
.Lx334_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n104_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx335_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s20_ω_d32
.Lx335_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n105_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n105_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx336_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n106_assign_var_α
.Lx336_0:
                        .quad            .Lx336_0_s
.Lx336_0_s:
                        .string          "D"
#-----------------------------------------------------------------------------------------------------------------------
n106_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx337_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s20_ω_d64
.Lx337_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n107_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n107_statement_α:
                        add              rsp, 80
                                                                                        jmp   n108_var_α
proc_LBL__roman_zw5s20_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n108_var_α
proc_LBL__roman_zw5s20_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n108_var_α
proc_LBL__roman_zw5s20_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n108_var_α
proc_LBL__roman_zw5s20_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n108_var_α
proc_LBL__roman_zw5s20_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n108_var_α
#=======================================================================================================================
#         r<4> = 'CD'
#-----------------------------------------------------------------------------------------------------------------------
n108_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n109_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n109_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx341_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n110_subscript_α
.Lx341_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n110_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx342_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s21_ω_d32
.Lx342_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n111_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n111_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx343_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n112_assign_var_α
.Lx343_0:
                        .quad            .Lx343_0_s
.Lx343_0_s:
                        .string          "CD"
#-----------------------------------------------------------------------------------------------------------------------
n112_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx344_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s21_ω_d64
.Lx344_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n113_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n113_statement_α:
                        add              rsp, 80
                                                                                        jmp   n114_var_α
proc_LBL__roman_zw5s21_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n114_var_α
proc_LBL__roman_zw5s21_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n114_var_α
proc_LBL__roman_zw5s21_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n114_var_α
proc_LBL__roman_zw5s21_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n114_var_α
proc_LBL__roman_zw5s21_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n114_var_α
#=======================================================================================================================
#         r<5> = 'C'
#-----------------------------------------------------------------------------------------------------------------------
n114_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n115_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n115_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx348_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n116_subscript_α
.Lx348_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n116_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx349_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s22_ω_d32
.Lx349_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n117_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n117_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx350_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n118_assign_var_α
.Lx350_0:
                        .quad            .Lx350_0_s
.Lx350_0_s:
                        .string          "C"
#-----------------------------------------------------------------------------------------------------------------------
n118_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx351_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s22_ω_d64
.Lx351_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n119_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n119_statement_α:
                        add              rsp, 80
                                                                                        jmp   n120_var_α
proc_LBL__roman_zw5s22_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n120_var_α
proc_LBL__roman_zw5s22_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n120_var_α
proc_LBL__roman_zw5s22_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n120_var_α
proc_LBL__roman_zw5s22_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n120_var_α
proc_LBL__roman_zw5s22_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n120_var_α
#=======================================================================================================================
#         r<6> = 'XC'
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
                        mov              rax, qword ptr [rip + .Lx355_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n122_subscript_α
.Lx355_0:
                        .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n122_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx356_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s23_ω_d32
.Lx356_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n123_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n123_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx357_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n124_assign_var_α
.Lx357_0:
                        .quad            .Lx357_0_s
.Lx357_0_s:
                        .string          "XC"
#-----------------------------------------------------------------------------------------------------------------------
n124_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx358_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s23_ω_d64
.Lx358_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n125_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n125_statement_α:
                        add              rsp, 80
                                                                                        jmp   n126_var_α
proc_LBL__roman_zw5s23_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n126_var_α
proc_LBL__roman_zw5s23_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n126_var_α
proc_LBL__roman_zw5s23_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n126_var_α
proc_LBL__roman_zw5s23_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n126_var_α
proc_LBL__roman_zw5s23_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n126_var_α
#=======================================================================================================================
#         r<7> = 'L'
#-----------------------------------------------------------------------------------------------------------------------
n126_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n127_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n127_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx362_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n128_subscript_α
.Lx362_0:
                        .quad            7
#-----------------------------------------------------------------------------------------------------------------------
n128_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx363_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s24_ω_d32
.Lx363_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n129_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n129_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx364_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n130_assign_var_α
.Lx364_0:
                        .quad            .Lx364_0_s
.Lx364_0_s:
                        .string          "L"
#-----------------------------------------------------------------------------------------------------------------------
n130_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx365_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s24_ω_d64
.Lx365_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n131_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n131_statement_α:
                        add              rsp, 80
                                                                                        jmp   n132_var_α
proc_LBL__roman_zw5s24_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n132_var_α
proc_LBL__roman_zw5s24_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n132_var_α
proc_LBL__roman_zw5s24_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n132_var_α
proc_LBL__roman_zw5s24_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n132_var_α
proc_LBL__roman_zw5s24_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n132_var_α
#=======================================================================================================================
#         r<8> = 'XL'
#-----------------------------------------------------------------------------------------------------------------------
n132_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n133_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n133_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx369_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n134_subscript_α
.Lx369_0:
                        .quad            8
#-----------------------------------------------------------------------------------------------------------------------
n134_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx370_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s25_ω_d32
.Lx370_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n135_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n135_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx371_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n136_assign_var_α
.Lx371_0:
                        .quad            .Lx371_0_s
.Lx371_0_s:
                        .string          "XL"
#-----------------------------------------------------------------------------------------------------------------------
n136_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx372_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s25_ω_d64
.Lx372_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n137_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n137_statement_α:
                        add              rsp, 80
                                                                                        jmp   n138_var_α
proc_LBL__roman_zw5s25_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n138_var_α
proc_LBL__roman_zw5s25_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n138_var_α
proc_LBL__roman_zw5s25_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n138_var_α
proc_LBL__roman_zw5s25_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n138_var_α
proc_LBL__roman_zw5s25_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n138_var_α
#=======================================================================================================================
#         r<9> = 'X'
#-----------------------------------------------------------------------------------------------------------------------
n138_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n139_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n139_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx376_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n140_subscript_α
.Lx376_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n140_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx377_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s26_ω_d32
.Lx377_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n141_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n141_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx378_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n142_assign_var_α
.Lx378_0:
                        .quad            .Lx378_0_s
.Lx378_0_s:
                        .string          "X"
#-----------------------------------------------------------------------------------------------------------------------
n142_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx379_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s26_ω_d64
.Lx379_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n143_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n143_statement_α:
                        add              rsp, 80
                                                                                        jmp   n144_var_α
proc_LBL__roman_zw5s26_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n144_var_α
proc_LBL__roman_zw5s26_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n144_var_α
proc_LBL__roman_zw5s26_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n144_var_α
proc_LBL__roman_zw5s26_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n144_var_α
proc_LBL__roman_zw5s26_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n144_var_α
#=======================================================================================================================
#         r<10> = 'IX'
#-----------------------------------------------------------------------------------------------------------------------
n144_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n145_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n145_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx383_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n146_subscript_α
.Lx383_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n146_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx384_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s27_ω_d32
.Lx384_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n147_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n147_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx385_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n148_assign_var_α
.Lx385_0:
                        .quad            .Lx385_0_s
.Lx385_0_s:
                        .string          "IX"
#-----------------------------------------------------------------------------------------------------------------------
n148_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx386_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s27_ω_d64
.Lx386_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n149_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n149_statement_α:
                        add              rsp, 80
                                                                                        jmp   n150_var_α
proc_LBL__roman_zw5s27_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n150_var_α
proc_LBL__roman_zw5s27_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n150_var_α
proc_LBL__roman_zw5s27_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n150_var_α
proc_LBL__roman_zw5s27_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n150_var_α
proc_LBL__roman_zw5s27_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n150_var_α
#=======================================================================================================================
#         r<11> = 'V'
#-----------------------------------------------------------------------------------------------------------------------
n150_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n151_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n151_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx390_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n152_subscript_α
.Lx390_0:
                        .quad            11
#-----------------------------------------------------------------------------------------------------------------------
n152_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx391_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s28_ω_d32
.Lx391_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n153_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n153_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx392_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n154_assign_var_α
.Lx392_0:
                        .quad            .Lx392_0_s
.Lx392_0_s:
                        .string          "V"
#-----------------------------------------------------------------------------------------------------------------------
n154_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx393_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s28_ω_d64
.Lx393_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n155_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n155_statement_α:
                        add              rsp, 80
                                                                                        jmp   n156_var_α
proc_LBL__roman_zw5s28_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n156_var_α
proc_LBL__roman_zw5s28_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n156_var_α
proc_LBL__roman_zw5s28_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n156_var_α
proc_LBL__roman_zw5s28_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n156_var_α
proc_LBL__roman_zw5s28_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n156_var_α
#=======================================================================================================================
#         r<12> = 'IV'
#-----------------------------------------------------------------------------------------------------------------------
n156_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n157_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n157_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx397_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n158_subscript_α
.Lx397_0:
                        .quad            12
#-----------------------------------------------------------------------------------------------------------------------
n158_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx398_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n162_var_α
.Lx398_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n159_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n159_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx399_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n160_assign_var_α
.Lx399_0:
                        .quad            .Lx399_0_s
.Lx399_0_s:
                        .string          "IV"
#-----------------------------------------------------------------------------------------------------------------------
n160_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx400_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n162_var_α
.Lx400_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n161_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n161_statement_α:
                        add              rsp, 80
                                                                                        jmp   n162_var_α
#=======================================================================================================================
#         r<13> = 'I'
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
                        mov              rax, qword ptr [rip + .Lx404_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n164_subscript_α
.Lx404_0:
                        .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n164_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx405_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n168_lit_integer_α
.Lx405_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n165_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n165_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx406_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n166_assign_var_α
.Lx406_0:
                        .quad            .Lx406_0_s
.Lx406_0_s:
                        .string          "I"
#-----------------------------------------------------------------------------------------------------------------------
n166_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx407_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n168_lit_integer_α
.Lx407_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n167_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n167_statement_α:
                        add              rsp, 80
                                                                                        jmp   n168_lit_integer_α
#=======================================================================================================================
#         i = 1
#-----------------------------------------------------------------------------------------------------------------------
n168_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx410_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n169_assign_α
.Lx410_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n169_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_integer
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052368], rax                    # i
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n170_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n170_statement_α:
                        add              rsp, 16
                                                                                        jmp   n171_var_α
proc_LBL__roman_zw5s31_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n171_var_α
proc_LBL__roman_zw5s31_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n171_var_α
#=======================================================================================================================
# RLOOP   GT(n, 0)                                                       :F(RDONE)
#-----------------------------------------------------------------------------------------------------------------------
n171_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # n
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n172_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n172_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx415_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n173_coerce_numeric_α
.Lx415_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n173_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # var
                        cmp              eax, 5
                                                                                        je    .Lx417_1
                        cmp              eax, 3
                                                                                        jne   .Lx417_0
                        mov              eax, dword ptr [rsp + 16]                      # lit_integer
                        cmp              eax, 3
                                                                                        jne   .Lx417_0
.Lx417_1:
                        mov              rax, qword ptr [rsp + 32]                      # var
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # var
                        mov              qword ptr [rsp + 8], rax                       # result
                                                                                        jmp   n174_coerce_numeric_α
.Lx417_0:
                        lea              rdi, [rsp + 32]                                # var
                        lea              rsi, [rsp + 16]                                # lit_integer
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 111                                       # codes
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n174_coerce_numeric_α
#-----------------------------------------------------------------------------------------------------------------------
n174_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # lit_integer
                        cmp              eax, 5
                                                                                        je    .Lx419_1
                        cmp              eax, 3
                                                                                        jne   .Lx419_0
                        mov              eax, dword ptr [rsp + 48]                      # var
                        cmp              eax, 3
                                                                                        jne   .Lx419_0
.Lx419_1:
                        mov              rax, qword ptr [rsp + 32]                      # lit_integer
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # lit_integer
                        mov              qword ptr [rsp + 8], rax                       # result
                                                                                        jmp   n175_cmp_test_α
.Lx419_0:
                        lea              rdi, [rsp + 32]                                # lit_integer
                        lea              rsi, [rsp + 48]                                # var
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 112                                       # codes
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n175_cmp_test_α
#-----------------------------------------------------------------------------------------------------------------------
n175_cmp_test_α:
                        sub              rsp, 16
                        lea              rdi, [rsp + 32]                                # coerce_numeric
                        lea              rsi, [rsp + 16]                                # b
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jg    .Lx421_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n207_var_α
.Lx421_240:
                                                                                        jmp   n176_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n176_statement_α:
                        add              rsp, 80
                                                                                        jmp   n177_var_α
#=======================================================================================================================
#         GE(n, v<i>)                                                    :F(RNEXT)
#-----------------------------------------------------------------------------------------------------------------------
n177_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # n
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n178_var_α
#-----------------------------------------------------------------------------------------------------------------------
n178_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n179_var_α
#-----------------------------------------------------------------------------------------------------------------------
n179_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052368]                    # i
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n180_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n180_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # idx
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx427_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s37_ω_d48
.Lx427_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n181_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n181_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # subscript
                        mov              rsi, qword ptr [rsp + 24]                      # d
                        call             rt_deref@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx428_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n202_var_α
.Lx428_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n182_coerce_numeric_α
#-----------------------------------------------------------------------------------------------------------------------
n182_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 80]                      # var
                        cmp              eax, 5
                                                                                        je    .Lx430_1
                        cmp              eax, 3
                                                                                        jne   .Lx430_0
                        mov              eax, dword ptr [rsp + 16]                      # deref
                        cmp              eax, 3
                                                                                        jne   .Lx430_0
.Lx430_1:
                        mov              rax, qword ptr [rsp + 80]                      # var
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 88]                      # var
                        mov              qword ptr [rsp + 8], rax                       # result
                                                                                        jmp   n183_coerce_numeric_α
.Lx430_0:
                        lea              rdi, [rsp + 80]                                # var
                        lea              rsi, [rsp + 16]                                # deref
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 109                                       # codes
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n183_coerce_numeric_α
#-----------------------------------------------------------------------------------------------------------------------
n183_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # deref
                        cmp              eax, 5
                                                                                        je    .Lx432_1
                        cmp              eax, 3
                                                                                        jne   .Lx432_0
                        mov              eax, dword ptr [rsp + 96]                      # var
                        cmp              eax, 3
                                                                                        jne   .Lx432_0
.Lx432_1:
                        mov              rax, qword ptr [rsp + 32]                      # deref
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # deref
                        mov              qword ptr [rsp + 8], rax                       # result
                                                                                        jmp   n184_cmp_test_α
.Lx432_0:
                        lea              rdi, [rsp + 32]                                # deref
                        lea              rsi, [rsp + 96]                                # var
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 110                                       # codes
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n184_cmp_test_α
#-----------------------------------------------------------------------------------------------------------------------
n184_cmp_test_α:
                        sub              rsp, 16
                        lea              rdi, [rsp + 32]                                # coerce_numeric
                        lea              rsi, [rsp + 16]                                # b
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jns   .Lx434_240
                        add              rsp, 16
                        add              rsp, 112
                                                                                        jmp   n202_var_α
.Lx434_240:
                                                                                        jmp   n185_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n185_statement_α:
                        add              rsp, 128
                                                                                        jmp   n186_var_α
#=======================================================================================================================
#         s = s r<i>
#-----------------------------------------------------------------------------------------------------------------------
n186_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # s
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n187_var_α
#-----------------------------------------------------------------------------------------------------------------------
n187_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n188_var_α
#-----------------------------------------------------------------------------------------------------------------------
n188_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052368]                    # i
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n189_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n189_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # idx
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx440_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s37_ω_d48
.Lx440_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n190_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n190_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # subscript
                        mov              rsi, qword ptr [rsp + 24]                      # d
                        call             rt_deref@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx441_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n194_var_α
.Lx441_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n191_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n191_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 80]                      # var
                        mov              rsi, qword ptr [rsp + 88]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # deref
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n192_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n192_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052320], rax                    # s
                        mov              qword ptr [1879052328], rdx
                                                                                        jmp   n193_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n193_statement_α:
                        add              rsp, 96
                                                                                        jmp   n194_var_α
#=======================================================================================================================
#         n = n - v<i>                                                   :(RLOOP)
#-----------------------------------------------------------------------------------------------------------------------
n194_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # n
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n195_var_α
#-----------------------------------------------------------------------------------------------------------------------
n195_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n196_var_α
#-----------------------------------------------------------------------------------------------------------------------
n196_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052368]                    # i
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n197_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n197_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # idx
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx449_240
                        add              rsp, 16
                                                                                        jmp   proc_LBL__roman_zw5s37_ω_d48
.Lx449_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n198_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n198_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # subscript
                        mov              rsi, qword ptr [rsp + 24]                      # d
                        call             rt_deref@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx450_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n171_var_α
.Lx450_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n199_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n199_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 80]                      # var
                        mov              rsi, qword ptr [rsp + 88]
                        mov              rdx, qword ptr [rsp + 16]                      # deref
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_sub@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx451_240
                        add              rsp, 16
                        add              rsp, 80
                                                                                        jmp   n171_var_α
.Lx451_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n200_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n200_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax                    # n
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   n201_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n201_statement_α:
                        add              rsp, 96
                                                                                        jmp   n171_var_α
#=======================================================================================================================
# RNEXT   i = i + 1                                                      :(RLOOP)
#-----------------------------------------------------------------------------------------------------------------------
n202_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052368]                    # i
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n203_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n203_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx456_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n204_binop_α
.Lx456_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n204_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_add@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx457_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n171_var_α
.Lx457_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n205_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n205_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052368], rax                    # i
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n206_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n206_statement_α:
                        add              rsp, 48
                                                                                        jmp   n171_var_α
#=======================================================================================================================
# RDONE   roman = s                                                      :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n207_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # s
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n208_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n208_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax                    # roman
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n209_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n209_statement_α:
                                                                                        jmp   n210_save_restore_α
proc_LBL__roman_zw5s37_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n210_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n210_save_restore_α:
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
n467_save_restore_α:
                        mov              rdi, rcx                                       # gw
                        mov              rsi, rdx                                       # ww
                        lea              rdx, [rsp + 0]                                 # rsp
                        mov              rcx, rbp                                       # rbp
                        call             rt_flat_wire_adopt@PLT
                                                                                        jmp   n468_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n468_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx472_0]                # name
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx472_1
.Lx472_0:
                        .quad            .Lx472_0_s
.Lx472_0_s:
                        .string          "roman"
.Lx472_1:
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
                        xor              esi, esi
                                                                                        jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 8
#=======================================================================================================================
#         DEFINE('roman(n)s,v,r,i')                                     :(roman_end)
#-----------------------------------------------------------------------------------------------------------------------
n473_statement_α:
                                                                                        jmp   n474_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n474_statement_α:
                                                                                        jmp   n475_lit_integer_α
#=======================================================================================================================
#         OUTPUT = roman(1)
#-----------------------------------------------------------------------------------------------------------------------
n475_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx718_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n476_call_α
.Lx718_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n476_call_α:
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
                        mov              rdi, qword ptr [rip + .Lx720_0]                # name
                        mov              esi, 5                                         # np
                        mov              edx, 1                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx720_5
                        lea              r10, [rip + g_call_args]
                        mov              rax, qword ptr [r10 + 0]
                        mov              qword ptr [1879052304], rax                    # n
                        mov              rax, qword ptr [r10 + 8]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx720_6]
                        lea              rdx, [rip + .Lx720_7]
                                                                                        jmp   rax
.Lx720_6:
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
                                                                                        jmp   .Lx720_2
.Lx720_7:
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
                                                                                        jmp   .Lx720_2
.Lx720_5:
                        add              rsp, 96
.Lx720_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx720_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s39_ω_d16
.Lx720_240:
                                                                                        jmp   n477_assign_α
n476_call_β:
                                                                                        jmp   main_zw5s39_ω_d16
.Lx720_0:
                        .quad            .Lx720_0_s
.Lx720_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n477_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx721_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n478_statement_α
.Lx721_0:
                        .quad            .Lx721_0_s
.Lx721_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n478_statement_α:
                        add              rsp, 32
                                                                                        jmp   n479_lit_integer_α
main_zw5s39_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n479_lit_integer_α
main_zw5s39_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n479_lit_integer_α
#=======================================================================================================================
#         OUTPUT = roman(4)
#-----------------------------------------------------------------------------------------------------------------------
n479_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx724_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n480_call_α
.Lx724_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n480_call_α:
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
                        mov              rdi, qword ptr [rip + .Lx726_0]                # name
                        mov              esi, 5                                         # np
                        mov              edx, 1                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx726_5
                        lea              r10, [rip + g_call_args]
                        mov              rax, qword ptr [r10 + 0]
                        mov              qword ptr [1879052304], rax                    # n
                        mov              rax, qword ptr [r10 + 8]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx726_6]
                        lea              rdx, [rip + .Lx726_7]
                                                                                        jmp   rax
.Lx726_6:
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
                                                                                        jmp   .Lx726_2
.Lx726_7:
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
                                                                                        jmp   .Lx726_2
.Lx726_5:
                        add              rsp, 96
.Lx726_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx726_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s40_ω_d16
.Lx726_240:
                                                                                        jmp   n481_assign_α
n480_call_β:
                                                                                        jmp   main_zw5s40_ω_d16
.Lx726_0:
                        .quad            .Lx726_0_s
.Lx726_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n481_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx727_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n482_statement_α
.Lx727_0:
                        .quad            .Lx727_0_s
.Lx727_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n482_statement_α:
                        add              rsp, 32
                                                                                        jmp   n483_lit_integer_α
main_zw5s40_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n483_lit_integer_α
main_zw5s40_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n483_lit_integer_α
#=======================================================================================================================
#         OUTPUT = roman(9)
#-----------------------------------------------------------------------------------------------------------------------
n483_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx730_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n484_call_α
.Lx730_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n484_call_α:
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
                        mov              rdi, qword ptr [rip + .Lx732_0]                # name
                        mov              esi, 5                                         # np
                        mov              edx, 1                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx732_5
                        lea              r10, [rip + g_call_args]
                        mov              rax, qword ptr [r10 + 0]
                        mov              qword ptr [1879052304], rax                    # n
                        mov              rax, qword ptr [r10 + 8]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx732_6]
                        lea              rdx, [rip + .Lx732_7]
                                                                                        jmp   rax
.Lx732_6:
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
                                                                                        jmp   .Lx732_2
.Lx732_7:
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
                                                                                        jmp   .Lx732_2
.Lx732_5:
                        add              rsp, 96
.Lx732_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx732_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s41_ω_d16
.Lx732_240:
                                                                                        jmp   n485_assign_α
n484_call_β:
                                                                                        jmp   main_zw5s41_ω_d16
.Lx732_0:
                        .quad            .Lx732_0_s
.Lx732_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n485_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx733_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n486_statement_α
.Lx733_0:
                        .quad            .Lx733_0_s
.Lx733_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n486_statement_α:
                        add              rsp, 32
                                                                                        jmp   n487_lit_integer_α
main_zw5s41_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n487_lit_integer_α
main_zw5s41_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n487_lit_integer_α
#=======================================================================================================================
#         OUTPUT = roman(42)
#-----------------------------------------------------------------------------------------------------------------------
n487_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx736_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n488_call_α
.Lx736_0:
                        .quad            42
#-----------------------------------------------------------------------------------------------------------------------
n488_call_α:
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
                        mov              rdi, qword ptr [rip + .Lx738_0]                # name
                        mov              esi, 5                                         # np
                        mov              edx, 1                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx738_5
                        lea              r10, [rip + g_call_args]
                        mov              rax, qword ptr [r10 + 0]
                        mov              qword ptr [1879052304], rax                    # n
                        mov              rax, qword ptr [r10 + 8]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx738_6]
                        lea              rdx, [rip + .Lx738_7]
                                                                                        jmp   rax
.Lx738_6:
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
                                                                                        jmp   .Lx738_2
.Lx738_7:
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
                                                                                        jmp   .Lx738_2
.Lx738_5:
                        add              rsp, 96
.Lx738_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx738_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s42_ω_d16
.Lx738_240:
                                                                                        jmp   n489_assign_α
n488_call_β:
                                                                                        jmp   main_zw5s42_ω_d16
.Lx738_0:
                        .quad            .Lx738_0_s
.Lx738_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n489_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx739_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n490_statement_α
.Lx739_0:
                        .quad            .Lx739_0_s
.Lx739_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n490_statement_α:
                        add              rsp, 32
                                                                                        jmp   n491_lit_integer_α
main_zw5s42_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n491_lit_integer_α
main_zw5s42_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n491_lit_integer_α
#=======================================================================================================================
#         OUTPUT = roman(1999)
#-----------------------------------------------------------------------------------------------------------------------
n491_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx742_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n492_call_α
.Lx742_0:
                        .quad            1999
#-----------------------------------------------------------------------------------------------------------------------
n492_call_α:
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
                        mov              rdi, qword ptr [rip + .Lx744_0]                # name
                        mov              esi, 5                                         # np
                        mov              edx, 1                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx744_5
                        lea              r10, [rip + g_call_args]
                        mov              rax, qword ptr [r10 + 0]
                        mov              qword ptr [1879052304], rax                    # n
                        mov              rax, qword ptr [r10 + 8]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx744_6]
                        lea              rdx, [rip + .Lx744_7]
                                                                                        jmp   rax
.Lx744_6:
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
                                                                                        jmp   .Lx744_2
.Lx744_7:
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
                                                                                        jmp   .Lx744_2
.Lx744_5:
                        add              rsp, 96
.Lx744_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx744_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s43_ω_d16
.Lx744_240:
                                                                                        jmp   n493_assign_α
n492_call_β:
                                                                                        jmp   main_zw5s43_ω_d16
.Lx744_0:
                        .quad            .Lx744_0_s
.Lx744_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n493_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx745_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n494_statement_α
.Lx745_0:
                        .quad            .Lx745_0_s
.Lx745_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n494_statement_α:
                        add              rsp, 32
                                                                                        jmp   n495_lit_integer_α
main_zw5s43_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n495_lit_integer_α
main_zw5s43_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n495_lit_integer_α
#=======================================================================================================================
#         OUTPUT = roman(2024)
#-----------------------------------------------------------------------------------------------------------------------
n495_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx748_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n496_call_α
.Lx748_0:
                        .quad            2024
#-----------------------------------------------------------------------------------------------------------------------
n496_call_α:
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
                        mov              rdi, qword ptr [rip + .Lx750_0]                # name
                        mov              esi, 5                                         # np
                        mov              edx, 1                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx750_5
                        lea              r10, [rip + g_call_args]
                        mov              rax, qword ptr [r10 + 0]
                        mov              qword ptr [1879052304], rax                    # n
                        mov              rax, qword ptr [r10 + 8]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx750_6]
                        lea              rdx, [rip + .Lx750_7]
                                                                                        jmp   rax
.Lx750_6:
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
                                                                                        jmp   .Lx750_2
.Lx750_7:
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
                                                                                        jmp   .Lx750_2
.Lx750_5:
                        add              rsp, 96
.Lx750_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx750_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s44_ω_d16
.Lx750_240:
                                                                                        jmp   n497_assign_α
n496_call_β:
                                                                                        jmp   main_zw5s44_ω_d16
.Lx750_0:
                        .quad            .Lx750_0_s
.Lx750_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n497_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx751_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n498_statement_α
.Lx751_0:
                        .quad            .Lx751_0_s
.Lx751_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n498_statement_α:
                        add              rsp, 32
                                                                                        jmp   main_γ
main_zw5s44_ω_d32:
                        add              rsp, 32
                                                                                        jmp   main_γ
main_zw5s44_ω_d16:
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n499_goto_α:
                                                                                        jmp   n500_lit_string_α
n499_goto_β:
                                                                                        jmp   main_ω
#=======================================================================================================================
# roman   s = ''
#-----------------------------------------------------------------------------------------------------------------------
n500_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx755_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n501_assign_α
.Lx755_0:
                        .quad            .Lx755_0_s
.Lx755_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n501_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052320], rax                    # s
                        mov              qword ptr [1879052328], rdx
                                                                                        jmp   n502_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n502_statement_α:
                        add              rsp, 16
                                                                                        jmp   n503_lit_integer_α
main_zw5s2_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n503_lit_integer_α
main_zw5s2_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n503_lit_integer_α
#=======================================================================================================================
#         v = ARRAY(13)
#-----------------------------------------------------------------------------------------------------------------------
n503_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx759_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n504_call_α
.Lx759_0:
                        .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n504_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd761:            .string          "ARRAY"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd761]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx760_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s3_ω_d16
.Lx760_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n505_assign_α
n504_call_β:
                        add              rsp, 16
                                                                                        jmp   main_zw5s3_ω_d16
#-----------------------------------------------------------------------------------------------------------------------
n505_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052336], rax                    # v
                        mov              qword ptr [1879052344], rdx
                                                                                        jmp   n506_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n506_statement_α:
                        add              rsp, 32
                                                                                        jmp   n507_var_α
main_zw5s3_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n507_var_α
main_zw5s3_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n507_var_α
#=======================================================================================================================
#         v<1> = 1000
#-----------------------------------------------------------------------------------------------------------------------
n507_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n508_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n508_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx766_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n509_subscript_α
.Lx766_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n509_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx767_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s4_ω_d32
.Lx767_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n510_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n510_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx768_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n511_assign_var_α
.Lx768_0:
                        .quad            1000
#-----------------------------------------------------------------------------------------------------------------------
n511_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx769_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s4_ω_d64
.Lx769_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n512_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n512_statement_α:
                        add              rsp, 80
                                                                                        jmp   n513_var_α
main_zw5s4_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n513_var_α
main_zw5s4_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n513_var_α
main_zw5s4_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n513_var_α
main_zw5s4_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n513_var_α
#=======================================================================================================================
#         v<2> = 900
#-----------------------------------------------------------------------------------------------------------------------
n513_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n514_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n514_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx773_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n515_subscript_α
.Lx773_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n515_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx774_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s5_ω_d32
.Lx774_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n516_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n516_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx775_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n517_assign_var_α
.Lx775_0:
                        .quad            900
#-----------------------------------------------------------------------------------------------------------------------
n517_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx776_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s5_ω_d64
.Lx776_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n518_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n518_statement_α:
                        add              rsp, 80
                                                                                        jmp   n519_var_α
main_zw5s5_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n519_var_α
main_zw5s5_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n519_var_α
main_zw5s5_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n519_var_α
main_zw5s5_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n519_var_α
main_zw5s5_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n519_var_α
#=======================================================================================================================
#         v<3> = 500
#-----------------------------------------------------------------------------------------------------------------------
n519_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n520_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n520_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx780_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n521_subscript_α
.Lx780_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n521_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx781_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s6_ω_d32
.Lx781_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n522_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n522_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx782_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n523_assign_var_α
.Lx782_0:
                        .quad            500
#-----------------------------------------------------------------------------------------------------------------------
n523_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx783_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s6_ω_d64
.Lx783_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n524_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n524_statement_α:
                        add              rsp, 80
                                                                                        jmp   n525_var_α
main_zw5s6_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n525_var_α
main_zw5s6_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n525_var_α
main_zw5s6_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n525_var_α
main_zw5s6_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n525_var_α
main_zw5s6_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n525_var_α
#=======================================================================================================================
#         v<4> = 400
#-----------------------------------------------------------------------------------------------------------------------
n525_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n526_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n526_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx787_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n527_subscript_α
.Lx787_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n527_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx788_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s7_ω_d32
.Lx788_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n528_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n528_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx789_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n529_assign_var_α
.Lx789_0:
                        .quad            400
#-----------------------------------------------------------------------------------------------------------------------
n529_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx790_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s7_ω_d64
.Lx790_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n530_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n530_statement_α:
                        add              rsp, 80
                                                                                        jmp   n531_var_α
main_zw5s7_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n531_var_α
main_zw5s7_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n531_var_α
main_zw5s7_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n531_var_α
main_zw5s7_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n531_var_α
main_zw5s7_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n531_var_α
#=======================================================================================================================
#         v<5> = 100
#-----------------------------------------------------------------------------------------------------------------------
n531_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n532_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n532_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx794_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n533_subscript_α
.Lx794_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n533_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx795_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s8_ω_d32
.Lx795_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n534_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n534_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx796_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n535_assign_var_α
.Lx796_0:
                        .quad            100
#-----------------------------------------------------------------------------------------------------------------------
n535_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx797_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s8_ω_d64
.Lx797_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n536_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n536_statement_α:
                        add              rsp, 80
                                                                                        jmp   n537_var_α
main_zw5s8_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n537_var_α
main_zw5s8_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n537_var_α
main_zw5s8_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n537_var_α
main_zw5s8_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n537_var_α
main_zw5s8_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n537_var_α
#=======================================================================================================================
#         v<6> = 90
#-----------------------------------------------------------------------------------------------------------------------
n537_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n538_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n538_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx801_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n539_subscript_α
.Lx801_0:
                        .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n539_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx802_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s9_ω_d32
.Lx802_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n540_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n540_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx803_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n541_assign_var_α
.Lx803_0:
                        .quad            90
#-----------------------------------------------------------------------------------------------------------------------
n541_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx804_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s9_ω_d64
.Lx804_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n542_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n542_statement_α:
                        add              rsp, 80
                                                                                        jmp   n543_var_α
main_zw5s9_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n543_var_α
main_zw5s9_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n543_var_α
main_zw5s9_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n543_var_α
main_zw5s9_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n543_var_α
main_zw5s9_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n543_var_α
#=======================================================================================================================
#         v<7> = 50
#-----------------------------------------------------------------------------------------------------------------------
n543_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n544_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n544_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx808_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n545_subscript_α
.Lx808_0:
                        .quad            7
#-----------------------------------------------------------------------------------------------------------------------
n545_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx809_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s10_ω_d32
.Lx809_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n546_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n546_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx810_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n547_assign_var_α
.Lx810_0:
                        .quad            50
#-----------------------------------------------------------------------------------------------------------------------
n547_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx811_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s10_ω_d64
.Lx811_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n548_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n548_statement_α:
                        add              rsp, 80
                                                                                        jmp   n549_var_α
main_zw5s10_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n549_var_α
main_zw5s10_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n549_var_α
main_zw5s10_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n549_var_α
main_zw5s10_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n549_var_α
main_zw5s10_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n549_var_α
#=======================================================================================================================
#         v<8> = 40
#-----------------------------------------------------------------------------------------------------------------------
n549_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n550_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n550_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx815_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n551_subscript_α
.Lx815_0:
                        .quad            8
#-----------------------------------------------------------------------------------------------------------------------
n551_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx816_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s11_ω_d32
.Lx816_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n552_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n552_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx817_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n553_assign_var_α
.Lx817_0:
                        .quad            40
#-----------------------------------------------------------------------------------------------------------------------
n553_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx818_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s11_ω_d64
.Lx818_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n554_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n554_statement_α:
                        add              rsp, 80
                                                                                        jmp   n555_var_α
main_zw5s11_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n555_var_α
main_zw5s11_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n555_var_α
main_zw5s11_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n555_var_α
main_zw5s11_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n555_var_α
main_zw5s11_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n555_var_α
#=======================================================================================================================
#         v<9> = 10
#-----------------------------------------------------------------------------------------------------------------------
n555_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n556_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n556_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx822_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n557_subscript_α
.Lx822_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n557_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx823_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s12_ω_d32
.Lx823_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n558_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n558_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx824_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n559_assign_var_α
.Lx824_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n559_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx825_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s12_ω_d64
.Lx825_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n560_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n560_statement_α:
                        add              rsp, 80
                                                                                        jmp   n561_var_α
main_zw5s12_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n561_var_α
main_zw5s12_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n561_var_α
main_zw5s12_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n561_var_α
main_zw5s12_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n561_var_α
main_zw5s12_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n561_var_α
#=======================================================================================================================
#         v<10> = 9
#-----------------------------------------------------------------------------------------------------------------------
n561_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n562_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n562_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx829_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n563_subscript_α
.Lx829_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n563_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx830_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s13_ω_d32
.Lx830_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n564_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n564_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx831_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n565_assign_var_α
.Lx831_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n565_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx832_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s13_ω_d64
.Lx832_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n566_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n566_statement_α:
                        add              rsp, 80
                                                                                        jmp   n567_var_α
main_zw5s13_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n567_var_α
main_zw5s13_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n567_var_α
main_zw5s13_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n567_var_α
main_zw5s13_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n567_var_α
main_zw5s13_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n567_var_α
#=======================================================================================================================
#         v<11> = 5
#-----------------------------------------------------------------------------------------------------------------------
n567_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n568_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n568_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx836_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n569_subscript_α
.Lx836_0:
                        .quad            11
#-----------------------------------------------------------------------------------------------------------------------
n569_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx837_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s14_ω_d32
.Lx837_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n570_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n570_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx838_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n571_assign_var_α
.Lx838_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n571_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx839_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s14_ω_d64
.Lx839_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n572_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n572_statement_α:
                        add              rsp, 80
                                                                                        jmp   n573_var_α
main_zw5s14_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n573_var_α
main_zw5s14_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n573_var_α
main_zw5s14_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n573_var_α
main_zw5s14_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n573_var_α
main_zw5s14_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n573_var_α
#=======================================================================================================================
#         v<12> = 4
#-----------------------------------------------------------------------------------------------------------------------
n573_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n574_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n574_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx843_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n575_subscript_α
.Lx843_0:
                        .quad            12
#-----------------------------------------------------------------------------------------------------------------------
n575_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx844_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s15_ω_d32
.Lx844_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n576_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n576_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx845_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n577_assign_var_α
.Lx845_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n577_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx846_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s15_ω_d64
.Lx846_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n578_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n578_statement_α:
                        add              rsp, 80
                                                                                        jmp   n579_var_α
main_zw5s15_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n579_var_α
main_zw5s15_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n579_var_α
main_zw5s15_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n579_var_α
main_zw5s15_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n579_var_α
main_zw5s15_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n579_var_α
#=======================================================================================================================
#         v<13> = 1
#-----------------------------------------------------------------------------------------------------------------------
n579_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n580_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n580_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx850_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n581_subscript_α
.Lx850_0:
                        .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n581_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx851_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s16_ω_d32
.Lx851_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n582_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n582_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx852_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n583_assign_var_α
.Lx852_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n583_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx853_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s16_ω_d64
.Lx853_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n584_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n584_statement_α:
                        add              rsp, 80
                                                                                        jmp   n585_lit_integer_α
main_zw5s16_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n585_lit_integer_α
main_zw5s16_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n585_lit_integer_α
main_zw5s16_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n585_lit_integer_α
main_zw5s16_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n585_lit_integer_α
main_zw5s16_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n585_lit_integer_α
#=======================================================================================================================
#         r = ARRAY(13)
#-----------------------------------------------------------------------------------------------------------------------
n585_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx856_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n586_call_α
.Lx856_0:
                        .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n586_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd858:            .string          "ARRAY"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd858]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx857_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s17_ω_d16
.Lx857_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n587_assign_α
n586_call_β:
                        add              rsp, 16
                                                                                        jmp   main_zw5s17_ω_d16
#-----------------------------------------------------------------------------------------------------------------------
n587_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052352], rax                    # r
                        mov              qword ptr [1879052360], rdx
                                                                                        jmp   n588_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n588_statement_α:
                        add              rsp, 32
                                                                                        jmp   n589_var_α
main_zw5s17_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n589_var_α
main_zw5s17_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n589_var_α
main_zw5s17_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n589_var_α
#=======================================================================================================================
#         r<1> = 'M'
#-----------------------------------------------------------------------------------------------------------------------
n589_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n590_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n590_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx863_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n591_subscript_α
.Lx863_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n591_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx864_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s18_ω_d32
.Lx864_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n592_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n592_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx865_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n593_assign_var_α
.Lx865_0:
                        .quad            .Lx865_0_s
.Lx865_0_s:
                        .string          "M"
#-----------------------------------------------------------------------------------------------------------------------
n593_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx866_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s18_ω_d64
.Lx866_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n594_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n594_statement_α:
                        add              rsp, 80
                                                                                        jmp   n595_var_α
main_zw5s18_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n595_var_α
main_zw5s18_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n595_var_α
main_zw5s18_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n595_var_α
main_zw5s18_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n595_var_α
#=======================================================================================================================
#         r<2> = 'CM'
#-----------------------------------------------------------------------------------------------------------------------
n595_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n596_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n596_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx870_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n597_subscript_α
.Lx870_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n597_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx871_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s19_ω_d32
.Lx871_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n598_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n598_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx872_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n599_assign_var_α
.Lx872_0:
                        .quad            .Lx872_0_s
.Lx872_0_s:
                        .string          "CM"
#-----------------------------------------------------------------------------------------------------------------------
n599_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx873_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s19_ω_d64
.Lx873_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n600_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n600_statement_α:
                        add              rsp, 80
                                                                                        jmp   n601_var_α
main_zw5s19_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n601_var_α
main_zw5s19_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n601_var_α
main_zw5s19_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n601_var_α
main_zw5s19_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n601_var_α
main_zw5s19_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n601_var_α
#=======================================================================================================================
#         r<3> = 'D'
#-----------------------------------------------------------------------------------------------------------------------
n601_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n602_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n602_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx877_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n603_subscript_α
.Lx877_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n603_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx878_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s20_ω_d32
.Lx878_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n604_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n604_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx879_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n605_assign_var_α
.Lx879_0:
                        .quad            .Lx879_0_s
.Lx879_0_s:
                        .string          "D"
#-----------------------------------------------------------------------------------------------------------------------
n605_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx880_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s20_ω_d64
.Lx880_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n606_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n606_statement_α:
                        add              rsp, 80
                                                                                        jmp   n607_var_α
main_zw5s20_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n607_var_α
main_zw5s20_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n607_var_α
main_zw5s20_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n607_var_α
main_zw5s20_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n607_var_α
main_zw5s20_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n607_var_α
#=======================================================================================================================
#         r<4> = 'CD'
#-----------------------------------------------------------------------------------------------------------------------
n607_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n608_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n608_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx884_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n609_subscript_α
.Lx884_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n609_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx885_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s21_ω_d32
.Lx885_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n610_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n610_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx886_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n611_assign_var_α
.Lx886_0:
                        .quad            .Lx886_0_s
.Lx886_0_s:
                        .string          "CD"
#-----------------------------------------------------------------------------------------------------------------------
n611_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx887_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s21_ω_d64
.Lx887_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n612_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n612_statement_α:
                        add              rsp, 80
                                                                                        jmp   n613_var_α
main_zw5s21_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n613_var_α
main_zw5s21_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n613_var_α
main_zw5s21_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n613_var_α
main_zw5s21_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n613_var_α
main_zw5s21_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n613_var_α
#=======================================================================================================================
#         r<5> = 'C'
#-----------------------------------------------------------------------------------------------------------------------
n613_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n614_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n614_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx891_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n615_subscript_α
.Lx891_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n615_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx892_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s22_ω_d32
.Lx892_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n616_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n616_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx893_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n617_assign_var_α
.Lx893_0:
                        .quad            .Lx893_0_s
.Lx893_0_s:
                        .string          "C"
#-----------------------------------------------------------------------------------------------------------------------
n617_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx894_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s22_ω_d64
.Lx894_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n618_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n618_statement_α:
                        add              rsp, 80
                                                                                        jmp   n619_var_α
main_zw5s22_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n619_var_α
main_zw5s22_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n619_var_α
main_zw5s22_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n619_var_α
main_zw5s22_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n619_var_α
main_zw5s22_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n619_var_α
#=======================================================================================================================
#         r<6> = 'XC'
#-----------------------------------------------------------------------------------------------------------------------
n619_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n620_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n620_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx898_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n621_subscript_α
.Lx898_0:
                        .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n621_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx899_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s23_ω_d32
.Lx899_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n622_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n622_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx900_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n623_assign_var_α
.Lx900_0:
                        .quad            .Lx900_0_s
.Lx900_0_s:
                        .string          "XC"
#-----------------------------------------------------------------------------------------------------------------------
n623_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx901_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s23_ω_d64
.Lx901_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n624_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n624_statement_α:
                        add              rsp, 80
                                                                                        jmp   n625_var_α
main_zw5s23_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n625_var_α
main_zw5s23_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n625_var_α
main_zw5s23_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n625_var_α
main_zw5s23_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n625_var_α
main_zw5s23_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n625_var_α
#=======================================================================================================================
#         r<7> = 'L'
#-----------------------------------------------------------------------------------------------------------------------
n625_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n626_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n626_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx905_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n627_subscript_α
.Lx905_0:
                        .quad            7
#-----------------------------------------------------------------------------------------------------------------------
n627_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx906_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s24_ω_d32
.Lx906_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n628_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n628_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx907_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n629_assign_var_α
.Lx907_0:
                        .quad            .Lx907_0_s
.Lx907_0_s:
                        .string          "L"
#-----------------------------------------------------------------------------------------------------------------------
n629_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx908_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s24_ω_d64
.Lx908_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n630_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n630_statement_α:
                        add              rsp, 80
                                                                                        jmp   n631_var_α
main_zw5s24_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n631_var_α
main_zw5s24_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n631_var_α
main_zw5s24_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n631_var_α
main_zw5s24_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n631_var_α
main_zw5s24_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n631_var_α
#=======================================================================================================================
#         r<8> = 'XL'
#-----------------------------------------------------------------------------------------------------------------------
n631_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n632_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n632_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx912_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n633_subscript_α
.Lx912_0:
                        .quad            8
#-----------------------------------------------------------------------------------------------------------------------
n633_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx913_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s25_ω_d32
.Lx913_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n634_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n634_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx914_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n635_assign_var_α
.Lx914_0:
                        .quad            .Lx914_0_s
.Lx914_0_s:
                        .string          "XL"
#-----------------------------------------------------------------------------------------------------------------------
n635_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx915_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s25_ω_d64
.Lx915_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n636_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n636_statement_α:
                        add              rsp, 80
                                                                                        jmp   n637_var_α
main_zw5s25_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n637_var_α
main_zw5s25_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n637_var_α
main_zw5s25_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n637_var_α
main_zw5s25_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n637_var_α
main_zw5s25_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n637_var_α
#=======================================================================================================================
#         r<9> = 'X'
#-----------------------------------------------------------------------------------------------------------------------
n637_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n638_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n638_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx919_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n639_subscript_α
.Lx919_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n639_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx920_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s26_ω_d32
.Lx920_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n640_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n640_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx921_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n641_assign_var_α
.Lx921_0:
                        .quad            .Lx921_0_s
.Lx921_0_s:
                        .string          "X"
#-----------------------------------------------------------------------------------------------------------------------
n641_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx922_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s26_ω_d64
.Lx922_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n642_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n642_statement_α:
                        add              rsp, 80
                                                                                        jmp   n643_var_α
main_zw5s26_ω_d64:
                        add              rsp, 64
                                                                                        jmp   n643_var_α
main_zw5s26_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n643_var_α
main_zw5s26_ω_d32:
                        add              rsp, 32
                                                                                        jmp   n643_var_α
main_zw5s26_ω_d16:
                        add              rsp, 16
                                                                                        jmp   n643_var_α
main_zw5s26_ω_d80:
                        add              rsp, 80
                                                                                        jmp   n643_var_α
#=======================================================================================================================
#         r<10> = 'IX'
#-----------------------------------------------------------------------------------------------------------------------
n643_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n644_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n644_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx926_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n645_subscript_α
.Lx926_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n645_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx927_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n649_var_α
.Lx927_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n646_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n646_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx928_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n647_assign_var_α
.Lx928_0:
                        .quad            .Lx928_0_s
.Lx928_0_s:
                        .string          "IX"
#-----------------------------------------------------------------------------------------------------------------------
n647_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx929_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n649_var_α
.Lx929_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n648_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n648_statement_α:
                        add              rsp, 80
                                                                                        jmp   n649_var_α
#=======================================================================================================================
#         r<11> = 'V'
#-----------------------------------------------------------------------------------------------------------------------
n649_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n650_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n650_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx933_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n651_subscript_α
.Lx933_0:
                        .quad            11
#-----------------------------------------------------------------------------------------------------------------------
n651_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx934_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n655_var_α
.Lx934_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n652_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n652_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx935_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n653_assign_var_α
.Lx935_0:
                        .quad            .Lx935_0_s
.Lx935_0_s:
                        .string          "V"
#-----------------------------------------------------------------------------------------------------------------------
n653_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx936_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n655_var_α
.Lx936_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n654_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n654_statement_α:
                        add              rsp, 80
                                                                                        jmp   n655_var_α
#=======================================================================================================================
#         r<12> = 'IV'
#-----------------------------------------------------------------------------------------------------------------------
n655_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n656_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n656_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx940_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n657_subscript_α
.Lx940_0:
                        .quad            12
#-----------------------------------------------------------------------------------------------------------------------
n657_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx941_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n661_var_α
.Lx941_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n658_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n658_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx942_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n659_assign_var_α
.Lx942_0:
                        .quad            .Lx942_0_s
.Lx942_0_s:
                        .string          "IV"
#-----------------------------------------------------------------------------------------------------------------------
n659_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx943_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n661_var_α
.Lx943_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n660_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n660_statement_α:
                        add              rsp, 80
                                                                                        jmp   n661_var_α
#=======================================================================================================================
#         r<13> = 'I'
#-----------------------------------------------------------------------------------------------------------------------
n661_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n662_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n662_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx947_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n663_subscript_α
.Lx947_0:
                        .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n663_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx948_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n667_lit_integer_α
.Lx948_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n664_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n664_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx949_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n665_assign_var_α
.Lx949_0:
                        .quad            .Lx949_0_s
.Lx949_0_s:
                        .string          "I"
#-----------------------------------------------------------------------------------------------------------------------
n665_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # subscript
                        mov              rsi, qword ptr [rsp + 40]                      # var
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # val
                        call             rt_assign_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx950_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n667_lit_integer_α
.Lx950_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n666_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n666_statement_α:
                        add              rsp, 80
                                                                                        jmp   n667_lit_integer_α
#=======================================================================================================================
#         i = 1
#-----------------------------------------------------------------------------------------------------------------------
n667_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx953_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n668_assign_α
.Lx953_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n668_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_integer
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052368], rax                    # i
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n669_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n669_statement_α:
                        add              rsp, 16
                                                                                        jmp   n670_var_α
#=======================================================================================================================
# RLOOP   GT(n, 0)                                                       :F(RDONE)
#-----------------------------------------------------------------------------------------------------------------------
n670_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # n
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n671_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n671_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx958_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n672_coerce_numeric_α
.Lx958_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n672_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # var
                        cmp              eax, 5
                                                                                        je    .Lx960_1
                        cmp              eax, 3
                                                                                        jne   .Lx960_0
                        mov              eax, dword ptr [rsp + 16]                      # lit_integer
                        cmp              eax, 3
                                                                                        jne   .Lx960_0
.Lx960_1:
                        mov              rax, qword ptr [rsp + 32]                      # var
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # var
                        mov              qword ptr [rsp + 8], rax                       # result
                                                                                        jmp   n673_coerce_numeric_α
.Lx960_0:
                        lea              rdi, [rsp + 32]                                # var
                        lea              rsi, [rsp + 16]                                # lit_integer
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 111                                       # codes
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n673_coerce_numeric_α
#-----------------------------------------------------------------------------------------------------------------------
n673_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # lit_integer
                        cmp              eax, 5
                                                                                        je    .Lx962_1
                        cmp              eax, 3
                                                                                        jne   .Lx962_0
                        mov              eax, dword ptr [rsp + 48]                      # var
                        cmp              eax, 3
                                                                                        jne   .Lx962_0
.Lx962_1:
                        mov              rax, qword ptr [rsp + 32]                      # lit_integer
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # lit_integer
                        mov              qword ptr [rsp + 8], rax                       # result
                                                                                        jmp   n674_cmp_test_α
.Lx962_0:
                        lea              rdi, [rsp + 32]                                # lit_integer
                        lea              rsi, [rsp + 48]                                # var
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 112                                       # codes
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n674_cmp_test_α
#-----------------------------------------------------------------------------------------------------------------------
n674_cmp_test_α:
                        sub              rsp, 16
                        lea              rdi, [rsp + 32]                                # coerce_numeric
                        lea              rsi, [rsp + 16]                                # b
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jg    .Lx964_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n706_var_α
.Lx964_240:
                                                                                        jmp   n675_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n675_statement_α:
                        add              rsp, 80
                                                                                        jmp   n676_var_α
#=======================================================================================================================
#         GE(n, v<i>)                                                    :F(RNEXT)
#-----------------------------------------------------------------------------------------------------------------------
n676_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # n
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n677_var_α
#-----------------------------------------------------------------------------------------------------------------------
n677_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n678_var_α
#-----------------------------------------------------------------------------------------------------------------------
n678_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052368]                    # i
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n679_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n679_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # idx
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx970_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s37_ω_d48
.Lx970_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n680_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n680_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # subscript
                        mov              rsi, qword ptr [rsp + 24]                      # d
                        call             rt_deref@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx971_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n701_var_α
.Lx971_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n681_coerce_numeric_α
#-----------------------------------------------------------------------------------------------------------------------
n681_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 80]                      # var
                        cmp              eax, 5
                                                                                        je    .Lx973_1
                        cmp              eax, 3
                                                                                        jne   .Lx973_0
                        mov              eax, dword ptr [rsp + 16]                      # deref
                        cmp              eax, 3
                                                                                        jne   .Lx973_0
.Lx973_1:
                        mov              rax, qword ptr [rsp + 80]                      # var
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 88]                      # var
                        mov              qword ptr [rsp + 8], rax                       # result
                                                                                        jmp   n682_coerce_numeric_α
.Lx973_0:
                        lea              rdi, [rsp + 80]                                # var
                        lea              rsi, [rsp + 16]                                # deref
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 109                                       # codes
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n682_coerce_numeric_α
#-----------------------------------------------------------------------------------------------------------------------
n682_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # deref
                        cmp              eax, 5
                                                                                        je    .Lx975_1
                        cmp              eax, 3
                                                                                        jne   .Lx975_0
                        mov              eax, dword ptr [rsp + 96]                      # var
                        cmp              eax, 3
                                                                                        jne   .Lx975_0
.Lx975_1:
                        mov              rax, qword ptr [rsp + 32]                      # deref
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # deref
                        mov              qword ptr [rsp + 8], rax                       # result
                                                                                        jmp   n683_cmp_test_α
.Lx975_0:
                        lea              rdi, [rsp + 32]                                # deref
                        lea              rsi, [rsp + 96]                                # var
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 110                                       # codes
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n683_cmp_test_α
#-----------------------------------------------------------------------------------------------------------------------
n683_cmp_test_α:
                        sub              rsp, 16
                        lea              rdi, [rsp + 32]                                # coerce_numeric
                        lea              rsi, [rsp + 16]                                # b
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jns   .Lx977_240
                        add              rsp, 16
                        add              rsp, 112
                                                                                        jmp   n701_var_α
.Lx977_240:
                                                                                        jmp   n684_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n684_statement_α:
                        add              rsp, 128
                                                                                        jmp   n685_var_α
#=======================================================================================================================
#         s = s r<i>
#-----------------------------------------------------------------------------------------------------------------------
n685_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # s
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n686_var_α
#-----------------------------------------------------------------------------------------------------------------------
n686_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # r
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n687_var_α
#-----------------------------------------------------------------------------------------------------------------------
n687_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052368]                    # i
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n688_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n688_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # idx
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx983_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s37_ω_d48
.Lx983_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n689_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n689_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # subscript
                        mov              rsi, qword ptr [rsp + 24]                      # d
                        call             rt_deref@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx984_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n693_var_α
.Lx984_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n690_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n690_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 80]                      # var
                        mov              rsi, qword ptr [rsp + 88]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # deref
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n691_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n691_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052320], rax                    # s
                        mov              qword ptr [1879052328], rdx
                                                                                        jmp   n692_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n692_statement_α:
                        add              rsp, 96
                                                                                        jmp   n693_var_α
#=======================================================================================================================
#         n = n - v<i>                                                   :(RLOOP)
#-----------------------------------------------------------------------------------------------------------------------
n693_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # n
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n694_var_α
#-----------------------------------------------------------------------------------------------------------------------
n694_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # v
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n695_var_α
#-----------------------------------------------------------------------------------------------------------------------
n695_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052368]                    # i
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n696_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n696_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # base
                        mov              rdx, qword ptr [rsp + 16]                      # idx
                        mov              rcx, qword ptr [rsp + 24]                      # idx
                        call             rt_subscript_var@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx992_240
                        add              rsp, 16
                                                                                        jmp   main_zw5s37_ω_d48
.Lx992_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n697_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n697_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # subscript
                        mov              rsi, qword ptr [rsp + 24]                      # d
                        call             rt_deref@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx993_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n670_var_α
.Lx993_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n698_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n698_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 80]                      # var
                        mov              rsi, qword ptr [rsp + 88]
                        mov              rdx, qword ptr [rsp + 16]                      # deref
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_sub@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx994_240
                        add              rsp, 16
                        add              rsp, 80
                                                                                        jmp   n670_var_α
.Lx994_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n699_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n699_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax                    # n
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   n700_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n700_statement_α:
                        add              rsp, 96
                                                                                        jmp   n670_var_α
#=======================================================================================================================
# RNEXT   i = i + 1                                                      :(RLOOP)
#-----------------------------------------------------------------------------------------------------------------------
n701_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052368]                    # i
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n702_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n702_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx999_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n703_binop_α
.Lx999_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n703_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_add@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx1000_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n670_var_α
.Lx1000_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n704_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n704_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052368], rax                    # i
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n705_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n705_statement_α:
                        add              rsp, 48
                                                                                        jmp   n670_var_α
#=======================================================================================================================
# RDONE   roman = s                                                      :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n706_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # s
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n707_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n707_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax                    # roman
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n708_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n708_statement_α:
                                                                                        jmp   n709_save_restore_α
main_zw5s37_ω_d48:
                        add              rsp, 48
                                                                                        jmp   n709_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n709_save_restore_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n710_goto_α:
                                                                                        jmp   n670_var_α
n710_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n711_goto_α:
                                                                                        jmp   n701_var_α
n711_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n712_goto_α:
                                                                                        jmp   n706_var_α
n712_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n713_goto_α:
                                                                                        jmp   n474_statement_α
n713_goto_β:
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

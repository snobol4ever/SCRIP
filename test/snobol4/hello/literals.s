                        .intel_syntax    noprefix
                        .text
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
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
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_begin_α:
                                                                                        jmp   n3_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n3_statement_end_α:
                                                                                        jmp   n4_statement_begin_α
#=======================================================================================================================
#       OUTPUT =
#-----------------------------------------------------------------------------------------------------------------------
n4_statement_begin_α:
                                                                                        jmp   n5_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n5_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx136_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n6_assign_α
.Lx136_0:
                        .quad            .Lx136_0_s
.Lx136_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n6_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx137_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n7_statement_end_α
.Lx137_0:
                        .quad            .Lx137_0_s
.Lx137_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n7_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n8_statement_begin_α
#=======================================================================================================================
#       OUTPUT = ""
#-----------------------------------------------------------------------------------------------------------------------
n8_statement_begin_α:
                                                                                        jmp   n9_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n9_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx142_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n10_assign_α
.Lx142_0:
                        .quad            .Lx142_0_s
.Lx142_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n10_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx143_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n11_statement_end_α
.Lx143_0:
                        .quad            .Lx143_0_s
.Lx143_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n11_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n12_statement_begin_α
#=======================================================================================================================
#       OUTPUT = "Hello World!"
#-----------------------------------------------------------------------------------------------------------------------
n12_statement_begin_α:
                                                                                        jmp   n13_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n13_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 12
                        mov              rax, qword ptr [rip + .Lx148_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n14_assign_α
.Lx148_0:
                        .quad            .Lx148_0_s
.Lx148_0_s:
                        .string          "Hello World!"
#-----------------------------------------------------------------------------------------------------------------------
n14_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx149_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n15_statement_end_α
.Lx149_0:
                        .quad            .Lx149_0_s
.Lx149_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n15_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n16_statement_begin_α
#=======================================================================================================================
#       OUTPUT = 0
#-----------------------------------------------------------------------------------------------------------------------
n16_statement_begin_α:
                                                                                        jmp   n17_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n17_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx154_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n18_assign_α
.Lx154_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n18_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # lit_integer
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx155_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n19_statement_end_α
.Lx155_0:
                        .quad            .Lx155_0_s
.Lx155_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n19_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n20_statement_begin_α
#=======================================================================================================================
#       OUTPUT = 1
#-----------------------------------------------------------------------------------------------------------------------
n20_statement_begin_α:
                                                                                        jmp   n21_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n21_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx160_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n22_assign_α
.Lx160_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n22_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # lit_integer
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx161_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n23_statement_end_α
.Lx161_0:
                        .quad            .Lx161_0_s
.Lx161_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n23_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n24_statement_begin_α
#=======================================================================================================================
#       OUTPUT = -1
#-----------------------------------------------------------------------------------------------------------------------
n24_statement_begin_α:
                                                                                        jmp   n25_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n25_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx166_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n26_unop_α
.Lx166_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n26_unop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # lit_integer
                        mov              rsi, qword ptr [rsp + 24]                      # a
                        call             rt_num_neg@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n27_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n27_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # unop
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx168_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n28_statement_end_α
.Lx168_0:
                        .quad            .Lx168_0_s
.Lx168_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n28_statement_end_α:
                        add              rsp, 32
                                                                                        jmp   n29_statement_begin_α
#=======================================================================================================================
#       OUTPUT = 1.0
#-----------------------------------------------------------------------------------------------------------------------
n29_statement_begin_α:
                                                                                        jmp   n30_lit_real_α
#-----------------------------------------------------------------------------------------------------------------------
n30_lit_real_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5                         # result
                        mov              rax, qword ptr [rip + .Lx173_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n31_assign_α
.Lx173_0:
                        .quad            4607182418800017408
#-----------------------------------------------------------------------------------------------------------------------
n31_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # lit_real
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx174_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n32_statement_end_α
.Lx174_0:
                        .quad            .Lx174_0_s
.Lx174_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n32_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n33_statement_begin_α
#=======================================================================================================================
#       OUTPUT = '1'
#-----------------------------------------------------------------------------------------------------------------------
n33_statement_begin_α:
                                                                                        jmp   n34_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n34_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx179_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n35_assign_α
.Lx179_0:
                        .quad            .Lx179_0_s
.Lx179_0_s:
                        .string          "1"
#-----------------------------------------------------------------------------------------------------------------------
n35_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx180_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n36_statement_end_α
.Lx180_0:
                        .quad            .Lx180_0_s
.Lx180_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n36_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n37_statement_begin_α
#=======================================================================================================================
#       OUTPUT = '1'
#-----------------------------------------------------------------------------------------------------------------------
n37_statement_begin_α:
                                                                                        jmp   n38_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n38_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx185_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n39_assign_α
.Lx185_0:
                        .quad            .Lx185_0_s
.Lx185_0_s:
                        .string          "1"
#-----------------------------------------------------------------------------------------------------------------------
n39_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx186_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n40_statement_end_α
.Lx186_0:
                        .quad            .Lx186_0_s
.Lx186_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n40_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n41_statement_begin_α
#=======================================================================================================================
#       OUTPUT = '1.0'
#-----------------------------------------------------------------------------------------------------------------------
n41_statement_begin_α:
                                                                                        jmp   n42_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n42_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx191_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n43_assign_α
.Lx191_0:
                        .quad            .Lx191_0_s
.Lx191_0_s:
                        .string          "1.0"
#-----------------------------------------------------------------------------------------------------------------------
n43_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx192_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n44_statement_end_α
.Lx192_0:
                        .quad            .Lx192_0_s
.Lx192_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n44_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n45_statement_begin_α
#=======================================================================================================================
#       OUTPUT = "I'm here"
#-----------------------------------------------------------------------------------------------------------------------
n45_statement_begin_α:
                                                                                        jmp   n46_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n46_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 8
                        mov              rax, qword ptr [rip + .Lx197_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n47_assign_α
.Lx197_0:
                        .quad            .Lx197_0_s
.Lx197_0_s:
                        .string          "I'm here"
#-----------------------------------------------------------------------------------------------------------------------
n47_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx198_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n48_statement_end_α
.Lx198_0:
                        .quad            .Lx198_0_s
.Lx198_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n48_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n49_statement_begin_α
#=======================================================================================================================
#       OUTPUT = '"Quote of the day"'
#-----------------------------------------------------------------------------------------------------------------------
n49_statement_begin_α:
                                                                                        jmp   n50_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n50_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 18
                        mov              rax, qword ptr [rip + .Lx203_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n51_assign_α
.Lx203_0:
                        .quad            .Lx203_0_s
.Lx203_0_s:
                        .string          "\"Quote of the day\""
#-----------------------------------------------------------------------------------------------------------------------
n51_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx204_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n52_statement_end_α
.Lx204_0:
                        .quad            .Lx204_0_s
.Lx204_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n52_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n53_statement_begin_α
#=======================================================================================================================
#       OUTPUT = '' + ''
#-----------------------------------------------------------------------------------------------------------------------
n53_statement_begin_α:
                                                                                        jmp   n54_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n54_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx209_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n55_lit_string_α
.Lx209_0:
                        .quad            .Lx209_0_s
.Lx209_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n55_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx210_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n56_binop_α
.Lx210_0:
                        .quad            .Lx210_0_s
.Lx210_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n56_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # lit_string
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_add@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx211_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n59_statement_begin_α
.Lx211_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n57_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n57_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx212_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n58_statement_end_α
.Lx212_0:
                        .quad            .Lx212_0_s
.Lx212_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n58_statement_end_α:
                        add              rsp, 48
                                                                                        jmp   n59_statement_begin_α
#=======================================================================================================================
#       OUTPUT = '' + 1
#-----------------------------------------------------------------------------------------------------------------------
n59_statement_begin_α:
                                                                                        jmp   n60_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n60_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx217_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n61_lit_integer_α
.Lx217_0:
                        .quad            .Lx217_0_s
.Lx217_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n61_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx218_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n62_binop_α
.Lx218_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n62_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # lit_string
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_add@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx219_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n65_statement_begin_α
.Lx219_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n63_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n63_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx220_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n64_statement_end_α
.Lx220_0:
                        .quad            .Lx220_0_s
.Lx220_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n64_statement_end_α:
                        add              rsp, 48
                                                                                        jmp   n65_statement_begin_α
#=======================================================================================================================
#       OUTPUT = 1 + ''
#-----------------------------------------------------------------------------------------------------------------------
n65_statement_begin_α:
                                                                                        jmp   n66_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n66_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx225_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n67_lit_string_α
.Lx225_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n67_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx226_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n68_binop_α
.Lx226_0:
                        .quad            .Lx226_0_s
.Lx226_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n68_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # lit_integer
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_add@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx227_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n71_statement_begin_α
.Lx227_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n69_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n69_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx228_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n70_statement_end_α
.Lx228_0:
                        .quad            .Lx228_0_s
.Lx228_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n70_statement_end_α:
                        add              rsp, 48
                                                                                        jmp   n71_statement_begin_α
#=======================================================================================================================
#       OUTPUT = '' ''
#-----------------------------------------------------------------------------------------------------------------------
n71_statement_begin_α:
                                                                                        jmp   n72_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n72_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx233_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n73_lit_string_α
.Lx233_0:
                        .quad            .Lx233_0_s
.Lx233_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n73_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx234_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n74_binop_α
.Lx234_0:
                        .quad            .Lx234_0_s
.Lx234_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n74_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # lit_string
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # b
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n75_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n75_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx236_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n76_statement_end_α
.Lx236_0:
                        .quad            .Lx236_0_s
.Lx236_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n76_statement_end_α:
                        add              rsp, 48
                                                                                        jmp   n77_statement_begin_α
#=======================================================================================================================
#       OUTPUT = '' 'Z'
#-----------------------------------------------------------------------------------------------------------------------
n77_statement_begin_α:
                                                                                        jmp   n78_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n78_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx241_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n79_lit_string_α
.Lx241_0:
                        .quad            .Lx241_0_s
.Lx241_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n79_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx242_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n80_binop_α
.Lx242_0:
                        .quad            .Lx242_0_s
.Lx242_0_s:
                        .string          "Z"
#-----------------------------------------------------------------------------------------------------------------------
n80_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # lit_string
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # b
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n81_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n81_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx244_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n82_statement_end_α
.Lx244_0:
                        .quad            .Lx244_0_s
.Lx244_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n82_statement_end_α:
                        add              rsp, 48
                                                                                        jmp   n83_statement_begin_α
#=======================================================================================================================
#       OUTPUT = 'A' ''
#-----------------------------------------------------------------------------------------------------------------------
n83_statement_begin_α:
                                                                                        jmp   n84_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n84_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx249_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n85_lit_string_α
.Lx249_0:
                        .quad            .Lx249_0_s
.Lx249_0_s:
                        .string          "A"
#-----------------------------------------------------------------------------------------------------------------------
n85_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx250_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n86_binop_α
.Lx250_0:
                        .quad            .Lx250_0_s
.Lx250_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n86_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # lit_string
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # b
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n87_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n87_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx252_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n88_statement_end_α
.Lx252_0:
                        .quad            .Lx252_0_s
.Lx252_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n88_statement_end_α:
                        add              rsp, 48
                                                                                        jmp   n89_statement_begin_α
#=======================================================================================================================
#       OUTPUT = 'A' 'Z'
#-----------------------------------------------------------------------------------------------------------------------
n89_statement_begin_α:
                                                                                        jmp   n90_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n90_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx257_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n91_lit_string_α
.Lx257_0:
                        .quad            .Lx257_0_s
.Lx257_0_s:
                        .string          "A"
#-----------------------------------------------------------------------------------------------------------------------
n91_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx258_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n92_binop_α
.Lx258_0:
                        .quad            .Lx258_0_s
.Lx258_0_s:
                        .string          "Z"
#-----------------------------------------------------------------------------------------------------------------------
n92_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # lit_string
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # b
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n93_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n93_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx260_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n94_statement_end_α
.Lx260_0:
                        .quad            .Lx260_0_s
.Lx260_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n94_statement_end_α:
                        add              rsp, 48
                                                                                        jmp   n95_statement_begin_α
#=======================================================================================================================
#       OUTPUT = 1 + 2
#-----------------------------------------------------------------------------------------------------------------------
n95_statement_begin_α:
                                                                                        jmp   n96_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n96_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx265_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n97_lit_integer_α
.Lx265_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n97_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx266_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n98_binop_α
.Lx266_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n98_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # lit_integer
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_add@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx267_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n101_statement_begin_α
.Lx267_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n99_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n99_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx268_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n100_statement_end_α
.Lx268_0:
                        .quad            .Lx268_0_s
.Lx268_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n100_statement_end_α:
                        add              rsp, 48
                                                                                        jmp   n101_statement_begin_α
#=======================================================================================================================
#       OUTPUT = 1 + 2 * 3
#-----------------------------------------------------------------------------------------------------------------------
n101_statement_begin_α:
                                                                                        jmp   n102_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n102_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx273_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n103_lit_integer_α
.Lx273_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n103_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx274_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n104_lit_integer_α
.Lx274_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n104_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx275_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n105_binop_α
.Lx275_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n105_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # lit_integer
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_mul@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx276_240
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n109_statement_begin_α
.Lx276_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n106_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n106_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 64]                      # lit_integer
                        mov              rsi, qword ptr [rsp + 72]
                        mov              rdx, qword ptr [rsp + 16]                      # binop
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_add@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx277_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n109_statement_begin_α
.Lx277_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n107_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n107_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx278_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n108_statement_end_α
.Lx278_0:
                        .quad            .Lx278_0_s
.Lx278_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n108_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n109_statement_begin_α
#=======================================================================================================================
#       OUTPUT = (1 + 2) * 3
#-----------------------------------------------------------------------------------------------------------------------
n109_statement_begin_α:
                                                                                        jmp   n110_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n110_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx283_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n111_lit_integer_α
.Lx283_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n111_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx284_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n112_binop_α
.Lx284_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n112_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # lit_integer
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_add@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx285_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n117_statement_begin_α
.Lx285_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n113_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n113_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx286_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n114_binop_α
.Lx286_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n114_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # binop
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_mul@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx287_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n117_statement_begin_α
.Lx287_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n115_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n115_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx288_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n116_statement_end_α
.Lx288_0:
                        .quad            .Lx288_0_s
.Lx288_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n116_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   n117_statement_begin_α
#=======================================================================================================================
#       OUTPUT = 1 + (2 * 3)
#-----------------------------------------------------------------------------------------------------------------------
n117_statement_begin_α:
                                                                                        jmp   n118_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n118_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx293_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n119_lit_integer_α
.Lx293_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n119_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx294_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n120_lit_integer_α
.Lx294_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n120_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx295_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n121_binop_α
.Lx295_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n121_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # lit_integer
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_mul@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx296_240
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   main_γ
.Lx296_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n122_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n122_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 64]                      # lit_integer
                        mov              rsi, qword ptr [rsp + 72]
                        mov              rdx, qword ptr [rsp + 16]                      # binop
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_add@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx297_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   main_γ
.Lx297_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n123_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n123_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx298_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n124_statement_end_α
.Lx298_0:
                        .quad            .Lx298_0_s
.Lx298_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n124_statement_end_α:
                        add              rsp, 80
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n125_goto_α:
                                                                                        jmp   n2_statement_begin_α
n125_goto_β:
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

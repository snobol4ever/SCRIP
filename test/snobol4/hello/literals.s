                        .intel_syntax    noprefix
                        .text
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        mov              r12, qword ptr [0x70000000]
                        call             rtcc_load_all@PLT
                        xor              esi, esi
                                                                              jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
#=======================================================================================================================
#         <stmt 1, line 1: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:                                                         jmp   n1_statement_end_α
n0_statement_begin_β:                                                         jmp   n2_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n1_statement_end_α:                                                           jmp   n2_statement_begin_α
#=======================================================================================================================
# START  <stmt 2, line 6: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_begin_α:                                                         jmp   n3_statement_end_α
n2_statement_begin_β:                                                         jmp   n4_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n3_statement_end_α:                                                           jmp   n4_statement_begin_α
#=======================================================================================================================
#       OUTPUT =
#-----------------------------------------------------------------------------------------------------------------------
n4_statement_begin_α:                                                         jmp   n5_lit_string_α
n4_statement_begin_β:                                                         jmp   n8_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n5_lit_string_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx135_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n6_assign_α
.Lx135_0:               .quad            .Lx135_0_s
.Lx135_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n6_assign_α:            mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx136_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n7_statement_end_α
.Lx136_0:               .quad            .Lx136_0_s
.Lx136_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n7_statement_end_α:     add              rsp, 16;                             jmp   n8_statement_begin_α
#=======================================================================================================================
#       OUTPUT = ""
#-----------------------------------------------------------------------------------------------------------------------
n8_statement_begin_α:                                                         jmp   n9_lit_string_α
n8_statement_begin_β:                                                         jmp   n12_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n9_lit_string_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx141_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n10_assign_α
.Lx141_0:               .quad            .Lx141_0_s
.Lx141_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n10_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx142_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n11_statement_end_α
.Lx142_0:               .quad            .Lx142_0_s
.Lx142_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n11_statement_end_α:    add              rsp, 16;                             jmp   n12_statement_begin_α
#=======================================================================================================================
#       OUTPUT = "Hello World!"
#-----------------------------------------------------------------------------------------------------------------------
n12_statement_begin_α:                                                        jmp   n13_lit_string_α
n12_statement_begin_β:                                                        jmp   n16_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n13_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 12
                        mov              rax, qword ptr [rip + .Lx147_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n14_assign_α
.Lx147_0:               .quad            .Lx147_0_s
.Lx147_0_s:             .string          "Hello World!"
#-----------------------------------------------------------------------------------------------------------------------
n14_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx148_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n15_statement_end_α
.Lx148_0:               .quad            .Lx148_0_s
.Lx148_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n15_statement_end_α:    add              rsp, 16;                             jmp   n16_statement_begin_α
#=======================================================================================================================
#       OUTPUT = 0
#-----------------------------------------------------------------------------------------------------------------------
n16_statement_begin_α:                                                        jmp   n17_lit_integer_α
n16_statement_begin_β:                                                        jmp   n20_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n17_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx153_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n18_assign_α
.Lx153_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n18_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_integer
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx154_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n19_statement_end_α
.Lx154_0:               .quad            .Lx154_0_s
.Lx154_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n19_statement_end_α:    add              rsp, 16;                             jmp   n20_statement_begin_α
#=======================================================================================================================
#       OUTPUT = 1
#-----------------------------------------------------------------------------------------------------------------------
n20_statement_begin_α:                                                        jmp   n21_lit_integer_α
n20_statement_begin_β:                                                        jmp   n24_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n21_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx159_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n22_assign_α
.Lx159_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n22_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_integer
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx160_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n23_statement_end_α
.Lx160_0:               .quad            .Lx160_0_s
.Lx160_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n23_statement_end_α:    add              rsp, 16;                             jmp   n24_statement_begin_α
#=======================================================================================================================
#       OUTPUT = -1
#-----------------------------------------------------------------------------------------------------------------------
n24_statement_begin_α:                                                        jmp   n25_lit_integer_α
n24_statement_begin_β:                                                        jmp   n29_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n25_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx165_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n26_unop_α
.Lx165_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n26_unop_α:             sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]            # lit_integer
                        mov              rsi, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_num_neg@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n27_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n27_assign_α:           mov              rsi, qword ptr [rsp + 0]             # unop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx167_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n28_statement_end_α
.Lx167_0:               .quad            .Lx167_0_s
.Lx167_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n28_statement_end_α:    add              rsp, 32;                             jmp   n29_statement_begin_α
#=======================================================================================================================
#       OUTPUT = 1.0
#-----------------------------------------------------------------------------------------------------------------------
n29_statement_begin_α:                                                        jmp   n30_lit_real_α
n29_statement_begin_β:                                                        jmp   n33_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n30_lit_real_α:         sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              rax, qword ptr [rip + .Lx172_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n31_assign_α
.Lx172_0:               .quad            4607182418800017408
#-----------------------------------------------------------------------------------------------------------------------
n31_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_real
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx173_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n32_statement_end_α
.Lx173_0:               .quad            .Lx173_0_s
.Lx173_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n32_statement_end_α:    add              rsp, 16;                             jmp   n33_statement_begin_α
#=======================================================================================================================
#       OUTPUT = '1'
#-----------------------------------------------------------------------------------------------------------------------
n33_statement_begin_α:                                                        jmp   n34_lit_string_α
n33_statement_begin_β:                                                        jmp   n37_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n34_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx178_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n35_assign_α
.Lx178_0:               .quad            .Lx178_0_s
.Lx178_0_s:             .string          "1"
#-----------------------------------------------------------------------------------------------------------------------
n35_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx179_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n36_statement_end_α
.Lx179_0:               .quad            .Lx179_0_s
.Lx179_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n36_statement_end_α:    add              rsp, 16;                             jmp   n37_statement_begin_α
#=======================================================================================================================
#       OUTPUT = '1'
#-----------------------------------------------------------------------------------------------------------------------
n37_statement_begin_α:                                                        jmp   n38_lit_string_α
n37_statement_begin_β:                                                        jmp   n41_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n38_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx184_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n39_assign_α
.Lx184_0:               .quad            .Lx184_0_s
.Lx184_0_s:             .string          "1"
#-----------------------------------------------------------------------------------------------------------------------
n39_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx185_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n40_statement_end_α
.Lx185_0:               .quad            .Lx185_0_s
.Lx185_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n40_statement_end_α:    add              rsp, 16;                             jmp   n41_statement_begin_α
#=======================================================================================================================
#       OUTPUT = '1.0'
#-----------------------------------------------------------------------------------------------------------------------
n41_statement_begin_α:                                                        jmp   n42_lit_string_α
n41_statement_begin_β:                                                        jmp   n45_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n42_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx190_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n43_assign_α
.Lx190_0:               .quad            .Lx190_0_s
.Lx190_0_s:             .string          "1.0"
#-----------------------------------------------------------------------------------------------------------------------
n43_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx191_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n44_statement_end_α
.Lx191_0:               .quad            .Lx191_0_s
.Lx191_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n44_statement_end_α:    add              rsp, 16;                             jmp   n45_statement_begin_α
#=======================================================================================================================
#       OUTPUT = "I'm here"
#-----------------------------------------------------------------------------------------------------------------------
n45_statement_begin_α:                                                        jmp   n46_lit_string_α
n45_statement_begin_β:                                                        jmp   n49_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n46_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 8
                        mov              rax, qword ptr [rip + .Lx196_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n47_assign_α
.Lx196_0:               .quad            .Lx196_0_s
.Lx196_0_s:             .string          "I'm here"
#-----------------------------------------------------------------------------------------------------------------------
n47_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx197_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n48_statement_end_α
.Lx197_0:               .quad            .Lx197_0_s
.Lx197_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n48_statement_end_α:    add              rsp, 16;                             jmp   n49_statement_begin_α
#=======================================================================================================================
#       OUTPUT = '"Quote of the day"'
#-----------------------------------------------------------------------------------------------------------------------
n49_statement_begin_α:                                                        jmp   n50_lit_string_α
n49_statement_begin_β:                                                        jmp   n53_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n50_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 18
                        mov              rax, qword ptr [rip + .Lx202_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n51_assign_α
.Lx202_0:               .quad            .Lx202_0_s
.Lx202_0_s:             .string          "\"Quote of the day\""
#-----------------------------------------------------------------------------------------------------------------------
n51_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx203_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n52_statement_end_α
.Lx203_0:               .quad            .Lx203_0_s
.Lx203_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n52_statement_end_α:    add              rsp, 16;                             jmp   n53_statement_begin_α
#=======================================================================================================================
#       OUTPUT = '' + ''
#-----------------------------------------------------------------------------------------------------------------------
n53_statement_begin_α:                                                        jmp   n54_lit_string_α
n53_statement_begin_β:                                                        jmp   n59_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n54_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx208_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n55_lit_string_α
.Lx208_0:               .quad            .Lx208_0_s
.Lx208_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n55_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx209_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n56_binop_α
n55_lit_string_β:       add              rsp, 16
                        add              rsp, 16;                             jmp   n53_statement_begin_β
.Lx209_0:               .quad            .Lx209_0_s
.Lx209_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n56_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # lit_string
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_add@PLT
                        cmp              eax, 104;                            jne   .Lx210_240
                        add              rsp, 16;                             jmp   n55_lit_string_β
.Lx210_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n57_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n57_assign_α:           mov              rsi, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx211_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n58_statement_end_α
.Lx211_0:               .quad            .Lx211_0_s
.Lx211_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n58_statement_end_α:    add              rsp, 48;                             jmp   n59_statement_begin_α
#=======================================================================================================================
#       OUTPUT = '' + 1
#-----------------------------------------------------------------------------------------------------------------------
n59_statement_begin_α:                                                        jmp   n60_lit_string_α
n59_statement_begin_β:                                                        jmp   n65_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n60_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx216_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n61_lit_integer_α
.Lx216_0:               .quad            .Lx216_0_s
.Lx216_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n61_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx217_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n62_binop_α
n61_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n59_statement_begin_β
.Lx217_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n62_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # lit_string
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_add@PLT
                        cmp              eax, 104;                            jne   .Lx218_240
                        add              rsp, 16;                             jmp   n61_lit_integer_β
.Lx218_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n63_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n63_assign_α:           mov              rsi, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx219_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n64_statement_end_α
.Lx219_0:               .quad            .Lx219_0_s
.Lx219_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n64_statement_end_α:    add              rsp, 48;                             jmp   n65_statement_begin_α
#=======================================================================================================================
#       OUTPUT = 1 + ''
#-----------------------------------------------------------------------------------------------------------------------
n65_statement_begin_α:                                                        jmp   n66_lit_integer_α
n65_statement_begin_β:                                                        jmp   n71_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n66_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx224_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n67_lit_string_α
.Lx224_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n67_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx225_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n68_binop_α
n67_lit_string_β:       add              rsp, 16
                        add              rsp, 16;                             jmp   n65_statement_begin_β
.Lx225_0:               .quad            .Lx225_0_s
.Lx225_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n68_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # lit_integer
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_add@PLT
                        cmp              eax, 104;                            jne   .Lx226_240
                        add              rsp, 16;                             jmp   n67_lit_string_β
.Lx226_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n69_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n69_assign_α:           mov              rsi, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx227_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n70_statement_end_α
.Lx227_0:               .quad            .Lx227_0_s
.Lx227_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n70_statement_end_α:    add              rsp, 48;                             jmp   n71_statement_begin_α
#=======================================================================================================================
#       OUTPUT = '' ''
#-----------------------------------------------------------------------------------------------------------------------
n71_statement_begin_α:                                                        jmp   n72_lit_string_α
n71_statement_begin_β:                                                        jmp   n77_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n72_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx232_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n73_lit_string_α
.Lx232_0:               .quad            .Lx232_0_s
.Lx232_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n73_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx233_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n74_binop_α
n73_lit_string_β:       add              rsp, 16
                        add              rsp, 16;                             jmp   n71_statement_begin_β
.Lx233_0:               .quad            .Lx233_0_s
.Lx233_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n74_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # lit_string
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n75_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n75_assign_α:           mov              rsi, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx235_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n76_statement_end_α
.Lx235_0:               .quad            .Lx235_0_s
.Lx235_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n76_statement_end_α:    add              rsp, 48;                             jmp   n77_statement_begin_α
#=======================================================================================================================
#       OUTPUT = '' 'Z'
#-----------------------------------------------------------------------------------------------------------------------
n77_statement_begin_α:                                                        jmp   n78_lit_string_α
n77_statement_begin_β:                                                        jmp   n83_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n78_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx240_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n79_lit_string_α
.Lx240_0:               .quad            .Lx240_0_s
.Lx240_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n79_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx241_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n80_binop_α
n79_lit_string_β:       add              rsp, 16
                        add              rsp, 16;                             jmp   n77_statement_begin_β
.Lx241_0:               .quad            .Lx241_0_s
.Lx241_0_s:             .string          "Z"
#-----------------------------------------------------------------------------------------------------------------------
n80_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # lit_string
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n81_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n81_assign_α:           mov              rsi, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx243_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n82_statement_end_α
.Lx243_0:               .quad            .Lx243_0_s
.Lx243_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n82_statement_end_α:    add              rsp, 48;                             jmp   n83_statement_begin_α
#=======================================================================================================================
#       OUTPUT = 'A' ''
#-----------------------------------------------------------------------------------------------------------------------
n83_statement_begin_α:                                                        jmp   n84_lit_string_α
n83_statement_begin_β:                                                        jmp   n89_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n84_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx248_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n85_lit_string_α
.Lx248_0:               .quad            .Lx248_0_s
.Lx248_0_s:             .string          "A"
#-----------------------------------------------------------------------------------------------------------------------
n85_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx249_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n86_binop_α
n85_lit_string_β:       add              rsp, 16
                        add              rsp, 16;                             jmp   n83_statement_begin_β
.Lx249_0:               .quad            .Lx249_0_s
.Lx249_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n86_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # lit_string
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n87_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n87_assign_α:           mov              rsi, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx251_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n88_statement_end_α
.Lx251_0:               .quad            .Lx251_0_s
.Lx251_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n88_statement_end_α:    add              rsp, 48;                             jmp   n89_statement_begin_α
#=======================================================================================================================
#       OUTPUT = 'A' 'Z'
#-----------------------------------------------------------------------------------------------------------------------
n89_statement_begin_α:                                                        jmp   n90_lit_string_α
n89_statement_begin_β:                                                        jmp   n95_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n90_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx256_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n91_lit_string_α
.Lx256_0:               .quad            .Lx256_0_s
.Lx256_0_s:             .string          "A"
#-----------------------------------------------------------------------------------------------------------------------
n91_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx257_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n92_binop_α
n91_lit_string_β:       add              rsp, 16
                        add              rsp, 16;                             jmp   n89_statement_begin_β
.Lx257_0:               .quad            .Lx257_0_s
.Lx257_0_s:             .string          "Z"
#-----------------------------------------------------------------------------------------------------------------------
n92_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # lit_string
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n93_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n93_assign_α:           mov              rsi, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx259_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n94_statement_end_α
.Lx259_0:               .quad            .Lx259_0_s
.Lx259_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n94_statement_end_α:    add              rsp, 48;                             jmp   n95_statement_begin_α
#=======================================================================================================================
#       OUTPUT = 1 + 2
#-----------------------------------------------------------------------------------------------------------------------
n95_statement_begin_α:                                                        jmp   n96_lit_integer_α
n95_statement_begin_β:                                                        jmp   n101_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n96_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx264_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n97_lit_integer_α
.Lx264_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n97_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx265_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n98_binop_α
n97_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n95_statement_begin_β
.Lx265_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n98_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # lit_integer
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_add@PLT
                        cmp              eax, 104;                            jne   .Lx266_240
                        add              rsp, 16;                             jmp   n97_lit_integer_β
.Lx266_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n99_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n99_assign_α:           mov              rsi, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx267_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n100_statement_end_α
.Lx267_0:               .quad            .Lx267_0_s
.Lx267_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n100_statement_end_α:   add              rsp, 48;                             jmp   n101_statement_begin_α
#=======================================================================================================================
#       OUTPUT = 1 + 2 * 3
#-----------------------------------------------------------------------------------------------------------------------
n101_statement_begin_α:                                                       jmp   n102_lit_integer_α
n101_statement_begin_β:                                                       jmp   n109_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n102_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx272_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n103_lit_integer_α
.Lx272_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n103_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx273_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n104_lit_integer_α
n103_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                             jmp   n101_statement_begin_β
.Lx273_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n104_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx274_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n105_binop_α
n104_lit_integer_β:     add              rsp, 16;                             jmp   n103_lit_integer_β
.Lx274_0:               .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n105_binop_α:           sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # lit_integer
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_mul@PLT
                        cmp              eax, 104;                            jne   .Lx275_240
                        add              rsp, 16;                             jmp   n104_lit_integer_β
.Lx275_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n106_binop_α
n105_binop_β:           add              rsp, 16;                             jmp   n104_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n106_binop_α:           sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 64]            # lit_integer
                        mov              rsi, qword ptr [rsp + 72]
                        mov              rdx, qword ptr [rsp + 16]            # binop
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_add@PLT
                        cmp              eax, 104;                            jne   .Lx276_240
                        add              rsp, 16;                             jmp   n105_binop_β
.Lx276_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n107_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n107_assign_α:          mov              rsi, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx277_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n108_statement_end_α
.Lx277_0:               .quad            .Lx277_0_s
.Lx277_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n108_statement_end_α:   add              rsp, 80;                             jmp   n109_statement_begin_α
#=======================================================================================================================
#       OUTPUT = (1 + 2) * 3
#-----------------------------------------------------------------------------------------------------------------------
n109_statement_begin_α:                                                       jmp   n110_lit_integer_α
n109_statement_begin_β:                                                       jmp   n117_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n110_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx282_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n111_lit_integer_α
.Lx282_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n111_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx283_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n112_binop_α
n111_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                             jmp   n109_statement_begin_β
.Lx283_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n112_binop_α:           sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # lit_integer
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_add@PLT
                        cmp              eax, 104;                            jne   .Lx284_240
                        add              rsp, 16;                             jmp   n111_lit_integer_β
.Lx284_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n113_lit_integer_α
n112_binop_β:           add              rsp, 16;                             jmp   n111_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n113_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx285_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n114_binop_α
n113_lit_integer_β:     add              rsp, 16;                             jmp   n112_binop_β
.Lx285_0:               .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n114_binop_α:           sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # binop
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_mul@PLT
                        cmp              eax, 104;                            jne   .Lx286_240
                        add              rsp, 16;                             jmp   n113_lit_integer_β
.Lx286_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n115_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n115_assign_α:          mov              rsi, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx287_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n116_statement_end_α
.Lx287_0:               .quad            .Lx287_0_s
.Lx287_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n116_statement_end_α:   add              rsp, 80;                             jmp   n117_statement_begin_α
#=======================================================================================================================
#       OUTPUT = 1 + (2 * 3)
#-----------------------------------------------------------------------------------------------------------------------
n117_statement_begin_α:                                                       jmp   n118_lit_integer_α
n117_statement_begin_β:                                                       jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n118_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx292_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n119_lit_integer_α
.Lx292_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n119_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx293_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n120_lit_integer_α
n119_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                             jmp   n117_statement_begin_β
.Lx293_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n120_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx294_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n121_binop_α
n120_lit_integer_β:     add              rsp, 16;                             jmp   n119_lit_integer_β
.Lx294_0:               .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n121_binop_α:           sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # lit_integer
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_mul@PLT
                        cmp              eax, 104;                            jne   .Lx295_240
                        add              rsp, 16;                             jmp   n120_lit_integer_β
.Lx295_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n122_binop_α
n121_binop_β:           add              rsp, 16;                             jmp   n120_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n122_binop_α:           sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 64]            # lit_integer
                        mov              rsi, qword ptr [rsp + 72]
                        mov              rdx, qword ptr [rsp + 16]            # binop
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_add@PLT
                        cmp              eax, 104;                            jne   .Lx296_240
                        add              rsp, 16;                             jmp   n121_binop_β
.Lx296_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n123_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n123_assign_α:          mov              rsi, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx297_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n124_statement_end_α
.Lx297_0:               .quad            .Lx297_0_s
.Lx297_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n124_statement_end_α:   add              rsp, 80;                             jmp   main_γ
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

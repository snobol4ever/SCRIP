                        .intel_syntax    noprefix
                        .text
                        .section         .rodata
.Lgvan0:                .string          "t"
.Lgvan1:                .string          "output"
.Lgvan2:                .string          "ta"
.Lgvan3:                .string          "ata"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .quad            .Lgvan1
                        .quad            .Lgvan2
                        .quad            .Lgvan3
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        mov              edi, 4
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 4
                        call             gva_register@PLT
                        xor              esi, esi
                        call             main_α
                        xor              eax, eax
                        add              rsp, 24
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_α:
                        .global          main_α
                        .global          main_β
                        .global          main_γ
                        .global          main_ω
                        sub              rsp, 1784
                        mov              rdi, rsp
                        mov              ecx, 1784
                        xor              eax, eax
                        rep stosb
main_α_body:
#=======================================================================================================================
#         t = table(10)
#-----------------------------------------------------------------------------------------------------------------------
n0_lit_integer_α:
                        mov              qword ptr [rsp + 64], 6
                        mov              rax, qword ptr [rip + .Lx98_0]
                        mov              qword ptr [rsp + 72], rax
                                                                                        jmp   n10_call_α
.Lx98_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n1_goto_α:
                                                                                        jmp   n11_var_α
n1_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n2_goto_α:
                                                                                        jmp   n12_var_α
n2_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n3_goto_α:
                                                                                        jmp   n13_var_α
n3_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n4_goto_α:
                                                                                        jmp   n14_var_α
n4_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n5_goto_α:
                                                                                        jmp   n15_var_α
n5_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n6_goto_α:
                                                                                        jmp   n16_var_α
n6_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n7_goto_α:
                                                                                        jmp   n17_var_α
n7_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n8_goto_α:
                                                                                        jmp   n18_lit_string_α
n8_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n9_goto_α:
                                                                                        jmp   main_γ
n9_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n10_call_α:
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        .section         .rodata
.Lrkfn109:              .string          "table"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn109]
                        lea              rsi, [rsp + 32]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rdx
                        cmp              eax, 99
                                                                                        je    n20_var_α
                                                                                        jmp   n19_assign_α
n10_call_β:
                                                                                        jmp   n20_var_α
#=======================================================================================================================
#         t<'cat'> = 'dog'
#-----------------------------------------------------------------------------------------------------------------------
n11_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 208], rax
                        mov              qword ptr [rsp + 216], rdx
                                                                                        jmp   n21_lit_string_α
#=======================================================================================================================
#         t<7> = 45
#-----------------------------------------------------------------------------------------------------------------------
n12_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 448], rax
                        mov              qword ptr [rsp + 456], rdx
                                                                                        jmp   n23_lit_integer_α
#=======================================================================================================================
#         differ(t<'cat'>, 'dog')                   :f(e004)
#-----------------------------------------------------------------------------------------------------------------------
n13_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 752], rax
                        mov              qword ptr [rsp + 760], rdx
                                                                                        jmp   n25_lit_string_α
#=======================================================================================================================
#         ta = convert(t, 'array')
#-----------------------------------------------------------------------------------------------------------------------
n14_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 912], rax
                        mov              qword ptr [rsp + 920], rdx
                                                                                        jmp   n26_lit_string_α
#=======================================================================================================================
#         ata = convert(ta, 'table')
#-----------------------------------------------------------------------------------------------------------------------
n15_var_α:
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 1168], rax
                        mov              qword ptr [rsp + 1176], rdx
                                                                                        jmp   n28_lit_string_α
#=======================================================================================================================
#         differ(ata<'cat'>, 'dog')                   :f(e007)
#-----------------------------------------------------------------------------------------------------------------------
n16_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 1424], rax
                        mov              qword ptr [rsp + 1432], rdx
                                                                                        jmp   n30_lit_string_α
#=======================================================================================================================
#         t['cat'] = 'fish'
#-----------------------------------------------------------------------------------------------------------------------
n17_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 1520], rax
                        mov              qword ptr [rsp + 1528], rdx
                                                                                        jmp   n31_lit_string_α
#=======================================================================================================================
#         output = 'PASS 1113_table (8/8)'
#-----------------------------------------------------------------------------------------------------------------------
n18_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 21
                        mov              rax, qword ptr [rip + .Lx117_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n33_assign_α
.Lx117_0:
                        .quad            .Lx117_0_s
.Lx117_0_s:
                        .string          "PASS 1113_table (8/8)"
#-----------------------------------------------------------------------------------------------------------------------
n19_assign_α:
                        mov              rax, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rsp + 24]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n20_var_α
#=======================================================================================================================
#         t = table(10)
#         differ(t<'cat'>)           :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n20_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 128], rax
                        mov              qword ptr [rsp + 136], rdx
                                                                                        jmp   n34_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n21_lit_string_α:
                        mov              qword ptr [rsp + 224], 1
                        mov              dword ptr [rsp + 228], 3
                        mov              rax, qword ptr [rip + .Lx120_0]
                        mov              qword ptr [rsp + 232], rax
                                                                                        jmp   n35_subscript_α
.Lx120_0:
                        .quad            .Lx120_0_s
.Lx120_0_s:
                        .string          "cat"
#=======================================================================================================================
#         differ(t<'cat'>, 'dog')                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n22_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 352], rax
                        mov              qword ptr [rsp + 360], rdx
                                                                                        jmp   n36_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n23_lit_integer_α:
                        mov              qword ptr [rsp + 464], 6
                        mov              rax, qword ptr [rip + .Lx122_0]
                        mov              qword ptr [rsp + 472], rax
                                                                                        jmp   n37_subscript_α
.Lx122_0:
                        .quad            7
#=======================================================================================================================
#         differ(t<7>, 45)                   :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n24_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 592], rax
                        mov              qword ptr [rsp + 600], rdx
                                                                                        jmp   n38_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n25_lit_string_α:
                        mov              qword ptr [rsp + 768], 1
                        mov              dword ptr [rsp + 772], 3
                        mov              rax, qword ptr [rip + .Lx124_0]
                        mov              qword ptr [rsp + 776], rax
                                                                                        jmp   n39_subscript_α
.Lx124_0:
                        .quad            .Lx124_0_s
.Lx124_0_s:
                        .string          "cat"
#-----------------------------------------------------------------------------------------------------------------------
n26_lit_string_α:
                        mov              qword ptr [rsp + 928], 1
                        mov              dword ptr [rsp + 932], 5
                        mov              rax, qword ptr [rip + .Lx125_0]
                        mov              qword ptr [rsp + 936], rax
                                                                                        jmp   n40_call_α
.Lx125_0:
                        .quad            .Lx125_0_s
.Lx125_0_s:
                        .string          "array"
#=======================================================================================================================
#         differ(prototype(ta), '2,2')                   :f(e005)
#-----------------------------------------------------------------------------------------------------------------------
n27_var_α:
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 1056], rax
                        mov              qword ptr [rsp + 1064], rdx
                                                                                        jmp   n41_call_α
#-----------------------------------------------------------------------------------------------------------------------
n28_lit_string_α:
                        mov              qword ptr [rsp + 1184], 1
                        mov              dword ptr [rsp + 1188], 5
                        mov              rax, qword ptr [rip + .Lx127_0]
                        mov              qword ptr [rsp + 1192], rax
                                                                                        jmp   n42_call_α
.Lx127_0:
                        .quad            .Lx127_0_s
.Lx127_0_s:
                        .string          "table"
#=======================================================================================================================
#         differ(ata<7>, 45)                   :f(e006)
#-----------------------------------------------------------------------------------------------------------------------
n29_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 1264], rax
                        mov              qword ptr [rsp + 1272], rdx
                                                                                        jmp   n43_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n30_lit_string_α:
                        mov              qword ptr [rsp + 1440], 1
                        mov              dword ptr [rsp + 1444], 3
                        mov              rax, qword ptr [rip + .Lx129_0]
                        mov              qword ptr [rsp + 1448], rax
                                                                                        jmp   n44_subscript_α
.Lx129_0:
                        .quad            .Lx129_0_s
.Lx129_0_s:
                        .string          "cat"
#-----------------------------------------------------------------------------------------------------------------------
n31_lit_string_α:
                        mov              qword ptr [rsp + 1536], 1
                        mov              dword ptr [rsp + 1540], 3
                        mov              rax, qword ptr [rip + .Lx130_0]
                        mov              qword ptr [rsp + 1544], rax
                                                                                        jmp   n45_subscript_α
.Lx130_0:
                        .quad            .Lx130_0_s
.Lx130_0_s:
                        .string          "cat"
#=======================================================================================================================
#         differ(t<'cat'>, 'fish')                   :f(e008)
#-----------------------------------------------------------------------------------------------------------------------
n32_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 1664], rax
                        mov              qword ptr [rsp + 1672], rdx
                                                                                        jmp   n46_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n33_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n34_lit_string_α:
                        mov              qword ptr [rsp + 144], 1
                        mov              dword ptr [rsp + 148], 3
                        mov              rax, qword ptr [rip + .Lx133_0]
                        mov              qword ptr [rsp + 152], rax
                                                                                        jmp   n47_subscript_α
.Lx133_0:
                        .quad            .Lx133_0_s
.Lx133_0_s:
                        .string          "cat"
#-----------------------------------------------------------------------------------------------------------------------
n35_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 208]
                        mov              rsi, qword ptr [rsp + 216]
                        mov              rdx, qword ptr [rsp + 224]
                        mov              rcx, qword ptr [rsp + 232]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx134_240
                        add              rsp, 16
                                                                                        jmp   n22_var_α
.Lx134_240:
                        mov              qword ptr [rsp + 240], rax
                        mov              qword ptr [rsp + 248], rdx
                        add              rsp, 16
                                                                                        jmp   n48_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n36_lit_string_α:
                        mov              qword ptr [rsp + 368], 1
                        mov              dword ptr [rsp + 372], 3
                        mov              rax, qword ptr [rip + .Lx135_0]
                        mov              qword ptr [rsp + 376], rax
                                                                                        jmp   n49_subscript_α
.Lx135_0:
                        .quad            .Lx135_0_s
.Lx135_0_s:
                        .string          "cat"
#-----------------------------------------------------------------------------------------------------------------------
n37_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 448]
                        mov              rsi, qword ptr [rsp + 456]
                        mov              rdx, qword ptr [rsp + 464]
                        mov              rcx, qword ptr [rsp + 472]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx136_240
                        add              rsp, 16
                                                                                        jmp   n24_var_α
.Lx136_240:
                        mov              qword ptr [rsp + 480], rax
                        mov              qword ptr [rsp + 488], rdx
                        add              rsp, 16
                                                                                        jmp   n50_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n38_lit_integer_α:
                        mov              qword ptr [rsp + 608], 6
                        mov              rax, qword ptr [rip + .Lx137_0]
                        mov              qword ptr [rsp + 616], rax
                                                                                        jmp   n51_subscript_α
.Lx137_0:
                        .quad            7
#-----------------------------------------------------------------------------------------------------------------------
n39_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 752]
                        mov              rsi, qword ptr [rsp + 760]
                        mov              rdx, qword ptr [rsp + 768]
                        mov              rcx, qword ptr [rsp + 776]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx138_240
                        add              rsp, 16
                                                                                        jmp   n14_var_α
.Lx138_240:
                        mov              qword ptr [rsp + 784], rax
                        mov              qword ptr [rsp + 792], rdx
                        add              rsp, 16
                                                                                        jmp   n52_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n40_call_α:
                        mov              rax, qword ptr [rsp + 912]
                        mov              qword ptr [rsp + 864], rax
                        mov              rax, qword ptr [rsp + 920]
                        mov              qword ptr [rsp + 872], rax
                        mov              rax, qword ptr [rsp + 928]
                        mov              qword ptr [rsp + 880], rax
                        mov              rax, qword ptr [rsp + 936]
                        mov              qword ptr [rsp + 888], rax
                        .section         .rodata
.Lbynamefn41:           .string          "convert"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn41]
                        lea              rsi, [rsp + 864]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 848], rax
                        mov              qword ptr [rsp + 856], rdx
                        cmp              eax, 99
                                                                                        je    n27_var_α
                                                                                        jmp   n53_assign_α
n40_call_β:
                                                                                        jmp   n27_var_α
#-----------------------------------------------------------------------------------------------------------------------
n41_call_α:
                        mov              rax, qword ptr [rsp + 1056]
                        mov              qword ptr [rsp + 1024], rax
                        mov              rax, qword ptr [rsp + 1064]
                        mov              qword ptr [rsp + 1032], rax
                        .section         .rodata
.Lbynamefn42:           .string          "prototype"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn42]
                        lea              rsi, [rsp + 1024]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1008], rax
                        mov              qword ptr [rsp + 1016], rdx
                        cmp              eax, 99
                                                                                        je    n15_var_α
                                                                                        jmp   n54_lit_string_α
n41_call_β:
                                                                                        jmp   n15_var_α
#-----------------------------------------------------------------------------------------------------------------------
n42_call_α:
                        mov              rax, qword ptr [rsp + 1168]
                        mov              qword ptr [rsp + 1120], rax
                        mov              rax, qword ptr [rsp + 1176]
                        mov              qword ptr [rsp + 1128], rax
                        mov              rax, qword ptr [rsp + 1184]
                        mov              qword ptr [rsp + 1136], rax
                        mov              rax, qword ptr [rsp + 1192]
                        mov              qword ptr [rsp + 1144], rax
                        .section         .rodata
.Lbynamefn43:           .string          "convert"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn43]
                        lea              rsi, [rsp + 1120]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1104], rax
                        mov              qword ptr [rsp + 1112], rdx
                        cmp              eax, 99
                                                                                        je    n29_var_α
                                                                                        jmp   n55_assign_α
n42_call_β:
                                                                                        jmp   n29_var_α
#-----------------------------------------------------------------------------------------------------------------------
n43_lit_integer_α:
                        mov              qword ptr [rsp + 1280], 6
                        mov              rax, qword ptr [rip + .Lx142_0]
                        mov              qword ptr [rsp + 1288], rax
                                                                                        jmp   n56_subscript_α
.Lx142_0:
                        .quad            7
#-----------------------------------------------------------------------------------------------------------------------
n44_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1424]
                        mov              rsi, qword ptr [rsp + 1432]
                        mov              rdx, qword ptr [rsp + 1440]
                        mov              rcx, qword ptr [rsp + 1448]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx143_240
                        add              rsp, 16
                                                                                        jmp   n17_var_α
.Lx143_240:
                        mov              qword ptr [rsp + 1456], rax
                        mov              qword ptr [rsp + 1464], rdx
                        add              rsp, 16
                                                                                        jmp   n57_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n45_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1520]
                        mov              rsi, qword ptr [rsp + 1528]
                        mov              rdx, qword ptr [rsp + 1536]
                        mov              rcx, qword ptr [rsp + 1544]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx144_240
                        add              rsp, 16
                                                                                        jmp   n32_var_α
.Lx144_240:
                        mov              qword ptr [rsp + 1552], rax
                        mov              qword ptr [rsp + 1560], rdx
                        add              rsp, 16
                                                                                        jmp   n58_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n46_lit_string_α:
                        mov              qword ptr [rsp + 1680], 1
                        mov              dword ptr [rsp + 1684], 3
                        mov              rax, qword ptr [rip + .Lx145_0]
                        mov              qword ptr [rsp + 1688], rax
                                                                                        jmp   n59_subscript_α
.Lx145_0:
                        .quad            .Lx145_0_s
.Lx145_0_s:
                        .string          "cat"
#-----------------------------------------------------------------------------------------------------------------------
n47_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 128]
                        mov              rsi, qword ptr [rsp + 136]
                        mov              rdx, qword ptr [rsp + 144]
                        mov              rcx, qword ptr [rsp + 152]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx146_240
                        add              rsp, 16
                                                                                        jmp   n11_var_α
.Lx146_240:
                        mov              qword ptr [rsp + 160], rax
                        mov              qword ptr [rsp + 168], rdx
                        add              rsp, 16
                                                                                        jmp   n60_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n48_lit_string_α:
                        mov              qword ptr [rsp + 256], 1
                        mov              dword ptr [rsp + 260], 3
                        mov              rax, qword ptr [rip + .Lx147_0]
                        mov              qword ptr [rsp + 264], rax
                                                                                        jmp   n61_assign_var_α
.Lx147_0:
                        .quad            .Lx147_0_s
.Lx147_0_s:
                        .string          "dog"
#-----------------------------------------------------------------------------------------------------------------------
n49_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 352]
                        mov              rsi, qword ptr [rsp + 360]
                        mov              rdx, qword ptr [rsp + 368]
                        mov              rcx, qword ptr [rsp + 376]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx148_240
                        add              rsp, 16
                                                                                        jmp   n12_var_α
.Lx148_240:
                        mov              qword ptr [rsp + 384], rax
                        mov              qword ptr [rsp + 392], rdx
                        add              rsp, 16
                                                                                        jmp   n62_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n50_lit_integer_α:
                        mov              qword ptr [rsp + 496], 6
                        mov              rax, qword ptr [rip + .Lx149_0]
                        mov              qword ptr [rsp + 504], rax
                                                                                        jmp   n63_assign_var_α
.Lx149_0:
                        .quad            45
#-----------------------------------------------------------------------------------------------------------------------
n51_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 592]
                        mov              rsi, qword ptr [rsp + 600]
                        mov              rdx, qword ptr [rsp + 608]
                        mov              rcx, qword ptr [rsp + 616]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx150_240
                        add              rsp, 16
                                                                                        jmp   n13_var_α
.Lx150_240:
                        mov              qword ptr [rsp + 624], rax
                        mov              qword ptr [rsp + 632], rdx
                        add              rsp, 16
                                                                                        jmp   n64_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n52_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 784]
                        mov              rsi, qword ptr [rsp + 792]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx151_240
                        add              rsp, 16
                                                                                        jmp   n14_var_α
.Lx151_240:
                        mov              qword ptr [rsp + 800], rax
                        mov              qword ptr [rsp + 808], rdx
                        add              rsp, 16
                                                                                        jmp   n65_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n53_assign_α:
                        mov              rax, qword ptr [rsp + 848]
                        mov              rdx, qword ptr [rsp + 856]
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rdx
                                                                                        jmp   n27_var_α
#-----------------------------------------------------------------------------------------------------------------------
n54_lit_string_α:
                        mov              qword ptr [rsp + 1072], 1
                        mov              dword ptr [rsp + 1076], 3
                        mov              rax, qword ptr [rip + .Lx153_0]
                        mov              qword ptr [rsp + 1080], rax
                                                                                        jmp   n66_call_α
.Lx153_0:
                        .quad            .Lx153_0_s
.Lx153_0_s:
                        .string          "2,2"
#-----------------------------------------------------------------------------------------------------------------------
n55_assign_α:
                        mov              rax, qword ptr [rsp + 1104]
                        mov              rdx, qword ptr [rsp + 1112]
                        mov              qword ptr [1879052336], rax
                        mov              qword ptr [1879052344], rdx
                                                                                        jmp   n29_var_α
#-----------------------------------------------------------------------------------------------------------------------
n56_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1264]
                        mov              rsi, qword ptr [rsp + 1272]
                        mov              rdx, qword ptr [rsp + 1280]
                        mov              rcx, qword ptr [rsp + 1288]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx155_240
                        add              rsp, 16
                                                                                        jmp   n16_var_α
.Lx155_240:
                        mov              qword ptr [rsp + 1296], rax
                        mov              qword ptr [rsp + 1304], rdx
                        add              rsp, 16
                                                                                        jmp   n67_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n57_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1456]
                        mov              rsi, qword ptr [rsp + 1464]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx156_240
                        add              rsp, 16
                                                                                        jmp   n17_var_α
.Lx156_240:
                        mov              qword ptr [rsp + 1472], rax
                        mov              qword ptr [rsp + 1480], rdx
                        add              rsp, 16
                                                                                        jmp   n68_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n58_lit_string_α:
                        mov              qword ptr [rsp + 1568], 1
                        mov              dword ptr [rsp + 1572], 4
                        mov              rax, qword ptr [rip + .Lx157_0]
                        mov              qword ptr [rsp + 1576], rax
                                                                                        jmp   n69_assign_var_α
.Lx157_0:
                        .quad            .Lx157_0_s
.Lx157_0_s:
                        .string          "fish"
#-----------------------------------------------------------------------------------------------------------------------
n59_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1664]
                        mov              rsi, qword ptr [rsp + 1672]
                        mov              rdx, qword ptr [rsp + 1680]
                        mov              rcx, qword ptr [rsp + 1688]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx158_240
                        add              rsp, 16
                                                                                        jmp   n18_lit_string_α
.Lx158_240:
                        mov              qword ptr [rsp + 1696], rax
                        mov              qword ptr [rsp + 1704], rdx
                        add              rsp, 16
                                                                                        jmp   n70_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n60_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 160]
                        mov              rsi, qword ptr [rsp + 168]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx159_240
                        add              rsp, 16
                                                                                        jmp   n11_var_α
.Lx159_240:
                        mov              qword ptr [rsp + 176], rax
                        mov              qword ptr [rsp + 184], rdx
                        add              rsp, 16
                                                                                        jmp   n71_call_α
#-----------------------------------------------------------------------------------------------------------------------
n61_assign_var_α:
                        mov              rdi, qword ptr [rsp + 240]
                        mov              rsi, qword ptr [rsp + 248]
                        mov              rdx, qword ptr [rsp + 256]
                        mov              rcx, qword ptr [rsp + 264]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n22_var_α
                        mov              qword ptr [rsp + 272], rax
                        mov              qword ptr [rsp + 280], rdx
                                                                                        jmp   n22_var_α
#-----------------------------------------------------------------------------------------------------------------------
n62_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 384]
                        mov              rsi, qword ptr [rsp + 392]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx161_240
                        add              rsp, 16
                                                                                        jmp   n12_var_α
.Lx161_240:
                        mov              qword ptr [rsp + 400], rax
                        mov              qword ptr [rsp + 408], rdx
                        add              rsp, 16
                                                                                        jmp   n72_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n63_assign_var_α:
                        mov              rdi, qword ptr [rsp + 480]
                        mov              rsi, qword ptr [rsp + 488]
                        mov              rdx, qword ptr [rsp + 496]
                        mov              rcx, qword ptr [rsp + 504]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n24_var_α
                        mov              qword ptr [rsp + 512], rax
                        mov              qword ptr [rsp + 520], rdx
                                                                                        jmp   n24_var_α
#-----------------------------------------------------------------------------------------------------------------------
n64_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 624]
                        mov              rsi, qword ptr [rsp + 632]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx163_240
                        add              rsp, 16
                                                                                        jmp   n13_var_α
.Lx163_240:
                        mov              qword ptr [rsp + 640], rax
                        mov              qword ptr [rsp + 648], rdx
                        add              rsp, 16
                                                                                        jmp   n73_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n65_lit_string_α:
                        mov              qword ptr [rsp + 816], 1
                        mov              dword ptr [rsp + 820], 3
                        mov              rax, qword ptr [rip + .Lx164_0]
                        mov              qword ptr [rsp + 824], rax
                                                                                        jmp   n74_call_α
.Lx164_0:
                        .quad            .Lx164_0_s
.Lx164_0_s:
                        .string          "dog"
#-----------------------------------------------------------------------------------------------------------------------
n66_call_α:
                        mov              rax, qword ptr [rsp + 1008]
                        mov              qword ptr [rsp + 960], rax
                        mov              rax, qword ptr [rsp + 1016]
                        mov              qword ptr [rsp + 968], rax
                        mov              rax, qword ptr [rsp + 1072]
                        mov              qword ptr [rsp + 976], rax
                        mov              rax, qword ptr [rsp + 1080]
                        mov              qword ptr [rsp + 984], rax
                        .section         .rodata
.Lbynamefn67:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn67]
                        lea              rsi, [rsp + 960]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 944], rax
                        mov              qword ptr [rsp + 952], rdx
                        cmp              eax, 99
                                                                                        je    n15_var_α
                                                                                        jmp   n75_lit_string_α
n66_call_β:
                                                                                        jmp   n15_var_α
#-----------------------------------------------------------------------------------------------------------------------
n67_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1296]
                        mov              rsi, qword ptr [rsp + 1304]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx166_240
                        add              rsp, 16
                                                                                        jmp   n16_var_α
.Lx166_240:
                        mov              qword ptr [rsp + 1312], rax
                        mov              qword ptr [rsp + 1320], rdx
                        add              rsp, 16
                                                                                        jmp   n76_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n68_lit_string_α:
                        mov              qword ptr [rsp + 1488], 1
                        mov              dword ptr [rsp + 1492], 3
                        mov              rax, qword ptr [rip + .Lx167_0]
                        mov              qword ptr [rsp + 1496], rax
                                                                                        jmp   n77_call_α
.Lx167_0:
                        .quad            .Lx167_0_s
.Lx167_0_s:
                        .string          "dog"
#-----------------------------------------------------------------------------------------------------------------------
n69_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1552]
                        mov              rsi, qword ptr [rsp + 1560]
                        mov              rdx, qword ptr [rsp + 1568]
                        mov              rcx, qword ptr [rsp + 1576]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n32_var_α
                        mov              qword ptr [rsp + 1584], rax
                        mov              qword ptr [rsp + 1592], rdx
                                                                                        jmp   n32_var_α
#-----------------------------------------------------------------------------------------------------------------------
n70_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1696]
                        mov              rsi, qword ptr [rsp + 1704]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx169_240
                        add              rsp, 16
                                                                                        jmp   n18_lit_string_α
.Lx169_240:
                        mov              qword ptr [rsp + 1712], rax
                        mov              qword ptr [rsp + 1720], rdx
                        add              rsp, 16
                                                                                        jmp   n78_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n71_call_α:
                        mov              rax, qword ptr [rsp + 176]
                        mov              qword ptr [rsp + 96], rax
                        mov              rax, qword ptr [rsp + 184]
                        mov              qword ptr [rsp + 104], rax
                        .section         .rodata
.Lbynamefn72:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn72]
                        lea              rsi, [rsp + 96]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 80], rax
                        mov              qword ptr [rsp + 88], rdx
                        cmp              eax, 99
                                                                                        je    n11_var_α
                                                                                        jmp   n79_lit_string_α
n71_call_β:
                                                                                        jmp   n11_var_α
#-----------------------------------------------------------------------------------------------------------------------
n72_lit_string_α:
                        mov              qword ptr [rsp + 416], 1
                        mov              dword ptr [rsp + 420], 3
                        mov              rax, qword ptr [rip + .Lx171_0]
                        mov              qword ptr [rsp + 424], rax
                                                                                        jmp   n80_call_α
.Lx171_0:
                        .quad            .Lx171_0_s
.Lx171_0_s:
                        .string          "dog"
#-----------------------------------------------------------------------------------------------------------------------
n73_lit_integer_α:
                        mov              qword ptr [rsp + 656], 6
                        mov              rax, qword ptr [rip + .Lx172_0]
                        mov              qword ptr [rsp + 664], rax
                                                                                        jmp   n81_call_α
.Lx172_0:
                        .quad            45
#-----------------------------------------------------------------------------------------------------------------------
n74_call_α:
                        mov              rax, qword ptr [rsp + 800]
                        mov              qword ptr [rsp + 704], rax
                        mov              rax, qword ptr [rsp + 808]
                        mov              qword ptr [rsp + 712], rax
                        mov              rax, qword ptr [rsp + 816]
                        mov              qword ptr [rsp + 720], rax
                        mov              rax, qword ptr [rsp + 824]
                        mov              qword ptr [rsp + 728], rax
                        .section         .rodata
.Lbynamefn75:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn75]
                        lea              rsi, [rsp + 704]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 688], rax
                        mov              qword ptr [rsp + 696], rdx
                        cmp              eax, 99
                                                                                        je    n14_var_α
                                                                                        jmp   n82_lit_string_α
n74_call_β:
                                                                                        jmp   n14_var_α
#=======================================================================================================================
#         output = 'FAIL 1113/005: table->array prototype 2,2' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n75_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 41
                        mov              rax, qword ptr [rip + .Lx174_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n83_assign_α
.Lx174_0:
                        .quad            .Lx174_0_s
.Lx174_0_s:
                        .string          "FAIL 1113/005: table->array prototype 2,2"
#-----------------------------------------------------------------------------------------------------------------------
n76_lit_integer_α:
                        mov              qword ptr [rsp + 1328], 6
                        mov              rax, qword ptr [rip + .Lx175_0]
                        mov              qword ptr [rsp + 1336], rax
                                                                                        jmp   n84_call_α
.Lx175_0:
                        .quad            45
#-----------------------------------------------------------------------------------------------------------------------
n77_call_α:
                        mov              rax, qword ptr [rsp + 1472]
                        mov              qword ptr [rsp + 1376], rax
                        mov              rax, qword ptr [rsp + 1480]
                        mov              qword ptr [rsp + 1384], rax
                        mov              rax, qword ptr [rsp + 1488]
                        mov              qword ptr [rsp + 1392], rax
                        mov              rax, qword ptr [rsp + 1496]
                        mov              qword ptr [rsp + 1400], rax
                        .section         .rodata
.Lbynamefn78:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn78]
                        lea              rsi, [rsp + 1376]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1360], rax
                        mov              qword ptr [rsp + 1368], rdx
                        cmp              eax, 99
                                                                                        je    n17_var_α
                                                                                        jmp   n85_lit_string_α
n77_call_β:
                                                                                        jmp   n17_var_α
#-----------------------------------------------------------------------------------------------------------------------
n78_lit_string_α:
                        mov              qword ptr [rsp + 1728], 1
                        mov              dword ptr [rsp + 1732], 4
                        mov              rax, qword ptr [rip + .Lx177_0]
                        mov              qword ptr [rsp + 1736], rax
                                                                                        jmp   n86_call_α
.Lx177_0:
                        .quad            .Lx177_0_s
.Lx177_0_s:
                        .string          "fish"
#=======================================================================================================================
#         output = 'FAIL 1113/001: absent key is null'   :(end)
#-----------------------------------------------------------------------------------------------------------------------
n79_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx178_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n87_assign_α
.Lx178_0:
                        .quad            .Lx178_0_s
.Lx178_0_s:
                        .string          "FAIL 1113/001: absent key is null"
#-----------------------------------------------------------------------------------------------------------------------
n80_call_α:
                        mov              rax, qword ptr [rsp + 400]
                        mov              qword ptr [rsp + 304], rax
                        mov              rax, qword ptr [rsp + 408]
                        mov              qword ptr [rsp + 312], rax
                        mov              rax, qword ptr [rsp + 416]
                        mov              qword ptr [rsp + 320], rax
                        mov              rax, qword ptr [rsp + 424]
                        mov              qword ptr [rsp + 328], rax
                        .section         .rodata
.Lbynamefn81:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn81]
                        lea              rsi, [rsp + 304]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 288], rax
                        mov              qword ptr [rsp + 296], rdx
                        cmp              eax, 99
                                                                                        je    n12_var_α
                                                                                        jmp   n88_lit_string_α
n80_call_β:
                                                                                        jmp   n12_var_α
#-----------------------------------------------------------------------------------------------------------------------
n81_call_α:
                        mov              rax, qword ptr [rsp + 640]
                        mov              qword ptr [rsp + 544], rax
                        mov              rax, qword ptr [rsp + 648]
                        mov              qword ptr [rsp + 552], rax
                        mov              rax, qword ptr [rsp + 656]
                        mov              qword ptr [rsp + 560], rax
                        mov              rax, qword ptr [rsp + 664]
                        mov              qword ptr [rsp + 568], rax
                        .section         .rodata
.Lbynamefn82:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn82]
                        lea              rsi, [rsp + 544]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 528], rax
                        mov              qword ptr [rsp + 536], rdx
                        cmp              eax, 99
                                                                                        je    n13_var_α
                                                                                        jmp   n89_lit_string_α
n81_call_β:
                                                                                        jmp   n13_var_α
#=======================================================================================================================
#         output = 'FAIL 1113/004: string key survives int key add' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n82_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 46
                        mov              rax, qword ptr [rip + .Lx181_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n90_assign_α
.Lx181_0:
                        .quad            .Lx181_0_s
.Lx181_0_s:
                        .string          "FAIL 1113/004: string key survives int key add"
#-----------------------------------------------------------------------------------------------------------------------
n83_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n84_call_α:
                        mov              rax, qword ptr [rsp + 1312]
                        mov              qword ptr [rsp + 1216], rax
                        mov              rax, qword ptr [rsp + 1320]
                        mov              qword ptr [rsp + 1224], rax
                        mov              rax, qword ptr [rsp + 1328]
                        mov              qword ptr [rsp + 1232], rax
                        mov              rax, qword ptr [rsp + 1336]
                        mov              qword ptr [rsp + 1240], rax
                        .section         .rodata
.Lbynamefn85:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn85]
                        lea              rsi, [rsp + 1216]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1200], rax
                        mov              qword ptr [rsp + 1208], rdx
                        cmp              eax, 99
                                                                                        je    n16_var_α
                                                                                        jmp   n91_lit_string_α
n84_call_β:
                                                                                        jmp   n16_var_α
#=======================================================================================================================
#         output = 'FAIL 1113/007: array->table string key roundtrip' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n85_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 48
                        mov              rax, qword ptr [rip + .Lx184_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n92_assign_α
.Lx184_0:
                        .quad            .Lx184_0_s
.Lx184_0_s:
                        .string          "FAIL 1113/007: array->table string key roundtrip"
#-----------------------------------------------------------------------------------------------------------------------
n86_call_α:
                        mov              rax, qword ptr [rsp + 1712]
                        mov              qword ptr [rsp + 1616], rax
                        mov              rax, qword ptr [rsp + 1720]
                        mov              qword ptr [rsp + 1624], rax
                        mov              rax, qword ptr [rsp + 1728]
                        mov              qword ptr [rsp + 1632], rax
                        mov              rax, qword ptr [rsp + 1736]
                        mov              qword ptr [rsp + 1640], rax
                        .section         .rodata
.Lbynamefn87:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn87]
                        lea              rsi, [rsp + 1616]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1600], rax
                        mov              qword ptr [rsp + 1608], rdx
                        cmp              eax, 99
                                                                                        je    n18_lit_string_α
                                                                                        jmp   n93_lit_string_α
n86_call_β:
                                                                                        jmp   n18_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n87_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   main_γ
#=======================================================================================================================
#         output = 'FAIL 1113/002: string key assign/read' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n88_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 37
                        mov              rax, qword ptr [rip + .Lx187_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n94_assign_α
.Lx187_0:
                        .quad            .Lx187_0_s
.Lx187_0_s:
                        .string          "FAIL 1113/002: string key assign/read"
#=======================================================================================================================
#         output = 'FAIL 1113/003: integer key assign/read' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n89_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 38
                        mov              rax, qword ptr [rip + .Lx188_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n95_assign_α
.Lx188_0:
                        .quad            .Lx188_0_s
.Lx188_0_s:
                        .string          "FAIL 1113/003: integer key assign/read"
#-----------------------------------------------------------------------------------------------------------------------
n90_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   main_γ
#=======================================================================================================================
#         output = 'FAIL 1113/006: array->table int key roundtrip' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n91_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 45
                        mov              rax, qword ptr [rip + .Lx190_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n96_assign_α
.Lx190_0:
                        .quad            .Lx190_0_s
.Lx190_0_s:
                        .string          "FAIL 1113/006: array->table int key roundtrip"
#-----------------------------------------------------------------------------------------------------------------------
n92_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   main_γ
#=======================================================================================================================
#         output = 'FAIL 1113/008: [] and <> syntax equivalent' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n93_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 42
                        mov              rax, qword ptr [rip + .Lx192_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n97_assign_α
.Lx192_0:
                        .quad            .Lx192_0_s
.Lx192_0_s:
                        .string          "FAIL 1113/008: [] and <> syntax equivalent"
#-----------------------------------------------------------------------------------------------------------------------
n94_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n95_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n96_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n97_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        add              rsp, 1784
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              eax, 99
                        xor              edx, edx
                        add              rsp, 1784
                        ret
                        .section         .note.GNU-stack,"",@progbits

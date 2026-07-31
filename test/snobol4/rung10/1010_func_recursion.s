                        .intel_syntax    noprefix
                        .text
                        .section         .rodata
.Lgvan0:                .string          "fact"
.Lgvan1:                .string          "n"
.Lgvan2:                .string          "output"
.Lgvan3:                .string          "facto"
.Lgvan4:                .string          "fact2_entry"
.Lgvan5:                .string          "fact2"
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
                        mov              edi, 6
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 6
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
                        sub              rsp, 1544
                        mov              rdi, rsp
                        mov              ecx, 1544
                        xor              eax, eax
                        rep stosb
main_α_body:
#=======================================================================================================================
#         define('fact(n)')                              :(fact_end)
#-----------------------------------------------------------------------------------------------------------------------
n0_lit_string_α:
                        mov              qword ptr [rsp + 144], 1
                        mov              dword ptr [rsp + 148], 7
                        mov              rax, qword ptr [rip + .Lx74_0]
                        mov              qword ptr [rsp + 152], rax
                                                                                        jmp   n1_call_α
.Lx74_0:
                        .quad            .Lx74_0_s
.Lx74_0_s:
                        .string          "fact(n)"
#-----------------------------------------------------------------------------------------------------------------------
n1_call_α:
                        mov              rax, qword ptr [rsp + 144]
                        mov              qword ptr [rsp + 112], rax
                        mov              rax, qword ptr [rsp + 152]
                        mov              qword ptr [rsp + 120], rax
                        .section         .rodata
.Lbynamefn2:            .string          "define"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn2]
                        lea              rsi, [rsp + 112]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 96], rax
                        mov              qword ptr [rsp + 104], rdx
                        cmp              eax, 99
                                                                                        je    n2_lit_integer_α
                                                                                        jmp   n2_lit_integer_α
n1_call_β:
                                                                                        jmp   n2_lit_integer_α
#=======================================================================================================================
#         ne(fact(5), 120)           :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n2_lit_integer_α:
                        mov              qword ptr [rsp + 544], 6
                        mov              rax, qword ptr [rip + .Lx76_0]
                        mov              qword ptr [rsp + 552], rax
                                                                                        jmp   n3_call_α
.Lx76_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n3_call_α:
                        mov              rax, qword ptr [rsp + 544]
                        mov              qword ptr [rsp + 512], rax
                        mov              rax, qword ptr [rsp + 552]
                        mov              qword ptr [rsp + 520], rax
                        .section         .rodata
.Lbynamefn4:            .string          "fact"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn4]
                        lea              rsi, [rsp + 512]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 496], rax
                        mov              qword ptr [rsp + 504], rdx
                        cmp              eax, 99
                                                                                        je    n8_lit_string_α
                                                                                        jmp   n4_lit_integer_α
n3_call_β:
                                                                                        jmp   n8_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n4_lit_integer_α:
                        mov              qword ptr [rsp + 560], 6
                        mov              rax, qword ptr [rip + .Lx78_0]
                        mov              qword ptr [rsp + 568], rax
                                                                                        jmp   n5_call_α
.Lx78_0:
                        .quad            120
#-----------------------------------------------------------------------------------------------------------------------
n5_call_α:
                        mov              rax, qword ptr [rsp + 496]
                        mov              qword ptr [rsp + 448], rax
                        mov              rax, qword ptr [rsp + 504]
                        mov              qword ptr [rsp + 456], rax
                        mov              rax, qword ptr [rsp + 560]
                        mov              qword ptr [rsp + 464], rax
                        mov              rax, qword ptr [rsp + 568]
                        mov              qword ptr [rsp + 472], rax
                        .section         .rodata
.Lbynamefn6:            .string          "ne"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn6]
                        lea              rsi, [rsp + 448]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 432], rax
                        mov              qword ptr [rsp + 440], rdx
                        cmp              eax, 99
                                                                                        je    n8_lit_string_α
                                                                                        jmp   n6_lit_string_α
n5_call_β:
                                                                                        jmp   n8_lit_string_α
#=======================================================================================================================
#         output = 'FAIL 1010/001: fact(5)=120'          :(end)
#-----------------------------------------------------------------------------------------------------------------------
n6_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 26
                        mov              rax, qword ptr [rip + .Lx80_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n7_assign_α
.Lx80_0:
                        .quad            .Lx80_0_s
.Lx80_0_s:
                        .string          "FAIL 1010/001: fact(5)=120"
#-----------------------------------------------------------------------------------------------------------------------
n7_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         differ(opsyn(.facto, 'fact'))                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n8_lit_string_α:
                        mov              qword ptr [rsp + 752], 1
                        mov              dword ptr [rsp + 756], 5
                        mov              rax, qword ptr [rip + .Lx82_0]
                        mov              qword ptr [rsp + 760], rax
                                                                                        jmp   n9_call_α
.Lx82_0:
                        .quad            .Lx82_0_s
.Lx82_0_s:
                        .string          "facto"
#-----------------------------------------------------------------------------------------------------------------------
n9_call_α:
                        mov              rax, qword ptr [rsp + 752]
                        mov              qword ptr [rsp + 720], rax
                        mov              rax, qword ptr [rsp + 760]
                        mov              qword ptr [rsp + 728], rax
                        .section         .rodata
.Lrkfn84:               .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn84]
                        lea              rsi, [rsp + 720]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 704], rax
                        mov              qword ptr [rsp + 712], rdx
                        cmp              eax, 99
                                                                                        je    n15_lit_integer_α
                                                                                        jmp   n10_lit_string_α
n9_call_β:
                                                                                        jmp   n15_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n10_lit_string_α:
                        mov              qword ptr [rsp + 768], 1
                        mov              dword ptr [rsp + 772], 4
                        mov              rax, qword ptr [rip + .Lx85_0]
                        mov              qword ptr [rsp + 776], rax
                                                                                        jmp   n11_call_α
.Lx85_0:
                        .quad            .Lx85_0_s
.Lx85_0_s:
                        .string          "fact"
#-----------------------------------------------------------------------------------------------------------------------
n11_call_α:
                        mov              rax, qword ptr [rsp + 704]
                        mov              qword ptr [rsp + 656], rax
                        mov              rax, qword ptr [rsp + 712]
                        mov              qword ptr [rsp + 664], rax
                        mov              rax, qword ptr [rsp + 768]
                        mov              qword ptr [rsp + 672], rax
                        mov              rax, qword ptr [rsp + 776]
                        mov              qword ptr [rsp + 680], rax
                        .section         .rodata
.Lbynamefn12:           .string          "opsyn"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn12]
                        lea              rsi, [rsp + 656]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 640], rax
                        mov              qword ptr [rsp + 648], rdx
                        cmp              eax, 99
                                                                                        je    n15_lit_integer_α
                                                                                        jmp   n12_call_α
n11_call_β:
                                                                                        jmp   n15_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n12_call_α:
                        mov              rax, qword ptr [rsp + 640]
                        mov              qword ptr [rsp + 608], rax
                        mov              rax, qword ptr [rsp + 648]
                        mov              qword ptr [rsp + 616], rax
                        .section         .rodata
.Lbynamefn13:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn13]
                        lea              rsi, [rsp + 608]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 592], rax
                        mov              qword ptr [rsp + 600], rdx
                        cmp              eax, 99
                                                                                        je    n15_lit_integer_α
                                                                                        jmp   n13_lit_string_α
n12_call_β:
                                                                                        jmp   n15_lit_integer_α
#=======================================================================================================================
#         output = 'FAIL 1010/002: opsyn alias'          :(end)
#-----------------------------------------------------------------------------------------------------------------------
n13_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 26
                        mov              rax, qword ptr [rip + .Lx88_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n14_assign_α
.Lx88_0:
                        .quad            .Lx88_0_s
.Lx88_0_s:
                        .string          "FAIL 1010/002: opsyn alias"
#-----------------------------------------------------------------------------------------------------------------------
n14_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         ne(facto(4), 24)           :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n15_lit_integer_α:
                        mov              qword ptr [rsp + 912], 6
                        mov              rax, qword ptr [rip + .Lx90_0]
                        mov              qword ptr [rsp + 920], rax
                                                                                        jmp   n16_call_α
.Lx90_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n16_call_α:
                        mov              rax, qword ptr [rsp + 912]
                        mov              qword ptr [rsp + 880], rax
                        mov              rax, qword ptr [rsp + 920]
                        mov              qword ptr [rsp + 888], rax
                        .section         .rodata
.Lbynamefn17:           .string          "facto"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn17]
                        lea              rsi, [rsp + 880]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 864], rax
                        mov              qword ptr [rsp + 872], rdx
                        cmp              eax, 99
                                                                                        je    n21_lit_string_α
                                                                                        jmp   n17_lit_integer_α
n16_call_β:
                                                                                        jmp   n21_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n17_lit_integer_α:
                        mov              qword ptr [rsp + 928], 6
                        mov              rax, qword ptr [rip + .Lx92_0]
                        mov              qword ptr [rsp + 936], rax
                                                                                        jmp   n18_call_α
.Lx92_0:
                        .quad            24
#-----------------------------------------------------------------------------------------------------------------------
n18_call_α:
                        mov              rax, qword ptr [rsp + 864]
                        mov              qword ptr [rsp + 816], rax
                        mov              rax, qword ptr [rsp + 872]
                        mov              qword ptr [rsp + 824], rax
                        mov              rax, qword ptr [rsp + 928]
                        mov              qword ptr [rsp + 832], rax
                        mov              rax, qword ptr [rsp + 936]
                        mov              qword ptr [rsp + 840], rax
                        .section         .rodata
.Lbynamefn19:           .string          "ne"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn19]
                        lea              rsi, [rsp + 816]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 800], rax
                        mov              qword ptr [rsp + 808], rdx
                        cmp              eax, 99
                                                                                        je    n21_lit_string_α
                                                                                        jmp   n19_lit_string_α
n18_call_β:
                                                                                        jmp   n21_lit_string_α
#=======================================================================================================================
#         output = 'FAIL 1010/003: facto(4)=24 via alias' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n19_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 36
                        mov              rax, qword ptr [rip + .Lx94_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n20_assign_α
.Lx94_0:
                        .quad            .Lx94_0_s
.Lx94_0_s:
                        .string          "FAIL 1010/003: facto(4)=24 via alias"
#-----------------------------------------------------------------------------------------------------------------------
n20_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         define('fact2(n)', .fact2_entry)               :(fact2_end)
#-----------------------------------------------------------------------------------------------------------------------
n21_lit_string_α:
                        mov              qword ptr [rsp + 1024], 1
                        mov              dword ptr [rsp + 1028], 8
                        mov              rax, qword ptr [rip + .Lx96_0]
                        mov              qword ptr [rsp + 1032], rax
                                                                                        jmp   n22_lit_string_α
.Lx96_0:
                        .quad            .Lx96_0_s
.Lx96_0_s:
                        .string          "fact2(n)"
#-----------------------------------------------------------------------------------------------------------------------
n22_lit_string_α:
                        mov              qword ptr [rsp + 1088], 1
                        mov              dword ptr [rsp + 1092], 11
                        mov              rax, qword ptr [rip + .Lx97_0]
                        mov              qword ptr [rsp + 1096], rax
                                                                                        jmp   n23_call_α
.Lx97_0:
                        .quad            .Lx97_0_s
.Lx97_0_s:
                        .string          "fact2_entry"
#-----------------------------------------------------------------------------------------------------------------------
n23_call_α:
                        mov              rax, qword ptr [rsp + 1088]
                        mov              qword ptr [rsp + 1056], rax
                        mov              rax, qword ptr [rsp + 1096]
                        mov              qword ptr [rsp + 1064], rax
                        .section         .rodata
.Lrkfn99:               .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn99]
                        lea              rsi, [rsp + 1056]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1040], rax
                        mov              qword ptr [rsp + 1048], rdx
                        cmp              eax, 99
                                                                                        je    n25_lit_integer_α
                                                                                        jmp   n24_call_α
n23_call_β:
                                                                                        jmp   n25_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n24_call_α:
                        mov              rax, qword ptr [rsp + 1024]
                        mov              qword ptr [rsp + 976], rax
                        mov              rax, qword ptr [rsp + 1032]
                        mov              qword ptr [rsp + 984], rax
                        mov              rax, qword ptr [rsp + 1040]
                        mov              qword ptr [rsp + 992], rax
                        mov              rax, qword ptr [rsp + 1048]
                        mov              qword ptr [rsp + 1000], rax
                        .section         .rodata
.Lbynamefn25:           .string          "define"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn25]
                        lea              rsi, [rsp + 976]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 960], rax
                        mov              qword ptr [rsp + 968], rdx
                        cmp              eax, 99
                                                                                        je    n25_lit_integer_α
                                                                                        jmp   n25_lit_integer_α
n24_call_β:
                                                                                        jmp   n25_lit_integer_α
#=======================================================================================================================
#         ne(fact2(6), 720)          :f(e004)
#-----------------------------------------------------------------------------------------------------------------------
n25_lit_integer_α:
                        mov              qword ptr [rsp + 1472], 6
                        mov              rax, qword ptr [rip + .Lx101_0]
                        mov              qword ptr [rsp + 1480], rax
                                                                                        jmp   n26_call_α
.Lx101_0:
                        .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n26_call_α:
                        mov              rax, qword ptr [rsp + 1472]
                        mov              qword ptr [rsp + 1440], rax
                        mov              rax, qword ptr [rsp + 1480]
                        mov              qword ptr [rsp + 1448], rax
                        .section         .rodata
.Lbynamefn27:           .string          "fact2"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn27]
                        lea              rsi, [rsp + 1440]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1424], rax
                        mov              qword ptr [rsp + 1432], rdx
                        cmp              eax, 99
                                                                                        je    n31_lit_string_α
                                                                                        jmp   n27_lit_integer_α
n26_call_β:
                                                                                        jmp   n31_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n27_lit_integer_α:
                        mov              qword ptr [rsp + 1488], 6
                        mov              rax, qword ptr [rip + .Lx103_0]
                        mov              qword ptr [rsp + 1496], rax
                                                                                        jmp   n28_call_α
.Lx103_0:
                        .quad            720
#-----------------------------------------------------------------------------------------------------------------------
n28_call_α:
                        mov              rax, qword ptr [rsp + 1424]
                        mov              qword ptr [rsp + 1376], rax
                        mov              rax, qword ptr [rsp + 1432]
                        mov              qword ptr [rsp + 1384], rax
                        mov              rax, qword ptr [rsp + 1488]
                        mov              qword ptr [rsp + 1392], rax
                        mov              rax, qword ptr [rsp + 1496]
                        mov              qword ptr [rsp + 1400], rax
                        .section         .rodata
.Lbynamefn29:           .string          "ne"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn29]
                        lea              rsi, [rsp + 1376]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1360], rax
                        mov              qword ptr [rsp + 1368], rdx
                        cmp              eax, 99
                                                                                        je    n31_lit_string_α
                                                                                        jmp   n29_lit_string_α
n28_call_β:
                                                                                        jmp   n31_lit_string_α
#=======================================================================================================================
#         output = 'FAIL 1010/004: fact2(6)=720 alt entry' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n29_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 37
                        mov              rax, qword ptr [rip + .Lx105_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n30_assign_α
.Lx105_0:
                        .quad            .Lx105_0_s
.Lx105_0_s:
                        .string          "FAIL 1010/004: fact2(6)=720 alt entry"
#-----------------------------------------------------------------------------------------------------------------------
n30_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         output = 'PASS 1010_func_recursion (4/4)'
#-----------------------------------------------------------------------------------------------------------------------
n31_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 30
                        mov              rax, qword ptr [rip + .Lx107_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n32_assign_α
.Lx107_0:
                        .quad            .Lx107_0_s
.Lx107_0_s:
                        .string          "PASS 1010_func_recursion (4/4)"
#-----------------------------------------------------------------------------------------------------------------------
n32_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n33_goto_α:
                                                                                        jmp   n34_var_α
n33_goto_β:
                                                                                        jmp   main_ω
#=======================================================================================================================
# fact    fact = eq(n, 1) 1                              :s(return)
#-----------------------------------------------------------------------------------------------------------------------
n34_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 256], rax
                        mov              qword ptr [rsp + 264], rdx
                                                                                        jmp   n35_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n35_lit_integer_α:
                        mov              qword ptr [rsp + 272], 6
                        mov              rax, qword ptr [rip + .Lx111_0]
                        mov              qword ptr [rsp + 280], rax
                                                                                        jmp   n36_call_α
.Lx111_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n36_call_α:
                        mov              rax, qword ptr [rsp + 256]
                        mov              qword ptr [rsp + 208], rax
                        mov              rax, qword ptr [rsp + 264]
                        mov              qword ptr [rsp + 216], rax
                        mov              rax, qword ptr [rsp + 272]
                        mov              qword ptr [rsp + 224], rax
                        mov              rax, qword ptr [rsp + 280]
                        mov              qword ptr [rsp + 232], rax
                        .section         .rodata
.Lbynamefn37:           .string          "eq"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn37]
                        lea              rsi, [rsp + 208]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 192], rax
                        mov              qword ptr [rsp + 200], rdx
                        cmp              eax, 99
                                                                                        je    n41_var_α
                                                                                        jmp   n37_lit_integer_α
n36_call_β:
                                                                                        jmp   n41_var_α
#-----------------------------------------------------------------------------------------------------------------------
n37_lit_integer_α:
                        mov              qword ptr [rsp + 288], 6
                        mov              rax, qword ptr [rip + .Lx113_0]
                        mov              qword ptr [rsp + 296], rax
                                                                                        jmp   n38_binop_α
.Lx113_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n38_binop_α:
                        mov              rdi, qword ptr [rsp + 192]
                        mov              rsi, qword ptr [rsp + 200]
                        mov              rdx, qword ptr [rsp + 288]
                        mov              rcx, qword ptr [rsp + 296]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 176], rax
                        mov              qword ptr [rsp + 184], rdx
                                                                                        jmp   n39_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n39_assign_α:
                        mov              rax, qword ptr [rsp + 176]
                        mov              rdx, qword ptr [rsp + 184]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n40_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n40_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx117_0]
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx117_1
.Lx117_0:
                        .quad            .Lx117_0_s
.Lx117_0_s:
                        .string          "return"
.Lx117_1:
                                                                                        jmp   main_γ
#=======================================================================================================================
#         fact = n * fact(n - 1)                         :(return)
#-----------------------------------------------------------------------------------------------------------------------
n41_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 320], rax
                        mov              qword ptr [rsp + 328], rdx
                                                                                        jmp   n42_var_α
#-----------------------------------------------------------------------------------------------------------------------
n42_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 400], rax
                        mov              qword ptr [rsp + 408], rdx
                                                                                        jmp   n43_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n43_lit_integer_α:
                        mov              qword ptr [rsp + 416], 6
                        mov              rax, qword ptr [rip + .Lx120_0]
                        mov              qword ptr [rsp + 424], rax
                                                                                        jmp   n44_binop_α
.Lx120_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n44_binop_α:
                        mov              eax, dword ptr [rsp + 400]
                        cmp              eax, 6
                                                                                        jne   .Lx121_0
                        mov              rax, qword ptr [rsp + 408]
                        mov              rcx, 1
                        sub              rax, rcx
                        mov              qword ptr [rsp + 384], 6
                        mov              qword ptr [rsp + 392], rax
                                                                                        jmp   n45_call_α
.Lx121_0:
                        mov              rdi, qword ptr [rsp + 400]
                        mov              rsi, qword ptr [rsp + 408]
                        mov              rdx, qword ptr [rsp + 416]
                        mov              rcx, qword ptr [rsp + 424]
                        call             rt_sub@PLT
                        cmp              eax, 99
                                                                                        je    n49_goto_deferred_α
                        mov              qword ptr [rsp + 384], rax
                        mov              qword ptr [rsp + 392], rdx
                                                                                        jmp   n45_call_α
#-----------------------------------------------------------------------------------------------------------------------
n45_call_α:
                        mov              rax, qword ptr [rsp + 384]
                        mov              qword ptr [rsp + 352], rax
                        mov              rax, qword ptr [rsp + 392]
                        mov              qword ptr [rsp + 360], rax
                        .section         .rodata
.Lbynamefn46:           .string          "fact"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn46]
                        lea              rsi, [rsp + 352]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 336], rax
                        mov              qword ptr [rsp + 344], rdx
                        cmp              eax, 99
                                                                                        je    n49_goto_deferred_α
                                                                                        jmp   n46_binop_α
n45_call_β:
                                                                                        jmp   n49_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n46_binop_α:
                        mov              eax, dword ptr [rsp + 320]
                        cmp              eax, 6
                                                                                        jne   .Lx123_0
                        mov              eax, dword ptr [rsp + 336]
                        cmp              eax, 6
                                                                                        jne   .Lx123_0
                        mov              rax, qword ptr [rsp + 328]
                        mov              rcx, qword ptr [rsp + 344]
                        imul             rax, rcx
                        mov              qword ptr [rsp + 304], 6
                        mov              qword ptr [rsp + 312], rax
                                                                                        jmp   n47_assign_α
.Lx123_0:
                        mov              rdi, qword ptr [rsp + 320]
                        mov              rsi, qword ptr [rsp + 328]
                        mov              rdx, qword ptr [rsp + 336]
                        mov              rcx, qword ptr [rsp + 344]
                        call             rt_mul@PLT
                        cmp              eax, 99
                                                                                        je    n49_goto_deferred_α
                        mov              qword ptr [rsp + 304], rax
                        mov              qword ptr [rsp + 312], rdx
                                                                                        jmp   n47_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n47_assign_α:
                        mov              rax, qword ptr [rsp + 304]
                        mov              rdx, qword ptr [rsp + 312]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n48_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n48_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx126_0]
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx126_1
.Lx126_0:
                        .quad            .Lx126_0_s
.Lx126_0_s:
                        .string          "return"
.Lx126_1:
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n49_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx128_0]
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx128_1
.Lx128_0:
                        .quad            .Lx128_0_s
.Lx128_0_s:
                        .string          "return"
.Lx128_1:
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n50_goto_α:
                                                                                        jmp   n2_lit_integer_α
n50_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n51_goto_α:
                                                                                        jmp   n8_lit_string_α
n51_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n52_goto_α:
                                                                                        jmp   n15_lit_integer_α
n52_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n53_goto_α:
                                                                                        jmp   n21_lit_string_α
n53_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n54_goto_α:
                                                                                        jmp   n55_var_α
n54_goto_β:
                                                                                        jmp   main_ω
#=======================================================================================================================
#         fact2 = eq(n, 1) 1                             :s(return)
#-----------------------------------------------------------------------------------------------------------------------
n55_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 1184], rax
                        mov              qword ptr [rsp + 1192], rdx
                                                                                        jmp   n56_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n56_lit_integer_α:
                        mov              qword ptr [rsp + 1200], 6
                        mov              rax, qword ptr [rip + .Lx135_0]
                        mov              qword ptr [rsp + 1208], rax
                                                                                        jmp   n57_call_α
.Lx135_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n57_call_α:
                        mov              rax, qword ptr [rsp + 1184]
                        mov              qword ptr [rsp + 1136], rax
                        mov              rax, qword ptr [rsp + 1192]
                        mov              qword ptr [rsp + 1144], rax
                        mov              rax, qword ptr [rsp + 1200]
                        mov              qword ptr [rsp + 1152], rax
                        mov              rax, qword ptr [rsp + 1208]
                        mov              qword ptr [rsp + 1160], rax
                        .section         .rodata
.Lbynamefn58:           .string          "eq"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn58]
                        lea              rsi, [rsp + 1136]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1120], rax
                        mov              qword ptr [rsp + 1128], rdx
                        cmp              eax, 99
                                                                                        je    n62_var_α
                                                                                        jmp   n58_lit_integer_α
n57_call_β:
                                                                                        jmp   n62_var_α
#-----------------------------------------------------------------------------------------------------------------------
n58_lit_integer_α:
                        mov              qword ptr [rsp + 1216], 6
                        mov              rax, qword ptr [rip + .Lx137_0]
                        mov              qword ptr [rsp + 1224], rax
                                                                                        jmp   n59_binop_α
.Lx137_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n59_binop_α:
                        mov              rdi, qword ptr [rsp + 1120]
                        mov              rsi, qword ptr [rsp + 1128]
                        mov              rdx, qword ptr [rsp + 1216]
                        mov              rcx, qword ptr [rsp + 1224]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 1104], rax
                        mov              qword ptr [rsp + 1112], rdx
                                                                                        jmp   n60_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n60_assign_α:
                        mov              rax, qword ptr [rsp + 1104]
                        mov              rdx, qword ptr [rsp + 1112]
                        mov              qword ptr [1879052368], rax
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n61_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n61_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx141_0]
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx141_1
.Lx141_0:
                        .quad            .Lx141_0_s
.Lx141_0_s:
                        .string          "return"
.Lx141_1:
                                                                                        jmp   main_γ
#=======================================================================================================================
#         fact2 = n * fact2(n - 1)                       :(return)
#-----------------------------------------------------------------------------------------------------------------------
n62_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 1248], rax
                        mov              qword ptr [rsp + 1256], rdx
                                                                                        jmp   n63_var_α
#-----------------------------------------------------------------------------------------------------------------------
n63_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 1328], rax
                        mov              qword ptr [rsp + 1336], rdx
                                                                                        jmp   n64_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n64_lit_integer_α:
                        mov              qword ptr [rsp + 1344], 6
                        mov              rax, qword ptr [rip + .Lx144_0]
                        mov              qword ptr [rsp + 1352], rax
                                                                                        jmp   n65_binop_α
.Lx144_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n65_binop_α:
                        mov              eax, dword ptr [rsp + 1328]
                        cmp              eax, 6
                                                                                        jne   .Lx145_0
                        mov              rax, qword ptr [rsp + 1336]
                        mov              rcx, 1
                        sub              rax, rcx
                        mov              qword ptr [rsp + 1312], 6
                        mov              qword ptr [rsp + 1320], rax
                                                                                        jmp   n66_call_α
.Lx145_0:
                        mov              rdi, qword ptr [rsp + 1328]
                        mov              rsi, qword ptr [rsp + 1336]
                        mov              rdx, qword ptr [rsp + 1344]
                        mov              rcx, qword ptr [rsp + 1352]
                        call             rt_sub@PLT
                        cmp              eax, 99
                                                                                        je    n70_goto_deferred_α
                        mov              qword ptr [rsp + 1312], rax
                        mov              qword ptr [rsp + 1320], rdx
                                                                                        jmp   n66_call_α
#-----------------------------------------------------------------------------------------------------------------------
n66_call_α:
                        mov              rax, qword ptr [rsp + 1312]
                        mov              qword ptr [rsp + 1280], rax
                        mov              rax, qword ptr [rsp + 1320]
                        mov              qword ptr [rsp + 1288], rax
                        .section         .rodata
.Lbynamefn67:           .string          "fact2"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn67]
                        lea              rsi, [rsp + 1280]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1264], rax
                        mov              qword ptr [rsp + 1272], rdx
                        cmp              eax, 99
                                                                                        je    n70_goto_deferred_α
                                                                                        jmp   n67_binop_α
n66_call_β:
                                                                                        jmp   n70_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n67_binop_α:
                        mov              eax, dword ptr [rsp + 1248]
                        cmp              eax, 6
                                                                                        jne   .Lx147_0
                        mov              eax, dword ptr [rsp + 1264]
                        cmp              eax, 6
                                                                                        jne   .Lx147_0
                        mov              rax, qword ptr [rsp + 1256]
                        mov              rcx, qword ptr [rsp + 1272]
                        imul             rax, rcx
                        mov              qword ptr [rsp + 1232], 6
                        mov              qword ptr [rsp + 1240], rax
                                                                                        jmp   n68_assign_α
.Lx147_0:
                        mov              rdi, qword ptr [rsp + 1248]
                        mov              rsi, qword ptr [rsp + 1256]
                        mov              rdx, qword ptr [rsp + 1264]
                        mov              rcx, qword ptr [rsp + 1272]
                        call             rt_mul@PLT
                        cmp              eax, 99
                                                                                        je    n70_goto_deferred_α
                        mov              qword ptr [rsp + 1232], rax
                        mov              qword ptr [rsp + 1240], rdx
                                                                                        jmp   n68_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n68_assign_α:
                        mov              rax, qword ptr [rsp + 1232]
                        mov              rdx, qword ptr [rsp + 1240]
                        mov              qword ptr [1879052368], rax
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n69_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n69_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx150_0]
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx150_1
.Lx150_0:
                        .quad            .Lx150_0_s
.Lx150_0_s:
                        .string          "return"
.Lx150_1:
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n70_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx152_0]
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx152_1
.Lx152_0:
                        .quad            .Lx152_0_s
.Lx152_0_s:
                        .string          "return"
.Lx152_1:
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n71_goto_α:
                                                                                        jmp   n25_lit_integer_α
n71_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n72_goto_α:
                                                                                        jmp   n31_lit_string_α
n72_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n73_goto_α:
                                                                                        jmp   main_γ
n73_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        add              rsp, 1544
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              eax, 99
                        xor              edx, edx
                        add              rsp, 1544
                        ret
                        .section         .note.GNU-stack,"",@progbits

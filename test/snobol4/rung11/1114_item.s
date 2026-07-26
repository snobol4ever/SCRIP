                        .intel_syntax    noprefix
                        .text
                        .section         .rodata
.Lgvan0:                .string          "aaa"
.Lgvan1:                .string          "output"
.Lgvan2:                .string          "ama"
.Lgvan3:                .string          "tt"
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
                        mov              r12, qword ptr [1879048192]
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
                        sub              rsp, 2856
                        mov              rdi, rsp
                        mov              ecx, 2856
                        xor              eax, eax
                        rep stosb
                        mov              [rsp + 2848], rbp
                        mov              rbp, rsp
main_α_body:
#=======================================================================================================================
#         aaa = array(10)
#-----------------------------------------------------------------------------------------------------------------------
n0_lit_integer_α:
                        mov              qword ptr [rbp + 64], 6
                        mov              rax, qword ptr [rip + .Lx121_0]
                        mov              qword ptr [rbp + 72], rax
                                                                                        jmp   n9_call_α
.Lx121_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n1_goto_α:
                                                                                        jmp   n10_var_α
n1_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n2_goto_α:
                                                                                        jmp   n11_var_α
n2_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n3_goto_α:
                                                                                        jmp   n12_lit_string_α
n3_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n4_goto_α:
                                                                                        jmp   n13_var_α
n4_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n5_goto_α:
                                                                                        jmp   n14_var_α
n5_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n6_goto_α:
                                                                                        jmp   n15_call_α
n6_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n7_goto_α:
                                                                                        jmp   n16_lit_string_α
n7_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n8_goto_α:
                                                                                        jmp   main_γ
n8_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n9_call_α:
                        mov              rax, qword ptr [rbp + 64]
                        mov              qword ptr [rbp + 32], rax
                        mov              rax, qword ptr [rbp + 72]
                        mov              qword ptr [rbp + 40], rax
                        .section         .rodata
.Lbynamefn10:           .string          "array"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn10]
                        lea              rsi, [rbp + 32]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 16], rax
                        mov              qword ptr [rbp + 24], rdx
                        cmp              eax, 99
                                                                                        je    n18_lit_string_α
                                                                                        jmp   n17_assign_α
n9_call_β:
                                                                                        jmp   n18_lit_string_α
#=======================================================================================================================
#         differ(aaa<1>, 5)                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n10_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rbp + 544], rax
                        mov              qword ptr [rbp + 552], rdx
                                                                                        jmp   n19_lit_integer_α
#=======================================================================================================================
#         aaa<2> = 22
#-----------------------------------------------------------------------------------------------------------------------
n11_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rbp + 656], rax
                        mov              qword ptr [rbp + 664], rdx
                                                                                        jmp   n20_lit_integer_α
#=======================================================================================================================
#         ama = array('2,2,2,2')
#-----------------------------------------------------------------------------------------------------------------------
n12_lit_string_α:
                        mov              qword ptr [rbp + 1008], 1
                        mov              rax, qword ptr [rip + .Lx133_0]
                        mov              qword ptr [rbp + 1016], rax
                                                                                        jmp   n22_call_α
.Lx133_0:
                        .quad            .Lx133_0_s
.Lx133_0_s:
                        .string          "2,2,2,2"
#=======================================================================================================================
#         differ(ama<1,2,1,2>, 1212)                   :f(e005)
#-----------------------------------------------------------------------------------------------------------------------
n13_var_α:
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rbp + 1680], rax
                        mov              qword ptr [rbp + 1688], rdx
                                                                                        jmp   n23_lit_integer_α
#=======================================================================================================================
#         ama<2,1,2,1> = 2121
#-----------------------------------------------------------------------------------------------------------------------
n14_var_α:
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rbp + 1888], rax
                        mov              qword ptr [rbp + 1896], rdx
                                                                                        jmp   n24_lit_integer_α
#=======================================================================================================================
#         tt = table()
#-----------------------------------------------------------------------------------------------------------------------
n15_call_α:
                        .section         .rodata
.Lrkfn137:              .string          "table"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn137]
                        lea              rsi, [rbp + 2400]
                        mov              edx, 0
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 2384], rax
                        mov              qword ptr [rbp + 2392], rdx
                        cmp              eax, 99
                                                                                        je    n27_lit_string_α
                                                                                        jmp   n26_assign_α
n15_call_β:
                                                                                        jmp   n27_lit_string_α
#=======================================================================================================================
#         output = 'PASS 1114_item (7/7)'
#-----------------------------------------------------------------------------------------------------------------------
n16_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              rax, qword ptr [rip + .Lx138_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n28_assign_α
.Lx138_0:
                        .quad            .Lx138_0_s
.Lx138_0_s:
                        .string          "PASS 1114_item (7/7)"
#-----------------------------------------------------------------------------------------------------------------------
n17_assign_α:
                        mov              rax, qword ptr [rbp + 16]
                        mov              rdx, qword ptr [rbp + 24]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        mov              qword ptr [rbp + 0], rax
                        mov              qword ptr [rbp + 8], rdx
                                                                                        jmp   n18_lit_string_α
#=======================================================================================================================
#         aaa = array(10)
#         item(aaa, 1) = 5
#-----------------------------------------------------------------------------------------------------------------------
n18_lit_string_α:
                        mov              qword ptr [rbp + 80], 1
                        mov              rax, qword ptr [rip + .Lx140_0]
                        mov              qword ptr [rbp + 88], rax
                                                                                        jmp   n29_call_α
.Lx140_0:
                        .quad            .Lx140_0_s
.Lx140_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n19_lit_integer_α:
                        mov              qword ptr [rbp + 560], 6
                        mov              rax, qword ptr [rip + .Lx141_0]
                        mov              qword ptr [rbp + 568], rax
                                                                                        jmp   n30_subscript_α
.Lx141_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n20_lit_integer_α:
                        mov              qword ptr [rbp + 672], 6
                        mov              rax, qword ptr [rip + .Lx142_0]
                        mov              qword ptr [rbp + 680], rax
                                                                                        jmp   n31_subscript_α
.Lx142_0:
                        .quad            2
#=======================================================================================================================
#         differ(item(aaa, 2), 22)                   :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n21_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rbp + 864], rax
                        mov              qword ptr [rbp + 872], rdx
                                                                                        jmp   n32_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n22_call_α:
                        mov              rax, qword ptr [rbp + 1008]
                        mov              qword ptr [rbp + 976], rax
                        mov              rax, qword ptr [rbp + 1016]
                        mov              qword ptr [rbp + 984], rax
                        .section         .rodata
.Lbynamefn23:           .string          "array"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn23]
                        lea              rsi, [rbp + 976]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 960], rax
                        mov              qword ptr [rbp + 968], rdx
                        cmp              eax, 99
                                                                                        je    n34_lit_string_α
                                                                                        jmp   n33_assign_α
n22_call_β:
                                                                                        jmp   n34_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n23_lit_integer_α:
                        mov              qword ptr [rbp + 1696], 6
                        mov              rax, qword ptr [rip + .Lx145_0]
                        mov              qword ptr [rbp + 1704], rax
                                                                                        jmp   n35_subscript_α
.Lx145_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n24_lit_integer_α:
                        mov              qword ptr [rbp + 1904], 6
                        mov              rax, qword ptr [rip + .Lx146_0]
                        mov              qword ptr [rbp + 1912], rax
                                                                                        jmp   n36_subscript_α
.Lx146_0:
                        .quad            2
#=======================================================================================================================
#         differ(item(ama, 2,1,2,1), 2121)                   :f(e006)
#-----------------------------------------------------------------------------------------------------------------------
n25_var_α:
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rbp + 2240], rax
                        mov              qword ptr [rbp + 2248], rdx
                                                                                        jmp   n37_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n26_assign_α:
                        mov              rax, qword ptr [rbp + 2384]
                        mov              rdx, qword ptr [rbp + 2392]
                        mov              qword ptr [1879052336], rax
                        mov              qword ptr [1879052344], rdx
                        mov              qword ptr [rbp + 2368], rax
                        mov              qword ptr [rbp + 2376], rdx
                                                                                        jmp   n27_lit_string_α
#=======================================================================================================================
#         item(tt, 'key') = 'val'
#-----------------------------------------------------------------------------------------------------------------------
n27_lit_string_α:
                        mov              qword ptr [rbp + 2416], 1
                        mov              rax, qword ptr [rip + .Lx149_0]
                        mov              qword ptr [rbp + 2424], rax
                                                                                        jmp   n38_call_α
.Lx149_0:
                        .quad            .Lx149_0_s
.Lx149_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n28_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        mov              qword ptr [rbp + 2816], rax
                        mov              qword ptr [rbp + 2824], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n29_call_α:
                        mov              rax, qword ptr [rbp + 80]
                        mov              qword ptr [rbp + 112], rax
                        mov              rax, qword ptr [rbp + 88]
                        mov              qword ptr [rbp + 120], rax
                        .section         .rodata
.Lrkfn152:              .string          "SNO$WANTNM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn152]
                        lea              rsi, [rbp + 112]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 96], rax
                        mov              qword ptr [rbp + 104], rdx
                        cmp              eax, 99
                                                                                        je    n40_var_α
                                                                                        jmp   n39_var_α
n29_call_β:
                                                                                        jmp   n40_var_α
#-----------------------------------------------------------------------------------------------------------------------
n30_subscript_α:
                        mov              rdi, qword ptr [rbp + 544]
                        mov              rsi, qword ptr [rbp + 552]
                        mov              rdx, qword ptr [rbp + 560]
                        mov              rcx, qword ptr [rbp + 568]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n11_var_α
                        mov              qword ptr [rbp + 576], rax
                        mov              qword ptr [rbp + 584], rdx
                                                                                        jmp   n41_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n31_subscript_α:
                        mov              rdi, qword ptr [rbp + 656]
                        mov              rsi, qword ptr [rbp + 664]
                        mov              rdx, qword ptr [rbp + 672]
                        mov              rcx, qword ptr [rbp + 680]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n21_var_α
                        mov              qword ptr [rbp + 688], rax
                        mov              qword ptr [rbp + 696], rdx
                                                                                        jmp   n42_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n32_lit_integer_α:
                        mov              qword ptr [rbp + 880], 6
                        mov              rax, qword ptr [rip + .Lx155_0]
                        mov              qword ptr [rbp + 888], rax
                                                                                        jmp   n43_call_α
.Lx155_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n33_assign_α:
                        mov              rax, qword ptr [rbp + 960]
                        mov              rdx, qword ptr [rbp + 968]
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rdx
                        mov              qword ptr [rbp + 944], rax
                        mov              qword ptr [rbp + 952], rdx
                                                                                        jmp   n34_lit_string_α
#=======================================================================================================================
#         item(ama, 1,2,1,2) = 1212
#-----------------------------------------------------------------------------------------------------------------------
n34_lit_string_α:
                        mov              qword ptr [rbp + 1024], 1
                        mov              rax, qword ptr [rip + .Lx157_0]
                        mov              qword ptr [rbp + 1032], rax
                                                                                        jmp   n44_call_α
.Lx157_0:
                        .quad            .Lx157_0_s
.Lx157_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n35_subscript_α:
                        mov              rdi, qword ptr [rbp + 1680]
                        mov              rsi, qword ptr [rbp + 1688]
                        mov              rdx, qword ptr [rbp + 1696]
                        mov              rcx, qword ptr [rbp + 1704]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n14_var_α
                        mov              qword ptr [rbp + 1712], rax
                        mov              qword ptr [rbp + 1720], rdx
                                                                                        jmp   n45_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n36_subscript_α:
                        mov              rdi, qword ptr [rbp + 1888]
                        mov              rsi, qword ptr [rbp + 1896]
                        mov              rdx, qword ptr [rbp + 1904]
                        mov              rcx, qword ptr [rbp + 1912]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n25_var_α
                        mov              qword ptr [rbp + 1920], rax
                        mov              qword ptr [rbp + 1928], rdx
                                                                                        jmp   n46_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n37_lit_integer_α:
                        mov              qword ptr [rbp + 2256], 6
                        mov              rax, qword ptr [rip + .Lx160_0]
                        mov              qword ptr [rbp + 2264], rax
                                                                                        jmp   n47_lit_integer_α
.Lx160_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n38_call_α:
                        mov              rax, qword ptr [rbp + 2416]
                        mov              qword ptr [rbp + 2448], rax
                        mov              rax, qword ptr [rbp + 2424]
                        mov              qword ptr [rbp + 2456], rax
                        .section         .rodata
.Lrkfn162:              .string          "SNO$WANTNM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn162]
                        lea              rsi, [rbp + 2448]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 2432], rax
                        mov              qword ptr [rbp + 2440], rdx
                        cmp              eax, 99
                                                                                        je    n49_var_α
                                                                                        jmp   n48_var_α
n38_call_β:
                                                                                        jmp   n49_var_α
#-----------------------------------------------------------------------------------------------------------------------
n39_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rbp + 208], rax
                        mov              qword ptr [rbp + 216], rdx
                                                                                        jmp   n50_lit_integer_α
#=======================================================================================================================
#         differ(item(aaa, 1), 5)                   :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n40_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rbp + 400], rax
                        mov              qword ptr [rbp + 408], rdx
                                                                                        jmp   n51_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n41_deref_α:
                        mov              rdi, qword ptr [rbp + 576]
                        mov              rsi, qword ptr [rbp + 584]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n11_var_α
                        mov              qword ptr [rbp + 592], rax
                        mov              qword ptr [rbp + 600], rdx
                                                                                        jmp   n52_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n42_lit_integer_α:
                        mov              qword ptr [rbp + 704], 6
                        mov              rax, qword ptr [rip + .Lx166_0]
                        mov              qword ptr [rbp + 712], rax
                                                                                        jmp   n53_assign_var_α
.Lx166_0:
                        .quad            22
#-----------------------------------------------------------------------------------------------------------------------
n43_call_α:
                        mov              rax, qword ptr [rbp + 864]
                        mov              qword ptr [rbp + 816], rax
                        mov              rax, qword ptr [rbp + 872]
                        mov              qword ptr [rbp + 824], rax
                        mov              rax, qword ptr [rbp + 880]
                        mov              qword ptr [rbp + 832], rax
                        mov              rax, qword ptr [rbp + 888]
                        mov              qword ptr [rbp + 840], rax
                        .section         .rodata
.Lbynamefn44:           .string          "item"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn44]
                        lea              rsi, [rbp + 816]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 800], rax
                        mov              qword ptr [rbp + 808], rdx
                        cmp              eax, 99
                                                                                        je    n12_lit_string_α
                                                                                        jmp   n54_lit_integer_α
n43_call_β:
                                                                                        jmp   n12_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n44_call_α:
                        mov              rax, qword ptr [rbp + 1024]
                        mov              qword ptr [rbp + 1056], rax
                        mov              rax, qword ptr [rbp + 1032]
                        mov              qword ptr [rbp + 1064], rax
                        .section         .rodata
.Lrkfn169:              .string          "SNO$WANTNM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn169]
                        lea              rsi, [rbp + 1056]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1040], rax
                        mov              qword ptr [rbp + 1048], rdx
                        cmp              eax, 99
                                                                                        je    n56_var_α
                                                                                        jmp   n55_var_α
n44_call_β:
                                                                                        jmp   n56_var_α
#-----------------------------------------------------------------------------------------------------------------------
n45_lit_integer_α:
                        mov              qword ptr [rbp + 1728], 6
                        mov              rax, qword ptr [rip + .Lx170_0]
                        mov              qword ptr [rbp + 1736], rax
                                                                                        jmp   n57_subscript_α
.Lx170_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n46_lit_integer_α:
                        mov              qword ptr [rbp + 1936], 6
                        mov              rax, qword ptr [rip + .Lx171_0]
                        mov              qword ptr [rbp + 1944], rax
                                                                                        jmp   n58_subscript_α
.Lx171_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n47_lit_integer_α:
                        mov              qword ptr [rbp + 2272], 6
                        mov              rax, qword ptr [rip + .Lx172_0]
                        mov              qword ptr [rbp + 2280], rax
                                                                                        jmp   n59_lit_integer_α
.Lx172_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n48_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 2544], rax
                        mov              qword ptr [rbp + 2552], rdx
                                                                                        jmp   n60_lit_string_α
#=======================================================================================================================
#         differ(item(tt, 'key'), 'val')                   :f(e007)
#-----------------------------------------------------------------------------------------------------------------------
n49_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 2736], rax
                        mov              qword ptr [rbp + 2744], rdx
                                                                                        jmp   n61_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n50_lit_integer_α:
                        mov              qword ptr [rbp + 224], 6
                        mov              rax, qword ptr [rip + .Lx175_0]
                        mov              qword ptr [rbp + 232], rax
                                                                                        jmp   n62_call_α
.Lx175_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n51_lit_integer_α:
                        mov              qword ptr [rbp + 416], 6
                        mov              rax, qword ptr [rip + .Lx176_0]
                        mov              qword ptr [rbp + 424], rax
                                                                                        jmp   n63_call_α
.Lx176_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n52_lit_integer_α:
                        mov              qword ptr [rbp + 608], 6
                        mov              rax, qword ptr [rip + .Lx177_0]
                        mov              qword ptr [rbp + 616], rax
                                                                                        jmp   n64_call_α
.Lx177_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n53_assign_var_α:
                        mov              rdi, qword ptr [rbp + 688]
                        mov              rsi, qword ptr [rbp + 696]
                        mov              rdx, qword ptr [rbp + 704]
                        mov              rcx, qword ptr [rbp + 712]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n21_var_α
                        mov              qword ptr [rbp + 720], rax
                        mov              qword ptr [rbp + 728], rdx
                                                                                        jmp   n21_var_α
#-----------------------------------------------------------------------------------------------------------------------
n54_lit_integer_α:
                        mov              qword ptr [rbp + 896], 6
                        mov              rax, qword ptr [rip + .Lx179_0]
                        mov              qword ptr [rbp + 904], rax
                                                                                        jmp   n65_call_α
.Lx179_0:
                        .quad            22
#-----------------------------------------------------------------------------------------------------------------------
n55_var_α:
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rbp + 1200], rax
                        mov              qword ptr [rbp + 1208], rdx
                                                                                        jmp   n66_lit_integer_α
#=======================================================================================================================
#         differ(item(ama, 1,2,1,2), 1212)                   :f(e004)
#-----------------------------------------------------------------------------------------------------------------------
n56_var_α:
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rbp + 1488], rax
                        mov              qword ptr [rbp + 1496], rdx
                                                                                        jmp   n67_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n57_subscript_α:
                        mov              rdi, qword ptr [rbp + 1712]
                        mov              rsi, qword ptr [rbp + 1720]
                        mov              rdx, qword ptr [rbp + 1728]
                        mov              rcx, qword ptr [rbp + 1736]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n14_var_α
                        mov              qword ptr [rbp + 1744], rax
                        mov              qword ptr [rbp + 1752], rdx
                                                                                        jmp   n68_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n58_subscript_α:
                        mov              rdi, qword ptr [rbp + 1920]
                        mov              rsi, qword ptr [rbp + 1928]
                        mov              rdx, qword ptr [rbp + 1936]
                        mov              rcx, qword ptr [rbp + 1944]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n25_var_α
                        mov              qword ptr [rbp + 1952], rax
                        mov              qword ptr [rbp + 1960], rdx
                                                                                        jmp   n69_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n59_lit_integer_α:
                        mov              qword ptr [rbp + 2288], 6
                        mov              rax, qword ptr [rip + .Lx184_0]
                        mov              qword ptr [rbp + 2296], rax
                                                                                        jmp   n70_lit_integer_α
.Lx184_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n60_lit_string_α:
                        mov              qword ptr [rbp + 2560], 1
                        mov              rax, qword ptr [rip + .Lx185_0]
                        mov              qword ptr [rbp + 2568], rax
                                                                                        jmp   n71_call_α
.Lx185_0:
                        .quad            .Lx185_0_s
.Lx185_0_s:
                        .string          "key"
#-----------------------------------------------------------------------------------------------------------------------
n61_lit_string_α:
                        mov              qword ptr [rbp + 2752], 1
                        mov              rax, qword ptr [rip + .Lx186_0]
                        mov              qword ptr [rbp + 2760], rax
                                                                                        jmp   n72_call_α
.Lx186_0:
                        .quad            .Lx186_0_s
.Lx186_0_s:
                        .string          "key"
#-----------------------------------------------------------------------------------------------------------------------
n62_call_α:
                        mov              rax, qword ptr [rbp + 208]
                        mov              qword ptr [rbp + 160], rax
                        mov              rax, qword ptr [rbp + 216]
                        mov              qword ptr [rbp + 168], rax
                        mov              rax, qword ptr [rbp + 224]
                        mov              qword ptr [rbp + 176], rax
                        mov              rax, qword ptr [rbp + 232]
                        mov              qword ptr [rbp + 184], rax
                        .section         .rodata
.Lbynamefn63:           .string          "item"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn63]
                        lea              rsi, [rbp + 160]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 144], rax
                        mov              qword ptr [rbp + 152], rdx
                        cmp              eax, 99
                                                                                        je    n40_var_α
                                                                                        jmp   n73_lit_integer_α
n62_call_β:
                                                                                        jmp   n40_var_α
#-----------------------------------------------------------------------------------------------------------------------
n63_call_α:
                        mov              rax, qword ptr [rbp + 400]
                        mov              qword ptr [rbp + 352], rax
                        mov              rax, qword ptr [rbp + 408]
                        mov              qword ptr [rbp + 360], rax
                        mov              rax, qword ptr [rbp + 416]
                        mov              qword ptr [rbp + 368], rax
                        mov              rax, qword ptr [rbp + 424]
                        mov              qword ptr [rbp + 376], rax
                        .section         .rodata
.Lbynamefn64:           .string          "item"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn64]
                        lea              rsi, [rbp + 352]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 336], rax
                        mov              qword ptr [rbp + 344], rdx
                        cmp              eax, 99
                                                                                        je    n10_var_α
                                                                                        jmp   n74_lit_integer_α
n63_call_β:
                                                                                        jmp   n10_var_α
#-----------------------------------------------------------------------------------------------------------------------
n64_call_α:
                        mov              rax, qword ptr [rbp + 592]
                        mov              qword ptr [rbp + 496], rax
                        mov              rax, qword ptr [rbp + 600]
                        mov              qword ptr [rbp + 504], rax
                        mov              rax, qword ptr [rbp + 608]
                        mov              qword ptr [rbp + 512], rax
                        mov              rax, qword ptr [rbp + 616]
                        mov              qword ptr [rbp + 520], rax
                        .section         .rodata
.Lbynamefn65:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn65]
                        lea              rsi, [rbp + 496]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 480], rax
                        mov              qword ptr [rbp + 488], rdx
                        cmp              eax, 99
                                                                                        je    n11_var_α
                                                                                        jmp   n75_lit_string_α
n64_call_β:
                                                                                        jmp   n11_var_α
#-----------------------------------------------------------------------------------------------------------------------
n65_call_α:
                        mov              rax, qword ptr [rbp + 800]
                        mov              qword ptr [rbp + 752], rax
                        mov              rax, qword ptr [rbp + 808]
                        mov              qword ptr [rbp + 760], rax
                        mov              rax, qword ptr [rbp + 896]
                        mov              qword ptr [rbp + 768], rax
                        mov              rax, qword ptr [rbp + 904]
                        mov              qword ptr [rbp + 776], rax
                        .section         .rodata
.Lbynamefn66:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn66]
                        lea              rsi, [rbp + 752]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 736], rax
                        mov              qword ptr [rbp + 744], rdx
                        cmp              eax, 99
                                                                                        je    n12_lit_string_α
                                                                                        jmp   n76_lit_string_α
n65_call_β:
                                                                                        jmp   n12_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n66_lit_integer_α:
                        mov              qword ptr [rbp + 1216], 6
                        mov              rax, qword ptr [rip + .Lx191_0]
                        mov              qword ptr [rbp + 1224], rax
                                                                                        jmp   n77_lit_integer_α
.Lx191_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n67_lit_integer_α:
                        mov              qword ptr [rbp + 1504], 6
                        mov              rax, qword ptr [rip + .Lx192_0]
                        mov              qword ptr [rbp + 1512], rax
                                                                                        jmp   n78_lit_integer_α
.Lx192_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n68_lit_integer_α:
                        mov              qword ptr [rbp + 1760], 6
                        mov              rax, qword ptr [rip + .Lx193_0]
                        mov              qword ptr [rbp + 1768], rax
                                                                                        jmp   n79_subscript_α
.Lx193_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n69_lit_integer_α:
                        mov              qword ptr [rbp + 1968], 6
                        mov              rax, qword ptr [rip + .Lx194_0]
                        mov              qword ptr [rbp + 1976], rax
                                                                                        jmp   n80_subscript_α
.Lx194_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n70_lit_integer_α:
                        mov              qword ptr [rbp + 2304], 6
                        mov              rax, qword ptr [rip + .Lx195_0]
                        mov              qword ptr [rbp + 2312], rax
                                                                                        jmp   n81_call_α
.Lx195_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n71_call_α:
                        mov              rax, qword ptr [rbp + 2544]
                        mov              qword ptr [rbp + 2496], rax
                        mov              rax, qword ptr [rbp + 2552]
                        mov              qword ptr [rbp + 2504], rax
                        mov              rax, qword ptr [rbp + 2560]
                        mov              qword ptr [rbp + 2512], rax
                        mov              rax, qword ptr [rbp + 2568]
                        mov              qword ptr [rbp + 2520], rax
                        .section         .rodata
.Lbynamefn72:           .string          "item"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn72]
                        lea              rsi, [rbp + 2496]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 2480], rax
                        mov              qword ptr [rbp + 2488], rdx
                        cmp              eax, 99
                                                                                        je    n49_var_α
                                                                                        jmp   n82_lit_string_α
n71_call_β:
                                                                                        jmp   n49_var_α
#-----------------------------------------------------------------------------------------------------------------------
n72_call_α:
                        mov              rax, qword ptr [rbp + 2736]
                        mov              qword ptr [rbp + 2688], rax
                        mov              rax, qword ptr [rbp + 2744]
                        mov              qword ptr [rbp + 2696], rax
                        mov              rax, qword ptr [rbp + 2752]
                        mov              qword ptr [rbp + 2704], rax
                        mov              rax, qword ptr [rbp + 2760]
                        mov              qword ptr [rbp + 2712], rax
                        .section         .rodata
.Lbynamefn73:           .string          "item"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn73]
                        lea              rsi, [rbp + 2688]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 2672], rax
                        mov              qword ptr [rbp + 2680], rdx
                        cmp              eax, 99
                                                                                        je    n16_lit_string_α
                                                                                        jmp   n83_lit_string_α
n72_call_β:
                                                                                        jmp   n16_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n73_lit_integer_α:
                        mov              qword ptr [rbp + 240], 6
                        mov              rax, qword ptr [rip + .Lx198_0]
                        mov              qword ptr [rbp + 248], rax
                                                                                        jmp   n84_assign_var_α
.Lx198_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n74_lit_integer_α:
                        mov              qword ptr [rbp + 432], 6
                        mov              rax, qword ptr [rip + .Lx199_0]
                        mov              qword ptr [rbp + 440], rax
                                                                                        jmp   n85_call_α
.Lx199_0:
                        .quad            5
#=======================================================================================================================
#         output = 'FAIL 1114/002: item == bracket read' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n75_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              rax, qword ptr [rip + .Lx200_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n86_assign_α
.Lx200_0:
                        .quad            .Lx200_0_s
.Lx200_0_s:
                        .string          "FAIL 1114/002: item == bracket read"
#=======================================================================================================================
#         output = 'FAIL 1114/003: bracket assign, item read' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n76_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              rax, qword ptr [rip + .Lx201_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n87_assign_α
.Lx201_0:
                        .quad            .Lx201_0_s
.Lx201_0_s:
                        .string          "FAIL 1114/003: bracket assign, item read"
#-----------------------------------------------------------------------------------------------------------------------
n77_lit_integer_α:
                        mov              qword ptr [rbp + 1232], 6
                        mov              rax, qword ptr [rip + .Lx202_0]
                        mov              qword ptr [rbp + 1240], rax
                                                                                        jmp   n88_lit_integer_α
.Lx202_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n78_lit_integer_α:
                        mov              qword ptr [rbp + 1520], 6
                        mov              rax, qword ptr [rip + .Lx203_0]
                        mov              qword ptr [rbp + 1528], rax
                                                                                        jmp   n89_lit_integer_α
.Lx203_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n79_subscript_α:
                        mov              rdi, qword ptr [rbp + 1744]
                        mov              rsi, qword ptr [rbp + 1752]
                        mov              rdx, qword ptr [rbp + 1760]
                        mov              rcx, qword ptr [rbp + 1768]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n14_var_α
                        mov              qword ptr [rbp + 1776], rax
                        mov              qword ptr [rbp + 1784], rdx
                                                                                        jmp   n90_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n80_subscript_α:
                        mov              rdi, qword ptr [rbp + 1952]
                        mov              rsi, qword ptr [rbp + 1960]
                        mov              rdx, qword ptr [rbp + 1968]
                        mov              rcx, qword ptr [rbp + 1976]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n25_var_α
                        mov              qword ptr [rbp + 1984], rax
                        mov              qword ptr [rbp + 1992], rdx
                                                                                        jmp   n91_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n81_call_α:
                        mov              rax, qword ptr [rbp + 2240]
                        mov              qword ptr [rbp + 2144], rax
                        mov              rax, qword ptr [rbp + 2248]
                        mov              qword ptr [rbp + 2152], rax
                        mov              rax, qword ptr [rbp + 2256]
                        mov              qword ptr [rbp + 2160], rax
                        mov              rax, qword ptr [rbp + 2264]
                        mov              qword ptr [rbp + 2168], rax
                        mov              rax, qword ptr [rbp + 2272]
                        mov              qword ptr [rbp + 2176], rax
                        mov              rax, qword ptr [rbp + 2280]
                        mov              qword ptr [rbp + 2184], rax
                        mov              rax, qword ptr [rbp + 2288]
                        mov              qword ptr [rbp + 2192], rax
                        mov              rax, qword ptr [rbp + 2296]
                        mov              qword ptr [rbp + 2200], rax
                        mov              rax, qword ptr [rbp + 2304]
                        mov              qword ptr [rbp + 2208], rax
                        mov              rax, qword ptr [rbp + 2312]
                        mov              qword ptr [rbp + 2216], rax
                        .section         .rodata
.Lbynamefn82:           .string          "item"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn82]
                        lea              rsi, [rbp + 2144]
                        mov              edx, 5
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 2128], rax
                        mov              qword ptr [rbp + 2136], rdx
                        cmp              eax, 99
                                                                                        je    n15_call_α
                                                                                        jmp   n92_lit_integer_α
n81_call_β:
                                                                                        jmp   n15_call_α
#-----------------------------------------------------------------------------------------------------------------------
n82_lit_string_α:
                        mov              qword ptr [rbp + 2576], 1
                        mov              rax, qword ptr [rip + .Lx207_0]
                        mov              qword ptr [rbp + 2584], rax
                                                                                        jmp   n93_assign_var_α
.Lx207_0:
                        .quad            .Lx207_0_s
.Lx207_0_s:
                        .string          "val"
#-----------------------------------------------------------------------------------------------------------------------
n83_lit_string_α:
                        mov              qword ptr [rbp + 2768], 1
                        mov              rax, qword ptr [rip + .Lx208_0]
                        mov              qword ptr [rbp + 2776], rax
                                                                                        jmp   n94_call_α
.Lx208_0:
                        .quad            .Lx208_0_s
.Lx208_0_s:
                        .string          "val"
#-----------------------------------------------------------------------------------------------------------------------
n84_assign_var_α:
                        mov              rdi, qword ptr [rbp + 144]
                        mov              rsi, qword ptr [rbp + 152]
                        mov              rdx, qword ptr [rbp + 240]
                        mov              rcx, qword ptr [rbp + 248]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n40_var_α
                        mov              qword ptr [rbp + 256], rax
                        mov              qword ptr [rbp + 264], rdx
                                                                                        jmp   n40_var_α
#-----------------------------------------------------------------------------------------------------------------------
n85_call_α:
                        mov              rax, qword ptr [rbp + 336]
                        mov              qword ptr [rbp + 288], rax
                        mov              rax, qword ptr [rbp + 344]
                        mov              qword ptr [rbp + 296], rax
                        mov              rax, qword ptr [rbp + 432]
                        mov              qword ptr [rbp + 304], rax
                        mov              rax, qword ptr [rbp + 440]
                        mov              qword ptr [rbp + 312], rax
                        .section         .rodata
.Lbynamefn86:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn86]
                        lea              rsi, [rbp + 288]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 272], rax
                        mov              qword ptr [rbp + 280], rdx
                        cmp              eax, 99
                                                                                        je    n10_var_α
                                                                                        jmp   n95_lit_string_α
n85_call_β:
                                                                                        jmp   n10_var_α
#-----------------------------------------------------------------------------------------------------------------------
n86_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        mov              qword ptr [rbp + 624], rax
                        mov              qword ptr [rbp + 632], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n87_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        mov              qword ptr [rbp + 912], rax
                        mov              qword ptr [rbp + 920], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n88_lit_integer_α:
                        mov              qword ptr [rbp + 1248], 6
                        mov              rax, qword ptr [rip + .Lx213_0]
                        mov              qword ptr [rbp + 1256], rax
                                                                                        jmp   n96_lit_integer_α
.Lx213_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n89_lit_integer_α:
                        mov              qword ptr [rbp + 1536], 6
                        mov              rax, qword ptr [rip + .Lx214_0]
                        mov              qword ptr [rbp + 1544], rax
                                                                                        jmp   n97_lit_integer_α
.Lx214_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n90_lit_integer_α:
                        mov              qword ptr [rbp + 1792], 6
                        mov              rax, qword ptr [rip + .Lx215_0]
                        mov              qword ptr [rbp + 1800], rax
                                                                                        jmp   n98_subscript_α
.Lx215_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n91_lit_integer_α:
                        mov              qword ptr [rbp + 2000], 6
                        mov              rax, qword ptr [rip + .Lx216_0]
                        mov              qword ptr [rbp + 2008], rax
                                                                                        jmp   n99_subscript_α
.Lx216_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n92_lit_integer_α:
                        mov              qword ptr [rbp + 2320], 6
                        mov              rax, qword ptr [rip + .Lx217_0]
                        mov              qword ptr [rbp + 2328], rax
                                                                                        jmp   n100_call_α
.Lx217_0:
                        .quad            2121
#-----------------------------------------------------------------------------------------------------------------------
n93_assign_var_α:
                        mov              rdi, qword ptr [rbp + 2480]
                        mov              rsi, qword ptr [rbp + 2488]
                        mov              rdx, qword ptr [rbp + 2576]
                        mov              rcx, qword ptr [rbp + 2584]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n49_var_α
                        mov              qword ptr [rbp + 2592], rax
                        mov              qword ptr [rbp + 2600], rdx
                                                                                        jmp   n49_var_α
#-----------------------------------------------------------------------------------------------------------------------
n94_call_α:
                        mov              rax, qword ptr [rbp + 2672]
                        mov              qword ptr [rbp + 2624], rax
                        mov              rax, qword ptr [rbp + 2680]
                        mov              qword ptr [rbp + 2632], rax
                        mov              rax, qword ptr [rbp + 2768]
                        mov              qword ptr [rbp + 2640], rax
                        mov              rax, qword ptr [rbp + 2776]
                        mov              qword ptr [rbp + 2648], rax
                        .section         .rodata
.Lbynamefn95:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn95]
                        lea              rsi, [rbp + 2624]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 2608], rax
                        mov              qword ptr [rbp + 2616], rdx
                        cmp              eax, 99
                                                                                        je    n16_lit_string_α
                                                                                        jmp   n101_lit_string_α
n94_call_β:
                                                                                        jmp   n16_lit_string_α
#=======================================================================================================================
#         output = 'FAIL 1114/001: item 1D assign/read'  :(end)
#-----------------------------------------------------------------------------------------------------------------------
n95_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              rax, qword ptr [rip + .Lx220_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n102_assign_α
.Lx220_0:
                        .quad            .Lx220_0_s
.Lx220_0_s:
                        .string          "FAIL 1114/001: item 1D assign/read"
#-----------------------------------------------------------------------------------------------------------------------
n96_lit_integer_α:
                        mov              qword ptr [rbp + 1264], 6
                        mov              rax, qword ptr [rip + .Lx221_0]
                        mov              qword ptr [rbp + 1272], rax
                                                                                        jmp   n103_call_α
.Lx221_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n97_lit_integer_α:
                        mov              qword ptr [rbp + 1552], 6
                        mov              rax, qword ptr [rip + .Lx222_0]
                        mov              qword ptr [rbp + 1560], rax
                                                                                        jmp   n104_call_α
.Lx222_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n98_subscript_α:
                        mov              rdi, qword ptr [rbp + 1776]
                        mov              rsi, qword ptr [rbp + 1784]
                        mov              rdx, qword ptr [rbp + 1792]
                        mov              rcx, qword ptr [rbp + 1800]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n14_var_α
                        mov              qword ptr [rbp + 1808], rax
                        mov              qword ptr [rbp + 1816], rdx
                                                                                        jmp   n105_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n99_subscript_α:
                        mov              rdi, qword ptr [rbp + 1984]
                        mov              rsi, qword ptr [rbp + 1992]
                        mov              rdx, qword ptr [rbp + 2000]
                        mov              rcx, qword ptr [rbp + 2008]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n25_var_α
                        mov              qword ptr [rbp + 2016], rax
                        mov              qword ptr [rbp + 2024], rdx
                                                                                        jmp   n106_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n100_call_α:
                        mov              rax, qword ptr [rbp + 2128]
                        mov              qword ptr [rbp + 2080], rax
                        mov              rax, qword ptr [rbp + 2136]
                        mov              qword ptr [rbp + 2088], rax
                        mov              rax, qword ptr [rbp + 2320]
                        mov              qword ptr [rbp + 2096], rax
                        mov              rax, qword ptr [rbp + 2328]
                        mov              qword ptr [rbp + 2104], rax
                        .section         .rodata
.Lbynamefn101:          .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn101]
                        lea              rsi, [rbp + 2080]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 2064], rax
                        mov              qword ptr [rbp + 2072], rdx
                        cmp              eax, 99
                                                                                        je    n15_call_α
                                                                                        jmp   n107_lit_string_α
n100_call_β:
                                                                                        jmp   n15_call_α
#=======================================================================================================================
#         output = 'FAIL 1114/007: item on table'        :(end)
#-----------------------------------------------------------------------------------------------------------------------
n101_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              rax, qword ptr [rip + .Lx226_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n108_assign_α
.Lx226_0:
                        .quad            .Lx226_0_s
.Lx226_0_s:
                        .string          "FAIL 1114/007: item on table"
#-----------------------------------------------------------------------------------------------------------------------
n102_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        mov              qword ptr [rbp + 448], rax
                        mov              qword ptr [rbp + 456], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n103_call_α:
                        mov              rax, qword ptr [rbp + 1200]
                        mov              qword ptr [rbp + 1104], rax
                        mov              rax, qword ptr [rbp + 1208]
                        mov              qword ptr [rbp + 1112], rax
                        mov              rax, qword ptr [rbp + 1216]
                        mov              qword ptr [rbp + 1120], rax
                        mov              rax, qword ptr [rbp + 1224]
                        mov              qword ptr [rbp + 1128], rax
                        mov              rax, qword ptr [rbp + 1232]
                        mov              qword ptr [rbp + 1136], rax
                        mov              rax, qword ptr [rbp + 1240]
                        mov              qword ptr [rbp + 1144], rax
                        mov              rax, qword ptr [rbp + 1248]
                        mov              qword ptr [rbp + 1152], rax
                        mov              rax, qword ptr [rbp + 1256]
                        mov              qword ptr [rbp + 1160], rax
                        mov              rax, qword ptr [rbp + 1264]
                        mov              qword ptr [rbp + 1168], rax
                        mov              rax, qword ptr [rbp + 1272]
                        mov              qword ptr [rbp + 1176], rax
                        .section         .rodata
.Lbynamefn104:          .string          "item"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn104]
                        lea              rsi, [rbp + 1104]
                        mov              edx, 5
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1088], rax
                        mov              qword ptr [rbp + 1096], rdx
                        cmp              eax, 99
                                                                                        je    n56_var_α
                                                                                        jmp   n109_lit_integer_α
n103_call_β:
                                                                                        jmp   n56_var_α
#-----------------------------------------------------------------------------------------------------------------------
n104_call_α:
                        mov              rax, qword ptr [rbp + 1488]
                        mov              qword ptr [rbp + 1392], rax
                        mov              rax, qword ptr [rbp + 1496]
                        mov              qword ptr [rbp + 1400], rax
                        mov              rax, qword ptr [rbp + 1504]
                        mov              qword ptr [rbp + 1408], rax
                        mov              rax, qword ptr [rbp + 1512]
                        mov              qword ptr [rbp + 1416], rax
                        mov              rax, qword ptr [rbp + 1520]
                        mov              qword ptr [rbp + 1424], rax
                        mov              rax, qword ptr [rbp + 1528]
                        mov              qword ptr [rbp + 1432], rax
                        mov              rax, qword ptr [rbp + 1536]
                        mov              qword ptr [rbp + 1440], rax
                        mov              rax, qword ptr [rbp + 1544]
                        mov              qword ptr [rbp + 1448], rax
                        mov              rax, qword ptr [rbp + 1552]
                        mov              qword ptr [rbp + 1456], rax
                        mov              rax, qword ptr [rbp + 1560]
                        mov              qword ptr [rbp + 1464], rax
                        .section         .rodata
.Lbynamefn105:          .string          "item"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn105]
                        lea              rsi, [rbp + 1392]
                        mov              edx, 5
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1376], rax
                        mov              qword ptr [rbp + 1384], rdx
                        cmp              eax, 99
                                                                                        je    n13_var_α
                                                                                        jmp   n110_lit_integer_α
n104_call_β:
                                                                                        jmp   n13_var_α
#-----------------------------------------------------------------------------------------------------------------------
n105_deref_α:
                        mov              rdi, qword ptr [rbp + 1808]
                        mov              rsi, qword ptr [rbp + 1816]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n14_var_α
                        mov              qword ptr [rbp + 1824], rax
                        mov              qword ptr [rbp + 1832], rdx
                                                                                        jmp   n111_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n106_lit_integer_α:
                        mov              qword ptr [rbp + 2032], 6
                        mov              rax, qword ptr [rip + .Lx231_0]
                        mov              qword ptr [rbp + 2040], rax
                                                                                        jmp   n112_assign_var_α
.Lx231_0:
                        .quad            2121
#=======================================================================================================================
#         output = 'FAIL 1114/006: bracket 4D assign, item read' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n107_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              rax, qword ptr [rip + .Lx232_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n113_assign_α
.Lx232_0:
                        .quad            .Lx232_0_s
.Lx232_0_s:
                        .string          "FAIL 1114/006: bracket 4D assign, item read"
#-----------------------------------------------------------------------------------------------------------------------
n108_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        mov              qword ptr [rbp + 2784], rax
                        mov              qword ptr [rbp + 2792], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n109_lit_integer_α:
                        mov              qword ptr [rbp + 1280], 6
                        mov              rax, qword ptr [rip + .Lx234_0]
                        mov              qword ptr [rbp + 1288], rax
                                                                                        jmp   n114_assign_var_α
.Lx234_0:
                        .quad            1212
#-----------------------------------------------------------------------------------------------------------------------
n110_lit_integer_α:
                        mov              qword ptr [rbp + 1568], 6
                        mov              rax, qword ptr [rip + .Lx235_0]
                        mov              qword ptr [rbp + 1576], rax
                                                                                        jmp   n115_call_α
.Lx235_0:
                        .quad            1212
#-----------------------------------------------------------------------------------------------------------------------
n111_lit_integer_α:
                        mov              qword ptr [rbp + 1840], 6
                        mov              rax, qword ptr [rip + .Lx236_0]
                        mov              qword ptr [rbp + 1848], rax
                                                                                        jmp   n116_call_α
.Lx236_0:
                        .quad            1212
#-----------------------------------------------------------------------------------------------------------------------
n112_assign_var_α:
                        mov              rdi, qword ptr [rbp + 2016]
                        mov              rsi, qword ptr [rbp + 2024]
                        mov              rdx, qword ptr [rbp + 2032]
                        mov              rcx, qword ptr [rbp + 2040]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n25_var_α
                        mov              qword ptr [rbp + 2048], rax
                        mov              qword ptr [rbp + 2056], rdx
                                                                                        jmp   n25_var_α
#-----------------------------------------------------------------------------------------------------------------------
n113_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        mov              qword ptr [rbp + 2336], rax
                        mov              qword ptr [rbp + 2344], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n114_assign_var_α:
                        mov              rdi, qword ptr [rbp + 1088]
                        mov              rsi, qword ptr [rbp + 1096]
                        mov              rdx, qword ptr [rbp + 1280]
                        mov              rcx, qword ptr [rbp + 1288]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n56_var_α
                        mov              qword ptr [rbp + 1296], rax
                        mov              qword ptr [rbp + 1304], rdx
                                                                                        jmp   n56_var_α
#-----------------------------------------------------------------------------------------------------------------------
n115_call_α:
                        mov              rax, qword ptr [rbp + 1376]
                        mov              qword ptr [rbp + 1328], rax
                        mov              rax, qword ptr [rbp + 1384]
                        mov              qword ptr [rbp + 1336], rax
                        mov              rax, qword ptr [rbp + 1568]
                        mov              qword ptr [rbp + 1344], rax
                        mov              rax, qword ptr [rbp + 1576]
                        mov              qword ptr [rbp + 1352], rax
                        .section         .rodata
.Lbynamefn116:          .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn116]
                        lea              rsi, [rbp + 1328]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1312], rax
                        mov              qword ptr [rbp + 1320], rdx
                        cmp              eax, 99
                                                                                        je    n13_var_α
                                                                                        jmp   n117_lit_string_α
n115_call_β:
                                                                                        jmp   n13_var_α
#-----------------------------------------------------------------------------------------------------------------------
n116_call_α:
                        mov              rax, qword ptr [rbp + 1824]
                        mov              qword ptr [rbp + 1632], rax
                        mov              rax, qword ptr [rbp + 1832]
                        mov              qword ptr [rbp + 1640], rax
                        mov              rax, qword ptr [rbp + 1840]
                        mov              qword ptr [rbp + 1648], rax
                        mov              rax, qword ptr [rbp + 1848]
                        mov              qword ptr [rbp + 1656], rax
                        .section         .rodata
.Lbynamefn117:          .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn117]
                        lea              rsi, [rbp + 1632]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1616], rax
                        mov              qword ptr [rbp + 1624], rdx
                        cmp              eax, 99
                                                                                        je    n14_var_α
                                                                                        jmp   n118_lit_string_α
n116_call_β:
                                                                                        jmp   n14_var_α
#=======================================================================================================================
#         output = 'FAIL 1114/004: item 4D assign/read'  :(end)
#-----------------------------------------------------------------------------------------------------------------------
n117_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              rax, qword ptr [rip + .Lx242_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n119_assign_α
.Lx242_0:
                        .quad            .Lx242_0_s
.Lx242_0_s:
                        .string          "FAIL 1114/004: item 4D assign/read"
#=======================================================================================================================
#         output = 'FAIL 1114/005: item 4D == bracket'   :(end)
#-----------------------------------------------------------------------------------------------------------------------
n118_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              rax, qword ptr [rip + .Lx243_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n120_assign_α
.Lx243_0:
                        .quad            .Lx243_0_s
.Lx243_0_s:
                        .string          "FAIL 1114/005: item 4D == bracket"
#-----------------------------------------------------------------------------------------------------------------------
n119_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        mov              qword ptr [rbp + 1584], rax
                        mov              qword ptr [rbp + 1592], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n120_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        mov              qword ptr [rbp + 1856], rax
                        mov              qword ptr [rbp + 1864], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        mov              rsp, rbp
                        mov              rbp, [rsp + 2848]
                        add              rsp, 2856
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              rsp, rbp
                        mov              eax, 99
                        xor              edx, edx
                        mov              rbp, [rsp + 2848]
                        add              rsp, 2856
                        ret
                        .section         .note.GNU-stack,"",@progbits

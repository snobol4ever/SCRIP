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
                        sub              rsp, 2696
                        mov              rdi, rsp
                        mov              ecx, 2696
                        xor              eax, eax
                        rep stosb
main_α_body:
#=======================================================================================================================
#         aaa = array(10)
#-----------------------------------------------------------------------------------------------------------------------
n0_lit_integer_α:
                        mov              qword ptr [rsp + 64], 6
                        mov              rax, qword ptr [rip + .Lx121_0]
                        mov              qword ptr [rsp + 72], rax
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
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        .section         .rodata
.Lbynamefn10:           .string          "array"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn10]
                        lea              rsi, [rsp + 32]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rdx
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
                        mov              qword ptr [rsp + 528], rax
                        mov              qword ptr [rsp + 536], rdx
                                                                                        jmp   n19_lit_integer_α
#=======================================================================================================================
#         aaa<2> = 22
#-----------------------------------------------------------------------------------------------------------------------
n11_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 624], rax
                        mov              qword ptr [rsp + 632], rdx
                                                                                        jmp   n20_lit_integer_α
#=======================================================================================================================
#         ama = array('2,2,2,2')
#-----------------------------------------------------------------------------------------------------------------------
n12_lit_string_α:
                        mov              qword ptr [rsp + 944], 1
                        mov              dword ptr [rsp + 948], 7
                        mov              rax, qword ptr [rip + .Lx133_0]
                        mov              qword ptr [rsp + 952], rax
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
                        mov              qword ptr [rsp + 1600], rax
                        mov              qword ptr [rsp + 1608], rdx
                                                                                        jmp   n23_lit_integer_α
#=======================================================================================================================
#         ama<2,1,2,1> = 2121
#-----------------------------------------------------------------------------------------------------------------------
n14_var_α:
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 1792], rax
                        mov              qword ptr [rsp + 1800], rdx
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
                        lea              rsi, [rsp + 2272]
                        mov              edx, 0
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 2256], rax
                        mov              qword ptr [rsp + 2264], rdx
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
                        mov              dword ptr [rsp + 4], 20
                        mov              rax, qword ptr [rip + .Lx138_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n28_assign_α
.Lx138_0:
                        .quad            .Lx138_0_s
.Lx138_0_s:
                        .string          "PASS 1114_item (7/7)"
#-----------------------------------------------------------------------------------------------------------------------
n17_assign_α:
                        mov              rax, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rsp + 24]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n18_lit_string_α
#=======================================================================================================================
#         aaa = array(10)
#         item(aaa, 1) = 5
#-----------------------------------------------------------------------------------------------------------------------
n18_lit_string_α:
                        mov              qword ptr [rsp + 80], 1
                        mov              dword ptr [rsp + 84], 0
                        mov              rax, qword ptr [rip + .Lx140_0]
                        mov              qword ptr [rsp + 88], rax
                                                                                        jmp   n29_call_α
.Lx140_0:
                        .quad            .Lx140_0_s
.Lx140_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n19_lit_integer_α:
                        mov              qword ptr [rsp + 544], 6
                        mov              rax, qword ptr [rip + .Lx141_0]
                        mov              qword ptr [rsp + 552], rax
                                                                                        jmp   n30_subscript_α
.Lx141_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n20_lit_integer_α:
                        mov              qword ptr [rsp + 640], 6
                        mov              rax, qword ptr [rip + .Lx142_0]
                        mov              qword ptr [rsp + 648], rax
                                                                                        jmp   n31_subscript_α
.Lx142_0:
                        .quad            2
#=======================================================================================================================
#         differ(item(aaa, 2), 22)                   :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n21_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 832], rax
                        mov              qword ptr [rsp + 840], rdx
                                                                                        jmp   n32_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n22_call_α:
                        mov              rax, qword ptr [rsp + 944]
                        mov              qword ptr [rsp + 912], rax
                        mov              rax, qword ptr [rsp + 952]
                        mov              qword ptr [rsp + 920], rax
                        .section         .rodata
.Lbynamefn23:           .string          "array"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn23]
                        lea              rsi, [rsp + 912]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 896], rax
                        mov              qword ptr [rsp + 904], rdx
                        cmp              eax, 99
                                                                                        je    n34_lit_string_α
                                                                                        jmp   n33_assign_α
n22_call_β:
                                                                                        jmp   n34_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n23_lit_integer_α:
                        mov              qword ptr [rsp + 1616], 6
                        mov              rax, qword ptr [rip + .Lx145_0]
                        mov              qword ptr [rsp + 1624], rax
                                                                                        jmp   n35_subscript_α
.Lx145_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n24_lit_integer_α:
                        mov              qword ptr [rsp + 1808], 6
                        mov              rax, qword ptr [rip + .Lx146_0]
                        mov              qword ptr [rsp + 1816], rax
                                                                                        jmp   n36_subscript_α
.Lx146_0:
                        .quad            2
#=======================================================================================================================
#         differ(item(ama, 2,1,2,1), 2121)                   :f(e006)
#-----------------------------------------------------------------------------------------------------------------------
n25_var_α:
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 2144], rax
                        mov              qword ptr [rsp + 2152], rdx
                                                                                        jmp   n37_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n26_assign_α:
                        mov              rax, qword ptr [rsp + 2256]
                        mov              rdx, qword ptr [rsp + 2264]
                        mov              qword ptr [1879052336], rax
                        mov              qword ptr [1879052344], rdx
                                                                                        jmp   n27_lit_string_α
#=======================================================================================================================
#         item(tt, 'key') = 'val'
#-----------------------------------------------------------------------------------------------------------------------
n27_lit_string_α:
                        mov              qword ptr [rsp + 2288], 1
                        mov              dword ptr [rsp + 2292], 0
                        mov              rax, qword ptr [rip + .Lx149_0]
                        mov              qword ptr [rsp + 2296], rax
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
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n29_call_α:
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 112], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 120], rax
                        .section         .rodata
.Lrkfn152:              .string          "SNO$WANTNM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn152]
                        lea              rsi, [rsp + 112]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 96], rax
                        mov              qword ptr [rsp + 104], rdx
                        cmp              eax, 99
                                                                                        je    n40_var_α
                                                                                        jmp   n39_var_α
n29_call_β:
                                                                                        jmp   n40_var_α
#-----------------------------------------------------------------------------------------------------------------------
n30_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 544]
                        mov              rsi, qword ptr [rsp + 552]
                        mov              rdx, qword ptr [rsp + 560]
                        mov              rcx, qword ptr [rsp + 568]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx153_240
                        add              rsp, 16
                                                                                        jmp   n11_var_α
.Lx153_240:
                        mov              qword ptr [rsp + 576], rax
                        mov              qword ptr [rsp + 584], rdx
                        add              rsp, 16
                                                                                        jmp   n41_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n31_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 640]
                        mov              rsi, qword ptr [rsp + 648]
                        mov              rdx, qword ptr [rsp + 656]
                        mov              rcx, qword ptr [rsp + 664]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx154_240
                        add              rsp, 16
                                                                                        jmp   n21_var_α
.Lx154_240:
                        mov              qword ptr [rsp + 672], rax
                        mov              qword ptr [rsp + 680], rdx
                        add              rsp, 16
                                                                                        jmp   n42_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n32_lit_integer_α:
                        mov              qword ptr [rsp + 848], 6
                        mov              rax, qword ptr [rip + .Lx155_0]
                        mov              qword ptr [rsp + 856], rax
                                                                                        jmp   n43_call_α
.Lx155_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n33_assign_α:
                        mov              rax, qword ptr [rsp + 896]
                        mov              rdx, qword ptr [rsp + 904]
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rdx
                                                                                        jmp   n34_lit_string_α
#=======================================================================================================================
#         item(ama, 1,2,1,2) = 1212
#-----------------------------------------------------------------------------------------------------------------------
n34_lit_string_α:
                        mov              qword ptr [rsp + 960], 1
                        mov              dword ptr [rsp + 964], 0
                        mov              rax, qword ptr [rip + .Lx157_0]
                        mov              qword ptr [rsp + 968], rax
                                                                                        jmp   n44_call_α
.Lx157_0:
                        .quad            .Lx157_0_s
.Lx157_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n35_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1616]
                        mov              rsi, qword ptr [rsp + 1624]
                        mov              rdx, qword ptr [rsp + 1632]
                        mov              rcx, qword ptr [rsp + 1640]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx158_240
                        add              rsp, 16
                                                                                        jmp   n14_var_α
.Lx158_240:
                        mov              qword ptr [rsp + 1648], rax
                        mov              qword ptr [rsp + 1656], rdx
                        add              rsp, 16
                                                                                        jmp   n45_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n36_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1808]
                        mov              rsi, qword ptr [rsp + 1816]
                        mov              rdx, qword ptr [rsp + 1824]
                        mov              rcx, qword ptr [rsp + 1832]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx159_240
                        add              rsp, 16
                                                                                        jmp   n25_var_α
.Lx159_240:
                        mov              qword ptr [rsp + 1840], rax
                        mov              qword ptr [rsp + 1848], rdx
                        add              rsp, 16
                                                                                        jmp   n46_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n37_lit_integer_α:
                        mov              qword ptr [rsp + 2160], 6
                        mov              rax, qword ptr [rip + .Lx160_0]
                        mov              qword ptr [rsp + 2168], rax
                                                                                        jmp   n47_lit_integer_α
.Lx160_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n38_call_α:
                        mov              rax, qword ptr [rsp + 2288]
                        mov              qword ptr [rsp + 2320], rax
                        mov              rax, qword ptr [rsp + 2296]
                        mov              qword ptr [rsp + 2328], rax
                        .section         .rodata
.Lrkfn162:              .string          "SNO$WANTNM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn162]
                        lea              rsi, [rsp + 2320]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 2304], rax
                        mov              qword ptr [rsp + 2312], rdx
                        cmp              eax, 99
                                                                                        je    n49_var_α
                                                                                        jmp   n48_var_α
n38_call_β:
                                                                                        jmp   n49_var_α
#-----------------------------------------------------------------------------------------------------------------------
n39_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 208], rax
                        mov              qword ptr [rsp + 216], rdx
                                                                                        jmp   n50_lit_integer_α
#=======================================================================================================================
#         differ(item(aaa, 1), 5)                   :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n40_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 400], rax
                        mov              qword ptr [rsp + 408], rdx
                                                                                        jmp   n51_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n41_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 576]
                        mov              rsi, qword ptr [rsp + 584]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx165_240
                        add              rsp, 16
                                                                                        jmp   n11_var_α
.Lx165_240:
                        mov              qword ptr [rsp + 592], rax
                        mov              qword ptr [rsp + 600], rdx
                        add              rsp, 16
                                                                                        jmp   n52_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n42_lit_integer_α:
                        mov              qword ptr [rsp + 672], 6
                        mov              rax, qword ptr [rip + .Lx166_0]
                        mov              qword ptr [rsp + 680], rax
                                                                                        jmp   n53_assign_var_α
.Lx166_0:
                        .quad            22
#-----------------------------------------------------------------------------------------------------------------------
n43_call_α:
                        mov              rax, qword ptr [rsp + 832]
                        mov              qword ptr [rsp + 784], rax
                        mov              rax, qword ptr [rsp + 840]
                        mov              qword ptr [rsp + 792], rax
                        mov              rax, qword ptr [rsp + 848]
                        mov              qword ptr [rsp + 800], rax
                        mov              rax, qword ptr [rsp + 856]
                        mov              qword ptr [rsp + 808], rax
                        .section         .rodata
.Lbynamefn44:           .string          "item"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn44]
                        lea              rsi, [rsp + 784]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 768], rax
                        mov              qword ptr [rsp + 776], rdx
                        cmp              eax, 99
                                                                                        je    n12_lit_string_α
                                                                                        jmp   n54_lit_integer_α
n43_call_β:
                                                                                        jmp   n12_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n44_call_α:
                        mov              rax, qword ptr [rsp + 960]
                        mov              qword ptr [rsp + 992], rax
                        mov              rax, qword ptr [rsp + 968]
                        mov              qword ptr [rsp + 1000], rax
                        .section         .rodata
.Lrkfn169:              .string          "SNO$WANTNM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn169]
                        lea              rsi, [rsp + 992]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 976], rax
                        mov              qword ptr [rsp + 984], rdx
                        cmp              eax, 99
                                                                                        je    n56_var_α
                                                                                        jmp   n55_var_α
n44_call_β:
                                                                                        jmp   n56_var_α
#-----------------------------------------------------------------------------------------------------------------------
n45_lit_integer_α:
                        mov              qword ptr [rsp + 1648], 6
                        mov              rax, qword ptr [rip + .Lx170_0]
                        mov              qword ptr [rsp + 1656], rax
                                                                                        jmp   n57_subscript_α
.Lx170_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n46_lit_integer_α:
                        mov              qword ptr [rsp + 1840], 6
                        mov              rax, qword ptr [rip + .Lx171_0]
                        mov              qword ptr [rsp + 1848], rax
                                                                                        jmp   n58_subscript_α
.Lx171_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n47_lit_integer_α:
                        mov              qword ptr [rsp + 2176], 6
                        mov              rax, qword ptr [rip + .Lx172_0]
                        mov              qword ptr [rsp + 2184], rax
                                                                                        jmp   n59_lit_integer_α
.Lx172_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n48_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 2416], rax
                        mov              qword ptr [rsp + 2424], rdx
                                                                                        jmp   n60_lit_string_α
#=======================================================================================================================
#         differ(item(tt, 'key'), 'val')                   :f(e007)
#-----------------------------------------------------------------------------------------------------------------------
n49_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 2608], rax
                        mov              qword ptr [rsp + 2616], rdx
                                                                                        jmp   n61_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n50_lit_integer_α:
                        mov              qword ptr [rsp + 224], 6
                        mov              rax, qword ptr [rip + .Lx175_0]
                        mov              qword ptr [rsp + 232], rax
                                                                                        jmp   n62_call_α
.Lx175_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n51_lit_integer_α:
                        mov              qword ptr [rsp + 416], 6
                        mov              rax, qword ptr [rip + .Lx176_0]
                        mov              qword ptr [rsp + 424], rax
                                                                                        jmp   n63_call_α
.Lx176_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n52_lit_integer_α:
                        mov              qword ptr [rsp + 592], 6
                        mov              rax, qword ptr [rip + .Lx177_0]
                        mov              qword ptr [rsp + 600], rax
                                                                                        jmp   n64_call_α
.Lx177_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n53_assign_var_α:
                        mov              rdi, qword ptr [rsp + 656]
                        mov              rsi, qword ptr [rsp + 664]
                        mov              rdx, qword ptr [rsp + 672]
                        mov              rcx, qword ptr [rsp + 680]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n21_var_α
                        mov              qword ptr [rsp + 688], rax
                        mov              qword ptr [rsp + 696], rdx
                                                                                        jmp   n21_var_α
#-----------------------------------------------------------------------------------------------------------------------
n54_lit_integer_α:
                        mov              qword ptr [rsp + 864], 6
                        mov              rax, qword ptr [rip + .Lx179_0]
                        mov              qword ptr [rsp + 872], rax
                                                                                        jmp   n65_call_α
.Lx179_0:
                        .quad            22
#-----------------------------------------------------------------------------------------------------------------------
n55_var_α:
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 1136], rax
                        mov              qword ptr [rsp + 1144], rdx
                                                                                        jmp   n66_lit_integer_α
#=======================================================================================================================
#         differ(item(ama, 1,2,1,2), 1212)                   :f(e004)
#-----------------------------------------------------------------------------------------------------------------------
n56_var_α:
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 1424], rax
                        mov              qword ptr [rsp + 1432], rdx
                                                                                        jmp   n67_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n57_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1648]
                        mov              rsi, qword ptr [rsp + 1656]
                        mov              rdx, qword ptr [rsp + 1664]
                        mov              rcx, qword ptr [rsp + 1672]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx182_240
                        add              rsp, 16
                                                                                        jmp   n14_var_α
.Lx182_240:
                        mov              qword ptr [rsp + 1680], rax
                        mov              qword ptr [rsp + 1688], rdx
                        add              rsp, 16
                                                                                        jmp   n68_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n58_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1840]
                        mov              rsi, qword ptr [rsp + 1848]
                        mov              rdx, qword ptr [rsp + 1856]
                        mov              rcx, qword ptr [rsp + 1864]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx183_240
                        add              rsp, 16
                                                                                        jmp   n25_var_α
.Lx183_240:
                        mov              qword ptr [rsp + 1872], rax
                        mov              qword ptr [rsp + 1880], rdx
                        add              rsp, 16
                                                                                        jmp   n69_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n59_lit_integer_α:
                        mov              qword ptr [rsp + 2192], 6
                        mov              rax, qword ptr [rip + .Lx184_0]
                        mov              qword ptr [rsp + 2200], rax
                                                                                        jmp   n70_lit_integer_α
.Lx184_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n60_lit_string_α:
                        mov              qword ptr [rsp + 2432], 1
                        mov              dword ptr [rsp + 2436], 3
                        mov              rax, qword ptr [rip + .Lx185_0]
                        mov              qword ptr [rsp + 2440], rax
                                                                                        jmp   n71_call_α
.Lx185_0:
                        .quad            .Lx185_0_s
.Lx185_0_s:
                        .string          "key"
#-----------------------------------------------------------------------------------------------------------------------
n61_lit_string_α:
                        mov              qword ptr [rsp + 2624], 1
                        mov              dword ptr [rsp + 2628], 3
                        mov              rax, qword ptr [rip + .Lx186_0]
                        mov              qword ptr [rsp + 2632], rax
                                                                                        jmp   n72_call_α
.Lx186_0:
                        .quad            .Lx186_0_s
.Lx186_0_s:
                        .string          "key"
#-----------------------------------------------------------------------------------------------------------------------
n62_call_α:
                        mov              rax, qword ptr [rsp + 208]
                        mov              qword ptr [rsp + 160], rax
                        mov              rax, qword ptr [rsp + 216]
                        mov              qword ptr [rsp + 168], rax
                        mov              rax, qword ptr [rsp + 224]
                        mov              qword ptr [rsp + 176], rax
                        mov              rax, qword ptr [rsp + 232]
                        mov              qword ptr [rsp + 184], rax
                        .section         .rodata
.Lbynamefn63:           .string          "item"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn63]
                        lea              rsi, [rsp + 160]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 144], rax
                        mov              qword ptr [rsp + 152], rdx
                        cmp              eax, 99
                                                                                        je    n40_var_α
                                                                                        jmp   n73_lit_integer_α
n62_call_β:
                                                                                        jmp   n40_var_α
#-----------------------------------------------------------------------------------------------------------------------
n63_call_α:
                        mov              rax, qword ptr [rsp + 400]
                        mov              qword ptr [rsp + 352], rax
                        mov              rax, qword ptr [rsp + 408]
                        mov              qword ptr [rsp + 360], rax
                        mov              rax, qword ptr [rsp + 416]
                        mov              qword ptr [rsp + 368], rax
                        mov              rax, qword ptr [rsp + 424]
                        mov              qword ptr [rsp + 376], rax
                        .section         .rodata
.Lbynamefn64:           .string          "item"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn64]
                        lea              rsi, [rsp + 352]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 336], rax
                        mov              qword ptr [rsp + 344], rdx
                        cmp              eax, 99
                                                                                        je    n10_var_α
                                                                                        jmp   n74_lit_integer_α
n63_call_β:
                                                                                        jmp   n10_var_α
#-----------------------------------------------------------------------------------------------------------------------
n64_call_α:
                        mov              rax, qword ptr [rsp + 576]
                        mov              qword ptr [rsp + 480], rax
                        mov              rax, qword ptr [rsp + 584]
                        mov              qword ptr [rsp + 488], rax
                        mov              rax, qword ptr [rsp + 592]
                        mov              qword ptr [rsp + 496], rax
                        mov              rax, qword ptr [rsp + 600]
                        mov              qword ptr [rsp + 504], rax
                        .section         .rodata
.Lbynamefn65:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn65]
                        lea              rsi, [rsp + 480]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 464], rax
                        mov              qword ptr [rsp + 472], rdx
                        cmp              eax, 99
                                                                                        je    n11_var_α
                                                                                        jmp   n75_lit_string_α
n64_call_β:
                                                                                        jmp   n11_var_α
#-----------------------------------------------------------------------------------------------------------------------
n65_call_α:
                        mov              rax, qword ptr [rsp + 768]
                        mov              qword ptr [rsp + 720], rax
                        mov              rax, qword ptr [rsp + 776]
                        mov              qword ptr [rsp + 728], rax
                        mov              rax, qword ptr [rsp + 864]
                        mov              qword ptr [rsp + 736], rax
                        mov              rax, qword ptr [rsp + 872]
                        mov              qword ptr [rsp + 744], rax
                        .section         .rodata
.Lbynamefn66:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn66]
                        lea              rsi, [rsp + 720]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 704], rax
                        mov              qword ptr [rsp + 712], rdx
                        cmp              eax, 99
                                                                                        je    n12_lit_string_α
                                                                                        jmp   n76_lit_string_α
n65_call_β:
                                                                                        jmp   n12_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n66_lit_integer_α:
                        mov              qword ptr [rsp + 1152], 6
                        mov              rax, qword ptr [rip + .Lx191_0]
                        mov              qword ptr [rsp + 1160], rax
                                                                                        jmp   n77_lit_integer_α
.Lx191_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n67_lit_integer_α:
                        mov              qword ptr [rsp + 1440], 6
                        mov              rax, qword ptr [rip + .Lx192_0]
                        mov              qword ptr [rsp + 1448], rax
                                                                                        jmp   n78_lit_integer_α
.Lx192_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n68_lit_integer_α:
                        mov              qword ptr [rsp + 1680], 6
                        mov              rax, qword ptr [rip + .Lx193_0]
                        mov              qword ptr [rsp + 1688], rax
                                                                                        jmp   n79_subscript_α
.Lx193_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n69_lit_integer_α:
                        mov              qword ptr [rsp + 1872], 6
                        mov              rax, qword ptr [rip + .Lx194_0]
                        mov              qword ptr [rsp + 1880], rax
                                                                                        jmp   n80_subscript_α
.Lx194_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n70_lit_integer_α:
                        mov              qword ptr [rsp + 2208], 6
                        mov              rax, qword ptr [rip + .Lx195_0]
                        mov              qword ptr [rsp + 2216], rax
                                                                                        jmp   n81_call_α
.Lx195_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n71_call_α:
                        mov              rax, qword ptr [rsp + 2416]
                        mov              qword ptr [rsp + 2368], rax
                        mov              rax, qword ptr [rsp + 2424]
                        mov              qword ptr [rsp + 2376], rax
                        mov              rax, qword ptr [rsp + 2432]
                        mov              qword ptr [rsp + 2384], rax
                        mov              rax, qword ptr [rsp + 2440]
                        mov              qword ptr [rsp + 2392], rax
                        .section         .rodata
.Lbynamefn72:           .string          "item"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn72]
                        lea              rsi, [rsp + 2368]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 2352], rax
                        mov              qword ptr [rsp + 2360], rdx
                        cmp              eax, 99
                                                                                        je    n49_var_α
                                                                                        jmp   n82_lit_string_α
n71_call_β:
                                                                                        jmp   n49_var_α
#-----------------------------------------------------------------------------------------------------------------------
n72_call_α:
                        mov              rax, qword ptr [rsp + 2608]
                        mov              qword ptr [rsp + 2560], rax
                        mov              rax, qword ptr [rsp + 2616]
                        mov              qword ptr [rsp + 2568], rax
                        mov              rax, qword ptr [rsp + 2624]
                        mov              qword ptr [rsp + 2576], rax
                        mov              rax, qword ptr [rsp + 2632]
                        mov              qword ptr [rsp + 2584], rax
                        .section         .rodata
.Lbynamefn73:           .string          "item"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn73]
                        lea              rsi, [rsp + 2560]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 2544], rax
                        mov              qword ptr [rsp + 2552], rdx
                        cmp              eax, 99
                                                                                        je    n16_lit_string_α
                                                                                        jmp   n83_lit_string_α
n72_call_β:
                                                                                        jmp   n16_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n73_lit_integer_α:
                        mov              qword ptr [rsp + 240], 6
                        mov              rax, qword ptr [rip + .Lx198_0]
                        mov              qword ptr [rsp + 248], rax
                                                                                        jmp   n84_assign_var_α
.Lx198_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n74_lit_integer_α:
                        mov              qword ptr [rsp + 432], 6
                        mov              rax, qword ptr [rip + .Lx199_0]
                        mov              qword ptr [rsp + 440], rax
                                                                                        jmp   n85_call_α
.Lx199_0:
                        .quad            5
#=======================================================================================================================
#         output = 'FAIL 1114/002: item == bracket read' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n75_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 35
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
                        mov              dword ptr [rsp + 4], 40
                        mov              rax, qword ptr [rip + .Lx201_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n87_assign_α
.Lx201_0:
                        .quad            .Lx201_0_s
.Lx201_0_s:
                        .string          "FAIL 1114/003: bracket assign, item read"
#-----------------------------------------------------------------------------------------------------------------------
n77_lit_integer_α:
                        mov              qword ptr [rsp + 1168], 6
                        mov              rax, qword ptr [rip + .Lx202_0]
                        mov              qword ptr [rsp + 1176], rax
                                                                                        jmp   n88_lit_integer_α
.Lx202_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n78_lit_integer_α:
                        mov              qword ptr [rsp + 1456], 6
                        mov              rax, qword ptr [rip + .Lx203_0]
                        mov              qword ptr [rsp + 1464], rax
                                                                                        jmp   n89_lit_integer_α
.Lx203_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n79_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1680]
                        mov              rsi, qword ptr [rsp + 1688]
                        mov              rdx, qword ptr [rsp + 1696]
                        mov              rcx, qword ptr [rsp + 1704]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx204_240
                        add              rsp, 16
                                                                                        jmp   n14_var_α
.Lx204_240:
                        mov              qword ptr [rsp + 1712], rax
                        mov              qword ptr [rsp + 1720], rdx
                        add              rsp, 16
                                                                                        jmp   n90_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n80_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1872]
                        mov              rsi, qword ptr [rsp + 1880]
                        mov              rdx, qword ptr [rsp + 1888]
                        mov              rcx, qword ptr [rsp + 1896]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx205_240
                        add              rsp, 16
                                                                                        jmp   n25_var_α
.Lx205_240:
                        mov              qword ptr [rsp + 1904], rax
                        mov              qword ptr [rsp + 1912], rdx
                        add              rsp, 16
                                                                                        jmp   n91_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n81_call_α:
                        mov              rax, qword ptr [rsp + 2144]
                        mov              qword ptr [rsp + 2048], rax
                        mov              rax, qword ptr [rsp + 2152]
                        mov              qword ptr [rsp + 2056], rax
                        mov              rax, qword ptr [rsp + 2160]
                        mov              qword ptr [rsp + 2064], rax
                        mov              rax, qword ptr [rsp + 2168]
                        mov              qword ptr [rsp + 2072], rax
                        mov              rax, qword ptr [rsp + 2176]
                        mov              qword ptr [rsp + 2080], rax
                        mov              rax, qword ptr [rsp + 2184]
                        mov              qword ptr [rsp + 2088], rax
                        mov              rax, qword ptr [rsp + 2192]
                        mov              qword ptr [rsp + 2096], rax
                        mov              rax, qword ptr [rsp + 2200]
                        mov              qword ptr [rsp + 2104], rax
                        mov              rax, qword ptr [rsp + 2208]
                        mov              qword ptr [rsp + 2112], rax
                        mov              rax, qword ptr [rsp + 2216]
                        mov              qword ptr [rsp + 2120], rax
                        .section         .rodata
.Lbynamefn82:           .string          "item"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn82]
                        lea              rsi, [rsp + 2048]
                        mov              edx, 5
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 2032], rax
                        mov              qword ptr [rsp + 2040], rdx
                        cmp              eax, 99
                                                                                        je    n15_call_α
                                                                                        jmp   n92_lit_integer_α
n81_call_β:
                                                                                        jmp   n15_call_α
#-----------------------------------------------------------------------------------------------------------------------
n82_lit_string_α:
                        mov              qword ptr [rsp + 2448], 1
                        mov              dword ptr [rsp + 2452], 3
                        mov              rax, qword ptr [rip + .Lx207_0]
                        mov              qword ptr [rsp + 2456], rax
                                                                                        jmp   n93_assign_var_α
.Lx207_0:
                        .quad            .Lx207_0_s
.Lx207_0_s:
                        .string          "val"
#-----------------------------------------------------------------------------------------------------------------------
n83_lit_string_α:
                        mov              qword ptr [rsp + 2640], 1
                        mov              dword ptr [rsp + 2644], 3
                        mov              rax, qword ptr [rip + .Lx208_0]
                        mov              qword ptr [rsp + 2648], rax
                                                                                        jmp   n94_call_α
.Lx208_0:
                        .quad            .Lx208_0_s
.Lx208_0_s:
                        .string          "val"
#-----------------------------------------------------------------------------------------------------------------------
n84_assign_var_α:
                        mov              rdi, qword ptr [rsp + 144]
                        mov              rsi, qword ptr [rsp + 152]
                        mov              rdx, qword ptr [rsp + 240]
                        mov              rcx, qword ptr [rsp + 248]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n40_var_α
                        mov              qword ptr [rsp + 256], rax
                        mov              qword ptr [rsp + 264], rdx
                                                                                        jmp   n40_var_α
#-----------------------------------------------------------------------------------------------------------------------
n85_call_α:
                        mov              rax, qword ptr [rsp + 336]
                        mov              qword ptr [rsp + 288], rax
                        mov              rax, qword ptr [rsp + 344]
                        mov              qword ptr [rsp + 296], rax
                        mov              rax, qword ptr [rsp + 432]
                        mov              qword ptr [rsp + 304], rax
                        mov              rax, qword ptr [rsp + 440]
                        mov              qword ptr [rsp + 312], rax
                        .section         .rodata
.Lbynamefn86:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn86]
                        lea              rsi, [rsp + 288]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 272], rax
                        mov              qword ptr [rsp + 280], rdx
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
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n87_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n88_lit_integer_α:
                        mov              qword ptr [rsp + 1184], 6
                        mov              rax, qword ptr [rip + .Lx213_0]
                        mov              qword ptr [rsp + 1192], rax
                                                                                        jmp   n96_lit_integer_α
.Lx213_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n89_lit_integer_α:
                        mov              qword ptr [rsp + 1472], 6
                        mov              rax, qword ptr [rip + .Lx214_0]
                        mov              qword ptr [rsp + 1480], rax
                                                                                        jmp   n97_lit_integer_α
.Lx214_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n90_lit_integer_α:
                        mov              qword ptr [rsp + 1712], 6
                        mov              rax, qword ptr [rip + .Lx215_0]
                        mov              qword ptr [rsp + 1720], rax
                                                                                        jmp   n98_subscript_α
.Lx215_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n91_lit_integer_α:
                        mov              qword ptr [rsp + 1904], 6
                        mov              rax, qword ptr [rip + .Lx216_0]
                        mov              qword ptr [rsp + 1912], rax
                                                                                        jmp   n99_subscript_α
.Lx216_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n92_lit_integer_α:
                        mov              qword ptr [rsp + 2224], 6
                        mov              rax, qword ptr [rip + .Lx217_0]
                        mov              qword ptr [rsp + 2232], rax
                                                                                        jmp   n100_call_α
.Lx217_0:
                        .quad            2121
#-----------------------------------------------------------------------------------------------------------------------
n93_assign_var_α:
                        mov              rdi, qword ptr [rsp + 2352]
                        mov              rsi, qword ptr [rsp + 2360]
                        mov              rdx, qword ptr [rsp + 2448]
                        mov              rcx, qword ptr [rsp + 2456]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n49_var_α
                        mov              qword ptr [rsp + 2464], rax
                        mov              qword ptr [rsp + 2472], rdx
                                                                                        jmp   n49_var_α
#-----------------------------------------------------------------------------------------------------------------------
n94_call_α:
                        mov              rax, qword ptr [rsp + 2544]
                        mov              qword ptr [rsp + 2496], rax
                        mov              rax, qword ptr [rsp + 2552]
                        mov              qword ptr [rsp + 2504], rax
                        mov              rax, qword ptr [rsp + 2640]
                        mov              qword ptr [rsp + 2512], rax
                        mov              rax, qword ptr [rsp + 2648]
                        mov              qword ptr [rsp + 2520], rax
                        .section         .rodata
.Lbynamefn95:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn95]
                        lea              rsi, [rsp + 2496]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 2480], rax
                        mov              qword ptr [rsp + 2488], rdx
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
                        mov              dword ptr [rsp + 4], 34
                        mov              rax, qword ptr [rip + .Lx220_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n102_assign_α
.Lx220_0:
                        .quad            .Lx220_0_s
.Lx220_0_s:
                        .string          "FAIL 1114/001: item 1D assign/read"
#-----------------------------------------------------------------------------------------------------------------------
n96_lit_integer_α:
                        mov              qword ptr [rsp + 1200], 6
                        mov              rax, qword ptr [rip + .Lx221_0]
                        mov              qword ptr [rsp + 1208], rax
                                                                                        jmp   n103_call_α
.Lx221_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n97_lit_integer_α:
                        mov              qword ptr [rsp + 1488], 6
                        mov              rax, qword ptr [rip + .Lx222_0]
                        mov              qword ptr [rsp + 1496], rax
                                                                                        jmp   n104_call_α
.Lx222_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n98_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1712]
                        mov              rsi, qword ptr [rsp + 1720]
                        mov              rdx, qword ptr [rsp + 1728]
                        mov              rcx, qword ptr [rsp + 1736]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx223_240
                        add              rsp, 16
                                                                                        jmp   n14_var_α
.Lx223_240:
                        mov              qword ptr [rsp + 1744], rax
                        mov              qword ptr [rsp + 1752], rdx
                        add              rsp, 16
                                                                                        jmp   n105_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n99_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1904]
                        mov              rsi, qword ptr [rsp + 1912]
                        mov              rdx, qword ptr [rsp + 1920]
                        mov              rcx, qword ptr [rsp + 1928]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx224_240
                        add              rsp, 16
                                                                                        jmp   n25_var_α
.Lx224_240:
                        mov              qword ptr [rsp + 1936], rax
                        mov              qword ptr [rsp + 1944], rdx
                        add              rsp, 16
                                                                                        jmp   n106_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n100_call_α:
                        mov              rax, qword ptr [rsp + 2032]
                        mov              qword ptr [rsp + 1984], rax
                        mov              rax, qword ptr [rsp + 2040]
                        mov              qword ptr [rsp + 1992], rax
                        mov              rax, qword ptr [rsp + 2224]
                        mov              qword ptr [rsp + 2000], rax
                        mov              rax, qword ptr [rsp + 2232]
                        mov              qword ptr [rsp + 2008], rax
                        .section         .rodata
.Lbynamefn101:          .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn101]
                        lea              rsi, [rsp + 1984]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1968], rax
                        mov              qword ptr [rsp + 1976], rdx
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
                        mov              dword ptr [rsp + 4], 28
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
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n103_call_α:
                        mov              rax, qword ptr [rsp + 1136]
                        mov              qword ptr [rsp + 1040], rax
                        mov              rax, qword ptr [rsp + 1144]
                        mov              qword ptr [rsp + 1048], rax
                        mov              rax, qword ptr [rsp + 1152]
                        mov              qword ptr [rsp + 1056], rax
                        mov              rax, qword ptr [rsp + 1160]
                        mov              qword ptr [rsp + 1064], rax
                        mov              rax, qword ptr [rsp + 1168]
                        mov              qword ptr [rsp + 1072], rax
                        mov              rax, qword ptr [rsp + 1176]
                        mov              qword ptr [rsp + 1080], rax
                        mov              rax, qword ptr [rsp + 1184]
                        mov              qword ptr [rsp + 1088], rax
                        mov              rax, qword ptr [rsp + 1192]
                        mov              qword ptr [rsp + 1096], rax
                        mov              rax, qword ptr [rsp + 1200]
                        mov              qword ptr [rsp + 1104], rax
                        mov              rax, qword ptr [rsp + 1208]
                        mov              qword ptr [rsp + 1112], rax
                        .section         .rodata
.Lbynamefn104:          .string          "item"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn104]
                        lea              rsi, [rsp + 1040]
                        mov              edx, 5
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1024], rax
                        mov              qword ptr [rsp + 1032], rdx
                        cmp              eax, 99
                                                                                        je    n56_var_α
                                                                                        jmp   n109_lit_integer_α
n103_call_β:
                                                                                        jmp   n56_var_α
#-----------------------------------------------------------------------------------------------------------------------
n104_call_α:
                        mov              rax, qword ptr [rsp + 1424]
                        mov              qword ptr [rsp + 1328], rax
                        mov              rax, qword ptr [rsp + 1432]
                        mov              qword ptr [rsp + 1336], rax
                        mov              rax, qword ptr [rsp + 1440]
                        mov              qword ptr [rsp + 1344], rax
                        mov              rax, qword ptr [rsp + 1448]
                        mov              qword ptr [rsp + 1352], rax
                        mov              rax, qword ptr [rsp + 1456]
                        mov              qword ptr [rsp + 1360], rax
                        mov              rax, qword ptr [rsp + 1464]
                        mov              qword ptr [rsp + 1368], rax
                        mov              rax, qword ptr [rsp + 1472]
                        mov              qword ptr [rsp + 1376], rax
                        mov              rax, qword ptr [rsp + 1480]
                        mov              qword ptr [rsp + 1384], rax
                        mov              rax, qword ptr [rsp + 1488]
                        mov              qword ptr [rsp + 1392], rax
                        mov              rax, qword ptr [rsp + 1496]
                        mov              qword ptr [rsp + 1400], rax
                        .section         .rodata
.Lbynamefn105:          .string          "item"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn105]
                        lea              rsi, [rsp + 1328]
                        mov              edx, 5
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1312], rax
                        mov              qword ptr [rsp + 1320], rdx
                        cmp              eax, 99
                                                                                        je    n13_var_α
                                                                                        jmp   n110_lit_integer_α
n104_call_β:
                                                                                        jmp   n13_var_α
#-----------------------------------------------------------------------------------------------------------------------
n105_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1744]
                        mov              rsi, qword ptr [rsp + 1752]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx230_240
                        add              rsp, 16
                                                                                        jmp   n14_var_α
.Lx230_240:
                        mov              qword ptr [rsp + 1760], rax
                        mov              qword ptr [rsp + 1768], rdx
                        add              rsp, 16
                                                                                        jmp   n111_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n106_lit_integer_α:
                        mov              qword ptr [rsp + 1936], 6
                        mov              rax, qword ptr [rip + .Lx231_0]
                        mov              qword ptr [rsp + 1944], rax
                                                                                        jmp   n112_assign_var_α
.Lx231_0:
                        .quad            2121
#=======================================================================================================================
#         output = 'FAIL 1114/006: bracket 4D assign, item read' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n107_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 43
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
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n109_lit_integer_α:
                        mov              qword ptr [rsp + 1216], 6
                        mov              rax, qword ptr [rip + .Lx234_0]
                        mov              qword ptr [rsp + 1224], rax
                                                                                        jmp   n114_assign_var_α
.Lx234_0:
                        .quad            1212
#-----------------------------------------------------------------------------------------------------------------------
n110_lit_integer_α:
                        mov              qword ptr [rsp + 1504], 6
                        mov              rax, qword ptr [rip + .Lx235_0]
                        mov              qword ptr [rsp + 1512], rax
                                                                                        jmp   n115_call_α
.Lx235_0:
                        .quad            1212
#-----------------------------------------------------------------------------------------------------------------------
n111_lit_integer_α:
                        mov              qword ptr [rsp + 1760], 6
                        mov              rax, qword ptr [rip + .Lx236_0]
                        mov              qword ptr [rsp + 1768], rax
                                                                                        jmp   n116_call_α
.Lx236_0:
                        .quad            1212
#-----------------------------------------------------------------------------------------------------------------------
n112_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1920]
                        mov              rsi, qword ptr [rsp + 1928]
                        mov              rdx, qword ptr [rsp + 1936]
                        mov              rcx, qword ptr [rsp + 1944]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n25_var_α
                        mov              qword ptr [rsp + 1952], rax
                        mov              qword ptr [rsp + 1960], rdx
                                                                                        jmp   n25_var_α
#-----------------------------------------------------------------------------------------------------------------------
n113_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n114_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1024]
                        mov              rsi, qword ptr [rsp + 1032]
                        mov              rdx, qword ptr [rsp + 1216]
                        mov              rcx, qword ptr [rsp + 1224]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n56_var_α
                        mov              qword ptr [rsp + 1232], rax
                        mov              qword ptr [rsp + 1240], rdx
                                                                                        jmp   n56_var_α
#-----------------------------------------------------------------------------------------------------------------------
n115_call_α:
                        mov              rax, qword ptr [rsp + 1312]
                        mov              qword ptr [rsp + 1264], rax
                        mov              rax, qword ptr [rsp + 1320]
                        mov              qword ptr [rsp + 1272], rax
                        mov              rax, qword ptr [rsp + 1504]
                        mov              qword ptr [rsp + 1280], rax
                        mov              rax, qword ptr [rsp + 1512]
                        mov              qword ptr [rsp + 1288], rax
                        .section         .rodata
.Lbynamefn116:          .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn116]
                        lea              rsi, [rsp + 1264]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1248], rax
                        mov              qword ptr [rsp + 1256], rdx
                        cmp              eax, 99
                                                                                        je    n13_var_α
                                                                                        jmp   n117_lit_string_α
n115_call_β:
                                                                                        jmp   n13_var_α
#-----------------------------------------------------------------------------------------------------------------------
n116_call_α:
                        mov              rax, qword ptr [rsp + 1744]
                        mov              qword ptr [rsp + 1552], rax
                        mov              rax, qword ptr [rsp + 1752]
                        mov              qword ptr [rsp + 1560], rax
                        mov              rax, qword ptr [rsp + 1760]
                        mov              qword ptr [rsp + 1568], rax
                        mov              rax, qword ptr [rsp + 1768]
                        mov              qword ptr [rsp + 1576], rax
                        .section         .rodata
.Lbynamefn117:          .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn117]
                        lea              rsi, [rsp + 1552]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1536], rax
                        mov              qword ptr [rsp + 1544], rdx
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
                        mov              dword ptr [rsp + 4], 34
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
                        mov              dword ptr [rsp + 4], 33
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
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n120_assign_α:
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
                        add              rsp, 2696
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              eax, 99
                        xor              edx, edx
                        add              rsp, 2696
                        ret
                        .section         .note.GNU-stack,"",@progbits

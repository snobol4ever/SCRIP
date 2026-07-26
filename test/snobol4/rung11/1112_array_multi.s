                        .intel_syntax    noprefix
                        .text
                        .section         .rodata
.Lgvan0:                .string          "c"
.Lgvan1:                .string          "output"
.Lgvan2:                .string          "d"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .quad            .Lgvan1
                        .quad            .Lgvan2
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        mov              edi, 3
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 3
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
                        sub              rsp, 1320
                        mov              rdi, rsp
                        mov              ecx, 1320
                        xor              eax, eax
                        rep stosb
                        mov              [rsp + 1312], rbp
                        mov              rbp, rsp
main_α_body:
#=======================================================================================================================
#         c = array('2,2')
#-----------------------------------------------------------------------------------------------------------------------
n0_lit_string_α:
                        mov              qword ptr [rbp + 64], 1
                        mov              rax, qword ptr [rip + .Lx73_0]
                        mov              qword ptr [rbp + 72], rax
                                                                                        jmp   n7_call_α
.Lx73_0:
                        .quad            .Lx73_0_s
.Lx73_0_s:
                        .string          "2,2"
#-----------------------------------------------------------------------------------------------------------------------
n1_goto_α:
                                                                                        jmp   n8_var_α
n1_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n2_goto_α:
                                                                                        jmp   n9_lit_string_α
n2_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n3_goto_α:
                                                                                        jmp   n10_var_α
n3_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n4_goto_α:
                                                                                        jmp   n11_var_α
n4_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n5_goto_α:
                                                                                        jmp   n12_lit_string_α
n5_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n6_goto_α:
                                                                                        jmp   main_γ
n6_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n7_call_α:
                        mov              rax, qword ptr [rbp + 64]
                        mov              qword ptr [rbp + 32], rax
                        mov              rax, qword ptr [rbp + 72]
                        mov              qword ptr [rbp + 40], rax
                        .section         .rodata
.Lbynamefn8:            .string          "array"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn8]
                        lea              rsi, [rbp + 32]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 16], rax
                        mov              qword ptr [rbp + 24], rdx
                        cmp              eax, 99
                                                                                        je    n14_var_α
                                                                                        jmp   n13_assign_α
n7_call_β:
                                                                                        jmp   n14_var_α
#=======================================================================================================================
#         differ(prototype(c), '2,2')                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n8_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rbp + 512], rax
                        mov              qword ptr [rbp + 520], rdx
                                                                                        jmp   n15_call_α
#=======================================================================================================================
#         d = array('-1:1,2')
#-----------------------------------------------------------------------------------------------------------------------
n9_lit_string_α:
                        mov              qword ptr [rbp + 640], 1
                        mov              rax, qword ptr [rip + .Lx82_0]
                        mov              qword ptr [rbp + 648], rax
                                                                                        jmp   n16_call_α
.Lx82_0:
                        .quad            .Lx82_0_s
.Lx82_0_s:
                        .string          "-1:1,2"
#=======================================================================================================================
#         d<-2,1>                    :f(e004)
#-----------------------------------------------------------------------------------------------------------------------
n10_var_α:
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rbp + 1008], rax
                        mov              qword ptr [rbp + 1016], rdx
                                                                                        jmp   n17_lit_integer_α
#=======================================================================================================================
#         d<2,1>                     :f(e005)
#-----------------------------------------------------------------------------------------------------------------------
n11_var_α:
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rbp + 1152], rax
                        mov              qword ptr [rbp + 1160], rdx
                                                                                        jmp   n18_lit_integer_α
#=======================================================================================================================
#         output = 'PASS 1112_array_multi (5/5)'
#-----------------------------------------------------------------------------------------------------------------------
n12_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              rax, qword ptr [rip + .Lx85_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n19_assign_α
.Lx85_0:
                        .quad            .Lx85_0_s
.Lx85_0_s:
                        .string          "PASS 1112_array_multi (5/5)"
#-----------------------------------------------------------------------------------------------------------------------
n13_assign_α:
                        mov              rax, qword ptr [rbp + 16]
                        mov              rdx, qword ptr [rbp + 24]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        mov              qword ptr [rbp + 0], rax
                        mov              qword ptr [rbp + 8], rdx
                                                                                        jmp   n14_var_α
#=======================================================================================================================
#         c<1,2> = '*'
#-----------------------------------------------------------------------------------------------------------------------
n14_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rbp + 80], rax
                        mov              qword ptr [rbp + 88], rdx
                                                                                        jmp   n20_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n15_call_α:
                        mov              rax, qword ptr [rbp + 512]
                        mov              qword ptr [rbp + 480], rax
                        mov              rax, qword ptr [rbp + 520]
                        mov              qword ptr [rbp + 488], rax
                        .section         .rodata
.Lbynamefn16:           .string          "prototype"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn16]
                        lea              rsi, [rbp + 480]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 464], rax
                        mov              qword ptr [rbp + 472], rdx
                        cmp              eax, 99
                                                                                        je    n9_lit_string_α
                                                                                        jmp   n22_lit_string_α
n15_call_β:
                                                                                        jmp   n9_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n16_call_α:
                        mov              rax, qword ptr [rbp + 640]
                        mov              qword ptr [rbp + 608], rax
                        mov              rax, qword ptr [rbp + 648]
                        mov              qword ptr [rbp + 616], rax
                        .section         .rodata
.Lbynamefn17:           .string          "array"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn17]
                        lea              rsi, [rbp + 608]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 592], rax
                        mov              qword ptr [rbp + 600], rdx
                        cmp              eax, 99
                                                                                        je    n24_var_α
                                                                                        jmp   n23_assign_α
n16_call_β:
                                                                                        jmp   n24_var_α
#-----------------------------------------------------------------------------------------------------------------------
n17_lit_integer_α:
                        mov              qword ptr [rbp + 1040], 6
                        mov              rax, qword ptr [rip + .Lx90_0]
                        mov              qword ptr [rbp + 1048], rax
                                                                                        jmp   n25_unop_α
.Lx90_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n18_lit_integer_α:
                        mov              qword ptr [rbp + 1168], 6
                        mov              rax, qword ptr [rip + .Lx91_0]
                        mov              qword ptr [rbp + 1176], rax
                                                                                        jmp   n26_subscript_α
.Lx91_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n19_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        mov              qword ptr [rbp + 1280], rax
                        mov              qword ptr [rbp + 1288], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n20_lit_integer_α:
                        mov              qword ptr [rbp + 96], 6
                        mov              rax, qword ptr [rip + .Lx93_0]
                        mov              qword ptr [rbp + 104], rax
                                                                                        jmp   n27_subscript_α
.Lx93_0:
                        .quad            1
#=======================================================================================================================
#         differ(c<1,2>, '*')                   :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n21_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rbp + 256], rax
                        mov              qword ptr [rbp + 264], rdx
                                                                                        jmp   n28_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n22_lit_string_α:
                        mov              qword ptr [rbp + 528], 1
                        mov              rax, qword ptr [rip + .Lx95_0]
                        mov              qword ptr [rbp + 536], rax
                                                                                        jmp   n29_call_α
.Lx95_0:
                        .quad            .Lx95_0_s
.Lx95_0_s:
                        .string          "2,2"
#-----------------------------------------------------------------------------------------------------------------------
n23_assign_α:
                        mov              rax, qword ptr [rbp + 592]
                        mov              rdx, qword ptr [rbp + 600]
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rdx
                        mov              qword ptr [rbp + 576], rax
                        mov              qword ptr [rbp + 584], rdx
                                                                                        jmp   n24_var_α
#=======================================================================================================================
#         d<-1,1> = 0
#-----------------------------------------------------------------------------------------------------------------------
n24_var_α:
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rbp + 656], rax
                        mov              qword ptr [rbp + 664], rdx
                                                                                        jmp   n30_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n25_unop_α:
                        mov              rdi, qword ptr [rbp + 1040]
                        mov              rsi, qword ptr [rbp + 1048]
                        call             rt_num_neg@PLT
                        mov              qword ptr [rbp + 1024], rax
                        mov              qword ptr [rbp + 1032], rdx
                                                                                        jmp   n32_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n26_subscript_α:
                        mov              rdi, qword ptr [rbp + 1152]
                        mov              rsi, qword ptr [rbp + 1160]
                        mov              rdx, qword ptr [rbp + 1168]
                        mov              rcx, qword ptr [rbp + 1176]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n12_lit_string_α
                        mov              qword ptr [rbp + 1184], rax
                        mov              qword ptr [rbp + 1192], rdx
                                                                                        jmp   n33_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n27_subscript_α:
                        mov              rdi, qword ptr [rbp + 80]
                        mov              rsi, qword ptr [rbp + 88]
                        mov              rdx, qword ptr [rbp + 96]
                        mov              rcx, qword ptr [rbp + 104]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n21_var_α
                        mov              qword ptr [rbp + 112], rax
                        mov              qword ptr [rbp + 120], rdx
                                                                                        jmp   n34_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n28_lit_integer_α:
                        mov              qword ptr [rbp + 272], 6
                        mov              rax, qword ptr [rip + .Lx101_0]
                        mov              qword ptr [rbp + 280], rax
                                                                                        jmp   n35_subscript_α
.Lx101_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n29_call_α:
                        mov              rax, qword ptr [rbp + 464]
                        mov              qword ptr [rbp + 416], rax
                        mov              rax, qword ptr [rbp + 472]
                        mov              qword ptr [rbp + 424], rax
                        mov              rax, qword ptr [rbp + 528]
                        mov              qword ptr [rbp + 432], rax
                        mov              rax, qword ptr [rbp + 536]
                        mov              qword ptr [rbp + 440], rax
                        .section         .rodata
.Lbynamefn30:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn30]
                        lea              rsi, [rbp + 416]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 400], rax
                        mov              qword ptr [rbp + 408], rdx
                        cmp              eax, 99
                                                                                        je    n9_lit_string_α
                                                                                        jmp   n36_lit_string_α
n29_call_β:
                                                                                        jmp   n9_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n30_lit_integer_α:
                        mov              qword ptr [rbp + 688], 6
                        mov              rax, qword ptr [rip + .Lx103_0]
                        mov              qword ptr [rbp + 696], rax
                                                                                        jmp   n37_unop_α
.Lx103_0:
                        .quad            1
#=======================================================================================================================
#         differ(d<-1,1>, 0)                   :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n31_var_α:
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rbp + 848], rax
                        mov              qword ptr [rbp + 856], rdx
                                                                                        jmp   n38_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n32_subscript_α:
                        mov              rdi, qword ptr [rbp + 1008]
                        mov              rsi, qword ptr [rbp + 1016]
                        mov              rdx, qword ptr [rbp + 1024]
                        mov              rcx, qword ptr [rbp + 1032]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n11_var_α
                        mov              qword ptr [rbp + 1056], rax
                        mov              qword ptr [rbp + 1064], rdx
                                                                                        jmp   n39_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n33_lit_integer_α:
                        mov              qword ptr [rbp + 1200], 6
                        mov              rax, qword ptr [rip + .Lx106_0]
                        mov              qword ptr [rbp + 1208], rax
                                                                                        jmp   n40_subscript_α
.Lx106_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n34_lit_integer_α:
                        mov              qword ptr [rbp + 128], 6
                        mov              rax, qword ptr [rip + .Lx107_0]
                        mov              qword ptr [rbp + 136], rax
                                                                                        jmp   n41_subscript_α
.Lx107_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n35_subscript_α:
                        mov              rdi, qword ptr [rbp + 256]
                        mov              rsi, qword ptr [rbp + 264]
                        mov              rdx, qword ptr [rbp + 272]
                        mov              rcx, qword ptr [rbp + 280]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n8_var_α
                        mov              qword ptr [rbp + 288], rax
                        mov              qword ptr [rbp + 296], rdx
                                                                                        jmp   n42_lit_integer_α
#=======================================================================================================================
#         output = 'FAIL 1112/002: prototype of 2D array' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n36_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              rax, qword ptr [rip + .Lx109_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n43_assign_α
.Lx109_0:
                        .quad            .Lx109_0_s
.Lx109_0_s:
                        .string          "FAIL 1112/002: prototype of 2D array"
#-----------------------------------------------------------------------------------------------------------------------
n37_unop_α:
                        mov              rdi, qword ptr [rbp + 688]
                        mov              rsi, qword ptr [rbp + 696]
                        call             rt_num_neg@PLT
                        mov              qword ptr [rbp + 672], rax
                        mov              qword ptr [rbp + 680], rdx
                                                                                        jmp   n44_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n38_lit_integer_α:
                        mov              qword ptr [rbp + 880], 6
                        mov              rax, qword ptr [rip + .Lx111_0]
                        mov              qword ptr [rbp + 888], rax
                                                                                        jmp   n45_unop_α
.Lx111_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n39_lit_integer_α:
                        mov              qword ptr [rbp + 1072], 6
                        mov              rax, qword ptr [rip + .Lx112_0]
                        mov              qword ptr [rbp + 1080], rax
                                                                                        jmp   n46_subscript_α
.Lx112_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n40_subscript_α:
                        mov              rdi, qword ptr [rbp + 1184]
                        mov              rsi, qword ptr [rbp + 1192]
                        mov              rdx, qword ptr [rbp + 1200]
                        mov              rcx, qword ptr [rbp + 1208]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n12_lit_string_α
                        mov              qword ptr [rbp + 1216], rax
                        mov              qword ptr [rbp + 1224], rdx
                                                                                        jmp   n47_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n41_subscript_α:
                        mov              rdi, qword ptr [rbp + 112]
                        mov              rsi, qword ptr [rbp + 120]
                        mov              rdx, qword ptr [rbp + 128]
                        mov              rcx, qword ptr [rbp + 136]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n21_var_α
                        mov              qword ptr [rbp + 144], rax
                        mov              qword ptr [rbp + 152], rdx
                                                                                        jmp   n48_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n42_lit_integer_α:
                        mov              qword ptr [rbp + 304], 6
                        mov              rax, qword ptr [rip + .Lx115_0]
                        mov              qword ptr [rbp + 312], rax
                                                                                        jmp   n49_subscript_α
.Lx115_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n43_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        mov              qword ptr [rbp + 544], rax
                        mov              qword ptr [rbp + 552], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n44_subscript_α:
                        mov              rdi, qword ptr [rbp + 656]
                        mov              rsi, qword ptr [rbp + 664]
                        mov              rdx, qword ptr [rbp + 672]
                        mov              rcx, qword ptr [rbp + 680]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n31_var_α
                        mov              qword ptr [rbp + 704], rax
                        mov              qword ptr [rbp + 712], rdx
                                                                                        jmp   n50_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n45_unop_α:
                        mov              rdi, qword ptr [rbp + 880]
                        mov              rsi, qword ptr [rbp + 888]
                        call             rt_num_neg@PLT
                        mov              qword ptr [rbp + 864], rax
                        mov              qword ptr [rbp + 872], rdx
                                                                                        jmp   n51_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n46_subscript_α:
                        mov              rdi, qword ptr [rbp + 1056]
                        mov              rsi, qword ptr [rbp + 1064]
                        mov              rdx, qword ptr [rbp + 1072]
                        mov              rcx, qword ptr [rbp + 1080]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n11_var_α
                        mov              qword ptr [rbp + 1088], rax
                        mov              qword ptr [rbp + 1096], rdx
                                                                                        jmp   n52_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n47_deref_α:
                        mov              rdi, qword ptr [rbp + 1216]
                        mov              rsi, qword ptr [rbp + 1224]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n12_lit_string_α
                        mov              qword ptr [rbp + 1232], rax
                        mov              qword ptr [rbp + 1240], rdx
                                                                                        jmp   n53_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n48_lit_string_α:
                        mov              qword ptr [rbp + 160], 1
                        mov              rax, qword ptr [rip + .Lx121_0]
                        mov              qword ptr [rbp + 168], rax
                                                                                        jmp   n54_assign_var_α
.Lx121_0:
                        .quad            .Lx121_0_s
.Lx121_0_s:
                        .string          "*"
#-----------------------------------------------------------------------------------------------------------------------
n49_subscript_α:
                        mov              rdi, qword ptr [rbp + 288]
                        mov              rsi, qword ptr [rbp + 296]
                        mov              rdx, qword ptr [rbp + 304]
                        mov              rcx, qword ptr [rbp + 312]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n8_var_α
                        mov              qword ptr [rbp + 320], rax
                        mov              qword ptr [rbp + 328], rdx
                                                                                        jmp   n55_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n50_lit_integer_α:
                        mov              qword ptr [rbp + 720], 6
                        mov              rax, qword ptr [rip + .Lx123_0]
                        mov              qword ptr [rbp + 728], rax
                                                                                        jmp   n56_subscript_α
.Lx123_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n51_subscript_α:
                        mov              rdi, qword ptr [rbp + 848]
                        mov              rsi, qword ptr [rbp + 856]
                        mov              rdx, qword ptr [rbp + 864]
                        mov              rcx, qword ptr [rbp + 872]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n10_var_α
                        mov              qword ptr [rbp + 896], rax
                        mov              qword ptr [rbp + 904], rdx
                                                                                        jmp   n57_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n52_deref_α:
                        mov              rdi, qword ptr [rbp + 1088]
                        mov              rsi, qword ptr [rbp + 1096]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n11_var_α
                        mov              qword ptr [rbp + 1104], rax
                        mov              qword ptr [rbp + 1112], rdx
                                                                                        jmp   n58_lit_string_α
#=======================================================================================================================
#         output = 'FAIL 1112/005: above upper bound fails' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n53_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              rax, qword ptr [rip + .Lx126_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n59_assign_α
.Lx126_0:
                        .quad            .Lx126_0_s
.Lx126_0_s:
                        .string          "FAIL 1112/005: above upper bound fails"
#-----------------------------------------------------------------------------------------------------------------------
n54_assign_var_α:
                        mov              rdi, qword ptr [rbp + 144]
                        mov              rsi, qword ptr [rbp + 152]
                        mov              rdx, qword ptr [rbp + 160]
                        mov              rcx, qword ptr [rbp + 168]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n21_var_α
                        mov              qword ptr [rbp + 176], rax
                        mov              qword ptr [rbp + 184], rdx
                                                                                        jmp   n21_var_α
#-----------------------------------------------------------------------------------------------------------------------
n55_deref_α:
                        mov              rdi, qword ptr [rbp + 320]
                        mov              rsi, qword ptr [rbp + 328]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n8_var_α
                        mov              qword ptr [rbp + 336], rax
                        mov              qword ptr [rbp + 344], rdx
                                                                                        jmp   n60_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n56_subscript_α:
                        mov              rdi, qword ptr [rbp + 704]
                        mov              rsi, qword ptr [rbp + 712]
                        mov              rdx, qword ptr [rbp + 720]
                        mov              rcx, qword ptr [rbp + 728]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n31_var_α
                        mov              qword ptr [rbp + 736], rax
                        mov              qword ptr [rbp + 744], rdx
                                                                                        jmp   n61_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n57_lit_integer_α:
                        mov              qword ptr [rbp + 912], 6
                        mov              rax, qword ptr [rip + .Lx130_0]
                        mov              qword ptr [rbp + 920], rax
                                                                                        jmp   n62_subscript_α
.Lx130_0:
                        .quad            1
#=======================================================================================================================
#         output = 'FAIL 1112/004: below lower bound fails' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n58_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              rax, qword ptr [rip + .Lx131_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n63_assign_α
.Lx131_0:
                        .quad            .Lx131_0_s
.Lx131_0_s:
                        .string          "FAIL 1112/004: below lower bound fails"
#-----------------------------------------------------------------------------------------------------------------------
n59_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        mov              qword ptr [rbp + 1248], rax
                        mov              qword ptr [rbp + 1256], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n60_lit_string_α:
                        mov              qword ptr [rbp + 352], 1
                        mov              rax, qword ptr [rip + .Lx133_0]
                        mov              qword ptr [rbp + 360], rax
                                                                                        jmp   n64_call_α
.Lx133_0:
                        .quad            .Lx133_0_s
.Lx133_0_s:
                        .string          "*"
#-----------------------------------------------------------------------------------------------------------------------
n61_lit_integer_α:
                        mov              qword ptr [rbp + 752], 6
                        mov              rax, qword ptr [rip + .Lx134_0]
                        mov              qword ptr [rbp + 760], rax
                                                                                        jmp   n65_assign_var_α
.Lx134_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n62_subscript_α:
                        mov              rdi, qword ptr [rbp + 896]
                        mov              rsi, qword ptr [rbp + 904]
                        mov              rdx, qword ptr [rbp + 912]
                        mov              rcx, qword ptr [rbp + 920]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n10_var_α
                        mov              qword ptr [rbp + 928], rax
                        mov              qword ptr [rbp + 936], rdx
                                                                                        jmp   n66_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n63_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        mov              qword ptr [rbp + 1120], rax
                        mov              qword ptr [rbp + 1128], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n64_call_α:
                        mov              rax, qword ptr [rbp + 336]
                        mov              qword ptr [rbp + 208], rax
                        mov              rax, qword ptr [rbp + 344]
                        mov              qword ptr [rbp + 216], rax
                        mov              rax, qword ptr [rbp + 352]
                        mov              qword ptr [rbp + 224], rax
                        mov              rax, qword ptr [rbp + 360]
                        mov              qword ptr [rbp + 232], rax
                        .section         .rodata
.Lbynamefn65:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn65]
                        lea              rsi, [rbp + 208]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 192], rax
                        mov              qword ptr [rbp + 200], rdx
                        cmp              eax, 99
                                                                                        je    n8_var_α
                                                                                        jmp   n67_lit_string_α
n64_call_β:
                                                                                        jmp   n8_var_α
#-----------------------------------------------------------------------------------------------------------------------
n65_assign_var_α:
                        mov              rdi, qword ptr [rbp + 736]
                        mov              rsi, qword ptr [rbp + 744]
                        mov              rdx, qword ptr [rbp + 752]
                        mov              rcx, qword ptr [rbp + 760]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n31_var_α
                        mov              qword ptr [rbp + 768], rax
                        mov              qword ptr [rbp + 776], rdx
                                                                                        jmp   n31_var_α
#-----------------------------------------------------------------------------------------------------------------------
n66_deref_α:
                        mov              rdi, qword ptr [rbp + 928]
                        mov              rsi, qword ptr [rbp + 936]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n10_var_α
                        mov              qword ptr [rbp + 944], rax
                        mov              qword ptr [rbp + 952], rdx
                                                                                        jmp   n68_lit_integer_α
#=======================================================================================================================
#         output = 'FAIL 1112/001: 2D array assign/read' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n67_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              rax, qword ptr [rip + .Lx140_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n69_assign_α
.Lx140_0:
                        .quad            .Lx140_0_s
.Lx140_0_s:
                        .string          "FAIL 1112/001: 2D array assign/read"
#-----------------------------------------------------------------------------------------------------------------------
n68_lit_integer_α:
                        mov              qword ptr [rbp + 960], 6
                        mov              rax, qword ptr [rip + .Lx141_0]
                        mov              qword ptr [rbp + 968], rax
                                                                                        jmp   n70_call_α
.Lx141_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n69_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        mov              qword ptr [rbp + 368], rax
                        mov              qword ptr [rbp + 376], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n70_call_α:
                        mov              rax, qword ptr [rbp + 944]
                        mov              qword ptr [rbp + 800], rax
                        mov              rax, qword ptr [rbp + 952]
                        mov              qword ptr [rbp + 808], rax
                        mov              rax, qword ptr [rbp + 960]
                        mov              qword ptr [rbp + 816], rax
                        mov              rax, qword ptr [rbp + 968]
                        mov              qword ptr [rbp + 824], rax
                        .section         .rodata
.Lbynamefn71:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn71]
                        lea              rsi, [rbp + 800]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 784], rax
                        mov              qword ptr [rbp + 792], rdx
                        cmp              eax, 99
                                                                                        je    n10_var_α
                                                                                        jmp   n71_lit_string_α
n70_call_β:
                                                                                        jmp   n10_var_α
#=======================================================================================================================
#         output = 'FAIL 1112/003: custom lower bound assign/read' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n71_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              rax, qword ptr [rip + .Lx144_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n72_assign_α
.Lx144_0:
                        .quad            .Lx144_0_s
.Lx144_0_s:
                        .string          "FAIL 1112/003: custom lower bound assign/read"
#-----------------------------------------------------------------------------------------------------------------------
n72_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        mov              qword ptr [rbp + 976], rax
                        mov              qword ptr [rbp + 984], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        mov              rsp, rbp
                        mov              rbp, [rsp + 1312]
                        add              rsp, 1320
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              rsp, rbp
                        mov              eax, 99
                        xor              edx, edx
                        mov              rbp, [rsp + 1312]
                        add              rsp, 1320
                        ret
                        .section         .note.GNU-stack,"",@progbits

                        .intel_syntax    noprefix
                        .text
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
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
                        sub              rsp, 1368
                        mov              rdi, rsp
                        mov              ecx, 1368
                        xor              eax, eax
                        rep stosb
                        mov              [rsp + 1360], rbp
                        mov              rbp, rsp
main_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n0_lit_integer_α:
                        mov              qword ptr [rbp + 1264], 6
                        mov              rax, qword ptr [rip + .Lx71_0]
                        mov              qword ptr [rbp + 1272], rax
                                                                                        jmp   n1_assign_α
.Lx71_0:
                        .quad            50
#-----------------------------------------------------------------------------------------------------------------------
n1_assign_α:
                        mov              rax, qword ptr [rbp + 1264]
                        mov              rdx, qword ptr [rbp + 1272]
                        mov              qword ptr [rbp + 1312], rax
                        mov              qword ptr [rbp + 1320], rdx
                                                                                        jmp   n2_var_α
#-----------------------------------------------------------------------------------------------------------------------
n2_var_α:
                        mov              rax, qword ptr [rbp + 1312]
                        mov              qword ptr [rbp + 1232], rax
                        mov              rax, qword ptr [rbp + 1320]
                        mov              qword ptr [rbp + 1240], rax
                                                                                        jmp   n3_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_integer_α:
                        mov              qword ptr [rbp + 1248], 6
                        mov              rax, qword ptr [rip + .Lx75_0]
                        mov              qword ptr [rbp + 1256], rax
                                                                                        jmp   n5_call_builtin_icon_α
.Lx75_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n4_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 1328]
                        mov              qword ptr [rbp + 1088], rax
                        mov              qword ptr [rbp + 1096], rdx
                                                                                        jmp   n6_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n5_call_builtin_icon_α:
                        mov              rax, qword ptr [rbp + 1232]
                        mov              qword ptr [rbp + 1184], rax
                        mov              rax, qword ptr [rbp + 1240]
                        mov              qword ptr [rbp + 1192], rax
                        mov              rax, qword ptr [rbp + 1248]
                        mov              qword ptr [rbp + 1200], rax
                        mov              rax, qword ptr [rbp + 1256]
                        mov              qword ptr [rbp + 1208], rax
                        .section         .rodata
.Lrkfn79:               .string          "list"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn79]
                        lea              rsi, [rbp + 1184]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1168], rax
                        mov              qword ptr [rbp + 1176], rdx
                        cmp              eax, 99
                                                                                        je    n4_var_ref_α
                                                                                        jmp   n7_assign_α
n5_call_builtin_icon_β:
                                                                                        jmp   n4_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n6_lit_integer_α:
                        mov              qword ptr [rbp + 1104], 6
                        mov              rax, qword ptr [rip + .Lx80_0]
                        mov              qword ptr [rbp + 1112], rax
                                                                                        jmp   n8_subscript_α
.Lx80_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n7_assign_α:
                        mov              rax, qword ptr [rbp + 1168]
                        mov              rdx, qword ptr [rbp + 1176]
                        mov              qword ptr [rbp + 1328], rax
                        mov              qword ptr [rbp + 1336], rdx
                                                                                        jmp   n4_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n8_subscript_α:
                        mov              rdi, qword ptr [rbp + 1088]
                        mov              rsi, qword ptr [rbp + 1096]
                        mov              rdx, qword ptr [rbp + 1104]
                        mov              rcx, qword ptr [rbp + 1112]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n10_lit_integer_α
                        mov              qword ptr [rbp + 1120], rax
                        mov              qword ptr [rbp + 1128], rdx
                                                                                        jmp   n9_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n9_lit_integer_α:
                        mov              qword ptr [rbp + 1152], 6
                        mov              rax, qword ptr [rip + .Lx83_0]
                        mov              qword ptr [rbp + 1160], rax
                                                                                        jmp   n11_assign_var_α
.Lx83_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n10_lit_integer_α:
                        mov              qword ptr [rbp + 608], 6
                        mov              rax, qword ptr [rip + .Lx84_0]
                        mov              qword ptr [rbp + 616], rax
                                                                                        jmp   n12_var_α
.Lx84_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n11_assign_var_α:
                        mov              rdi, qword ptr [rbp + 1120]
                        mov              rsi, qword ptr [rbp + 1128]
                        mov              rdx, qword ptr [rbp + 1152]
                        mov              rcx, qword ptr [rbp + 1160]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n10_lit_integer_α
                        mov              qword ptr [rbp + 1136], rax
                        mov              qword ptr [rbp + 1144], rdx
                                                                                        jmp   n10_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n12_var_α:
                        mov              rax, qword ptr [rbp + 1312]
                        mov              qword ptr [rbp + 624], rax
                        mov              rax, qword ptr [rbp + 1320]
                        mov              qword ptr [rbp + 632], rax
                                                                                        jmp   n13_to_α
#-----------------------------------------------------------------------------------------------------------------------
n13_to_α:
                        mov              rdi, qword ptr [rbp + 608]
                        mov              rsi, qword ptr [rbp + 616]
                        call             to_int@PLT
                        mov              qword ptr [rbp + 608], 6
                        mov              qword ptr [rbp + 616], rax
                        mov              rdi, qword ptr [rbp + 624]
                        mov              rsi, qword ptr [rbp + 632]
                        call             to_int@PLT
                        mov              qword ptr [rbp + 624], 6
                        mov              qword ptr [rbp + 632], rax
                        mov              rax, qword ptr [rbp + 616]
                        mov              qword ptr [rbp + 592], rax
.Lx89_0:
                        mov              rax, qword ptr [rbp + 592]
                        mov              rcx, qword ptr [rbp + 632]
                        cmp              rax, rcx
                                                                                        jg    n14_lit_string_α
                        mov              qword ptr [rbp + 576], 6
                        mov              qword ptr [rbp + 584], rax
                                                                                        jmp   n15_assign_α
n13_to_β:
                        inc              qword ptr [rbp + 592]
                                                                                        jmp   .Lx89_0
#-----------------------------------------------------------------------------------------------------------------------
n14_lit_string_α:
                        mov              qword ptr [rbp + 560], 1
                        mov              rax, qword ptr [rip + .Lx90_0]
                        mov              qword ptr [rbp + 568], rax
                                                                                        jmp   n16_assign_α
.Lx90_0:
                        .quad            .Lx90_0_s
.Lx90_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n15_assign_α:
                        mov              rax, qword ptr [rbp + 576]
                        mov              rdx, qword ptr [rbp + 584]
                        mov              qword ptr [rbp + 1296], rax
                        mov              qword ptr [rbp + 1304], rdx
                                                                                        jmp   n17_bound_α
#-----------------------------------------------------------------------------------------------------------------------
n16_assign_α:
                        mov              rax, qword ptr [rbp + 560]
                        mov              rdx, qword ptr [rbp + 568]
                        mov              qword ptr [rbp + 1280], rax
                        mov              qword ptr [rbp + 1288], rdx
                                                                                        jmp   n18_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n17_bound_α:
                        mov              qword ptr [rbp + 640], rsp
                                                                                        jmp   n19_disjunction_α
#-----------------------------------------------------------------------------------------------------------------------
n18_lit_integer_α:
                        mov              qword ptr [rbp + 112], 6
                        mov              rax, qword ptr [rip + .Lx95_0]
                        mov              qword ptr [rbp + 120], rax
                                                                                        jmp   n20_var_α
.Lx95_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n19_disjunction_α:
                        mov              qword ptr [rbp + 688], 0
                        mov              qword ptr [rbp + 696], 0
                        mov              dword ptr [rbp + 704], 0
                                                                                        jmp   n22_var_ref_α
n19_disjunction_as:
                        mov              eax, dword ptr [rbp + 704]
                        cmp              eax, 0
                                                                                        jne   .Lx97_0
                                                                                        jmp   n21_unmark_α
.Lx97_0:
                                                                                        jmp   n21_unmark_α
n19_disjunction_β:
                        mov              eax, dword ptr [rbp + 704]
                                                                                        jmp   n21_unmark_α
n19_disjunction_af:
                        add              dword ptr [rbp + 704], 1
                        mov              eax, dword ptr [rbp + 704]
                                                                                        jmp   n21_unmark_α
#-----------------------------------------------------------------------------------------------------------------------
n20_var_α:
                        mov              rax, qword ptr [rbp + 1312]
                        mov              qword ptr [rbp + 128], rax
                        mov              rax, qword ptr [rbp + 1320]
                        mov              qword ptr [rbp + 136], rax
                                                                                        jmp   n23_to_α
#-----------------------------------------------------------------------------------------------------------------------
n21_unmark_α:
                        mov              rsp, qword ptr [rbp + 640]
                                                                                        jmp   n13_to_β
#-----------------------------------------------------------------------------------------------------------------------
n22_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 1328]
                        mov              qword ptr [rbp + 1008], rax
                        mov              qword ptr [rbp + 1016], rdx
                                                                                        jmp   n25_var_α
n22_var_ref_β:
                                                                                        jmp   n19_disjunction_af
#-----------------------------------------------------------------------------------------------------------------------
n23_to_α:
                        mov              rdi, qword ptr [rbp + 112]
                        mov              rsi, qword ptr [rbp + 120]
                        call             to_int@PLT
                        mov              qword ptr [rbp + 112], 6
                        mov              qword ptr [rbp + 120], rax
                        mov              rdi, qword ptr [rbp + 128]
                        mov              rsi, qword ptr [rbp + 136]
                        call             to_int@PLT
                        mov              qword ptr [rbp + 128], 6
                        mov              qword ptr [rbp + 136], rax
                        mov              rax, qword ptr [rbp + 120]
                        mov              qword ptr [rbp + 96], rax
.Lx105_0:
                        mov              rax, qword ptr [rbp + 96]
                        mov              rcx, qword ptr [rbp + 136]
                        cmp              rax, rcx
                                                                                        jg    n24_var_α
                        mov              qword ptr [rbp + 80], 6
                        mov              qword ptr [rbp + 88], rax
                                                                                        jmp   n26_assign_α
n23_to_β:
                        inc              qword ptr [rbp + 96]
                                                                                        jmp   .Lx105_0
#-----------------------------------------------------------------------------------------------------------------------
n24_var_α:
                        mov              rax, qword ptr [rbp + 1280]
                        mov              qword ptr [rbp + 48], rax
                        mov              rax, qword ptr [rbp + 1288]
                        mov              qword ptr [rbp + 56], rax
                                                                                        jmp   n27_call_builtin_icon_α
#-----------------------------------------------------------------------------------------------------------------------
n25_var_α:
                        mov              rax, qword ptr [rbp + 1296]
                        mov              qword ptr [rbp + 1024], rax
                        mov              rax, qword ptr [rbp + 1304]
                        mov              qword ptr [rbp + 1032], rax
                                                                                        jmp   n28_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n26_assign_α:
                        mov              rax, qword ptr [rbp + 80]
                        mov              rdx, qword ptr [rbp + 88]
                        mov              qword ptr [rbp + 1296], rax
                        mov              qword ptr [rbp + 1304], rdx
                                                                                        jmp   n29_bound_α
#-----------------------------------------------------------------------------------------------------------------------
n27_call_builtin_icon_α:
                        mov              rax, qword ptr [rbp + 48]
                        mov              qword ptr [rbp + 16], rax
                        mov              rax, qword ptr [rbp + 56]
                        mov              qword ptr [rbp + 24], rax
                        .section         .rodata
.Lrkfn112:              .string          "write"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn112]
                        lea              rsi, [rbp + 16]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 0], rax
                        mov              qword ptr [rbp + 8], rdx
                        cmp              eax, 99
                                                                                        je    main_ω
                                                                                        jmp   main_ω
n27_call_builtin_icon_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n28_subscript_α:
                        mov              rdi, qword ptr [rbp + 1008]
                        mov              rsi, qword ptr [rbp + 1016]
                        mov              rdx, qword ptr [rbp + 1024]
                        mov              rcx, qword ptr [rbp + 1032]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n19_disjunction_af
                        mov              qword ptr [rbp + 1040], rax
                        mov              qword ptr [rbp + 1048], rdx
                                                                                        jmp   n30_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n29_bound_α:
                        mov              qword ptr [rbp + 144], rsp
                                                                                        jmp   n31_disjunction_α
#-----------------------------------------------------------------------------------------------------------------------
n30_deref_α:
                        mov              rdi, qword ptr [rbp + 1040]
                        mov              rsi, qword ptr [rbp + 1048]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n19_disjunction_af
                        mov              qword ptr [rbp + 1056], rax
                        mov              qword ptr [rbp + 1064], rdx
                                                                                        jmp   n32_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n31_disjunction_α:
                        mov              qword ptr [rbp + 192], 0
                        mov              qword ptr [rbp + 200], 0
                        mov              dword ptr [rbp + 208], 0
                                                                                        jmp   n34_var_ref_α
n31_disjunction_as:
                        mov              eax, dword ptr [rbp + 208]
                        cmp              eax, 0
                                                                                        jne   .Lx118_0
                        mov              rax, qword ptr [rbp + 240]
                        mov              qword ptr [rbp + 192], rax
                        mov              rax, qword ptr [rbp + 248]
                        mov              qword ptr [rbp + 200], rax
                                                                                        jmp   n33_unmark_α
.Lx118_0:
                                                                                        jmp   n33_unmark_α
n31_disjunction_β:
                        mov              eax, dword ptr [rbp + 208]
                                                                                        jmp   n33_unmark_α
n31_disjunction_af:
                        add              dword ptr [rbp + 208], 1
                        mov              eax, dword ptr [rbp + 208]
                                                                                        jmp   n33_unmark_α
#-----------------------------------------------------------------------------------------------------------------------
n32_lit_integer_α:
                        mov              qword ptr [rbp + 1072], 6
                        mov              rax, qword ptr [rip + .Lx119_0]
                        mov              qword ptr [rbp + 1080], rax
                                                                                        jmp   n36_binop_test_α
.Lx119_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n33_unmark_α:
                        mov              rsp, qword ptr [rbp + 144]
                                                                                        jmp   n23_to_β
#-----------------------------------------------------------------------------------------------------------------------
n34_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 1328]
                        mov              qword ptr [rbp + 480], rax
                        mov              qword ptr [rbp + 488], rdx
                                                                                        jmp   n37_var_α
n34_var_ref_β:
                                                                                        jmp   n31_disjunction_af
#-----------------------------------------------------------------------------------------------------------------------
n35_conjunction_α:
                                                                                        jmp   n31_disjunction_as
n35_conjunction_β:
                                                                                        jmp   n33_unmark_α
#-----------------------------------------------------------------------------------------------------------------------
n36_binop_test_α:
                        mov              eax, dword ptr [rbp + 1056]
                        cmp              eax, 100
                                                                                        je    .Lx125_0
                        mov              eax, dword ptr [rbp + 1072]
                        cmp              eax, 100
                                                                                        je    .Lx125_0
                        mov              eax, dword ptr [rbp + 1056]
                        cmp              eax, 6
                                                                                        jne   .Lx125_2
                        mov              eax, dword ptr [rbp + 1072]
                        cmp              eax, 6
                                                                                        jne   .Lx125_2
.Lx125_1:
                        mov              rax, qword ptr [rbp + 1064]
                        mov              rcx, qword ptr [rbp + 1080]
                        cmp              rax, rcx
                                                                                        jne   n19_disjunction_af
                        mov              rcx, qword ptr [rbp + 1072]
                        mov              qword ptr [rbp + 992], rcx
                        mov              rcx, qword ptr [rbp + 1080]
                        mov              qword ptr [rbp + 1000], rcx
                                                                                        jmp   n38_var_α
.Lx125_0:
                        mov              rdi, qword ptr [rbp + 1056]
                        mov              rsi, qword ptr [rbp + 1064]
                        mov              rdx, qword ptr [rbp + 1072]
                        mov              rcx, qword ptr [rbp + 1080]
                        mov              r8d, 9
                        lea              r9, [rbp + 992]
                        call             rt_relop_overload@PLT
                        test             eax, eax
                                                                                        je    .Lx125_1
                        cmp              eax, 1
                                                                                        je    n19_disjunction_af
                                                                                        jmp   n38_var_α
.Lx125_2:
                        mov              rdi, qword ptr [rbp + 1056]
                        mov              rsi, qword ptr [rbp + 1064]
                        mov              rdx, qword ptr [rbp + 1072]
                        mov              rcx, qword ptr [rbp + 1080]
                        mov              r8d, 9
                        call             rt_jct_relop@PLT
                        test             eax, eax
                                                                                        jz    n19_disjunction_af
                        mov              rax, qword ptr [rbp + 1072]
                        mov              qword ptr [rbp + 992], rax
                        mov              rax, qword ptr [rbp + 1080]
                        mov              qword ptr [rbp + 1000], rax
                                                                                        jmp   n38_var_α
#-----------------------------------------------------------------------------------------------------------------------
n37_var_α:
                        mov              rax, qword ptr [rbp + 1296]
                        mov              qword ptr [rbp + 496], rax
                        mov              rax, qword ptr [rbp + 1304]
                        mov              qword ptr [rbp + 504], rax
                                                                                        jmp   n39_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n38_var_α:
                        mov              rax, qword ptr [rbp + 1296]
                        mov              qword ptr [rbp + 800], rax
                        mov              rax, qword ptr [rbp + 1304]
                        mov              qword ptr [rbp + 808], rax
                                                                                        jmp   n40_var_α
#-----------------------------------------------------------------------------------------------------------------------
n39_subscript_α:
                        mov              rdi, qword ptr [rbp + 480]
                        mov              rsi, qword ptr [rbp + 488]
                        mov              rdx, qword ptr [rbp + 496]
                        mov              rcx, qword ptr [rbp + 504]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n31_disjunction_af
                        mov              qword ptr [rbp + 512], rax
                        mov              qword ptr [rbp + 520], rdx
                                                                                        jmp   n41_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n40_var_α:
                        mov              rax, qword ptr [rbp + 1296]
                        mov              qword ptr [rbp + 816], rax
                        mov              rax, qword ptr [rbp + 1304]
                        mov              qword ptr [rbp + 824], rax
                                                                                        jmp   n42_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n41_deref_α:
                        mov              rdi, qword ptr [rbp + 512]
                        mov              rsi, qword ptr [rbp + 520]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n31_disjunction_af
                        mov              qword ptr [rbp + 528], rax
                        mov              qword ptr [rbp + 536], rdx
                                                                                        jmp   n43_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n42_op75_α:
                        mov              eax, dword ptr [rbp + 1296]
                        cmp              eax, 7
                                                                                        je    .Lx135_1
                        cmp              eax, 6
                                                                                        jne   .Lx135_0
                        mov              eax, dword ptr [rbp + 1296]
                        cmp              eax, 6
                                                                                        jne   .Lx135_0
.Lx135_1:
                        mov              rax, qword ptr [rbp + 1296]
                        mov              qword ptr [rbp + 784], rax
                        mov              rax, qword ptr [rbp + 1304]
                        mov              qword ptr [rbp + 792], rax
                                                                                        jmp   n44_op75_α
.Lx135_0:
                        lea              rdi, [rbp + 1296]
                        lea              rsi, [rbp + 1296]
                        lea              rdx, [rbp + 784]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n44_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n43_lit_integer_α:
                        mov              qword ptr [rbp + 544], 6
                        mov              rax, qword ptr [rip + .Lx136_0]
                        mov              qword ptr [rbp + 552], rax
                                                                                        jmp   n45_binop_test_α
.Lx136_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n44_op75_α:
                        mov              eax, dword ptr [rbp + 1296]
                        cmp              eax, 7
                                                                                        je    .Lx138_1
                        cmp              eax, 6
                                                                                        jne   .Lx138_0
                        mov              eax, dword ptr [rbp + 1296]
                        cmp              eax, 6
                                                                                        jne   .Lx138_0
.Lx138_1:
                        mov              rax, qword ptr [rbp + 1296]
                        mov              qword ptr [rbp + 768], rax
                        mov              rax, qword ptr [rbp + 1304]
                        mov              qword ptr [rbp + 776], rax
                                                                                        jmp   n46_binop_α
.Lx138_0:
                        lea              rdi, [rbp + 1296]
                        lea              rsi, [rbp + 1296]
                        lea              rdx, [rbp + 768]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n46_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n45_binop_test_α:
                        mov              eax, dword ptr [rbp + 528]
                        cmp              eax, 100
                                                                                        je    .Lx139_0
                        mov              eax, dword ptr [rbp + 544]
                        cmp              eax, 100
                                                                                        je    .Lx139_0
                        mov              eax, dword ptr [rbp + 528]
                        cmp              eax, 6
                                                                                        jne   .Lx139_2
                        mov              eax, dword ptr [rbp + 544]
                        cmp              eax, 6
                                                                                        jne   .Lx139_2
.Lx139_1:
                        mov              rax, qword ptr [rbp + 536]
                        mov              rcx, qword ptr [rbp + 552]
                        cmp              rax, rcx
                                                                                        jne   n31_disjunction_af
                        mov              rcx, qword ptr [rbp + 544]
                        mov              qword ptr [rbp + 464], rcx
                        mov              rcx, qword ptr [rbp + 552]
                        mov              qword ptr [rbp + 472], rcx
                                                                                        jmp   n47_disjunction_α
.Lx139_0:
                        mov              rdi, qword ptr [rbp + 528]
                        mov              rsi, qword ptr [rbp + 536]
                        mov              rdx, qword ptr [rbp + 544]
                        mov              rcx, qword ptr [rbp + 552]
                        mov              r8d, 9
                        lea              r9, [rbp + 464]
                        call             rt_relop_overload@PLT
                        test             eax, eax
                                                                                        je    .Lx139_1
                        cmp              eax, 1
                                                                                        je    n31_disjunction_af
                                                                                        jmp   n47_disjunction_α
.Lx139_2:
                        mov              rdi, qword ptr [rbp + 528]
                        mov              rsi, qword ptr [rbp + 536]
                        mov              rdx, qword ptr [rbp + 544]
                        mov              rcx, qword ptr [rbp + 552]
                        mov              r8d, 9
                        call             rt_jct_relop@PLT
                        test             eax, eax
                                                                                        jz    n31_disjunction_af
                        mov              rax, qword ptr [rbp + 544]
                        mov              qword ptr [rbp + 464], rax
                        mov              rax, qword ptr [rbp + 552]
                        mov              qword ptr [rbp + 472], rax
                                                                                        jmp   n47_disjunction_α
#-----------------------------------------------------------------------------------------------------------------------
n46_binop_α:
                        mov              eax, dword ptr [rbp + 784]
                        cmp              eax, 6
                                                                                        jne   .Lx140_0
                        mov              eax, dword ptr [rbp + 768]
                        cmp              eax, 6
                                                                                        jne   .Lx140_0
                        mov              rax, qword ptr [rbp + 792]
                        mov              rcx, qword ptr [rbp + 776]
                        imul             rax, rcx
                        mov              qword ptr [rbp + 752], 6
                        mov              qword ptr [rbp + 760], rax
                                                                                        jmp   n48_var_α
.Lx140_0:
                        mov              rdi, qword ptr [rbp + 784]
                        mov              rsi, qword ptr [rbp + 792]
                        mov              rdx, qword ptr [rbp + 768]
                        mov              rcx, qword ptr [rbp + 776]
                        call             rt_mul@PLT
                        cmp              eax, 99
                                                                                        je    n21_unmark_α
                        mov              qword ptr [rbp + 752], rax
                        mov              qword ptr [rbp + 760], rdx
                                                                                        jmp   n48_var_α
#-----------------------------------------------------------------------------------------------------------------------
n47_disjunction_α:
                        mov              qword ptr [rbp + 304], 0
                        mov              qword ptr [rbp + 312], 0
                        mov              dword ptr [rbp + 320], 0
                                                                                        jmp   n50_var_α
n47_disjunction_as:
                        mov              eax, dword ptr [rbp + 320]
                        cmp              eax, 0
                                                                                        jne   .Lx142_0
                        mov              rax, qword ptr [rbp + 336]
                        mov              qword ptr [rbp + 304], rax
                        mov              rax, qword ptr [rbp + 344]
                        mov              qword ptr [rbp + 312], rax
                                                                                        jmp   n49_var_α
.Lx142_0:
                                                                                        jmp   n49_var_α
n47_disjunction_β:
                        mov              eax, dword ptr [rbp + 320]
                                                                                        jmp   n49_var_α
n47_disjunction_af:
                        add              dword ptr [rbp + 320], 1
                        mov              eax, dword ptr [rbp + 320]
                                                                                        jmp   n49_var_α
#-----------------------------------------------------------------------------------------------------------------------
n48_var_α:
                        mov              rax, qword ptr [rbp + 1312]
                        mov              qword ptr [rbp + 832], rax
                        mov              rax, qword ptr [rbp + 1320]
                        mov              qword ptr [rbp + 840], rax
                                                                                        jmp   n52_var_α
#-----------------------------------------------------------------------------------------------------------------------
n49_var_α:
                        mov              rax, qword ptr [rbp + 1280]
                        mov              qword ptr [rbp + 272], rax
                        mov              rax, qword ptr [rbp + 1288]
                        mov              qword ptr [rbp + 280], rax
                                                                                        jmp   n53_var_α
#-----------------------------------------------------------------------------------------------------------------------
n50_var_α:
                        mov              rax, qword ptr [rbp + 1280]
                        mov              qword ptr [rbp + 432], rax
                        mov              rax, qword ptr [rbp + 1288]
                        mov              qword ptr [rbp + 440], rax
                                                                                        jmp   n54_unop_α
n50_var_β:
                                                                                        jmp   n47_disjunction_af
#-----------------------------------------------------------------------------------------------------------------------
n51_assign_α:
                        mov              rax, qword ptr [rbp + 352]
                        mov              rdx, qword ptr [rbp + 360]
                        mov              qword ptr [rbp + 1280], rax
                        mov              qword ptr [rbp + 1288], rdx
                        mov              qword ptr [rbp + 336], rax
                        mov              qword ptr [rbp + 344], rdx
                                                                                        jmp   n47_disjunction_as
n51_assign_β:
                                                                                        jmp   n49_var_α
#-----------------------------------------------------------------------------------------------------------------------
n52_var_α:
                        mov              rax, qword ptr [rbp + 1296]
                        mov              qword ptr [rbp + 848], rax
                        mov              rax, qword ptr [rbp + 1304]
                        mov              qword ptr [rbp + 856], rax
                                                                                        jmp   n55_to_by_α
#-----------------------------------------------------------------------------------------------------------------------
n53_var_α:
                        mov              rax, qword ptr [rbp + 1296]
                        mov              qword ptr [rbp + 288], rax
                        mov              rax, qword ptr [rbp + 1304]
                        mov              qword ptr [rbp + 296], rax
                                                                                        jmp   n56_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n54_unop_α:
                        mov              rdi, qword ptr [rbp + 1280]
                        mov              rsi, qword ptr [rbp + 1288]
                        call             rt_size_d@PLT
                        mov              qword ptr [rbp + 416], rax
                        mov              qword ptr [rbp + 424], rdx
                                                                                        jmp   n57_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n55_to_by_α:
                        mov              rdi, qword ptr [rbp + 752]
                        mov              rsi, qword ptr [rbp + 760]
                        call             to_int@PLT
                        mov              qword ptr [rbp + 752], 6
                        mov              qword ptr [rbp + 760], rax
                        mov              rdi, qword ptr [rbp + 832]
                        mov              rsi, qword ptr [rbp + 840]
                        call             to_int@PLT
                        mov              qword ptr [rbp + 832], 6
                        mov              qword ptr [rbp + 840], rax
                        mov              rdi, qword ptr [rbp + 848]
                        mov              rsi, qword ptr [rbp + 856]
                        call             to_int@PLT
                        mov              qword ptr [rbp + 848], 6
                        mov              qword ptr [rbp + 856], rax
                        mov              rax, qword ptr [rbp + 760]
                        mov              qword ptr [rbp + 736], rax
.Lx156_0:
                        mov              rax, qword ptr [rbp + 736]
                        mov              rcx, qword ptr [rbp + 840]
                        mov              rdx, qword ptr [rbp + 856]
                        cmp              rdx, 0
                                                                                        jl    .Lx156_1
                        cmp              rax, rcx
                                                                                        jg    n21_unmark_α
                                                                                        jmp   .Lx156_2
.Lx156_1:
                        cmp              rax, rcx
                                                                                        jl    n21_unmark_α
.Lx156_2:
                        mov              qword ptr [rbp + 720], 6
                        mov              qword ptr [rbp + 728], rax
                                                                                        jmp   n58_assign_α
n55_to_by_β:
                        mov              rdx, qword ptr [rbp + 856]
                        mov              rax, qword ptr [rbp + 736]
                        add              rax, rdx
                        mov              qword ptr [rbp + 736], rax
                                                                                        jmp   .Lx156_0
#-----------------------------------------------------------------------------------------------------------------------
n56_binop_α:
                        mov              rdi, qword ptr [rbp + 1280]
                        mov              rsi, qword ptr [rbp + 1288]
                        mov              rdx, qword ptr [rbp + 1296]
                        mov              rcx, qword ptr [rbp + 1304]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 256], rax
                        mov              qword ptr [rbp + 264], rdx
                                                                                        jmp   n59_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n57_lit_integer_α:
                        mov              qword ptr [rbp + 448], 6
                        mov              rax, qword ptr [rip + .Lx158_0]
                        mov              qword ptr [rbp + 456], rax
                                                                                        jmp   n60_binop_test_α
.Lx158_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n58_assign_α:
                        mov              rax, qword ptr [rbp + 720]
                        mov              rdx, qword ptr [rbp + 728]
                        mov              qword ptr [rbp + 1344], rax
                        mov              qword ptr [rbp + 1352], rdx
                                                                                        jmp   n61_bound_α
#-----------------------------------------------------------------------------------------------------------------------
n59_assign_α:
                        mov              rax, qword ptr [rbp + 256]
                        mov              rdx, qword ptr [rbp + 264]
                        mov              qword ptr [rbp + 1280], rax
                        mov              qword ptr [rbp + 1288], rdx
                        mov              qword ptr [rbp + 240], rax
                        mov              qword ptr [rbp + 248], rdx
                                                                                        jmp   n35_conjunction_α
#-----------------------------------------------------------------------------------------------------------------------
n60_binop_test_α:
                        mov              eax, dword ptr [rbp + 416]
                        cmp              eax, 100
                                                                                        je    .Lx161_0
                        mov              eax, dword ptr [rbp + 448]
                        cmp              eax, 100
                                                                                        je    .Lx161_0
                        mov              eax, dword ptr [rbp + 416]
                        cmp              eax, 6
                                                                                        jne   .Lx161_2
                        mov              eax, dword ptr [rbp + 448]
                        cmp              eax, 6
                                                                                        jne   .Lx161_2
.Lx161_1:
                        mov              rax, qword ptr [rbp + 424]
                        mov              rcx, qword ptr [rbp + 456]
                        cmp              rax, rcx
                                                                                        jle   n47_disjunction_af
                        mov              rcx, qword ptr [rbp + 448]
                        mov              qword ptr [rbp + 400], rcx
                        mov              rcx, qword ptr [rbp + 456]
                        mov              qword ptr [rbp + 408], rcx
                                                                                        jmp   n62_var_α
.Lx161_0:
                        mov              rdi, qword ptr [rbp + 416]
                        mov              rsi, qword ptr [rbp + 424]
                        mov              rdx, qword ptr [rbp + 448]
                        mov              rcx, qword ptr [rbp + 456]
                        mov              r8d, 7
                        lea              r9, [rbp + 400]
                        call             rt_relop_overload@PLT
                        test             eax, eax
                                                                                        je    .Lx161_1
                        cmp              eax, 1
                                                                                        je    n47_disjunction_af
                                                                                        jmp   n62_var_α
.Lx161_2:
                        mov              rdi, qword ptr [rbp + 416]
                        mov              rsi, qword ptr [rbp + 424]
                        mov              rdx, qword ptr [rbp + 448]
                        mov              rcx, qword ptr [rbp + 456]
                        mov              r8d, 7
                        call             rt_jct_relop@PLT
                        test             eax, eax
                                                                                        jz    n47_disjunction_af
                        mov              rax, qword ptr [rbp + 448]
                        mov              qword ptr [rbp + 400], rax
                        mov              rax, qword ptr [rbp + 456]
                        mov              qword ptr [rbp + 408], rax
                                                                                        jmp   n62_var_α
#-----------------------------------------------------------------------------------------------------------------------
n61_bound_α:
                        mov              qword ptr [rbp + 864], rsp
                                                                                        jmp   n63_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n62_var_α:
                        mov              rax, qword ptr [rbp + 1280]
                        mov              qword ptr [rbp + 368], rax
                        mov              rax, qword ptr [rbp + 1288]
                        mov              qword ptr [rbp + 376], rax
                                                                                        jmp   n64_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n63_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 1328]
                        mov              qword ptr [rbp + 912], rax
                        mov              qword ptr [rbp + 920], rdx
                                                                                        jmp   n65_var_α
#-----------------------------------------------------------------------------------------------------------------------
n64_lit_string_α:
                        mov              qword ptr [rbp + 384], 1
                        mov              rax, qword ptr [rip + .Lx168_0]
                        mov              qword ptr [rbp + 392], rax
                                                                                        jmp   n66_binop_α
.Lx168_0:
                        .quad            .Lx168_0_s
.Lx168_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n65_var_α:
                        mov              rax, qword ptr [rbp + 1344]
                        mov              qword ptr [rbp + 928], rax
                        mov              rax, qword ptr [rbp + 1352]
                        mov              qword ptr [rbp + 936], rax
                                                                                        jmp   n67_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n66_binop_α:
                        mov              rdi, qword ptr [rbp + 1280]
                        mov              rsi, qword ptr [rbp + 1288]
                        mov              rdx, qword ptr [rbp + 384]
                        mov              rcx, qword ptr [rbp + 392]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 352], rax
                        mov              qword ptr [rbp + 360], rdx
                                                                                        jmp   n51_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n67_subscript_α:
                        mov              rdi, qword ptr [rbp + 912]
                        mov              rsi, qword ptr [rbp + 920]
                        mov              rdx, qword ptr [rbp + 928]
                        mov              rcx, qword ptr [rbp + 936]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n68_unmark_α
                        mov              qword ptr [rbp + 944], rax
                        mov              qword ptr [rbp + 952], rdx
                                                                                        jmp   n69_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n68_unmark_α:
                        mov              rsp, qword ptr [rbp + 864]
                                                                                        jmp   n55_to_by_β
#-----------------------------------------------------------------------------------------------------------------------
n69_lit_integer_α:
                        mov              qword ptr [rbp + 976], 6
                        mov              rax, qword ptr [rip + .Lx175_0]
                        mov              qword ptr [rbp + 984], rax
                                                                                        jmp   n70_assign_var_α
.Lx175_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n70_assign_var_α:
                        mov              rdi, qword ptr [rbp + 944]
                        mov              rsi, qword ptr [rbp + 952]
                        mov              rdx, qword ptr [rbp + 976]
                        mov              rcx, qword ptr [rbp + 984]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n68_unmark_α
                        mov              qword ptr [rbp + 960], rax
                        mov              qword ptr [rbp + 968], rdx
                                                                                        jmp   n68_unmark_α
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        mov              rsp, rbp
                        mov              rbp, [rsp + 1360]
                        add              rsp, 1368
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              rsp, rbp
                        mov              eax, 99
                        xor              edx, edx
                        mov              rbp, [rsp + 1360]
                        add              rsp, 1368
                        ret
                        .section         .note.GNU-stack,"",@progbits

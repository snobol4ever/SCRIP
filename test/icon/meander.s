                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_meander_α
proc_meander_α:
                        .global          proc_meander_α
                        .global          proc_meander_β
                        .global          proc_meander_γ
                        .global          proc_meander_ω
                        sub              rsp, 928
                        mov              [rsp + 904], rcx
                        mov              [rsp + 912], rdx
                        mov              [rsp + 920], rbp
                        mov              rbp, rsp
                        mov              rdi, rsp
                        mov              esi, 816
                        mov              edx, 896
                        call             rt_jmp_frame_lexprep2@PLT
proc_meander_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n0_var_α:
                        mov              rax, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + 800], rax
                        mov              rax, qword ptr [rbp + 24]
                        mov              qword ptr [rbp + 808], rax
                                                                                        jmp   n1_unop_α
#-----------------------------------------------------------------------------------------------------------------------
n1_unop_α:
                        mov              rdi, qword ptr [rbp + 16]
                        mov              rsi, qword ptr [rbp + 24]
                        call             rt_size_d@PLT
                        mov              qword ptr [rbp + 784], rax
                        mov              qword ptr [rbp + 792], rdx
                                                                                        jmp   n3_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n2_var_α:
                        mov              rax, qword ptr [rbp + 32]
                        mov              qword ptr [rbp + 736], rax
                        mov              rax, qword ptr [rbp + 40]
                        mov              qword ptr [rbp + 744], rax
                                                                                        jmp   n4_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n3_assign_α:
                        mov              rax, qword ptr [rbp + 784]
                        mov              rdx, qword ptr [rbp + 792]
                        mov              qword ptr [rbp + 880], rax
                        mov              qword ptr [rbp + 888], rdx
                        mov              qword ptr [rbp + 768], rax
                        mov              qword ptr [rbp + 776], rdx
                                                                                        jmp   n6_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n4_lit_integer_α:
                        mov              qword ptr [rbp + 752], 6
                        mov              rax, qword ptr [rip + .Lx52_0]
                        mov              qword ptr [rbp + 760], rax
                                                                                        jmp   n7_op75_α
.Lx52_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n5_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 16]
                        mov              qword ptr [rbp + 624], rax
                        mov              qword ptr [rbp + 632], rdx
                                                                                        jmp   n8_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n6_assign_α:
                        mov              rax, qword ptr [rbp + 768]
                        mov              rdx, qword ptr [rbp + 776]
                        mov              qword ptr [rbp + 848], rax
                        mov              qword ptr [rbp + 856], rdx
                                                                                        jmp   n2_var_α
#-----------------------------------------------------------------------------------------------------------------------
n7_op75_α:
                        mov              eax, dword ptr [rbp + 32]
                        cmp              eax, 7
                                                                                        je    .Lx57_1
                        cmp              eax, 6
                                                                                        jne   .Lx57_0
                        mov              eax, dword ptr [rbp + 752]
                        cmp              eax, 6
                                                                                        jne   .Lx57_0
.Lx57_1:
                        mov              rax, qword ptr [rbp + 32]
                        mov              qword ptr [rbp + 720], rax
                        mov              rax, qword ptr [rbp + 40]
                        mov              qword ptr [rbp + 728], rax
                                                                                        jmp   n9_binop_α
.Lx57_0:
                        lea              rdi, [rbp + 32]
                        lea              rsi, [rbp + 752]
                        lea              rdx, [rbp + 720]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n9_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n8_lit_integer_α:
                        mov              qword ptr [rbp + 640], 6
                        mov              rax, qword ptr [rip + .Lx58_0]
                        mov              qword ptr [rbp + 648], rax
                                                                                        jmp   n10_subscript_α
.Lx58_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n9_binop_α:
                        mov              eax, dword ptr [rbp + 720]
                        cmp              eax, 6
                                                                                        jne   .Lx59_0
                        mov              rax, qword ptr [rbp + 728]
                        mov              rcx, 1
                        sub              rax, rcx
                        mov              qword ptr [rbp + 704], 6
                        mov              qword ptr [rbp + 712], rax
                                                                                        jmp   n11_assign_α
.Lx59_0:
                        mov              rdi, qword ptr [rbp + 720]
                        mov              rsi, qword ptr [rbp + 728]
                        mov              rdx, qword ptr [rbp + 752]
                        mov              rcx, qword ptr [rbp + 760]
                        call             rt_sub@PLT
                        cmp              eax, 99
                                                                                        je    n5_var_ref_α
                        mov              qword ptr [rbp + 704], rax
                        mov              qword ptr [rbp + 712], rdx
                                                                                        jmp   n11_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n10_subscript_α:
                        mov              rdi, qword ptr [rbp + 624]
                        mov              rsi, qword ptr [rbp + 632]
                        mov              rdx, qword ptr [rbp + 640]
                        mov              rcx, qword ptr [rbp + 648]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n13_var_ref_α
                        mov              qword ptr [rbp + 656], rax
                        mov              qword ptr [rbp + 664], rdx
                                                                                        jmp   n12_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n11_assign_α:
                        mov              rax, qword ptr [rbp + 704]
                        mov              rdx, qword ptr [rbp + 712]
                        mov              qword ptr [rbp + 864], rax
                        mov              qword ptr [rbp + 872], rdx
                                                                                        jmp   n5_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n12_deref_α:
                        mov              rdi, qword ptr [rbp + 656]
                        mov              rsi, qword ptr [rbp + 664]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n13_var_ref_α
                        mov              qword ptr [rbp + 672], rax
                        mov              qword ptr [rbp + 680], rdx
                                                                                        jmp   n14_var_α
#-----------------------------------------------------------------------------------------------------------------------
n13_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 16]
                        mov              qword ptr [rbp + 80], rax
                        mov              qword ptr [rbp + 88], rdx
                                                                                        jmp   n15_var_α
#-----------------------------------------------------------------------------------------------------------------------
n14_var_α:
                        mov              rax, qword ptr [rbp + 864]
                        mov              qword ptr [rbp + 688], rax
                        mov              rax, qword ptr [rbp + 872]
                        mov              qword ptr [rbp + 696], rax
                                                                                        jmp   n16_call_builtin_icon_α
#-----------------------------------------------------------------------------------------------------------------------
n15_var_α:
                        mov              rax, qword ptr [rbp + 848]
                        mov              qword ptr [rbp + 96], rax
                        mov              rax, qword ptr [rbp + 856]
                        mov              qword ptr [rbp + 104], rax
                                                                                        jmp   n17_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n16_call_builtin_icon_α:
                        mov              rax, qword ptr [rbp + 672]
                        mov              qword ptr [rbp + 576], rax
                        mov              rax, qword ptr [rbp + 680]
                        mov              qword ptr [rbp + 584], rax
                        mov              rax, qword ptr [rbp + 688]
                        mov              qword ptr [rbp + 592], rax
                        mov              rax, qword ptr [rbp + 696]
                        mov              qword ptr [rbp + 600], rax
                        .section         .rodata
.Lrkfn70:               .string          "repl"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn70]
                        lea              rsi, [rbp + 576]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 560], rax
                        mov              qword ptr [rbp + 568], rdx
                        cmp              eax, 99
                                                                                        je    n13_var_ref_α
                                                                                        jmp   n19_assign_α
n16_call_builtin_icon_β:
                                                                                        jmp   n13_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n17_subscript_α:
                        mov              rdi, qword ptr [rbp + 80]
                        mov              rsi, qword ptr [rbp + 88]
                        mov              rdx, qword ptr [rbp + 96]
                        mov              rcx, qword ptr [rbp + 104]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n18_var_α
                        mov              qword ptr [rbp + 112], rax
                        mov              qword ptr [rbp + 120], rdx
                                                                                        jmp   n20_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n18_var_α:
                        mov              rax, qword ptr [rbp + 816]
                        mov              qword ptr [rbp + 48], rax
                        mov              rax, qword ptr [rbp + 824]
                        mov              qword ptr [rbp + 56], rax
                                                                                        jmp   n21_return_α
#-----------------------------------------------------------------------------------------------------------------------
n19_assign_α:
                        mov              rax, qword ptr [rbp + 560]
                        mov              rdx, qword ptr [rbp + 568]
                        mov              qword ptr [rbp + 816], rax
                        mov              qword ptr [rbp + 824], rdx
                                                                                        jmp   n13_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n20_deref_α:
                        mov              rdi, qword ptr [rbp + 112]
                        mov              rsi, qword ptr [rbp + 120]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n18_var_α
                        mov              qword ptr [rbp + 128], rax
                        mov              qword ptr [rbp + 136], rdx
                                                                                        jmp   n22_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n21_return_α:
                        mov              rax, qword ptr [rbp + 48]
                        mov              rdx, qword ptr [rbp + 56]
                        mov              qword ptr [rbp + 0], rax
                        mov              qword ptr [rbp + 8], rdx
                                                                                        jmp   proc_meander_γ
#-----------------------------------------------------------------------------------------------------------------------
n22_assign_α:
                        mov              rax, qword ptr [rbp + 128]
                        mov              rdx, qword ptr [rbp + 136]
                        mov              qword ptr [rbp + 832], rax
                        mov              qword ptr [rbp + 840], rdx
                                                                                        jmp   n23_disjunction_α
#-----------------------------------------------------------------------------------------------------------------------
n23_disjunction_α:
                        mov              qword ptr [rbp + 144], 0
                        mov              qword ptr [rbp + 152], 0
                        mov              dword ptr [rbp + 160], 0
                                                                                        jmp   n24_var_α
n23_disjunction_as:
                        mov              eax, dword ptr [rbp + 160]
                        cmp              eax, 0
                                                                                        jne   .Lx79_0
                        mov              rax, qword ptr [rbp + 176]
                        mov              qword ptr [rbp + 144], rax
                        mov              rax, qword ptr [rbp + 184]
                        mov              qword ptr [rbp + 152], rax
                                                                                        jmp   n13_var_ref_α
.Lx79_0:
                        cmp              eax, 1
                                                                                        jne   .Lx79_1
                        mov              rax, qword ptr [rbp + 480]
                        mov              qword ptr [rbp + 144], rax
                        mov              rax, qword ptr [rbp + 488]
                        mov              qword ptr [rbp + 152], rax
                                                                                        jmp   n13_var_ref_α
.Lx79_1:
                                                                                        jmp   n13_var_ref_α
n23_disjunction_β:
                        mov              eax, dword ptr [rbp + 160]
                        cmp              eax, 0
                                                                                        je    n13_var_ref_α
                                                                                        jmp   n13_var_ref_α
n23_disjunction_af:
                        add              dword ptr [rbp + 160], 1
                        mov              eax, dword ptr [rbp + 160]
                        cmp              eax, 1
                                                                                        je    n25_var_α
                                                                                        jmp   n13_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n24_var_α:
                        mov              rax, qword ptr [rbp + 816]
                        mov              qword ptr [rbp + 352], rax
                        mov              rax, qword ptr [rbp + 824]
                        mov              qword ptr [rbp + 360], rax
                                                                                        jmp   n28_var_α
n24_var_β:
                                                                                        jmp   n23_disjunction_af
#-----------------------------------------------------------------------------------------------------------------------
n25_var_α:
                        mov              rax, qword ptr [rbp + 816]
                        mov              qword ptr [rbp + 528], rax
                        mov              rax, qword ptr [rbp + 824]
                        mov              qword ptr [rbp + 536], rax
                                                                                        jmp   n29_var_α
n25_var_β:
                                                                                        jmp   n30_var_α
#-----------------------------------------------------------------------------------------------------------------------
n26_assign_α:
                        mov              rax, qword ptr [rbp + 192]
                        mov              rdx, qword ptr [rbp + 200]
                        mov              qword ptr [rbp + 848], rax
                        mov              qword ptr [rbp + 856], rdx
                        mov              qword ptr [rbp + 176], rax
                        mov              qword ptr [rbp + 184], rdx
                                                                                        jmp   n23_disjunction_as
n26_assign_β:
                                                                                        jmp   n13_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n27_conjunction_α:
                                                                                        jmp   n23_disjunction_as
n27_conjunction_β:
                                                                                        jmp   n13_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n28_var_α:
                        mov              rax, qword ptr [rbp + 864]
                        mov              qword ptr [rbp + 400], rax
                        mov              rax, qword ptr [rbp + 872]
                        mov              qword ptr [rbp + 408], rax
                                                                                        jmp   n31_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n29_var_α:
                        mov              rax, qword ptr [rbp + 832]
                        mov              qword ptr [rbp + 544], rax
                        mov              rax, qword ptr [rbp + 840]
                        mov              qword ptr [rbp + 552], rax
                                                                                        jmp   n32_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n30_var_α:
                        mov              rax, qword ptr [rbp + 880]
                        mov              qword ptr [rbp + 496], rax
                        mov              rax, qword ptr [rbp + 888]
                        mov              qword ptr [rbp + 504], rax
                                                                                        jmp   n33_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n31_op75_α:
                        mov              eax, dword ptr [rbp + 864]
                        cmp              eax, 7
                                                                                        je    .Lx93_1
                        cmp              eax, 6
                                                                                        jne   .Lx93_0
                        mov              eax, dword ptr [rbp + 864]
                        cmp              eax, 6
                                                                                        jne   .Lx93_0
.Lx93_1:
                        mov              rax, qword ptr [rbp + 864]
                        mov              qword ptr [rbp + 384], rax
                        mov              rax, qword ptr [rbp + 872]
                        mov              qword ptr [rbp + 392], rax
                                                                                        jmp   n34_unop_α
.Lx93_0:
                        lea              rdi, [rbp + 864]
                        lea              rsi, [rbp + 864]
                        lea              rdx, [rbp + 384]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n34_unop_α
#-----------------------------------------------------------------------------------------------------------------------
n32_binop_α:
                        mov              rdi, qword ptr [rbp + 816]
                        mov              rsi, qword ptr [rbp + 824]
                        mov              rdx, qword ptr [rbp + 832]
                        mov              rcx, qword ptr [rbp + 840]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 512], rax
                        mov              qword ptr [rbp + 520], rdx
                                                                                        jmp   n35_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n33_assign_α:
                        mov              rax, qword ptr [rbp + 496]
                        mov              rdx, qword ptr [rbp + 504]
                        mov              qword ptr [rbp + 848], rax
                        mov              qword ptr [rbp + 856], rdx
                        mov              qword ptr [rbp + 480], rax
                        mov              qword ptr [rbp + 488], rdx
                                                                                        jmp   n27_conjunction_α
#-----------------------------------------------------------------------------------------------------------------------
n34_unop_α:
                        mov              rdi, qword ptr [rbp + 384]
                        mov              rsi, qword ptr [rbp + 392]
                        call             rt_num_neg@PLT
                        mov              qword ptr [rbp + 368], rax
                        mov              qword ptr [rbp + 376], rdx
                                                                                        jmp   n36_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n35_assign_α:
                        mov              rax, qword ptr [rbp + 512]
                        mov              rdx, qword ptr [rbp + 520]
                        mov              qword ptr [rbp + 816], rax
                        mov              qword ptr [rbp + 824], rdx
                                                                                        jmp   n30_var_α
#-----------------------------------------------------------------------------------------------------------------------
n36_lit_integer_α:
                        mov              qword ptr [rbp + 416], 6
                        mov              rax, qword ptr [rip + .Lx98_0]
                        mov              qword ptr [rbp + 424], rax
                                                                                        jmp   n37_subscript_α
.Lx98_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n37_subscript_α:
                        mov              rdi, qword ptr [rbp + 352]
                        mov              rsi, qword ptr [rbp + 360]
                        mov              rdx, qword ptr [rbp + 368]
                        mov              rcx, qword ptr [rbp + 376]
                        mov              r8, qword ptr [rbp + 416]
                        mov              r9, qword ptr [rbp + 424]
                        call             subscript_get2@PLT
                        cmp              eax, 99
                                                                                        je    n23_disjunction_af
                        mov              qword ptr [rbp + 336], rax
                        mov              qword ptr [rbp + 344], rdx
                                                                                        jmp   n38_var_α
#-----------------------------------------------------------------------------------------------------------------------
n38_var_α:
                        mov              rax, qword ptr [rbp + 832]
                        mov              qword ptr [rbp + 432], rax
                        mov              rax, qword ptr [rbp + 840]
                        mov              qword ptr [rbp + 440], rax
                                                                                        jmp   n39_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n39_binop_α:
                        mov              rdi, qword ptr [rbp + 336]
                        mov              rsi, qword ptr [rbp + 344]
                        mov              rdx, qword ptr [rbp + 832]
                        mov              rcx, qword ptr [rbp + 840]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 320], rax
                        mov              qword ptr [rbp + 328], rdx
                                                                                        jmp   n40_var_α
#-----------------------------------------------------------------------------------------------------------------------
n40_var_α:
                        mov              rax, qword ptr [rbp + 816]
                        mov              qword ptr [rbp + 448], rax
                        mov              rax, qword ptr [rbp + 824]
                        mov              qword ptr [rbp + 456], rax
                                                                                        jmp   n41_call_builtin_gen_α
#-----------------------------------------------------------------------------------------------------------------------
n41_call_builtin_gen_α:
                        mov              rax, qword ptr [rbp + 320]
                        mov              qword ptr [rbp + 272], rax
                        mov              rax, qword ptr [rbp + 328]
                        mov              qword ptr [rbp + 280], rax
                        mov              rax, qword ptr [rbp + 448]
                        mov              qword ptr [rbp + 288], rax
                        mov              rax, qword ptr [rbp + 456]
                        mov              qword ptr [rbp + 296], rax
                        mov              qword ptr [rbp + 304], 0
                        mov              rdi, r14
                        call             rt_scan_sync_out@PLT
.Lx105_60:
                        .section         .rodata
.Lbynamegenfn42:        .string          "find"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamegenfn42]
                        lea              rsi, [rbp + 272]
                        mov              edx, 2
                        lea              rcx, [rbp + 304]
                        call             rt_call_arr_gen@PLT
                        mov              qword ptr [rbp + 256], rax
                        mov              qword ptr [rbp + 264], rdx
                        cmp              eax, 99
                                                                                        je    n23_disjunction_af
                                                                                        jmp   n42_var_α
n41_call_builtin_gen_β:
                                                                                        jmp   .Lx105_60
#-----------------------------------------------------------------------------------------------------------------------
n42_var_α:
                        mov              rax, qword ptr [rbp + 848]
                        mov              qword ptr [rbp + 224], rax
                        mov              rax, qword ptr [rbp + 856]
                        mov              qword ptr [rbp + 232], rax
                                                                                        jmp   n43_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n43_lit_integer_α:
                        mov              qword ptr [rbp + 240], 6
                        mov              rax, qword ptr [rip + .Lx108_0]
                        mov              qword ptr [rbp + 248], rax
                                                                                        jmp   n44_op75_α
.Lx108_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n44_op75_α:
                        mov              eax, dword ptr [rbp + 848]
                        cmp              eax, 7
                                                                                        je    .Lx110_1
                        cmp              eax, 6
                                                                                        jne   .Lx110_0
                        mov              eax, dword ptr [rbp + 240]
                        cmp              eax, 6
                                                                                        jne   .Lx110_0
.Lx110_1:
                        mov              rax, qword ptr [rbp + 848]
                        mov              qword ptr [rbp + 208], rax
                        mov              rax, qword ptr [rbp + 856]
                        mov              qword ptr [rbp + 216], rax
                                                                                        jmp   n45_binop_α
.Lx110_0:
                        lea              rdi, [rbp + 848]
                        lea              rsi, [rbp + 240]
                        lea              rdx, [rbp + 208]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n45_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n45_binop_α:
                        mov              eax, dword ptr [rbp + 208]
                        cmp              eax, 6
                                                                                        jne   .Lx111_0
                        mov              rax, qword ptr [rbp + 216]
                        mov              rcx, 1
                        sub              rax, rcx
                        mov              qword ptr [rbp + 192], 6
                        mov              qword ptr [rbp + 200], rax
                                                                                        jmp   n26_assign_α
.Lx111_0:
                        mov              rdi, qword ptr [rbp + 208]
                        mov              rsi, qword ptr [rbp + 216]
                        mov              rdx, qword ptr [rbp + 240]
                        mov              rcx, qword ptr [rbp + 248]
                        call             rt_sub@PLT
                        cmp              eax, 99
                                                                                        je    n13_var_ref_α
                        mov              qword ptr [rbp + 192], rax
                        mov              qword ptr [rbp + 200], rdx
                                                                                        jmp   n26_assign_α
#-----------------------------------------------------------------------------------------------------------------------
proc_meander_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_meander_β:
                                                                                        jmp   proc_meander_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_meander_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 904]
                        lea              rsp, [rbp + 928]
                        mov              rbp, [rbp + 920]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_meander_ω:
                        mov              rax, [rbp + 912]
                        lea              rsp, [rbp + 928]
                        mov              rbp, [rbp + 920]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_meander_dcα:
                        pop              r11
                        sub              rsp, 944
                        mov              qword ptr [rsp + 920], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rbp + 896], r11
                        lea              rax, [rip + .Lx112_2]
                        mov              qword ptr [rbp + 904], rax
                        lea              rax, [rip + .Lx112_3]
                        mov              qword ptr [rbp + 912], rax
                        mov              qword ptr [rbp + 16], rsi
                        mov              qword ptr [rbp + 24], rdx
                        mov              rdi, rbp
                        mov              esi, 816
                        mov              edx, 896
                        mov              ecx, 2
                        mov              r8d, 2
                        mov              r9d, 0
                        call             rt_pl_dc_prep@PLT
                                                                                        jmp   proc_meander_α_body
.Lx112_2:
                        mov              rdx, qword ptr [rsp + 0]
                        mov              rcx, rsp
                        add              rcx, -928
                        mov              r11, qword ptr [rsp + -32]
                        mov              rbp, qword ptr [rsp + -8]
                        add              rsp, 16
                        push             r11
                                                                                        jmp   rt_pl_dc_leave_γ@PLT
.Lx112_3:
                        mov              rdi, qword ptr [rsp + 0]
                        mov              rsi, rsp
                        add              rsi, -928
                        mov              r11, qword ptr [rsp + -32]
                        mov              rbp, qword ptr [rsp + -8]
                        add              rsp, 16
                        push             r11
                                                                                        jmp   rt_pl_dc_leave_ω@PLT
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lstartup_pname0:       .string          "meander"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_meander_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 2
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 896
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_meander_dcα]
                        call             rt_proc_set_dcfn@PLT
                        add              rsp, 8
                        ret
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             proc_startup
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
                        sub              rsp, 696
                        mov              rdi, rsp
                        mov              ecx, 696
                        xor              eax, eax
                        rep stosb
                        mov              [rsp + 688], rbp
                        mov              rbp, rsp
main_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n113_call_builtin_icon_α:
                        .section         .rodata
.Lrkfn138:              .string          "read"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn138]
                        lea              rsi, [rbp + 32]
                        mov              edx, 0
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 16], rax
                        mov              qword ptr [rbp + 24], rdx
                        cmp              eax, 99
                                                                                        je    main_ω
                                                                                        jmp   n114_assign_α
n113_call_builtin_icon_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n114_assign_α:
                        mov              rax, qword ptr [rbp + 16]
                        mov              rdx, qword ptr [rbp + 24]
                        mov              qword ptr [rbp + 640], rax
                        mov              qword ptr [rbp + 648], rdx
                                                                                        jmp   n115_var_α
#-----------------------------------------------------------------------------------------------------------------------
n115_var_α:
                        mov              rax, qword ptr [rbp + 640]
                        mov              qword ptr [rbp + 624], rax
                        mov              rax, qword ptr [rbp + 648]
                        mov              qword ptr [rbp + 632], rax
                                                                                        jmp   n116_scan_enter_α
#-----------------------------------------------------------------------------------------------------------------------
n116_scan_enter_α:
                        mov              rdi, qword ptr [rbp + 624]
                        mov              rsi, qword ptr [rbp + 632]
                        mov              rdx, r13
                        mov              rcx, r14
                        mov              r8, r15
                        call             rt_scan_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r14, 0
                                                                                        jmp   n117_disjunction_α
#-----------------------------------------------------------------------------------------------------------------------
n117_disjunction_α:
                        mov              qword ptr [rbp + 128], 0
                        mov              qword ptr [rbp + 136], 0
                        mov              dword ptr [rbp + 144], 0
                                                                                        jmp   n119_lit_charset_α
n117_disjunction_as:
                        mov              eax, dword ptr [rbp + 144]
                        cmp              eax, 0
                                                                                        jne   .Lx145_0
                        mov              rax, qword ptr [rbp + 160]
                        mov              qword ptr [rbp + 128], rax
                        mov              rax, qword ptr [rbp + 168]
                        mov              qword ptr [rbp + 136], rax
                                                                                        jmp   n118_scan_α
.Lx145_0:
                        cmp              eax, 1
                                                                                        jne   .Lx145_1
                        mov              rax, qword ptr [rbp + 560]
                        mov              qword ptr [rbp + 128], rax
                        mov              rax, qword ptr [rbp + 568]
                        mov              qword ptr [rbp + 136], rax
                                                                                        jmp   n118_scan_α
.Lx145_1:
                                                                                        jmp   n118_scan_α
n117_disjunction_β:
                        mov              eax, dword ptr [rbp + 144]
                        cmp              eax, 0
                                                                                        je    n124_scan_α
                                                                                        jmp   n124_scan_α
n117_disjunction_af:
                        add              dword ptr [rbp + 144], 1
                        mov              eax, dword ptr [rbp + 144]
                        cmp              eax, 1
                                                                                        je    n120_lit_string_α
                                                                                        jmp   n124_scan_α
#-----------------------------------------------------------------------------------------------------------------------
n118_scan_α:
                        mov              rax, qword ptr [rbp + 128]
                        mov              qword ptr [rbp + 96], rax
                        mov              rax, qword ptr [rbp + 136]
                        mov              qword ptr [rbp + 104], rax
                        lea              rdi, [rbp + 64]
                        call             rt_scan_leave@PLT
                        mov              r13, qword ptr [rbp + 64]
                        mov              r14, qword ptr [rbp + 72]
                        mov              r15, qword ptr [rbp + 80]
                                                                                        jmp   n113_call_builtin_icon_α
n118_scan_β:
                        call             rt_scan_reenter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        call             rt_scan_sync_in@PLT
                        mov              r14, rax
                                                                                        jmp   n117_disjunction_β
                                                                                        jmp   n113_call_builtin_icon_α
#-----------------------------------------------------------------------------------------------------------------------
n119_lit_charset_α:
                        mov              qword ptr [rbp + 544], 1
                        mov              dword ptr [rbp + 548], -1
                        mov              rax, qword ptr [rip + .Lx148_0]
                        mov              qword ptr [rbp + 552], rax
                                                                                        jmp   n123_scan_upto_α
n119_lit_charset_β:
                                                                                        jmp   n117_disjunction_af
.Lx148_0:
                        .quad            .Lx148_0_s
.Lx148_0_s:
                        .string          ":"
#-----------------------------------------------------------------------------------------------------------------------
n120_lit_string_α:
                        mov              qword ptr [rbp + 608], 1
                        mov              rax, qword ptr [rip + .Lx149_0]
                        mov              qword ptr [rbp + 616], rax
                                                                                        jmp   n122_call_builtin_icon_α
n120_lit_string_β:
                                                                                        jmp   n124_scan_α
.Lx149_0:
                        .quad            .Lx149_0_s
.Lx149_0_s:
                        .string          "*** erroneous input ***"
#-----------------------------------------------------------------------------------------------------------------------
n121_call_builtin_icon_α:
                        mov              rax, qword ptr [rbp + 208]
                        mov              qword ptr [rbp + 176], rax
                        mov              rax, qword ptr [rbp + 216]
                        mov              qword ptr [rbp + 184], rax
                        .section         .rodata
.Lrkfn151:              .string          "write"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn151]
                        lea              rsi, [rbp + 176]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 160], rax
                        mov              qword ptr [rbp + 168], rdx
                        cmp              eax, 99
                                                                                        je    n124_scan_α
                                                                                        jmp   n117_disjunction_as
n121_call_builtin_icon_β:
                                                                                        jmp   n124_scan_α
#-----------------------------------------------------------------------------------------------------------------------
n122_call_builtin_icon_α:
                        mov              rax, qword ptr [rbp + 608]
                        mov              qword ptr [rbp + 576], rax
                        mov              rax, qword ptr [rbp + 616]
                        mov              qword ptr [rbp + 584], rax
                        .section         .rodata
.Lrkfn153:              .string          "stop"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn153]
                        lea              rsi, [rbp + 576]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 560], rax
                        mov              qword ptr [rbp + 568], rdx
                        cmp              eax, 99
                                                                                        je    n124_scan_α
                                                                                        jmp   n117_disjunction_as
n122_call_builtin_icon_β:
                                                                                        jmp   n124_scan_α
#-----------------------------------------------------------------------------------------------------------------------
n123_scan_upto_α:
                        mov              qword ptr [rbp + 528], r14
.Lx155_0:
                        mov              rax, qword ptr [rbp + 528]
                        cmp              rax, r15
                                                                                        jge   n117_disjunction_af
                        mov              rcx, rax
                        movzx            esi, byte ptr [r13+rcx]
                        mov              rdi, qword ptr [rip + .Lx155_2]
                        push             rax
                        push             r10
                        call             strchr@PLT
                        pop              r10
                        test             rax, rax
                        pop              rax
                                                                                        je    .Lx155_1
                        mov              qword ptr [rbp + 512], 6
                        add              rax, 1
                        mov              qword ptr [rbp + 520], rax
                                                                                        jmp   n125_scan_tab_α
.Lx155_1:
                        inc              qword ptr [rbp + 528]
                                                                                        jmp   .Lx155_0
n123_scan_upto_β:
                        inc              qword ptr [rbp + 528]
                                                                                        jmp   .Lx155_0
.Lx155_2:
                        .quad            .Lx155_2_s
.Lx155_2_s:
                        .string          ":"
#-----------------------------------------------------------------------------------------------------------------------
n124_scan_α:
                        lea              rdi, [rbp + 64]
                        call             rt_scan_leave@PLT
                        mov              r13, qword ptr [rbp + 64]
                        mov              r14, qword ptr [rbp + 72]
                        mov              r15, qword ptr [rbp + 80]
                                                                                        jmp   n113_call_builtin_icon_α
n124_scan_β:
                                                                                        jmp   n113_call_builtin_icon_α
#-----------------------------------------------------------------------------------------------------------------------
n125_scan_tab_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [rbp + 520]
                        cmp              rax, 1
                                                                                        jge   .Lx159_0
                        add              rax, r15
                        add              rax, 1
.Lx159_0:
                        cmp              rax, 1
                                                                                        jge   .Lx159_239
                        add              rsp, 16
                                                                                        jmp   n123_scan_upto_β
.Lx159_239:
                        mov              rcx, r15
                        add              rcx, 1
                        cmp              rax, rcx
                                                                                        jle   .Lx159_240
                        add              rsp, 16
                                                                                        jmp   n123_scan_upto_β
.Lx159_240:
                        mov              qword ptr [rsp + 0], r14
                        mov              rdi, r13
                        mov              rsi, r14
                        mov              rdx, rax
                        sub              rdx, 1
                        mov              r14, rdx
                        push             r10
                        push             r10
                        call             rt_substr@PLT
                        pop              r10
                        pop              r10
                        mov              qword ptr [rbp + 480], rax
                        mov              qword ptr [rbp + 488], rdx
                                                                                        jmp   n126_assign_α
n125_scan_tab_β:
                        mov              r14, qword ptr [rsp + 0]
                        add              rsp, 16
                                                                                        jmp   n123_scan_upto_β
#-----------------------------------------------------------------------------------------------------------------------
n126_assign_α:
                        mov              rax, qword ptr [rbp + 480]
                        mov              rdx, qword ptr [rbp + 488]
                        mov              qword ptr [rbp + 656], rax
                        mov              qword ptr [rbp + 664], rdx
                                                                                        jmp   n127_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n127_lit_integer_α:
                        mov              qword ptr [rbp + 464], 6
                        mov              rax, qword ptr [rip + .Lx161_0]
                        mov              qword ptr [rbp + 472], rax
                                                                                        jmp   n128_scan_move_α
.Lx161_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n128_scan_move_α:
                        sub              rsp, 16
                        mov              rax, 1
                        add              rax, r14
                        add              rax, 1
                        cmp              rax, 1
                                                                                        jge   .Lx163_239
                        add              rsp, 16
                                                                                        jmp   n117_disjunction_af
.Lx163_239:
                        mov              rcx, r15
                        add              rcx, 1
                        cmp              rax, rcx
                                                                                        jle   .Lx163_240
                        add              rsp, 16
                                                                                        jmp   n117_disjunction_af
.Lx163_240:
                        mov              qword ptr [rsp + 0], r14
                        mov              rdi, r13
                        mov              rsi, r14
                        mov              rdx, rax
                        sub              rdx, 1
                        mov              r14, rdx
                        push             r10
                        push             r10
                        call             rt_substr@PLT
                        pop              r10
                        pop              r10
                        mov              qword ptr [rbp + 432], rax
                        mov              qword ptr [rbp + 440], rdx
                                                                                        jmp   n129_lit_integer_α
n128_scan_move_β:
                        mov              r14, qword ptr [rsp + 0]
                        add              rsp, 16
                                                                                        jmp   n117_disjunction_af
#-----------------------------------------------------------------------------------------------------------------------
n129_lit_integer_α:
                        mov              qword ptr [rbp + 416], 6
                        mov              rax, qword ptr [rip + .Lx164_0]
                        mov              qword ptr [rbp + 424], rax
                                                                                        jmp   n130_scan_tab_α
.Lx164_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n130_scan_tab_α:
                        sub              rsp, 16
                        mov              rax, 0
                        cmp              rax, 1
                                                                                        jge   .Lx166_0
                        add              rax, r15
                        add              rax, 1
.Lx166_0:
                        cmp              rax, 1
                                                                                        jge   .Lx166_239
                        add              rsp, 16
                                                                                        jmp   n117_disjunction_af
.Lx166_239:
                        mov              rcx, r15
                        add              rcx, 1
                        cmp              rax, rcx
                                                                                        jle   .Lx166_240
                        add              rsp, 16
                                                                                        jmp   n117_disjunction_af
.Lx166_240:
                        mov              qword ptr [rsp + 0], r14
                        mov              rdi, r13
                        mov              rsi, r14
                        mov              rdx, rax
                        sub              rdx, 1
                        mov              r14, rdx
                        push             r10
                        push             r10
                        call             rt_substr@PLT
                        pop              r10
                        pop              r10
                        mov              qword ptr [rbp + 384], rax
                        mov              qword ptr [rbp + 392], rdx
                                                                                        jmp   n131_call_builtin_icon_α
n130_scan_tab_β:
                        mov              r14, qword ptr [rsp + 0]
                        add              rsp, 16
                                                                                        jmp   n117_disjunction_af
#-----------------------------------------------------------------------------------------------------------------------
n131_call_builtin_icon_α:
                        mov              rax, qword ptr [rbp + 384]
                        mov              qword ptr [rbp + 352], rax
                        mov              rax, qword ptr [rbp + 392]
                        mov              qword ptr [rbp + 360], rax
                        .section         .rodata
.Lrkfn168:              .string          "integer"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn168]
                        lea              rsi, [rbp + 352]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 336], rax
                        mov              qword ptr [rbp + 344], rdx
                        cmp              eax, 99
                                                                                        je    n117_disjunction_af
                                                                                        jmp   n132_assign_α
n131_call_builtin_icon_β:
                                                                                        jmp   n117_disjunction_af
#-----------------------------------------------------------------------------------------------------------------------
n132_assign_α:
                        mov              rax, qword ptr [rbp + 336]
                        mov              rdx, qword ptr [rbp + 344]
                        mov              qword ptr [rbp + 672], rax
                        mov              qword ptr [rbp + 680], rdx
                        mov              qword ptr [rbp + 320], rax
                        mov              qword ptr [rbp + 328], rdx
                                                                                        jmp   n133_conjunction_α
#-----------------------------------------------------------------------------------------------------------------------
n133_conjunction_α:
                        mov              rax, qword ptr [rbp + 320]
                        mov              qword ptr [rbp + 304], rax
                        mov              rax, qword ptr [rbp + 328]
                        mov              qword ptr [rbp + 312], rax
                                                                                        jmp   n134_var_α
n133_conjunction_β:
                                                                                        jmp   n117_disjunction_af
#-----------------------------------------------------------------------------------------------------------------------
n134_var_α:
                        mov              rax, qword ptr [rbp + 656]
                        mov              qword ptr [rbp + 272], rax
                        mov              rax, qword ptr [rbp + 664]
                        mov              qword ptr [rbp + 280], rax
                                                                                        jmp   n135_var_α
#-----------------------------------------------------------------------------------------------------------------------
n135_var_α:
                        mov              rax, qword ptr [rbp + 672]
                        mov              qword ptr [rbp + 288], rax
                        mov              rax, qword ptr [rbp + 680]
                        mov              qword ptr [rbp + 296], rax
                                                                                        jmp   n136_call_proc_staged_α
#-----------------------------------------------------------------------------------------------------------------------
n136_call_proc_staged_α:
                        mov              rdi, r14
                        call             rt_scan_sync_out@PLT
                        lea              rsi, [rbp + 272]
                        lea              rdx, [rbp + 288]
                        call             proc_meander_dcα
                                                                                        jmp   .Lx176_2
.Lx176_2:
                        push             rax
                        push             rdx
                        call             rt_scan_sync_in@PLT
                        mov              r14, rax
                        pop              rdx
                        pop              rax
                        mov              qword ptr [rbp + 208], rax
                        mov              qword ptr [rbp + 216], rdx
                        cmp              eax, 99
                                                                                        je    n124_scan_α
                                                                                        jmp   n121_call_builtin_icon_α
n136_call_proc_staged_β:
                                                                                        jmp   n124_scan_α
.Lx176_0:
                        .quad            .Lx176_0_s
.Lx176_0_s:
                        .string          "meander"
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        mov              rsp, rbp
                        mov              rbp, [rsp + 688]
                        add              rsp, 696
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              rsp, rbp
                        mov              eax, 99
                        xor              edx, edx
                        mov              rbp, [rsp + 688]
                        add              rsp, 696
                        ret
                        .section         .note.GNU-stack,"",@progbits

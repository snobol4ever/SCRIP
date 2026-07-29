                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_safe_α
proc_safe_α:
                        .global          proc_safe_α
                        .global          proc_safe_β
                        .global          proc_safe_γ
                        .global          proc_safe_ω
                        sub              rsp, 704
                        mov              [rsp + 680], rcx
                        mov              [rsp + 688], rdx
                        mov              [rsp + 696], rbp
                        mov              rbp, rsp
                        mov              rdi, rsp
                        mov              esi, 672
                        mov              edx, 672
                        call             rt_jmp_frame_lexprep2@PLT
proc_safe_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n0_disjunction_α:
                        mov              qword ptr [rbp + 544], 0
                        mov              qword ptr [rbp + 552], 0
                        mov              dword ptr [rbp + 560], 0
                                                                                        jmp   n2_var_ref_α
n0_disjunction_as:
                        mov              eax, dword ptr [rbp + 560]
                        cmp              eax, 0
                                                                                        jne   .Lx38_0
                                                                                        jmp   n1_disjunction_α
.Lx38_0:
                                                                                        jmp   n1_disjunction_α
n0_disjunction_β:
                        mov              eax, dword ptr [rbp + 560]
                                                                                        jmp   n1_disjunction_α
n0_disjunction_af:
                        add              dword ptr [rbp + 560], 1
                        mov              eax, dword ptr [rbp + 560]
                                                                                        jmp   n1_disjunction_α
#-----------------------------------------------------------------------------------------------------------------------
n1_disjunction_α:
                        mov              qword ptr [rbp + 288], 0
                        mov              qword ptr [rbp + 296], 0
                        mov              dword ptr [rbp + 304], 0
                                                                                        jmp   n4_var_ref_α
n1_disjunction_as:
                        mov              eax, dword ptr [rbp + 304]
                        cmp              eax, 0
                                                                                        jne   .Lx40_0
                                                                                        jmp   n3_disjunction_α
.Lx40_0:
                                                                                        jmp   n3_disjunction_α
n1_disjunction_β:
                        mov              eax, dword ptr [rbp + 304]
                                                                                        jmp   n3_disjunction_α
n1_disjunction_af:
                        add              dword ptr [rbp + 304], 1
                        mov              eax, dword ptr [rbp + 304]
                                                                                        jmp   n3_disjunction_α
#-----------------------------------------------------------------------------------------------------------------------
n2_var_ref_α:
                        mov              rax, 4294967305
                        mov              rdx, 1879052320
                        mov              qword ptr [rbp + 592], rax
                        mov              qword ptr [rbp + 600], rdx
                                                                                        jmp   n5_var_α
n2_var_ref_β:
                                                                                        jmp   n0_disjunction_af
#-----------------------------------------------------------------------------------------------------------------------
n3_disjunction_α:
                        mov              qword ptr [rbp + 48], 0
                        mov              qword ptr [rbp + 56], 0
                        mov              dword ptr [rbp + 64], 0
                                                                                        jmp   n7_var_ref_α
n3_disjunction_as:
                        mov              eax, dword ptr [rbp + 64]
                        cmp              eax, 0
                                                                                        jne   .Lx44_0
                                                                                        jmp   n6_return_α
.Lx44_0:
                                                                                        jmp   n6_return_α
n3_disjunction_β:
                        mov              eax, dword ptr [rbp + 64]
                                                                                        jmp   n6_return_α
n3_disjunction_af:
                        add              dword ptr [rbp + 64], 1
                        mov              eax, dword ptr [rbp + 64]
                                                                                        jmp   n6_return_α
#-----------------------------------------------------------------------------------------------------------------------
n4_var_ref_α:
                        mov              rax, 4294967305
                        mov              rdx, 1879052336
                        mov              qword ptr [rbp + 336], rax
                        mov              qword ptr [rbp + 344], rdx
                                                                                        jmp   n8_var_α
n4_var_ref_β:
                                                                                        jmp   n1_disjunction_af
#-----------------------------------------------------------------------------------------------------------------------
n5_var_α:
                        mov              rax, qword ptr [rbp + 32]
                        mov              qword ptr [rbp + 608], rax
                        mov              rax, qword ptr [rbp + 40]
                        mov              qword ptr [rbp + 616], rax
                                                                                        jmp   n9_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n6_return_α:
                        mov              qword ptr [rbp + 0], 0
                        mov              qword ptr [rbp + 8], 0
                                                                                        jmp   proc_safe_γ
#-----------------------------------------------------------------------------------------------------------------------
n7_var_ref_α:
                        mov              rax, 4294967305
                        mov              rdx, 1879052352
                        mov              qword ptr [rbp + 96], rax
                        mov              qword ptr [rbp + 104], rdx
                                                                                        jmp   n10_var_α
n7_var_ref_β:
                                                                                        jmp   n3_disjunction_af
#-----------------------------------------------------------------------------------------------------------------------
n8_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rbp + 448], rax
                        mov              qword ptr [rbp + 456], rdx
                                                                                        jmp   n11_var_α
#-----------------------------------------------------------------------------------------------------------------------
n9_subscript_α:
                        mov              rdi, qword ptr [rbp + 592]
                        mov              rsi, qword ptr [rbp + 600]
                        mov              rdx, qword ptr [rbp + 608]
                        mov              rcx, qword ptr [rbp + 616]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n0_disjunction_af
                        mov              qword ptr [rbp + 624], rax
                        mov              qword ptr [rbp + 632], rdx
                                                                                        jmp   n12_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n10_var_α:
                        mov              rax, qword ptr [rbp + 32]
                        mov              qword ptr [rbp + 192], rax
                        mov              rax, qword ptr [rbp + 40]
                        mov              qword ptr [rbp + 200], rax
                                                                                        jmp   n13_var_α
#-----------------------------------------------------------------------------------------------------------------------
n11_var_α:
                        mov              rax, qword ptr [rbp + 32]
                        mov              qword ptr [rbp + 464], rax
                        mov              rax, qword ptr [rbp + 40]
                        mov              qword ptr [rbp + 472], rax
                                                                                        jmp   n14_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n12_deref_α:
                        mov              rdi, qword ptr [rbp + 624]
                        mov              rsi, qword ptr [rbp + 632]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n0_disjunction_af
                        mov              qword ptr [rbp + 640], rax
                        mov              qword ptr [rbp + 648], rdx
                                                                                        jmp   n15_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n13_var_α:
                        mov              rax, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + 208], rax
                        mov              rax, qword ptr [rbp + 24]
                        mov              qword ptr [rbp + 216], rax
                                                                                        jmp   n16_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n14_op75_α:
                        mov              eax, dword ptr [rbp + 448]
                        cmp              eax, 7
                                                                                        je    .Lx62_1
                        cmp              eax, 6
                                                                                        jne   .Lx62_0
                        mov              eax, dword ptr [rbp + 32]
                        cmp              eax, 6
                                                                                        jne   .Lx62_0
.Lx62_1:
                        mov              rax, qword ptr [rbp + 448]
                        mov              qword ptr [rbp + 432], rax
                        mov              rax, qword ptr [rbp + 456]
                        mov              qword ptr [rbp + 440], rax
                                                                                        jmp   n17_op75_α
.Lx62_0:
                        lea              rdi, [rbp + 448]
                        lea              rsi, [rbp + 32]
                        lea              rdx, [rbp + 432]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n17_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n15_lit_integer_α:
                        mov              qword ptr [rbp + 656], 6
                        mov              rax, qword ptr [rip + .Lx63_0]
                        mov              qword ptr [rbp + 664], rax
                                                                                        jmp   n18_binop_test_α
.Lx63_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n16_op75_α:
                        mov              eax, dword ptr [rbp + 32]
                        cmp              eax, 7
                                                                                        je    .Lx65_1
                        cmp              eax, 6
                                                                                        jne   .Lx65_0
                        mov              eax, dword ptr [rbp + 16]
                        cmp              eax, 6
                                                                                        jne   .Lx65_0
.Lx65_1:
                        mov              rax, qword ptr [rbp + 32]
                        mov              qword ptr [rbp + 176], rax
                        mov              rax, qword ptr [rbp + 40]
                        mov              qword ptr [rbp + 184], rax
                                                                                        jmp   n19_op75_α
.Lx65_0:
                        lea              rdi, [rbp + 32]
                        lea              rsi, [rbp + 16]
                        lea              rdx, [rbp + 176]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n19_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n17_op75_α:
                        mov              eax, dword ptr [rbp + 32]
                        cmp              eax, 7
                                                                                        je    .Lx67_1
                        cmp              eax, 6
                                                                                        jne   .Lx67_0
                        mov              eax, dword ptr [rbp + 448]
                        cmp              eax, 6
                                                                                        jne   .Lx67_0
.Lx67_1:
                        mov              rax, qword ptr [rbp + 32]
                        mov              qword ptr [rbp + 416], rax
                        mov              rax, qword ptr [rbp + 40]
                        mov              qword ptr [rbp + 424], rax
                                                                                        jmp   n20_binop_α
.Lx67_0:
                        lea              rdi, [rbp + 32]
                        lea              rsi, [rbp + 448]
                        lea              rdx, [rbp + 416]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n20_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n18_binop_test_α:
                        mov              eax, dword ptr [rbp + 640]
                        cmp              eax, 100
                                                                                        je    .Lx68_0
                        mov              eax, dword ptr [rbp + 656]
                        cmp              eax, 100
                                                                                        je    .Lx68_0
                        mov              eax, dword ptr [rbp + 640]
                        cmp              eax, 6
                                                                                        jne   .Lx68_2
                        mov              eax, dword ptr [rbp + 656]
                        cmp              eax, 6
                                                                                        jne   .Lx68_2
.Lx68_1:
                        mov              rax, qword ptr [rbp + 648]
                        mov              rcx, qword ptr [rbp + 664]
                        cmp              rax, rcx
                                                                                        je    n0_disjunction_af
                        mov              rcx, qword ptr [rbp + 656]
                        mov              qword ptr [rbp + 576], rcx
                        mov              rcx, qword ptr [rbp + 664]
                        mov              qword ptr [rbp + 584], rcx
                                                                                        jmp   proc_safe_ω
.Lx68_0:
                        mov              rdi, qword ptr [rbp + 640]
                        mov              rsi, qword ptr [rbp + 648]
                        mov              rdx, qword ptr [rbp + 656]
                        mov              rcx, qword ptr [rbp + 664]
                        mov              r8d, 10
                        lea              r9, [rbp + 576]
                        call             rt_relop_overload@PLT
                        test             eax, eax
                                                                                        je    .Lx68_1
                        cmp              eax, 1
                                                                                        je    n0_disjunction_af
                                                                                        jmp   proc_safe_ω
.Lx68_2:
                        mov              rdi, qword ptr [rbp + 640]
                        mov              rsi, qword ptr [rbp + 648]
                        mov              rdx, qword ptr [rbp + 656]
                        mov              rcx, qword ptr [rbp + 664]
                        mov              r8d, 10
                        call             rt_jct_relop@PLT
                        test             eax, eax
                                                                                        jz    n0_disjunction_af
                        mov              rax, qword ptr [rbp + 656]
                        mov              qword ptr [rbp + 576], rax
                        mov              rax, qword ptr [rbp + 664]
                        mov              qword ptr [rbp + 584], rax
                                                                                        jmp   proc_safe_ω
#-----------------------------------------------------------------------------------------------------------------------
n19_op75_α:
                        mov              eax, dword ptr [rbp + 16]
                        cmp              eax, 7
                                                                                        je    .Lx70_1
                        cmp              eax, 6
                                                                                        jne   .Lx70_0
                        mov              eax, dword ptr [rbp + 32]
                        cmp              eax, 6
                                                                                        jne   .Lx70_0
.Lx70_1:
                        mov              rax, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + 160], rax
                        mov              rax, qword ptr [rbp + 24]
                        mov              qword ptr [rbp + 168], rax
                                                                                        jmp   n21_binop_α
.Lx70_0:
                        lea              rdi, [rbp + 16]
                        lea              rsi, [rbp + 32]
                        lea              rdx, [rbp + 160]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n21_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n20_binop_α:
                        mov              eax, dword ptr [rbp + 432]
                        cmp              eax, 6
                                                                                        jne   .Lx71_0
                        mov              eax, dword ptr [rbp + 416]
                        cmp              eax, 6
                                                                                        jne   .Lx71_0
                        mov              rax, qword ptr [rbp + 440]
                        mov              rcx, qword ptr [rbp + 424]
                        add              rax, rcx
                        mov              qword ptr [rbp + 400], 6
                        mov              qword ptr [rbp + 408], rax
                                                                                        jmp   n22_var_α
.Lx71_0:
                        mov              rdi, qword ptr [rbp + 432]
                        mov              rsi, qword ptr [rbp + 440]
                        mov              rdx, qword ptr [rbp + 416]
                        mov              rcx, qword ptr [rbp + 424]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        je    n1_disjunction_af
                        mov              qword ptr [rbp + 400], rax
                        mov              qword ptr [rbp + 408], rdx
                                                                                        jmp   n22_var_α
#-----------------------------------------------------------------------------------------------------------------------
n21_binop_α:
                        mov              eax, dword ptr [rbp + 176]
                        cmp              eax, 6
                                                                                        jne   .Lx72_0
                        mov              eax, dword ptr [rbp + 160]
                        cmp              eax, 6
                                                                                        jne   .Lx72_0
                        mov              rax, qword ptr [rbp + 184]
                        mov              rcx, qword ptr [rbp + 168]
                        add              rax, rcx
                        mov              qword ptr [rbp + 144], 6
                        mov              qword ptr [rbp + 152], rax
                                                                                        jmp   n23_lit_integer_α
.Lx72_0:
                        mov              rdi, qword ptr [rbp + 176]
                        mov              rsi, qword ptr [rbp + 184]
                        mov              rdx, qword ptr [rbp + 160]
                        mov              rcx, qword ptr [rbp + 168]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        je    n3_disjunction_af
                        mov              qword ptr [rbp + 144], rax
                        mov              qword ptr [rbp + 152], rdx
                                                                                        jmp   n23_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n22_var_α:
                        mov              rax, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + 480], rax
                        mov              rax, qword ptr [rbp + 24]
                        mov              qword ptr [rbp + 488], rax
                                                                                        jmp   n24_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n23_lit_integer_α:
                        mov              qword ptr [rbp + 224], 6
                        mov              rax, qword ptr [rip + .Lx75_0]
                        mov              qword ptr [rbp + 232], rax
                                                                                        jmp   n25_op75_α
.Lx75_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n24_op75_α:
                        mov              eax, dword ptr [rbp + 400]
                        cmp              eax, 7
                                                                                        je    .Lx77_1
                        cmp              eax, 6
                                                                                        jne   .Lx77_0
                        mov              eax, dword ptr [rbp + 16]
                        cmp              eax, 6
                                                                                        jne   .Lx77_0
.Lx77_1:
                        mov              rax, qword ptr [rbp + 400]
                        mov              qword ptr [rbp + 384], rax
                        mov              rax, qword ptr [rbp + 408]
                        mov              qword ptr [rbp + 392], rax
                                                                                        jmp   n26_op75_α
.Lx77_0:
                        lea              rdi, [rbp + 400]
                        lea              rsi, [rbp + 16]
                        lea              rdx, [rbp + 384]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n26_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n25_op75_α:
                        mov              eax, dword ptr [rbp + 144]
                        cmp              eax, 7
                                                                                        je    .Lx79_1
                        cmp              eax, 6
                                                                                        jne   .Lx79_0
                        mov              eax, dword ptr [rbp + 224]
                        cmp              eax, 6
                                                                                        jne   .Lx79_0
.Lx79_1:
                        mov              rax, qword ptr [rbp + 144]
                        mov              qword ptr [rbp + 128], rax
                        mov              rax, qword ptr [rbp + 152]
                        mov              qword ptr [rbp + 136], rax
                                                                                        jmp   n27_binop_α
.Lx79_0:
                        lea              rdi, [rbp + 144]
                        lea              rsi, [rbp + 224]
                        lea              rdx, [rbp + 128]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n27_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n26_op75_α:
                        mov              eax, dword ptr [rbp + 16]
                        cmp              eax, 7
                                                                                        je    .Lx81_1
                        cmp              eax, 6
                                                                                        jne   .Lx81_0
                        mov              eax, dword ptr [rbp + 400]
                        cmp              eax, 6
                                                                                        jne   .Lx81_0
.Lx81_1:
                        mov              rax, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + 368], rax
                        mov              rax, qword ptr [rbp + 24]
                        mov              qword ptr [rbp + 376], rax
                                                                                        jmp   n28_binop_α
.Lx81_0:
                        lea              rdi, [rbp + 16]
                        lea              rsi, [rbp + 400]
                        lea              rdx, [rbp + 368]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n28_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n27_binop_α:
                        mov              eax, dword ptr [rbp + 128]
                        cmp              eax, 6
                                                                                        jne   .Lx82_0
                        mov              rax, qword ptr [rbp + 136]
                        mov              rcx, 1
                        sub              rax, rcx
                        mov              qword ptr [rbp + 112], 6
                        mov              qword ptr [rbp + 120], rax
                                                                                        jmp   n29_subscript_α
.Lx82_0:
                        mov              rdi, qword ptr [rbp + 128]
                        mov              rsi, qword ptr [rbp + 136]
                        mov              rdx, qword ptr [rbp + 224]
                        mov              rcx, qword ptr [rbp + 232]
                        call             rt_sub@PLT
                        cmp              eax, 99
                                                                                        je    n3_disjunction_af
                        mov              qword ptr [rbp + 112], rax
                        mov              qword ptr [rbp + 120], rdx
                                                                                        jmp   n29_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n28_binop_α:
                        mov              eax, dword ptr [rbp + 384]
                        cmp              eax, 6
                                                                                        jne   .Lx83_0
                        mov              eax, dword ptr [rbp + 368]
                        cmp              eax, 6
                                                                                        jne   .Lx83_0
                        mov              rax, qword ptr [rbp + 392]
                        mov              rcx, qword ptr [rbp + 376]
                        sub              rax, rcx
                        mov              qword ptr [rbp + 352], 6
                        mov              qword ptr [rbp + 360], rax
                                                                                        jmp   n30_subscript_α
.Lx83_0:
                        mov              rdi, qword ptr [rbp + 384]
                        mov              rsi, qword ptr [rbp + 392]
                        mov              rdx, qword ptr [rbp + 368]
                        mov              rcx, qword ptr [rbp + 376]
                        call             rt_sub@PLT
                        cmp              eax, 99
                                                                                        je    n1_disjunction_af
                        mov              qword ptr [rbp + 352], rax
                        mov              qword ptr [rbp + 360], rdx
                                                                                        jmp   n30_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n29_subscript_α:
                        mov              rdi, qword ptr [rbp + 96]
                        mov              rsi, qword ptr [rbp + 104]
                        mov              rdx, qword ptr [rbp + 112]
                        mov              rcx, qword ptr [rbp + 120]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n3_disjunction_af
                        mov              qword ptr [rbp + 240], rax
                        mov              qword ptr [rbp + 248], rdx
                                                                                        jmp   n31_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n30_subscript_α:
                        mov              rdi, qword ptr [rbp + 336]
                        mov              rsi, qword ptr [rbp + 344]
                        mov              rdx, qword ptr [rbp + 352]
                        mov              rcx, qword ptr [rbp + 360]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n1_disjunction_af
                        mov              qword ptr [rbp + 496], rax
                        mov              qword ptr [rbp + 504], rdx
                                                                                        jmp   n32_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n31_deref_α:
                        mov              rdi, qword ptr [rbp + 240]
                        mov              rsi, qword ptr [rbp + 248]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n3_disjunction_af
                        mov              qword ptr [rbp + 256], rax
                        mov              qword ptr [rbp + 264], rdx
                                                                                        jmp   n33_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n32_deref_α:
                        mov              rdi, qword ptr [rbp + 496]
                        mov              rsi, qword ptr [rbp + 504]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n1_disjunction_af
                        mov              qword ptr [rbp + 512], rax
                        mov              qword ptr [rbp + 520], rdx
                                                                                        jmp   n34_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n33_lit_integer_α:
                        mov              qword ptr [rbp + 272], 6
                        mov              rax, qword ptr [rip + .Lx88_0]
                        mov              qword ptr [rbp + 280], rax
                                                                                        jmp   n35_binop_test_α
.Lx88_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n34_lit_integer_α:
                        mov              qword ptr [rbp + 528], 6
                        mov              rax, qword ptr [rip + .Lx89_0]
                        mov              qword ptr [rbp + 536], rax
                                                                                        jmp   n36_binop_test_α
.Lx89_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n35_binop_test_α:
                        mov              eax, dword ptr [rbp + 256]
                        cmp              eax, 100
                                                                                        je    .Lx90_0
                        mov              eax, dword ptr [rbp + 272]
                        cmp              eax, 100
                                                                                        je    .Lx90_0
                        mov              eax, dword ptr [rbp + 256]
                        cmp              eax, 6
                                                                                        jne   .Lx90_2
                        mov              eax, dword ptr [rbp + 272]
                        cmp              eax, 6
                                                                                        jne   .Lx90_2
.Lx90_1:
                        mov              rax, qword ptr [rbp + 264]
                        mov              rcx, qword ptr [rbp + 280]
                        cmp              rax, rcx
                                                                                        je    n3_disjunction_af
                        mov              rcx, qword ptr [rbp + 272]
                        mov              qword ptr [rbp + 80], rcx
                        mov              rcx, qword ptr [rbp + 280]
                        mov              qword ptr [rbp + 88], rcx
                                                                                        jmp   proc_safe_ω
.Lx90_0:
                        mov              rdi, qword ptr [rbp + 256]
                        mov              rsi, qword ptr [rbp + 264]
                        mov              rdx, qword ptr [rbp + 272]
                        mov              rcx, qword ptr [rbp + 280]
                        mov              r8d, 10
                        lea              r9, [rbp + 80]
                        call             rt_relop_overload@PLT
                        test             eax, eax
                                                                                        je    .Lx90_1
                        cmp              eax, 1
                                                                                        je    n3_disjunction_af
                                                                                        jmp   proc_safe_ω
.Lx90_2:
                        mov              rdi, qword ptr [rbp + 256]
                        mov              rsi, qword ptr [rbp + 264]
                        mov              rdx, qword ptr [rbp + 272]
                        mov              rcx, qword ptr [rbp + 280]
                        mov              r8d, 10
                        call             rt_jct_relop@PLT
                        test             eax, eax
                                                                                        jz    n3_disjunction_af
                        mov              rax, qword ptr [rbp + 272]
                        mov              qword ptr [rbp + 80], rax
                        mov              rax, qword ptr [rbp + 280]
                        mov              qword ptr [rbp + 88], rax
                                                                                        jmp   proc_safe_ω
#-----------------------------------------------------------------------------------------------------------------------
n36_binop_test_α:
                        mov              eax, dword ptr [rbp + 512]
                        cmp              eax, 100
                                                                                        je    .Lx91_0
                        mov              eax, dword ptr [rbp + 528]
                        cmp              eax, 100
                                                                                        je    .Lx91_0
                        mov              eax, dword ptr [rbp + 512]
                        cmp              eax, 6
                                                                                        jne   .Lx91_2
                        mov              eax, dword ptr [rbp + 528]
                        cmp              eax, 6
                                                                                        jne   .Lx91_2
.Lx91_1:
                        mov              rax, qword ptr [rbp + 520]
                        mov              rcx, qword ptr [rbp + 536]
                        cmp              rax, rcx
                                                                                        je    n1_disjunction_af
                        mov              rcx, qword ptr [rbp + 528]
                        mov              qword ptr [rbp + 320], rcx
                        mov              rcx, qword ptr [rbp + 536]
                        mov              qword ptr [rbp + 328], rcx
                                                                                        jmp   proc_safe_ω
.Lx91_0:
                        mov              rdi, qword ptr [rbp + 512]
                        mov              rsi, qword ptr [rbp + 520]
                        mov              rdx, qword ptr [rbp + 528]
                        mov              rcx, qword ptr [rbp + 536]
                        mov              r8d, 10
                        lea              r9, [rbp + 320]
                        call             rt_relop_overload@PLT
                        test             eax, eax
                                                                                        je    .Lx91_1
                        cmp              eax, 1
                                                                                        je    n1_disjunction_af
                                                                                        jmp   proc_safe_ω
.Lx91_2:
                        mov              rdi, qword ptr [rbp + 512]
                        mov              rsi, qword ptr [rbp + 520]
                        mov              rdx, qword ptr [rbp + 528]
                        mov              rcx, qword ptr [rbp + 536]
                        mov              r8d, 10
                        call             rt_jct_relop@PLT
                        test             eax, eax
                                                                                        jz    n1_disjunction_af
                        mov              rax, qword ptr [rbp + 528]
                        mov              qword ptr [rbp + 320], rax
                        mov              rax, qword ptr [rbp + 536]
                        mov              qword ptr [rbp + 328], rax
                                                                                        jmp   proc_safe_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_safe_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_safe_β:
                                                                                        jmp   proc_safe_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_safe_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 680]
                        lea              rsp, [rbp + 704]
                        mov              rbp, [rbp + 696]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_safe_ω:
                        mov              rax, [rbp + 688]
                        lea              rsp, [rbp + 704]
                        mov              rbp, [rbp + 696]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_safe_dcα:
                        pop              r11
                        sub              rsp, 720
                        mov              qword ptr [rsp + 696], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rbp + 672], r11
                        lea              rax, [rip + .Lx92_2]
                        mov              qword ptr [rbp + 680], rax
                        lea              rax, [rip + .Lx92_3]
                        mov              qword ptr [rbp + 688], rax
                        mov              qword ptr [rbp + 16], rsi
                        mov              qword ptr [rbp + 24], rdx
                        mov              rdi, rbp
                        mov              esi, 672
                        mov              edx, 672
                        mov              ecx, 2
                        mov              r8d, 2
                        mov              r9d, 0
                        call             rt_pl_dc_prep@PLT
                                                                                        jmp   proc_safe_α_body
.Lx92_2:
                        mov              rdx, qword ptr [rsp + 0]
                        mov              rcx, rsp
                        add              rcx, -704
                        mov              r11, qword ptr [rsp + -32]
                        mov              rbp, qword ptr [rsp + -8]
                        add              rsp, 16
                        push             r11
                                                                                        jmp   rt_pl_dc_leave_γ@PLT
.Lx92_3:
                        mov              rdi, qword ptr [rsp + 0]
                        mov              rsi, rsp
                        add              rsi, -704
                        mov              r11, qword ptr [rsp + -32]
                        mov              rbp, qword ptr [rsp + -8]
                        add              rsp, 16
                        push             r11
                                                                                        jmp   rt_pl_dc_leave_ω@PLT
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_show_α
proc_show_α:
                        .global          proc_show_α
                        .global          proc_show_β
                        .global          proc_show_γ
                        .global          proc_show_ω
                        sub              rsp, 1296
                        mov              [rsp + 1272], rcx
                        mov              [rsp + 1280], rdx
                        mov              [rsp + 1288], rbp
                        mov              rbp, rsp
                        mov              rdi, rsp
                        mov              esi, 1216
                        mov              edx, 1264
                        call             rt_jmp_frame_lexprep2@PLT
proc_show_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n93_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rbp + 1184], rax
                        mov              qword ptr [rbp + 1192], rdx
                                                                                        jmp   n94_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n94_lit_integer_α:
                        mov              qword ptr [rbp + 1200], 6
                        mov              rax, qword ptr [rip + .Lx152_0]
                        mov              qword ptr [rbp + 1208], rax
                                                                                        jmp   n96_op75_α
.Lx152_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n95_lit_string_α:
                        mov              qword ptr [rbp + 1024], 1
                        mov              rax, qword ptr [rip + .Lx153_0]
                        mov              qword ptr [rbp + 1032], rax
                                                                                        jmp   n97_lit_integer_α
.Lx153_0:
                        .quad            .Lx153_0_s
.Lx153_0_s:
                        .string          "-"
#-----------------------------------------------------------------------------------------------------------------------
n96_op75_α:
                        mov              eax, dword ptr [rbp + 1184]
                        cmp              eax, 7
                                                                                        je    .Lx155_1
                        cmp              eax, 6
                                                                                        jne   .Lx155_0
                        mov              eax, dword ptr [rbp + 1200]
                        cmp              eax, 6
                                                                                        jne   .Lx155_0
.Lx155_1:
                        mov              rax, qword ptr [rbp + 1184]
                        mov              qword ptr [rbp + 1168], rax
                        mov              rax, qword ptr [rbp + 1192]
                        mov              qword ptr [rbp + 1176], rax
                                                                                        jmp   n98_binop_α
.Lx155_0:
                        lea              rdi, [rbp + 1184]
                        lea              rsi, [rbp + 1200]
                        lea              rdx, [rbp + 1168]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n98_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n97_lit_integer_α:
                        mov              qword ptr [rbp + 1104], 6
                        mov              rax, qword ptr [rip + .Lx156_0]
                        mov              qword ptr [rbp + 1112], rax
                                                                                        jmp   n99_var_α
.Lx156_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n98_binop_α:
                        mov              eax, dword ptr [rbp + 1168]
                        cmp              eax, 6
                                                                                        jne   .Lx157_0
                        mov              rax, qword ptr [rbp + 1176]
                        mov              rcx, 1
                        add              rax, rcx
                        mov              qword ptr [rbp + 1152], 6
                        mov              qword ptr [rbp + 1160], rax
                                                                                        jmp   n100_assign_α
.Lx157_0:
                        mov              rdi, qword ptr [rbp + 1168]
                        mov              rsi, qword ptr [rbp + 1176]
                        mov              rdx, qword ptr [rbp + 1200]
                        mov              rcx, qword ptr [rbp + 1208]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        je    n95_lit_string_α
                        mov              qword ptr [rbp + 1152], rax
                        mov              qword ptr [rbp + 1160], rdx
                                                                                        jmp   n100_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n99_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rbp + 1120], rax
                        mov              qword ptr [rbp + 1128], rdx
                                                                                        jmp   n101_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n100_assign_α:
                        mov              rax, qword ptr [rbp + 1152]
                        mov              rdx, qword ptr [rbp + 1160]
                        mov              qword ptr [1879052368], rax
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n95_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n101_op75_α:
                        mov              eax, dword ptr [rbp + 1120]
                        cmp              eax, 7
                                                                                        je    .Lx161_1
                        cmp              eax, 6
                                                                                        jne   .Lx161_0
                        mov              eax, dword ptr [rbp + 1104]
                        cmp              eax, 6
                                                                                        jne   .Lx161_0
.Lx161_1:
                        mov              rax, qword ptr [rbp + 1120]
                        mov              qword ptr [rbp + 1088], rax
                        mov              rax, qword ptr [rbp + 1128]
                        mov              qword ptr [rbp + 1096], rax
                                                                                        jmp   n103_binop_α
.Lx161_0:
                        lea              rdi, [rbp + 1120]
                        lea              rsi, [rbp + 1104]
                        lea              rdx, [rbp + 1088]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n103_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n102_lit_string_α:
                        mov              qword ptr [rbp + 912], 1
                        mov              rax, qword ptr [rip + .Lx162_0]
                        mov              qword ptr [rbp + 920], rax
                                                                                        jmp   n104_var_α
.Lx162_0:
                        .quad            .Lx162_0_s
.Lx162_0_s:
                        .string          "solution "
#-----------------------------------------------------------------------------------------------------------------------
n103_binop_α:
                        mov              eax, dword ptr [rbp + 1088]
                        cmp              eax, 6
                                                                                        jne   .Lx163_0
                        mov              rax, 4
                        mov              rcx, qword ptr [rbp + 1096]
                        imul             rax, rcx
                        mov              qword ptr [rbp + 1072], 6
                        mov              qword ptr [rbp + 1080], rax
                                                                                        jmp   n105_lit_integer_α
.Lx163_0:
                        mov              rdi, qword ptr [rbp + 1104]
                        mov              rsi, qword ptr [rbp + 1112]
                        mov              rdx, qword ptr [rbp + 1088]
                        mov              rcx, qword ptr [rbp + 1096]
                        call             rt_mul@PLT
                        cmp              eax, 99
                                                                                        je    n102_lit_string_α
                        mov              qword ptr [rbp + 1072], rax
                        mov              qword ptr [rbp + 1080], rdx
                                                                                        jmp   n105_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n104_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rbp + 928], rax
                        mov              qword ptr [rbp + 936], rdx
                                                                                        jmp   n106_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n105_lit_integer_α:
                        mov              qword ptr [rbp + 1136], 6
                        mov              rax, qword ptr [rip + .Lx165_0]
                        mov              qword ptr [rbp + 1144], rax
                                                                                        jmp   n108_op75_α
.Lx165_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n106_lit_string_α:
                        mov              qword ptr [rbp + 944], 1
                        mov              rax, qword ptr [rip + .Lx166_0]
                        mov              qword ptr [rbp + 952], rax
                                                                                        jmp   n109_call_builtin_icon_α
.Lx166_0:
                        .quad            .Lx166_0_s
.Lx166_0_s:
                        .string          ":"
#-----------------------------------------------------------------------------------------------------------------------
n107_lit_integer_α:
                        mov              qword ptr [rbp + 144], 6
                        mov              rax, qword ptr [rip + .Lx167_0]
                        mov              qword ptr [rbp + 152], rax
                                                                                        jmp   n110_var_α
.Lx167_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n108_op75_α:
                        mov              eax, dword ptr [rbp + 1072]
                        cmp              eax, 7
                                                                                        je    .Lx169_1
                        cmp              eax, 6
                                                                                        jne   .Lx169_0
                        mov              eax, dword ptr [rbp + 1136]
                        cmp              eax, 6
                                                                                        jne   .Lx169_0
.Lx169_1:
                        mov              rax, qword ptr [rbp + 1072]
                        mov              qword ptr [rbp + 1056], rax
                        mov              rax, qword ptr [rbp + 1080]
                        mov              qword ptr [rbp + 1064], rax
                                                                                        jmp   n111_binop_α
.Lx169_0:
                        lea              rdi, [rbp + 1072]
                        lea              rsi, [rbp + 1136]
                        lea              rdx, [rbp + 1056]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n111_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n109_call_builtin_icon_α:
                        mov              rax, qword ptr [rbp + 912]
                        mov              qword ptr [rbp + 848], rax
                        mov              rax, qword ptr [rbp + 920]
                        mov              qword ptr [rbp + 856], rax
                        mov              rax, qword ptr [rbp + 928]
                        mov              qword ptr [rbp + 864], rax
                        mov              rax, qword ptr [rbp + 936]
                        mov              qword ptr [rbp + 872], rax
                        mov              rax, qword ptr [rbp + 944]
                        mov              qword ptr [rbp + 880], rax
                        mov              rax, qword ptr [rbp + 952]
                        mov              qword ptr [rbp + 888], rax
                        .section         .rodata
.Lrkfn171:              .string          "write"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn171]
                        lea              rsi, [rbp + 848]
                        mov              edx, 3
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 832], rax
                        mov              qword ptr [rbp + 840], rdx
                        cmp              eax, 99
                                                                                        je    n107_lit_integer_α
                                                                                        jmp   n107_lit_integer_α
n109_call_builtin_icon_β:
                                                                                        jmp   n107_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n110_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rbp + 160], rax
                        mov              qword ptr [rbp + 168], rdx
                                                                                        jmp   n112_to_α
#-----------------------------------------------------------------------------------------------------------------------
n111_binop_α:
                        mov              eax, dword ptr [rbp + 1056]
                        cmp              eax, 6
                                                                                        jne   .Lx173_0
                        mov              rax, qword ptr [rbp + 1064]
                        mov              rcx, 1
                        add              rax, rcx
                        mov              qword ptr [rbp + 1040], 6
                        mov              qword ptr [rbp + 1048], rax
                                                                                        jmp   n114_call_builtin_icon_α
.Lx173_0:
                        mov              rdi, qword ptr [rbp + 1056]
                        mov              rsi, qword ptr [rbp + 1064]
                        mov              rdx, qword ptr [rbp + 1136]
                        mov              rcx, qword ptr [rbp + 1144]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        je    n102_lit_string_α
                        mov              qword ptr [rbp + 1040], rax
                        mov              qword ptr [rbp + 1048], rdx
                                                                                        jmp   n114_call_builtin_icon_α
#-----------------------------------------------------------------------------------------------------------------------
n112_to_α:
                        mov              rdi, qword ptr [rbp + 144]
                        mov              rsi, qword ptr [rbp + 152]
                        call             to_int@PLT
                        mov              qword ptr [rbp + 144], 6
                        mov              qword ptr [rbp + 152], rax
                        mov              rdi, qword ptr [rbp + 160]
                        mov              rsi, qword ptr [rbp + 168]
                        call             to_int@PLT
                        mov              qword ptr [rbp + 160], 6
                        mov              qword ptr [rbp + 168], rax
                        mov              rax, qword ptr [rbp + 152]
                        mov              qword ptr [rbp + 128], rax
.Lx175_0:
                        mov              rax, qword ptr [rbp + 128]
                        mov              rcx, qword ptr [rbp + 168]
                        cmp              rax, rcx
                                                                                        jg    n113_lit_string_α
                        mov              qword ptr [rbp + 112], 6
                        mov              qword ptr [rbp + 120], rax
                                                                                        jmp   n115_assign_α
n112_to_β:
                        inc              qword ptr [rbp + 128]
                                                                                        jmp   .Lx175_0
#-----------------------------------------------------------------------------------------------------------------------
n113_lit_string_α:
                        mov              qword ptr [rbp + 64], 1
                        mov              rax, qword ptr [rip + .Lx176_0]
                        mov              qword ptr [rbp + 72], rax
                                                                                        jmp   n116_var_α
.Lx176_0:
                        .quad            .Lx176_0_s
.Lx176_0_s:
                        .string          "  "
#-----------------------------------------------------------------------------------------------------------------------
n114_call_builtin_icon_α:
                        mov              rax, qword ptr [rbp + 1024]
                        mov              qword ptr [rbp + 976], rax
                        mov              rax, qword ptr [rbp + 1032]
                        mov              qword ptr [rbp + 984], rax
                        mov              rax, qword ptr [rbp + 1040]
                        mov              qword ptr [rbp + 992], rax
                        mov              rax, qword ptr [rbp + 1048]
                        mov              qword ptr [rbp + 1000], rax
                        .section         .rodata
.Lrkfn178:              .string          "repl"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn178]
                        lea              rsi, [rbp + 976]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 960], rax
                        mov              qword ptr [rbp + 968], rdx
                        cmp              eax, 99
                                                                                        je    n102_lit_string_α
                                                                                        jmp   n117_assign_α
n114_call_builtin_icon_β:
                                                                                        jmp   n102_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n115_assign_α:
                        mov              rax, qword ptr [rbp + 112]
                        mov              rdx, qword ptr [rbp + 120]
                        mov              qword ptr [rbp + 1232], rax
                        mov              qword ptr [rbp + 1240], rdx
                                                                                        jmp   n118_bound_α
#-----------------------------------------------------------------------------------------------------------------------
n116_var_α:
                        mov              rax, qword ptr [rbp + 1216]
                        mov              qword ptr [rbp + 80], rax
                        mov              rax, qword ptr [rbp + 1224]
                        mov              qword ptr [rbp + 88], rax
                                                                                        jmp   n119_call_builtin_icon_α
#-----------------------------------------------------------------------------------------------------------------------
n117_assign_α:
                        mov              rax, qword ptr [rbp + 960]
                        mov              rdx, qword ptr [rbp + 968]
                        mov              qword ptr [rbp + 1216], rax
                        mov              qword ptr [rbp + 1224], rdx
                                                                                        jmp   n102_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n118_bound_α:
                        mov              qword ptr [rbp + 176], rsp
                                                                                        jmp   n120_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n119_call_builtin_icon_α:
                        mov              rax, qword ptr [rbp + 64]
                        mov              qword ptr [rbp + 16], rax
                        mov              rax, qword ptr [rbp + 72]
                        mov              qword ptr [rbp + 24], rax
                        mov              rax, qword ptr [rbp + 80]
                        mov              qword ptr [rbp + 32], rax
                        mov              rax, qword ptr [rbp + 88]
                        mov              qword ptr [rbp + 40], rax
                        .section         .rodata
.Lrkfn186:              .string          "write"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn186]
                        lea              rsi, [rbp + 16]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 0], rax
                        mov              qword ptr [rbp + 8], rdx
                        cmp              eax, 99
                                                                                        je    proc_show_ω
                                                                                        jmp   proc_show_ω
n119_call_builtin_icon_β:
                                                                                        jmp   proc_show_ω
#-----------------------------------------------------------------------------------------------------------------------
n120_lit_string_α:
                        mov              qword ptr [rbp + 800], 1
                        mov              rax, qword ptr [rip + .Lx187_0]
                        mov              qword ptr [rbp + 808], rax
                                                                                        jmp   n121_var_α
.Lx187_0:
                        .quad            .Lx187_0_s
.Lx187_0_s:
                        .string          "  "
#-----------------------------------------------------------------------------------------------------------------------
n121_var_α:
                        mov              rax, qword ptr [rbp + 1216]
                        mov              qword ptr [rbp + 816], rax
                        mov              rax, qword ptr [rbp + 1224]
                        mov              qword ptr [rbp + 824], rax
                                                                                        jmp   n122_call_builtin_icon_α
#-----------------------------------------------------------------------------------------------------------------------
n122_call_builtin_icon_α:
                        mov              rax, qword ptr [rbp + 800]
                        mov              qword ptr [rbp + 752], rax
                        mov              rax, qword ptr [rbp + 808]
                        mov              qword ptr [rbp + 760], rax
                        mov              rax, qword ptr [rbp + 816]
                        mov              qword ptr [rbp + 768], rax
                        mov              rax, qword ptr [rbp + 824]
                        mov              qword ptr [rbp + 776], rax
                        .section         .rodata
.Lrkfn191:              .string          "write"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn191]
                        lea              rsi, [rbp + 752]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 736], rax
                        mov              qword ptr [rbp + 744], rdx
                        cmp              eax, 99
                                                                                        je    n123_lit_string_α
                                                                                        jmp   n123_lit_string_α
n122_call_builtin_icon_β:
                                                                                        jmp   n123_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n123_lit_string_α:
                        mov              qword ptr [rbp + 688], 1
                        mov              rax, qword ptr [rip + .Lx192_0]
                        mov              qword ptr [rbp + 696], rax
                                                                                        jmp   n124_var_α
.Lx192_0:
                        .quad            .Lx192_0_s
.Lx192_0_s:
                        .string          "|   "
#-----------------------------------------------------------------------------------------------------------------------
n124_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rbp + 704], rax
                        mov              qword ptr [rbp + 712], rdx
                                                                                        jmp   n125_call_builtin_icon_α
#-----------------------------------------------------------------------------------------------------------------------
n125_call_builtin_icon_α:
                        mov              rax, qword ptr [rbp + 688]
                        mov              qword ptr [rbp + 640], rax
                        mov              rax, qword ptr [rbp + 696]
                        mov              qword ptr [rbp + 648], rax
                        mov              rax, qword ptr [rbp + 704]
                        mov              qword ptr [rbp + 656], rax
                        mov              rax, qword ptr [rbp + 712]
                        mov              qword ptr [rbp + 664], rax
                        .section         .rodata
.Lrkfn195:              .string          "repl"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn195]
                        lea              rsi, [rbp + 640]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 624], rax
                        mov              qword ptr [rbp + 632], rdx
                        cmp              eax, 99
                                                                                        je    n126_var_ref_α
                                                                                        jmp   n127_lit_string_α
n125_call_builtin_icon_β:
                                                                                        jmp   n126_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n126_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 1248]
                        mov              qword ptr [rbp + 336], rax
                        mov              qword ptr [rbp + 344], rdx
                                                                                        jmp   n128_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n127_lit_string_α:
                        mov              qword ptr [rbp + 720], 1
                        mov              rax, qword ptr [rip + .Lx198_0]
                        mov              qword ptr [rbp + 728], rax
                                                                                        jmp   n129_binop_α
.Lx198_0:
                        .quad            .Lx198_0_s
.Lx198_0_s:
                        .string          "|"
#-----------------------------------------------------------------------------------------------------------------------
n128_lit_integer_α:
                        mov              qword ptr [rbp + 416], 6
                        mov              rax, qword ptr [rip + .Lx199_0]
                        mov              qword ptr [rbp + 424], rax
                                                                                        jmp   n130_var_ref_α
.Lx199_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n129_binop_α:
                        mov              rdi, qword ptr [rbp + 624]
                        mov              rsi, qword ptr [rbp + 632]
                        mov              rdx, qword ptr [rbp + 720]
                        mov              rcx, qword ptr [rbp + 728]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 608], rax
                        mov              qword ptr [rbp + 616], rdx
                                                                                        jmp   n131_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n130_var_ref_α:
                        mov              rax, 4294967305
                        mov              rdx, 1879052304
                        mov              qword ptr [rbp + 464], rax
                        mov              qword ptr [rbp + 472], rdx
                                                                                        jmp   n132_var_α
#-----------------------------------------------------------------------------------------------------------------------
n131_assign_α:
                        mov              rax, qword ptr [rbp + 608]
                        mov              rdx, qword ptr [rbp + 616]
                        mov              qword ptr [rbp + 1248], rax
                        mov              qword ptr [rbp + 1256], rdx
                                                                                        jmp   n126_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n132_var_α:
                        mov              rax, qword ptr [rbp + 1232]
                        mov              qword ptr [rbp + 480], rax
                        mov              rax, qword ptr [rbp + 1240]
                        mov              qword ptr [rbp + 488], rax
                                                                                        jmp   n133_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n133_subscript_α:
                        mov              rdi, qword ptr [rbp + 464]
                        mov              rsi, qword ptr [rbp + 472]
                        mov              rdx, qword ptr [rbp + 480]
                        mov              rcx, qword ptr [rbp + 488]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n134_lit_string_α
                        mov              qword ptr [rbp + 496], rax
                        mov              qword ptr [rbp + 504], rdx
                                                                                        jmp   n135_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n134_lit_string_α:
                        mov              qword ptr [rbp + 304], 1
                        mov              rax, qword ptr [rip + .Lx207_0]
                        mov              qword ptr [rbp + 312], rax
                                                                                        jmp   n136_var_α
.Lx207_0:
                        .quad            .Lx207_0_s
.Lx207_0_s:
                        .string          "  "
#-----------------------------------------------------------------------------------------------------------------------
n135_deref_α:
                        mov              rdi, qword ptr [rbp + 496]
                        mov              rsi, qword ptr [rbp + 504]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n134_lit_string_α
                        mov              qword ptr [rbp + 512], rax
                        mov              qword ptr [rbp + 520], rdx
                                                                                        jmp   n137_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n136_var_α:
                        mov              rax, qword ptr [rbp + 1248]
                        mov              qword ptr [rbp + 320], rax
                        mov              rax, qword ptr [rbp + 1256]
                        mov              qword ptr [rbp + 328], rax
                                                                                        jmp   n138_call_builtin_icon_α
#-----------------------------------------------------------------------------------------------------------------------
n137_lit_integer_α:
                        mov              qword ptr [rbp + 528], 6
                        mov              rax, qword ptr [rip + .Lx211_0]
                        mov              qword ptr [rbp + 536], rax
                                                                                        jmp   n140_op75_α
.Lx211_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n138_call_builtin_icon_α:
                        mov              rax, qword ptr [rbp + 304]
                        mov              qword ptr [rbp + 256], rax
                        mov              rax, qword ptr [rbp + 312]
                        mov              qword ptr [rbp + 264], rax
                        mov              rax, qword ptr [rbp + 320]
                        mov              qword ptr [rbp + 272], rax
                        mov              rax, qword ptr [rbp + 328]
                        mov              qword ptr [rbp + 280], rax
                        .section         .rodata
.Lrkfn213:              .string          "write"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn213]
                        lea              rsi, [rbp + 256]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 240], rax
                        mov              qword ptr [rbp + 248], rdx
                        cmp              eax, 99
                                                                                        je    n139_unmark_α
                                                                                        jmp   n141_conjunction_α
n138_call_builtin_icon_β:
                                                                                        jmp   n139_unmark_α
#-----------------------------------------------------------------------------------------------------------------------
n139_unmark_α:
                        mov              rsp, qword ptr [rbp + 176]
                                                                                        jmp   n112_to_β
#-----------------------------------------------------------------------------------------------------------------------
n140_op75_α:
                        mov              eax, dword ptr [rbp + 512]
                        cmp              eax, 7
                                                                                        je    .Lx217_1
                        cmp              eax, 6
                                                                                        jne   .Lx217_0
                        mov              eax, dword ptr [rbp + 528]
                        cmp              eax, 6
                                                                                        jne   .Lx217_0
.Lx217_1:
                        mov              rax, qword ptr [rbp + 512]
                        mov              qword ptr [rbp + 448], rax
                        mov              rax, qword ptr [rbp + 520]
                        mov              qword ptr [rbp + 456], rax
                                                                                        jmp   n142_binop_α
.Lx217_0:
                        lea              rdi, [rbp + 512]
                        lea              rsi, [rbp + 528]
                        lea              rdx, [rbp + 448]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n142_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n141_conjunction_α:
                        mov              rax, qword ptr [rbp + 240]
                        mov              qword ptr [rbp + 224], rax
                        mov              rax, qword ptr [rbp + 248]
                        mov              qword ptr [rbp + 232], rax
                                                                                        jmp   n139_unmark_α
n141_conjunction_β:
                                                                                        jmp   n139_unmark_α
#-----------------------------------------------------------------------------------------------------------------------
n142_binop_α:
                        mov              eax, dword ptr [rbp + 448]
                        cmp              eax, 6
                                                                                        jne   .Lx219_0
                        mov              rax, qword ptr [rbp + 456]
                        mov              rcx, 1
                        sub              rax, rcx
                        mov              qword ptr [rbp + 432], 6
                        mov              qword ptr [rbp + 440], rax
                                                                                        jmp   n143_op75_α
.Lx219_0:
                        mov              rdi, qword ptr [rbp + 448]
                        mov              rsi, qword ptr [rbp + 456]
                        mov              rdx, qword ptr [rbp + 528]
                        mov              rcx, qword ptr [rbp + 536]
                        call             rt_sub@PLT
                        cmp              eax, 99
                                                                                        je    n134_lit_string_α
                        mov              qword ptr [rbp + 432], rax
                        mov              qword ptr [rbp + 440], rdx
                                                                                        jmp   n143_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n143_op75_α:
                        mov              eax, dword ptr [rbp + 432]
                        cmp              eax, 7
                                                                                        je    .Lx221_1
                        cmp              eax, 6
                                                                                        jne   .Lx221_0
                        mov              eax, dword ptr [rbp + 416]
                        cmp              eax, 6
                                                                                        jne   .Lx221_0
.Lx221_1:
                        mov              rax, qword ptr [rbp + 432]
                        mov              qword ptr [rbp + 400], rax
                        mov              rax, qword ptr [rbp + 440]
                        mov              qword ptr [rbp + 408], rax
                                                                                        jmp   n144_binop_α
.Lx221_0:
                        lea              rdi, [rbp + 432]
                        lea              rsi, [rbp + 416]
                        lea              rdx, [rbp + 400]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n144_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n144_binop_α:
                        mov              eax, dword ptr [rbp + 400]
                        cmp              eax, 6
                                                                                        jne   .Lx222_0
                        mov              rax, 4
                        mov              rcx, qword ptr [rbp + 408]
                        imul             rax, rcx
                        mov              qword ptr [rbp + 384], 6
                        mov              qword ptr [rbp + 392], rax
                                                                                        jmp   n145_lit_integer_α
.Lx222_0:
                        mov              rdi, qword ptr [rbp + 416]
                        mov              rsi, qword ptr [rbp + 424]
                        mov              rdx, qword ptr [rbp + 400]
                        mov              rcx, qword ptr [rbp + 408]
                        call             rt_mul@PLT
                        cmp              eax, 99
                                                                                        je    n134_lit_string_α
                        mov              qword ptr [rbp + 384], rax
                        mov              qword ptr [rbp + 392], rdx
                                                                                        jmp   n145_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n145_lit_integer_α:
                        mov              qword ptr [rbp + 544], 6
                        mov              rax, qword ptr [rip + .Lx223_0]
                        mov              qword ptr [rbp + 552], rax
                                                                                        jmp   n146_op75_α
.Lx223_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n146_op75_α:
                        mov              eax, dword ptr [rbp + 384]
                        cmp              eax, 7
                                                                                        je    .Lx225_1
                        cmp              eax, 6
                                                                                        jne   .Lx225_0
                        mov              eax, dword ptr [rbp + 544]
                        cmp              eax, 6
                                                                                        jne   .Lx225_0
.Lx225_1:
                        mov              rax, qword ptr [rbp + 384]
                        mov              qword ptr [rbp + 368], rax
                        mov              rax, qword ptr [rbp + 392]
                        mov              qword ptr [rbp + 376], rax
                                                                                        jmp   n147_binop_α
.Lx225_0:
                        lea              rdi, [rbp + 384]
                        lea              rsi, [rbp + 544]
                        lea              rdx, [rbp + 368]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n147_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n147_binop_α:
                        mov              eax, dword ptr [rbp + 368]
                        cmp              eax, 6
                                                                                        jne   .Lx226_0
                        mov              rax, qword ptr [rbp + 376]
                        mov              rcx, 3
                        add              rax, rcx
                        mov              qword ptr [rbp + 352], 6
                        mov              qword ptr [rbp + 360], rax
                                                                                        jmp   n148_subscript_α
.Lx226_0:
                        mov              rdi, qword ptr [rbp + 368]
                        mov              rsi, qword ptr [rbp + 376]
                        mov              rdx, qword ptr [rbp + 544]
                        mov              rcx, qword ptr [rbp + 552]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        je    n134_lit_string_α
                        mov              qword ptr [rbp + 352], rax
                        mov              qword ptr [rbp + 360], rdx
                                                                                        jmp   n148_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n148_subscript_α:
                        mov              rdi, qword ptr [rbp + 336]
                        mov              rsi, qword ptr [rbp + 344]
                        mov              rdx, qword ptr [rbp + 352]
                        mov              rcx, qword ptr [rbp + 360]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n134_lit_string_α
                        mov              qword ptr [rbp + 560], rax
                        mov              qword ptr [rbp + 568], rdx
                                                                                        jmp   n149_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n149_lit_string_α:
                        mov              qword ptr [rbp + 592], 1
                        mov              rax, qword ptr [rip + .Lx228_0]
                        mov              qword ptr [rbp + 600], rax
                                                                                        jmp   n150_assign_var_α
.Lx228_0:
                        .quad            .Lx228_0_s
.Lx228_0_s:
                        .string          "Q"
#-----------------------------------------------------------------------------------------------------------------------
n150_assign_var_α:
                        mov              rdi, qword ptr [rbp + 560]
                        mov              rsi, qword ptr [rbp + 568]
                        mov              rdx, qword ptr [rbp + 592]
                        mov              rcx, qword ptr [rbp + 600]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n134_lit_string_α
                        mov              qword ptr [rbp + 576], rax
                        mov              qword ptr [rbp + 584], rdx
                                                                                        jmp   n134_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
proc_show_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_show_β:
                                                                                        jmp   proc_show_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_show_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 1272]
                        lea              rsp, [rbp + 1296]
                        mov              rbp, [rbp + 1288]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_show_ω:
                        mov              rax, [rbp + 1280]
                        lea              rsp, [rbp + 1296]
                        mov              rbp, [rbp + 1288]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_show_dcα:
                        pop              r11
                        sub              rsp, 1312
                        mov              qword ptr [rsp + 1288], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rbp + 1264], r11
                        lea              rax, [rip + .Lx230_2]
                        mov              qword ptr [rbp + 1272], rax
                        lea              rax, [rip + .Lx230_3]
                        mov              qword ptr [rbp + 1280], rax
                        mov              rdi, rbp
                        mov              esi, 1216
                        mov              edx, 1264
                        mov              ecx, 0
                        mov              r8d, 0
                        mov              r9d, 0
                        call             rt_pl_dc_prep@PLT
                                                                                        jmp   proc_show_α_body
.Lx230_2:
                        mov              rdx, qword ptr [rsp + 0]
                        mov              rcx, rsp
                        add              rcx, -1296
                        mov              r11, qword ptr [rsp + -32]
                        mov              rbp, qword ptr [rsp + -8]
                        add              rsp, 16
                        push             r11
                                                                                        jmp   rt_pl_dc_leave_γ@PLT
.Lx230_3:
                        mov              rdi, qword ptr [rsp + 0]
                        mov              rsi, rsp
                        add              rsi, -1296
                        mov              r11, qword ptr [rsp + -32]
                        mov              rbp, qword ptr [rsp + -8]
                        add              rsp, 16
                        push             r11
                                                                                        jmp   rt_pl_dc_leave_ω@PLT
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_try_col_α
proc_try_col_α:
                        .global          proc_try_col_α
                        .global          proc_try_col_β
                        .global          proc_try_col_γ
                        .global          proc_try_col_ω
                        sub              rsp, 1632
                        mov              [rsp + 1608], rcx
                        mov              [rsp + 1616], rdx
                        mov              [rsp + 1624], rbp
                        mov              rbp, rsp
                        mov              rdi, rsp
                        mov              esi, 1584
                        mov              edx, 1600
                        call             rt_jmp_frame_lexprep2@PLT
proc_try_col_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n231_disjunction_α:
                        mov              qword ptr [rbp + 1456], 0
                        mov              qword ptr [rbp + 1464], 0
                        mov              dword ptr [rbp + 1472], 0
                                                                                        jmp   n233_var_α
n231_disjunction_as:
                        mov              eax, dword ptr [rbp + 1472]
                        cmp              eax, 0
                                                                                        jne   .Lx320_0
                                                                                        jmp   n232_lit_integer_α
.Lx320_0:
                                                                                        jmp   n232_lit_integer_α
n231_disjunction_β:
                        mov              eax, dword ptr [rbp + 1472]
                                                                                        jmp   n232_lit_integer_α
n231_disjunction_af:
                        add              dword ptr [rbp + 1472], 1
                        mov              eax, dword ptr [rbp + 1472]
                                                                                        jmp   n232_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n232_lit_integer_α:
                        mov              qword ptr [rbp + 80], 6
                        mov              rax, qword ptr [rip + .Lx321_0]
                        mov              qword ptr [rbp + 88], rax
                                                                                        jmp   n235_var_α
.Lx321_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n233_var_α:
                        mov              rax, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + 1552], rax
                        mov              rax, qword ptr [rbp + 24]
                        mov              qword ptr [rbp + 1560], rax
                                                                                        jmp   n236_var_α
n233_var_β:
                                                                                        jmp   n231_disjunction_af
#-----------------------------------------------------------------------------------------------------------------------
n234_conjunction_α:
                                                                                        jmp   n231_disjunction_as
n234_conjunction_β:
                                                                                        jmp   n232_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n235_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rbp + 96], rax
                        mov              qword ptr [rbp + 104], rdx
                                                                                        jmp   n237_to_α
#-----------------------------------------------------------------------------------------------------------------------
n236_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rbp + 1568], rax
                        mov              qword ptr [rbp + 1576], rdx
                                                                                        jmp   n238_binop_test_α
#-----------------------------------------------------------------------------------------------------------------------
n237_to_α:
                        mov              rdi, qword ptr [rbp + 80]
                        mov              rsi, qword ptr [rbp + 88]
                        call             to_int@PLT
                        mov              qword ptr [rbp + 80], 6
                        mov              qword ptr [rbp + 88], rax
                        mov              rdi, qword ptr [rbp + 96]
                        mov              rsi, qword ptr [rbp + 104]
                        call             to_int@PLT
                        mov              qword ptr [rbp + 96], 6
                        mov              qword ptr [rbp + 104], rax
                        mov              rax, qword ptr [rbp + 88]
                        mov              qword ptr [rbp + 64], rax
.Lx328_0:
                        mov              rax, qword ptr [rbp + 64]
                        mov              rcx, qword ptr [rbp + 104]
                        cmp              rax, rcx
                                                                                        jg    proc_try_col_ω
                        mov              qword ptr [rbp + 48], 6
                        mov              qword ptr [rbp + 56], rax
                                                                                        jmp   n239_assign_α
n237_to_β:
                        inc              qword ptr [rbp + 64]
                                                                                        jmp   .Lx328_0
#-----------------------------------------------------------------------------------------------------------------------
n238_binop_test_α:
                        mov              eax, dword ptr [rbp + 16]
                        cmp              eax, 100
                                                                                        je    .Lx329_0
                        mov              eax, dword ptr [rbp + 1568]
                        cmp              eax, 100
                                                                                        je    .Lx329_0
                        mov              eax, dword ptr [rbp + 16]
                        cmp              eax, 6
                                                                                        jne   .Lx329_2
                        mov              eax, dword ptr [rbp + 1568]
                        cmp              eax, 6
                                                                                        jne   .Lx329_2
.Lx329_1:
                        mov              rax, qword ptr [rbp + 24]
                        mov              rcx, qword ptr [rbp + 1576]
                        cmp              rax, rcx
                                                                                        jle   n231_disjunction_af
                        mov              rcx, qword ptr [rbp + 1568]
                        mov              qword ptr [rbp + 1536], rcx
                        mov              rcx, qword ptr [rbp + 1576]
                        mov              qword ptr [rbp + 1544], rcx
                                                                                        jmp   n240_call_proc_staged_α
.Lx329_0:
                        mov              rdi, qword ptr [rbp + 16]
                        mov              rsi, qword ptr [rbp + 24]
                        mov              rdx, qword ptr [rbp + 1568]
                        mov              rcx, qword ptr [rbp + 1576]
                        mov              r8d, 7
                        lea              r9, [rbp + 1536]
                        call             rt_relop_overload@PLT
                        test             eax, eax
                                                                                        je    .Lx329_1
                        cmp              eax, 1
                                                                                        je    n231_disjunction_af
                                                                                        jmp   n240_call_proc_staged_α
.Lx329_2:
                        mov              rdi, qword ptr [rbp + 16]
                        mov              rsi, qword ptr [rbp + 24]
                        mov              rdx, qword ptr [rbp + 1568]
                        mov              rcx, qword ptr [rbp + 1576]
                        mov              r8d, 7
                        call             rt_jct_relop@PLT
                        test             eax, eax
                                                                                        jz    n231_disjunction_af
                        mov              rax, qword ptr [rbp + 1568]
                        mov              qword ptr [rbp + 1536], rax
                        mov              rax, qword ptr [rbp + 1576]
                        mov              qword ptr [rbp + 1544], rax
                                                                                        jmp   n240_call_proc_staged_α
#-----------------------------------------------------------------------------------------------------------------------
n239_assign_α:
                        mov              rax, qword ptr [rbp + 48]
                        mov              rdx, qword ptr [rbp + 56]
                        mov              qword ptr [rbp + 1584], rax
                        mov              qword ptr [rbp + 1592], rdx
                                                                                        jmp   n241_bound_α
#-----------------------------------------------------------------------------------------------------------------------
n240_call_proc_staged_α:
                        call             proc_show_dcα
                                                                                        jmp   .Lx332_2
.Lx332_2:
                        mov              qword ptr [rbp + 1504], rax
                        mov              qword ptr [rbp + 1512], rdx
                        cmp              eax, 99
                                                                                        je    n242_return_α
                                                                                        jmp   n242_return_α
n240_call_proc_staged_β:
                                                                                        jmp   n242_return_α
.Lx332_0:
                        .quad            .Lx332_0_s
.Lx332_0_s:
                        .string          "show"
#-----------------------------------------------------------------------------------------------------------------------
n241_bound_α:
                        mov              qword ptr [rbp + 112], rsp
                                                                                        jmp   n243_disjunction_α
#-----------------------------------------------------------------------------------------------------------------------
n242_return_α:
                        mov              qword ptr [rbp + 0], 0
                        mov              qword ptr [rbp + 8], 0
                                                                                        jmp   proc_try_col_γ
#-----------------------------------------------------------------------------------------------------------------------
n243_disjunction_α:
                        mov              qword ptr [rbp + 160], 0
                        mov              qword ptr [rbp + 168], 0
                        mov              dword ptr [rbp + 176], 0
                                                                                        jmp   n245_var_α
n243_disjunction_as:
                        mov              eax, dword ptr [rbp + 176]
                        cmp              eax, 0
                                                                                        jne   .Lx337_0
                        mov              rax, qword ptr [rbp + 368]
                        mov              qword ptr [rbp + 160], rax
                        mov              rax, qword ptr [rbp + 376]
                        mov              qword ptr [rbp + 168], rax
                                                                                        jmp   n244_unmark_α
.Lx337_0:
                                                                                        jmp   n244_unmark_α
n243_disjunction_β:
                        mov              eax, dword ptr [rbp + 176]
                                                                                        jmp   n244_unmark_α
n243_disjunction_af:
                        add              dword ptr [rbp + 176], 1
                        mov              eax, dword ptr [rbp + 176]
                                                                                        jmp   n244_unmark_α
#-----------------------------------------------------------------------------------------------------------------------
n244_unmark_α:
                        mov              rsp, qword ptr [rbp + 112]
                                                                                        jmp   n237_to_β
#-----------------------------------------------------------------------------------------------------------------------
n245_var_α:
                        mov              rax, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + 1424], rax
                        mov              rax, qword ptr [rbp + 24]
                        mov              qword ptr [rbp + 1432], rax
                                                                                        jmp   n247_var_α
n245_var_β:
                                                                                        jmp   n243_disjunction_af
#-----------------------------------------------------------------------------------------------------------------------
n246_conjunction_α:
                                                                                        jmp   n243_disjunction_as
n246_conjunction_β:
                                                                                        jmp   n244_unmark_α
#-----------------------------------------------------------------------------------------------------------------------
n247_var_α:
                        mov              rax, qword ptr [rbp + 1584]
                        mov              qword ptr [rbp + 1440], rax
                        mov              rax, qword ptr [rbp + 1592]
                        mov              qword ptr [rbp + 1448], rax
                                                                                        jmp   n248_call_proc_staged_α
#-----------------------------------------------------------------------------------------------------------------------
n248_call_proc_staged_α:
                        lea              rsi, [rbp + 1424]
                        lea              rdx, [rbp + 1440]
                        call             proc_safe_dcα
                                                                                        jmp   .Lx346_2
.Lx346_2:
                        mov              qword ptr [rbp + 1360], rax
                        mov              qword ptr [rbp + 1368], rdx
                        cmp              eax, 99
                                                                                        je    n243_disjunction_af
                                                                                        jmp   n249_var_ref_α
n248_call_proc_staged_β:
                                                                                        jmp   n243_disjunction_af
.Lx346_0:
                        .quad            .Lx346_0_s
.Lx346_0_s:
                        .string          "safe"
#-----------------------------------------------------------------------------------------------------------------------
n249_var_ref_α:
                        mov              rax, 4294967305
                        mov              rdx, 1879052304
                        mov              qword ptr [rbp + 1280], rax
                        mov              qword ptr [rbp + 1288], rdx
                                                                                        jmp   n250_var_α
#-----------------------------------------------------------------------------------------------------------------------
n250_var_α:
                        mov              rax, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + 1296], rax
                        mov              rax, qword ptr [rbp + 24]
                        mov              qword ptr [rbp + 1304], rax
                                                                                        jmp   n251_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n251_subscript_α:
                        mov              rdi, qword ptr [rbp + 1280]
                        mov              rsi, qword ptr [rbp + 1288]
                        mov              rdx, qword ptr [rbp + 1296]
                        mov              rcx, qword ptr [rbp + 1304]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n252_var_ref_α
                        mov              qword ptr [rbp + 1312], rax
                        mov              qword ptr [rbp + 1320], rdx
                                                                                        jmp   n253_var_α
#-----------------------------------------------------------------------------------------------------------------------
n252_var_ref_α:
                        mov              rax, 4294967305
                        mov              rdx, 1879052320
                        mov              qword ptr [rbp + 1200], rax
                        mov              qword ptr [rbp + 1208], rdx
                                                                                        jmp   n254_var_α
#-----------------------------------------------------------------------------------------------------------------------
n253_var_α:
                        mov              rax, qword ptr [rbp + 1584]
                        mov              qword ptr [rbp + 1344], rax
                        mov              rax, qword ptr [rbp + 1592]
                        mov              qword ptr [rbp + 1352], rax
                                                                                        jmp   n255_assign_var_α
#-----------------------------------------------------------------------------------------------------------------------
n254_var_α:
                        mov              rax, qword ptr [rbp + 1584]
                        mov              qword ptr [rbp + 1216], rax
                        mov              rax, qword ptr [rbp + 1592]
                        mov              qword ptr [rbp + 1224], rax
                                                                                        jmp   n256_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n255_assign_var_α:
                        mov              rdi, qword ptr [rbp + 1312]
                        mov              rsi, qword ptr [rbp + 1320]
                        mov              rdx, qword ptr [rbp + 1344]
                        mov              rcx, qword ptr [rbp + 1352]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n252_var_ref_α
                        mov              qword ptr [rbp + 1328], rax
                        mov              qword ptr [rbp + 1336], rdx
                                                                                        jmp   n252_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n256_subscript_α:
                        mov              rdi, qword ptr [rbp + 1200]
                        mov              rsi, qword ptr [rbp + 1208]
                        mov              rdx, qword ptr [rbp + 1216]
                        mov              rcx, qword ptr [rbp + 1224]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n257_var_ref_α
                        mov              qword ptr [rbp + 1232], rax
                        mov              qword ptr [rbp + 1240], rdx
                                                                                        jmp   n258_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n257_var_ref_α:
                        mov              rax, 4294967305
                        mov              rdx, 1879052336
                        mov              qword ptr [rbp + 992], rax
                        mov              qword ptr [rbp + 1000], rdx
                                                                                        jmp   n259_var_α
#-----------------------------------------------------------------------------------------------------------------------
n258_lit_integer_α:
                        mov              qword ptr [rbp + 1264], 6
                        mov              rax, qword ptr [rip + .Lx362_0]
                        mov              qword ptr [rbp + 1272], rax
                                                                                        jmp   n260_assign_var_α
.Lx362_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n259_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rbp + 1104], rax
                        mov              qword ptr [rbp + 1112], rdx
                                                                                        jmp   n261_var_α
#-----------------------------------------------------------------------------------------------------------------------
n260_assign_var_α:
                        mov              rdi, qword ptr [rbp + 1232]
                        mov              rsi, qword ptr [rbp + 1240]
                        mov              rdx, qword ptr [rbp + 1264]
                        mov              rcx, qword ptr [rbp + 1272]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n257_var_ref_α
                        mov              qword ptr [rbp + 1248], rax
                        mov              qword ptr [rbp + 1256], rdx
                                                                                        jmp   n257_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n261_var_α:
                        mov              rax, qword ptr [rbp + 1584]
                        mov              qword ptr [rbp + 1120], rax
                        mov              rax, qword ptr [rbp + 1592]
                        mov              qword ptr [rbp + 1128], rax
                                                                                        jmp   n263_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n262_var_ref_α:
                        mov              rax, 4294967305
                        mov              rdx, 1879052352
                        mov              qword ptr [rbp + 800], rax
                        mov              qword ptr [rbp + 808], rdx
                                                                                        jmp   n264_var_α
#-----------------------------------------------------------------------------------------------------------------------
n263_op75_α:
                        mov              eax, dword ptr [rbp + 1104]
                        cmp              eax, 7
                                                                                        je    .Lx370_1
                        cmp              eax, 6
                                                                                        jne   .Lx370_0
                        mov              eax, dword ptr [rbp + 1584]
                        cmp              eax, 6
                                                                                        jne   .Lx370_0
.Lx370_1:
                        mov              rax, qword ptr [rbp + 1104]
                        mov              qword ptr [rbp + 1088], rax
                        mov              rax, qword ptr [rbp + 1112]
                        mov              qword ptr [rbp + 1096], rax
                                                                                        jmp   n265_op75_α
.Lx370_0:
                        lea              rdi, [rbp + 1104]
                        lea              rsi, [rbp + 1584]
                        lea              rdx, [rbp + 1088]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n265_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n264_var_α:
                        mov              rax, qword ptr [rbp + 1584]
                        mov              qword ptr [rbp + 896], rax
                        mov              rax, qword ptr [rbp + 1592]
                        mov              qword ptr [rbp + 904], rax
                                                                                        jmp   n266_var_α
#-----------------------------------------------------------------------------------------------------------------------
n265_op75_α:
                        mov              eax, dword ptr [rbp + 1584]
                        cmp              eax, 7
                                                                                        je    .Lx374_1
                        cmp              eax, 6
                                                                                        jne   .Lx374_0
                        mov              eax, dword ptr [rbp + 1104]
                        cmp              eax, 6
                                                                                        jne   .Lx374_0
.Lx374_1:
                        mov              rax, qword ptr [rbp + 1584]
                        mov              qword ptr [rbp + 1072], rax
                        mov              rax, qword ptr [rbp + 1592]
                        mov              qword ptr [rbp + 1080], rax
                                                                                        jmp   n268_binop_α
.Lx374_0:
                        lea              rdi, [rbp + 1584]
                        lea              rsi, [rbp + 1104]
                        lea              rdx, [rbp + 1072]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n268_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n266_var_α:
                        mov              rax, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + 912], rax
                        mov              rax, qword ptr [rbp + 24]
                        mov              qword ptr [rbp + 920], rax
                                                                                        jmp   n269_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n267_var_α:
                        mov              rax, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + 768], rax
                        mov              rax, qword ptr [rbp + 24]
                        mov              qword ptr [rbp + 776], rax
                                                                                        jmp   n270_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n268_binop_α:
                        mov              eax, dword ptr [rbp + 1088]
                        cmp              eax, 6
                                                                                        jne   .Lx379_0
                        mov              eax, dword ptr [rbp + 1072]
                        cmp              eax, 6
                                                                                        jne   .Lx379_0
                        mov              rax, qword ptr [rbp + 1096]
                        mov              rcx, qword ptr [rbp + 1080]
                        add              rax, rcx
                        mov              qword ptr [rbp + 1056], 6
                        mov              qword ptr [rbp + 1064], rax
                                                                                        jmp   n272_var_α
.Lx379_0:
                        mov              rdi, qword ptr [rbp + 1088]
                        mov              rsi, qword ptr [rbp + 1096]
                        mov              rdx, qword ptr [rbp + 1072]
                        mov              rcx, qword ptr [rbp + 1080]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        je    n262_var_ref_α
                        mov              qword ptr [rbp + 1056], rax
                        mov              qword ptr [rbp + 1064], rdx
                                                                                        jmp   n272_var_α
#-----------------------------------------------------------------------------------------------------------------------
n269_op75_α:
                        mov              eax, dword ptr [rbp + 1584]
                        cmp              eax, 7
                                                                                        je    .Lx381_1
                        cmp              eax, 6
                                                                                        jne   .Lx381_0
                        mov              eax, dword ptr [rbp + 16]
                        cmp              eax, 6
                                                                                        jne   .Lx381_0
.Lx381_1:
                        mov              rax, qword ptr [rbp + 1584]
                        mov              qword ptr [rbp + 880], rax
                        mov              rax, qword ptr [rbp + 1592]
                        mov              qword ptr [rbp + 888], rax
                                                                                        jmp   n273_op75_α
.Lx381_0:
                        lea              rdi, [rbp + 1584]
                        lea              rsi, [rbp + 16]
                        lea              rdx, [rbp + 880]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n273_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n270_lit_integer_α:
                        mov              qword ptr [rbp + 784], 6
                        mov              rax, qword ptr [rip + .Lx382_0]
                        mov              qword ptr [rbp + 792], rax
                                                                                        jmp   n274_op75_α
.Lx382_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n271_var_ref_α:
                        mov              rax, 4294967305
                        mov              rdx, 1879052320
                        mov              qword ptr [rbp + 608], rax
                        mov              qword ptr [rbp + 616], rdx
                                                                                        jmp   n275_var_α
#-----------------------------------------------------------------------------------------------------------------------
n272_var_α:
                        mov              rax, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + 1136], rax
                        mov              rax, qword ptr [rbp + 24]
                        mov              qword ptr [rbp + 1144], rax
                                                                                        jmp   n276_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n273_op75_α:
                        mov              eax, dword ptr [rbp + 16]
                        cmp              eax, 7
                                                                                        je    .Lx388_1
                        cmp              eax, 6
                                                                                        jne   .Lx388_0
                        mov              eax, dword ptr [rbp + 1584]
                        cmp              eax, 6
                                                                                        jne   .Lx388_0
.Lx388_1:
                        mov              rax, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + 864], rax
                        mov              rax, qword ptr [rbp + 24]
                        mov              qword ptr [rbp + 872], rax
                                                                                        jmp   n277_binop_α
.Lx388_0:
                        lea              rdi, [rbp + 16]
                        lea              rsi, [rbp + 1584]
                        lea              rdx, [rbp + 864]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n277_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n274_op75_α:
                        mov              eax, dword ptr [rbp + 16]
                        cmp              eax, 7
                                                                                        je    .Lx390_1
                        cmp              eax, 6
                                                                                        jne   .Lx390_0
                        mov              eax, dword ptr [rbp + 784]
                        cmp              eax, 6
                                                                                        jne   .Lx390_0
.Lx390_1:
                        mov              rax, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + 752], rax
                        mov              rax, qword ptr [rbp + 24]
                        mov              qword ptr [rbp + 760], rax
                                                                                        jmp   n278_binop_α
.Lx390_0:
                        lea              rdi, [rbp + 16]
                        lea              rsi, [rbp + 784]
                        lea              rdx, [rbp + 752]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n278_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n275_var_α:
                        mov              rax, qword ptr [rbp + 1584]
                        mov              qword ptr [rbp + 624], rax
                        mov              rax, qword ptr [rbp + 1592]
                        mov              qword ptr [rbp + 632], rax
                                                                                        jmp   n279_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n276_op75_α:
                        mov              eax, dword ptr [rbp + 1056]
                        cmp              eax, 7
                                                                                        je    .Lx394_1
                        cmp              eax, 6
                                                                                        jne   .Lx394_0
                        mov              eax, dword ptr [rbp + 16]
                        cmp              eax, 6
                                                                                        jne   .Lx394_0
.Lx394_1:
                        mov              rax, qword ptr [rbp + 1056]
                        mov              qword ptr [rbp + 1040], rax
                        mov              rax, qword ptr [rbp + 1064]
                        mov              qword ptr [rbp + 1048], rax
                                                                                        jmp   n281_op75_α
.Lx394_0:
                        lea              rdi, [rbp + 1056]
                        lea              rsi, [rbp + 16]
                        lea              rdx, [rbp + 1040]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n281_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n277_binop_α:
                        mov              eax, dword ptr [rbp + 880]
                        cmp              eax, 6
                                                                                        jne   .Lx395_0
                        mov              eax, dword ptr [rbp + 864]
                        cmp              eax, 6
                                                                                        jne   .Lx395_0
                        mov              rax, qword ptr [rbp + 888]
                        mov              rcx, qword ptr [rbp + 872]
                        add              rax, rcx
                        mov              qword ptr [rbp + 848], 6
                        mov              qword ptr [rbp + 856], rax
                                                                                        jmp   n282_lit_integer_α
.Lx395_0:
                        mov              rdi, qword ptr [rbp + 880]
                        mov              rsi, qword ptr [rbp + 888]
                        mov              rdx, qword ptr [rbp + 864]
                        mov              rcx, qword ptr [rbp + 872]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        je    n267_var_α
                        mov              qword ptr [rbp + 848], rax
                        mov              qword ptr [rbp + 856], rdx
                                                                                        jmp   n282_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n278_binop_α:
                        mov              eax, dword ptr [rbp + 752]
                        cmp              eax, 6
                                                                                        jne   .Lx396_0
                        mov              rax, qword ptr [rbp + 760]
                        mov              rcx, 1
                        add              rax, rcx
                        mov              qword ptr [rbp + 736], 6
                        mov              qword ptr [rbp + 744], rax
                                                                                        jmp   n283_call_proc_staged_α
.Lx396_0:
                        mov              rdi, qword ptr [rbp + 752]
                        mov              rsi, qword ptr [rbp + 760]
                        mov              rdx, qword ptr [rbp + 784]
                        mov              rcx, qword ptr [rbp + 792]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        je    n271_var_ref_α
                        mov              qword ptr [rbp + 736], rax
                        mov              qword ptr [rbp + 744], rdx
                                                                                        jmp   n283_call_proc_staged_α
#-----------------------------------------------------------------------------------------------------------------------
n279_subscript_α:
                        mov              rdi, qword ptr [rbp + 608]
                        mov              rsi, qword ptr [rbp + 616]
                        mov              rdx, qword ptr [rbp + 624]
                        mov              rcx, qword ptr [rbp + 632]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n280_var_ref_α
                        mov              qword ptr [rbp + 640], rax
                        mov              qword ptr [rbp + 648], rdx
                                                                                        jmp   n284_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n280_var_ref_α:
                        mov              rax, 4294967305
                        mov              rdx, 1879052336
                        mov              qword ptr [rbp + 400], rax
                        mov              qword ptr [rbp + 408], rdx
                                                                                        jmp   n285_var_α
#-----------------------------------------------------------------------------------------------------------------------
n281_op75_α:
                        mov              eax, dword ptr [rbp + 16]
                        cmp              eax, 7
                                                                                        je    .Lx401_1
                        cmp              eax, 6
                                                                                        jne   .Lx401_0
                        mov              eax, dword ptr [rbp + 1056]
                        cmp              eax, 6
                                                                                        jne   .Lx401_0
.Lx401_1:
                        mov              rax, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + 1024], rax
                        mov              rax, qword ptr [rbp + 24]
                        mov              qword ptr [rbp + 1032], rax
                                                                                        jmp   n286_binop_α
.Lx401_0:
                        lea              rdi, [rbp + 16]
                        lea              rsi, [rbp + 1056]
                        lea              rdx, [rbp + 1024]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n286_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n282_lit_integer_α:
                        mov              qword ptr [rbp + 928], 6
                        mov              rax, qword ptr [rip + .Lx402_0]
                        mov              qword ptr [rbp + 936], rax
                                                                                        jmp   n287_op75_α
.Lx402_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n283_call_proc_staged_α:
                        lea              rsi, [rbp + 736]
                        call             proc_try_col_dcα
                                                                                        jmp   .Lx404_2
.Lx404_2:
                        mov              qword ptr [rbp + 688], rax
                        mov              qword ptr [rbp + 696], rdx
                        cmp              eax, 99
                                                                                        je    n271_var_ref_α
                                                                                        jmp   n271_var_ref_α
n283_call_proc_staged_β:
                                                                                        jmp   n271_var_ref_α
.Lx404_0:
                        .quad            .Lx404_0_s
.Lx404_0_s:
                        .string          "try_col"
#-----------------------------------------------------------------------------------------------------------------------
n284_lit_integer_α:
                        mov              qword ptr [rbp + 672], 6
                        mov              rax, qword ptr [rip + .Lx405_0]
                        mov              qword ptr [rbp + 680], rax
                                                                                        jmp   n288_assign_var_α
.Lx405_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n285_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rbp + 512], rax
                        mov              qword ptr [rbp + 520], rdx
                                                                                        jmp   n289_var_α
#-----------------------------------------------------------------------------------------------------------------------
n286_binop_α:
                        mov              eax, dword ptr [rbp + 1040]
                        cmp              eax, 6
                                                                                        jne   .Lx407_0
                        mov              eax, dword ptr [rbp + 1024]
                        cmp              eax, 6
                                                                                        jne   .Lx407_0
                        mov              rax, qword ptr [rbp + 1048]
                        mov              rcx, qword ptr [rbp + 1032]
                        sub              rax, rcx
                        mov              qword ptr [rbp + 1008], 6
                        mov              qword ptr [rbp + 1016], rax
                                                                                        jmp   n291_subscript_α
.Lx407_0:
                        mov              rdi, qword ptr [rbp + 1040]
                        mov              rsi, qword ptr [rbp + 1048]
                        mov              rdx, qword ptr [rbp + 1024]
                        mov              rcx, qword ptr [rbp + 1032]
                        call             rt_sub@PLT
                        cmp              eax, 99
                                                                                        je    n262_var_ref_α
                        mov              qword ptr [rbp + 1008], rax
                        mov              qword ptr [rbp + 1016], rdx
                                                                                        jmp   n291_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n287_op75_α:
                        mov              eax, dword ptr [rbp + 848]
                        cmp              eax, 7
                                                                                        je    .Lx409_1
                        cmp              eax, 6
                                                                                        jne   .Lx409_0
                        mov              eax, dword ptr [rbp + 928]
                        cmp              eax, 6
                                                                                        jne   .Lx409_0
.Lx409_1:
                        mov              rax, qword ptr [rbp + 848]
                        mov              qword ptr [rbp + 832], rax
                        mov              rax, qword ptr [rbp + 856]
                        mov              qword ptr [rbp + 840], rax
                                                                                        jmp   n292_binop_α
.Lx409_0:
                        lea              rdi, [rbp + 848]
                        lea              rsi, [rbp + 928]
                        lea              rdx, [rbp + 832]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n292_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n288_assign_var_α:
                        mov              rdi, qword ptr [rbp + 640]
                        mov              rsi, qword ptr [rbp + 648]
                        mov              rdx, qword ptr [rbp + 672]
                        mov              rcx, qword ptr [rbp + 680]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n280_var_ref_α
                        mov              qword ptr [rbp + 656], rax
                        mov              qword ptr [rbp + 664], rdx
                                                                                        jmp   n280_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n289_var_α:
                        mov              rax, qword ptr [rbp + 1584]
                        mov              qword ptr [rbp + 528], rax
                        mov              rax, qword ptr [rbp + 1592]
                        mov              qword ptr [rbp + 536], rax
                                                                                        jmp   n293_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n290_var_ref_α:
                        mov              rax, 4294967305
                        mov              rdx, 1879052352
                        mov              qword ptr [rbp + 208], rax
                        mov              qword ptr [rbp + 216], rdx
                                                                                        jmp   n294_var_α
#-----------------------------------------------------------------------------------------------------------------------
n291_subscript_α:
                        mov              rdi, qword ptr [rbp + 992]
                        mov              rsi, qword ptr [rbp + 1000]
                        mov              rdx, qword ptr [rbp + 1008]
                        mov              rcx, qword ptr [rbp + 1016]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n262_var_ref_α
                        mov              qword ptr [rbp + 1152], rax
                        mov              qword ptr [rbp + 1160], rdx
                                                                                        jmp   n295_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n292_binop_α:
                        mov              eax, dword ptr [rbp + 832]
                        cmp              eax, 6
                                                                                        jne   .Lx416_0
                        mov              rax, qword ptr [rbp + 840]
                        mov              rcx, 1
                        sub              rax, rcx
                        mov              qword ptr [rbp + 816], 6
                        mov              qword ptr [rbp + 824], rax
                                                                                        jmp   n296_subscript_α
.Lx416_0:
                        mov              rdi, qword ptr [rbp + 832]
                        mov              rsi, qword ptr [rbp + 840]
                        mov              rdx, qword ptr [rbp + 928]
                        mov              rcx, qword ptr [rbp + 936]
                        call             rt_sub@PLT
                        cmp              eax, 99
                                                                                        je    n267_var_α
                        mov              qword ptr [rbp + 816], rax
                        mov              qword ptr [rbp + 824], rdx
                                                                                        jmp   n296_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n293_op75_α:
                        mov              eax, dword ptr [rbp + 512]
                        cmp              eax, 7
                                                                                        je    .Lx418_1
                        cmp              eax, 6
                                                                                        jne   .Lx418_0
                        mov              eax, dword ptr [rbp + 1584]
                        cmp              eax, 6
                                                                                        jne   .Lx418_0
.Lx418_1:
                        mov              rax, qword ptr [rbp + 512]
                        mov              qword ptr [rbp + 496], rax
                        mov              rax, qword ptr [rbp + 520]
                        mov              qword ptr [rbp + 504], rax
                                                                                        jmp   n297_op75_α
.Lx418_0:
                        lea              rdi, [rbp + 512]
                        lea              rsi, [rbp + 1584]
                        lea              rdx, [rbp + 496]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n297_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n294_var_α:
                        mov              rax, qword ptr [rbp + 1584]
                        mov              qword ptr [rbp + 304], rax
                        mov              rax, qword ptr [rbp + 1592]
                        mov              qword ptr [rbp + 312], rax
                                                                                        jmp   n298_var_α
#-----------------------------------------------------------------------------------------------------------------------
n295_lit_integer_α:
                        mov              qword ptr [rbp + 1184], 6
                        mov              rax, qword ptr [rip + .Lx421_0]
                        mov              qword ptr [rbp + 1192], rax
                                                                                        jmp   n299_assign_var_α
.Lx421_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n296_subscript_α:
                        mov              rdi, qword ptr [rbp + 800]
                        mov              rsi, qword ptr [rbp + 808]
                        mov              rdx, qword ptr [rbp + 816]
                        mov              rcx, qword ptr [rbp + 824]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n267_var_α
                        mov              qword ptr [rbp + 944], rax
                        mov              qword ptr [rbp + 952], rdx
                                                                                        jmp   n300_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n297_op75_α:
                        mov              eax, dword ptr [rbp + 1584]
                        cmp              eax, 7
                                                                                        je    .Lx424_1
                        cmp              eax, 6
                                                                                        jne   .Lx424_0
                        mov              eax, dword ptr [rbp + 512]
                        cmp              eax, 6
                                                                                        jne   .Lx424_0
.Lx424_1:
                        mov              rax, qword ptr [rbp + 1584]
                        mov              qword ptr [rbp + 480], rax
                        mov              rax, qword ptr [rbp + 1592]
                        mov              qword ptr [rbp + 488], rax
                                                                                        jmp   n301_binop_α
.Lx424_0:
                        lea              rdi, [rbp + 1584]
                        lea              rsi, [rbp + 512]
                        lea              rdx, [rbp + 480]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n301_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n298_var_α:
                        mov              rax, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + 320], rax
                        mov              rax, qword ptr [rbp + 24]
                        mov              qword ptr [rbp + 328], rax
                                                                                        jmp   n302_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n299_assign_var_α:
                        mov              rdi, qword ptr [rbp + 1152]
                        mov              rsi, qword ptr [rbp + 1160]
                        mov              rdx, qword ptr [rbp + 1184]
                        mov              rcx, qword ptr [rbp + 1192]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n262_var_ref_α
                        mov              qword ptr [rbp + 1168], rax
                        mov              qword ptr [rbp + 1176], rdx
                                                                                        jmp   n262_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n300_lit_integer_α:
                        mov              qword ptr [rbp + 976], 6
                        mov              rax, qword ptr [rip + .Lx428_0]
                        mov              qword ptr [rbp + 984], rax
                                                                                        jmp   n303_assign_var_α
.Lx428_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n301_binop_α:
                        mov              eax, dword ptr [rbp + 496]
                        cmp              eax, 6
                                                                                        jne   .Lx429_0
                        mov              eax, dword ptr [rbp + 480]
                        cmp              eax, 6
                                                                                        jne   .Lx429_0
                        mov              rax, qword ptr [rbp + 504]
                        mov              rcx, qword ptr [rbp + 488]
                        add              rax, rcx
                        mov              qword ptr [rbp + 464], 6
                        mov              qword ptr [rbp + 472], rax
                                                                                        jmp   n304_var_α
.Lx429_0:
                        mov              rdi, qword ptr [rbp + 496]
                        mov              rsi, qword ptr [rbp + 504]
                        mov              rdx, qword ptr [rbp + 480]
                        mov              rcx, qword ptr [rbp + 488]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        je    n290_var_ref_α
                        mov              qword ptr [rbp + 464], rax
                        mov              qword ptr [rbp + 472], rdx
                                                                                        jmp   n304_var_α
#-----------------------------------------------------------------------------------------------------------------------
n302_op75_α:
                        mov              eax, dword ptr [rbp + 1584]
                        cmp              eax, 7
                                                                                        je    .Lx431_1
                        cmp              eax, 6
                                                                                        jne   .Lx431_0
                        mov              eax, dword ptr [rbp + 16]
                        cmp              eax, 6
                                                                                        jne   .Lx431_0
.Lx431_1:
                        mov              rax, qword ptr [rbp + 1584]
                        mov              qword ptr [rbp + 288], rax
                        mov              rax, qword ptr [rbp + 1592]
                        mov              qword ptr [rbp + 296], rax
                                                                                        jmp   n305_op75_α
.Lx431_0:
                        lea              rdi, [rbp + 1584]
                        lea              rsi, [rbp + 16]
                        lea              rdx, [rbp + 288]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n305_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n303_assign_var_α:
                        mov              rdi, qword ptr [rbp + 944]
                        mov              rsi, qword ptr [rbp + 952]
                        mov              rdx, qword ptr [rbp + 976]
                        mov              rcx, qword ptr [rbp + 984]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n267_var_α
                        mov              qword ptr [rbp + 960], rax
                        mov              qword ptr [rbp + 968], rdx
                                                                                        jmp   n267_var_α
#-----------------------------------------------------------------------------------------------------------------------
n304_var_α:
                        mov              rax, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + 544], rax
                        mov              rax, qword ptr [rbp + 24]
                        mov              qword ptr [rbp + 552], rax
                                                                                        jmp   n306_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n305_op75_α:
                        mov              eax, dword ptr [rbp + 16]
                        cmp              eax, 7
                                                                                        je    .Lx436_1
                        cmp              eax, 6
                                                                                        jne   .Lx436_0
                        mov              eax, dword ptr [rbp + 1584]
                        cmp              eax, 6
                                                                                        jne   .Lx436_0
.Lx436_1:
                        mov              rax, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + 272], rax
                        mov              rax, qword ptr [rbp + 24]
                        mov              qword ptr [rbp + 280], rax
                                                                                        jmp   n307_binop_α
.Lx436_0:
                        lea              rdi, [rbp + 16]
                        lea              rsi, [rbp + 1584]
                        lea              rdx, [rbp + 272]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n307_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n306_op75_α:
                        mov              eax, dword ptr [rbp + 464]
                        cmp              eax, 7
                                                                                        je    .Lx438_1
                        cmp              eax, 6
                                                                                        jne   .Lx438_0
                        mov              eax, dword ptr [rbp + 16]
                        cmp              eax, 6
                                                                                        jne   .Lx438_0
.Lx438_1:
                        mov              rax, qword ptr [rbp + 464]
                        mov              qword ptr [rbp + 448], rax
                        mov              rax, qword ptr [rbp + 472]
                        mov              qword ptr [rbp + 456], rax
                                                                                        jmp   n308_op75_α
.Lx438_0:
                        lea              rdi, [rbp + 464]
                        lea              rsi, [rbp + 16]
                        lea              rdx, [rbp + 448]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n308_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n307_binop_α:
                        mov              eax, dword ptr [rbp + 288]
                        cmp              eax, 6
                                                                                        jne   .Lx439_0
                        mov              eax, dword ptr [rbp + 272]
                        cmp              eax, 6
                                                                                        jne   .Lx439_0
                        mov              rax, qword ptr [rbp + 296]
                        mov              rcx, qword ptr [rbp + 280]
                        add              rax, rcx
                        mov              qword ptr [rbp + 256], 6
                        mov              qword ptr [rbp + 264], rax
                                                                                        jmp   n309_lit_integer_α
.Lx439_0:
                        mov              rdi, qword ptr [rbp + 288]
                        mov              rsi, qword ptr [rbp + 296]
                        mov              rdx, qword ptr [rbp + 272]
                        mov              rcx, qword ptr [rbp + 280]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        je    n244_unmark_α
                        mov              qword ptr [rbp + 256], rax
                        mov              qword ptr [rbp + 264], rdx
                                                                                        jmp   n309_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n308_op75_α:
                        mov              eax, dword ptr [rbp + 16]
                        cmp              eax, 7
                                                                                        je    .Lx441_1
                        cmp              eax, 6
                                                                                        jne   .Lx441_0
                        mov              eax, dword ptr [rbp + 464]
                        cmp              eax, 6
                                                                                        jne   .Lx441_0
.Lx441_1:
                        mov              rax, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + 432], rax
                        mov              rax, qword ptr [rbp + 24]
                        mov              qword ptr [rbp + 440], rax
                                                                                        jmp   n310_binop_α
.Lx441_0:
                        lea              rdi, [rbp + 16]
                        lea              rsi, [rbp + 464]
                        lea              rdx, [rbp + 432]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n310_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n309_lit_integer_α:
                        mov              qword ptr [rbp + 336], 6
                        mov              rax, qword ptr [rip + .Lx442_0]
                        mov              qword ptr [rbp + 344], rax
                                                                                        jmp   n311_op75_α
.Lx442_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n310_binop_α:
                        mov              eax, dword ptr [rbp + 448]
                        cmp              eax, 6
                                                                                        jne   .Lx443_0
                        mov              eax, dword ptr [rbp + 432]
                        cmp              eax, 6
                                                                                        jne   .Lx443_0
                        mov              rax, qword ptr [rbp + 456]
                        mov              rcx, qword ptr [rbp + 440]
                        sub              rax, rcx
                        mov              qword ptr [rbp + 416], 6
                        mov              qword ptr [rbp + 424], rax
                                                                                        jmp   n312_subscript_α
.Lx443_0:
                        mov              rdi, qword ptr [rbp + 448]
                        mov              rsi, qword ptr [rbp + 456]
                        mov              rdx, qword ptr [rbp + 432]
                        mov              rcx, qword ptr [rbp + 440]
                        call             rt_sub@PLT
                        cmp              eax, 99
                                                                                        je    n290_var_ref_α
                        mov              qword ptr [rbp + 416], rax
                        mov              qword ptr [rbp + 424], rdx
                                                                                        jmp   n312_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n311_op75_α:
                        mov              eax, dword ptr [rbp + 256]
                        cmp              eax, 7
                                                                                        je    .Lx445_1
                        cmp              eax, 6
                                                                                        jne   .Lx445_0
                        mov              eax, dword ptr [rbp + 336]
                        cmp              eax, 6
                                                                                        jne   .Lx445_0
.Lx445_1:
                        mov              rax, qword ptr [rbp + 256]
                        mov              qword ptr [rbp + 240], rax
                        mov              rax, qword ptr [rbp + 264]
                        mov              qword ptr [rbp + 248], rax
                                                                                        jmp   n313_binop_α
.Lx445_0:
                        lea              rdi, [rbp + 256]
                        lea              rsi, [rbp + 336]
                        lea              rdx, [rbp + 240]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n313_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n312_subscript_α:
                        mov              rdi, qword ptr [rbp + 400]
                        mov              rsi, qword ptr [rbp + 408]
                        mov              rdx, qword ptr [rbp + 416]
                        mov              rcx, qword ptr [rbp + 424]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n290_var_ref_α
                        mov              qword ptr [rbp + 560], rax
                        mov              qword ptr [rbp + 568], rdx
                                                                                        jmp   n314_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n313_binop_α:
                        mov              eax, dword ptr [rbp + 240]
                        cmp              eax, 6
                                                                                        jne   .Lx447_0
                        mov              rax, qword ptr [rbp + 248]
                        mov              rcx, 1
                        sub              rax, rcx
                        mov              qword ptr [rbp + 224], 6
                        mov              qword ptr [rbp + 232], rax
                                                                                        jmp   n315_subscript_α
.Lx447_0:
                        mov              rdi, qword ptr [rbp + 240]
                        mov              rsi, qword ptr [rbp + 248]
                        mov              rdx, qword ptr [rbp + 336]
                        mov              rcx, qword ptr [rbp + 344]
                        call             rt_sub@PLT
                        cmp              eax, 99
                                                                                        je    n244_unmark_α
                        mov              qword ptr [rbp + 224], rax
                        mov              qword ptr [rbp + 232], rdx
                                                                                        jmp   n315_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n314_lit_integer_α:
                        mov              qword ptr [rbp + 592], 6
                        mov              rax, qword ptr [rip + .Lx448_0]
                        mov              qword ptr [rbp + 600], rax
                                                                                        jmp   n316_assign_var_α
.Lx448_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n315_subscript_α:
                        mov              rdi, qword ptr [rbp + 208]
                        mov              rsi, qword ptr [rbp + 216]
                        mov              rdx, qword ptr [rbp + 224]
                        mov              rcx, qword ptr [rbp + 232]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n244_unmark_α
                        mov              qword ptr [rbp + 352], rax
                        mov              qword ptr [rbp + 360], rdx
                                                                                        jmp   n317_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n316_assign_var_α:
                        mov              rdi, qword ptr [rbp + 560]
                        mov              rsi, qword ptr [rbp + 568]
                        mov              rdx, qword ptr [rbp + 592]
                        mov              rcx, qword ptr [rbp + 600]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n290_var_ref_α
                        mov              qword ptr [rbp + 576], rax
                        mov              qword ptr [rbp + 584], rdx
                                                                                        jmp   n290_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n317_lit_integer_α:
                        mov              qword ptr [rbp + 384], 6
                        mov              rax, qword ptr [rip + .Lx451_0]
                        mov              qword ptr [rbp + 392], rax
                                                                                        jmp   n318_assign_var_α
.Lx451_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n318_assign_var_α:
                        mov              rdi, qword ptr [rbp + 352]
                        mov              rsi, qword ptr [rbp + 360]
                        mov              rdx, qword ptr [rbp + 384]
                        mov              rcx, qword ptr [rbp + 392]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n244_unmark_α
                        mov              qword ptr [rbp + 368], rax
                        mov              qword ptr [rbp + 376], rdx
                                                                                        jmp   n246_conjunction_α
#-----------------------------------------------------------------------------------------------------------------------
proc_try_col_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_try_col_β:
                                                                                        jmp   proc_try_col_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_try_col_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 1608]
                        lea              rsp, [rbp + 1632]
                        mov              rbp, [rbp + 1624]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_try_col_ω:
                        mov              rax, [rbp + 1616]
                        lea              rsp, [rbp + 1632]
                        mov              rbp, [rbp + 1624]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_try_col_dcα:
                        pop              r11
                        sub              rsp, 1648
                        mov              qword ptr [rsp + 1624], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rbp + 1600], r11
                        lea              rax, [rip + .Lx453_2]
                        mov              qword ptr [rbp + 1608], rax
                        lea              rax, [rip + .Lx453_3]
                        mov              qword ptr [rbp + 1616], rax
                        mov              qword ptr [rbp + 16], rsi
                        mov              rdi, rbp
                        mov              esi, 1584
                        mov              edx, 1600
                        mov              ecx, 1
                        mov              r8d, 1
                        mov              r9d, 0
                        call             rt_pl_dc_prep@PLT
                                                                                        jmp   proc_try_col_α_body
.Lx453_2:
                        mov              rdx, qword ptr [rsp + 0]
                        mov              rcx, rsp
                        add              rcx, -1632
                        mov              r11, qword ptr [rsp + -32]
                        mov              rbp, qword ptr [rsp + -8]
                        add              rsp, 16
                        push             r11
                                                                                        jmp   rt_pl_dc_leave_γ@PLT
.Lx453_3:
                        mov              rdi, qword ptr [rsp + 0]
                        mov              rsi, rsp
                        add              rsi, -1632
                        mov              r11, qword ptr [rsp + -32]
                        mov              rbp, qword ptr [rsp + -8]
                        add              rsp, 16
                        push             r11
                                                                                        jmp   rt_pl_dc_leave_ω@PLT
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lstartup_pname0:       .string          "safe"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_safe_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 2
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 672
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_safe_dcα]
                        call             rt_proc_set_dcfn@PLT
                        .section         .rodata
.Lstartup_pname1:       .string          "show"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname1]
                        lea              rsi, [rip + proc_show_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 1264
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        lea              rsi, [rip + proc_show_dcα]
                        call             rt_proc_set_dcfn@PLT
                        .section         .rodata
.Lstartup_pname2:       .string          "try_col"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname2]
                        lea              rsi, [rip + proc_try_col_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 1
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 1600
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        lea              rsi, [rip + proc_try_col_dcα]
                        call             rt_proc_set_dcfn@PLT
                        add              rsp, 8
                        ret
                        .section         .rodata
.Lgvan0:                .string          "n"
.Lgvan1:                .string          "placed"
.Lgvan2:                .string          "rows"
.Lgvan3:                .string          "up"
.Lgvan4:                .string          "down"
.Lgvan5:                .string          "qcount"
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
                        mov              rdi, qword ptr [rsp]
                        add              rdi, 8
                        mov              esi, dword ptr [rsp + 8]
                        sub              esi, 1
                        call             rt_main_args_stage@PLT
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
                        sub              rsp, 1064
                        mov              rdi, rsp
                        mov              ecx, 1064
                        xor              eax, eax
                        rep stosb
                        mov              [rsp + 1056], rbp
                        mov              rbp, rsp
                        push             rsi
                        sub              rsp, 8
                        call             rt_main_args_fetch@PLT
                        add              rsp, 8
                        pop              rsi
                        mov              [rbp + 16], rax
                        mov              [rbp + 24], rdx
main_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n454_disjunction_α:
                        mov              qword ptr [rbp + 896], 0
                        mov              qword ptr [rbp + 904], 0
                        mov              dword ptr [rbp + 912], 0
                                                                                        jmp   n456_var_ref_α
n454_disjunction_as:
                        mov              eax, dword ptr [rbp + 912]
                        cmp              eax, 0
                                                                                        jne   .Lx501_0
                        mov              rax, qword ptr [rbp + 928]
                        mov              qword ptr [rbp + 896], rax
                        mov              rax, qword ptr [rbp + 936]
                        mov              qword ptr [rbp + 904], rax
                                                                                        jmp   n455_assign_α
.Lx501_0:
                        cmp              eax, 1
                                                                                        jne   .Lx501_1
                        mov              rax, qword ptr [rbp + 1040]
                        mov              qword ptr [rbp + 896], rax
                        mov              rax, qword ptr [rbp + 1048]
                        mov              qword ptr [rbp + 904], rax
                                                                                        jmp   n455_assign_α
.Lx501_1:
                                                                                        jmp   n455_assign_α
n454_disjunction_β:
                        mov              eax, dword ptr [rbp + 912]
                        cmp              eax, 0
                                                                                        je    n454_disjunction_af
                                                                                        jmp   n454_disjunction_af
n454_disjunction_af:
                        add              dword ptr [rbp + 912], 1
                        mov              eax, dword ptr [rbp + 912]
                        cmp              eax, 1
                                                                                        je    n457_lit_integer_α
                                                                                        jmp   n459_var_α
#-----------------------------------------------------------------------------------------------------------------------
n455_assign_α:
                        mov              rax, qword ptr [rbp + 896]
                        mov              rdx, qword ptr [rbp + 904]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n459_var_α
#-----------------------------------------------------------------------------------------------------------------------
n456_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 16]
                        mov              qword ptr [rbp + 976], rax
                        mov              qword ptr [rbp + 984], rdx
                                                                                        jmp   n460_lit_integer_α
n456_var_ref_β:
                                                                                        jmp   n454_disjunction_af
#-----------------------------------------------------------------------------------------------------------------------
n457_lit_integer_α:
                        mov              qword ptr [rbp + 1040], 6
                        mov              rax, qword ptr [rip + .Lx505_0]
                        mov              qword ptr [rbp + 1048], rax
                                                                                        jmp   n454_disjunction_as
n457_lit_integer_β:
                                                                                        jmp   n454_disjunction_af
.Lx505_0:
                        .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n458_call_builtin_icon_α:
                        mov              rax, qword ptr [rbp + 1024]
                        mov              qword ptr [rbp + 944], rax
                        mov              rax, qword ptr [rbp + 1032]
                        mov              qword ptr [rbp + 952], rax
                        .section         .rodata
.Lrkfn507:              .string          "integer"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn507]
                        lea              rsi, [rbp + 944]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 928], rax
                        mov              qword ptr [rbp + 936], rdx
                        cmp              eax, 99
                                                                                        je    n454_disjunction_af
                                                                                        jmp   n454_disjunction_as
n458_call_builtin_icon_β:
                                                                                        jmp   n454_disjunction_af
#-----------------------------------------------------------------------------------------------------------------------
n459_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rbp + 864], rax
                        mov              qword ptr [rbp + 872], rdx
                                                                                        jmp   n461_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n460_lit_integer_α:
                        mov              qword ptr [rbp + 992], 6
                        mov              rax, qword ptr [rip + .Lx509_0]
                        mov              qword ptr [rbp + 1000], rax
                                                                                        jmp   n463_subscript_α
.Lx509_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n461_lit_integer_α:
                        mov              qword ptr [rbp + 880], 6
                        mov              rax, qword ptr [rip + .Lx510_0]
                        mov              qword ptr [rbp + 888], rax
                                                                                        jmp   n464_call_builtin_icon_α
.Lx510_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n462_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rbp + 768], rax
                        mov              qword ptr [rbp + 776], rdx
                                                                                        jmp   n465_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n463_subscript_α:
                        mov              rdi, qword ptr [rbp + 976]
                        mov              rsi, qword ptr [rbp + 984]
                        mov              rdx, qword ptr [rbp + 992]
                        mov              rcx, qword ptr [rbp + 1000]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n454_disjunction_af
                        mov              qword ptr [rbp + 1008], rax
                        mov              qword ptr [rbp + 1016], rdx
                                                                                        jmp   n467_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n464_call_builtin_icon_α:
                        mov              rax, qword ptr [rbp + 864]
                        mov              qword ptr [rbp + 816], rax
                        mov              rax, qword ptr [rbp + 872]
                        mov              qword ptr [rbp + 824], rax
                        mov              rax, qword ptr [rbp + 880]
                        mov              qword ptr [rbp + 832], rax
                        mov              rax, qword ptr [rbp + 888]
                        mov              qword ptr [rbp + 840], rax
                        .section         .rodata
.Lrkfn514:              .string          "list"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn514]
                        lea              rsi, [rbp + 816]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 800], rax
                        mov              qword ptr [rbp + 808], rdx
                        cmp              eax, 99
                                                                                        je    n462_var_α
                                                                                        jmp   n468_assign_α
n464_call_builtin_icon_β:
                                                                                        jmp   n462_var_α
#-----------------------------------------------------------------------------------------------------------------------
n465_lit_integer_α:
                        mov              qword ptr [rbp + 784], 6
                        mov              rax, qword ptr [rip + .Lx515_0]
                        mov              qword ptr [rbp + 792], rax
                                                                                        jmp   n469_call_builtin_icon_α
.Lx515_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n466_lit_integer_α:
                        mov              qword ptr [rbp + 640], 6
                        mov              rax, qword ptr [rip + .Lx516_0]
                        mov              qword ptr [rbp + 648], rax
                                                                                        jmp   n470_var_α
.Lx516_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n467_deref_α:
                        mov              rdi, qword ptr [rbp + 1008]
                        mov              rsi, qword ptr [rbp + 1016]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n454_disjunction_af
                        mov              qword ptr [rbp + 1024], rax
                        mov              qword ptr [rbp + 1032], rdx
                                                                                        jmp   n458_call_builtin_icon_α
#-----------------------------------------------------------------------------------------------------------------------
n468_assign_α:
                        mov              rax, qword ptr [rbp + 800]
                        mov              rdx, qword ptr [rbp + 808]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   n462_var_α
#-----------------------------------------------------------------------------------------------------------------------
n469_call_builtin_icon_α:
                        mov              rax, qword ptr [rbp + 768]
                        mov              qword ptr [rbp + 720], rax
                        mov              rax, qword ptr [rbp + 776]
                        mov              qword ptr [rbp + 728], rax
                        mov              rax, qword ptr [rbp + 784]
                        mov              qword ptr [rbp + 736], rax
                        mov              rax, qword ptr [rbp + 792]
                        mov              qword ptr [rbp + 744], rax
                        .section         .rodata
.Lrkfn520:              .string          "list"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn520]
                        lea              rsi, [rbp + 720]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 704], rax
                        mov              qword ptr [rbp + 712], rdx
                        cmp              eax, 99
                                                                                        je    n466_lit_integer_α
                                                                                        jmp   n471_assign_α
n469_call_builtin_icon_β:
                                                                                        jmp   n466_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n470_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rbp + 656], rax
                        mov              qword ptr [rbp + 664], rdx
                                                                                        jmp   n472_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n471_assign_α:
                        mov              rax, qword ptr [rbp + 704]
                        mov              rdx, qword ptr [rbp + 712]
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rdx
                                                                                        jmp   n466_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n472_op75_α:
                        mov              eax, dword ptr [rbp + 656]
                        cmp              eax, 7
                                                                                        je    .Lx524_1
                        cmp              eax, 6
                                                                                        jne   .Lx524_0
                        mov              eax, dword ptr [rbp + 640]
                        cmp              eax, 6
                                                                                        jne   .Lx524_0
.Lx524_1:
                        mov              rax, qword ptr [rbp + 656]
                        mov              qword ptr [rbp + 624], rax
                        mov              rax, qword ptr [rbp + 664]
                        mov              qword ptr [rbp + 632], rax
                                                                                        jmp   n474_binop_α
.Lx524_0:
                        lea              rdi, [rbp + 656]
                        lea              rsi, [rbp + 640]
                        lea              rdx, [rbp + 624]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n474_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n473_lit_integer_α:
                        mov              qword ptr [rbp + 448], 6
                        mov              rax, qword ptr [rip + .Lx525_0]
                        mov              qword ptr [rbp + 456], rax
                                                                                        jmp   n475_var_α
.Lx525_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n474_binop_α:
                        mov              eax, dword ptr [rbp + 624]
                        cmp              eax, 6
                                                                                        jne   .Lx526_0
                        mov              rax, 2
                        mov              rcx, qword ptr [rbp + 632]
                        imul             rax, rcx
                        mov              qword ptr [rbp + 608], 6
                        mov              qword ptr [rbp + 616], rax
                                                                                        jmp   n476_lit_integer_α
.Lx526_0:
                        mov              rdi, qword ptr [rbp + 640]
                        mov              rsi, qword ptr [rbp + 648]
                        mov              rdx, qword ptr [rbp + 624]
                        mov              rcx, qword ptr [rbp + 632]
                        call             rt_mul@PLT
                        cmp              eax, 99
                                                                                        je    n473_lit_integer_α
                        mov              qword ptr [rbp + 608], rax
                        mov              qword ptr [rbp + 616], rdx
                                                                                        jmp   n476_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n475_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rbp + 464], rax
                        mov              qword ptr [rbp + 472], rdx
                                                                                        jmp   n477_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n476_lit_integer_α:
                        mov              qword ptr [rbp + 672], 6
                        mov              rax, qword ptr [rip + .Lx528_0]
                        mov              qword ptr [rbp + 680], rax
                                                                                        jmp   n479_op75_α
.Lx528_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n477_op75_α:
                        mov              eax, dword ptr [rbp + 464]
                        cmp              eax, 7
                                                                                        je    .Lx530_1
                        cmp              eax, 6
                                                                                        jne   .Lx530_0
                        mov              eax, dword ptr [rbp + 448]
                        cmp              eax, 6
                                                                                        jne   .Lx530_0
.Lx530_1:
                        mov              rax, qword ptr [rbp + 464]
                        mov              qword ptr [rbp + 432], rax
                        mov              rax, qword ptr [rbp + 472]
                        mov              qword ptr [rbp + 440], rax
                                                                                        jmp   n480_binop_α
.Lx530_0:
                        lea              rdi, [rbp + 464]
                        lea              rsi, [rbp + 448]
                        lea              rdx, [rbp + 432]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n480_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n478_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx531_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n481_assign_α
.Lx531_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n479_op75_α:
                        mov              eax, dword ptr [rbp + 608]
                        cmp              eax, 7
                                                                                        je    .Lx533_1
                        cmp              eax, 6
                                                                                        jne   .Lx533_0
                        mov              eax, dword ptr [rbp + 672]
                        cmp              eax, 6
                                                                                        jne   .Lx533_0
.Lx533_1:
                        mov              rax, qword ptr [rbp + 608]
                        mov              qword ptr [rbp + 592], rax
                        mov              rax, qword ptr [rbp + 616]
                        mov              qword ptr [rbp + 600], rax
                                                                                        jmp   n482_binop_α
.Lx533_0:
                        lea              rdi, [rbp + 608]
                        lea              rsi, [rbp + 672]
                        lea              rdx, [rbp + 592]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n482_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n480_binop_α:
                        mov              eax, dword ptr [rbp + 432]
                        cmp              eax, 6
                                                                                        jne   .Lx534_0
                        mov              rax, 2
                        mov              rcx, qword ptr [rbp + 440]
                        imul             rax, rcx
                        mov              qword ptr [rbp + 416], 6
                        mov              qword ptr [rbp + 424], rax
                                                                                        jmp   n483_lit_integer_α
.Lx534_0:
                        mov              rdi, qword ptr [rbp + 448]
                        mov              rsi, qword ptr [rbp + 456]
                        mov              rdx, qword ptr [rbp + 432]
                        mov              rcx, qword ptr [rbp + 440]
                        call             rt_mul@PLT
                        cmp              eax, 99
                                                                                        je    n478_lit_integer_α
                        mov              qword ptr [rbp + 416], rax
                        mov              qword ptr [rbp + 424], rdx
                                                                                        jmp   n483_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n481_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052368], rax
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n484_var_α
#-----------------------------------------------------------------------------------------------------------------------
n482_binop_α:
                        mov              eax, dword ptr [rbp + 592]
                        cmp              eax, 6
                                                                                        jne   .Lx536_0
                        mov              rax, qword ptr [rbp + 600]
                        mov              rcx, 1
                        sub              rax, rcx
                        mov              qword ptr [rbp + 576], 6
                        mov              qword ptr [rbp + 584], rax
                                                                                        jmp   n485_lit_integer_α
.Lx536_0:
                        mov              rdi, qword ptr [rbp + 592]
                        mov              rsi, qword ptr [rbp + 600]
                        mov              rdx, qword ptr [rbp + 672]
                        mov              rcx, qword ptr [rbp + 680]
                        call             rt_sub@PLT
                        cmp              eax, 99
                                                                                        je    n473_lit_integer_α
                        mov              qword ptr [rbp + 576], rax
                        mov              qword ptr [rbp + 584], rdx
                                                                                        jmp   n485_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n483_lit_integer_α:
                        mov              qword ptr [rbp + 480], 6
                        mov              rax, qword ptr [rip + .Lx537_0]
                        mov              qword ptr [rbp + 488], rax
                                                                                        jmp   n486_op75_α
.Lx537_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n484_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rbp + 256], rax
                        mov              qword ptr [rbp + 264], rdx
                                                                                        jmp   n487_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n485_lit_integer_α:
                        mov              qword ptr [rbp + 688], 6
                        mov              rax, qword ptr [rip + .Lx539_0]
                        mov              qword ptr [rbp + 696], rax
                                                                                        jmp   n489_call_builtin_icon_α
.Lx539_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n486_op75_α:
                        mov              eax, dword ptr [rbp + 416]
                        cmp              eax, 7
                                                                                        je    .Lx541_1
                        cmp              eax, 6
                                                                                        jne   .Lx541_0
                        mov              eax, dword ptr [rbp + 480]
                        cmp              eax, 6
                                                                                        jne   .Lx541_0
.Lx541_1:
                        mov              rax, qword ptr [rbp + 416]
                        mov              qword ptr [rbp + 400], rax
                        mov              rax, qword ptr [rbp + 424]
                        mov              qword ptr [rbp + 408], rax
                                                                                        jmp   n490_binop_α
.Lx541_0:
                        lea              rdi, [rbp + 416]
                        lea              rsi, [rbp + 480]
                        lea              rdx, [rbp + 400]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n490_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n487_lit_string_α:
                        mov              qword ptr [rbp + 272], 1
                        mov              rax, qword ptr [rip + .Lx542_0]
                        mov              qword ptr [rbp + 280], rax
                                                                                        jmp   n491_call_builtin_icon_α
.Lx542_0:
                        .quad            .Lx542_0_s
.Lx542_0_s:
                        .string          "-Queens:"
#-----------------------------------------------------------------------------------------------------------------------
n488_lit_integer_α:
                        mov              qword ptr [rbp + 176], 6
                        mov              rax, qword ptr [rip + .Lx543_0]
                        mov              qword ptr [rbp + 184], rax
                                                                                        jmp   n492_call_proc_staged_α
.Lx543_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n489_call_builtin_icon_α:
                        mov              rax, qword ptr [rbp + 576]
                        mov              qword ptr [rbp + 528], rax
                        mov              rax, qword ptr [rbp + 584]
                        mov              qword ptr [rbp + 536], rax
                        mov              rax, qword ptr [rbp + 688]
                        mov              qword ptr [rbp + 544], rax
                        mov              rax, qword ptr [rbp + 696]
                        mov              qword ptr [rbp + 552], rax
                        .section         .rodata
.Lrkfn545:              .string          "list"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn545]
                        lea              rsi, [rbp + 528]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 512], rax
                        mov              qword ptr [rbp + 520], rdx
                        cmp              eax, 99
                                                                                        je    n473_lit_integer_α
                                                                                        jmp   n493_assign_α
n489_call_builtin_icon_β:
                                                                                        jmp   n473_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n490_binop_α:
                        mov              eax, dword ptr [rbp + 400]
                        cmp              eax, 6
                                                                                        jne   .Lx546_0
                        mov              rax, qword ptr [rbp + 408]
                        mov              rcx, 1
                        sub              rax, rcx
                        mov              qword ptr [rbp + 384], 6
                        mov              qword ptr [rbp + 392], rax
                                                                                        jmp   n494_lit_integer_α
.Lx546_0:
                        mov              rdi, qword ptr [rbp + 400]
                        mov              rsi, qword ptr [rbp + 408]
                        mov              rdx, qword ptr [rbp + 480]
                        mov              rcx, qword ptr [rbp + 488]
                        call             rt_sub@PLT
                        cmp              eax, 99
                                                                                        je    n478_lit_integer_α
                        mov              qword ptr [rbp + 384], rax
                        mov              qword ptr [rbp + 392], rdx
                                                                                        jmp   n494_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n491_call_builtin_icon_α:
                        mov              rax, qword ptr [rbp + 256]
                        mov              qword ptr [rbp + 208], rax
                        mov              rax, qword ptr [rbp + 264]
                        mov              qword ptr [rbp + 216], rax
                        mov              rax, qword ptr [rbp + 272]
                        mov              qword ptr [rbp + 224], rax
                        mov              rax, qword ptr [rbp + 280]
                        mov              qword ptr [rbp + 232], rax
                        .section         .rodata
.Lrkfn548:              .string          "write"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn548]
                        lea              rsi, [rbp + 208]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 192], rax
                        mov              qword ptr [rbp + 200], rdx
                        cmp              eax, 99
                                                                                        je    n488_lit_integer_α
                                                                                        jmp   n488_lit_integer_α
n491_call_builtin_icon_β:
                                                                                        jmp   n488_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n492_call_proc_staged_α:
                        lea              rsi, [rbp + 176]
                        call             proc_try_col_dcα
                                                                                        jmp   .Lx550_2
.Lx550_2:
                        mov              qword ptr [rbp + 128], rax
                        mov              qword ptr [rbp + 136], rdx
                        cmp              eax, 99
                                                                                        je    n495_var_α
                                                                                        jmp   n495_var_α
n492_call_proc_staged_β:
                                                                                        jmp   n495_var_α
.Lx550_0:
                        .quad            .Lx550_0_s
.Lx550_0_s:
                        .string          "try_col"
#-----------------------------------------------------------------------------------------------------------------------
n493_assign_α:
                        mov              rax, qword ptr [rbp + 512]
                        mov              rdx, qword ptr [rbp + 520]
                        mov              qword ptr [1879052336], rax
                        mov              qword ptr [1879052344], rdx
                                                                                        jmp   n473_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n494_lit_integer_α:
                        mov              qword ptr [rbp + 496], 6
                        mov              rax, qword ptr [rip + .Lx552_0]
                        mov              qword ptr [rbp + 504], rax
                                                                                        jmp   n496_call_builtin_icon_α
.Lx552_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n495_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rbp + 96], rax
                        mov              qword ptr [rbp + 104], rdx
                                                                                        jmp   n497_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n496_call_builtin_icon_α:
                        mov              rax, qword ptr [rbp + 384]
                        mov              qword ptr [rbp + 336], rax
                        mov              rax, qword ptr [rbp + 392]
                        mov              qword ptr [rbp + 344], rax
                        mov              rax, qword ptr [rbp + 496]
                        mov              qword ptr [rbp + 352], rax
                        mov              rax, qword ptr [rbp + 504]
                        mov              qword ptr [rbp + 360], rax
                        .section         .rodata
.Lrkfn555:              .string          "list"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn555]
                        lea              rsi, [rbp + 336]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 320], rax
                        mov              qword ptr [rbp + 328], rdx
                        cmp              eax, 99
                                                                                        je    n478_lit_integer_α
                                                                                        jmp   n498_assign_α
n496_call_builtin_icon_β:
                                                                                        jmp   n478_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n497_lit_string_α:
                        mov              qword ptr [rbp + 112], 1
                        mov              rax, qword ptr [rip + .Lx556_0]
                        mov              qword ptr [rbp + 120], rax
                                                                                        jmp   n499_call_builtin_icon_α
.Lx556_0:
                        .quad            .Lx556_0_s
.Lx556_0_s:
                        .string          " solutions total."
#-----------------------------------------------------------------------------------------------------------------------
n498_assign_α:
                        mov              rax, qword ptr [rbp + 320]
                        mov              rdx, qword ptr [rbp + 328]
                        mov              qword ptr [1879052352], rax
                        mov              qword ptr [1879052360], rdx
                                                                                        jmp   n478_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n499_call_builtin_icon_α:
                        mov              rax, qword ptr [rbp + 96]
                        mov              qword ptr [rbp + 48], rax
                        mov              rax, qword ptr [rbp + 104]
                        mov              qword ptr [rbp + 56], rax
                        mov              rax, qword ptr [rbp + 112]
                        mov              qword ptr [rbp + 64], rax
                        mov              rax, qword ptr [rbp + 120]
                        mov              qword ptr [rbp + 72], rax
                        .section         .rodata
.Lrkfn559:              .string          "write"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn559]
                        lea              rsi, [rbp + 48]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 32], rax
                        mov              qword ptr [rbp + 40], rdx
                        cmp              eax, 99
                                                                                        je    main_ω
                                                                                        jmp   main_ω
n499_call_builtin_icon_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        mov              rsp, rbp
                        mov              rbp, [rsp + 1056]
                        add              rsp, 1064
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              rsp, rbp
                        mov              eax, 99
                        xor              edx, edx
                        mov              rbp, [rsp + 1056]
                        add              rsp, 1064
                        ret
                        .section         .note.GNU-stack,"",@progbits

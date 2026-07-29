                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_range_α
proc_range_α:
                        .global          proc_range_α
                        .global          proc_range_β
                        .global          proc_range_γ
                        .global          proc_range_ω
                        sub              rsp, 256
                        mov              [rsp + 232], rcx
                        mov              [rsp + 240], rdx
                        mov              [rsp + 248], rbp
                        mov              rbp, rsp
                        mov              rdi, rsp
                        mov              esi, 192
                        mov              edx, 224
                        call             rt_jmp_frame_lexprep2@PLT
proc_range_α_body:
                        lea              rax, [rip + n6_suspend_β]
                        mov              qword ptr [rbp + 192], rax
#-----------------------------------------------------------------------------------------------------------------------
n0_var_α:
                        mov              rax, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + 96], rax
                        mov              rax, qword ptr [rbp + 24]
                        mov              qword ptr [rbp + 104], rax
                                                                                        jmp   n1_var_α
#-----------------------------------------------------------------------------------------------------------------------
n1_var_α:
                        mov              rax, qword ptr [rbp + 32]
                        mov              qword ptr [rbp + 112], rax
                        mov              rax, qword ptr [rbp + 40]
                        mov              qword ptr [rbp + 120], rax
                                                                                        jmp   n2_to_α
#-----------------------------------------------------------------------------------------------------------------------
n2_to_α:
                        mov              rdi, qword ptr [rbp + 96]
                        mov              rsi, qword ptr [rbp + 104]
                        call             to_int@PLT
                        mov              qword ptr [rbp + 96], 6
                        mov              qword ptr [rbp + 104], rax
                        mov              rdi, qword ptr [rbp + 112]
                        mov              rsi, qword ptr [rbp + 120]
                        call             to_int@PLT
                        mov              qword ptr [rbp + 112], 6
                        mov              qword ptr [rbp + 120], rax
                        mov              rax, qword ptr [rbp + 104]
                        mov              qword ptr [rbp + 80], rax
.Lx13_0:
                        mov              rax, qword ptr [rbp + 80]
                        mov              rcx, qword ptr [rbp + 120]
                        cmp              rax, rcx
                                                                                        jg    proc_range_ω
                        mov              qword ptr [rbp + 64], 6
                        mov              qword ptr [rbp + 72], rax
                                                                                        jmp   n3_assign_α
n2_to_β:
                        inc              qword ptr [rbp + 80]
                                                                                        jmp   .Lx13_0
#-----------------------------------------------------------------------------------------------------------------------
n3_assign_α:
                        mov              rax, qword ptr [rbp + 64]
                        mov              rdx, qword ptr [rbp + 72]
                        mov              qword ptr [rbp + 208], rax
                        mov              qword ptr [rbp + 216], rdx
                                                                                        jmp   n4_bound_α
#-----------------------------------------------------------------------------------------------------------------------
n4_bound_α:
                        mov              qword ptr [rbp + 128], rsp
                                                                                        jmp   n5_var_α
#-----------------------------------------------------------------------------------------------------------------------
n5_var_α:
                        mov              rax, qword ptr [rbp + 208]
                        mov              qword ptr [rbp + 176], rax
                        mov              rax, qword ptr [rbp + 216]
                        mov              qword ptr [rbp + 184], rax
                                                                                        jmp   n6_suspend_α
#-----------------------------------------------------------------------------------------------------------------------
n6_suspend_α:
                        lea              rax, [rip + n6_suspend_β]
                        mov              qword ptr [rbp + 192], rax
                        mov              rax, qword ptr [rbp + 176]
                        mov              qword ptr [rbp + 0], rax
                        mov              rax, qword ptr [rbp + 184]
                        mov              qword ptr [rbp + 8], rax
                                                                                        jmp   proc_range_γ
n6_suspend_β:
                                                                                        jmp   n7_unmark_α
#-----------------------------------------------------------------------------------------------------------------------
n7_unmark_α:
                        mov              rsp, qword ptr [rbp + 128]
                                                                                        jmp   n2_to_β
#-----------------------------------------------------------------------------------------------------------------------
proc_range_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_range_β:
                                                                                        jmp   qword ptr [rbp + 192]
#-----------------------------------------------------------------------------------------------------------------------
proc_range_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        push             rbp
                        lea              rax, [rip + proc_range_res]
                        push             rax
                        mov              rax, [rbp + 232]
                        mov              rbp, [rbp + 248]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_range_ω:
                        mov              rax, [rbp + 240]
                        lea              rsp, [rbp + 256]
                        mov              rbp, [rbp + 248]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_divisors_α
proc_divisors_α:
                        .global          proc_divisors_α
                        .global          proc_divisors_β
                        .global          proc_divisors_γ
                        .global          proc_divisors_ω
                        sub              rsp, 384
                        mov              [rsp + 360], rcx
                        mov              [rsp + 368], rdx
                        mov              [rsp + 376], rbp
                        mov              rbp, rsp
                        mov              rdi, rsp
                        mov              esi, 320
                        mov              edx, 352
                        call             rt_jmp_frame_lexprep2@PLT
proc_divisors_α_body:
                        lea              rax, [rip + n38_suspend_β]
                        mov              qword ptr [rbp + 320], rax
#-----------------------------------------------------------------------------------------------------------------------
n23_lit_integer_α:
                        mov              qword ptr [rbp + 80], 6
                        mov              rax, qword ptr [rip + .Lx39_0]
                        mov              qword ptr [rbp + 88], rax
                                                                                        jmp   n24_var_α
.Lx39_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n24_var_α:
                        mov              rax, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + 96], rax
                        mov              rax, qword ptr [rbp + 24]
                        mov              qword ptr [rbp + 104], rax
                                                                                        jmp   n25_to_α
#-----------------------------------------------------------------------------------------------------------------------
n25_to_α:
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
.Lx43_0:
                        mov              rax, qword ptr [rbp + 64]
                        mov              rcx, qword ptr [rbp + 104]
                        cmp              rax, rcx
                                                                                        jg    proc_divisors_ω
                        mov              qword ptr [rbp + 48], 6
                        mov              qword ptr [rbp + 56], rax
                                                                                        jmp   n26_assign_α
n25_to_β:
                        inc              qword ptr [rbp + 64]
                                                                                        jmp   .Lx43_0
#-----------------------------------------------------------------------------------------------------------------------
n26_assign_α:
                        mov              rax, qword ptr [rbp + 48]
                        mov              rdx, qword ptr [rbp + 56]
                        mov              qword ptr [rbp + 336], rax
                        mov              qword ptr [rbp + 344], rdx
                                                                                        jmp   n27_bound_α
#-----------------------------------------------------------------------------------------------------------------------
n27_bound_α:
                        mov              qword ptr [rbp + 112], rsp
                                                                                        jmp   n28_disjunction_α
#-----------------------------------------------------------------------------------------------------------------------
n28_disjunction_α:
                        mov              qword ptr [rbp + 160], 0
                        mov              qword ptr [rbp + 168], 0
                        mov              dword ptr [rbp + 176], 0
                                                                                        jmp   n30_var_α
n28_disjunction_as:
                        mov              eax, dword ptr [rbp + 176]
                        cmp              eax, 0
                                                                                        jne   .Lx48_0
                                                                                        jmp   n29_unmark_α
.Lx48_0:
                                                                                        jmp   n29_unmark_α
n28_disjunction_β:
                        mov              eax, dword ptr [rbp + 176]
                                                                                        jmp   n29_unmark_α
n28_disjunction_af:
                        add              dword ptr [rbp + 176], 1
                        mov              eax, dword ptr [rbp + 176]
                                                                                        jmp   n29_unmark_α
#-----------------------------------------------------------------------------------------------------------------------
n29_unmark_α:
                        mov              rsp, qword ptr [rbp + 112]
                                                                                        jmp   n25_to_β
#-----------------------------------------------------------------------------------------------------------------------
n30_var_α:
                        mov              rax, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + 272], rax
                        mov              rax, qword ptr [rbp + 24]
                        mov              qword ptr [rbp + 280], rax
                                                                                        jmp   n31_var_α
n30_var_β:
                                                                                        jmp   n28_disjunction_af
#-----------------------------------------------------------------------------------------------------------------------
n31_var_α:
                        mov              rax, qword ptr [rbp + 336]
                        mov              qword ptr [rbp + 288], rax
                        mov              rax, qword ptr [rbp + 344]
                        mov              qword ptr [rbp + 296], rax
                                                                                        jmp   n32_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n32_op75_α:
                        mov              eax, dword ptr [rbp + 16]
                        cmp              eax, 7
                                                                                        je    .Lx56_1
                        cmp              eax, 6
                                                                                        jne   .Lx56_0
                        mov              eax, dword ptr [rbp + 336]
                        cmp              eax, 6
                                                                                        jne   .Lx56_0
.Lx56_1:
                        mov              rax, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + 256], rax
                        mov              rax, qword ptr [rbp + 24]
                        mov              qword ptr [rbp + 264], rax
                                                                                        jmp   n33_op75_α
.Lx56_0:
                        lea              rdi, [rbp + 16]
                        lea              rsi, [rbp + 336]
                        lea              rdx, [rbp + 256]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n33_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n33_op75_α:
                        mov              eax, dword ptr [rbp + 336]
                        cmp              eax, 7
                                                                                        je    .Lx58_1
                        cmp              eax, 6
                                                                                        jne   .Lx58_0
                        mov              eax, dword ptr [rbp + 16]
                        cmp              eax, 6
                                                                                        jne   .Lx58_0
.Lx58_1:
                        mov              rax, qword ptr [rbp + 336]
                        mov              qword ptr [rbp + 240], rax
                        mov              rax, qword ptr [rbp + 344]
                        mov              qword ptr [rbp + 248], rax
                                                                                        jmp   n34_binop_α
.Lx58_0:
                        lea              rdi, [rbp + 336]
                        lea              rsi, [rbp + 16]
                        lea              rdx, [rbp + 240]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n34_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n34_binop_α:
                        mov              rdi, qword ptr [rbp + 256]
                        mov              rsi, qword ptr [rbp + 264]
                        mov              rdx, qword ptr [rbp + 240]
                        mov              rcx, qword ptr [rbp + 248]
                        call             rt_mod@PLT
                        cmp              eax, 99
                                                                                        je    n28_disjunction_af
                        mov              qword ptr [rbp + 224], rax
                        mov              qword ptr [rbp + 232], rdx
                                                                                        jmp   n35_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n35_lit_integer_α:
                        mov              qword ptr [rbp + 304], 6
                        mov              rax, qword ptr [rip + .Lx60_0]
                        mov              qword ptr [rbp + 312], rax
                                                                                        jmp   n36_binop_test_α
.Lx60_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n36_binop_test_α:
                        mov              eax, dword ptr [rbp + 224]
                        cmp              eax, 100
                                                                                        je    .Lx61_0
                        mov              eax, dword ptr [rbp + 304]
                        cmp              eax, 100
                                                                                        je    .Lx61_0
                        mov              eax, dword ptr [rbp + 224]
                        cmp              eax, 6
                                                                                        jne   .Lx61_2
                        mov              eax, dword ptr [rbp + 304]
                        cmp              eax, 6
                                                                                        jne   .Lx61_2
.Lx61_1:
                        mov              rax, qword ptr [rbp + 232]
                        mov              rcx, qword ptr [rbp + 312]
                        cmp              rax, rcx
                                                                                        jne   n28_disjunction_af
                        mov              rcx, qword ptr [rbp + 304]
                        mov              qword ptr [rbp + 208], rcx
                        mov              rcx, qword ptr [rbp + 312]
                        mov              qword ptr [rbp + 216], rcx
                                                                                        jmp   n37_var_α
.Lx61_0:
                        mov              rdi, qword ptr [rbp + 224]
                        mov              rsi, qword ptr [rbp + 232]
                        mov              rdx, qword ptr [rbp + 304]
                        mov              rcx, qword ptr [rbp + 312]
                        mov              r8d, 9
                        lea              r9, [rbp + 208]
                        call             rt_relop_overload@PLT
                        test             eax, eax
                                                                                        je    .Lx61_1
                        cmp              eax, 1
                                                                                        je    n28_disjunction_af
                                                                                        jmp   n37_var_α
.Lx61_2:
                        mov              rdi, qword ptr [rbp + 224]
                        mov              rsi, qword ptr [rbp + 232]
                        mov              rdx, qword ptr [rbp + 304]
                        mov              rcx, qword ptr [rbp + 312]
                        mov              r8d, 9
                        call             rt_jct_relop@PLT
                        test             eax, eax
                                                                                        jz    n28_disjunction_af
                        mov              rax, qword ptr [rbp + 304]
                        mov              qword ptr [rbp + 208], rax
                        mov              rax, qword ptr [rbp + 312]
                        mov              qword ptr [rbp + 216], rax
                                                                                        jmp   n37_var_α
#-----------------------------------------------------------------------------------------------------------------------
n37_var_α:
                        mov              rax, qword ptr [rbp + 336]
                        mov              qword ptr [rbp + 192], rax
                        mov              rax, qword ptr [rbp + 344]
                        mov              qword ptr [rbp + 200], rax
                                                                                        jmp   n38_suspend_α
#-----------------------------------------------------------------------------------------------------------------------
n38_suspend_α:
                        lea              rax, [rip + n38_suspend_β]
                        mov              qword ptr [rbp + 320], rax
                        mov              rax, qword ptr [rbp + 192]
                        mov              qword ptr [rbp + 0], rax
                        mov              rax, qword ptr [rbp + 200]
                        mov              qword ptr [rbp + 8], rax
                                                                                        jmp   proc_divisors_γ
n38_suspend_β:
                                                                                        jmp   n28_disjunction_β
#-----------------------------------------------------------------------------------------------------------------------
proc_divisors_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_divisors_β:
                                                                                        jmp   qword ptr [rbp + 320]
#-----------------------------------------------------------------------------------------------------------------------
proc_divisors_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        push             rbp
                        lea              rax, [rip + proc_divisors_res]
                        push             rax
                        mov              rax, [rbp + 360]
                        mov              rbp, [rbp + 376]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_divisors_ω:
                        mov              rax, [rbp + 368]
                        lea              rsp, [rbp + 384]
                        mov              rbp, [rbp + 376]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_fibs_α
proc_fibs_α:
                        .global          proc_fibs_α
                        .global          proc_fibs_β
                        .global          proc_fibs_γ
                        .global          proc_fibs_ω
                        sub              rsp, 384
                        mov              [rsp + 360], rcx
                        mov              [rsp + 368], rdx
                        mov              [rsp + 376], rbp
                        mov              rbp, rsp
                        mov              rdi, rsp
                        mov              esi, 288
                        mov              edx, 352
                        call             rt_jmp_frame_lexprep2@PLT
proc_fibs_α_body:
                        lea              rax, [rip + n74_suspend_β]
                        mov              qword ptr [rbp + 288], rax
#-----------------------------------------------------------------------------------------------------------------------
n66_lit_integer_α:
                        mov              qword ptr [rbp + 272], 6
                        mov              rax, qword ptr [rip + .Lx86_0]
                        mov              qword ptr [rbp + 280], rax
                                                                                        jmp   n67_assign_α
.Lx86_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n67_assign_α:
                        mov              rax, qword ptr [rbp + 272]
                        mov              rdx, qword ptr [rbp + 280]
                        mov              qword ptr [rbp + 304], rax
                        mov              qword ptr [rbp + 312], rdx
                                                                                        jmp   n68_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n68_lit_integer_α:
                        mov              qword ptr [rbp + 256], 6
                        mov              rax, qword ptr [rip + .Lx88_0]
                        mov              qword ptr [rbp + 264], rax
                                                                                        jmp   n69_assign_α
.Lx88_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n69_assign_α:
                        mov              rax, qword ptr [rbp + 256]
                        mov              rdx, qword ptr [rbp + 264]
                        mov              qword ptr [rbp + 320], rax
                        mov              qword ptr [rbp + 328], rdx
                                                                                        jmp   n70_var_α
#-----------------------------------------------------------------------------------------------------------------------
n70_var_α:
                        mov              rax, qword ptr [rbp + 304]
                        mov              qword ptr [rbp + 48], rax
                        mov              rax, qword ptr [rbp + 312]
                        mov              qword ptr [rbp + 56], rax
                                                                                        jmp   n71_var_α
#-----------------------------------------------------------------------------------------------------------------------
n71_var_α:
                        mov              rax, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + 64], rax
                        mov              rax, qword ptr [rbp + 24]
                        mov              qword ptr [rbp + 72], rax
                                                                                        jmp   n72_binop_test_α
#-----------------------------------------------------------------------------------------------------------------------
n72_binop_test_α:
                        mov              eax, dword ptr [rbp + 304]
                        cmp              eax, 100
                                                                                        je    .Lx94_0
                        mov              eax, dword ptr [rbp + 16]
                        cmp              eax, 100
                                                                                        je    .Lx94_0
                        mov              eax, dword ptr [rbp + 304]
                        cmp              eax, 6
                                                                                        jne   .Lx94_2
                        mov              eax, dword ptr [rbp + 16]
                        cmp              eax, 6
                                                                                        jne   .Lx94_2
.Lx94_1:
                        mov              rax, qword ptr [rbp + 312]
                        mov              rcx, qword ptr [rbp + 24]
                        cmp              rax, rcx
                                                                                        jg    proc_fibs_ω
                        mov              rcx, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + 32], rcx
                        mov              rcx, qword ptr [rbp + 24]
                        mov              qword ptr [rbp + 40], rcx
                                                                                        jmp   n73_var_α
.Lx94_0:
                        mov              rdi, qword ptr [rbp + 304]
                        mov              rsi, qword ptr [rbp + 312]
                        mov              rdx, qword ptr [rbp + 16]
                        mov              rcx, qword ptr [rbp + 24]
                        mov              r8d, 6
                        lea              r9, [rbp + 32]
                        call             rt_relop_overload@PLT
                        test             eax, eax
                                                                                        je    .Lx94_1
                        cmp              eax, 1
                                                                                        je    proc_fibs_ω
                                                                                        jmp   n73_var_α
.Lx94_2:
                        mov              rdi, qword ptr [rbp + 304]
                        mov              rsi, qword ptr [rbp + 312]
                        mov              rdx, qword ptr [rbp + 16]
                        mov              rcx, qword ptr [rbp + 24]
                        mov              r8d, 6
                        call             rt_jct_relop@PLT
                        test             eax, eax
                                                                                        jz    proc_fibs_ω
                        mov              rax, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + 32], rax
                        mov              rax, qword ptr [rbp + 24]
                        mov              qword ptr [rbp + 40], rax
                                                                                        jmp   n73_var_α
#-----------------------------------------------------------------------------------------------------------------------
n73_var_α:
                        mov              rax, qword ptr [rbp + 304]
                        mov              qword ptr [rbp + 240], rax
                        mov              rax, qword ptr [rbp + 312]
                        mov              qword ptr [rbp + 248], rax
                                                                                        jmp   n74_suspend_α
#-----------------------------------------------------------------------------------------------------------------------
n74_suspend_α:
                        lea              rax, [rip + n74_suspend_β]
                        mov              qword ptr [rbp + 288], rax
                        mov              rax, qword ptr [rbp + 240]
                        mov              qword ptr [rbp + 0], rax
                        mov              rax, qword ptr [rbp + 248]
                        mov              qword ptr [rbp + 8], rax
                                                                                        jmp   proc_fibs_γ
n74_suspend_β:
                                                                                        jmp   n75_var_α
#-----------------------------------------------------------------------------------------------------------------------
n75_var_α:
                        mov              rax, qword ptr [rbp + 304]
                        mov              qword ptr [rbp + 208], rax
                        mov              rax, qword ptr [rbp + 312]
                        mov              qword ptr [rbp + 216], rax
                                                                                        jmp   n76_var_α
#-----------------------------------------------------------------------------------------------------------------------
n76_var_α:
                        mov              rax, qword ptr [rbp + 320]
                        mov              qword ptr [rbp + 224], rax
                        mov              rax, qword ptr [rbp + 328]
                        mov              qword ptr [rbp + 232], rax
                                                                                        jmp   n78_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n77_var_α:
                        mov              rax, qword ptr [rbp + 320]
                        mov              qword ptr [rbp + 144], rax
                        mov              rax, qword ptr [rbp + 328]
                        mov              qword ptr [rbp + 152], rax
                                                                                        jmp   n79_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n78_op75_α:
                        mov              eax, dword ptr [rbp + 304]
                        cmp              eax, 7
                                                                                        je    .Lx106_1
                        cmp              eax, 6
                                                                                        jne   .Lx106_0
                        mov              eax, dword ptr [rbp + 320]
                        cmp              eax, 6
                                                                                        jne   .Lx106_0
.Lx106_1:
                        mov              rax, qword ptr [rbp + 304]
                        mov              qword ptr [rbp + 192], rax
                        mov              rax, qword ptr [rbp + 312]
                        mov              qword ptr [rbp + 200], rax
                                                                                        jmp   n81_op75_α
.Lx106_0:
                        lea              rdi, [rbp + 304]
                        lea              rsi, [rbp + 320]
                        lea              rdx, [rbp + 192]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n81_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n79_assign_α:
                        mov              rax, qword ptr [rbp + 144]
                        mov              rdx, qword ptr [rbp + 152]
                        mov              qword ptr [rbp + 304], rax
                        mov              qword ptr [rbp + 312], rdx
                                                                                        jmp   n80_var_α
#-----------------------------------------------------------------------------------------------------------------------
n80_var_α:
                        mov              rax, qword ptr [rbp + 336]
                        mov              qword ptr [rbp + 112], rax
                        mov              rax, qword ptr [rbp + 344]
                        mov              qword ptr [rbp + 120], rax
                                                                                        jmp   n82_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n81_op75_α:
                        mov              eax, dword ptr [rbp + 320]
                        cmp              eax, 7
                                                                                        je    .Lx111_1
                        cmp              eax, 6
                                                                                        jne   .Lx111_0
                        mov              eax, dword ptr [rbp + 304]
                        cmp              eax, 6
                                                                                        jne   .Lx111_0
.Lx111_1:
                        mov              rax, qword ptr [rbp + 320]
                        mov              qword ptr [rbp + 176], rax
                        mov              rax, qword ptr [rbp + 328]
                        mov              qword ptr [rbp + 184], rax
                                                                                        jmp   n83_binop_α
.Lx111_0:
                        lea              rdi, [rbp + 320]
                        lea              rsi, [rbp + 304]
                        lea              rdx, [rbp + 176]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n83_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n82_assign_α:
                        mov              rax, qword ptr [rbp + 112]
                        mov              rdx, qword ptr [rbp + 120]
                        mov              qword ptr [rbp + 320], rax
                        mov              qword ptr [rbp + 328], rdx
                        mov              qword ptr [rbp + 96], rax
                        mov              qword ptr [rbp + 104], rdx
                                                                                        jmp   n84_conjunction_α
#-----------------------------------------------------------------------------------------------------------------------
n83_binop_α:
                        mov              eax, dword ptr [rbp + 192]
                        cmp              eax, 6
                                                                                        jne   .Lx113_0
                        mov              eax, dword ptr [rbp + 176]
                        cmp              eax, 6
                                                                                        jne   .Lx113_0
                        mov              rax, qword ptr [rbp + 200]
                        mov              rcx, qword ptr [rbp + 184]
                        add              rax, rcx
                        mov              qword ptr [rbp + 160], 6
                        mov              qword ptr [rbp + 168], rax
                                                                                        jmp   n85_assign_α
.Lx113_0:
                        mov              rdi, qword ptr [rbp + 192]
                        mov              rsi, qword ptr [rbp + 200]
                        mov              rdx, qword ptr [rbp + 176]
                        mov              rcx, qword ptr [rbp + 184]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        je    n77_var_α
                        mov              qword ptr [rbp + 160], rax
                        mov              qword ptr [rbp + 168], rdx
                                                                                        jmp   n85_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n84_conjunction_α:
                        mov              rax, qword ptr [rbp + 96]
                        mov              qword ptr [rbp + 80], rax
                        mov              rax, qword ptr [rbp + 104]
                        mov              qword ptr [rbp + 88], rax
                                                                                        jmp   n70_var_α
n84_conjunction_β:
                                                                                        jmp   n70_var_α
#-----------------------------------------------------------------------------------------------------------------------
n85_assign_α:
                        mov              rax, qword ptr [rbp + 160]
                        mov              rdx, qword ptr [rbp + 168]
                        mov              qword ptr [rbp + 336], rax
                        mov              qword ptr [rbp + 344], rdx
                                                                                        jmp   n77_var_α
#-----------------------------------------------------------------------------------------------------------------------
proc_fibs_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_fibs_β:
                                                                                        jmp   qword ptr [rbp + 288]
#-----------------------------------------------------------------------------------------------------------------------
proc_fibs_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        push             rbp
                        lea              rax, [rip + proc_fibs_res]
                        push             rax
                        mov              rax, [rbp + 360]
                        mov              rbp, [rbp + 376]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_fibs_ω:
                        mov              rax, [rbp + 368]
                        lea              rsp, [rbp + 384]
                        mov              rbp, [rbp + 376]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_perfect_α
proc_perfect_α:
                        .global          proc_perfect_α
                        .global          proc_perfect_β
                        .global          proc_perfect_γ
                        .global          proc_perfect_ω
                        sub              rsp, 640
                        mov              [rsp + 616], rcx
                        mov              [rsp + 624], rdx
                        mov              [rsp + 632], rbp
                        mov              rbp, rsp
                        mov              rdi, rsp
                        mov              esi, 560
                        mov              edx, 608
                        call             rt_jmp_frame_lexprep2@PLT
proc_perfect_α_body:
                        lea              rax, [rip + n143_suspend_β]
                        mov              qword ptr [rbp + 560], rax
#-----------------------------------------------------------------------------------------------------------------------
n116_lit_integer_α:
                        mov              qword ptr [rbp + 128], 6
                        mov              rax, qword ptr [rip + .Lx144_0]
                        mov              qword ptr [rbp + 136], rax
                                                                                        jmp   n117_var_α
.Lx144_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n117_var_α:
                        mov              rax, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + 144], rax
                        mov              rax, qword ptr [rbp + 24]
                        mov              qword ptr [rbp + 152], rax
                                                                                        jmp   n118_proc_gen_α
#-----------------------------------------------------------------------------------------------------------------------
n118_proc_gen_α:
                        mov              qword ptr [rbp + 96], 0
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx148_20
                        mov              rax, qword ptr [rbp + 128]
                        mov              rdx, qword ptr [rbp + 136]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx148_21
.Lx148_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rbp + 128]
                        mov              rdx, qword ptr [rbp + 136]
                        call             rt_arg_stage@PLT
.Lx148_21:
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx148_22
                        mov              rax, qword ptr [rbp + 144]
                        mov              rdx, qword ptr [rbp + 152]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 16], rax
                        mov              qword ptr [r10 + 24], rdx
                                                                                        jmp   .Lx148_23
.Lx148_22:
                        mov              edi, 1
                        mov              rsi, qword ptr [rbp + 144]
                        mov              rdx, qword ptr [rbp + 152]
                        call             rt_arg_stage@PLT
.Lx148_23:
                        mov              edi, 0
                        mov              esi, 2
                        call             rt_proc_call_open_det@PLT
                        test             rax, rax
                                                                                        je    .Lx148_1
                        lea              rcx, [rip + .Lx148_3]
                        lea              rdx, [rip + .Lx148_4]
                                                                                        jmp   rax
.Lx148_3:
                        mov              qword ptr [rbp + 104], rsp
                        mov              rax, qword ptr [rbp + 96]
                        test             rax, rax
                                                                                        jne   .Lx148_5
                        mov              qword ptr [rbp + 96], 1
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx148_2
.Lx148_5:
                        call             rt_gen_spine_pass_γ@PLT
                                                                                        jmp   .Lx148_2
.Lx148_4:
                        mov              rax, qword ptr [rbp + 96]
                        test             rax, rax
                                                                                        jne   .Lx148_6
                        mov              qword ptr [rbp + 96], 1
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx148_2
.Lx148_6:
                        call             rt_gen_spine_pass_ω@PLT
                                                                                        jmp   .Lx148_2
.Lx148_1:
                        call             rt_faildescr@PLT
.Lx148_2:
                        mov              qword ptr [rbp + 48], rax
                        mov              qword ptr [rbp + 56], rdx
                        cmp              eax, 99
                                                                                        je    proc_perfect_ω
                                                                                        jmp   n119_assign_α
n118_proc_gen_β:
                        call             rt_gen_spine_resume_enter@PLT
                        mov              rsp, qword ptr [rbp + 104]
                                                                                        jmp   qword ptr [rsp]
.Lx148_0:
                        .quad            .Lx148_0_s
.Lx148_0_s:
                        .string          "range"
#-----------------------------------------------------------------------------------------------------------------------
n119_assign_α:
                        mov              rax, qword ptr [rbp + 48]
                        mov              rdx, qword ptr [rbp + 56]
                        mov              qword ptr [rbp + 576], rax
                        mov              qword ptr [rbp + 584], rdx
                                                                                        jmp   n120_bound_α
#-----------------------------------------------------------------------------------------------------------------------
n120_bound_α:
                        mov              qword ptr [rbp + 160], rsp
                                                                                        jmp   n121_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n121_lit_integer_α:
                        mov              qword ptr [rbp + 544], 6
                        mov              rax, qword ptr [rip + .Lx152_0]
                        mov              qword ptr [rbp + 552], rax
                                                                                        jmp   n122_assign_α
.Lx152_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n122_assign_α:
                        mov              rax, qword ptr [rbp + 544]
                        mov              rdx, qword ptr [rbp + 552]
                        mov              qword ptr [rbp + 592], rax
                        mov              qword ptr [rbp + 600], rdx
                                                                                        jmp   n123_var_α
#-----------------------------------------------------------------------------------------------------------------------
n123_var_α:
                        mov              rax, qword ptr [rbp + 592]
                        mov              qword ptr [rbp + 448], rax
                        mov              rax, qword ptr [rbp + 600]
                        mov              qword ptr [rbp + 456], rax
                                                                                        jmp   n124_var_α
#-----------------------------------------------------------------------------------------------------------------------
n124_var_α:
                        mov              rax, qword ptr [rbp + 576]
                        mov              qword ptr [rbp + 528], rax
                        mov              rax, qword ptr [rbp + 584]
                        mov              qword ptr [rbp + 536], rax
                                                                                        jmp   n126_proc_gen_α
#-----------------------------------------------------------------------------------------------------------------------
n125_var_α:
                        mov              rax, qword ptr [rbp + 592]
                        mov              qword ptr [rbp + 368], rax
                        mov              rax, qword ptr [rbp + 600]
                        mov              qword ptr [rbp + 376], rax
                                                                                        jmp   n127_var_α
#-----------------------------------------------------------------------------------------------------------------------
n126_proc_gen_α:
                        mov              qword ptr [rbp + 496], 0
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx161_20
                        mov              rax, qword ptr [rbp + 528]
                        mov              rdx, qword ptr [rbp + 536]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx161_21
.Lx161_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rbp + 528]
                        mov              rdx, qword ptr [rbp + 536]
                        call             rt_arg_stage@PLT
.Lx161_21:
                        mov              edi, 1
                        mov              esi, 1
                        call             rt_proc_call_open_det@PLT
                        test             rax, rax
                                                                                        je    .Lx161_1
                        lea              rcx, [rip + .Lx161_3]
                        lea              rdx, [rip + .Lx161_4]
                                                                                        jmp   rax
.Lx161_3:
                        mov              qword ptr [rbp + 504], rsp
                        mov              rax, qword ptr [rbp + 496]
                        test             rax, rax
                                                                                        jne   .Lx161_5
                        mov              qword ptr [rbp + 496], 1
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx161_2
.Lx161_5:
                        call             rt_gen_spine_pass_γ@PLT
                                                                                        jmp   .Lx161_2
.Lx161_4:
                        mov              rax, qword ptr [rbp + 496]
                        test             rax, rax
                                                                                        jne   .Lx161_6
                        mov              qword ptr [rbp + 496], 1
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx161_2
.Lx161_6:
                        call             rt_gen_spine_pass_ω@PLT
                                                                                        jmp   .Lx161_2
.Lx161_1:
                        call             rt_faildescr@PLT
.Lx161_2:
                        mov              qword ptr [rbp + 464], rax
                        mov              qword ptr [rbp + 472], rdx
                        cmp              eax, 99
                                                                                        je    n125_var_α
                                                                                        jmp   n129_op75_α
n126_proc_gen_β:
                        call             rt_gen_spine_resume_enter@PLT
                        mov              rsp, qword ptr [rbp + 504]
                                                                                        jmp   qword ptr [rsp]
.Lx161_0:
                        .quad            .Lx161_0_s
.Lx161_0_s:
                        .string          "divisors"
#-----------------------------------------------------------------------------------------------------------------------
n127_var_α:
                        mov              rax, qword ptr [rbp + 576]
                        mov              qword ptr [rbp + 384], rax
                        mov              rax, qword ptr [rbp + 584]
                        mov              qword ptr [rbp + 392], rax
                                                                                        jmp   n130_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n128_disjunction_α:
                        mov              qword ptr [rbp + 224], 0
                        mov              qword ptr [rbp + 232], 0
                        mov              dword ptr [rbp + 240], 0
                                                                                        jmp   n132_var_α
n128_disjunction_as:
                        mov              eax, dword ptr [rbp + 240]
                        cmp              eax, 0
                                                                                        jne   .Lx165_0
                                                                                        jmp   n131_conjunction_α
.Lx165_0:
                                                                                        jmp   n131_conjunction_α
n128_disjunction_β:
                        mov              eax, dword ptr [rbp + 240]
                                                                                        jmp   n135_unmark_α
n128_disjunction_af:
                        add              dword ptr [rbp + 240], 1
                        mov              eax, dword ptr [rbp + 240]
                                                                                        jmp   n135_unmark_α
#-----------------------------------------------------------------------------------------------------------------------
n129_op75_α:
                        mov              eax, dword ptr [rbp + 592]
                        cmp              eax, 7
                                                                                        je    .Lx167_1
                        cmp              eax, 6
                                                                                        jne   .Lx167_0
                        mov              eax, dword ptr [rbp + 464]
                        cmp              eax, 6
                                                                                        jne   .Lx167_0
.Lx167_1:
                        mov              rax, qword ptr [rbp + 592]
                        mov              qword ptr [rbp + 432], rax
                        mov              rax, qword ptr [rbp + 600]
                        mov              qword ptr [rbp + 440], rax
                                                                                        jmp   n133_op75_α
.Lx167_0:
                        lea              rdi, [rbp + 592]
                        lea              rsi, [rbp + 464]
                        lea              rdx, [rbp + 432]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n133_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n130_op75_α:
                        mov              eax, dword ptr [rbp + 592]
                        cmp              eax, 7
                                                                                        je    .Lx169_1
                        cmp              eax, 6
                                                                                        jne   .Lx169_0
                        mov              eax, dword ptr [rbp + 576]
                        cmp              eax, 6
                                                                                        jne   .Lx169_0
.Lx169_1:
                        mov              rax, qword ptr [rbp + 592]
                        mov              qword ptr [rbp + 352], rax
                        mov              rax, qword ptr [rbp + 600]
                        mov              qword ptr [rbp + 360], rax
                                                                                        jmp   n134_op75_α
.Lx169_0:
                        lea              rdi, [rbp + 592]
                        lea              rsi, [rbp + 576]
                        lea              rdx, [rbp + 352]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n134_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n131_conjunction_α:
                        mov              rax, qword ptr [rbp + 224]
                        mov              qword ptr [rbp + 208], rax
                        mov              rax, qword ptr [rbp + 232]
                        mov              qword ptr [rbp + 216], rax
                                                                                        jmp   n135_unmark_α
n131_conjunction_β:
                                                                                        jmp   n135_unmark_α
#-----------------------------------------------------------------------------------------------------------------------
n132_var_α:
                        mov              rax, qword ptr [rbp + 592]
                        mov              qword ptr [rbp + 288], rax
                        mov              rax, qword ptr [rbp + 600]
                        mov              qword ptr [rbp + 296], rax
                                                                                        jmp   n136_var_α
n132_var_β:
                                                                                        jmp   n128_disjunction_af
#-----------------------------------------------------------------------------------------------------------------------
n133_op75_α:
                        mov              eax, dword ptr [rbp + 464]
                        cmp              eax, 7
                                                                                        je    .Lx174_1
                        cmp              eax, 6
                                                                                        jne   .Lx174_0
                        mov              eax, dword ptr [rbp + 592]
                        cmp              eax, 6
                                                                                        jne   .Lx174_0
.Lx174_1:
                        mov              rax, qword ptr [rbp + 464]
                        mov              qword ptr [rbp + 416], rax
                        mov              rax, qword ptr [rbp + 472]
                        mov              qword ptr [rbp + 424], rax
                                                                                        jmp   n137_binop_α
.Lx174_0:
                        lea              rdi, [rbp + 464]
                        lea              rsi, [rbp + 592]
                        lea              rdx, [rbp + 416]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n137_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n134_op75_α:
                        mov              eax, dword ptr [rbp + 576]
                        cmp              eax, 7
                                                                                        je    .Lx176_1
                        cmp              eax, 6
                                                                                        jne   .Lx176_0
                        mov              eax, dword ptr [rbp + 592]
                        cmp              eax, 6
                                                                                        jne   .Lx176_0
.Lx176_1:
                        mov              rax, qword ptr [rbp + 576]
                        mov              qword ptr [rbp + 336], rax
                        mov              rax, qword ptr [rbp + 584]
                        mov              qword ptr [rbp + 344], rax
                                                                                        jmp   n138_binop_α
.Lx176_0:
                        lea              rdi, [rbp + 576]
                        lea              rsi, [rbp + 592]
                        lea              rdx, [rbp + 336]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n138_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n135_unmark_α:
                        mov              rsp, qword ptr [rbp + 160]
                                                                                        jmp   n118_proc_gen_β
#-----------------------------------------------------------------------------------------------------------------------
n136_var_α:
                        mov              rax, qword ptr [rbp + 576]
                        mov              qword ptr [rbp + 304], rax
                        mov              rax, qword ptr [rbp + 584]
                        mov              qword ptr [rbp + 312], rax
                                                                                        jmp   n139_binop_test_α
#-----------------------------------------------------------------------------------------------------------------------
n137_binop_α:
                        mov              eax, dword ptr [rbp + 432]
                        cmp              eax, 6
                                                                                        jne   .Lx181_0
                        mov              eax, dword ptr [rbp + 416]
                        cmp              eax, 6
                                                                                        jne   .Lx181_0
                        mov              rax, qword ptr [rbp + 440]
                        mov              rcx, qword ptr [rbp + 424]
                        add              rax, rcx
                        mov              qword ptr [rbp + 400], 6
                        mov              qword ptr [rbp + 408], rax
                                                                                        jmp   n140_assign_α
.Lx181_0:
                        mov              rdi, qword ptr [rbp + 432]
                        mov              rsi, qword ptr [rbp + 440]
                        mov              rdx, qword ptr [rbp + 416]
                        mov              rcx, qword ptr [rbp + 424]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        je    n125_var_α
                        mov              qword ptr [rbp + 400], rax
                        mov              qword ptr [rbp + 408], rdx
                                                                                        jmp   n140_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n138_binop_α:
                        mov              eax, dword ptr [rbp + 352]
                        cmp              eax, 6
                                                                                        jne   .Lx182_0
                        mov              eax, dword ptr [rbp + 336]
                        cmp              eax, 6
                                                                                        jne   .Lx182_0
                        mov              rax, qword ptr [rbp + 360]
                        mov              rcx, qword ptr [rbp + 344]
                        sub              rax, rcx
                        mov              qword ptr [rbp + 320], 6
                        mov              qword ptr [rbp + 328], rax
                                                                                        jmp   n141_assign_α
.Lx182_0:
                        mov              rdi, qword ptr [rbp + 352]
                        mov              rsi, qword ptr [rbp + 360]
                        mov              rdx, qword ptr [rbp + 336]
                        mov              rcx, qword ptr [rbp + 344]
                        call             rt_sub@PLT
                        cmp              eax, 99
                                                                                        je    n128_disjunction_α
                        mov              qword ptr [rbp + 320], rax
                        mov              qword ptr [rbp + 328], rdx
                                                                                        jmp   n141_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n139_binop_test_α:
                        mov              eax, dword ptr [rbp + 592]
                        cmp              eax, 100
                                                                                        je    .Lx183_0
                        mov              eax, dword ptr [rbp + 576]
                        cmp              eax, 100
                                                                                        je    .Lx183_0
                        mov              eax, dword ptr [rbp + 592]
                        cmp              eax, 6
                                                                                        jne   .Lx183_2
                        mov              eax, dword ptr [rbp + 576]
                        cmp              eax, 6
                                                                                        jne   .Lx183_2
.Lx183_1:
                        mov              rax, qword ptr [rbp + 600]
                        mov              rcx, qword ptr [rbp + 584]
                        cmp              rax, rcx
                                                                                        jne   n128_disjunction_af
                        mov              rcx, qword ptr [rbp + 576]
                        mov              qword ptr [rbp + 272], rcx
                        mov              rcx, qword ptr [rbp + 584]
                        mov              qword ptr [rbp + 280], rcx
                                                                                        jmp   n142_var_α
.Lx183_0:
                        mov              rdi, qword ptr [rbp + 592]
                        mov              rsi, qword ptr [rbp + 600]
                        mov              rdx, qword ptr [rbp + 576]
                        mov              rcx, qword ptr [rbp + 584]
                        mov              r8d, 9
                        lea              r9, [rbp + 272]
                        call             rt_relop_overload@PLT
                        test             eax, eax
                                                                                        je    .Lx183_1
                        cmp              eax, 1
                                                                                        je    n128_disjunction_af
                                                                                        jmp   n142_var_α
.Lx183_2:
                        mov              rdi, qword ptr [rbp + 592]
                        mov              rsi, qword ptr [rbp + 600]
                        mov              rdx, qword ptr [rbp + 576]
                        mov              rcx, qword ptr [rbp + 584]
                        mov              r8d, 9
                        call             rt_jct_relop@PLT
                        test             eax, eax
                                                                                        jz    n128_disjunction_af
                        mov              rax, qword ptr [rbp + 576]
                        mov              qword ptr [rbp + 272], rax
                        mov              rax, qword ptr [rbp + 584]
                        mov              qword ptr [rbp + 280], rax
                                                                                        jmp   n142_var_α
#-----------------------------------------------------------------------------------------------------------------------
n140_assign_α:
                        mov              rax, qword ptr [rbp + 400]
                        mov              rdx, qword ptr [rbp + 408]
                        mov              qword ptr [rbp + 592], rax
                        mov              qword ptr [rbp + 600], rdx
                                                                                        jmp   n126_proc_gen_β
#-----------------------------------------------------------------------------------------------------------------------
n141_assign_α:
                        mov              rax, qword ptr [rbp + 320]
                        mov              rdx, qword ptr [rbp + 328]
                        mov              qword ptr [rbp + 592], rax
                        mov              qword ptr [rbp + 600], rdx
                                                                                        jmp   n128_disjunction_α
#-----------------------------------------------------------------------------------------------------------------------
n142_var_α:
                        mov              rax, qword ptr [rbp + 576]
                        mov              qword ptr [rbp + 256], rax
                        mov              rax, qword ptr [rbp + 584]
                        mov              qword ptr [rbp + 264], rax
                                                                                        jmp   n143_suspend_α
#-----------------------------------------------------------------------------------------------------------------------
n143_suspend_α:
                        lea              rax, [rip + n143_suspend_β]
                        mov              qword ptr [rbp + 560], rax
                        mov              rax, qword ptr [rbp + 256]
                        mov              qword ptr [rbp + 0], rax
                        mov              rax, qword ptr [rbp + 264]
                        mov              qword ptr [rbp + 8], rax
                                                                                        jmp   proc_perfect_γ
n143_suspend_β:
                                                                                        jmp   n128_disjunction_β
#-----------------------------------------------------------------------------------------------------------------------
proc_perfect_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_perfect_β:
                                                                                        jmp   qword ptr [rbp + 560]
#-----------------------------------------------------------------------------------------------------------------------
proc_perfect_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        push             rbp
                        lea              rax, [rip + proc_perfect_res]
                        push             rax
                        mov              rax, [rbp + 616]
                        mov              rbp, [rbp + 632]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_perfect_ω:
                        mov              rax, [rbp + 624]
                        lea              rsp, [rbp + 640]
                        mov              rbp, [rbp + 632]
                                                                                        jmp   rax
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lstartup_pname0:       .string          "range"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_range_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 2
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 224
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_generator@PLT
                        .section         .rodata
.Lstartup_pname1:       .string          "divisors"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname1]
                        lea              rsi, [rip + proc_divisors_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 1
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 352
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 1
                        call             rt_proc_set_generator@PLT
                        .section         .rodata
.Lstartup_pname2:       .string          "fibs"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname2]
                        lea              rsi, [rip + proc_fibs_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 1
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 352
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 1
                        call             rt_proc_set_generator@PLT
                        .section         .rodata
.Lstartup_pname3:       .string          "perfect"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname3]
                        lea              rsi, [rip + proc_perfect_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 1
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 608
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 1
                        call             rt_proc_set_generator@PLT
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
                        sub              rsp, 1416
                        mov              rdi, rsp
                        mov              ecx, 1416
                        xor              eax, eax
                        rep stosb
                        mov              [rsp + 1408], rbp
                        mov              rbp, rsp
main_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n190_lit_string_α:
                        mov              qword ptr [rbp + 1360], 1
                        mov              rax, qword ptr [rip + .Lx252_0]
                        mov              qword ptr [rbp + 1368], rax
                                                                                        jmp   n191_call_builtin_icon_α
.Lx252_0:
                        .quad            .Lx252_0_s
.Lx252_0_s:
                        .string          "Fibs to 100:"
#-----------------------------------------------------------------------------------------------------------------------
n191_call_builtin_icon_α:
                        mov              rax, qword ptr [rbp + 1360]
                        mov              qword ptr [rbp + 1328], rax
                        mov              rax, qword ptr [rbp + 1368]
                        mov              qword ptr [rbp + 1336], rax
                        .section         .rodata
.Lrkfn254:              .string          "write"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn254]
                        lea              rsi, [rbp + 1328]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1312], rax
                        mov              qword ptr [rbp + 1320], rdx
                        cmp              eax, 99
                                                                                        je    n192_lit_string_α
                                                                                        jmp   n192_lit_string_α
n191_call_builtin_icon_β:
                                                                                        jmp   n192_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n192_lit_string_α:
                        mov              qword ptr [rbp + 1296], 1
                        mov              rax, qword ptr [rip + .Lx255_0]
                        mov              qword ptr [rbp + 1304], rax
                                                                                        jmp   n193_assign_α
.Lx255_0:
                        .quad            .Lx255_0_s
.Lx255_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n193_assign_α:
                        mov              rax, qword ptr [rbp + 1296]
                        mov              rdx, qword ptr [rbp + 1304]
                        mov              qword ptr [rbp + 1376], rax
                        mov              qword ptr [rbp + 1384], rdx
                                                                                        jmp   n194_var_α
#-----------------------------------------------------------------------------------------------------------------------
n194_var_α:
                        mov              rax, qword ptr [rbp + 1376]
                        mov              qword ptr [rbp + 1168], rax
                        mov              rax, qword ptr [rbp + 1384]
                        mov              qword ptr [rbp + 1176], rax
                                                                                        jmp   n195_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n195_lit_string_α:
                        mov              qword ptr [rbp + 1200], 1
                        mov              rax, qword ptr [rip + .Lx259_0]
                        mov              qword ptr [rbp + 1208], rax
                                                                                        jmp   n197_lit_integer_α
.Lx259_0:
                        .quad            .Lx259_0_s
.Lx259_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n196_lit_string_α:
                        mov              qword ptr [rbp + 1072], 1
                        mov              rax, qword ptr [rip + .Lx260_0]
                        mov              qword ptr [rbp + 1080], rax
                                                                                        jmp   n198_var_α
.Lx260_0:
                        .quad            .Lx260_0_s
.Lx260_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n197_lit_integer_α:
                        mov              qword ptr [rbp + 1280], 6
                        mov              rax, qword ptr [rip + .Lx261_0]
                        mov              qword ptr [rbp + 1288], rax
                                                                                        jmp   n199_proc_gen_α
.Lx261_0:
                        .quad            100
#-----------------------------------------------------------------------------------------------------------------------
n198_var_α:
                        mov              rax, qword ptr [rbp + 1376]
                        mov              qword ptr [rbp + 1104], rax
                        mov              rax, qword ptr [rbp + 1384]
                        mov              qword ptr [rbp + 1112], rax
                                                                                        jmp   n200_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n199_proc_gen_α:
                        mov              qword ptr [rbp + 1248], 0
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx265_20
                        mov              rax, qword ptr [rbp + 1280]
                        mov              rdx, qword ptr [rbp + 1288]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx265_21
.Lx265_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rbp + 1280]
                        mov              rdx, qword ptr [rbp + 1288]
                        call             rt_arg_stage@PLT
.Lx265_21:
                        mov              edi, 2
                        mov              esi, 1
                        call             rt_proc_call_open_det@PLT
                        test             rax, rax
                                                                                        je    .Lx265_1
                        lea              rcx, [rip + .Lx265_3]
                        lea              rdx, [rip + .Lx265_4]
                                                                                        jmp   rax
.Lx265_3:
                        mov              qword ptr [rbp + 1256], rsp
                        mov              rax, qword ptr [rbp + 1248]
                        test             rax, rax
                                                                                        jne   .Lx265_5
                        mov              qword ptr [rbp + 1248], 1
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx265_2
.Lx265_5:
                        call             rt_gen_spine_pass_γ@PLT
                                                                                        jmp   .Lx265_2
.Lx265_4:
                        mov              rax, qword ptr [rbp + 1248]
                        test             rax, rax
                                                                                        jne   .Lx265_6
                        mov              qword ptr [rbp + 1248], 1
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx265_2
.Lx265_6:
                        call             rt_gen_spine_pass_ω@PLT
                                                                                        jmp   .Lx265_2
.Lx265_1:
                        call             rt_faildescr@PLT
.Lx265_2:
                        mov              qword ptr [rbp + 1216], rax
                        mov              qword ptr [rbp + 1224], rdx
                        cmp              eax, 99
                                                                                        je    n196_lit_string_α
                                                                                        jmp   n202_binop_α
n199_proc_gen_β:
                        call             rt_gen_spine_resume_enter@PLT
                        mov              rsp, qword ptr [rbp + 1256]
                                                                                        jmp   qword ptr [rsp]
.Lx265_0:
                        .quad            .Lx265_0_s
.Lx265_0_s:
                        .string          "fibs"
#-----------------------------------------------------------------------------------------------------------------------
n200_lit_integer_α:
                        mov              qword ptr [rbp + 1120], 6
                        mov              rax, qword ptr [rip + .Lx266_0]
                        mov              qword ptr [rbp + 1128], rax
                                                                                        jmp   n203_lit_integer_α
.Lx266_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n201_lit_string_α:
                        mov              qword ptr [rbp + 992], 1
                        mov              rax, qword ptr [rip + .Lx267_0]
                        mov              qword ptr [rbp + 1000], rax
                                                                                        jmp   n204_call_builtin_icon_α
.Lx267_0:
                        .quad            .Lx267_0_s
.Lx267_0_s:
                        .string          "Perfect numbers to 10000:"
#-----------------------------------------------------------------------------------------------------------------------
n202_binop_α:
                        mov              rdi, qword ptr [rbp + 1200]
                        mov              rsi, qword ptr [rbp + 1208]
                        mov              rdx, qword ptr [rbp + 1216]
                        mov              rcx, qword ptr [rbp + 1224]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 1184], rax
                        mov              qword ptr [rbp + 1192], rdx
                                                                                        jmp   n205_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n203_lit_integer_α:
                        mov              qword ptr [rbp + 1136], 6
                        mov              rax, qword ptr [rip + .Lx269_0]
                        mov              qword ptr [rbp + 1144], rax
                                                                                        jmp   n206_subscript_α
.Lx269_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n204_call_builtin_icon_α:
                        mov              rax, qword ptr [rbp + 992]
                        mov              qword ptr [rbp + 960], rax
                        mov              rax, qword ptr [rbp + 1000]
                        mov              qword ptr [rbp + 968], rax
                        .section         .rodata
.Lrkfn271:              .string          "write"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn271]
                        lea              rsi, [rbp + 960]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 944], rax
                        mov              qword ptr [rbp + 952], rdx
                        cmp              eax, 99
                                                                                        je    n207_lit_string_α
                                                                                        jmp   n207_lit_string_α
n204_call_builtin_icon_β:
                                                                                        jmp   n207_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n205_binop_α:
                        mov              rdi, qword ptr [rbp + 1376]
                        mov              rsi, qword ptr [rbp + 1384]
                        mov              rdx, qword ptr [rbp + 1184]
                        mov              rcx, qword ptr [rbp + 1192]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 1152], rax
                        mov              qword ptr [rbp + 1160], rdx
                                                                                        jmp   n208_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n206_subscript_α:
                        mov              rdi, qword ptr [rbp + 1104]
                        mov              rsi, qword ptr [rbp + 1112]
                        mov              rdx, qword ptr [rbp + 1120]
                        mov              rcx, qword ptr [rbp + 1128]
                        mov              r8, qword ptr [rbp + 1136]
                        mov              r9, qword ptr [rbp + 1144]
                        call             subscript_get2@PLT
                        cmp              eax, 99
                                                                                        je    n201_lit_string_α
                        mov              qword ptr [rbp + 1088], rax
                        mov              qword ptr [rbp + 1096], rdx
                                                                                        jmp   n209_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n207_lit_string_α:
                        mov              qword ptr [rbp + 848], 1
                        mov              rax, qword ptr [rip + .Lx274_0]
                        mov              qword ptr [rbp + 856], rax
                                                                                        jmp   n210_lit_integer_α
.Lx274_0:
                        .quad            .Lx274_0_s
.Lx274_0_s:
                        .string          "  "
#-----------------------------------------------------------------------------------------------------------------------
n208_assign_α:
                        mov              rax, qword ptr [rbp + 1152]
                        mov              rdx, qword ptr [rbp + 1160]
                        mov              qword ptr [rbp + 1376], rax
                        mov              qword ptr [rbp + 1384], rdx
                                                                                        jmp   n199_proc_gen_β
#-----------------------------------------------------------------------------------------------------------------------
n209_binop_α:
                        mov              rdi, qword ptr [rbp + 1072]
                        mov              rsi, qword ptr [rbp + 1080]
                        mov              rdx, qword ptr [rbp + 1088]
                        mov              rcx, qword ptr [rbp + 1096]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 1056], rax
                        mov              qword ptr [rbp + 1064], rdx
                                                                                        jmp   n211_call_builtin_icon_α
#-----------------------------------------------------------------------------------------------------------------------
n210_lit_integer_α:
                        mov              qword ptr [rbp + 928], 6
                        mov              rax, qword ptr [rip + .Lx277_0]
                        mov              qword ptr [rbp + 936], rax
                                                                                        jmp   n212_proc_gen_α
.Lx277_0:
                        .quad            10000
#-----------------------------------------------------------------------------------------------------------------------
n211_call_builtin_icon_α:
                        mov              rax, qword ptr [rbp + 1056]
                        mov              qword ptr [rbp + 1024], rax
                        mov              rax, qword ptr [rbp + 1064]
                        mov              qword ptr [rbp + 1032], rax
                        .section         .rodata
.Lrkfn279:              .string          "write"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn279]
                        lea              rsi, [rbp + 1024]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1008], rax
                        mov              qword ptr [rbp + 1016], rdx
                        cmp              eax, 99
                                                                                        je    n201_lit_string_α
                                                                                        jmp   n201_lit_string_α
n211_call_builtin_icon_β:
                                                                                        jmp   n201_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n212_proc_gen_α:
                        mov              qword ptr [rbp + 896], 0
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx281_20
                        mov              rax, qword ptr [rbp + 928]
                        mov              rdx, qword ptr [rbp + 936]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx281_21
.Lx281_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rbp + 928]
                        mov              rdx, qword ptr [rbp + 936]
                        call             rt_arg_stage@PLT
.Lx281_21:
                        mov              edi, 3
                        mov              esi, 1
                        call             rt_proc_call_open_det@PLT
                        test             rax, rax
                                                                                        je    .Lx281_1
                        lea              rcx, [rip + .Lx281_3]
                        lea              rdx, [rip + .Lx281_4]
                                                                                        jmp   rax
.Lx281_3:
                        mov              qword ptr [rbp + 904], rsp
                        mov              rax, qword ptr [rbp + 896]
                        test             rax, rax
                                                                                        jne   .Lx281_5
                        mov              qword ptr [rbp + 896], 1
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx281_2
.Lx281_5:
                        call             rt_gen_spine_pass_γ@PLT
                                                                                        jmp   .Lx281_2
.Lx281_4:
                        mov              rax, qword ptr [rbp + 896]
                        test             rax, rax
                                                                                        jne   .Lx281_6
                        mov              qword ptr [rbp + 896], 1
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx281_2
.Lx281_6:
                        call             rt_gen_spine_pass_ω@PLT
                                                                                        jmp   .Lx281_2
.Lx281_1:
                        call             rt_faildescr@PLT
.Lx281_2:
                        mov              qword ptr [rbp + 864], rax
                        mov              qword ptr [rbp + 872], rdx
                        cmp              eax, 99
                                                                                        je    n214_lit_string_α
                                                                                        jmp   n213_binop_α
n212_proc_gen_β:
                        call             rt_gen_spine_resume_enter@PLT
                        mov              rsp, qword ptr [rbp + 904]
                                                                                        jmp   qword ptr [rsp]
.Lx281_0:
                        .quad            .Lx281_0_s
.Lx281_0_s:
                        .string          "perfect"
#-----------------------------------------------------------------------------------------------------------------------
n213_binop_α:
                        mov              rdi, qword ptr [rbp + 848]
                        mov              rsi, qword ptr [rbp + 856]
                        mov              rdx, qword ptr [rbp + 864]
                        mov              rcx, qword ptr [rbp + 872]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 832], rax
                        mov              qword ptr [rbp + 840], rdx
                                                                                        jmp   n215_call_builtin_icon_α
#-----------------------------------------------------------------------------------------------------------------------
n214_lit_string_α:
                        mov              qword ptr [rbp + 768], 1
                        mov              rax, qword ptr [rip + .Lx283_0]
                        mov              qword ptr [rbp + 776], rax
                                                                                        jmp   n216_call_builtin_icon_α
.Lx283_0:
                        .quad            .Lx283_0_s
.Lx283_0_s:
                        .string          "Primes to 50 (trial division by generators):"
#-----------------------------------------------------------------------------------------------------------------------
n215_call_builtin_icon_α:
                        mov              rax, qword ptr [rbp + 832]
                        mov              qword ptr [rbp + 800], rax
                        mov              rax, qword ptr [rbp + 840]
                        mov              qword ptr [rbp + 808], rax
                        .section         .rodata
.Lrkfn285:              .string          "write"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn285]
                        lea              rsi, [rbp + 800]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 784], rax
                        mov              qword ptr [rbp + 792], rdx
                        cmp              eax, 99
                                                                                        je    n212_proc_gen_β
                                                                                        jmp   n212_proc_gen_β
n215_call_builtin_icon_β:
                                                                                        jmp   n212_proc_gen_β
#-----------------------------------------------------------------------------------------------------------------------
n216_call_builtin_icon_α:
                        mov              rax, qword ptr [rbp + 768]
                        mov              qword ptr [rbp + 736], rax
                        mov              rax, qword ptr [rbp + 776]
                        mov              qword ptr [rbp + 744], rax
                        .section         .rodata
.Lrkfn287:              .string          "write"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn287]
                        lea              rsi, [rbp + 736]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 720], rax
                        mov              qword ptr [rbp + 728], rdx
                        cmp              eax, 99
                                                                                        je    n217_lit_string_α
                                                                                        jmp   n217_lit_string_α
n216_call_builtin_icon_β:
                                                                                        jmp   n217_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n217_lit_string_α:
                        mov              qword ptr [rbp + 704], 1
                        mov              rax, qword ptr [rip + .Lx288_0]
                        mov              qword ptr [rbp + 712], rax
                                                                                        jmp   n218_assign_α
.Lx288_0:
                        .quad            .Lx288_0_s
.Lx288_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n218_assign_α:
                        mov              rax, qword ptr [rbp + 704]
                        mov              rdx, qword ptr [rbp + 712]
                        mov              qword ptr [rbp + 1376], rax
                        mov              qword ptr [rbp + 1384], rdx
                                                                                        jmp   n219_var_α
#-----------------------------------------------------------------------------------------------------------------------
n219_var_α:
                        mov              rax, qword ptr [rbp + 1376]
                        mov              qword ptr [rbp + 176], rax
                        mov              rax, qword ptr [rbp + 1384]
                        mov              qword ptr [rbp + 184], rax
                                                                                        jmp   n220_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n220_lit_string_α:
                        mov              qword ptr [rbp + 208], 1
                        mov              rax, qword ptr [rip + .Lx292_0]
                        mov              qword ptr [rbp + 216], rax
                                                                                        jmp   n222_lit_integer_α
.Lx292_0:
                        .quad            .Lx292_0_s
.Lx292_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n221_lit_string_α:
                        mov              qword ptr [rbp + 64], 1
                        mov              rax, qword ptr [rip + .Lx293_0]
                        mov              qword ptr [rbp + 72], rax
                                                                                        jmp   n223_var_α
.Lx293_0:
                        .quad            .Lx293_0_s
.Lx293_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n222_lit_integer_α:
                        mov              qword ptr [rbp + 672], 6
                        mov              rax, qword ptr [rip + .Lx294_0]
                        mov              qword ptr [rbp + 680], rax
                                                                                        jmp   n224_lit_integer_α
.Lx294_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n223_var_α:
                        mov              rax, qword ptr [rbp + 1376]
                        mov              qword ptr [rbp + 96], rax
                        mov              rax, qword ptr [rbp + 1384]
                        mov              qword ptr [rbp + 104], rax
                                                                                        jmp   n225_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n224_lit_integer_α:
                        mov              qword ptr [rbp + 688], 6
                        mov              rax, qword ptr [rip + .Lx297_0]
                        mov              qword ptr [rbp + 696], rax
                                                                                        jmp   n226_proc_gen_α
.Lx297_0:
                        .quad            50
#-----------------------------------------------------------------------------------------------------------------------
n225_lit_integer_α:
                        mov              qword ptr [rbp + 112], 6
                        mov              rax, qword ptr [rip + .Lx298_0]
                        mov              qword ptr [rbp + 120], rax
                                                                                        jmp   n227_lit_integer_α
.Lx298_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n226_proc_gen_α:
                        mov              qword ptr [rbp + 640], 0
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx300_20
                        mov              rax, qword ptr [rbp + 672]
                        mov              rdx, qword ptr [rbp + 680]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx300_21
.Lx300_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rbp + 672]
                        mov              rdx, qword ptr [rbp + 680]
                        call             rt_arg_stage@PLT
.Lx300_21:
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx300_22
                        mov              rax, qword ptr [rbp + 688]
                        mov              rdx, qword ptr [rbp + 696]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 16], rax
                        mov              qword ptr [r10 + 24], rdx
                                                                                        jmp   .Lx300_23
.Lx300_22:
                        mov              edi, 1
                        mov              rsi, qword ptr [rbp + 688]
                        mov              rdx, qword ptr [rbp + 696]
                        call             rt_arg_stage@PLT
.Lx300_23:
                        mov              edi, 0
                        mov              esi, 2
                        call             rt_proc_call_open_det@PLT
                        test             rax, rax
                                                                                        je    .Lx300_1
                        lea              rcx, [rip + .Lx300_3]
                        lea              rdx, [rip + .Lx300_4]
                                                                                        jmp   rax
.Lx300_3:
                        mov              qword ptr [rbp + 648], rsp
                        mov              rax, qword ptr [rbp + 640]
                        test             rax, rax
                                                                                        jne   .Lx300_5
                        mov              qword ptr [rbp + 640], 1
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx300_2
.Lx300_5:
                        call             rt_gen_spine_pass_γ@PLT
                                                                                        jmp   .Lx300_2
.Lx300_4:
                        mov              rax, qword ptr [rbp + 640]
                        test             rax, rax
                                                                                        jne   .Lx300_6
                        mov              qword ptr [rbp + 640], 1
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx300_2
.Lx300_6:
                        call             rt_gen_spine_pass_ω@PLT
                                                                                        jmp   .Lx300_2
.Lx300_1:
                        call             rt_faildescr@PLT
.Lx300_2:
                        mov              qword ptr [rbp + 592], rax
                        mov              qword ptr [rbp + 600], rdx
                        cmp              eax, 99
                                                                                        je    n229_lit_integer_α
                                                                                        jmp   n228_assign_α
n226_proc_gen_β:
                        call             rt_gen_spine_resume_enter@PLT
                        mov              rsp, qword ptr [rbp + 648]
                                                                                        jmp   qword ptr [rsp]
.Lx300_0:
                        .quad            .Lx300_0_s
.Lx300_0_s:
                        .string          "range"
#-----------------------------------------------------------------------------------------------------------------------
n227_lit_integer_α:
                        mov              qword ptr [rbp + 128], 6
                        mov              rax, qword ptr [rip + .Lx301_0]
                        mov              qword ptr [rbp + 136], rax
                                                                                        jmp   n230_subscript_α
.Lx301_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n228_assign_α:
                        mov              rax, qword ptr [rbp + 592]
                        mov              rdx, qword ptr [rbp + 600]
                        mov              qword ptr [rbp + 1392], rax
                        mov              qword ptr [rbp + 1400], rdx
                                                                                        jmp   n229_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n229_lit_integer_α:
                        mov              qword ptr [rbp + 304], 6
                        mov              rax, qword ptr [rip + .Lx303_0]
                        mov              qword ptr [rbp + 312], rax
                                                                                        jmp   n231_var_α
.Lx303_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n230_subscript_α:
                        mov              rdi, qword ptr [rbp + 96]
                        mov              rsi, qword ptr [rbp + 104]
                        mov              rdx, qword ptr [rbp + 112]
                        mov              rcx, qword ptr [rbp + 120]
                        mov              r8, qword ptr [rbp + 128]
                        mov              r9, qword ptr [rbp + 136]
                        call             subscript_get2@PLT
                        cmp              eax, 99
                                                                                        je    main_ω
                        mov              qword ptr [rbp + 80], rax
                        mov              qword ptr [rbp + 88], rdx
                                                                                        jmp   n232_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n231_var_α:
                        mov              rax, qword ptr [rbp + 1392]
                        mov              qword ptr [rbp + 368], rax
                        mov              rax, qword ptr [rbp + 1400]
                        mov              qword ptr [rbp + 376], rax
                                                                                        jmp   n233_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n232_binop_α:
                        mov              rdi, qword ptr [rbp + 64]
                        mov              rsi, qword ptr [rbp + 72]
                        mov              rdx, qword ptr [rbp + 80]
                        mov              rcx, qword ptr [rbp + 88]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 48], rax
                        mov              qword ptr [rbp + 56], rdx
                                                                                        jmp   n235_call_builtin_icon_α
#-----------------------------------------------------------------------------------------------------------------------
n233_lit_integer_α:
                        mov              qword ptr [rbp + 464], 6
                        mov              rax, qword ptr [rip + .Lx308_0]
                        mov              qword ptr [rbp + 472], rax
                                                                                        jmp   n236_var_α
.Lx308_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n234_var_α:
                        mov              qword ptr [rbp + 272], 0
                        mov              qword ptr [rbp + 280], 0
                                                                                        jmp   n237_var_α
n234_var_β:
                                                                                        jmp   n221_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n235_call_builtin_icon_α:
                        mov              rax, qword ptr [rbp + 48]
                        mov              qword ptr [rbp + 16], rax
                        mov              rax, qword ptr [rbp + 56]
                        mov              qword ptr [rbp + 24], rax
                        .section         .rodata
.Lrkfn311:              .string          "write"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn311]
                        lea              rsi, [rbp + 16]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 0], rax
                        mov              qword ptr [rbp + 8], rdx
                        cmp              eax, 99
                                                                                        je    main_ω
                                                                                        jmp   main_ω
n235_call_builtin_icon_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n236_var_α:
                        mov              rax, qword ptr [rbp + 1392]
                        mov              qword ptr [rbp + 560], rax
                        mov              rax, qword ptr [rbp + 1400]
                        mov              qword ptr [rbp + 568], rax
                                                                                        jmp   n238_lit_real_α
#-----------------------------------------------------------------------------------------------------------------------
n237_var_α:
                        mov              rax, qword ptr [rbp + 1392]
                        mov              qword ptr [rbp + 256], rax
                        mov              rax, qword ptr [rbp + 1400]
                        mov              qword ptr [rbp + 264], rax
                                                                                        jmp   n239_conjunction_α
#-----------------------------------------------------------------------------------------------------------------------
n238_lit_real_α:
                        mov              qword ptr [rbp + 576], 7
                        mov              rax, qword ptr [rip + .Lx316_0]
                        mov              qword ptr [rbp + 584], rax
                                                                                        jmp   n240_op75_α
.Lx316_0:
                        .quad            4602678819172646912
#-----------------------------------------------------------------------------------------------------------------------
n239_conjunction_α:
                        mov              rax, qword ptr [rbp + 256]
                        mov              qword ptr [rbp + 240], rax
                        mov              rax, qword ptr [rbp + 264]
                        mov              qword ptr [rbp + 248], rax
                                                                                        jmp   n241_conjunction_α
n239_conjunction_β:
                                                                                        jmp   n221_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n240_op75_α:
                        mov              eax, dword ptr [rbp + 1392]
                        cmp              eax, 7
                                                                                        je    .Lx319_1
                        cmp              eax, 6
                                                                                        jne   .Lx319_0
                        mov              eax, dword ptr [rbp + 576]
                        cmp              eax, 6
                                                                                        jne   .Lx319_0
.Lx319_1:
                        mov              rax, qword ptr [rbp + 1392]
                        mov              qword ptr [rbp + 544], rax
                        mov              rax, qword ptr [rbp + 1400]
                        mov              qword ptr [rbp + 552], rax
                                                                                        jmp   n242_binop_α
.Lx319_0:
                        lea              rdi, [rbp + 1392]
                        lea              rsi, [rbp + 576]
                        lea              rdx, [rbp + 544]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n242_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n241_conjunction_α:
                        mov              rax, qword ptr [rbp + 256]
                        mov              qword ptr [rbp + 224], rax
                        mov              rax, qword ptr [rbp + 264]
                        mov              qword ptr [rbp + 232], rax
                                                                                        jmp   n243_binop_α
n241_conjunction_β:
                                                                                        jmp   n221_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n242_binop_α:
                        mov              rdi, qword ptr [rbp + 544]
                        mov              rsi, qword ptr [rbp + 552]
                        mov              rdx, qword ptr [rbp + 576]
                        mov              rcx, qword ptr [rbp + 584]
                        call             rt_pow@PLT
                        cmp              eax, 99
                                                                                        je    n234_var_α
                        mov              qword ptr [rbp + 528], rax
                        mov              qword ptr [rbp + 536], rdx
                                                                                        jmp   n244_call_builtin_icon_α
#-----------------------------------------------------------------------------------------------------------------------
n243_binop_α:
                        mov              rdi, qword ptr [rbp + 208]
                        mov              rsi, qword ptr [rbp + 216]
                        mov              rdx, qword ptr [rbp + 240]
                        mov              rcx, qword ptr [rbp + 248]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 192], rax
                        mov              qword ptr [rbp + 200], rdx
                                                                                        jmp   n245_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n244_call_builtin_icon_α:
                        mov              rax, qword ptr [rbp + 528]
                        mov              qword ptr [rbp + 496], rax
                        mov              rax, qword ptr [rbp + 536]
                        mov              qword ptr [rbp + 504], rax
                        .section         .rodata
.Lrkfn324:              .string          "integer"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn324]
                        lea              rsi, [rbp + 496]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 480], rax
                        mov              qword ptr [rbp + 488], rdx
                        cmp              eax, 99
                                                                                        je    n234_var_α
                                                                                        jmp   n246_proc_gen_α
n244_call_builtin_icon_β:
                                                                                        jmp   n234_var_α
#-----------------------------------------------------------------------------------------------------------------------
n245_binop_α:
                        mov              rdi, qword ptr [rbp + 1376]
                        mov              rsi, qword ptr [rbp + 1384]
                        mov              rdx, qword ptr [rbp + 192]
                        mov              rcx, qword ptr [rbp + 200]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 160], rax
                        mov              qword ptr [rbp + 168], rdx
                                                                                        jmp   n247_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n246_proc_gen_α:
                        mov              qword ptr [rbp + 432], 0
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx327_20
                        mov              rax, qword ptr [rbp + 464]
                        mov              rdx, qword ptr [rbp + 472]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx327_21
.Lx327_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rbp + 464]
                        mov              rdx, qword ptr [rbp + 472]
                        call             rt_arg_stage@PLT
.Lx327_21:
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx327_22
                        mov              rax, qword ptr [rbp + 480]
                        mov              rdx, qword ptr [rbp + 488]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 16], rax
                        mov              qword ptr [r10 + 24], rdx
                                                                                        jmp   .Lx327_23
.Lx327_22:
                        mov              edi, 1
                        mov              rsi, qword ptr [rbp + 480]
                        mov              rdx, qword ptr [rbp + 488]
                        call             rt_arg_stage@PLT
.Lx327_23:
                        mov              edi, 0
                        mov              esi, 2
                        call             rt_proc_call_open_det@PLT
                        test             rax, rax
                                                                                        je    .Lx327_1
                        lea              rcx, [rip + .Lx327_3]
                        lea              rdx, [rip + .Lx327_4]
                                                                                        jmp   rax
.Lx327_3:
                        mov              qword ptr [rbp + 440], rsp
                        mov              rax, qword ptr [rbp + 432]
                        test             rax, rax
                                                                                        jne   .Lx327_5
                        mov              qword ptr [rbp + 432], 1
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx327_2
.Lx327_5:
                        call             rt_gen_spine_pass_γ@PLT
                                                                                        jmp   .Lx327_2
.Lx327_4:
                        mov              rax, qword ptr [rbp + 432]
                        test             rax, rax
                                                                                        jne   .Lx327_6
                        mov              qword ptr [rbp + 432], 1
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx327_2
.Lx327_6:
                        call             rt_gen_spine_pass_ω@PLT
                                                                                        jmp   .Lx327_2
.Lx327_1:
                        call             rt_faildescr@PLT
.Lx327_2:
                        mov              qword ptr [rbp + 384], rax
                        mov              qword ptr [rbp + 392], rdx
                        cmp              eax, 99
                                                                                        je    n234_var_α
                                                                                        jmp   n248_op75_α
n246_proc_gen_β:
                        call             rt_gen_spine_resume_enter@PLT
                        mov              rsp, qword ptr [rbp + 440]
                                                                                        jmp   qword ptr [rsp]
.Lx327_0:
                        .quad            .Lx327_0_s
.Lx327_0_s:
                        .string          "range"
#-----------------------------------------------------------------------------------------------------------------------
n247_assign_α:
                        mov              rax, qword ptr [rbp + 160]
                        mov              rdx, qword ptr [rbp + 168]
                        mov              qword ptr [rbp + 1376], rax
                        mov              qword ptr [rbp + 1384], rdx
                                                                                        jmp   n221_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n248_op75_α:
                        mov              eax, dword ptr [rbp + 1392]
                        cmp              eax, 7
                                                                                        je    .Lx330_1
                        cmp              eax, 6
                                                                                        jne   .Lx330_0
                        mov              eax, dword ptr [rbp + 384]
                        cmp              eax, 6
                                                                                        jne   .Lx330_0
.Lx330_1:
                        mov              rax, qword ptr [rbp + 1392]
                        mov              qword ptr [rbp + 352], rax
                        mov              rax, qword ptr [rbp + 1400]
                        mov              qword ptr [rbp + 360], rax
                                                                                        jmp   n249_op75_α
.Lx330_0:
                        lea              rdi, [rbp + 1392]
                        lea              rsi, [rbp + 384]
                        lea              rdx, [rbp + 352]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n249_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n249_op75_α:
                        mov              eax, dword ptr [rbp + 384]
                        cmp              eax, 7
                                                                                        je    .Lx332_1
                        cmp              eax, 6
                                                                                        jne   .Lx332_0
                        mov              eax, dword ptr [rbp + 1392]
                        cmp              eax, 6
                                                                                        jne   .Lx332_0
.Lx332_1:
                        mov              rax, qword ptr [rbp + 384]
                        mov              qword ptr [rbp + 336], rax
                        mov              rax, qword ptr [rbp + 392]
                        mov              qword ptr [rbp + 344], rax
                                                                                        jmp   n250_binop_α
.Lx332_0:
                        lea              rdi, [rbp + 384]
                        lea              rsi, [rbp + 1392]
                        lea              rdx, [rbp + 336]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n250_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n250_binop_α:
                        mov              rdi, qword ptr [rbp + 352]
                        mov              rsi, qword ptr [rbp + 360]
                        mov              rdx, qword ptr [rbp + 336]
                        mov              rcx, qword ptr [rbp + 344]
                        call             rt_mod@PLT
                        cmp              eax, 99
                                                                                        je    n234_var_α
                        mov              qword ptr [rbp + 320], rax
                        mov              qword ptr [rbp + 328], rdx
                                                                                        jmp   n251_binop_test_α
#-----------------------------------------------------------------------------------------------------------------------
n251_binop_test_α:
                        mov              eax, dword ptr [rbp + 304]
                        cmp              eax, 100
                                                                                        je    .Lx334_0
                        mov              eax, dword ptr [rbp + 320]
                        cmp              eax, 100
                                                                                        je    .Lx334_0
                        mov              eax, dword ptr [rbp + 304]
                        cmp              eax, 6
                                                                                        jne   .Lx334_2
                        mov              eax, dword ptr [rbp + 320]
                        cmp              eax, 6
                                                                                        jne   .Lx334_2
.Lx334_1:
                        mov              rax, qword ptr [rbp + 312]
                        mov              rcx, qword ptr [rbp + 328]
                        cmp              rax, rcx
                                                                                        jne   n246_proc_gen_β
                        mov              rcx, qword ptr [rbp + 320]
                        mov              qword ptr [rbp + 288], rcx
                        mov              rcx, qword ptr [rbp + 328]
                        mov              qword ptr [rbp + 296], rcx
                                                                                        jmp   n221_lit_string_α
.Lx334_0:
                        mov              rdi, qword ptr [rbp + 304]
                        mov              rsi, qword ptr [rbp + 312]
                        mov              rdx, qword ptr [rbp + 320]
                        mov              rcx, qword ptr [rbp + 328]
                        mov              r8d, 9
                        lea              r9, [rbp + 288]
                        call             rt_relop_overload@PLT
                        test             eax, eax
                                                                                        je    .Lx334_1
                        cmp              eax, 1
                                                                                        je    n246_proc_gen_β
                                                                                        jmp   n221_lit_string_α
.Lx334_2:
                        mov              rdi, qword ptr [rbp + 304]
                        mov              rsi, qword ptr [rbp + 312]
                        mov              rdx, qword ptr [rbp + 320]
                        mov              rcx, qword ptr [rbp + 328]
                        mov              r8d, 9
                        call             rt_jct_relop@PLT
                        test             eax, eax
                                                                                        jz    n246_proc_gen_β
                        mov              rax, qword ptr [rbp + 320]
                        mov              qword ptr [rbp + 288], rax
                        mov              rax, qword ptr [rbp + 328]
                        mov              qword ptr [rbp + 296], rax
                                                                                        jmp   n221_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        mov              rsp, rbp
                        mov              rbp, [rsp + 1408]
                        add              rsp, 1416
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              rsp, rbp
                        mov              eax, 99
                        xor              edx, edx
                        mov              rbp, [rsp + 1408]
                        add              rsp, 1416
                        ret
                        .section         .note.GNU-stack,"",@progbits

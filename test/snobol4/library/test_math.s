                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_LBL__max_α
proc_LBL__max_α:
                        .global          proc_LBL__max_α
                        .global          proc_LBL__max_β
                        .global          proc_LBL__max_γ
                        .global          proc_LBL__max_ω
                        sub              rsp, 2112
                        mov              [rsp + 2088], rcx
                        mov              [rsp + 2096], rdx
                        mov              [rsp + 2104], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
proc_LBL__max_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n0_goto_α:
                                                                                        jmp   n1_var_α
n0_goto_β:
                                                                                        jmp   proc_LBL__max_ω
#-----------------------------------------------------------------------------------------------------------------------
n1_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 112], rax
                        mov              qword ptr [rsp + 120], rdx
                                                                                        jmp   n2_var_α
#-----------------------------------------------------------------------------------------------------------------------
n2_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 96], rax
                        mov              qword ptr [rsp + 104], rdx
                                                                                        jmp   n4_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n3_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n4_op75_α:
                        mov              eax, dword ptr [rsp + 112]
                        cmp              eax, 7
                                                                                        je    .Lx16_1
                        cmp              eax, 6
                                                                                        jne   .Lx16_0
                        mov              eax, dword ptr [rsp + 96]
                        cmp              eax, 6
                                                                                        jne   .Lx16_0
.Lx16_1:
                        mov              rax, qword ptr [rsp + 112]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [rsp + 120]
                        mov              qword ptr [rsp + 88], rax
                                                                                        jmp   n5_op75_α
.Lx16_0:
                        lea              rdi, [rsp + 112]
                        lea              rsi, [rsp + 96]
                        lea              rdx, [rsp + 80]
                        mov              rcx, 147
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n5_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n5_op75_α:
                        mov              eax, dword ptr [rsp + 96]
                        cmp              eax, 7
                                                                                        je    .Lx18_1
                        cmp              eax, 6
                                                                                        jne   .Lx18_0
                        mov              eax, dword ptr [rsp + 112]
                        cmp              eax, 6
                                                                                        jne   .Lx18_0
.Lx18_1:
                        mov              rax, qword ptr [rsp + 96]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 72], rax
                                                                                        jmp   n6_op77_α
.Lx18_0:
                        lea              rdi, [rsp + 96]
                        lea              rsi, [rsp + 112]
                        lea              rdx, [rsp + 64]
                        mov              rcx, 148
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n6_op77_α
#-----------------------------------------------------------------------------------------------------------------------
n6_op77_α:
                        lea              rdi, [rsp + 80]
                        lea              rsi, [rsp + 64]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jns   n3_op14_α
                        mov              qword ptr [rsp + 48], 0
                        mov              qword ptr [rsp + 56], 0
                                                                                        jmp   n7_var_α
#-----------------------------------------------------------------------------------------------------------------------
n7_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 128], rax
                        mov              qword ptr [rsp + 136], rdx
                                                                                        jmp   n8_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n8_binop_α:
                        mov              rdi, qword ptr [rsp + 48]
                        mov              rsi, qword ptr [rsp + 56]
                        mov              rdx, qword ptr [rsp + 128]
                        mov              rcx, qword ptr [rsp + 136]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 32], rax
                        mov              qword ptr [rsp + 40], rdx
                                                                                        jmp   n9_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n9_assign_α:
                        mov              rax, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rsp + 40]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n3_op14_α
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__max_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__max_β:
                                                                                        jmp   proc_LBL__max_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__max_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 2088]
                        lea              rsp, [rbp + 2112]
                        mov              rbp, [rbp + 2104]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__max_ω:
                        mov              rax, [rbp + 2096]
                        lea              rsp, [rbp + 2112]
                        mov              rbp, [rbp + 2104]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_LBL__min_α
proc_LBL__min_α:
                        .global          proc_LBL__min_α
                        .global          proc_LBL__min_β
                        .global          proc_LBL__min_γ
                        .global          proc_LBL__min_ω
                        sub              rsp, 2112
                        mov              [rsp + 2088], rcx
                        mov              [rsp + 2096], rdx
                        mov              [rsp + 2104], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
proc_LBL__min_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n24_goto_α:
                                                                                        jmp   n25_var_α
n24_goto_β:
                                                                                        jmp   proc_LBL__min_ω
#-----------------------------------------------------------------------------------------------------------------------
n25_var_α:
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 224], rax
                        mov              qword ptr [rsp + 232], rdx
                                                                                        jmp   n26_var_α
#-----------------------------------------------------------------------------------------------------------------------
n26_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 208], rax
                        mov              qword ptr [rsp + 216], rdx
                                                                                        jmp   n28_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n27_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n28_op75_α:
                        mov              eax, dword ptr [rsp + 224]
                        cmp              eax, 7
                                                                                        je    .Lx40_1
                        cmp              eax, 6
                                                                                        jne   .Lx40_0
                        mov              eax, dword ptr [rsp + 208]
                        cmp              eax, 6
                                                                                        jne   .Lx40_0
.Lx40_1:
                        mov              rax, qword ptr [rsp + 224]
                        mov              qword ptr [rsp + 192], rax
                        mov              rax, qword ptr [rsp + 232]
                        mov              qword ptr [rsp + 200], rax
                                                                                        jmp   n29_op75_α
.Lx40_0:
                        lea              rdi, [rsp + 224]
                        lea              rsi, [rsp + 208]
                        lea              rdx, [rsp + 192]
                        mov              rcx, 111
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n29_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n29_op75_α:
                        mov              eax, dword ptr [rsp + 208]
                        cmp              eax, 7
                                                                                        je    .Lx42_1
                        cmp              eax, 6
                                                                                        jne   .Lx42_0
                        mov              eax, dword ptr [rsp + 224]
                        cmp              eax, 6
                                                                                        jne   .Lx42_0
.Lx42_1:
                        mov              rax, qword ptr [rsp + 208]
                        mov              qword ptr [rsp + 176], rax
                        mov              rax, qword ptr [rsp + 216]
                        mov              qword ptr [rsp + 184], rax
                                                                                        jmp   n30_op77_α
.Lx42_0:
                        lea              rdi, [rsp + 208]
                        lea              rsi, [rsp + 224]
                        lea              rdx, [rsp + 176]
                        mov              rcx, 112
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n30_op77_α
#-----------------------------------------------------------------------------------------------------------------------
n30_op77_α:
                        lea              rdi, [rsp + 192]
                        lea              rsi, [rsp + 176]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jle   n27_op14_α
                        mov              qword ptr [rsp + 160], 0
                        mov              qword ptr [rsp + 168], 0
                                                                                        jmp   n31_var_α
#-----------------------------------------------------------------------------------------------------------------------
n31_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 240], rax
                        mov              qword ptr [rsp + 248], rdx
                                                                                        jmp   n32_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n32_binop_α:
                        mov              rdi, qword ptr [rsp + 160]
                        mov              rsi, qword ptr [rsp + 168]
                        mov              rdx, qword ptr [rsp + 240]
                        mov              rcx, qword ptr [rsp + 248]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 144], rax
                        mov              qword ptr [rsp + 152], rdx
                                                                                        jmp   n33_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n33_assign_α:
                        mov              rax, qword ptr [rsp + 144]
                        mov              rdx, qword ptr [rsp + 152]
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rdx
                                                                                        jmp   n27_op14_α
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__min_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__min_β:
                                                                                        jmp   proc_LBL__min_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__min_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 2088]
                        lea              rsp, [rbp + 2112]
                        mov              rbp, [rbp + 2104]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__min_ω:
                        mov              rax, [rbp + 2096]
                        lea              rsp, [rbp + 2112]
                        mov              rbp, [rbp + 2104]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_LBL__abs_α
proc_LBL__abs_α:
                        .global          proc_LBL__abs_α
                        .global          proc_LBL__abs_β
                        .global          proc_LBL__abs_γ
                        .global          proc_LBL__abs_ω
                        sub              rsp, 2112
                        mov              [rsp + 2088], rcx
                        mov              [rsp + 2096], rdx
                        mov              [rsp + 2104], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
proc_LBL__abs_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n48_goto_α:
                                                                                        jmp   n49_var_α
n48_goto_β:
                                                                                        jmp   proc_LBL__abs_ω
#-----------------------------------------------------------------------------------------------------------------------
n49_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 336], rax
                        mov              qword ptr [rsp + 344], rdx
                                                                                        jmp   n50_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n50_lit_integer_α:
                        mov              qword ptr [rsp + 320], 6
                        mov              rax, qword ptr [rip + .Lx61_0]
                        mov              qword ptr [rsp + 328], rax
                                                                                        jmp   n52_op75_α
.Lx61_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n51_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n52_op75_α:
                        mov              eax, dword ptr [rsp + 336]
                        cmp              eax, 7
                                                                                        je    .Lx65_1
                        cmp              eax, 6
                                                                                        jne   .Lx65_0
                        mov              eax, dword ptr [rsp + 320]
                        cmp              eax, 6
                                                                                        jne   .Lx65_0
.Lx65_1:
                        mov              rax, qword ptr [rsp + 336]
                        mov              qword ptr [rsp + 304], rax
                        mov              rax, qword ptr [rsp + 344]
                        mov              qword ptr [rsp + 312], rax
                                                                                        jmp   n53_op75_α
.Lx65_0:
                        lea              rdi, [rsp + 336]
                        lea              rsi, [rsp + 320]
                        lea              rdx, [rsp + 304]
                        mov              rcx, 147
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n53_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n53_op75_α:
                        mov              eax, dword ptr [rsp + 320]
                        cmp              eax, 7
                                                                                        je    .Lx67_1
                        cmp              eax, 6
                                                                                        jne   .Lx67_0
                        mov              eax, dword ptr [rsp + 336]
                        cmp              eax, 6
                                                                                        jne   .Lx67_0
.Lx67_1:
                        mov              rax, qword ptr [rsp + 320]
                        mov              qword ptr [rsp + 288], rax
                        mov              rax, qword ptr [rsp + 328]
                        mov              qword ptr [rsp + 296], rax
                                                                                        jmp   n54_op77_α
.Lx67_0:
                        lea              rdi, [rsp + 320]
                        lea              rsi, [rsp + 336]
                        lea              rdx, [rsp + 288]
                        mov              rcx, 148
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n54_op77_α
#-----------------------------------------------------------------------------------------------------------------------
n54_op77_α:
                        lea              rdi, [rsp + 304]
                        lea              rsi, [rsp + 288]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jns   n51_op14_α
                        mov              qword ptr [rsp + 272], 0
                        mov              qword ptr [rsp + 280], 0
                                                                                        jmp   n55_var_α
#-----------------------------------------------------------------------------------------------------------------------
n55_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 368], rax
                        mov              qword ptr [rsp + 376], rdx
                                                                                        jmp   n56_unop_α
#-----------------------------------------------------------------------------------------------------------------------
n56_unop_α:
                        mov              rdi, qword ptr [rsp + 368]
                        mov              rsi, qword ptr [rsp + 376]
                        call             rt_num_neg@PLT
                        mov              qword ptr [rsp + 352], rax
                        mov              qword ptr [rsp + 360], rdx
                                                                                        jmp   n57_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n57_binop_α:
                        mov              rdi, qword ptr [rsp + 272]
                        mov              rsi, qword ptr [rsp + 280]
                        mov              rdx, qword ptr [rsp + 352]
                        mov              rcx, qword ptr [rsp + 360]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 256], rax
                        mov              qword ptr [rsp + 264], rdx
                                                                                        jmp   n58_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n58_assign_α:
                        mov              rax, qword ptr [rsp + 256]
                        mov              rdx, qword ptr [rsp + 264]
                        mov              qword ptr [1879052336], rax
                        mov              qword ptr [1879052344], rdx
                                                                                        jmp   n51_op14_α
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__abs_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__abs_β:
                                                                                        jmp   proc_LBL__abs_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__abs_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 2088]
                        lea              rsp, [rbp + 2112]
                        mov              rbp, [rbp + 2104]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__abs_ω:
                        mov              rax, [rbp + 2096]
                        lea              rsp, [rbp + 2112]
                        mov              rbp, [rbp + 2104]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_LBL__sign_α
proc_LBL__sign_α:
                        .global          proc_LBL__sign_α
                        .global          proc_LBL__sign_β
                        .global          proc_LBL__sign_γ
                        .global          proc_LBL__sign_ω
                        sub              rsp, 2112
                        mov              [rsp + 2088], rcx
                        mov              [rsp + 2096], rdx
                        mov              [rsp + 2104], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
proc_LBL__sign_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n74_goto_α:
                                                                                        jmp   n75_var_α
n74_goto_β:
                                                                                        jmp   proc_LBL__sign_ω
#-----------------------------------------------------------------------------------------------------------------------
n75_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 464], rax
                        mov              qword ptr [rsp + 472], rdx
                                                                                        jmp   n76_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n76_lit_integer_α:
                        mov              qword ptr [rsp + 448], 6
                        mov              rax, qword ptr [rip + .Lx95_0]
                        mov              qword ptr [rsp + 456], rax
                                                                                        jmp   n78_op75_α
.Lx95_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n77_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 592], rax
                        mov              qword ptr [rsp + 600], rdx
                                                                                        jmp   n79_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n78_op75_α:
                        mov              eax, dword ptr [rsp + 464]
                        cmp              eax, 7
                                                                                        je    .Lx98_1
                        cmp              eax, 6
                                                                                        jne   .Lx98_0
                        mov              eax, dword ptr [rsp + 448]
                        cmp              eax, 6
                                                                                        jne   .Lx98_0
.Lx98_1:
                        mov              rax, qword ptr [rsp + 464]
                        mov              qword ptr [rsp + 432], rax
                        mov              rax, qword ptr [rsp + 472]
                        mov              qword ptr [rsp + 440], rax
                                                                                        jmp   n81_op75_α
.Lx98_0:
                        lea              rdi, [rsp + 464]
                        lea              rsi, [rsp + 448]
                        lea              rdx, [rsp + 432]
                        mov              rcx, 147
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n81_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n79_lit_integer_α:
                        mov              qword ptr [rsp + 576], 6
                        mov              rax, qword ptr [rip + .Lx99_0]
                        mov              qword ptr [rsp + 584], rax
                                                                                        jmp   n82_op75_α
.Lx99_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n80_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n81_op75_α:
                        mov              eax, dword ptr [rsp + 448]
                        cmp              eax, 7
                                                                                        je    .Lx103_1
                        cmp              eax, 6
                                                                                        jne   .Lx103_0
                        mov              eax, dword ptr [rsp + 464]
                        cmp              eax, 6
                                                                                        jne   .Lx103_0
.Lx103_1:
                        mov              rax, qword ptr [rsp + 448]
                        mov              qword ptr [rsp + 416], rax
                        mov              rax, qword ptr [rsp + 456]
                        mov              qword ptr [rsp + 424], rax
                                                                                        jmp   n83_op77_α
.Lx103_0:
                        lea              rdi, [rsp + 448]
                        lea              rsi, [rsp + 464]
                        lea              rdx, [rsp + 416]
                        mov              rcx, 148
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n83_op77_α
#-----------------------------------------------------------------------------------------------------------------------
n82_op75_α:
                        mov              eax, dword ptr [rsp + 592]
                        cmp              eax, 7
                                                                                        je    .Lx105_1
                        cmp              eax, 6
                                                                                        jne   .Lx105_0
                        mov              eax, dword ptr [rsp + 576]
                        cmp              eax, 6
                                                                                        jne   .Lx105_0
.Lx105_1:
                        mov              rax, qword ptr [rsp + 592]
                        mov              qword ptr [rsp + 560], rax
                        mov              rax, qword ptr [rsp + 600]
                        mov              qword ptr [rsp + 568], rax
                                                                                        jmp   n84_op75_α
.Lx105_0:
                        lea              rdi, [rsp + 592]
                        lea              rsi, [rsp + 576]
                        lea              rdx, [rsp + 560]
                        mov              rcx, 111
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n84_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n83_op77_α:
                        lea              rdi, [rsp + 432]
                        lea              rsi, [rsp + 416]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jns   n77_var_α
                        mov              qword ptr [rsp + 400], 0
                        mov              qword ptr [rsp + 408], 0
                                                                                        jmp   n85_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n84_op75_α:
                        mov              eax, dword ptr [rsp + 576]
                        cmp              eax, 7
                                                                                        je    .Lx109_1
                        cmp              eax, 6
                                                                                        jne   .Lx109_0
                        mov              eax, dword ptr [rsp + 592]
                        cmp              eax, 6
                                                                                        jne   .Lx109_0
.Lx109_1:
                        mov              rax, qword ptr [rsp + 576]
                        mov              qword ptr [rsp + 544], rax
                        mov              rax, qword ptr [rsp + 584]
                        mov              qword ptr [rsp + 552], rax
                                                                                        jmp   n86_op77_α
.Lx109_0:
                        lea              rdi, [rsp + 576]
                        lea              rsi, [rsp + 592]
                        lea              rdx, [rsp + 544]
                        mov              rcx, 112
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n86_op77_α
#-----------------------------------------------------------------------------------------------------------------------
n85_lit_integer_α:
                        mov              qword ptr [rsp + 496], 6
                        mov              rax, qword ptr [rip + .Lx110_0]
                        mov              qword ptr [rsp + 504], rax
                                                                                        jmp   n87_unop_α
.Lx110_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n86_op77_α:
                        lea              rdi, [rsp + 560]
                        lea              rsi, [rsp + 544]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jle   n80_op14_α
                        mov              qword ptr [rsp + 528], 0
                        mov              qword ptr [rsp + 536], 0
                                                                                        jmp   n88_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n87_unop_α:
                        mov              rdi, qword ptr [rsp + 496]
                        mov              rsi, qword ptr [rsp + 504]
                        call             rt_num_neg@PLT
                        mov              qword ptr [rsp + 480], rax
                        mov              qword ptr [rsp + 488], rdx
                                                                                        jmp   n89_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n88_lit_integer_α:
                        mov              qword ptr [rsp + 608], 6
                        mov              rax, qword ptr [rip + .Lx114_0]
                        mov              qword ptr [rsp + 616], rax
                                                                                        jmp   n90_binop_α
.Lx114_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n89_binop_α:
                        mov              rdi, qword ptr [rsp + 400]
                        mov              rsi, qword ptr [rsp + 408]
                        mov              rdx, qword ptr [rsp + 480]
                        mov              rcx, qword ptr [rsp + 488]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 384], rax
                        mov              qword ptr [rsp + 392], rdx
                                                                                        jmp   n91_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n90_binop_α:
                        mov              rdi, qword ptr [rsp + 528]
                        mov              rsi, qword ptr [rsp + 536]
                        mov              rdx, qword ptr [rsp + 608]
                        mov              rcx, qword ptr [rsp + 616]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 512], rax
                        mov              qword ptr [rsp + 520], rdx
                                                                                        jmp   n92_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n91_assign_α:
                        mov              rax, qword ptr [rsp + 384]
                        mov              rdx, qword ptr [rsp + 392]
                        mov              qword ptr [1879052352], rax
                        mov              qword ptr [1879052360], rdx
                                                                                        jmp   n80_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n92_assign_α:
                        mov              rax, qword ptr [rsp + 512]
                        mov              rdx, qword ptr [rsp + 520]
                        mov              qword ptr [1879052352], rax
                        mov              qword ptr [1879052360], rdx
                                                                                        jmp   n80_op14_α
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__sign_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__sign_β:
                                                                                        jmp   proc_LBL__sign_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__sign_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 2088]
                        lea              rsp, [rbp + 2112]
                        mov              rbp, [rbp + 2104]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__sign_ω:
                        mov              rax, [rbp + 2096]
                        lea              rsp, [rbp + 2112]
                        mov              rbp, [rbp + 2104]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_LBL__gcd_α
proc_LBL__gcd_α:
                        .global          proc_LBL__gcd_α
                        .global          proc_LBL__gcd_β
                        .global          proc_LBL__gcd_γ
                        .global          proc_LBL__gcd_ω
                        sub              rsp, 2112
                        mov              [rsp + 2088], rcx
                        mov              [rsp + 2096], rdx
                        mov              [rsp + 2104], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
proc_LBL__gcd_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n119_goto_α:
                                                                                        jmp   n120_var_α
n119_goto_β:
                                                                                        jmp   proc_LBL__gcd_ω
#-----------------------------------------------------------------------------------------------------------------------
n120_var_α:
                        mov              rax, qword ptr [1879052384]
                        mov              rdx, qword ptr [1879052392]
                        mov              qword ptr [rsp + 688], rax
                        mov              qword ptr [rsp + 696], rdx
                                                                                        jmp   n121_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n121_lit_integer_α:
                        mov              qword ptr [rsp + 704], 6
                        mov              rax, qword ptr [rip + .Lx134_0]
                        mov              qword ptr [rsp + 712], rax
                                                                                        jmp   n123_call_α
.Lx134_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n122_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n123_call_α:
                        mov              rax, qword ptr [rsp + 688]
                        mov              qword ptr [rsp + 640], rax
                        mov              rax, qword ptr [rsp + 696]
                        mov              qword ptr [rsp + 648], rax
                        mov              rax, qword ptr [rsp + 704]
                        mov              qword ptr [rsp + 656], rax
                        mov              rax, qword ptr [rsp + 712]
                        mov              qword ptr [rsp + 664], rax
                        .section         .rodata
.Lrkfn138:              .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn138]
                        lea              rsi, [rsp + 640]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 624], rax
                        mov              qword ptr [rsp + 632], rdx
                        cmp              eax, 99
                                                                                        je    n122_op14_α
                                                                                        jmp   n124_var_α
n123_call_β:
                                                                                        jmp   n122_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n124_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 784], rax
                        mov              qword ptr [rsp + 792], rdx
                                                                                        jmp   n125_var_α
#-----------------------------------------------------------------------------------------------------------------------
n125_var_α:
                        mov              rax, qword ptr [1879052384]
                        mov              rdx, qword ptr [1879052392]
                        mov              qword ptr [rsp + 800], rax
                        mov              qword ptr [rsp + 808], rdx
                                                                                        jmp   n127_call_α
#-----------------------------------------------------------------------------------------------------------------------
n126_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052384]
                        mov              rdx, qword ptr [1879052392]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n128_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n127_call_α:
                        mov              rax, qword ptr [rsp + 784]
                        mov              qword ptr [rsp + 736], rax
                        mov              rax, qword ptr [rsp + 792]
                        mov              qword ptr [rsp + 744], rax
                        mov              rax, qword ptr [rsp + 800]
                        mov              qword ptr [rsp + 752], rax
                        mov              rax, qword ptr [rsp + 808]
                        mov              qword ptr [rsp + 760], rax
                        .section         .rodata
.Lrkfn143:              .string          "REMDR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn143]
                        lea              rsi, [rsp + 736]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 720], rax
                        mov              qword ptr [rsp + 728], rdx
                        cmp              eax, 99
                                                                                        je    n126_var_α
                                                                                        jmp   n130_assign_α
n127_call_β:
                                                                                        jmp   n126_var_α
#-----------------------------------------------------------------------------------------------------------------------
n128_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052368], rax
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n129_var_α
#-----------------------------------------------------------------------------------------------------------------------
n129_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052400]
                        mov              rdx, qword ptr [1879052408]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n131_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n130_assign_α:
                        mov              rax, qword ptr [rsp + 720]
                        mov              rdx, qword ptr [rsp + 728]
                        mov              qword ptr [1879052400], rax
                        mov              qword ptr [1879052408], rdx
                                                                                        jmp   n126_var_α
#-----------------------------------------------------------------------------------------------------------------------
n131_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052384], rax
                        mov              qword ptr [1879052392], rdx
                                                                                        jmp   n120_var_α
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__gcd_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__gcd_β:
                                                                                        jmp   proc_LBL__gcd_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__gcd_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 2088]
                        lea              rsp, [rbp + 2112]
                        mov              rbp, [rbp + 2104]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__gcd_ω:
                        mov              rax, [rbp + 2096]
                        lea              rsp, [rbp + 2112]
                        mov              rbp, [rbp + 2104]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_LBL__lcm_α
proc_LBL__lcm_α:
                        .global          proc_LBL__lcm_α
                        .global          proc_LBL__lcm_β
                        .global          proc_LBL__lcm_γ
                        .global          proc_LBL__lcm_ω
                        sub              rsp, 2112
                        mov              [rsp + 2088], rcx
                        mov              [rsp + 2096], rdx
                        mov              [rsp + 2104], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
proc_LBL__lcm_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n148_goto_α:
                                                                                        jmp   n149_var_α
n148_goto_β:
                                                                                        jmp   proc_LBL__lcm_ω
#-----------------------------------------------------------------------------------------------------------------------
n149_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 912], rax
                        mov              qword ptr [rsp + 920], rdx
                                                                                        jmp   n150_var_α
#-----------------------------------------------------------------------------------------------------------------------
n150_var_α:
                        mov              rax, qword ptr [1879052384]
                        mov              rdx, qword ptr [1879052392]
                        mov              qword ptr [rsp + 928], rax
                        mov              qword ptr [rsp + 936], rdx
                                                                                        jmp   n152_call_α
#-----------------------------------------------------------------------------------------------------------------------
n151_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 976], rax
                        mov              qword ptr [rsp + 984], rdx
                                                                                        jmp   n153_var_α
#-----------------------------------------------------------------------------------------------------------------------
n152_call_α:
                        sub              rsp, 48
                        mov              rax, qword ptr [1879052368]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052376]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052384]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052392]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [1879052400]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [1879052408]
                        mov              qword ptr [rsp + 40], rax
                        mov              rdi, qword ptr [rip + .Lx165_0]
                        mov              esi, 3
                        mov              edx, 2
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx165_5
                        mov              rax, qword ptr [rsp + 912]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 920]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 928]
                        mov              qword ptr [1879052384], rax
                        mov              rax, qword ptr [rsp + 936]
                        mov              qword ptr [1879052392], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx165_6]
                        lea              rdx, [rip + .Lx165_7]
                                                                                        jmp   rax
.Lx165_6:
                        mov              rdi, qword ptr [1879052368]
                        mov              rsi, qword ptr [1879052376]
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052400], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052408], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052384], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052392], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052376], rax
                        add              rsp, 48
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx165_2
.Lx165_7:
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052400], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052408], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052384], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052392], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052376], rax
                        add              rsp, 48
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx165_2
.Lx165_5:
                        add              rsp, 48
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx165_20
                        mov              rax, qword ptr [rsp + 912]
                        mov              rdx, qword ptr [rsp + 920]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx165_21
.Lx165_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 912]
                        mov              rdx, qword ptr [rsp + 920]
                        call             rt_arg_stage@PLT
.Lx165_21:
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx165_22
                        mov              rax, qword ptr [rsp + 928]
                        mov              rdx, qword ptr [rsp + 936]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 16], rax
                        mov              qword ptr [r10 + 24], rdx
                                                                                        jmp   .Lx165_23
.Lx165_22:
                        mov              edi, 1
                        mov              rsi, qword ptr [rsp + 928]
                        mov              rdx, qword ptr [rsp + 936]
                        call             rt_arg_stage@PLT
.Lx165_23:
                        mov              rdi, qword ptr [rip + .Lx165_0]
                        mov              esi, 2
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx165_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx165_3]
                        lea              rdx, [rip + .Lx165_4]
                                                                                        jmp   rax
.Lx165_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx165_2
.Lx165_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx165_2
.Lx165_1:
                        call             rt_faildescr@PLT
.Lx165_2:
                        mov              qword ptr [rsp + 848], rax
                        mov              qword ptr [rsp + 856], rdx
                        cmp              eax, 99
                                                                                        je    n151_var_α
                                                                                        jmp   n155_assign_α
n152_call_β:
                                                                                        jmp   n151_var_α
.Lx165_0:
                        .quad            .Lx165_0_s
.Lx165_0_s:
                        .string          "gcd"
#-----------------------------------------------------------------------------------------------------------------------
n153_var_α:
                        mov              rax, qword ptr [1879052448]
                        mov              rdx, qword ptr [1879052456]
                        mov              qword ptr [rsp + 992], rax
                        mov              qword ptr [rsp + 1000], rdx
                                                                                        jmp   n156_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n154_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n155_assign_α:
                        mov              rax, qword ptr [rsp + 848]
                        mov              rdx, qword ptr [rsp + 856]
                        mov              qword ptr [1879052448], rax
                        mov              qword ptr [1879052456], rdx
                                                                                        jmp   n151_var_α
#-----------------------------------------------------------------------------------------------------------------------
n156_binop_α:
                        mov              eax, dword ptr [rsp + 976]
                        cmp              eax, 100
                                                                                        je    .Lx170_0
                        mov              eax, dword ptr [rsp + 992]
                        cmp              eax, 100
                                                                                        je    .Lx170_0
                        mov              eax, dword ptr [rsp + 976]
                        cmp              eax, 6
                                                                                        jne   .Lx170_2
                        mov              eax, dword ptr [rsp + 992]
                        cmp              eax, 6
                                                                                        jne   .Lx170_2
.Lx170_1:
                        mov              rax, qword ptr [rsp + 984]
                        mov              rcx, qword ptr [rsp + 1000]
                        cqo
                        idiv             rcx
                        mov              qword ptr [rsp + 960], 6
                        mov              qword ptr [rsp + 968], rax
                                                                                        jmp   n157_var_α
.Lx170_0:
                        mov              rdi, qword ptr [rsp + 976]
                        mov              rsi, qword ptr [rsp + 984]
                        mov              rdx, qword ptr [rsp + 992]
                        mov              rcx, qword ptr [rsp + 1000]
                        mov              r8d, 3
                        lea              r9, [rsp + 960]
                        call             rt_binop_overload@PLT
                        test             eax, eax
                                                                                        jne   n157_var_α
.Lx170_2:
                        mov              rdi, qword ptr [rsp + 976]
                        mov              rsi, qword ptr [rsp + 984]
                        mov              rdx, qword ptr [rsp + 992]
                        mov              rcx, qword ptr [rsp + 1000]
                        mov              r8d, 3
                        call             rt_num_arith@PLT
                        cmp              eax, 99
                                                                                        je    n154_op14_α
                        mov              qword ptr [rsp + 960], rax
                        mov              qword ptr [rsp + 968], rdx
                                                                                        jmp   n157_var_α
n156_binop_β:
                                                                                        jmp   n154_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n157_var_α:
                        mov              rax, qword ptr [1879052384]
                        mov              rdx, qword ptr [1879052392]
                        mov              qword ptr [rsp + 1008], rax
                        mov              qword ptr [rsp + 1016], rdx
                                                                                        jmp   n158_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n158_binop_α:
                        mov              eax, dword ptr [rsp + 960]
                        cmp              eax, 100
                                                                                        je    .Lx172_0
                        mov              eax, dword ptr [rsp + 1008]
                        cmp              eax, 100
                                                                                        je    .Lx172_0
                        mov              eax, dword ptr [rsp + 960]
                        cmp              eax, 6
                                                                                        jne   .Lx172_2
                        mov              eax, dword ptr [rsp + 1008]
                        cmp              eax, 6
                                                                                        jne   .Lx172_2
.Lx172_1:
                        mov              rax, qword ptr [rsp + 968]
                        mov              rcx, qword ptr [rsp + 1016]
                        imul             rax, rcx
                        mov              qword ptr [rsp + 944], 6
                        mov              qword ptr [rsp + 952], rax
                                                                                        jmp   n159_assign_α
.Lx172_0:
                        mov              rdi, qword ptr [rsp + 960]
                        mov              rsi, qword ptr [rsp + 968]
                        mov              rdx, qword ptr [rsp + 1008]
                        mov              rcx, qword ptr [rsp + 1016]
                        mov              r8d, 2
                        lea              r9, [rsp + 944]
                        call             rt_binop_overload@PLT
                        test             eax, eax
                                                                                        jne   n159_assign_α
.Lx172_2:
                        mov              rdi, qword ptr [rsp + 960]
                        mov              rsi, qword ptr [rsp + 968]
                        mov              rdx, qword ptr [rsp + 1008]
                        mov              rcx, qword ptr [rsp + 1016]
                        mov              r8d, 2
                        call             rt_num_arith@PLT
                        cmp              eax, 99
                                                                                        je    n154_op14_α
                        mov              qword ptr [rsp + 944], rax
                        mov              qword ptr [rsp + 952], rdx
                                                                                        jmp   n159_assign_α
n158_binop_β:
                                                                                        jmp   n154_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n159_assign_α:
                        mov              rax, qword ptr [rsp + 944]
                        mov              rdx, qword ptr [rsp + 952]
                        mov              qword ptr [1879052416], rax
                        mov              qword ptr [1879052424], rdx
                                                                                        jmp   n154_op14_α
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__lcm_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__lcm_β:
                                                                                        jmp   proc_LBL__lcm_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__lcm_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 2088]
                        lea              rsp, [rbp + 2112]
                        mov              rbp, [rbp + 2104]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__lcm_ω:
                        mov              rax, [rbp + 2096]
                        lea              rsp, [rbp + 2112]
                        mov              rbp, [rbp + 2104]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_max_α
proc_max_α:
                        .global          proc_max_α
                        .global          proc_max_β
                        .global          proc_max_γ
                        .global          proc_max_ω
                        sub              rsp, 2112
                        mov              [rsp + 2088], rcx
                        mov              [rsp + 2096], rdx
                        mov              [rsp + 2104], rbp
                        mov              rbp, rsp
                        mov              rdi, rsp
                        mov              ecx, 2080
                        xor              eax, eax
                        rep stosb
proc_max_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n174_op14_α:
                        mov              rdi, qword ptr [rbp + 2088]
                        mov              rsi, qword ptr [rbp + 2096]
                        lea              rdx, [rbp + 2112]
                        mov              rcx, qword ptr [rbp + 2104]
                        call             rt_flat_wire_adopt@PLT
                                                                                        jmp   n175_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n175_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx179_0]
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx179_1
.Lx179_0:
                        .quad            .Lx179_0_s
.Lx179_0_s:
                        .string          "max"
.Lx179_1:
                                                                                        jmp   proc_max_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_max_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_max_β:
                                                                                        jmp   proc_max_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_max_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 2088]
                        lea              rsp, [rbp + 2112]
                        mov              rbp, [rbp + 2104]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_max_ω:
                        mov              rax, [rbp + 2096]
                        lea              rsp, [rbp + 2112]
                        mov              rbp, [rbp + 2104]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_min_α
proc_min_α:
                        .global          proc_min_α
                        .global          proc_min_β
                        .global          proc_min_γ
                        .global          proc_min_ω
                        sub              rsp, 2112
                        mov              [rsp + 2088], rcx
                        mov              [rsp + 2096], rdx
                        mov              [rsp + 2104], rbp
                        mov              rbp, rsp
                        mov              rdi, rsp
                        mov              ecx, 2080
                        xor              eax, eax
                        rep stosb
proc_min_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n180_op14_α:
                        mov              rdi, qword ptr [rbp + 2088]
                        mov              rsi, qword ptr [rbp + 2096]
                        lea              rdx, [rbp + 2112]
                        mov              rcx, qword ptr [rbp + 2104]
                        call             rt_flat_wire_adopt@PLT
                                                                                        jmp   n181_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n181_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx185_0]
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx185_1
.Lx185_0:
                        .quad            .Lx185_0_s
.Lx185_0_s:
                        .string          "min"
.Lx185_1:
                                                                                        jmp   proc_min_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_min_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_min_β:
                                                                                        jmp   proc_min_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_min_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 2088]
                        lea              rsp, [rbp + 2112]
                        mov              rbp, [rbp + 2104]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_min_ω:
                        mov              rax, [rbp + 2096]
                        lea              rsp, [rbp + 2112]
                        mov              rbp, [rbp + 2104]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_abs_α
proc_abs_α:
                        .global          proc_abs_α
                        .global          proc_abs_β
                        .global          proc_abs_γ
                        .global          proc_abs_ω
                        sub              rsp, 2112
                        mov              [rsp + 2088], rcx
                        mov              [rsp + 2096], rdx
                        mov              [rsp + 2104], rbp
                        mov              rbp, rsp
                        mov              rdi, rsp
                        mov              ecx, 2080
                        xor              eax, eax
                        rep stosb
proc_abs_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n186_op14_α:
                        mov              rdi, qword ptr [rbp + 2088]
                        mov              rsi, qword ptr [rbp + 2096]
                        lea              rdx, [rbp + 2112]
                        mov              rcx, qword ptr [rbp + 2104]
                        call             rt_flat_wire_adopt@PLT
                                                                                        jmp   n187_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n187_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx191_0]
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx191_1
.Lx191_0:
                        .quad            .Lx191_0_s
.Lx191_0_s:
                        .string          "abs"
.Lx191_1:
                                                                                        jmp   proc_abs_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_abs_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_abs_β:
                                                                                        jmp   proc_abs_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_abs_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 2088]
                        lea              rsp, [rbp + 2112]
                        mov              rbp, [rbp + 2104]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_abs_ω:
                        mov              rax, [rbp + 2096]
                        lea              rsp, [rbp + 2112]
                        mov              rbp, [rbp + 2104]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_sign_α
proc_sign_α:
                        .global          proc_sign_α
                        .global          proc_sign_β
                        .global          proc_sign_γ
                        .global          proc_sign_ω
                        sub              rsp, 2112
                        mov              [rsp + 2088], rcx
                        mov              [rsp + 2096], rdx
                        mov              [rsp + 2104], rbp
                        mov              rbp, rsp
                        mov              rdi, rsp
                        mov              ecx, 2080
                        xor              eax, eax
                        rep stosb
proc_sign_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n192_op14_α:
                        mov              rdi, qword ptr [rbp + 2088]
                        mov              rsi, qword ptr [rbp + 2096]
                        lea              rdx, [rbp + 2112]
                        mov              rcx, qword ptr [rbp + 2104]
                        call             rt_flat_wire_adopt@PLT
                                                                                        jmp   n193_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n193_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx197_0]
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx197_1
.Lx197_0:
                        .quad            .Lx197_0_s
.Lx197_0_s:
                        .string          "sign"
.Lx197_1:
                                                                                        jmp   proc_sign_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_sign_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_sign_β:
                                                                                        jmp   proc_sign_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_sign_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 2088]
                        lea              rsp, [rbp + 2112]
                        mov              rbp, [rbp + 2104]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_sign_ω:
                        mov              rax, [rbp + 2096]
                        lea              rsp, [rbp + 2112]
                        mov              rbp, [rbp + 2104]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_gcd_α
proc_gcd_α:
                        .global          proc_gcd_α
                        .global          proc_gcd_β
                        .global          proc_gcd_γ
                        .global          proc_gcd_ω
                        sub              rsp, 2112
                        mov              [rsp + 2088], rcx
                        mov              [rsp + 2096], rdx
                        mov              [rsp + 2104], rbp
                        mov              rbp, rsp
                        mov              rdi, rsp
                        mov              ecx, 2080
                        xor              eax, eax
                        rep stosb
proc_gcd_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n198_op14_α:
                        mov              rdi, qword ptr [rbp + 2088]
                        mov              rsi, qword ptr [rbp + 2096]
                        lea              rdx, [rbp + 2112]
                        mov              rcx, qword ptr [rbp + 2104]
                        call             rt_flat_wire_adopt@PLT
                                                                                        jmp   n199_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n199_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx203_0]
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx203_1
.Lx203_0:
                        .quad            .Lx203_0_s
.Lx203_0_s:
                        .string          "gcd"
.Lx203_1:
                                                                                        jmp   proc_gcd_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_gcd_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_gcd_β:
                                                                                        jmp   proc_gcd_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_gcd_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 2088]
                        lea              rsp, [rbp + 2112]
                        mov              rbp, [rbp + 2104]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_gcd_ω:
                        mov              rax, [rbp + 2096]
                        lea              rsp, [rbp + 2112]
                        mov              rbp, [rbp + 2104]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_lcm_α
proc_lcm_α:
                        .global          proc_lcm_α
                        .global          proc_lcm_β
                        .global          proc_lcm_γ
                        .global          proc_lcm_ω
                        sub              rsp, 2112
                        mov              [rsp + 2088], rcx
                        mov              [rsp + 2096], rdx
                        mov              [rsp + 2104], rbp
                        mov              rbp, rsp
                        mov              rdi, rsp
                        mov              ecx, 2080
                        xor              eax, eax
                        rep stosb
proc_lcm_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n204_op14_α:
                        mov              rdi, qword ptr [rbp + 2088]
                        mov              rsi, qword ptr [rbp + 2096]
                        lea              rdx, [rbp + 2112]
                        mov              rcx, qword ptr [rbp + 2104]
                        call             rt_flat_wire_adopt@PLT
                                                                                        jmp   n205_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n205_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx209_0]
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx209_1
.Lx209_0:
                        .quad            .Lx209_0_s
.Lx209_0_s:
                        .string          "lcm"
.Lx209_1:
                                                                                        jmp   proc_lcm_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_lcm_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_lcm_β:
                                                                                        jmp   proc_lcm_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_lcm_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 2088]
                        lea              rsp, [rbp + 2112]
                        mov              rbp, [rbp + 2104]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_lcm_ω:
                        mov              rax, [rbp + 2096]
                        lea              rsp, [rbp + 2112]
                        mov              rbp, [rbp + 2104]
                                                                                        jmp   rax
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lstartup_pname0:       .string          "LBL__max"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_LBL__max_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 2080
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname1:       .string          "LBL__min"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname1]
                        lea              rsi, [rip + proc_LBL__min_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 2080
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname2:       .string          "LBL__abs"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname2]
                        lea              rsi, [rip + proc_LBL__abs_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 2080
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname3:       .string          "LBL__sign"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname3]
                        lea              rsi, [rip + proc_LBL__sign_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 2080
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname4:       .string          "LBL__gcd"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname4]
                        lea              rsi, [rip + proc_LBL__gcd_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname4]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname4]
                        mov              esi, 2080
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname4]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname5:       .string          "LBL__lcm"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname5]
                        lea              rsi, [rip + proc_LBL__lcm_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 2080
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname6:       .string          "max"
.Lstartup_pp6_0:        .string          "max"
.Lstartup_pp6_1:        .string          "x"
                        .align           8
.Lstartup_pnames6:
                        .quad            .Lstartup_pp6_0
                        .quad            .Lstartup_pp6_1
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname6]
                        lea              rsi, [rip + .Lstartup_pnames6]
                        mov              edx, 2
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname6]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname6]
                        lea              rsi, [rip + proc_max_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname6]
                        mov              esi, 2
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname6]
                        mov              esi, 16
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname6]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname7:       .string          "min"
.Lstartup_pp7_0:        .string          "min"
.Lstartup_pp7_1:        .string          "x"
                        .align           8
.Lstartup_pnames7:
                        .quad            .Lstartup_pp7_0
                        .quad            .Lstartup_pp7_1
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname7]
                        lea              rsi, [rip + .Lstartup_pnames7]
                        mov              edx, 2
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname7]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname7]
                        lea              rsi, [rip + proc_min_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname7]
                        mov              esi, 2
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname7]
                        mov              esi, 16
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname7]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname8:       .string          "abs"
.Lstartup_pp8_0:        .string          "abs"
                        .align           8
.Lstartup_pnames8:
                        .quad            .Lstartup_pp8_0
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname8]
                        lea              rsi, [rip + .Lstartup_pnames8]
                        mov              edx, 1
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname8]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname8]
                        lea              rsi, [rip + proc_abs_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname8]
                        mov              esi, 1
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname8]
                        mov              esi, 16
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname8]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname9:       .string          "sign"
.Lstartup_pp9_0:        .string          "sign"
                        .align           8
.Lstartup_pnames9:
                        .quad            .Lstartup_pp9_0
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname9]
                        lea              rsi, [rip + .Lstartup_pnames9]
                        mov              edx, 1
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname9]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname9]
                        lea              rsi, [rip + proc_sign_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname9]
                        mov              esi, 1
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname9]
                        mov              esi, 16
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname9]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname10:      .string          "gcd"
.Lstartup_pp10_0:       .string          "gcd"
.Lstartup_pp10_1:       .string          "b"
.Lstartup_pp10_2:       .string          "r"
                        .align           8
.Lstartup_pnames10:
                        .quad            .Lstartup_pp10_0
                        .quad            .Lstartup_pp10_1
                        .quad            .Lstartup_pp10_2
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname10]
                        lea              rsi, [rip + .Lstartup_pnames10]
                        mov              edx, 3
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname10]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname10]
                        lea              rsi, [rip + proc_gcd_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname10]
                        mov              esi, 3
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname10]
                        mov              esi, 16
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname10]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname11:      .string          "lcm"
.Lstartup_pp11_0:       .string          "a"
.Lstartup_pp11_1:       .string          "b"
.Lstartup_pp11_2:       .string          "g"
                        .align           8
.Lstartup_pnames11:
                        .quad            .Lstartup_pp11_0
                        .quad            .Lstartup_pp11_1
                        .quad            .Lstartup_pp11_2
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname11]
                        lea              rsi, [rip + .Lstartup_pnames11]
                        mov              edx, 3
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname11]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname11]
                        lea              rsi, [rip + proc_lcm_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname11]
                        mov              esi, 3
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname11]
                        mov              esi, 16
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname11]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        add              rsp, 8
                        ret
                        .section         .rodata
.Lgvan0:                .string          "max"
.Lgvan1:                .string          "x"
.Lgvan2:                .string          "min"
.Lgvan3:                .string          "abs"
.Lgvan4:                .string          "sign"
.Lgvan5:                .string          "gcd"
.Lgvan6:                .string          "b"
.Lgvan7:                .string          "r"
.Lgvan8:                .string          "lcm"
.Lgvan9:                .string          "a"
.Lgvan10:               .string          "g"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .quad            .Lgvan1
                        .quad            .Lgvan2
                        .quad            .Lgvan3
                        .quad            .Lgvan4
                        .quad            .Lgvan5
                        .quad            .Lgvan6
                        .quad            .Lgvan7
                        .quad            .Lgvan8
                        .quad            .Lgvan9
                        .quad            .Lgvan10
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             proc_startup
                        mov              edi, 11
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 11
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
                        sub              rsp, 2088
                        mov              rdi, rsp
                        mov              ecx, 2088
                        xor              eax, eax
                        rep stosb
                        mov              [rsp + 2080], rbp
                        mov              rbp, rsp
main_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n210_lit_string_α:
                        mov              qword ptr [rsp + 1088], 1
                        mov              rax, qword ptr [rip + .Lx331_0]
                        mov              qword ptr [rsp + 1096], rax
                                                                                        jmp   n223_lit_integer_α
.Lx331_0:
                        .quad            .Lx331_0_s
.Lx331_0_s:
                        .string          "TRIM"
#-----------------------------------------------------------------------------------------------------------------------
n211_goto_α:
                                                                                        jmp   n224_var_α
n211_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n212_goto_α:
                                                                                        jmp   n210_lit_string_α
n212_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n213_goto_α:
                                                                                        jmp   n225_var_α
n213_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n214_goto_α:
                                                                                        jmp   n210_lit_string_α
n214_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n215_goto_α:
                                                                                        jmp   n226_var_α
n215_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n216_goto_α:
                                                                                        jmp   n210_lit_string_α
n216_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n217_goto_α:
                                                                                        jmp   n227_var_α
n217_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n218_goto_α:
                                                                                        jmp   n210_lit_string_α
n218_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n219_goto_α:
                                                                                        jmp   n228_var_α
n219_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n220_goto_α:
                                                                                        jmp   n210_lit_string_α
n220_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n221_goto_α:
                                                                                        jmp   n229_var_α
n221_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n222_goto_α:
                                                                                        jmp   n210_lit_string_α
n222_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n223_lit_integer_α:
                        mov              qword ptr [rsp + 1104], 6
                        mov              rax, qword ptr [rip + .Lx344_0]
                        mov              qword ptr [rsp + 1112], rax
                                                                                        jmp   n230_call_α
.Lx344_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n224_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 112], rax
                        mov              qword ptr [rsp + 120], rdx
                                                                                        jmp   n231_var_α
#-----------------------------------------------------------------------------------------------------------------------
n225_var_α:
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 224], rax
                        mov              qword ptr [rsp + 232], rdx
                                                                                        jmp   n233_var_α
#-----------------------------------------------------------------------------------------------------------------------
n226_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 336], rax
                        mov              qword ptr [rsp + 344], rdx
                                                                                        jmp   n234_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n227_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 464], rax
                        mov              qword ptr [rsp + 472], rdx
                                                                                        jmp   n235_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n228_var_α:
                        mov              rax, qword ptr [1879052384]
                        mov              rdx, qword ptr [1879052392]
                        mov              qword ptr [rsp + 688], rax
                        mov              qword ptr [rsp + 696], rdx
                                                                                        jmp   n237_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n229_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 912], rax
                        mov              qword ptr [rsp + 920], rdx
                                                                                        jmp   n238_var_α
#-----------------------------------------------------------------------------------------------------------------------
n230_call_α:
                        mov              rax, qword ptr [rsp + 1088]
                        mov              qword ptr [rsp + 1040], rax
                        mov              rax, qword ptr [rsp + 1096]
                        mov              qword ptr [rsp + 1048], rax
                        mov              rax, qword ptr [rsp + 1104]
                        mov              qword ptr [rsp + 1056], rax
                        mov              rax, qword ptr [rsp + 1112]
                        mov              qword ptr [rsp + 1064], rax
                        .section         .rodata
.Lrkfn352:              .string          "SNO$KWSET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn352]
                        lea              rsi, [rsp + 1040]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1024], rax
                        mov              qword ptr [rsp + 1032], rdx
                        cmp              eax, 99
                                                                                        je    n240_lit_integer_α
                                                                                        jmp   n240_lit_integer_α
n230_call_β:
                                                                                        jmp   n240_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n231_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 96], rax
                        mov              qword ptr [rsp + 104], rdx
                                                                                        jmp   n241_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n232_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n233_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 208], rax
                        mov              qword ptr [rsp + 216], rdx
                                                                                        jmp   n242_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n234_lit_integer_α:
                        mov              qword ptr [rsp + 320], 6
                        mov              rax, qword ptr [rip + .Lx357_0]
                        mov              qword ptr [rsp + 328], rax
                                                                                        jmp   n243_op75_α
.Lx357_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n235_lit_integer_α:
                        mov              qword ptr [rsp + 448], 6
                        mov              rax, qword ptr [rip + .Lx358_0]
                        mov              qword ptr [rsp + 456], rax
                                                                                        jmp   n244_op75_α
.Lx358_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n236_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 592], rax
                        mov              qword ptr [rsp + 600], rdx
                                                                                        jmp   n245_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n237_lit_integer_α:
                        mov              qword ptr [rsp + 704], 6
                        mov              rax, qword ptr [rip + .Lx360_0]
                        mov              qword ptr [rsp + 712], rax
                                                                                        jmp   n246_call_α
.Lx360_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n238_var_α:
                        mov              rax, qword ptr [1879052384]
                        mov              rdx, qword ptr [1879052392]
                        mov              qword ptr [rsp + 928], rax
                        mov              qword ptr [rsp + 936], rdx
                                                                                        jmp   n247_call_α
#-----------------------------------------------------------------------------------------------------------------------
n239_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 976], rax
                        mov              qword ptr [rsp + 984], rdx
                                                                                        jmp   n248_var_α
#-----------------------------------------------------------------------------------------------------------------------
n240_lit_integer_α:
                        mov              qword ptr [rsp + 1184], 6
                        mov              rax, qword ptr [rip + .Lx363_0]
                        mov              qword ptr [rsp + 1192], rax
                                                                                        jmp   n249_lit_integer_α
.Lx363_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n241_op75_α:
                        mov              eax, dword ptr [rsp + 112]
                        cmp              eax, 7
                                                                                        je    .Lx365_1
                        cmp              eax, 6
                                                                                        jne   .Lx365_0
                        mov              eax, dword ptr [rsp + 96]
                        cmp              eax, 6
                                                                                        jne   .Lx365_0
.Lx365_1:
                        mov              rax, qword ptr [rsp + 112]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [rsp + 120]
                        mov              qword ptr [rsp + 88], rax
                                                                                        jmp   n250_op75_α
.Lx365_0:
                        lea              rdi, [rsp + 112]
                        lea              rsi, [rsp + 96]
                        lea              rdx, [rsp + 80]
                        mov              rcx, 147
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n250_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n242_op75_α:
                        mov              eax, dword ptr [rsp + 224]
                        cmp              eax, 7
                                                                                        je    .Lx367_1
                        cmp              eax, 6
                                                                                        jne   .Lx367_0
                        mov              eax, dword ptr [rsp + 208]
                        cmp              eax, 6
                                                                                        jne   .Lx367_0
.Lx367_1:
                        mov              rax, qword ptr [rsp + 224]
                        mov              qword ptr [rsp + 192], rax
                        mov              rax, qword ptr [rsp + 232]
                        mov              qword ptr [rsp + 200], rax
                                                                                        jmp   n251_op75_α
.Lx367_0:
                        lea              rdi, [rsp + 224]
                        lea              rsi, [rsp + 208]
                        lea              rdx, [rsp + 192]
                        mov              rcx, 111
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n251_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n243_op75_α:
                        mov              eax, dword ptr [rsp + 336]
                        cmp              eax, 7
                                                                                        je    .Lx369_1
                        cmp              eax, 6
                                                                                        jne   .Lx369_0
                        mov              eax, dword ptr [rsp + 320]
                        cmp              eax, 6
                                                                                        jne   .Lx369_0
.Lx369_1:
                        mov              rax, qword ptr [rsp + 336]
                        mov              qword ptr [rsp + 304], rax
                        mov              rax, qword ptr [rsp + 344]
                        mov              qword ptr [rsp + 312], rax
                                                                                        jmp   n252_op75_α
.Lx369_0:
                        lea              rdi, [rsp + 336]
                        lea              rsi, [rsp + 320]
                        lea              rdx, [rsp + 304]
                        mov              rcx, 147
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n252_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n244_op75_α:
                        mov              eax, dword ptr [rsp + 464]
                        cmp              eax, 7
                                                                                        je    .Lx371_1
                        cmp              eax, 6
                                                                                        jne   .Lx371_0
                        mov              eax, dword ptr [rsp + 448]
                        cmp              eax, 6
                                                                                        jne   .Lx371_0
.Lx371_1:
                        mov              rax, qword ptr [rsp + 464]
                        mov              qword ptr [rsp + 432], rax
                        mov              rax, qword ptr [rsp + 472]
                        mov              qword ptr [rsp + 440], rax
                                                                                        jmp   n253_op75_α
.Lx371_0:
                        lea              rdi, [rsp + 464]
                        lea              rsi, [rsp + 448]
                        lea              rdx, [rsp + 432]
                        mov              rcx, 147
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n253_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n245_lit_integer_α:
                        mov              qword ptr [rsp + 576], 6
                        mov              rax, qword ptr [rip + .Lx372_0]
                        mov              qword ptr [rsp + 584], rax
                                                                                        jmp   n254_op75_α
.Lx372_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n246_call_α:
                        mov              rax, qword ptr [rsp + 688]
                        mov              qword ptr [rsp + 640], rax
                        mov              rax, qword ptr [rsp + 696]
                        mov              qword ptr [rsp + 648], rax
                        mov              rax, qword ptr [rsp + 704]
                        mov              qword ptr [rsp + 656], rax
                        mov              rax, qword ptr [rsp + 712]
                        mov              qword ptr [rsp + 664], rax
                        .section         .rodata
.Lrkfn374:              .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn374]
                        lea              rsi, [rsp + 640]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 624], rax
                        mov              qword ptr [rsp + 632], rdx
                        cmp              eax, 99
                                                                                        je    n232_op14_α
                                                                                        jmp   n255_var_α
n246_call_β:
                                                                                        jmp   n232_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n247_call_α:
                        sub              rsp, 48
                        mov              rax, qword ptr [1879052368]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052376]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052384]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052392]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [1879052400]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [1879052408]
                        mov              qword ptr [rsp + 40], rax
                        mov              rdi, qword ptr [rip + .Lx376_0]
                        mov              esi, 3
                        mov              edx, 2
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx376_5
                        mov              rax, qword ptr [rsp + 912]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 920]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 928]
                        mov              qword ptr [1879052384], rax
                        mov              rax, qword ptr [rsp + 936]
                        mov              qword ptr [1879052392], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx376_6]
                        lea              rdx, [rip + .Lx376_7]
                                                                                        jmp   rax
.Lx376_6:
                        mov              rdi, qword ptr [1879052368]
                        mov              rsi, qword ptr [1879052376]
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052400], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052408], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052384], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052392], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052376], rax
                        add              rsp, 48
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx376_2
.Lx376_7:
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052400], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052408], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052384], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052392], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052376], rax
                        add              rsp, 48
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx376_2
.Lx376_5:
                        add              rsp, 48
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx376_20
                        mov              rax, qword ptr [rsp + 912]
                        mov              rdx, qword ptr [rsp + 920]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx376_21
.Lx376_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 912]
                        mov              rdx, qword ptr [rsp + 920]
                        call             rt_arg_stage@PLT
.Lx376_21:
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx376_22
                        mov              rax, qword ptr [rsp + 928]
                        mov              rdx, qword ptr [rsp + 936]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 16], rax
                        mov              qword ptr [r10 + 24], rdx
                                                                                        jmp   .Lx376_23
.Lx376_22:
                        mov              edi, 1
                        mov              rsi, qword ptr [rsp + 928]
                        mov              rdx, qword ptr [rsp + 936]
                        call             rt_arg_stage@PLT
.Lx376_23:
                        mov              rdi, qword ptr [rip + .Lx376_0]
                        mov              esi, 2
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx376_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx376_3]
                        lea              rdx, [rip + .Lx376_4]
                                                                                        jmp   rax
.Lx376_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx376_2
.Lx376_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx376_2
.Lx376_1:
                        call             rt_faildescr@PLT
.Lx376_2:
                        mov              qword ptr [rsp + 848], rax
                        mov              qword ptr [rsp + 856], rdx
                        cmp              eax, 99
                                                                                        je    n239_var_α
                                                                                        jmp   n256_assign_α
n247_call_β:
                                                                                        jmp   n239_var_α
.Lx376_0:
                        .quad            .Lx376_0_s
.Lx376_0_s:
                        .string          "gcd"
#-----------------------------------------------------------------------------------------------------------------------
n248_var_α:
                        mov              rax, qword ptr [1879052448]
                        mov              rdx, qword ptr [1879052456]
                        mov              qword ptr [rsp + 992], rax
                        mov              qword ptr [rsp + 1000], rdx
                                                                                        jmp   n257_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n249_lit_integer_α:
                        mov              qword ptr [rsp + 1200], 6
                        mov              rax, qword ptr [rip + .Lx378_0]
                        mov              qword ptr [rsp + 1208], rax
                                                                                        jmp   n258_call_α
.Lx378_0:
                        .quad            7
#-----------------------------------------------------------------------------------------------------------------------
n250_op75_α:
                        mov              eax, dword ptr [rsp + 96]
                        cmp              eax, 7
                                                                                        je    .Lx380_1
                        cmp              eax, 6
                                                                                        jne   .Lx380_0
                        mov              eax, dword ptr [rsp + 112]
                        cmp              eax, 6
                                                                                        jne   .Lx380_0
.Lx380_1:
                        mov              rax, qword ptr [rsp + 96]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 72], rax
                                                                                        jmp   n259_op77_α
.Lx380_0:
                        lea              rdi, [rsp + 96]
                        lea              rsi, [rsp + 112]
                        lea              rdx, [rsp + 64]
                        mov              rcx, 148
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n259_op77_α
#-----------------------------------------------------------------------------------------------------------------------
n251_op75_α:
                        mov              eax, dword ptr [rsp + 208]
                        cmp              eax, 7
                                                                                        je    .Lx382_1
                        cmp              eax, 6
                                                                                        jne   .Lx382_0
                        mov              eax, dword ptr [rsp + 224]
                        cmp              eax, 6
                                                                                        jne   .Lx382_0
.Lx382_1:
                        mov              rax, qword ptr [rsp + 208]
                        mov              qword ptr [rsp + 176], rax
                        mov              rax, qword ptr [rsp + 216]
                        mov              qword ptr [rsp + 184], rax
                                                                                        jmp   n260_op77_α
.Lx382_0:
                        lea              rdi, [rsp + 208]
                        lea              rsi, [rsp + 224]
                        lea              rdx, [rsp + 176]
                        mov              rcx, 112
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n260_op77_α
#-----------------------------------------------------------------------------------------------------------------------
n252_op75_α:
                        mov              eax, dword ptr [rsp + 320]
                        cmp              eax, 7
                                                                                        je    .Lx384_1
                        cmp              eax, 6
                                                                                        jne   .Lx384_0
                        mov              eax, dword ptr [rsp + 336]
                        cmp              eax, 6
                                                                                        jne   .Lx384_0
.Lx384_1:
                        mov              rax, qword ptr [rsp + 320]
                        mov              qword ptr [rsp + 288], rax
                        mov              rax, qword ptr [rsp + 328]
                        mov              qword ptr [rsp + 296], rax
                                                                                        jmp   n261_op77_α
.Lx384_0:
                        lea              rdi, [rsp + 320]
                        lea              rsi, [rsp + 336]
                        lea              rdx, [rsp + 288]
                        mov              rcx, 148
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n261_op77_α
#-----------------------------------------------------------------------------------------------------------------------
n253_op75_α:
                        mov              eax, dword ptr [rsp + 448]
                        cmp              eax, 7
                                                                                        je    .Lx386_1
                        cmp              eax, 6
                                                                                        jne   .Lx386_0
                        mov              eax, dword ptr [rsp + 464]
                        cmp              eax, 6
                                                                                        jne   .Lx386_0
.Lx386_1:
                        mov              rax, qword ptr [rsp + 448]
                        mov              qword ptr [rsp + 416], rax
                        mov              rax, qword ptr [rsp + 456]
                        mov              qword ptr [rsp + 424], rax
                                                                                        jmp   n262_op77_α
.Lx386_0:
                        lea              rdi, [rsp + 448]
                        lea              rsi, [rsp + 464]
                        lea              rdx, [rsp + 416]
                        mov              rcx, 148
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n262_op77_α
#-----------------------------------------------------------------------------------------------------------------------
n254_op75_α:
                        mov              eax, dword ptr [rsp + 592]
                        cmp              eax, 7
                                                                                        je    .Lx388_1
                        cmp              eax, 6
                                                                                        jne   .Lx388_0
                        mov              eax, dword ptr [rsp + 576]
                        cmp              eax, 6
                                                                                        jne   .Lx388_0
.Lx388_1:
                        mov              rax, qword ptr [rsp + 592]
                        mov              qword ptr [rsp + 560], rax
                        mov              rax, qword ptr [rsp + 600]
                        mov              qword ptr [rsp + 568], rax
                                                                                        jmp   n263_op75_α
.Lx388_0:
                        lea              rdi, [rsp + 592]
                        lea              rsi, [rsp + 576]
                        lea              rdx, [rsp + 560]
                        mov              rcx, 111
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n263_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n255_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 784], rax
                        mov              qword ptr [rsp + 792], rdx
                                                                                        jmp   n264_var_α
#-----------------------------------------------------------------------------------------------------------------------
n256_assign_α:
                        mov              rax, qword ptr [rsp + 848]
                        mov              rdx, qword ptr [rsp + 856]
                        mov              qword ptr [1879052448], rax
                        mov              qword ptr [1879052456], rdx
                                                                                        jmp   n239_var_α
#-----------------------------------------------------------------------------------------------------------------------
n257_binop_α:
                        mov              eax, dword ptr [rsp + 976]
                        cmp              eax, 100
                                                                                        je    .Lx391_0
                        mov              eax, dword ptr [rsp + 992]
                        cmp              eax, 100
                                                                                        je    .Lx391_0
                        mov              eax, dword ptr [rsp + 976]
                        cmp              eax, 6
                                                                                        jne   .Lx391_2
                        mov              eax, dword ptr [rsp + 992]
                        cmp              eax, 6
                                                                                        jne   .Lx391_2
.Lx391_1:
                        mov              rax, qword ptr [rsp + 984]
                        mov              rcx, qword ptr [rsp + 1000]
                        cqo
                        idiv             rcx
                        mov              qword ptr [rsp + 960], 6
                        mov              qword ptr [rsp + 968], rax
                                                                                        jmp   n266_var_α
.Lx391_0:
                        mov              rdi, qword ptr [rsp + 976]
                        mov              rsi, qword ptr [rsp + 984]
                        mov              rdx, qword ptr [rsp + 992]
                        mov              rcx, qword ptr [rsp + 1000]
                        mov              r8d, 3
                        lea              r9, [rsp + 960]
                        call             rt_binop_overload@PLT
                        test             eax, eax
                                                                                        jne   n266_var_α
.Lx391_2:
                        mov              rdi, qword ptr [rsp + 976]
                        mov              rsi, qword ptr [rsp + 984]
                        mov              rdx, qword ptr [rsp + 992]
                        mov              rcx, qword ptr [rsp + 1000]
                        mov              r8d, 3
                        call             rt_num_arith@PLT
                        cmp              eax, 99
                                                                                        je    n232_op14_α
                        mov              qword ptr [rsp + 960], rax
                        mov              qword ptr [rsp + 968], rdx
                                                                                        jmp   n266_var_α
n257_binop_β:
                                                                                        jmp   n232_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n258_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx393_0]
                        mov              esi, 2
                        mov              edx, 2
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx393_5
                        mov              rax, qword ptr [rsp + 1184]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 1192]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 1200]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 1208]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx393_6]
                        lea              rdx, [rip + .Lx393_7]
                                                                                        jmp   rax
.Lx393_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052312], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052296], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx393_2
.Lx393_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052312], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052296], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx393_2
.Lx393_5:
                        add              rsp, 32
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx393_20
                        mov              rax, qword ptr [rsp + 1184]
                        mov              rdx, qword ptr [rsp + 1192]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx393_21
.Lx393_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 1184]
                        mov              rdx, qword ptr [rsp + 1192]
                        call             rt_arg_stage@PLT
.Lx393_21:
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx393_22
                        mov              rax, qword ptr [rsp + 1200]
                        mov              rdx, qword ptr [rsp + 1208]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 16], rax
                        mov              qword ptr [r10 + 24], rdx
                                                                                        jmp   .Lx393_23
.Lx393_22:
                        mov              edi, 1
                        mov              rsi, qword ptr [rsp + 1200]
                        mov              rdx, qword ptr [rsp + 1208]
                        call             rt_arg_stage@PLT
.Lx393_23:
                        mov              rdi, qword ptr [rip + .Lx393_0]
                        mov              esi, 2
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx393_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx393_3]
                        lea              rdx, [rip + .Lx393_4]
                                                                                        jmp   rax
.Lx393_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx393_2
.Lx393_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx393_2
.Lx393_1:
                        call             rt_faildescr@PLT
.Lx393_2:
                        mov              qword ptr [rsp + 1120], rax
                        mov              qword ptr [rsp + 1128], rdx
                        cmp              eax, 99
                                                                                        je    n268_lit_integer_α
                                                                                        jmp   n267_assign_α
n258_call_β:
                                                                                        jmp   n268_lit_integer_α
.Lx393_0:
                        .quad            .Lx393_0_s
.Lx393_0_s:
                        .string          "max"
#-----------------------------------------------------------------------------------------------------------------------
n259_op77_α:
                        lea              rdi, [rsp + 80]
                        lea              rsi, [rsp + 64]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jns   n232_op14_α
                        mov              qword ptr [rsp + 48], 0
                        mov              qword ptr [rsp + 56], 0
                                                                                        jmp   n269_var_α
#-----------------------------------------------------------------------------------------------------------------------
n260_op77_α:
                        lea              rdi, [rsp + 192]
                        lea              rsi, [rsp + 176]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jle   n232_op14_α
                        mov              qword ptr [rsp + 160], 0
                        mov              qword ptr [rsp + 168], 0
                                                                                        jmp   n270_var_α
#-----------------------------------------------------------------------------------------------------------------------
n261_op77_α:
                        lea              rdi, [rsp + 304]
                        lea              rsi, [rsp + 288]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jns   n232_op14_α
                        mov              qword ptr [rsp + 272], 0
                        mov              qword ptr [rsp + 280], 0
                                                                                        jmp   n271_var_α
#-----------------------------------------------------------------------------------------------------------------------
n262_op77_α:
                        lea              rdi, [rsp + 432]
                        lea              rsi, [rsp + 416]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jns   n236_var_α
                        mov              qword ptr [rsp + 400], 0
                        mov              qword ptr [rsp + 408], 0
                                                                                        jmp   n272_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n263_op75_α:
                        mov              eax, dword ptr [rsp + 576]
                        cmp              eax, 7
                                                                                        je    .Lx403_1
                        cmp              eax, 6
                                                                                        jne   .Lx403_0
                        mov              eax, dword ptr [rsp + 592]
                        cmp              eax, 6
                                                                                        jne   .Lx403_0
.Lx403_1:
                        mov              rax, qword ptr [rsp + 576]
                        mov              qword ptr [rsp + 544], rax
                        mov              rax, qword ptr [rsp + 584]
                        mov              qword ptr [rsp + 552], rax
                                                                                        jmp   n273_op77_α
.Lx403_0:
                        lea              rdi, [rsp + 576]
                        lea              rsi, [rsp + 592]
                        lea              rdx, [rsp + 544]
                        mov              rcx, 112
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n273_op77_α
#-----------------------------------------------------------------------------------------------------------------------
n264_var_α:
                        mov              rax, qword ptr [1879052384]
                        mov              rdx, qword ptr [1879052392]
                        mov              qword ptr [rsp + 800], rax
                        mov              qword ptr [rsp + 808], rdx
                                                                                        jmp   n274_call_α
#-----------------------------------------------------------------------------------------------------------------------
n265_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052384]
                        mov              rdx, qword ptr [1879052392]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n275_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n266_var_α:
                        mov              rax, qword ptr [1879052384]
                        mov              rdx, qword ptr [1879052392]
                        mov              qword ptr [rsp + 1008], rax
                        mov              qword ptr [rsp + 1016], rdx
                                                                                        jmp   n277_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n267_assign_α:
                        mov              rsi, qword ptr [rsp + 1120]
                        mov              rdx, qword ptr [rsp + 1128]
                        mov              rdi, qword ptr [rip + .Lx407_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n268_lit_integer_α
.Lx407_0:
                        .quad            .Lx407_0_s
.Lx407_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n268_lit_integer_α:
                        mov              qword ptr [rsp + 1280], 6
                        mov              rax, qword ptr [rip + .Lx408_0]
                        mov              qword ptr [rsp + 1288], rax
                                                                                        jmp   n278_lit_integer_α
.Lx408_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n269_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 128], rax
                        mov              qword ptr [rsp + 136], rdx
                                                                                        jmp   n279_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n270_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 240], rax
                        mov              qword ptr [rsp + 248], rdx
                                                                                        jmp   n280_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n271_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 368], rax
                        mov              qword ptr [rsp + 376], rdx
                                                                                        jmp   n281_unop_α
#-----------------------------------------------------------------------------------------------------------------------
n272_lit_integer_α:
                        mov              qword ptr [rsp + 496], 6
                        mov              rax, qword ptr [rip + .Lx412_0]
                        mov              qword ptr [rsp + 504], rax
                                                                                        jmp   n282_unop_α
.Lx412_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n273_op77_α:
                        lea              rdi, [rsp + 560]
                        lea              rsi, [rsp + 544]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jle   n232_op14_α
                        mov              qword ptr [rsp + 528], 0
                        mov              qword ptr [rsp + 536], 0
                                                                                        jmp   n283_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n274_call_α:
                        mov              rax, qword ptr [rsp + 784]
                        mov              qword ptr [rsp + 736], rax
                        mov              rax, qword ptr [rsp + 792]
                        mov              qword ptr [rsp + 744], rax
                        mov              rax, qword ptr [rsp + 800]
                        mov              qword ptr [rsp + 752], rax
                        mov              rax, qword ptr [rsp + 808]
                        mov              qword ptr [rsp + 760], rax
                        .section         .rodata
.Lrkfn416:              .string          "REMDR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn416]
                        lea              rsi, [rsp + 736]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 720], rax
                        mov              qword ptr [rsp + 728], rdx
                        cmp              eax, 99
                                                                                        je    n265_var_α
                                                                                        jmp   n284_assign_α
n274_call_β:
                                                                                        jmp   n265_var_α
#-----------------------------------------------------------------------------------------------------------------------
n275_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052368], rax
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n276_var_α
#-----------------------------------------------------------------------------------------------------------------------
n276_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052400]
                        mov              rdx, qword ptr [1879052408]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n285_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n277_binop_α:
                        mov              eax, dword ptr [rsp + 960]
                        cmp              eax, 100
                                                                                        je    .Lx419_0
                        mov              eax, dword ptr [rsp + 1008]
                        cmp              eax, 100
                                                                                        je    .Lx419_0
                        mov              eax, dword ptr [rsp + 960]
                        cmp              eax, 6
                                                                                        jne   .Lx419_2
                        mov              eax, dword ptr [rsp + 1008]
                        cmp              eax, 6
                                                                                        jne   .Lx419_2
.Lx419_1:
                        mov              rax, qword ptr [rsp + 968]
                        mov              rcx, qword ptr [rsp + 1016]
                        imul             rax, rcx
                        mov              qword ptr [rsp + 944], 6
                        mov              qword ptr [rsp + 952], rax
                                                                                        jmp   n286_assign_α
.Lx419_0:
                        mov              rdi, qword ptr [rsp + 960]
                        mov              rsi, qword ptr [rsp + 968]
                        mov              rdx, qword ptr [rsp + 1008]
                        mov              rcx, qword ptr [rsp + 1016]
                        mov              r8d, 2
                        lea              r9, [rsp + 944]
                        call             rt_binop_overload@PLT
                        test             eax, eax
                                                                                        jne   n286_assign_α
.Lx419_2:
                        mov              rdi, qword ptr [rsp + 960]
                        mov              rsi, qword ptr [rsp + 968]
                        mov              rdx, qword ptr [rsp + 1008]
                        mov              rcx, qword ptr [rsp + 1016]
                        mov              r8d, 2
                        call             rt_num_arith@PLT
                        cmp              eax, 99
                                                                                        je    n232_op14_α
                        mov              qword ptr [rsp + 944], rax
                        mov              qword ptr [rsp + 952], rdx
                                                                                        jmp   n286_assign_α
n277_binop_β:
                                                                                        jmp   n232_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n278_lit_integer_α:
                        mov              qword ptr [rsp + 1296], 6
                        mov              rax, qword ptr [rip + .Lx420_0]
                        mov              qword ptr [rsp + 1304], rax
                                                                                        jmp   n287_call_α
.Lx420_0:
                        .quad            7
#-----------------------------------------------------------------------------------------------------------------------
n279_binop_α:
                        mov              rdi, qword ptr [rsp + 48]
                        mov              rsi, qword ptr [rsp + 56]
                        mov              rdx, qword ptr [rsp + 128]
                        mov              rcx, qword ptr [rsp + 136]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 32], rax
                        mov              qword ptr [rsp + 40], rdx
                                                                                        jmp   n288_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n280_binop_α:
                        mov              rdi, qword ptr [rsp + 160]
                        mov              rsi, qword ptr [rsp + 168]
                        mov              rdx, qword ptr [rsp + 240]
                        mov              rcx, qword ptr [rsp + 248]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 144], rax
                        mov              qword ptr [rsp + 152], rdx
                                                                                        jmp   n289_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n281_unop_α:
                        mov              rdi, qword ptr [rsp + 368]
                        mov              rsi, qword ptr [rsp + 376]
                        call             rt_num_neg@PLT
                        mov              qword ptr [rsp + 352], rax
                        mov              qword ptr [rsp + 360], rdx
                                                                                        jmp   n290_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n282_unop_α:
                        mov              rdi, qword ptr [rsp + 496]
                        mov              rsi, qword ptr [rsp + 504]
                        call             rt_num_neg@PLT
                        mov              qword ptr [rsp + 480], rax
                        mov              qword ptr [rsp + 488], rdx
                                                                                        jmp   n291_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n283_lit_integer_α:
                        mov              qword ptr [rsp + 608], 6
                        mov              rax, qword ptr [rip + .Lx425_0]
                        mov              qword ptr [rsp + 616], rax
                                                                                        jmp   n292_binop_α
.Lx425_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n284_assign_α:
                        mov              rax, qword ptr [rsp + 720]
                        mov              rdx, qword ptr [rsp + 728]
                        mov              qword ptr [1879052400], rax
                        mov              qword ptr [1879052408], rdx
                                                                                        jmp   n265_var_α
#-----------------------------------------------------------------------------------------------------------------------
n285_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052384], rax
                        mov              qword ptr [1879052392], rdx
                                                                                        jmp   n228_var_α
#-----------------------------------------------------------------------------------------------------------------------
n286_assign_α:
                        mov              rax, qword ptr [rsp + 944]
                        mov              rdx, qword ptr [rsp + 952]
                        mov              qword ptr [1879052416], rax
                        mov              qword ptr [1879052424], rdx
                                                                                        jmp   n232_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n287_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx430_0]
                        mov              esi, 2
                        mov              edx, 2
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx430_5
                        mov              rax, qword ptr [rsp + 1280]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 1288]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 1296]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 1304]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx430_6]
                        lea              rdx, [rip + .Lx430_7]
                                                                                        jmp   rax
.Lx430_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052312], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx430_2
.Lx430_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052312], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx430_2
.Lx430_5:
                        add              rsp, 32
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx430_20
                        mov              rax, qword ptr [rsp + 1280]
                        mov              rdx, qword ptr [rsp + 1288]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx430_21
.Lx430_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 1280]
                        mov              rdx, qword ptr [rsp + 1288]
                        call             rt_arg_stage@PLT
.Lx430_21:
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx430_22
                        mov              rax, qword ptr [rsp + 1296]
                        mov              rdx, qword ptr [rsp + 1304]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 16], rax
                        mov              qword ptr [r10 + 24], rdx
                                                                                        jmp   .Lx430_23
.Lx430_22:
                        mov              edi, 1
                        mov              rsi, qword ptr [rsp + 1296]
                        mov              rdx, qword ptr [rsp + 1304]
                        call             rt_arg_stage@PLT
.Lx430_23:
                        mov              rdi, qword ptr [rip + .Lx430_0]
                        mov              esi, 2
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx430_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx430_3]
                        lea              rdx, [rip + .Lx430_4]
                                                                                        jmp   rax
.Lx430_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx430_2
.Lx430_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx430_2
.Lx430_1:
                        call             rt_faildescr@PLT
.Lx430_2:
                        mov              qword ptr [rsp + 1216], rax
                        mov              qword ptr [rsp + 1224], rdx
                        cmp              eax, 99
                                                                                        je    n294_lit_real_α
                                                                                        jmp   n293_assign_α
n287_call_β:
                                                                                        jmp   n294_lit_real_α
.Lx430_0:
                        .quad            .Lx430_0_s
.Lx430_0_s:
                        .string          "min"
#-----------------------------------------------------------------------------------------------------------------------
n288_assign_α:
                        mov              rax, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rsp + 40]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n232_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n289_assign_α:
                        mov              rax, qword ptr [rsp + 144]
                        mov              rdx, qword ptr [rsp + 152]
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rdx
                                                                                        jmp   n232_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n290_binop_α:
                        mov              rdi, qword ptr [rsp + 272]
                        mov              rsi, qword ptr [rsp + 280]
                        mov              rdx, qword ptr [rsp + 352]
                        mov              rcx, qword ptr [rsp + 360]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 256], rax
                        mov              qword ptr [rsp + 264], rdx
                                                                                        jmp   n295_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n291_binop_α:
                        mov              rdi, qword ptr [rsp + 400]
                        mov              rsi, qword ptr [rsp + 408]
                        mov              rdx, qword ptr [rsp + 480]
                        mov              rcx, qword ptr [rsp + 488]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 384], rax
                        mov              qword ptr [rsp + 392], rdx
                                                                                        jmp   n296_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n292_binop_α:
                        mov              rdi, qword ptr [rsp + 528]
                        mov              rsi, qword ptr [rsp + 536]
                        mov              rdx, qword ptr [rsp + 608]
                        mov              rcx, qword ptr [rsp + 616]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 512], rax
                        mov              qword ptr [rsp + 520], rdx
                                                                                        jmp   n297_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n293_assign_α:
                        mov              rsi, qword ptr [rsp + 1216]
                        mov              rdx, qword ptr [rsp + 1224]
                        mov              rdi, qword ptr [rip + .Lx436_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n294_lit_real_α
.Lx436_0:
                        .quad            .Lx436_0_s
.Lx436_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n294_lit_real_α:
                        mov              qword ptr [rsp + 1376], 7
                        mov              rax, qword ptr [rip + .Lx437_0]
                        mov              qword ptr [rsp + 1384], rax
                                                                                        jmp   n298_lit_real_α
.Lx437_0:
                        .quad            4615063718147915776
#-----------------------------------------------------------------------------------------------------------------------
n295_assign_α:
                        mov              rax, qword ptr [rsp + 256]
                        mov              rdx, qword ptr [rsp + 264]
                        mov              qword ptr [1879052336], rax
                        mov              qword ptr [1879052344], rdx
                                                                                        jmp   n232_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n296_assign_α:
                        mov              rax, qword ptr [rsp + 384]
                        mov              rdx, qword ptr [rsp + 392]
                        mov              qword ptr [1879052352], rax
                        mov              qword ptr [1879052360], rdx
                                                                                        jmp   n232_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n297_assign_α:
                        mov              rax, qword ptr [rsp + 512]
                        mov              rdx, qword ptr [rsp + 520]
                        mov              qword ptr [1879052352], rax
                        mov              qword ptr [1879052360], rdx
                                                                                        jmp   n232_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n298_lit_real_α:
                        mov              qword ptr [rsp + 1392], 7
                        mov              rax, qword ptr [rip + .Lx441_0]
                        mov              qword ptr [rsp + 1400], rax
                                                                                        jmp   n299_call_α
.Lx441_0:
                        .quad            4611911198408756429
#-----------------------------------------------------------------------------------------------------------------------
n299_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx443_0]
                        mov              esi, 2
                        mov              edx, 2
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx443_5
                        mov              rax, qword ptr [rsp + 1376]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 1384]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 1392]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 1400]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx443_6]
                        lea              rdx, [rip + .Lx443_7]
                                                                                        jmp   rax
.Lx443_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052312], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052296], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx443_2
.Lx443_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052312], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052296], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx443_2
.Lx443_5:
                        add              rsp, 32
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx443_20
                        mov              rax, qword ptr [rsp + 1376]
                        mov              rdx, qword ptr [rsp + 1384]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx443_21
.Lx443_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 1376]
                        mov              rdx, qword ptr [rsp + 1384]
                        call             rt_arg_stage@PLT
.Lx443_21:
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx443_22
                        mov              rax, qword ptr [rsp + 1392]
                        mov              rdx, qword ptr [rsp + 1400]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 16], rax
                        mov              qword ptr [r10 + 24], rdx
                                                                                        jmp   .Lx443_23
.Lx443_22:
                        mov              edi, 1
                        mov              rsi, qword ptr [rsp + 1392]
                        mov              rdx, qword ptr [rsp + 1400]
                        call             rt_arg_stage@PLT
.Lx443_23:
                        mov              rdi, qword ptr [rip + .Lx443_0]
                        mov              esi, 2
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx443_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx443_3]
                        lea              rdx, [rip + .Lx443_4]
                                                                                        jmp   rax
.Lx443_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx443_2
.Lx443_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx443_2
.Lx443_1:
                        call             rt_faildescr@PLT
.Lx443_2:
                        mov              qword ptr [rsp + 1312], rax
                        mov              qword ptr [rsp + 1320], rdx
                        cmp              eax, 99
                                                                                        je    n301_lit_real_α
                                                                                        jmp   n300_assign_α
n299_call_β:
                                                                                        jmp   n301_lit_real_α
.Lx443_0:
                        .quad            .Lx443_0_s
.Lx443_0_s:
                        .string          "max"
#-----------------------------------------------------------------------------------------------------------------------
n300_assign_α:
                        mov              rsi, qword ptr [rsp + 1312]
                        mov              rdx, qword ptr [rsp + 1320]
                        mov              rdi, qword ptr [rip + .Lx444_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n301_lit_real_α
.Lx444_0:
                        .quad            .Lx444_0_s
.Lx444_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n301_lit_real_α:
                        mov              qword ptr [rsp + 1472], 7
                        mov              rax, qword ptr [rip + .Lx445_0]
                        mov              qword ptr [rsp + 1480], rax
                                                                                        jmp   n302_lit_real_α
.Lx445_0:
                        .quad            4615063718147915776
#-----------------------------------------------------------------------------------------------------------------------
n302_lit_real_α:
                        mov              qword ptr [rsp + 1488], 7
                        mov              rax, qword ptr [rip + .Lx446_0]
                        mov              qword ptr [rsp + 1496], rax
                                                                                        jmp   n303_call_α
.Lx446_0:
                        .quad            4611911198408756429
#-----------------------------------------------------------------------------------------------------------------------
n303_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx448_0]
                        mov              esi, 2
                        mov              edx, 2
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx448_5
                        mov              rax, qword ptr [rsp + 1472]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 1480]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 1488]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 1496]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx448_6]
                        lea              rdx, [rip + .Lx448_7]
                                                                                        jmp   rax
.Lx448_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052312], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx448_2
.Lx448_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052312], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx448_2
.Lx448_5:
                        add              rsp, 32
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx448_20
                        mov              rax, qword ptr [rsp + 1472]
                        mov              rdx, qword ptr [rsp + 1480]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx448_21
.Lx448_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 1472]
                        mov              rdx, qword ptr [rsp + 1480]
                        call             rt_arg_stage@PLT
.Lx448_21:
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx448_22
                        mov              rax, qword ptr [rsp + 1488]
                        mov              rdx, qword ptr [rsp + 1496]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 16], rax
                        mov              qword ptr [r10 + 24], rdx
                                                                                        jmp   .Lx448_23
.Lx448_22:
                        mov              edi, 1
                        mov              rsi, qword ptr [rsp + 1488]
                        mov              rdx, qword ptr [rsp + 1496]
                        call             rt_arg_stage@PLT
.Lx448_23:
                        mov              rdi, qword ptr [rip + .Lx448_0]
                        mov              esi, 2
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx448_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx448_3]
                        lea              rdx, [rip + .Lx448_4]
                                                                                        jmp   rax
.Lx448_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx448_2
.Lx448_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx448_2
.Lx448_1:
                        call             rt_faildescr@PLT
.Lx448_2:
                        mov              qword ptr [rsp + 1408], rax
                        mov              qword ptr [rsp + 1416], rdx
                        cmp              eax, 99
                                                                                        je    n305_lit_integer_α
                                                                                        jmp   n304_assign_α
n303_call_β:
                                                                                        jmp   n305_lit_integer_α
.Lx448_0:
                        .quad            .Lx448_0_s
.Lx448_0_s:
                        .string          "min"
#-----------------------------------------------------------------------------------------------------------------------
n304_assign_α:
                        mov              rsi, qword ptr [rsp + 1408]
                        mov              rdx, qword ptr [rsp + 1416]
                        mov              rdi, qword ptr [rip + .Lx449_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n305_lit_integer_α
.Lx449_0:
                        .quad            .Lx449_0_s
.Lx449_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n305_lit_integer_α:
                        mov              qword ptr [rsp + 1568], 6
                        mov              rax, qword ptr [rip + .Lx450_0]
                        mov              qword ptr [rsp + 1576], rax
                                                                                        jmp   n306_unop_α
.Lx450_0:
                        .quad            42
#-----------------------------------------------------------------------------------------------------------------------
n306_unop_α:
                        mov              rdi, qword ptr [rsp + 1568]
                        mov              rsi, qword ptr [rsp + 1576]
                        call             rt_num_neg@PLT
                        mov              qword ptr [rsp + 1552], rax
                        mov              qword ptr [rsp + 1560], rdx
                                                                                        jmp   n307_call_α
#-----------------------------------------------------------------------------------------------------------------------
n307_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx453_0]
                        mov              esi, 1
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx453_5
                        mov              rax, qword ptr [rsp + 1552]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 1560]
                        mov              qword ptr [1879052344], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx453_6]
                        lea              rdx, [rip + .Lx453_7]
                                                                                        jmp   rax
.Lx453_6:
                        mov              rdi, qword ptr [1879052336]
                        mov              rsi, qword ptr [1879052344]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052344], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx453_2
.Lx453_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052344], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx453_2
.Lx453_5:
                        add              rsp, 16
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx453_20
                        mov              rax, qword ptr [rsp + 1552]
                        mov              rdx, qword ptr [rsp + 1560]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx453_21
.Lx453_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 1552]
                        mov              rdx, qword ptr [rsp + 1560]
                        call             rt_arg_stage@PLT
.Lx453_21:
                        mov              rdi, qword ptr [rip + .Lx453_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx453_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx453_3]
                        lea              rdx, [rip + .Lx453_4]
                                                                                        jmp   rax
.Lx453_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx453_2
.Lx453_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx453_2
.Lx453_1:
                        call             rt_faildescr@PLT
.Lx453_2:
                        mov              qword ptr [rsp + 1504], rax
                        mov              qword ptr [rsp + 1512], rdx
                        cmp              eax, 99
                                                                                        je    n308_lit_integer_α
                                                                                        jmp   n309_assign_α
n307_call_β:
                                                                                        jmp   n308_lit_integer_α
.Lx453_0:
                        .quad            .Lx453_0_s
.Lx453_0_s:
                        .string          "abs"
#-----------------------------------------------------------------------------------------------------------------------
n308_lit_integer_α:
                        mov              qword ptr [rsp + 1632], 6
                        mov              rax, qword ptr [rip + .Lx454_0]
                        mov              qword ptr [rsp + 1640], rax
                                                                                        jmp   n310_call_α
.Lx454_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n309_assign_α:
                        mov              rsi, qword ptr [rsp + 1504]
                        mov              rdx, qword ptr [rsp + 1512]
                        mov              rdi, qword ptr [rip + .Lx455_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n308_lit_integer_α
.Lx455_0:
                        .quad            .Lx455_0_s
.Lx455_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n310_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx457_0]
                        mov              esi, 1
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx457_5
                        mov              rax, qword ptr [rsp + 1632]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 1640]
                        mov              qword ptr [1879052360], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx457_6]
                        lea              rdx, [rip + .Lx457_7]
                                                                                        jmp   rax
.Lx457_6:
                        mov              rdi, qword ptr [1879052352]
                        mov              rsi, qword ptr [1879052360]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052360], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx457_2
.Lx457_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052360], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx457_2
.Lx457_5:
                        add              rsp, 16
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx457_20
                        mov              rax, qword ptr [rsp + 1632]
                        mov              rdx, qword ptr [rsp + 1640]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx457_21
.Lx457_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 1632]
                        mov              rdx, qword ptr [rsp + 1640]
                        call             rt_arg_stage@PLT
.Lx457_21:
                        mov              rdi, qword ptr [rip + .Lx457_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx457_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx457_3]
                        lea              rdx, [rip + .Lx457_4]
                                                                                        jmp   rax
.Lx457_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx457_2
.Lx457_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx457_2
.Lx457_1:
                        call             rt_faildescr@PLT
.Lx457_2:
                        mov              qword ptr [rsp + 1584], rax
                        mov              qword ptr [rsp + 1592], rdx
                        cmp              eax, 99
                                                                                        je    n312_lit_integer_α
                                                                                        jmp   n311_assign_α
n310_call_β:
                                                                                        jmp   n312_lit_integer_α
.Lx457_0:
                        .quad            .Lx457_0_s
.Lx457_0_s:
                        .string          "sign"
#-----------------------------------------------------------------------------------------------------------------------
n311_assign_α:
                        mov              rsi, qword ptr [rsp + 1584]
                        mov              rdx, qword ptr [rsp + 1592]
                        mov              rdi, qword ptr [rip + .Lx458_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n312_lit_integer_α
.Lx458_0:
                        .quad            .Lx458_0_s
.Lx458_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n312_lit_integer_α:
                        mov              qword ptr [rsp + 1696], 6
                        mov              rax, qword ptr [rip + .Lx459_0]
                        mov              qword ptr [rsp + 1704], rax
                                                                                        jmp   n313_call_α
.Lx459_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n313_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx461_0]
                        mov              esi, 1
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx461_5
                        mov              rax, qword ptr [rsp + 1696]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 1704]
                        mov              qword ptr [1879052360], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx461_6]
                        lea              rdx, [rip + .Lx461_7]
                                                                                        jmp   rax
.Lx461_6:
                        mov              rdi, qword ptr [1879052352]
                        mov              rsi, qword ptr [1879052360]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052360], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx461_2
.Lx461_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052360], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx461_2
.Lx461_5:
                        add              rsp, 16
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx461_20
                        mov              rax, qword ptr [rsp + 1696]
                        mov              rdx, qword ptr [rsp + 1704]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx461_21
.Lx461_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 1696]
                        mov              rdx, qword ptr [rsp + 1704]
                        call             rt_arg_stage@PLT
.Lx461_21:
                        mov              rdi, qword ptr [rip + .Lx461_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx461_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx461_3]
                        lea              rdx, [rip + .Lx461_4]
                                                                                        jmp   rax
.Lx461_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx461_2
.Lx461_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx461_2
.Lx461_1:
                        call             rt_faildescr@PLT
.Lx461_2:
                        mov              qword ptr [rsp + 1648], rax
                        mov              qword ptr [rsp + 1656], rdx
                        cmp              eax, 99
                                                                                        je    n315_lit_integer_α
                                                                                        jmp   n314_assign_α
n313_call_β:
                                                                                        jmp   n315_lit_integer_α
.Lx461_0:
                        .quad            .Lx461_0_s
.Lx461_0_s:
                        .string          "sign"
#-----------------------------------------------------------------------------------------------------------------------
n314_assign_α:
                        mov              rsi, qword ptr [rsp + 1648]
                        mov              rdx, qword ptr [rsp + 1656]
                        mov              rdi, qword ptr [rip + .Lx462_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n315_lit_integer_α
.Lx462_0:
                        .quad            .Lx462_0_s
.Lx462_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n315_lit_integer_α:
                        mov              qword ptr [rsp + 1776], 6
                        mov              rax, qword ptr [rip + .Lx463_0]
                        mov              qword ptr [rsp + 1784], rax
                                                                                        jmp   n316_unop_α
.Lx463_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n316_unop_α:
                        mov              rdi, qword ptr [rsp + 1776]
                        mov              rsi, qword ptr [rsp + 1784]
                        call             rt_num_neg@PLT
                        mov              qword ptr [rsp + 1760], rax
                        mov              qword ptr [rsp + 1768], rdx
                                                                                        jmp   n317_call_α
#-----------------------------------------------------------------------------------------------------------------------
n317_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx466_0]
                        mov              esi, 1
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx466_5
                        mov              rax, qword ptr [rsp + 1760]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 1768]
                        mov              qword ptr [1879052360], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx466_6]
                        lea              rdx, [rip + .Lx466_7]
                                                                                        jmp   rax
.Lx466_6:
                        mov              rdi, qword ptr [1879052352]
                        mov              rsi, qword ptr [1879052360]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052360], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx466_2
.Lx466_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052360], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx466_2
.Lx466_5:
                        add              rsp, 16
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx466_20
                        mov              rax, qword ptr [rsp + 1760]
                        mov              rdx, qword ptr [rsp + 1768]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx466_21
.Lx466_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 1760]
                        mov              rdx, qword ptr [rsp + 1768]
                        call             rt_arg_stage@PLT
.Lx466_21:
                        mov              rdi, qword ptr [rip + .Lx466_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx466_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx466_3]
                        lea              rdx, [rip + .Lx466_4]
                                                                                        jmp   rax
.Lx466_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx466_2
.Lx466_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx466_2
.Lx466_1:
                        call             rt_faildescr@PLT
.Lx466_2:
                        mov              qword ptr [rsp + 1712], rax
                        mov              qword ptr [rsp + 1720], rdx
                        cmp              eax, 99
                                                                                        je    n318_lit_integer_α
                                                                                        jmp   n319_assign_α
n317_call_β:
                                                                                        jmp   n318_lit_integer_α
.Lx466_0:
                        .quad            .Lx466_0_s
.Lx466_0_s:
                        .string          "sign"
#-----------------------------------------------------------------------------------------------------------------------
n318_lit_integer_α:
                        mov              qword ptr [rsp + 1856], 6
                        mov              rax, qword ptr [rip + .Lx467_0]
                        mov              qword ptr [rsp + 1864], rax
                                                                                        jmp   n320_lit_integer_α
.Lx467_0:
                        .quad            12
#-----------------------------------------------------------------------------------------------------------------------
n319_assign_α:
                        mov              rsi, qword ptr [rsp + 1712]
                        mov              rdx, qword ptr [rsp + 1720]
                        mov              rdi, qword ptr [rip + .Lx468_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n318_lit_integer_α
.Lx468_0:
                        .quad            .Lx468_0_s
.Lx468_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n320_lit_integer_α:
                        mov              qword ptr [rsp + 1872], 6
                        mov              rax, qword ptr [rip + .Lx469_0]
                        mov              qword ptr [rsp + 1880], rax
                                                                                        jmp   n321_call_α
.Lx469_0:
                        .quad            8
#-----------------------------------------------------------------------------------------------------------------------
n321_call_α:
                        sub              rsp, 48
                        mov              rax, qword ptr [1879052368]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052376]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052384]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052392]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [1879052400]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [1879052408]
                        mov              qword ptr [rsp + 40], rax
                        mov              rdi, qword ptr [rip + .Lx471_0]
                        mov              esi, 3
                        mov              edx, 2
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx471_5
                        mov              rax, qword ptr [rsp + 1856]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 1864]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 1872]
                        mov              qword ptr [1879052384], rax
                        mov              rax, qword ptr [rsp + 1880]
                        mov              qword ptr [1879052392], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx471_6]
                        lea              rdx, [rip + .Lx471_7]
                                                                                        jmp   rax
.Lx471_6:
                        mov              rdi, qword ptr [1879052368]
                        mov              rsi, qword ptr [1879052376]
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052400], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052408], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052384], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052392], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052376], rax
                        add              rsp, 48
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx471_2
.Lx471_7:
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052400], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052408], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052384], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052392], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052376], rax
                        add              rsp, 48
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx471_2
.Lx471_5:
                        add              rsp, 48
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx471_20
                        mov              rax, qword ptr [rsp + 1856]
                        mov              rdx, qword ptr [rsp + 1864]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx471_21
.Lx471_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 1856]
                        mov              rdx, qword ptr [rsp + 1864]
                        call             rt_arg_stage@PLT
.Lx471_21:
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx471_22
                        mov              rax, qword ptr [rsp + 1872]
                        mov              rdx, qword ptr [rsp + 1880]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 16], rax
                        mov              qword ptr [r10 + 24], rdx
                                                                                        jmp   .Lx471_23
.Lx471_22:
                        mov              edi, 1
                        mov              rsi, qword ptr [rsp + 1872]
                        mov              rdx, qword ptr [rsp + 1880]
                        call             rt_arg_stage@PLT
.Lx471_23:
                        mov              rdi, qword ptr [rip + .Lx471_0]
                        mov              esi, 2
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx471_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx471_3]
                        lea              rdx, [rip + .Lx471_4]
                                                                                        jmp   rax
.Lx471_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx471_2
.Lx471_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx471_2
.Lx471_1:
                        call             rt_faildescr@PLT
.Lx471_2:
                        mov              qword ptr [rsp + 1792], rax
                        mov              qword ptr [rsp + 1800], rdx
                        cmp              eax, 99
                                                                                        je    n323_lit_integer_α
                                                                                        jmp   n322_assign_α
n321_call_β:
                                                                                        jmp   n323_lit_integer_α
.Lx471_0:
                        .quad            .Lx471_0_s
.Lx471_0_s:
                        .string          "gcd"
#-----------------------------------------------------------------------------------------------------------------------
n322_assign_α:
                        mov              rsi, qword ptr [rsp + 1792]
                        mov              rdx, qword ptr [rsp + 1800]
                        mov              rdi, qword ptr [rip + .Lx472_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n323_lit_integer_α
.Lx472_0:
                        .quad            .Lx472_0_s
.Lx472_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n323_lit_integer_α:
                        mov              qword ptr [rsp + 1952], 6
                        mov              rax, qword ptr [rip + .Lx473_0]
                        mov              qword ptr [rsp + 1960], rax
                                                                                        jmp   n324_lit_integer_α
.Lx473_0:
                        .quad            100
#-----------------------------------------------------------------------------------------------------------------------
n324_lit_integer_α:
                        mov              qword ptr [rsp + 1968], 6
                        mov              rax, qword ptr [rip + .Lx474_0]
                        mov              qword ptr [rsp + 1976], rax
                                                                                        jmp   n325_call_α
.Lx474_0:
                        .quad            75
#-----------------------------------------------------------------------------------------------------------------------
n325_call_α:
                        sub              rsp, 48
                        mov              rax, qword ptr [1879052368]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052376]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052384]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052392]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [1879052400]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [1879052408]
                        mov              qword ptr [rsp + 40], rax
                        mov              rdi, qword ptr [rip + .Lx476_0]
                        mov              esi, 3
                        mov              edx, 2
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx476_5
                        mov              rax, qword ptr [rsp + 1952]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 1960]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 1968]
                        mov              qword ptr [1879052384], rax
                        mov              rax, qword ptr [rsp + 1976]
                        mov              qword ptr [1879052392], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx476_6]
                        lea              rdx, [rip + .Lx476_7]
                                                                                        jmp   rax
.Lx476_6:
                        mov              rdi, qword ptr [1879052368]
                        mov              rsi, qword ptr [1879052376]
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052400], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052408], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052384], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052392], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052376], rax
                        add              rsp, 48
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx476_2
.Lx476_7:
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052400], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052408], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052384], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052392], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052376], rax
                        add              rsp, 48
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx476_2
.Lx476_5:
                        add              rsp, 48
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx476_20
                        mov              rax, qword ptr [rsp + 1952]
                        mov              rdx, qword ptr [rsp + 1960]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx476_21
.Lx476_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 1952]
                        mov              rdx, qword ptr [rsp + 1960]
                        call             rt_arg_stage@PLT
.Lx476_21:
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx476_22
                        mov              rax, qword ptr [rsp + 1968]
                        mov              rdx, qword ptr [rsp + 1976]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 16], rax
                        mov              qword ptr [r10 + 24], rdx
                                                                                        jmp   .Lx476_23
.Lx476_22:
                        mov              edi, 1
                        mov              rsi, qword ptr [rsp + 1968]
                        mov              rdx, qword ptr [rsp + 1976]
                        call             rt_arg_stage@PLT
.Lx476_23:
                        mov              rdi, qword ptr [rip + .Lx476_0]
                        mov              esi, 2
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx476_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx476_3]
                        lea              rdx, [rip + .Lx476_4]
                                                                                        jmp   rax
.Lx476_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx476_2
.Lx476_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx476_2
.Lx476_1:
                        call             rt_faildescr@PLT
.Lx476_2:
                        mov              qword ptr [rsp + 1888], rax
                        mov              qword ptr [rsp + 1896], rdx
                        cmp              eax, 99
                                                                                        je    n327_lit_integer_α
                                                                                        jmp   n326_assign_α
n325_call_β:
                                                                                        jmp   n327_lit_integer_α
.Lx476_0:
                        .quad            .Lx476_0_s
.Lx476_0_s:
                        .string          "gcd"
#-----------------------------------------------------------------------------------------------------------------------
n326_assign_α:
                        mov              rsi, qword ptr [rsp + 1888]
                        mov              rdx, qword ptr [rsp + 1896]
                        mov              rdi, qword ptr [rip + .Lx477_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n327_lit_integer_α
.Lx477_0:
                        .quad            .Lx477_0_s
.Lx477_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n327_lit_integer_α:
                        mov              qword ptr [rsp + 2048], 6
                        mov              rax, qword ptr [rip + .Lx478_0]
                        mov              qword ptr [rsp + 2056], rax
                                                                                        jmp   n328_lit_integer_α
.Lx478_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n328_lit_integer_α:
                        mov              qword ptr [rsp + 2064], 6
                        mov              rax, qword ptr [rip + .Lx479_0]
                        mov              qword ptr [rsp + 2072], rax
                                                                                        jmp   n329_call_α
.Lx479_0:
                        .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n329_call_α:
                        sub              rsp, 64
                        mov              rax, qword ptr [1879052432]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052440]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052384]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052392]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [1879052448]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [1879052456]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [1879052416]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [1879052424]
                        mov              qword ptr [rsp + 56], rax
                        mov              rdi, qword ptr [rip + .Lx481_0]
                        mov              esi, 3
                        mov              edx, 2
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx481_5
                        mov              rax, qword ptr [rsp + 2048]
                        mov              qword ptr [1879052432], rax
                        mov              rax, qword ptr [rsp + 2056]
                        mov              qword ptr [1879052440], rax
                        mov              rax, qword ptr [rsp + 2064]
                        mov              qword ptr [1879052384], rax
                        mov              rax, qword ptr [rsp + 2072]
                        mov              qword ptr [1879052392], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx481_6]
                        lea              rdx, [rip + .Lx481_7]
                                                                                        jmp   rax
.Lx481_6:
                        mov              rdi, qword ptr [1879052416]
                        mov              rsi, qword ptr [1879052424]
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052416], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052424], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052448], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052456], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052384], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052392], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052432], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052440], rax
                        add              rsp, 64
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx481_2
.Lx481_7:
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052416], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052424], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052448], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052456], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052384], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052392], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052432], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052440], rax
                        add              rsp, 64
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx481_2
.Lx481_5:
                        add              rsp, 64
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx481_20
                        mov              rax, qword ptr [rsp + 2048]
                        mov              rdx, qword ptr [rsp + 2056]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx481_21
.Lx481_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 2048]
                        mov              rdx, qword ptr [rsp + 2056]
                        call             rt_arg_stage@PLT
.Lx481_21:
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx481_22
                        mov              rax, qword ptr [rsp + 2064]
                        mov              rdx, qword ptr [rsp + 2072]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 16], rax
                        mov              qword ptr [r10 + 24], rdx
                                                                                        jmp   .Lx481_23
.Lx481_22:
                        mov              edi, 1
                        mov              rsi, qword ptr [rsp + 2064]
                        mov              rdx, qword ptr [rsp + 2072]
                        call             rt_arg_stage@PLT
.Lx481_23:
                        mov              rdi, qword ptr [rip + .Lx481_0]
                        mov              esi, 2
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx481_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx481_3]
                        lea              rdx, [rip + .Lx481_4]
                                                                                        jmp   rax
.Lx481_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx481_2
.Lx481_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx481_2
.Lx481_1:
                        call             rt_faildescr@PLT
.Lx481_2:
                        mov              qword ptr [rsp + 1984], rax
                        mov              qword ptr [rsp + 1992], rdx
                        cmp              eax, 99
                                                                                        je    main_γ
                                                                                        jmp   n330_assign_α
n329_call_β:
                                                                                        jmp   main_γ
.Lx481_0:
                        .quad            .Lx481_0_s
.Lx481_0_s:
                        .string          "lcm"
#-----------------------------------------------------------------------------------------------------------------------
n330_assign_α:
                        mov              rsi, qword ptr [rsp + 1984]
                        mov              rdx, qword ptr [rsp + 1992]
                        mov              rdi, qword ptr [rip + .Lx482_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   main_γ
.Lx482_0:
                        .quad            .Lx482_0_s
.Lx482_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        mov              rsp, rbp
                        mov              rbp, [rsp + 2080]
                        add              rsp, 2088
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              rsp, rbp
                        mov              eax, 99
                        xor              edx, edx
                        mov              rbp, [rsp + 2080]
                        add              rsp, 2088
                        ret
                        .section         .note.GNU-stack,"",@progbits

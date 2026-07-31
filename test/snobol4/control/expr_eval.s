                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_LBL__Push_α
proc_LBL__Push_α:
                        .global          proc_LBL__Push_α
                        .global          proc_LBL__Push_β
                        .global          proc_LBL__Push_γ
                        .global          proc_LBL__Push_ω
                        sub              rsp, 48
                        mov              [rsp + 24], rcx
                        mov              [rsp + 32], rdx
                        mov              [rsp + 40], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
proc_LBL__Push_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n0_goto_α:
                                                                                        jmp   n1_var_α
n0_goto_β:
                                                                                        jmp   proc_LBL__Push_ω
#=======================================================================================================================
# Push     stk[0]   =  stk[0] + 1
#-----------------------------------------------------------------------------------------------------------------------
n1_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 144], rax
                        mov              qword ptr [rsp + 152], rdx
                                                                                        jmp   n2_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n2_lit_integer_α:
                        mov              qword ptr [rsp + 160], 6
                        mov              rax, qword ptr [rip + .Lx28_0]
                        mov              qword ptr [rsp + 168], rax
                                                                                        jmp   n3_subscript_α
.Lx28_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n3_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 160]
                        mov              rsi, qword ptr [rsp + 168]
                        mov              rdx, qword ptr [rsp + 176]
                        mov              rcx, qword ptr [rsp + 184]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx29_240
                        add              rsp, 16
                                                                                        jmp   n11_var_α
.Lx29_240:
                        mov              qword ptr [rsp + 192], rax
                        mov              qword ptr [rsp + 200], rdx
                        add              rsp, 16
                                                                                        jmp   n4_var_α
#-----------------------------------------------------------------------------------------------------------------------
n4_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 208], rax
                        mov              qword ptr [rsp + 216], rdx
                                                                                        jmp   n5_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n5_lit_integer_α:
                        mov              qword ptr [rsp + 224], 6
                        mov              rax, qword ptr [rip + .Lx31_0]
                        mov              qword ptr [rsp + 232], rax
                                                                                        jmp   n6_subscript_α
.Lx31_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n6_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 224]
                        mov              rsi, qword ptr [rsp + 232]
                        mov              rdx, qword ptr [rsp + 240]
                        mov              rcx, qword ptr [rsp + 248]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx32_240
                        add              rsp, 16
                                                                                        jmp   n11_var_α
.Lx32_240:
                        mov              qword ptr [rsp + 256], rax
                        mov              qword ptr [rsp + 264], rdx
                        add              rsp, 16
                                                                                        jmp   n7_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n7_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 256]
                        mov              rsi, qword ptr [rsp + 264]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx33_240
                        add              rsp, 16
                                                                                        jmp   n11_var_α
.Lx33_240:
                        mov              qword ptr [rsp + 272], rax
                        mov              qword ptr [rsp + 280], rdx
                        add              rsp, 16
                                                                                        jmp   n8_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n8_lit_integer_α:
                        mov              qword ptr [rsp + 272], 6
                        mov              rax, qword ptr [rip + .Lx34_0]
                        mov              qword ptr [rsp + 280], rax
                                                                                        jmp   n9_binop_α
.Lx34_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n9_binop_α:
                        mov              eax, dword ptr [rsp + 256]
                        cmp              eax, 6
                                                                                        jne   .Lx35_0
                        mov              rax, qword ptr [rsp + 264]
                        mov              rcx, 1
                        add              rax, rcx
                        mov              qword ptr [rsp + 192], 6
                        mov              qword ptr [rsp + 200], rax
                                                                                        jmp   n10_assign_var_α
.Lx35_0:
                        mov              rdi, qword ptr [rsp + 256]
                        mov              rsi, qword ptr [rsp + 264]
                        mov              rdx, qword ptr [rsp + 272]
                        mov              rcx, qword ptr [rsp + 280]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        je    n11_var_α
                        mov              qword ptr [rsp + 192], rax
                        mov              qword ptr [rsp + 200], rdx
                                                                                        jmp   n10_assign_var_α
#-----------------------------------------------------------------------------------------------------------------------
n10_assign_var_α:
                        mov              rdi, qword ptr [rsp + 176]
                        mov              rsi, qword ptr [rsp + 184]
                        mov              rdx, qword ptr [rsp + 192]
                        mov              rcx, qword ptr [rsp + 200]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n11_var_α
                        mov              qword ptr [rsp + 288], rax
                        mov              qword ptr [rsp + 296], rdx
                                                                                        jmp   n11_var_α
#=======================================================================================================================
#          Push     =  .stk[stk[0]]
#-----------------------------------------------------------------------------------------------------------------------
n11_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 304], rax
                        mov              qword ptr [rsp + 312], rdx
                                                                                        jmp   n12_var_α
#-----------------------------------------------------------------------------------------------------------------------
n12_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 320], rax
                        mov              qword ptr [rsp + 328], rdx
                                                                                        jmp   n13_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n13_lit_integer_α:
                        mov              qword ptr [rsp + 336], 6
                        mov              rax, qword ptr [rip + .Lx39_0]
                        mov              qword ptr [rsp + 344], rax
                                                                                        jmp   n14_subscript_α
.Lx39_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n14_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 336]
                        mov              rsi, qword ptr [rsp + 344]
                        mov              rdx, qword ptr [rsp + 352]
                        mov              rcx, qword ptr [rsp + 360]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx40_240
                        add              rsp, 16
                                                                                        jmp   n18_var_α
.Lx40_240:
                        mov              qword ptr [rsp + 368], rax
                        mov              qword ptr [rsp + 376], rdx
                        add              rsp, 16
                                                                                        jmp   n15_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n15_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 368]
                        mov              rsi, qword ptr [rsp + 376]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx41_240
                        add              rsp, 16
                                                                                        jmp   n18_var_α
.Lx41_240:
                        mov              qword ptr [rsp + 384], rax
                        mov              qword ptr [rsp + 392], rdx
                        add              rsp, 16
                                                                                        jmp   n16_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n16_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 320]
                        mov              rsi, qword ptr [rsp + 328]
                        mov              rdx, qword ptr [rsp + 384]
                        mov              rcx, qword ptr [rsp + 392]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx42_240
                        add              rsp, 16
                                                                                        jmp   n18_var_α
.Lx42_240:
                        mov              qword ptr [rsp + 400], rax
                        mov              qword ptr [rsp + 408], rdx
                        add              rsp, 16
                                                                                        jmp   n17_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n17_assign_α:
                        mov              rax, qword ptr [rsp + 384]
                        mov              rdx, qword ptr [rsp + 392]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n18_var_α
#=======================================================================================================================
#          $Push    =  x                             :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n18_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 448], rax
                        mov              qword ptr [rsp + 456], rdx
                                                                                        jmp   n19_call_α
#-----------------------------------------------------------------------------------------------------------------------
n19_call_α:
                        mov              rax, qword ptr [rsp + 448]
                        mov              qword ptr [rsp + 416], rax
                        mov              rax, qword ptr [rsp + 456]
                        mov              qword ptr [rsp + 424], rax
                        .section         .rodata
.Lrkfn46:               .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn46]
                        lea              rsi, [rsp + 416]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 400], rax
                        mov              qword ptr [rsp + 408], rdx
                        cmp              eax, 99
                                                                                        je    n22_lit_string_α
                                                                                        jmp   n20_var_α
n19_call_β:
                                                                                        jmp   n22_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n20_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 464], rax
                        mov              qword ptr [rsp + 472], rdx
                                                                                        jmp   n21_assign_var_α
#-----------------------------------------------------------------------------------------------------------------------
n21_assign_var_α:
                        mov              rdi, qword ptr [rsp + 400]
                        mov              rsi, qword ptr [rsp + 408]
                        mov              rdx, qword ptr [rsp + 464]
                        mov              rcx, qword ptr [rsp + 472]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n22_lit_string_α
                        mov              qword ptr [rsp + 480], rax
                        mov              qword ptr [rsp + 488], rdx
                                                                                        jmp   n22_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n22_lit_string_α:
                        mov              qword ptr [rsp + 32], 1
                        mov              dword ptr [rsp + 36], 0
                        mov              rax, qword ptr [rip + .Lx49_0]
                        mov              qword ptr [rsp + 40], rax
                                                                                        jmp   n23_call_α
.Lx49_0:
                        .quad            .Lx49_0_s
.Lx49_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n23_call_α:
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 72], rax
                        .section         .rodata
.Lrkfn51:               .string          "SNO$NRET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn51]
                        lea              rsi, [rsp + 64]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 48], rax
                        mov              qword ptr [rsp + 56], rdx
                        cmp              eax, 99
                                                                                        je    n25_save_restore_α
                                                                                        jmp   n24_save_restore_α
n23_call_β:
                                                                                        jmp   n25_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n24_save_restore_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n25_save_restore_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Push_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Push_β:
                                                                                        jmp   proc_LBL__Push_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Push_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 24]
                        lea              rsp, [rbp + 48]
                        mov              rbp, [rbp + 40]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Push_ω:
                        mov              rax, [rbp + 32]
                        lea              rsp, [rbp + 48]
                        mov              rbp, [rbp + 40]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_LBL__Pop_α
proc_LBL__Pop_α:
                        .global          proc_LBL__Pop_α
                        .global          proc_LBL__Pop_β
                        .global          proc_LBL__Pop_γ
                        .global          proc_LBL__Pop_ω
                        sub              rsp, 48
                        mov              [rsp + 24], rcx
                        mov              [rsp + 32], rdx
                        mov              [rsp + 40], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
proc_LBL__Pop_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n56_goto_α:
                                                                                        jmp   n57_var_α
n56_goto_β:
                                                                                        jmp   proc_LBL__Pop_ω
#=======================================================================================================================
# Pop      Pop      =  stk[stk[0]]
#-----------------------------------------------------------------------------------------------------------------------
n57_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 496], rax
                        mov              qword ptr [rsp + 504], rdx
                                                                                        jmp   n58_var_α
#-----------------------------------------------------------------------------------------------------------------------
n58_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 512], rax
                        mov              qword ptr [rsp + 520], rdx
                                                                                        jmp   n59_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n59_lit_integer_α:
                        mov              qword ptr [rsp + 528], 6
                        mov              rax, qword ptr [rip + .Lx79_0]
                        mov              qword ptr [rsp + 536], rax
                                                                                        jmp   n60_subscript_α
.Lx79_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n60_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 528]
                        mov              rsi, qword ptr [rsp + 536]
                        mov              rdx, qword ptr [rsp + 544]
                        mov              rcx, qword ptr [rsp + 552]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx80_240
                        add              rsp, 16
                                                                                        jmp   n65_var_α
.Lx80_240:
                        mov              qword ptr [rsp + 560], rax
                        mov              qword ptr [rsp + 568], rdx
                        add              rsp, 16
                                                                                        jmp   n61_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n61_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 560]
                        mov              rsi, qword ptr [rsp + 568]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx81_240
                        add              rsp, 16
                                                                                        jmp   n65_var_α
.Lx81_240:
                        mov              qword ptr [rsp + 576], rax
                        mov              qword ptr [rsp + 584], rdx
                        add              rsp, 16
                                                                                        jmp   n62_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n62_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 512]
                        mov              rsi, qword ptr [rsp + 520]
                        mov              rdx, qword ptr [rsp + 576]
                        mov              rcx, qword ptr [rsp + 584]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx82_240
                        add              rsp, 16
                                                                                        jmp   n65_var_α
.Lx82_240:
                        mov              qword ptr [rsp + 592], rax
                        mov              qword ptr [rsp + 600], rdx
                        add              rsp, 16
                                                                                        jmp   n63_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n63_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 592]
                        mov              rsi, qword ptr [rsp + 600]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx83_240
                        add              rsp, 16
                                                                                        jmp   n65_var_α
.Lx83_240:
                        mov              qword ptr [rsp + 608], rax
                        mov              qword ptr [rsp + 616], rdx
                        add              rsp, 16
                                                                                        jmp   n64_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n64_assign_α:
                        mov              rax, qword ptr [rsp + 592]
                        mov              rdx, qword ptr [rsp + 600]
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rdx
                                                                                        jmp   n65_var_α
#=======================================================================================================================
#          stk[0]   =  stk[0] - 1                    :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n65_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 608], rax
                        mov              qword ptr [rsp + 616], rdx
                                                                                        jmp   n66_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n66_lit_integer_α:
                        mov              qword ptr [rsp + 624], 6
                        mov              rax, qword ptr [rip + .Lx86_0]
                        mov              qword ptr [rsp + 632], rax
                                                                                        jmp   n67_subscript_α
.Lx86_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n67_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 624]
                        mov              rsi, qword ptr [rsp + 632]
                        mov              rdx, qword ptr [rsp + 640]
                        mov              rcx, qword ptr [rsp + 648]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx87_240
                        add              rsp, 16
                                                                                        jmp   n75_save_restore_α
.Lx87_240:
                        mov              qword ptr [rsp + 656], rax
                        mov              qword ptr [rsp + 664], rdx
                        add              rsp, 16
                                                                                        jmp   n68_var_α
#-----------------------------------------------------------------------------------------------------------------------
n68_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 672], rax
                        mov              qword ptr [rsp + 680], rdx
                                                                                        jmp   n69_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n69_lit_integer_α:
                        mov              qword ptr [rsp + 688], 6
                        mov              rax, qword ptr [rip + .Lx89_0]
                        mov              qword ptr [rsp + 696], rax
                                                                                        jmp   n70_subscript_α
.Lx89_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n70_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 688]
                        mov              rsi, qword ptr [rsp + 696]
                        mov              rdx, qword ptr [rsp + 704]
                        mov              rcx, qword ptr [rsp + 712]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx90_240
                        add              rsp, 16
                                                                                        jmp   n75_save_restore_α
.Lx90_240:
                        mov              qword ptr [rsp + 720], rax
                        mov              qword ptr [rsp + 728], rdx
                        add              rsp, 16
                                                                                        jmp   n71_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n71_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 720]
                        mov              rsi, qword ptr [rsp + 728]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx91_240
                        add              rsp, 16
                                                                                        jmp   n75_save_restore_α
.Lx91_240:
                        mov              qword ptr [rsp + 736], rax
                        mov              qword ptr [rsp + 744], rdx
                        add              rsp, 16
                                                                                        jmp   n72_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n72_lit_integer_α:
                        mov              qword ptr [rsp + 736], 6
                        mov              rax, qword ptr [rip + .Lx92_0]
                        mov              qword ptr [rsp + 744], rax
                                                                                        jmp   n73_binop_α
.Lx92_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n73_binop_α:
                        mov              eax, dword ptr [rsp + 720]
                        cmp              eax, 6
                                                                                        jne   .Lx93_0
                        mov              rax, qword ptr [rsp + 728]
                        mov              rcx, 1
                        sub              rax, rcx
                        mov              qword ptr [rsp + 656], 6
                        mov              qword ptr [rsp + 664], rax
                                                                                        jmp   n74_assign_var_α
.Lx93_0:
                        mov              rdi, qword ptr [rsp + 720]
                        mov              rsi, qword ptr [rsp + 728]
                        mov              rdx, qword ptr [rsp + 736]
                        mov              rcx, qword ptr [rsp + 744]
                        call             rt_sub@PLT
                        cmp              eax, 99
                                                                                        je    n75_save_restore_α
                        mov              qword ptr [rsp + 656], rax
                        mov              qword ptr [rsp + 664], rdx
                                                                                        jmp   n74_assign_var_α
#-----------------------------------------------------------------------------------------------------------------------
n74_assign_var_α:
                        mov              rdi, qword ptr [rsp + 640]
                        mov              rsi, qword ptr [rsp + 648]
                        mov              rdx, qword ptr [rsp + 656]
                        mov              rcx, qword ptr [rsp + 664]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n75_save_restore_α
                        mov              qword ptr [rsp + 752], rax
                        mov              qword ptr [rsp + 760], rdx
                                                                                        jmp   n75_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n75_save_restore_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Pop_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Pop_β:
                                                                                        jmp   proc_LBL__Pop_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Pop_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 24]
                        lea              rsp, [rbp + 48]
                        mov              rbp, [rbp + 40]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Pop_ω:
                        mov              rax, [rbp + 32]
                        lea              rsp, [rbp + 48]
                        mov              rbp, [rbp + 40]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_LBL__Unary_α
proc_LBL__Unary_α:
                        .global          proc_LBL__Unary_α
                        .global          proc_LBL__Unary_β
                        .global          proc_LBL__Unary_γ
                        .global          proc_LBL__Unary_ω
                        sub              rsp, 48
                        mov              [rsp + 24], rcx
                        mov              [rsp + 32], rdx
                        mov              [rsp + 40], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
proc_LBL__Unary_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n97_goto_α:
                                                                                        jmp   n98_call_α
n97_goto_β:
                                                                                        jmp   proc_LBL__Unary_ω
#=======================================================================================================================
# Unary    arg      =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n98_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx119_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx119_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx119_6]
                        lea              rdx, [rip + .Lx119_7]
                                                                                        jmp   rax
.Lx119_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx119_2
.Lx119_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx119_2
.Lx119_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx119_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx119_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx119_3]
                        lea              rdx, [rip + .Lx119_4]
                                                                                        jmp   rax
.Lx119_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx119_2
.Lx119_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx119_2
.Lx119_1:
                        call             rt_faildescr@PLT
.Lx119_2:
                        mov              qword ptr [rsp + 768], rax
                        mov              qword ptr [rsp + 776], rdx
                        cmp              eax, 99
                                                                                        je    n100_call_α
                                                                                        jmp   n99_assign_α
n98_call_β:
                                                                                        jmp   n100_call_α
.Lx119_0:
                        .quad            .Lx119_0_s
.Lx119_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n99_assign_α:
                        mov              rax, qword ptr [rsp + 768]
                        mov              rdx, qword ptr [rsp + 776]
                        mov              qword ptr [1879052352], rax
                        mov              qword ptr [1879052360], rdx
                                                                                        jmp   n100_call_α
#=======================================================================================================================
#          op       =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n100_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx122_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx122_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx122_6]
                        lea              rdx, [rip + .Lx122_7]
                                                                                        jmp   rax
.Lx122_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx122_2
.Lx122_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx122_2
.Lx122_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx122_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx122_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx122_3]
                        lea              rdx, [rip + .Lx122_4]
                                                                                        jmp   rax
.Lx122_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx122_2
.Lx122_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx122_2
.Lx122_1:
                        call             rt_faildescr@PLT
.Lx122_2:
                        mov              qword ptr [rsp + 800], rax
                        mov              qword ptr [rsp + 808], rdx
                        cmp              eax, 99
                                                                                        je    n102_lit_string_α
                                                                                        jmp   n101_assign_α
n100_call_β:
                                                                                        jmp   n102_lit_string_α
.Lx122_0:
                        .quad            .Lx122_0_s
.Lx122_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n101_assign_α:
                        mov              rax, qword ptr [rsp + 800]
                        mov              rdx, qword ptr [rsp + 808]
                        mov              qword ptr [1879052368], rax
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n102_lit_string_α
#=======================================================================================================================
#          Push()   =  EVAL(op arg)
#-----------------------------------------------------------------------------------------------------------------------
n102_lit_string_α:
                        mov              qword ptr [rsp + 832], 1
                        mov              dword ptr [rsp + 836], 0
                        mov              rax, qword ptr [rip + .Lx124_0]
                        mov              qword ptr [rsp + 840], rax
                                                                                        jmp   n103_call_α
.Lx124_0:
                        .quad            .Lx124_0_s
.Lx124_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n103_call_α:
                        mov              rax, qword ptr [rsp + 832]
                        mov              qword ptr [rsp + 864], rax
                        mov              rax, qword ptr [rsp + 840]
                        mov              qword ptr [rsp + 872], rax
                        .section         .rodata
.Lrkfn126:              .string          "SNO$WANTNM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn126]
                        lea              rsi, [rsp + 864]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 848], rax
                        mov              qword ptr [rsp + 856], rdx
                        cmp              eax, 99
                                                                                        je    n110_lit_string_α
                                                                                        jmp   n104_call_α
n103_call_β:
                                                                                        jmp   n110_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n104_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx128_0]
                        mov              esi, 1
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx128_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx128_6]
                        lea              rdx, [rip + .Lx128_7]
                                                                                        jmp   rax
.Lx128_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx128_2
.Lx128_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx128_2
.Lx128_5:
                        add              rsp, 32
                        mov              rdi, qword ptr [rip + .Lx128_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx128_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx128_3]
                        lea              rdx, [rip + .Lx128_4]
                                                                                        jmp   rax
.Lx128_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx128_2
.Lx128_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx128_2
.Lx128_1:
                        call             rt_faildescr@PLT
.Lx128_2:
                        mov              qword ptr [rsp + 896], rax
                        mov              qword ptr [rsp + 904], rdx
                        cmp              eax, 99
                                                                                        je    n110_lit_string_α
                                                                                        jmp   n105_var_α
n104_call_β:
                                                                                        jmp   n110_lit_string_α
.Lx128_0:
                        .quad            .Lx128_0_s
.Lx128_0_s:
                        .string          "Push"
#-----------------------------------------------------------------------------------------------------------------------
n105_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 992], rax
                        mov              qword ptr [rsp + 1000], rdx
                                                                                        jmp   n106_var_α
#-----------------------------------------------------------------------------------------------------------------------
n106_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 1008], rax
                        mov              qword ptr [rsp + 1016], rdx
                                                                                        jmp   n107_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n107_binop_α:
                        mov              rdi, qword ptr [rsp + 992]
                        mov              rsi, qword ptr [rsp + 1000]
                        mov              rdx, qword ptr [rsp + 1008]
                        mov              rcx, qword ptr [rsp + 1016]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 976], rax
                        mov              qword ptr [rsp + 984], rdx
                                                                                        jmp   n108_call_α
#-----------------------------------------------------------------------------------------------------------------------
n108_call_α:
                        mov              rax, qword ptr [rsp + 976]
                        mov              qword ptr [rsp + 944], rax
                        mov              rax, qword ptr [rsp + 984]
                        mov              qword ptr [rsp + 952], rax
                        .section         .rodata
.Lrkfn133:              .string          "EVAL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn133]
                        lea              rsi, [rsp + 944]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 928], rax
                        mov              qword ptr [rsp + 936], rdx
                        cmp              eax, 99
                                                                                        je    n110_lit_string_α
                                                                                        jmp   n109_assign_var_α
n108_call_β:
                                                                                        jmp   n110_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n109_assign_var_α:
                        mov              rdi, qword ptr [rsp + 896]
                        mov              rsi, qword ptr [rsp + 904]
                        mov              rdx, qword ptr [rsp + 928]
                        mov              rcx, qword ptr [rsp + 936]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n110_lit_string_α
                        mov              qword ptr [rsp + 1024], rax
                        mov              qword ptr [rsp + 1032], rdx
                                                                                        jmp   n110_lit_string_α
#=======================================================================================================================
#          Unary    =  .dummy                        :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n110_lit_string_α:
                        mov              qword ptr [rsp + 1088], 1
                        mov              dword ptr [rsp + 1092], 5
                        mov              rax, qword ptr [rip + .Lx135_0]
                        mov              qword ptr [rsp + 1096], rax
                                                                                        jmp   n111_call_α
.Lx135_0:
                        .quad            .Lx135_0_s
.Lx135_0_s:
                        .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n111_call_α:
                        mov              rax, qword ptr [rsp + 1088]
                        mov              qword ptr [rsp + 1056], rax
                        mov              rax, qword ptr [rsp + 1096]
                        mov              qword ptr [rsp + 1064], rax
                        .section         .rodata
.Lrkfn137:              .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn137]
                        lea              rsi, [rsp + 1056]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1040], rax
                        mov              qword ptr [rsp + 1048], rdx
                        cmp              eax, 99
                                                                                        je    n113_lit_string_α
                                                                                        jmp   n112_assign_α
n111_call_β:
                                                                                        jmp   n113_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n112_assign_α:
                        mov              rax, qword ptr [rsp + 1040]
                        mov              rdx, qword ptr [rsp + 1048]
                        mov              qword ptr [1879052336], rax
                        mov              qword ptr [1879052344], rdx
                                                                                        jmp   n113_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n113_lit_string_α:
                        mov              qword ptr [rsp + 32], 1
                        mov              dword ptr [rsp + 36], 0
                        mov              rax, qword ptr [rip + .Lx139_0]
                        mov              qword ptr [rsp + 40], rax
                                                                                        jmp   n114_call_α
.Lx139_0:
                        .quad            .Lx139_0_s
.Lx139_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n114_call_α:
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 72], rax
                        .section         .rodata
.Lrkfn141:              .string          "SNO$NRET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn141]
                        lea              rsi, [rsp + 64]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 48], rax
                        mov              qword ptr [rsp + 56], rdx
                        cmp              eax, 99
                                                                                        je    n116_save_restore_α
                                                                                        jmp   n115_save_restore_α
n114_call_β:
                                                                                        jmp   n116_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n115_save_restore_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n116_save_restore_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Unary_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Unary_β:
                                                                                        jmp   proc_LBL__Unary_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Unary_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 24]
                        lea              rsp, [rbp + 48]
                        mov              rbp, [rbp + 40]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Unary_ω:
                        mov              rax, [rbp + 32]
                        lea              rsp, [rbp + 48]
                        mov              rbp, [rbp + 40]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_LBL__Binary_α
proc_LBL__Binary_α:
                        .global          proc_LBL__Binary_α
                        .global          proc_LBL__Binary_β
                        .global          proc_LBL__Binary_γ
                        .global          proc_LBL__Binary_ω
                        sub              rsp, 48
                        mov              [rsp + 24], rcx
                        mov              [rsp + 32], rdx
                        mov              [rsp + 40], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
proc_LBL__Binary_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n146_goto_α:
                                                                                        jmp   n147_call_α
n146_goto_β:
                                                                                        jmp   proc_LBL__Binary_ω
#=======================================================================================================================
# Binary   right    =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n147_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx176_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx176_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx176_6]
                        lea              rdx, [rip + .Lx176_7]
                                                                                        jmp   rax
.Lx176_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx176_2
.Lx176_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx176_2
.Lx176_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx176_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx176_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx176_3]
                        lea              rdx, [rip + .Lx176_4]
                                                                                        jmp   rax
.Lx176_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx176_2
.Lx176_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx176_2
.Lx176_1:
                        call             rt_faildescr@PLT
.Lx176_2:
                        mov              qword ptr [rsp + 1104], rax
                        mov              qword ptr [rsp + 1112], rdx
                        cmp              eax, 99
                                                                                        je    n149_call_α
                                                                                        jmp   n148_assign_α
n147_call_β:
                                                                                        jmp   n149_call_α
.Lx176_0:
                        .quad            .Lx176_0_s
.Lx176_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n148_assign_α:
                        mov              rax, qword ptr [rsp + 1104]
                        mov              rdx, qword ptr [rsp + 1112]
                        mov              qword ptr [1879052416], rax
                        mov              qword ptr [1879052424], rdx
                                                                                        jmp   n149_call_α
#=======================================================================================================================
#          op       =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n149_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx179_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx179_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx179_6]
                        lea              rdx, [rip + .Lx179_7]
                                                                                        jmp   rax
.Lx179_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx179_2
.Lx179_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx179_2
.Lx179_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx179_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx179_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx179_3]
                        lea              rdx, [rip + .Lx179_4]
                                                                                        jmp   rax
.Lx179_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx179_2
.Lx179_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx179_2
.Lx179_1:
                        call             rt_faildescr@PLT
.Lx179_2:
                        mov              qword ptr [rsp + 1136], rax
                        mov              qword ptr [rsp + 1144], rdx
                        cmp              eax, 99
                                                                                        je    n151_call_α
                                                                                        jmp   n150_assign_α
n149_call_β:
                                                                                        jmp   n151_call_α
.Lx179_0:
                        .quad            .Lx179_0_s
.Lx179_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n150_assign_α:
                        mov              rax, qword ptr [rsp + 1136]
                        mov              rdx, qword ptr [rsp + 1144]
                        mov              qword ptr [1879052368], rax
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n151_call_α
#=======================================================================================================================
#          left     =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n151_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx182_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx182_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx182_6]
                        lea              rdx, [rip + .Lx182_7]
                                                                                        jmp   rax
.Lx182_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx182_2
.Lx182_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx182_2
.Lx182_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx182_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx182_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx182_3]
                        lea              rdx, [rip + .Lx182_4]
                                                                                        jmp   rax
.Lx182_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx182_2
.Lx182_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx182_2
.Lx182_1:
                        call             rt_faildescr@PLT
.Lx182_2:
                        mov              qword ptr [rsp + 1168], rax
                        mov              qword ptr [rsp + 1176], rdx
                        cmp              eax, 99
                                                                                        je    n153_lit_string_α
                                                                                        jmp   n152_assign_α
n151_call_β:
                                                                                        jmp   n153_lit_string_α
.Lx182_0:
                        .quad            .Lx182_0_s
.Lx182_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n152_assign_α:
                        mov              rax, qword ptr [rsp + 1168]
                        mov              rdx, qword ptr [rsp + 1176]
                        mov              qword ptr [1879052400], rax
                        mov              qword ptr [1879052408], rdx
                                                                                        jmp   n153_lit_string_α
#=======================================================================================================================
#          Push()   =  EVAL(left ' ' op ' ' right)
#-----------------------------------------------------------------------------------------------------------------------
n153_lit_string_α:
                        mov              qword ptr [rsp + 1200], 1
                        mov              dword ptr [rsp + 1204], 0
                        mov              rax, qword ptr [rip + .Lx184_0]
                        mov              qword ptr [rsp + 1208], rax
                                                                                        jmp   n154_call_α
.Lx184_0:
                        .quad            .Lx184_0_s
.Lx184_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n154_call_α:
                        mov              rax, qword ptr [rsp + 1200]
                        mov              qword ptr [rsp + 1232], rax
                        mov              rax, qword ptr [rsp + 1208]
                        mov              qword ptr [rsp + 1240], rax
                        .section         .rodata
.Lrkfn186:              .string          "SNO$WANTNM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn186]
                        lea              rsi, [rsp + 1232]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1216], rax
                        mov              qword ptr [rsp + 1224], rdx
                        cmp              eax, 99
                                                                                        je    n167_lit_string_α
                                                                                        jmp   n155_call_α
n154_call_β:
                                                                                        jmp   n167_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n155_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx188_0]
                        mov              esi, 1
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx188_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx188_6]
                        lea              rdx, [rip + .Lx188_7]
                                                                                        jmp   rax
.Lx188_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx188_2
.Lx188_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx188_2
.Lx188_5:
                        add              rsp, 32
                        mov              rdi, qword ptr [rip + .Lx188_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx188_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx188_3]
                        lea              rdx, [rip + .Lx188_4]
                                                                                        jmp   rax
.Lx188_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx188_2
.Lx188_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx188_2
.Lx188_1:
                        call             rt_faildescr@PLT
.Lx188_2:
                        mov              qword ptr [rsp + 1264], rax
                        mov              qword ptr [rsp + 1272], rdx
                        cmp              eax, 99
                                                                                        je    n167_lit_string_α
                                                                                        jmp   n156_var_α
n155_call_β:
                                                                                        jmp   n167_lit_string_α
.Lx188_0:
                        .quad            .Lx188_0_s
.Lx188_0_s:
                        .string          "Push"
#-----------------------------------------------------------------------------------------------------------------------
n156_var_α:
                        mov              rax, qword ptr [1879052400]
                        mov              rdx, qword ptr [1879052408]
                        mov              qword ptr [rsp + 1408], rax
                        mov              qword ptr [rsp + 1416], rdx
                                                                                        jmp   n157_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n157_lit_string_α:
                        mov              qword ptr [rsp + 1424], 1
                        mov              dword ptr [rsp + 1428], 1
                        mov              rax, qword ptr [rip + .Lx190_0]
                        mov              qword ptr [rsp + 1432], rax
                                                                                        jmp   n158_binop_α
.Lx190_0:
                        .quad            .Lx190_0_s
.Lx190_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n158_binop_α:
                        mov              rdi, qword ptr [rsp + 1408]
                        mov              rsi, qword ptr [rsp + 1416]
                        mov              rdx, qword ptr [rsp + 1424]
                        mov              rcx, qword ptr [rsp + 1432]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 1392], rax
                        mov              qword ptr [rsp + 1400], rdx
                                                                                        jmp   n159_var_α
#-----------------------------------------------------------------------------------------------------------------------
n159_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 1440], rax
                        mov              qword ptr [rsp + 1448], rdx
                                                                                        jmp   n160_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n160_binop_α:
                        mov              rdi, qword ptr [rsp + 1392]
                        mov              rsi, qword ptr [rsp + 1400]
                        mov              rdx, qword ptr [rsp + 1440]
                        mov              rcx, qword ptr [rsp + 1448]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 1376], rax
                        mov              qword ptr [rsp + 1384], rdx
                                                                                        jmp   n161_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n161_lit_string_α:
                        mov              qword ptr [rsp + 1456], 1
                        mov              dword ptr [rsp + 1460], 1
                        mov              rax, qword ptr [rip + .Lx194_0]
                        mov              qword ptr [rsp + 1464], rax
                                                                                        jmp   n162_binop_α
.Lx194_0:
                        .quad            .Lx194_0_s
.Lx194_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n162_binop_α:
                        mov              rdi, qword ptr [rsp + 1376]
                        mov              rsi, qword ptr [rsp + 1384]
                        mov              rdx, qword ptr [rsp + 1456]
                        mov              rcx, qword ptr [rsp + 1464]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 1360], rax
                        mov              qword ptr [rsp + 1368], rdx
                                                                                        jmp   n163_var_α
#-----------------------------------------------------------------------------------------------------------------------
n163_var_α:
                        mov              rax, qword ptr [1879052416]
                        mov              rdx, qword ptr [1879052424]
                        mov              qword ptr [rsp + 1472], rax
                        mov              qword ptr [rsp + 1480], rdx
                                                                                        jmp   n164_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n164_binop_α:
                        mov              rdi, qword ptr [rsp + 1360]
                        mov              rsi, qword ptr [rsp + 1368]
                        mov              rdx, qword ptr [rsp + 1472]
                        mov              rcx, qword ptr [rsp + 1480]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 1344], rax
                        mov              qword ptr [rsp + 1352], rdx
                                                                                        jmp   n165_call_α
#-----------------------------------------------------------------------------------------------------------------------
n165_call_α:
                        mov              rax, qword ptr [rsp + 1344]
                        mov              qword ptr [rsp + 1312], rax
                        mov              rax, qword ptr [rsp + 1352]
                        mov              qword ptr [rsp + 1320], rax
                        .section         .rodata
.Lrkfn199:              .string          "EVAL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn199]
                        lea              rsi, [rsp + 1312]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1296], rax
                        mov              qword ptr [rsp + 1304], rdx
                        cmp              eax, 99
                                                                                        je    n167_lit_string_α
                                                                                        jmp   n166_assign_var_α
n165_call_β:
                                                                                        jmp   n167_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n166_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1264]
                        mov              rsi, qword ptr [rsp + 1272]
                        mov              rdx, qword ptr [rsp + 1296]
                        mov              rcx, qword ptr [rsp + 1304]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n167_lit_string_α
                        mov              qword ptr [rsp + 1488], rax
                        mov              qword ptr [rsp + 1496], rdx
                                                                                        jmp   n167_lit_string_α
#=======================================================================================================================
#          Binary   =  .dummy                        :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n167_lit_string_α:
                        mov              qword ptr [rsp + 1552], 1
                        mov              dword ptr [rsp + 1556], 5
                        mov              rax, qword ptr [rip + .Lx201_0]
                        mov              qword ptr [rsp + 1560], rax
                                                                                        jmp   n168_call_α
.Lx201_0:
                        .quad            .Lx201_0_s
.Lx201_0_s:
                        .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n168_call_α:
                        mov              rax, qword ptr [rsp + 1552]
                        mov              qword ptr [rsp + 1520], rax
                        mov              rax, qword ptr [rsp + 1560]
                        mov              qword ptr [rsp + 1528], rax
                        .section         .rodata
.Lrkfn203:              .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn203]
                        lea              rsi, [rsp + 1520]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1504], rax
                        mov              qword ptr [rsp + 1512], rdx
                        cmp              eax, 99
                                                                                        je    n170_lit_string_α
                                                                                        jmp   n169_assign_α
n168_call_β:
                                                                                        jmp   n170_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n169_assign_α:
                        mov              rax, qword ptr [rsp + 1504]
                        mov              rdx, qword ptr [rsp + 1512]
                        mov              qword ptr [1879052384], rax
                        mov              qword ptr [1879052392], rdx
                                                                                        jmp   n170_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n170_lit_string_α:
                        mov              qword ptr [rsp + 32], 1
                        mov              dword ptr [rsp + 36], 0
                        mov              rax, qword ptr [rip + .Lx205_0]
                        mov              qword ptr [rsp + 40], rax
                                                                                        jmp   n171_call_α
.Lx205_0:
                        .quad            .Lx205_0_s
.Lx205_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n171_call_α:
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 72], rax
                        .section         .rodata
.Lrkfn207:              .string          "SNO$NRET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn207]
                        lea              rsi, [rsp + 64]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 48], rax
                        mov              qword ptr [rsp + 56], rdx
                        cmp              eax, 99
                                                                                        je    n173_save_restore_α
                                                                                        jmp   n172_save_restore_α
n171_call_β:
                                                                                        jmp   n173_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n172_save_restore_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n173_save_restore_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Binary_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Binary_β:
                                                                                        jmp   proc_LBL__Binary_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Binary_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 24]
                        lea              rsp, [rbp + 48]
                        mov              rbp, [rbp + 40]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Binary_ω:
                        mov              rax, [rbp + 32]
                        lea              rsp, [rbp + 48]
                        mov              rbp, [rbp + 40]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_Push_α
proc_Push_α:
                        .global          proc_Push_α
                        .global          proc_Push_β
                        .global          proc_Push_γ
                        .global          proc_Push_ω
                        sub              rsp, 48
                        mov              [rsp + 24], rcx
                        mov              [rsp + 32], rdx
                        mov              rdi, rsp
                        mov              ecx, 16
                        xor              eax, eax
                        rep stosb
proc_Push_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n212_save_restore_α:
                        mov              rdi, qword ptr [rsp + 24]
                        mov              rsi, qword ptr [rsp + 32]
                        lea              rdx, [rsp + 48]
                        mov              rcx, rbp
                        call             rt_flat_wire_adopt@PLT
                                                                                        jmp   n213_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n213_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx217_0]
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx217_1
.Lx217_0:
                        .quad            .Lx217_0_s
.Lx217_0_s:
                        .string          "Push"
.Lx217_1:
                                                                                        jmp   proc_Push_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_Push_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_Push_β:
                                                                                        jmp   proc_Push_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_Push_γ:
                        mov              rdi, [rsp]
                        mov              rsi, [rsp + 8]
                        mov              rax, [rsp + 24]
                        add              rsp, 48
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_Push_ω:
                        mov              rax, [rsp + 32]
                        add              rsp, 48
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_Pop_α
proc_Pop_α:
                        .global          proc_Pop_α
                        .global          proc_Pop_β
                        .global          proc_Pop_γ
                        .global          proc_Pop_ω
                        sub              rsp, 48
                        mov              [rsp + 24], rcx
                        mov              [rsp + 32], rdx
                        mov              rdi, rsp
                        mov              ecx, 16
                        xor              eax, eax
                        rep stosb
proc_Pop_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n218_save_restore_α:
                        mov              rdi, qword ptr [rsp + 24]
                        mov              rsi, qword ptr [rsp + 32]
                        lea              rdx, [rsp + 48]
                        mov              rcx, rbp
                        call             rt_flat_wire_adopt@PLT
                                                                                        jmp   n219_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n219_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx223_0]
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx223_1
.Lx223_0:
                        .quad            .Lx223_0_s
.Lx223_0_s:
                        .string          "Pop"
.Lx223_1:
                                                                                        jmp   proc_Pop_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_Pop_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_Pop_β:
                                                                                        jmp   proc_Pop_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_Pop_γ:
                        mov              rdi, [rsp]
                        mov              rsi, [rsp + 8]
                        mov              rax, [rsp + 24]
                        add              rsp, 48
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_Pop_ω:
                        mov              rax, [rsp + 32]
                        add              rsp, 48
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_Unary_α
proc_Unary_α:
                        .global          proc_Unary_α
                        .global          proc_Unary_β
                        .global          proc_Unary_γ
                        .global          proc_Unary_ω
                        sub              rsp, 48
                        mov              [rsp + 24], rcx
                        mov              [rsp + 32], rdx
                        mov              rdi, rsp
                        mov              ecx, 16
                        xor              eax, eax
                        rep stosb
proc_Unary_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n224_save_restore_α:
                        mov              rdi, qword ptr [rsp + 24]
                        mov              rsi, qword ptr [rsp + 32]
                        lea              rdx, [rsp + 48]
                        mov              rcx, rbp
                        call             rt_flat_wire_adopt@PLT
                                                                                        jmp   n225_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n225_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx229_0]
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx229_1
.Lx229_0:
                        .quad            .Lx229_0_s
.Lx229_0_s:
                        .string          "Unary"
.Lx229_1:
                                                                                        jmp   proc_Unary_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_Unary_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_Unary_β:
                                                                                        jmp   proc_Unary_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_Unary_γ:
                        mov              rdi, [rsp]
                        mov              rsi, [rsp + 8]
                        mov              rax, [rsp + 24]
                        add              rsp, 48
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_Unary_ω:
                        mov              rax, [rsp + 32]
                        add              rsp, 48
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_Binary_α
proc_Binary_α:
                        .global          proc_Binary_α
                        .global          proc_Binary_β
                        .global          proc_Binary_γ
                        .global          proc_Binary_ω
                        sub              rsp, 48
                        mov              [rsp + 24], rcx
                        mov              [rsp + 32], rdx
                        mov              rdi, rsp
                        mov              ecx, 16
                        xor              eax, eax
                        rep stosb
proc_Binary_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n230_save_restore_α:
                        mov              rdi, qword ptr [rsp + 24]
                        mov              rsi, qword ptr [rsp + 32]
                        lea              rdx, [rsp + 48]
                        mov              rcx, rbp
                        call             rt_flat_wire_adopt@PLT
                                                                                        jmp   n231_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n231_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx235_0]
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx235_1
.Lx235_0:
                        .quad            .Lx235_0_s
.Lx235_0_s:
                        .string          "Binary"
.Lx235_1:
                                                                                        jmp   proc_Binary_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_Binary_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_Binary_β:
                                                                                        jmp   proc_Binary_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_Binary_γ:
                        mov              rdi, [rsp]
                        mov              rsi, [rsp + 8]
                        mov              rax, [rsp + 24]
                        add              rsp, 48
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_Binary_ω:
                        mov              rax, [rsp + 32]
                        add              rsp, 48
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$0_α
proc_PAT$0_α:
                        .global          proc_PAT$0_α
                        .global          proc_PAT$0_β
                        .global          proc_PAT$0_γ
                        .global          proc_PAT$0_ω
                        sub              rsp, 48
                        mov              [rsp + 24], rcx
                        mov              [rsp + 32], rdx
                        mov              [rsp + 40], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 16], r8
                        mov              dword ptr [rsp + 8], r14d
proc_PAT$0_attempt:
proc_PAT$0_α_body:
                        lea              rax, [rip + n236_match_span_β]
                        mov              qword ptr [rbp + 16], rax
#-----------------------------------------------------------------------------------------------------------------------
n236_match_span_α:
                        sub              rsp, 16
                        lea              rdi, [rip + .C0]
                        movsxd           rcx, r14d
.Lx238_0:
                        cmp              ecx, r15d
                                                                                        jge   .Lx238_1
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              byte ptr [rdi+rsi], 0
                                                                                        je    .Lx238_1
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jge   .Lx238_1
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              byte ptr [rdi+rsi], 0
                                                                                        je    .Lx238_1
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jge   .Lx238_1
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              byte ptr [rdi+rsi], 0
                                                                                        je    .Lx238_1
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jge   .Lx238_1
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              byte ptr [rdi+rsi], 0
                                                                                        je    .Lx238_1
                        add              ecx, 1
                                                                                        jmp   .Lx238_0
.Lx238_1:
                        cmp              ecx, r14d
                                                                                        jg    .Lx238_240
                        add              rsp, 16
                                                                                        jmp   proc_PAT$0_scanfail
.Lx238_240:
                        mov              dword ptr [rsp + 4], r14d
                        mov              r14d, ecx
                                                                                        jmp   proc_PAT$0_scanhit
n236_match_span_β:
                        mov              r14d, dword ptr [rsp + 4]
                        add              rsp, 16
                                                                                        jmp   proc_PAT$0_scanfail
proc_PAT$0_scanhit:
                        cmp              qword ptr [rbp + 16], 1
                                                                                        jne   7f
                        mov              ecx, dword ptr [rbp + 8]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx], ecx
7:
                                                                                        jmp   proc_PAT$0_γ
proc_PAT$0_scanfail:
                        cmp              qword ptr [rbp + 16], 1
                                                                                        jne   8f
                        mov              eax, dword ptr [rbp + 8]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    8f
                        lea              rcx, [rip + g_anchor]
                        cmp              qword ptr [rcx], 0
                                                                                        jne   8f
                        mov              dword ptr [rbp + 8], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$0_attempt
8:
                                                                                        jmp   proc_PAT$0_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_β:
                                                                                        jmp   qword ptr [rbp + 16]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$0_res]
                        push             rax
                        mov              rax, [rbp + 24]
                        mov              rbp, [rbp + 40]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_ω:
                        mov              rax, [rbp + 32]
                        lea              rsp, [rbp + 48]
                        mov              rbp, [rbp + 40]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$1_α
proc_PAT$1_α:
                        .global          proc_PAT$1_α
                        .global          proc_PAT$1_β
                        .global          proc_PAT$1_γ
                        .global          proc_PAT$1_ω
                        sub              rsp, 48
                        mov              [rsp + 24], rcx
                        mov              [rsp + 32], rdx
                        mov              [rsp + 40], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 16], r8
                        mov              dword ptr [rsp + 8], r14d
proc_PAT$1_attempt:
proc_PAT$1_α_body:
                        lea              rax, [rip + n239_match_sequence_β]
                        mov              qword ptr [rbp + 112], rax
#-----------------------------------------------------------------------------------------------------------------------
n239_match_sequence_α:
                        mov              dword ptr [rbp + 16], r14d
                                                                                        jmp   n244_match_any_α
n239_match_sequence_as:
                                                                                        jmp   proc_PAT$1_γ
n239_match_sequence_β:
                                                                                        jmp   n240_match_patref_β
n239_match_sequence_af:
                                                                                        jmp   proc_PAT$1_ω
#-----------------------------------------------------------------------------------------------------------------------
n240_match_patref_α:
                        mov              rax, qword ptr [1879052464]
                        mov              rdx, qword ptr [1879052472]
                        cmp              eax, 3
                                                                                        jne   .Lx247_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx247_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx247_10
.Lx247_9:
                        xor              eax, eax
.Lx247_10:
                        test             rax, rax
                                                                                        jz    .Lx247_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx247_4]
                        lea              rdx, [rip + .Lx247_5]
                                                                                        jmp   rax
.Lx247_4:
                                                                                        jmp   proc_PAT$1_γ
.Lx247_5:
                                                                                        jmp   n241_match_alternate_β
.Lx247_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S0]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx247_2:
                        test             rax, rax
                                                                                        je    .Lx247_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx247_7]
                        lea              rdx, [rip + .Lx247_8]
                                                                                        jmp   rax
.Lx247_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx247_2
.Lx247_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx247_2
.Lx247_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n241_match_alternate_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx247_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   proc_PAT$1_γ
.Lx247_6:
                        add              rsp, 16
                                                                                        jmp   n241_match_alternate_β
n240_match_patref_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n241_match_alternate_α:
                        sub              rsp, 32
                        mov              dword ptr [rbp + 48], r14d
                        lea              rax, [rip + .Lx249_21]
                        mov              qword ptr [rbp + 64], rax
                                                                                        jmp   n243_match_any_α
.Lx249_21:
                        lea              rax, [rip + .Lx249_19]
                        mov              qword ptr [rbp + 64], rax
                                                                                        jmp   n242_match_patref_α
n241_match_alternate_s0:
                        lea              rax, [rip + .Lx249_40]
                        mov              qword ptr [rbp + 56], rax
                                                                                        jmp   n241_match_alternate_as
n241_match_alternate_s1:
                        lea              rax, [rip + .Lx249_41]
                        mov              qword ptr [rbp + 56], rax
                                                                                        jmp   n241_match_alternate_as
.Lx249_40:
                                                                                        jmp   n243_match_any_β
.Lx249_41:
                                                                                        jmp   n242_match_patref_β
n241_match_alternate_as:
                        add              rsp, 32
                                                                                        jmp   n240_match_patref_α
n241_match_alternate_β:
                        sub              rsp, 32
                        mov              rax, qword ptr [rbp + 56]
                                                                                        jmp   rax
n241_match_alternate_af:
                        mov              r14d, dword ptr [rbp + 48]
                        mov              rax, qword ptr [rbp + 64]
                                                                                        jmp   rax
.Lx249_19:
                        add              rsp, 32
                                                                                        jmp   n244_match_any_β
#-----------------------------------------------------------------------------------------------------------------------
n242_match_patref_α:
                        lea              rdi, [rip + .S1]
                        xor              esi, esi
                        call             rt_defer_get_pat_fn@PLT
                        test             rax, rax
                                                                                        jz    .Lx250_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx250_4]
                        lea              rdx, [rip + .Lx250_5]
                                                                                        jmp   rax
.Lx250_4:
                                                                                        jmp   n241_match_alternate_s1
.Lx250_5:
                                                                                        jmp   n241_match_alternate_af
.Lx250_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S1]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx250_2:
                        test             rax, rax
                                                                                        je    .Lx250_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx250_7]
                        lea              rdx, [rip + .Lx250_8]
                                                                                        jmp   rax
.Lx250_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx250_2
.Lx250_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx250_2
.Lx250_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n241_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx250_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n241_match_alternate_s1
.Lx250_6:
                        add              rsp, 16
                                                                                        jmp   n241_match_alternate_af
n242_match_patref_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n243_match_any_α:
                        mov              eax, r14d
                        cmp              eax, r15d
                                                                                        jge   n241_match_alternate_af
                        movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 43
                                                                                        je    .Lx252_0
                        cmp              esi, 45
                                                                                        je    .Lx252_0
                                                                                        jmp   n241_match_alternate_af
.Lx252_0:
                        add              r14d, 1
                                                                                        jmp   n241_match_alternate_s0
n243_match_any_β:
                        sub              r14d, 1
                                                                                        jmp   n241_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n244_match_any_α:
                        mov              eax, r14d
                        cmp              eax, r15d
                                                                                        jge   proc_PAT$1_ω
                        movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        lea              rdi, [rip + .C1]
                        cmp              byte ptr [rdi+rsi], 0
                                                                                        je    proc_PAT$1_ω
                        add              r14d, 1
                                                                                        jmp   n241_match_alternate_α
n244_match_any_β:
                        sub              r14d, 1
                                                                                        jmp   proc_PAT$1_ω
proc_PAT$1_scanhit:
                        cmp              qword ptr [rbp + 16], 1
                                                                                        jne   7f
                        mov              ecx, dword ptr [rbp + 8]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx], ecx
7:
                                                                                        jmp   proc_PAT$1_γ
proc_PAT$1_scanfail:
                        cmp              qword ptr [rbp + 16], 1
                                                                                        jne   8f
                        mov              eax, dword ptr [rbp + 8]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    8f
                        lea              rcx, [rip + g_anchor]
                        cmp              qword ptr [rcx], 0
                                                                                        jne   8f
                        mov              dword ptr [rbp + 8], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$1_attempt
8:
                                                                                        jmp   proc_PAT$1_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$1_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$1_β:
                                                                                        jmp   qword ptr [rbp + 112]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$1_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$1_res]
                        push             rax
                        mov              rax, [rbp + 24]
                        mov              rbp, [rbp + 40]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$1_ω:
                        mov              rax, [rbp + 32]
                        lea              rsp, [rbp + 48]
                        mov              rbp, [rbp + 40]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$2_α
proc_PAT$2_α:
                        .global          proc_PAT$2_α
                        .global          proc_PAT$2_β
                        .global          proc_PAT$2_γ
                        .global          proc_PAT$2_ω
                        sub              rsp, 48
                        mov              [rsp + 24], rcx
                        mov              [rsp + 32], rdx
                        mov              [rsp + 40], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 16], r8
                        mov              dword ptr [rsp + 8], r14d
proc_PAT$2_attempt:
proc_PAT$2_α_body:
                        lea              rax, [rip + n255_match_alternate_β]
                        mov              qword ptr [rbp + 272], rax
#-----------------------------------------------------------------------------------------------------------------------
n255_match_alternate_α:
                        sub              rsp, 32
                        mov              dword ptr [rbp + 16], r14d
                        lea              rax, [rip + .Lx269_21]
                        mov              qword ptr [rbp + 32], rax
                                                                                        jmp   n259_match_sequence_α
.Lx269_21:
                        lea              rax, [rip + .Lx269_19]
                        mov              qword ptr [rbp + 32], rax
                                                                                        jmp   n256_match_sequence_α
n255_match_alternate_s0:
                        lea              rax, [rip + .Lx269_40]
                        mov              qword ptr [rbp + 24], rax
                                                                                        jmp   n255_match_alternate_as
n255_match_alternate_s1:
                        lea              rax, [rip + .Lx269_41]
                        mov              qword ptr [rbp + 24], rax
                                                                                        jmp   n255_match_alternate_as
.Lx269_40:
                                                                                        jmp   n259_match_sequence_β
.Lx269_41:
                                                                                        jmp   n256_match_sequence_β
n255_match_alternate_as:
                        add              rsp, 32
                                                                                        jmp   proc_PAT$2_γ
n255_match_alternate_β:
                        sub              rsp, 32
                        mov              rax, qword ptr [rbp + 24]
                                                                                        jmp   rax
n255_match_alternate_af:
                        mov              r14d, dword ptr [rbp + 16]
                        mov              rax, qword ptr [rbp + 32]
                                                                                        jmp   rax
.Lx269_19:
                        add              rsp, 32
                                                                                        jmp   proc_PAT$2_ω
#-----------------------------------------------------------------------------------------------------------------------
n256_match_sequence_α:
                        mov              dword ptr [rbp + 224], r14d
                                                                                        jmp   n258_match_patref_α
n256_match_sequence_as:
                                                                                        jmp   n255_match_alternate_s1
n256_match_sequence_β:
                                                                                        jmp   n257_match_patref_β
n256_match_sequence_af:
                                                                                        jmp   n255_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n257_match_patref_α:
                        mov              rax, qword ptr [1879052480]
                        mov              rdx, qword ptr [1879052488]
                        cmp              eax, 3
                                                                                        jne   .Lx272_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx272_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx272_10
.Lx272_9:
                        xor              eax, eax
.Lx272_10:
                        test             rax, rax
                                                                                        jz    .Lx272_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx272_4]
                        lea              rdx, [rip + .Lx272_5]
                                                                                        jmp   rax
.Lx272_4:
                                                                                        jmp   n255_match_alternate_s1
.Lx272_5:
                                                                                        jmp   n258_match_patref_β
.Lx272_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S2]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx272_2:
                        test             rax, rax
                                                                                        je    .Lx272_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx272_7]
                        lea              rdx, [rip + .Lx272_8]
                                                                                        jmp   rax
.Lx272_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx272_2
.Lx272_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx272_2
.Lx272_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n258_match_patref_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx272_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n255_match_alternate_s1
.Lx272_6:
                        add              rsp, 16
                                                                                        jmp   n258_match_patref_β
n257_match_patref_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n258_match_patref_α:
                        mov              rax, qword ptr [1879052464]
                        mov              rdx, qword ptr [1879052472]
                        cmp              eax, 3
                                                                                        jne   .Lx273_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx273_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx273_10
.Lx273_9:
                        xor              eax, eax
.Lx273_10:
                        test             rax, rax
                                                                                        jz    .Lx273_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx273_4]
                        lea              rdx, [rip + .Lx273_5]
                                                                                        jmp   rax
.Lx273_4:
                                                                                        jmp   n257_match_patref_α
.Lx273_5:
                                                                                        jmp   n255_match_alternate_af
.Lx273_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S0]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx273_2:
                        test             rax, rax
                                                                                        je    .Lx273_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx273_7]
                        lea              rdx, [rip + .Lx273_8]
                                                                                        jmp   rax
.Lx273_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx273_2
.Lx273_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx273_2
.Lx273_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n255_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx273_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n257_match_patref_α
.Lx273_6:
                        add              rsp, 16
                                                                                        jmp   n255_match_alternate_af
n258_match_patref_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n259_match_sequence_α:
                        mov              dword ptr [rbp + 48], r14d
                                                                                        jmp   n267_match_patref_α
n259_match_sequence_as:
                                                                                        jmp   n255_match_alternate_s0
n259_match_sequence_β:
                                                                                        jmp   n260_match_alternate_β
n259_match_sequence_af:
                                                                                        jmp   n255_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n260_match_alternate_α:
                        sub              rsp, 32
                        mov              dword ptr [rbp + 160], r14d
                        lea              rax, [rip + .Lx277_21]
                        mov              qword ptr [rbp + 176], rax
                                                                                        jmp   n262_match_patref_α
.Lx277_21:
                        lea              rax, [rip + .Lx277_19]
                        mov              qword ptr [rbp + 176], rax
                                                                                        jmp   n261_match_patref_α
n260_match_alternate_s0:
                        lea              rax, [rip + .Lx277_40]
                        mov              qword ptr [rbp + 168], rax
                                                                                        jmp   n260_match_alternate_as
n260_match_alternate_s1:
                        lea              rax, [rip + .Lx277_41]
                        mov              qword ptr [rbp + 168], rax
                                                                                        jmp   n260_match_alternate_as
.Lx277_40:
                                                                                        jmp   n262_match_patref_β
.Lx277_41:
                                                                                        jmp   n261_match_patref_β
n260_match_alternate_as:
                        add              rsp, 32
                                                                                        jmp   n255_match_alternate_s0
n260_match_alternate_β:
                        sub              rsp, 32
                        mov              rax, qword ptr [rbp + 168]
                                                                                        jmp   rax
n260_match_alternate_af:
                        mov              r14d, dword ptr [rbp + 160]
                        mov              rax, qword ptr [rbp + 176]
                                                                                        jmp   rax
.Lx277_19:
                        add              rsp, 32
                                                                                        jmp   n263_match_alternate_β
#-----------------------------------------------------------------------------------------------------------------------
n261_match_patref_α:
                        lea              rdi, [rip + .S1]
                        xor              esi, esi
                        call             rt_defer_get_pat_fn@PLT
                        test             rax, rax
                                                                                        jz    .Lx278_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx278_4]
                        lea              rdx, [rip + .Lx278_5]
                                                                                        jmp   rax
.Lx278_4:
                                                                                        jmp   n260_match_alternate_s1
.Lx278_5:
                                                                                        jmp   n260_match_alternate_af
.Lx278_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S1]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx278_2:
                        test             rax, rax
                                                                                        je    .Lx278_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx278_7]
                        lea              rdx, [rip + .Lx278_8]
                                                                                        jmp   rax
.Lx278_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx278_2
.Lx278_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx278_2
.Lx278_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n260_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx278_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n260_match_alternate_s1
.Lx278_6:
                        add              rsp, 16
                                                                                        jmp   n260_match_alternate_af
n261_match_patref_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n262_match_patref_α:
                        mov              rax, qword ptr [1879052480]
                        mov              rdx, qword ptr [1879052488]
                        cmp              eax, 3
                                                                                        jne   .Lx279_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx279_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx279_10
.Lx279_9:
                        xor              eax, eax
.Lx279_10:
                        test             rax, rax
                                                                                        jz    .Lx279_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx279_4]
                        lea              rdx, [rip + .Lx279_5]
                                                                                        jmp   rax
.Lx279_4:
                                                                                        jmp   n260_match_alternate_s0
.Lx279_5:
                                                                                        jmp   n260_match_alternate_af
.Lx279_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S2]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx279_2:
                        test             rax, rax
                                                                                        je    .Lx279_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx279_7]
                        lea              rdx, [rip + .Lx279_8]
                                                                                        jmp   rax
.Lx279_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx279_2
.Lx279_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx279_2
.Lx279_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n260_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx279_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n260_match_alternate_s0
.Lx279_6:
                        add              rsp, 16
                                                                                        jmp   n260_match_alternate_af
n262_match_patref_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n263_match_alternate_α:
                        sub              rsp, 32
                        mov              dword ptr [rbp + 96], r14d
                        lea              rax, [rip + .Lx281_21]
                        mov              qword ptr [rbp + 112], rax
                                                                                        jmp   n265_match_patref_α
.Lx281_21:
                        lea              rax, [rip + .Lx281_19]
                        mov              qword ptr [rbp + 112], rax
                                                                                        jmp   n264_match_patref_α
n263_match_alternate_s0:
                        lea              rax, [rip + .Lx281_40]
                        mov              qword ptr [rbp + 104], rax
                                                                                        jmp   n263_match_alternate_as
n263_match_alternate_s1:
                        lea              rax, [rip + .Lx281_41]
                        mov              qword ptr [rbp + 104], rax
                                                                                        jmp   n263_match_alternate_as
.Lx281_40:
                                                                                        jmp   n265_match_patref_β
.Lx281_41:
                                                                                        jmp   n264_match_patref_β
n263_match_alternate_as:
                        add              rsp, 32
                                                                                        jmp   n260_match_alternate_α
n263_match_alternate_β:
                        sub              rsp, 32
                        mov              rax, qword ptr [rbp + 104]
                                                                                        jmp   rax
n263_match_alternate_af:
                        mov              r14d, dword ptr [rbp + 96]
                        mov              rax, qword ptr [rbp + 112]
                                                                                        jmp   rax
.Lx281_19:
                        add              rsp, 32
                                                                                        jmp   n266_match_lit_β
#-----------------------------------------------------------------------------------------------------------------------
n264_match_patref_α:
                        lea              rdi, [rip + .S1]
                        xor              esi, esi
                        call             rt_defer_get_pat_fn@PLT
                        test             rax, rax
                                                                                        jz    .Lx282_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx282_4]
                        lea              rdx, [rip + .Lx282_5]
                                                                                        jmp   rax
.Lx282_4:
                                                                                        jmp   n263_match_alternate_s1
.Lx282_5:
                                                                                        jmp   n263_match_alternate_af
.Lx282_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S1]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx282_2:
                        test             rax, rax
                                                                                        je    .Lx282_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx282_7]
                        lea              rdx, [rip + .Lx282_8]
                                                                                        jmp   rax
.Lx282_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx282_2
.Lx282_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx282_2
.Lx282_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n263_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx282_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n263_match_alternate_s1
.Lx282_6:
                        add              rsp, 16
                                                                                        jmp   n263_match_alternate_af
n264_match_patref_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n265_match_patref_α:
                        mov              rax, qword ptr [1879052464]
                        mov              rdx, qword ptr [1879052472]
                        cmp              eax, 3
                                                                                        jne   .Lx283_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx283_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx283_10
.Lx283_9:
                        xor              eax, eax
.Lx283_10:
                        test             rax, rax
                                                                                        jz    .Lx283_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx283_4]
                        lea              rdx, [rip + .Lx283_5]
                                                                                        jmp   rax
.Lx283_4:
                                                                                        jmp   n263_match_alternate_s0
.Lx283_5:
                                                                                        jmp   n263_match_alternate_af
.Lx283_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S0]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx283_2:
                        test             rax, rax
                                                                                        je    .Lx283_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx283_7]
                        lea              rdx, [rip + .Lx283_8]
                                                                                        jmp   rax
.Lx283_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx283_2
.Lx283_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx283_2
.Lx283_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n263_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx283_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n263_match_alternate_s0
.Lx283_6:
                        add              rsp, 16
                                                                                        jmp   n263_match_alternate_af
n265_match_patref_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n266_match_lit_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jg    n267_match_patref_β
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 46
                                                                                        jne   n267_match_patref_β
                        add              r14d, 1
                                                                                        jmp   n263_match_alternate_α
n266_match_lit_β:
                        sub              r14d, 1
                                                                                        jmp   n267_match_patref_β
#-----------------------------------------------------------------------------------------------------------------------
n267_match_patref_α:
                        mov              rax, qword ptr [1879052464]
                        mov              rdx, qword ptr [1879052472]
                        cmp              eax, 3
                                                                                        jne   .Lx286_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx286_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx286_10
.Lx286_9:
                        xor              eax, eax
.Lx286_10:
                        test             rax, rax
                                                                                        jz    .Lx286_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx286_4]
                        lea              rdx, [rip + .Lx286_5]
                                                                                        jmp   rax
.Lx286_4:
                                                                                        jmp   n266_match_lit_α
.Lx286_5:
                                                                                        jmp   n255_match_alternate_af
.Lx286_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S0]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx286_2:
                        test             rax, rax
                                                                                        je    .Lx286_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx286_7]
                        lea              rdx, [rip + .Lx286_8]
                                                                                        jmp   rax
.Lx286_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx286_2
.Lx286_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx286_2
.Lx286_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n255_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx286_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n266_match_lit_α
.Lx286_6:
                        add              rsp, 16
                                                                                        jmp   n255_match_alternate_af
n267_match_patref_β:
                                                                                        jmp   qword ptr [rsp]
proc_PAT$2_scanhit:
                        cmp              qword ptr [rbp + 16], 1
                                                                                        jne   7f
                        mov              ecx, dword ptr [rbp + 8]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx], ecx
7:
                                                                                        jmp   proc_PAT$2_γ
proc_PAT$2_scanfail:
                        cmp              qword ptr [rbp + 16], 1
                                                                                        jne   8f
                        mov              eax, dword ptr [rbp + 8]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    8f
                        lea              rcx, [rip + g_anchor]
                        cmp              qword ptr [rcx], 0
                                                                                        jne   8f
                        mov              dword ptr [rbp + 8], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$2_attempt
8:
                                                                                        jmp   proc_PAT$2_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$2_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$2_β:
                                                                                        jmp   qword ptr [rbp + 272]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$2_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$2_res]
                        push             rax
                        mov              rax, [rbp + 24]
                        mov              rbp, [rbp + 40]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$2_ω:
                        mov              rax, [rbp + 32]
                        lea              rsp, [rbp + 48]
                        mov              rbp, [rbp + 40]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$3_α
proc_PAT$3_α:
                        .global          proc_PAT$3_α
                        .global          proc_PAT$3_β
                        .global          proc_PAT$3_γ
                        .global          proc_PAT$3_ω
                        sub              rsp, 48
                        mov              [rsp + 24], rcx
                        mov              [rsp + 32], rdx
                        mov              [rsp + 40], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 16], r8
                        mov              dword ptr [rsp + 8], r14d
proc_PAT$3_attempt:
proc_PAT$3_α_body:
                        lea              rax, [rip + n289_match_assign_cond_β]
                        mov              qword ptr [rbp + 80], rax
#-----------------------------------------------------------------------------------------------------------------------
n287_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                                                                                        jmp   n288_match_any_α
n287_match_assign_save_β:
                        add              rsp, 16
                                                                                        jmp   proc_PAT$3_scanfail
#-----------------------------------------------------------------------------------------------------------------------
n288_match_any_α:
                        mov              eax, r14d
                        cmp              eax, r15d
                                                                                        jl    .Lx293_240
                        add              rsp, 16
                                                                                        jmp   proc_PAT$3_scanfail
.Lx293_240:
                        movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 43
                                                                                        je    .Lx293_0
                        cmp              esi, 45
                                                                                        je    .Lx293_0
                        add              rsp, 16
                                                                                        jmp   proc_PAT$3_scanfail
.Lx293_0:
                        add              r14d, 1
                                                                                        jmp   n289_match_assign_cond_α
n288_match_any_β:
                        sub              r14d, 1
                        add              rsp, 16
                                                                                        jmp   proc_PAT$3_scanfail
#-----------------------------------------------------------------------------------------------------------------------
n289_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 0]
                        lea              rcx, [rip + .S3]
                        mov              r10, qword ptr [1879048192]
                        mov              qword ptr [r10 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r10 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r10 + 16], rdx
                        add              r10, 24
                        mov              qword ptr [1879048192], r10
                                                                                        jmp   proc_PAT$3_scanhit
n289_match_assign_cond_β:
                        mov              rax, qword ptr [1879048192]
                        sub              rax, 24
                        mov              qword ptr [1879048192], rax
                                                                                        jmp   n288_match_any_β
proc_PAT$3_scanhit:
                        cmp              qword ptr [rbp + 16], 1
                                                                                        jne   7f
                        mov              ecx, dword ptr [rbp + 8]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx], ecx
7:
                                                                                        jmp   proc_PAT$3_γ
proc_PAT$3_scanfail:
                        cmp              qword ptr [rbp + 16], 1
                                                                                        jne   8f
                        mov              eax, dword ptr [rbp + 8]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    8f
                        lea              rcx, [rip + g_anchor]
                        cmp              qword ptr [rcx], 0
                                                                                        jne   8f
                        mov              dword ptr [rbp + 8], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$3_attempt
8:
                                                                                        jmp   proc_PAT$3_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$3_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$3_β:
                                                                                        jmp   qword ptr [rbp + 80]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$3_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$3_res]
                        push             rax
                        mov              rax, [rbp + 24]
                        mov              rbp, [rbp + 40]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$3_ω:
                        mov              rax, [rbp + 32]
                        lea              rsp, [rbp + 48]
                        mov              rbp, [rbp + 40]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$4_α
proc_PAT$4_α:
                        .global          proc_PAT$4_α
                        .global          proc_PAT$4_β
                        .global          proc_PAT$4_γ
                        .global          proc_PAT$4_ω
                        sub              rsp, 48
                        mov              [rsp + 24], rcx
                        mov              [rsp + 32], rdx
                        mov              [rsp + 40], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 16], r8
                        mov              dword ptr [rsp + 8], r14d
proc_PAT$4_attempt:
proc_PAT$4_α_body:
                        lea              rax, [rip + n298_match_assign_cond_β]
                        mov              qword ptr [rbp + 80], rax
#-----------------------------------------------------------------------------------------------------------------------
n296_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                                                                                        jmp   n297_match_any_α
n296_match_assign_save_β:
                        add              rsp, 16
                                                                                        jmp   proc_PAT$4_scanfail
#-----------------------------------------------------------------------------------------------------------------------
n297_match_any_α:
                        mov              eax, r14d
                        cmp              eax, r15d
                                                                                        jl    .Lx302_240
                        add              rsp, 16
                                                                                        jmp   proc_PAT$4_scanfail
.Lx302_240:
                        movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 42
                                                                                        je    .Lx302_0
                        cmp              esi, 47
                                                                                        je    .Lx302_0
                        add              rsp, 16
                                                                                        jmp   proc_PAT$4_scanfail
.Lx302_0:
                        add              r14d, 1
                                                                                        jmp   n298_match_assign_cond_α
n297_match_any_β:
                        sub              r14d, 1
                        add              rsp, 16
                                                                                        jmp   proc_PAT$4_scanfail
#-----------------------------------------------------------------------------------------------------------------------
n298_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 0]
                        lea              rcx, [rip + .S3]
                        mov              r10, qword ptr [1879048192]
                        mov              qword ptr [r10 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r10 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r10 + 16], rdx
                        add              r10, 24
                        mov              qword ptr [1879048192], r10
                                                                                        jmp   proc_PAT$4_scanhit
n298_match_assign_cond_β:
                        mov              rax, qword ptr [1879048192]
                        sub              rax, 24
                        mov              qword ptr [1879048192], rax
                                                                                        jmp   n297_match_any_β
proc_PAT$4_scanhit:
                        cmp              qword ptr [rbp + 16], 1
                                                                                        jne   7f
                        mov              ecx, dword ptr [rbp + 8]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx], ecx
7:
                                                                                        jmp   proc_PAT$4_γ
proc_PAT$4_scanfail:
                        cmp              qword ptr [rbp + 16], 1
                                                                                        jne   8f
                        mov              eax, dword ptr [rbp + 8]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    8f
                        lea              rcx, [rip + g_anchor]
                        cmp              qword ptr [rcx], 0
                                                                                        jne   8f
                        mov              dword ptr [rbp + 8], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$4_attempt
8:
                                                                                        jmp   proc_PAT$4_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$4_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$4_β:
                                                                                        jmp   qword ptr [rbp + 80]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$4_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$4_res]
                        push             rax
                        mov              rax, [rbp + 24]
                        mov              rbp, [rbp + 40]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$4_ω:
                        mov              rax, [rbp + 32]
                        lea              rsp, [rbp + 48]
                        mov              rbp, [rbp + 40]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$5_α
proc_PAT$5_α:
                        .global          proc_PAT$5_α
                        .global          proc_PAT$5_β
                        .global          proc_PAT$5_γ
                        .global          proc_PAT$5_ω
                        sub              rsp, 48
                        mov              [rsp + 24], rcx
                        mov              [rsp + 32], rdx
                        mov              [rsp + 40], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 16], r8
                        mov              dword ptr [rsp + 8], r14d
proc_PAT$5_attempt:
proc_PAT$5_α_body:
                        lea              rax, [rip + n307_match_assign_cond_β]
                        mov              qword ptr [rbp + 128], rax
#-----------------------------------------------------------------------------------------------------------------------
n305_match_assign_save_α:
                        sub              rsp, 32
                        mov              dword ptr [rsp + 0], r14d
                        add              rsp, 32
                                                                                        jmp   n306_match_alternate_α
n305_match_assign_save_β:
                        sub              rsp, 32
                        add              rsp, 32
                                                                                        jmp   proc_PAT$5_scanfail
#-----------------------------------------------------------------------------------------------------------------------
n306_match_alternate_α:
                        sub              rsp, 32
                        mov              dword ptr [rbp + 64], r14d
                        lea              rax, [rip + .Lx313_21]
                        mov              qword ptr [rbp + 80], rax
                                                                                        jmp   n309_match_patref_α
.Lx313_21:
                        lea              rax, [rip + .Lx313_19]
                        mov              qword ptr [rbp + 80], rax
                                                                                        jmp   n308_match_patref_α
n306_match_alternate_s0:
                        lea              rax, [rip + .Lx313_40]
                        mov              qword ptr [rbp + 72], rax
                                                                                        jmp   n306_match_alternate_as
n306_match_alternate_s1:
                        lea              rax, [rip + .Lx313_41]
                        mov              qword ptr [rbp + 72], rax
                                                                                        jmp   n306_match_alternate_as
.Lx313_40:
                                                                                        jmp   n309_match_patref_β
.Lx313_41:
                                                                                        jmp   n308_match_patref_β
n306_match_alternate_as:
                        add              rsp, 32
                                                                                        jmp   n307_match_assign_cond_α
n306_match_alternate_β:
                        sub              rsp, 32
                        mov              rax, qword ptr [rbp + 72]
                                                                                        jmp   rax
n306_match_alternate_af:
                        mov              r14d, dword ptr [rbp + 64]
                        mov              rax, qword ptr [rbp + 80]
                                                                                        jmp   rax
.Lx313_19:
                        add              rsp, 32
                                                                                        jmp   n305_match_assign_save_β
#-----------------------------------------------------------------------------------------------------------------------
n307_match_assign_cond_α:
                        lea              rdi, [rbp + 48]
                        call             rt_cap_top@PLT
                        lea              rcx, [rip + .S3]
                        mov              r10, qword ptr [1879048192]
                        mov              qword ptr [r10 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r10 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r10 + 16], rdx
                        add              r10, 24
                        mov              qword ptr [1879048192], r10
                                                                                        jmp   proc_PAT$5_scanhit
n307_match_assign_cond_β:
                        mov              rax, qword ptr [1879048192]
                        sub              rax, 24
                        mov              qword ptr [1879048192], rax
                                                                                        jmp   n306_match_alternate_β
#-----------------------------------------------------------------------------------------------------------------------
n308_match_patref_α:
                        mov              rax, qword ptr [1879052464]
                        mov              rdx, qword ptr [1879052472]
                        cmp              eax, 3
                                                                                        jne   .Lx316_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx316_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx316_10
.Lx316_9:
                        xor              eax, eax
.Lx316_10:
                        test             rax, rax
                                                                                        jz    .Lx316_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx316_4]
                        lea              rdx, [rip + .Lx316_5]
                                                                                        jmp   rax
.Lx316_4:
                                                                                        jmp   n306_match_alternate_s1
.Lx316_5:
                                                                                        jmp   n306_match_alternate_af
.Lx316_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S0]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx316_2:
                        test             rax, rax
                                                                                        je    .Lx316_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx316_7]
                        lea              rdx, [rip + .Lx316_8]
                                                                                        jmp   rax
.Lx316_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx316_2
.Lx316_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx316_2
.Lx316_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n306_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx316_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n306_match_alternate_s1
.Lx316_6:
                        add              rsp, 16
                                                                                        jmp   n306_match_alternate_af
n308_match_patref_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n309_match_patref_α:
                        mov              rax, qword ptr [1879052496]
                        mov              rdx, qword ptr [1879052504]
                        cmp              eax, 3
                                                                                        jne   .Lx317_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx317_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx317_10
.Lx317_9:
                        xor              eax, eax
.Lx317_10:
                        test             rax, rax
                                                                                        jz    .Lx317_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx317_4]
                        lea              rdx, [rip + .Lx317_5]
                                                                                        jmp   rax
.Lx317_4:
                                                                                        jmp   n306_match_alternate_s0
.Lx317_5:
                                                                                        jmp   n306_match_alternate_af
.Lx317_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S4]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx317_2:
                        test             rax, rax
                                                                                        je    .Lx317_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx317_7]
                        lea              rdx, [rip + .Lx317_8]
                                                                                        jmp   rax
.Lx317_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx317_2
.Lx317_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx317_2
.Lx317_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n306_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx317_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n306_match_alternate_s0
.Lx317_6:
                        add              rsp, 16
                                                                                        jmp   n306_match_alternate_af
n309_match_patref_β:
                                                                                        jmp   qword ptr [rsp]
proc_PAT$5_scanhit:
                        cmp              qword ptr [rbp + 16], 1
                                                                                        jne   7f
                        mov              ecx, dword ptr [rbp + 8]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx], ecx
7:
                                                                                        jmp   proc_PAT$5_γ
proc_PAT$5_scanfail:
                        cmp              qword ptr [rbp + 16], 1
                                                                                        jne   8f
                        mov              eax, dword ptr [rbp + 8]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    8f
                        lea              rcx, [rip + g_anchor]
                        cmp              qword ptr [rcx], 0
                                                                                        jne   8f
                        mov              dword ptr [rbp + 8], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$5_attempt
8:
                                                                                        jmp   proc_PAT$5_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$5_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$5_β:
                                                                                        jmp   qword ptr [rbp + 128]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$5_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$5_res]
                        push             rax
                        mov              rax, [rbp + 24]
                        mov              rbp, [rbp + 40]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$5_ω:
                        mov              rax, [rbp + 32]
                        lea              rsp, [rbp + 48]
                        mov              rbp, [rbp + 40]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$6_α
proc_PAT$6_α:
                        .global          proc_PAT$6_α
                        .global          proc_PAT$6_β
                        .global          proc_PAT$6_γ
                        .global          proc_PAT$6_ω
                        sub              rsp, 48
                        mov              [rsp + 24], rcx
                        mov              [rsp + 32], rdx
                        mov              [rsp + 40], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 16], r8
                        mov              dword ptr [rsp + 8], r14d
proc_PAT$6_attempt:
proc_PAT$6_α_body:
                        lea              rax, [rip + n318_match_alternate_β]
                        mov              qword ptr [rbp + 112], rax
#-----------------------------------------------------------------------------------------------------------------------
n318_match_alternate_α:
                        sub              rsp, 32
                        mov              dword ptr [rbp + 16], r14d
                        lea              rax, [rip + .Lx325_21]
                        mov              qword ptr [rbp + 32], rax
                                                                                        jmp   n323_match_patref_α
.Lx325_21:
                        lea              rax, [rip + .Lx325_19]
                        mov              qword ptr [rbp + 32], rax
                                                                                        jmp   n319_match_sequence_α
n318_match_alternate_s0:
                        lea              rax, [rip + .Lx325_40]
                        mov              qword ptr [rbp + 24], rax
                                                                                        jmp   n318_match_alternate_as
n318_match_alternate_s1:
                        lea              rax, [rip + .Lx325_41]
                        mov              qword ptr [rbp + 24], rax
                                                                                        jmp   n318_match_alternate_as
.Lx325_40:
                                                                                        jmp   n323_match_patref_β
.Lx325_41:
                                                                                        jmp   n319_match_sequence_β
n318_match_alternate_as:
                        add              rsp, 32
                                                                                        jmp   proc_PAT$6_γ
n318_match_alternate_β:
                        sub              rsp, 32
                        mov              rax, qword ptr [rbp + 24]
                                                                                        jmp   rax
n318_match_alternate_af:
                        mov              r14d, dword ptr [rbp + 16]
                        mov              rax, qword ptr [rbp + 32]
                                                                                        jmp   rax
.Lx325_19:
                        add              rsp, 32
                                                                                        jmp   proc_PAT$6_ω
#-----------------------------------------------------------------------------------------------------------------------
n319_match_sequence_α:
                        mov              dword ptr [rbp + 64], r14d
                                                                                        jmp   n322_match_lit_α
n319_match_sequence_as:
                                                                                        jmp   n318_match_alternate_s1
n319_match_sequence_β:
                                                                                        jmp   n320_match_lit_β
n319_match_sequence_af:
                                                                                        jmp   n318_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n320_match_lit_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jg    n321_match_defer_β
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 41
                                                                                        jne   n321_match_defer_β
                        add              r14d, 1
                                                                                        jmp   n318_match_alternate_s1
n320_match_lit_β:
                        sub              r14d, 1
                                                                                        jmp   n321_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n321_match_defer_α:
                        mov              rax, qword ptr [1879052608]
                        mov              rdx, qword ptr [1879052616]
                        cmp              eax, 3
                                                                                        jne   .Lx330_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx330_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx330_10
.Lx330_9:
                        xor              eax, eax
.Lx330_10:
                        test             rax, rax
                                                                                        jz    .Lx330_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx330_4]
                        lea              rdx, [rip + .Lx330_5]
                                                                                        jmp   rax
.Lx330_4:
                                                                                        jmp   n320_match_lit_α
.Lx330_5:
                                                                                        jmp   n322_match_lit_β
.Lx330_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S5]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx330_2:
                        test             rax, rax
                                                                                        je    .Lx330_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx330_7]
                        lea              rdx, [rip + .Lx330_8]
                                                                                        jmp   rax
.Lx330_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx330_2
.Lx330_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx330_2
.Lx330_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n322_match_lit_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx330_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n320_match_lit_α
.Lx330_6:
                        add              rsp, 16
                                                                                        jmp   n322_match_lit_β
n321_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n322_match_lit_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jg    n318_match_alternate_af
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 40
                                                                                        jne   n318_match_alternate_af
                        add              r14d, 1
                                                                                        jmp   n321_match_defer_α
n322_match_lit_β:
                        sub              r14d, 1
                                                                                        jmp   n318_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n323_match_patref_α:
                        mov              rax, qword ptr [1879052544]
                        mov              rdx, qword ptr [1879052552]
                        cmp              eax, 3
                                                                                        jne   .Lx333_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx333_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx333_10
.Lx333_9:
                        xor              eax, eax
.Lx333_10:
                        test             rax, rax
                                                                                        jz    .Lx333_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx333_4]
                        lea              rdx, [rip + .Lx333_5]
                                                                                        jmp   rax
.Lx333_4:
                                                                                        jmp   n318_match_alternate_s0
.Lx333_5:
                                                                                        jmp   n318_match_alternate_af
.Lx333_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S6]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx333_2:
                        test             rax, rax
                                                                                        je    .Lx333_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx333_7]
                        lea              rdx, [rip + .Lx333_8]
                                                                                        jmp   rax
.Lx333_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx333_2
.Lx333_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx333_2
.Lx333_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n318_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx333_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n318_match_alternate_s0
.Lx333_6:
                        add              rsp, 16
                                                                                        jmp   n318_match_alternate_af
n323_match_patref_β:
                                                                                        jmp   qword ptr [rsp]
proc_PAT$6_scanhit:
                        cmp              qword ptr [rbp + 16], 1
                                                                                        jne   7f
                        mov              ecx, dword ptr [rbp + 8]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx], ecx
7:
                                                                                        jmp   proc_PAT$6_γ
proc_PAT$6_scanfail:
                        cmp              qword ptr [rbp + 16], 1
                                                                                        jne   8f
                        mov              eax, dword ptr [rbp + 8]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    8f
                        lea              rcx, [rip + g_anchor]
                        cmp              qword ptr [rcx], 0
                                                                                        jne   8f
                        mov              dword ptr [rbp + 8], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$6_attempt
8:
                                                                                        jmp   proc_PAT$6_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$6_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$6_β:
                                                                                        jmp   qword ptr [rbp + 112]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$6_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$6_res]
                        push             rax
                        mov              rax, [rbp + 24]
                        mov              rbp, [rbp + 40]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$6_ω:
                        mov              rax, [rbp + 32]
                        lea              rsp, [rbp + 48]
                        mov              rbp, [rbp + 40]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$7_α
proc_PAT$7_α:
                        .global          proc_PAT$7_α
                        .global          proc_PAT$7_β
                        .global          proc_PAT$7_γ
                        .global          proc_PAT$7_ω
                        sub              rsp, 48
                        mov              [rsp + 24], rcx
                        mov              [rsp + 32], rdx
                        mov              [rsp + 40], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 16], r8
                        mov              dword ptr [rsp + 8], r14d
proc_PAT$7_attempt:
proc_PAT$7_α_body:
                        lea              rax, [rip + n334_match_alternate_β]
                        mov              qword ptr [rbp + 160], rax
#-----------------------------------------------------------------------------------------------------------------------
n334_match_alternate_α:
                        sub              rsp, 32
                        mov              dword ptr [rbp + 16], r14d
                        lea              rax, [rip + .Lx342_21]
                        mov              qword ptr [rbp + 32], rax
                                                                                        jmp   n336_match_sequence_α
.Lx342_21:
                        lea              rax, [rip + .Lx342_19]
                        mov              qword ptr [rbp + 32], rax
                                                                                        jmp   n335_match_defer_α
n334_match_alternate_s0:
                        lea              rax, [rip + .Lx342_40]
                        mov              qword ptr [rbp + 24], rax
                                                                                        jmp   n334_match_alternate_as
n334_match_alternate_s1:
                        lea              rax, [rip + .Lx342_41]
                        mov              qword ptr [rbp + 24], rax
                                                                                        jmp   n334_match_alternate_as
.Lx342_40:
                                                                                        jmp   n336_match_sequence_β
.Lx342_41:
                                                                                        jmp   n335_match_defer_β
n334_match_alternate_as:
                        add              rsp, 32
                                                                                        jmp   proc_PAT$7_γ
n334_match_alternate_β:
                        sub              rsp, 32
                        mov              rax, qword ptr [rbp + 24]
                                                                                        jmp   rax
n334_match_alternate_af:
                        mov              r14d, dword ptr [rbp + 16]
                        mov              rax, qword ptr [rbp + 32]
                                                                                        jmp   rax
.Lx342_19:
                        add              rsp, 32
                                                                                        jmp   proc_PAT$7_ω
#-----------------------------------------------------------------------------------------------------------------------
n335_match_defer_α:
                        mov              rax, qword ptr [1879052560]
                        mov              rdx, qword ptr [1879052568]
                        cmp              eax, 3
                                                                                        jne   .Lx343_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx343_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx343_10
.Lx343_9:
                        xor              eax, eax
.Lx343_10:
                        test             rax, rax
                                                                                        jz    .Lx343_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx343_4]
                        lea              rdx, [rip + .Lx343_5]
                                                                                        jmp   rax
.Lx343_4:
                                                                                        jmp   n334_match_alternate_s1
.Lx343_5:
                                                                                        jmp   n334_match_alternate_af
.Lx343_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S7]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx343_2:
                        test             rax, rax
                                                                                        je    .Lx343_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx343_7]
                        lea              rdx, [rip + .Lx343_8]
                                                                                        jmp   rax
.Lx343_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx343_2
.Lx343_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx343_2
.Lx343_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n334_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx343_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n334_match_alternate_s1
.Lx343_6:
                        add              rsp, 16
                                                                                        jmp   n334_match_alternate_af
n335_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n336_match_sequence_α:
                        mov              dword ptr [rbp + 48], r14d
                                                                                        jmp   n340_match_patref_α
n336_match_sequence_as:
                                                                                        jmp   n334_match_alternate_s0
n336_match_sequence_β:
                                                                                        jmp   n339_match_assign_cond_β
n336_match_sequence_af:
                                                                                        jmp   n334_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n337_match_assign_save_α:
                        sub              rsp, 32
                        mov              dword ptr [rsp + 0], r14d
                        add              rsp, 32
                                                                                        jmp   n338_match_defer_α
n337_match_assign_save_β:
                        sub              rsp, 32
                        add              rsp, 32
                                                                                        jmp   n340_match_patref_β
#-----------------------------------------------------------------------------------------------------------------------
n338_match_defer_α:
                        mov              rax, qword ptr [1879052576]
                        mov              rdx, qword ptr [1879052584]
                        cmp              eax, 3
                                                                                        jne   .Lx348_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx348_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx348_10
.Lx348_9:
                        xor              eax, eax
.Lx348_10:
                        test             rax, rax
                                                                                        jz    .Lx348_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx348_4]
                        lea              rdx, [rip + .Lx348_5]
                                                                                        jmp   rax
.Lx348_4:
                                                                                        jmp   n339_match_assign_cond_α
.Lx348_5:
                                                                                        jmp   n337_match_assign_save_β
.Lx348_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S8]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx348_2:
                        test             rax, rax
                                                                                        je    .Lx348_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx348_7]
                        lea              rdx, [rip + .Lx348_8]
                                                                                        jmp   rax
.Lx348_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx348_2
.Lx348_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx348_2
.Lx348_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n337_match_assign_save_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx348_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n339_match_assign_cond_α
.Lx348_6:
                        add              rsp, 16
                                                                                        jmp   n337_match_assign_save_β
n338_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n339_match_assign_cond_α:
                        lea              rdi, [rbp + 112]
                        call             rt_cap_top@PLT
                        lea              rcx, [rip + .S9]
                        mov              r10, qword ptr [1879048192]
                        mov              qword ptr [r10 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r10 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r10 + 16], rdx
                        add              r10, 24
                        mov              qword ptr [1879048192], r10
                                                                                        jmp   n334_match_alternate_s0
n339_match_assign_cond_β:
                        mov              rax, qword ptr [1879048192]
                        sub              rax, 24
                        mov              qword ptr [1879048192], rax
                                                                                        jmp   n338_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n340_match_patref_α:
                        mov              rax, qword ptr [1879052512]
                        mov              rdx, qword ptr [1879052520]
                        cmp              eax, 3
                                                                                        jne   .Lx351_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx351_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx351_10
.Lx351_9:
                        xor              eax, eax
.Lx351_10:
                        test             rax, rax
                                                                                        jz    .Lx351_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx351_4]
                        lea              rdx, [rip + .Lx351_5]
                                                                                        jmp   rax
.Lx351_4:
                                                                                        jmp   n337_match_assign_save_α
.Lx351_5:
                                                                                        jmp   n334_match_alternate_af
.Lx351_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S10]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx351_2:
                        test             rax, rax
                                                                                        je    .Lx351_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx351_7]
                        lea              rdx, [rip + .Lx351_8]
                                                                                        jmp   rax
.Lx351_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx351_2
.Lx351_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx351_2
.Lx351_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n334_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx351_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n337_match_assign_save_α
.Lx351_6:
                        add              rsp, 16
                                                                                        jmp   n334_match_alternate_af
n340_match_patref_β:
                                                                                        jmp   qword ptr [rsp]
proc_PAT$7_scanhit:
                        cmp              qword ptr [rbp + 16], 1
                                                                                        jne   7f
                        mov              ecx, dword ptr [rbp + 8]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx], ecx
7:
                                                                                        jmp   proc_PAT$7_γ
proc_PAT$7_scanfail:
                        cmp              qword ptr [rbp + 16], 1
                                                                                        jne   8f
                        mov              eax, dword ptr [rbp + 8]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    8f
                        lea              rcx, [rip + g_anchor]
                        cmp              qword ptr [rcx], 0
                                                                                        jne   8f
                        mov              dword ptr [rbp + 8], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$7_attempt
8:
                                                                                        jmp   proc_PAT$7_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$7_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$7_β:
                                                                                        jmp   qword ptr [rbp + 160]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$7_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$7_res]
                        push             rax
                        mov              rax, [rbp + 24]
                        mov              rbp, [rbp + 40]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$7_ω:
                        mov              rax, [rbp + 32]
                        lea              rsp, [rbp + 48]
                        mov              rbp, [rbp + 40]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$8_α
proc_PAT$8_α:
                        .global          proc_PAT$8_α
                        .global          proc_PAT$8_β
                        .global          proc_PAT$8_γ
                        .global          proc_PAT$8_ω
                        sub              rsp, 48
                        mov              [rsp + 24], rcx
                        mov              [rsp + 32], rdx
                        mov              [rsp + 40], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 16], r8
                        mov              dword ptr [rsp + 8], r14d
proc_PAT$8_attempt:
proc_PAT$8_α_body:
                        lea              rax, [rip + n352_match_alternate_β]
                        mov              qword ptr [rbp + 176], rax
#-----------------------------------------------------------------------------------------------------------------------
n352_match_alternate_α:
                        sub              rsp, 32
                        mov              dword ptr [rbp + 16], r14d
                        lea              rax, [rip + .Lx361_21]
                        mov              qword ptr [rbp + 32], rax
                                                                                        jmp   n354_match_sequence_α
.Lx361_21:
                        lea              rax, [rip + .Lx361_19]
                        mov              qword ptr [rbp + 32], rax
                                                                                        jmp   n353_match_defer_α
n352_match_alternate_s0:
                        lea              rax, [rip + .Lx361_40]
                        mov              qword ptr [rbp + 24], rax
                                                                                        jmp   n352_match_alternate_as
n352_match_alternate_s1:
                        lea              rax, [rip + .Lx361_41]
                        mov              qword ptr [rbp + 24], rax
                                                                                        jmp   n352_match_alternate_as
.Lx361_40:
                                                                                        jmp   n354_match_sequence_β
.Lx361_41:
                                                                                        jmp   n353_match_defer_β
n352_match_alternate_as:
                        add              rsp, 32
                                                                                        jmp   proc_PAT$8_γ
n352_match_alternate_β:
                        sub              rsp, 32
                        mov              rax, qword ptr [rbp + 24]
                                                                                        jmp   rax
n352_match_alternate_af:
                        mov              r14d, dword ptr [rbp + 16]
                        mov              rax, qword ptr [rbp + 32]
                                                                                        jmp   rax
.Lx361_19:
                        add              rsp, 32
                                                                                        jmp   proc_PAT$8_ω
#-----------------------------------------------------------------------------------------------------------------------
n353_match_defer_α:
                        mov              rax, qword ptr [1879052576]
                        mov              rdx, qword ptr [1879052584]
                        cmp              eax, 3
                                                                                        jne   .Lx362_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx362_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx362_10
.Lx362_9:
                        xor              eax, eax
.Lx362_10:
                        test             rax, rax
                                                                                        jz    .Lx362_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx362_4]
                        lea              rdx, [rip + .Lx362_5]
                                                                                        jmp   rax
.Lx362_4:
                                                                                        jmp   n352_match_alternate_s1
.Lx362_5:
                                                                                        jmp   n352_match_alternate_af
.Lx362_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S8]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx362_2:
                        test             rax, rax
                                                                                        je    .Lx362_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx362_7]
                        lea              rdx, [rip + .Lx362_8]
                                                                                        jmp   rax
.Lx362_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx362_2
.Lx362_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx362_2
.Lx362_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n352_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx362_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n352_match_alternate_s1
.Lx362_6:
                        add              rsp, 16
                                                                                        jmp   n352_match_alternate_af
n353_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n354_match_sequence_α:
                        mov              dword ptr [rbp + 48], r14d
                                                                                        jmp   n359_match_defer_α
n354_match_sequence_as:
                                                                                        jmp   n352_match_alternate_s0
n354_match_sequence_β:
                                                                                        jmp   n357_match_assign_cond_β
n354_match_sequence_af:
                                                                                        jmp   n352_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n355_match_assign_save_α:
                        sub              rsp, 32
                        mov              dword ptr [rsp + 0], r14d
                        add              rsp, 32
                                                                                        jmp   n356_match_defer_α
n355_match_assign_save_β:
                        sub              rsp, 32
                        add              rsp, 32
                                                                                        jmp   n358_match_patref_β
#-----------------------------------------------------------------------------------------------------------------------
n356_match_defer_α:
                        mov              rax, qword ptr [1879052592]
                        mov              rdx, qword ptr [1879052600]
                        cmp              eax, 3
                                                                                        jne   .Lx367_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx367_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx367_10
.Lx367_9:
                        xor              eax, eax
.Lx367_10:
                        test             rax, rax
                                                                                        jz    .Lx367_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx367_4]
                        lea              rdx, [rip + .Lx367_5]
                                                                                        jmp   rax
.Lx367_4:
                                                                                        jmp   n357_match_assign_cond_α
.Lx367_5:
                                                                                        jmp   n355_match_assign_save_β
.Lx367_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S11]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx367_2:
                        test             rax, rax
                                                                                        je    .Lx367_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx367_7]
                        lea              rdx, [rip + .Lx367_8]
                                                                                        jmp   rax
.Lx367_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx367_2
.Lx367_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx367_2
.Lx367_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n355_match_assign_save_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx367_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n357_match_assign_cond_α
.Lx367_6:
                        add              rsp, 16
                                                                                        jmp   n355_match_assign_save_β
n356_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n357_match_assign_cond_α:
                        lea              rdi, [rbp + 128]
                        call             rt_cap_top@PLT
                        lea              rcx, [rip + .S12]
                        mov              r10, qword ptr [1879048192]
                        mov              qword ptr [r10 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r10 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r10 + 16], rdx
                        add              r10, 24
                        mov              qword ptr [1879048192], r10
                                                                                        jmp   n352_match_alternate_s0
n357_match_assign_cond_β:
                        mov              rax, qword ptr [1879048192]
                        sub              rax, 24
                        mov              qword ptr [1879048192], rax
                                                                                        jmp   n356_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n358_match_patref_α:
                        mov              rax, qword ptr [1879052528]
                        mov              rdx, qword ptr [1879052536]
                        cmp              eax, 3
                                                                                        jne   .Lx370_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx370_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx370_10
.Lx370_9:
                        xor              eax, eax
.Lx370_10:
                        test             rax, rax
                                                                                        jz    .Lx370_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx370_4]
                        lea              rdx, [rip + .Lx370_5]
                                                                                        jmp   rax
.Lx370_4:
                                                                                        jmp   n355_match_assign_save_α
.Lx370_5:
                                                                                        jmp   n359_match_defer_β
.Lx370_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S13]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx370_2:
                        test             rax, rax
                                                                                        je    .Lx370_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx370_7]
                        lea              rdx, [rip + .Lx370_8]
                                                                                        jmp   rax
.Lx370_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx370_2
.Lx370_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx370_2
.Lx370_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n359_match_defer_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx370_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n355_match_assign_save_α
.Lx370_6:
                        add              rsp, 16
                                                                                        jmp   n359_match_defer_β
n358_match_patref_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n359_match_defer_α:
                        mov              rax, qword ptr [1879052576]
                        mov              rdx, qword ptr [1879052584]
                        cmp              eax, 3
                                                                                        jne   .Lx371_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx371_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx371_10
.Lx371_9:
                        xor              eax, eax
.Lx371_10:
                        test             rax, rax
                                                                                        jz    .Lx371_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx371_4]
                        lea              rdx, [rip + .Lx371_5]
                                                                                        jmp   rax
.Lx371_4:
                                                                                        jmp   n358_match_patref_α
.Lx371_5:
                                                                                        jmp   n352_match_alternate_af
.Lx371_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S8]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx371_2:
                        test             rax, rax
                                                                                        je    .Lx371_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx371_7]
                        lea              rdx, [rip + .Lx371_8]
                                                                                        jmp   rax
.Lx371_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx371_2
.Lx371_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx371_2
.Lx371_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n352_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx371_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n358_match_patref_α
.Lx371_6:
                        add              rsp, 16
                                                                                        jmp   n352_match_alternate_af
n359_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
proc_PAT$8_scanhit:
                        cmp              qword ptr [rbp + 16], 1
                                                                                        jne   7f
                        mov              ecx, dword ptr [rbp + 8]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx], ecx
7:
                                                                                        jmp   proc_PAT$8_γ
proc_PAT$8_scanfail:
                        cmp              qword ptr [rbp + 16], 1
                                                                                        jne   8f
                        mov              eax, dword ptr [rbp + 8]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    8f
                        lea              rcx, [rip + g_anchor]
                        cmp              qword ptr [rcx], 0
                                                                                        jne   8f
                        mov              dword ptr [rbp + 8], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$8_attempt
8:
                                                                                        jmp   proc_PAT$8_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$8_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$8_β:
                                                                                        jmp   qword ptr [rbp + 176]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$8_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$8_res]
                        push             rax
                        mov              rax, [rbp + 24]
                        mov              rbp, [rbp + 40]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$8_ω:
                        mov              rax, [rbp + 32]
                        lea              rsp, [rbp + 48]
                        mov              rbp, [rbp + 40]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$9_α
proc_PAT$9_α:
                        .global          proc_PAT$9_α
                        .global          proc_PAT$9_β
                        .global          proc_PAT$9_γ
                        .global          proc_PAT$9_ω
                        sub              rsp, 48
                        mov              [rsp + 24], rcx
                        mov              [rsp + 32], rdx
                        mov              [rsp + 40], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 16], r8
                        mov              dword ptr [rsp + 8], r14d
proc_PAT$9_attempt:
proc_PAT$9_α_body:
                        lea              rax, [rip + n372_match_alternate_β]
                        mov              qword ptr [rbp + 176], rax
#-----------------------------------------------------------------------------------------------------------------------
n372_match_alternate_α:
                        sub              rsp, 32
                        mov              dword ptr [rbp + 16], r14d
                        lea              rax, [rip + .Lx381_21]
                        mov              qword ptr [rbp + 32], rax
                                                                                        jmp   n374_match_sequence_α
.Lx381_21:
                        lea              rax, [rip + .Lx381_19]
                        mov              qword ptr [rbp + 32], rax
                                                                                        jmp   n373_match_defer_α
n372_match_alternate_s0:
                        lea              rax, [rip + .Lx381_40]
                        mov              qword ptr [rbp + 24], rax
                                                                                        jmp   n372_match_alternate_as
n372_match_alternate_s1:
                        lea              rax, [rip + .Lx381_41]
                        mov              qword ptr [rbp + 24], rax
                                                                                        jmp   n372_match_alternate_as
.Lx381_40:
                                                                                        jmp   n374_match_sequence_β
.Lx381_41:
                                                                                        jmp   n373_match_defer_β
n372_match_alternate_as:
                        add              rsp, 32
                                                                                        jmp   proc_PAT$9_γ
n372_match_alternate_β:
                        sub              rsp, 32
                        mov              rax, qword ptr [rbp + 24]
                                                                                        jmp   rax
n372_match_alternate_af:
                        mov              r14d, dword ptr [rbp + 16]
                        mov              rax, qword ptr [rbp + 32]
                                                                                        jmp   rax
.Lx381_19:
                        add              rsp, 32
                                                                                        jmp   proc_PAT$9_ω
#-----------------------------------------------------------------------------------------------------------------------
n373_match_defer_α:
                        mov              rax, qword ptr [1879052592]
                        mov              rdx, qword ptr [1879052600]
                        cmp              eax, 3
                                                                                        jne   .Lx382_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx382_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx382_10
.Lx382_9:
                        xor              eax, eax
.Lx382_10:
                        test             rax, rax
                                                                                        jz    .Lx382_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx382_4]
                        lea              rdx, [rip + .Lx382_5]
                                                                                        jmp   rax
.Lx382_4:
                                                                                        jmp   n372_match_alternate_s1
.Lx382_5:
                                                                                        jmp   n372_match_alternate_af
.Lx382_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S11]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx382_2:
                        test             rax, rax
                                                                                        je    .Lx382_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx382_7]
                        lea              rdx, [rip + .Lx382_8]
                                                                                        jmp   rax
.Lx382_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx382_2
.Lx382_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx382_2
.Lx382_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n372_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx382_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n372_match_alternate_s1
.Lx382_6:
                        add              rsp, 16
                                                                                        jmp   n372_match_alternate_af
n373_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n374_match_sequence_α:
                        mov              dword ptr [rbp + 48], r14d
                                                                                        jmp   n379_match_defer_α
n374_match_sequence_as:
                                                                                        jmp   n372_match_alternate_s0
n374_match_sequence_β:
                                                                                        jmp   n377_match_assign_cond_β
n374_match_sequence_af:
                                                                                        jmp   n372_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n375_match_assign_save_α:
                        sub              rsp, 32
                        mov              dword ptr [rsp + 0], r14d
                        add              rsp, 32
                                                                                        jmp   n376_match_defer_α
n375_match_assign_save_β:
                        sub              rsp, 32
                        add              rsp, 32
                                                                                        jmp   n378_match_patref_β
#-----------------------------------------------------------------------------------------------------------------------
n376_match_defer_α:
                        mov              rax, qword ptr [1879052608]
                        mov              rdx, qword ptr [1879052616]
                        cmp              eax, 3
                                                                                        jne   .Lx387_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx387_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx387_10
.Lx387_9:
                        xor              eax, eax
.Lx387_10:
                        test             rax, rax
                                                                                        jz    .Lx387_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx387_4]
                        lea              rdx, [rip + .Lx387_5]
                                                                                        jmp   rax
.Lx387_4:
                                                                                        jmp   n377_match_assign_cond_α
.Lx387_5:
                                                                                        jmp   n375_match_assign_save_β
.Lx387_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S5]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx387_2:
                        test             rax, rax
                                                                                        je    .Lx387_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx387_7]
                        lea              rdx, [rip + .Lx387_8]
                                                                                        jmp   rax
.Lx387_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx387_2
.Lx387_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx387_2
.Lx387_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n375_match_assign_save_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx387_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n377_match_assign_cond_α
.Lx387_6:
                        add              rsp, 16
                                                                                        jmp   n375_match_assign_save_β
n376_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n377_match_assign_cond_α:
                        lea              rdi, [rbp + 128]
                        call             rt_cap_top@PLT
                        lea              rcx, [rip + .S12]
                        mov              r10, qword ptr [1879048192]
                        mov              qword ptr [r10 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r10 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r10 + 16], rdx
                        add              r10, 24
                        mov              qword ptr [1879048192], r10
                                                                                        jmp   n372_match_alternate_s0
n377_match_assign_cond_β:
                        mov              rax, qword ptr [1879048192]
                        sub              rax, 24
                        mov              qword ptr [1879048192], rax
                                                                                        jmp   n376_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n378_match_patref_α:
                        mov              rax, qword ptr [1879052512]
                        mov              rdx, qword ptr [1879052520]
                        cmp              eax, 3
                                                                                        jne   .Lx390_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx390_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx390_10
.Lx390_9:
                        xor              eax, eax
.Lx390_10:
                        test             rax, rax
                                                                                        jz    .Lx390_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx390_4]
                        lea              rdx, [rip + .Lx390_5]
                                                                                        jmp   rax
.Lx390_4:
                                                                                        jmp   n375_match_assign_save_α
.Lx390_5:
                                                                                        jmp   n379_match_defer_β
.Lx390_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S10]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx390_2:
                        test             rax, rax
                                                                                        je    .Lx390_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx390_7]
                        lea              rdx, [rip + .Lx390_8]
                                                                                        jmp   rax
.Lx390_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx390_2
.Lx390_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx390_2
.Lx390_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n379_match_defer_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx390_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n375_match_assign_save_α
.Lx390_6:
                        add              rsp, 16
                                                                                        jmp   n379_match_defer_β
n378_match_patref_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n379_match_defer_α:
                        mov              rax, qword ptr [1879052592]
                        mov              rdx, qword ptr [1879052600]
                        cmp              eax, 3
                                                                                        jne   .Lx391_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx391_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx391_10
.Lx391_9:
                        xor              eax, eax
.Lx391_10:
                        test             rax, rax
                                                                                        jz    .Lx391_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx391_4]
                        lea              rdx, [rip + .Lx391_5]
                                                                                        jmp   rax
.Lx391_4:
                                                                                        jmp   n378_match_patref_α
.Lx391_5:
                                                                                        jmp   n372_match_alternate_af
.Lx391_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S11]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx391_2:
                        test             rax, rax
                                                                                        je    .Lx391_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx391_7]
                        lea              rdx, [rip + .Lx391_8]
                                                                                        jmp   rax
.Lx391_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx391_2
.Lx391_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx391_2
.Lx391_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n372_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx391_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n378_match_patref_α
.Lx391_6:
                        add              rsp, 16
                                                                                        jmp   n372_match_alternate_af
n379_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
proc_PAT$9_scanhit:
                        cmp              qword ptr [rbp + 16], 1
                                                                                        jne   7f
                        mov              ecx, dword ptr [rbp + 8]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx], ecx
7:
                                                                                        jmp   proc_PAT$9_γ
proc_PAT$9_scanfail:
                        cmp              qword ptr [rbp + 16], 1
                                                                                        jne   8f
                        mov              eax, dword ptr [rbp + 8]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    8f
                        lea              rcx, [rip + g_anchor]
                        cmp              qword ptr [rcx], 0
                                                                                        jne   8f
                        mov              dword ptr [rbp + 8], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$9_attempt
8:
                                                                                        jmp   proc_PAT$9_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$9_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$9_β:
                                                                                        jmp   qword ptr [rbp + 176]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$9_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$9_res]
                        push             rax
                        mov              rax, [rbp + 24]
                        mov              rbp, [rbp + 40]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$9_ω:
                        mov              rax, [rbp + 32]
                        lea              rsp, [rbp + 48]
                        mov              rbp, [rbp + 40]
                                                                                        jmp   rax
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lstartup_pname0:       .string          "LBL__Push"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_LBL__Push_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 2544
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname1:       .string          "LBL__Pop"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname1]
                        lea              rsi, [rip + proc_LBL__Pop_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 2544
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname2:       .string          "LBL__Unary"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname2]
                        lea              rsi, [rip + proc_LBL__Unary_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 2544
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname3:       .string          "LBL__Binary"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname3]
                        lea              rsi, [rip + proc_LBL__Binary_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 2544
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname4:       .string          "Push"
.Lstartup_pp4_0:        .string          "x"
                        .align           8
.Lstartup_pnames4:
                        .quad            .Lstartup_pp4_0
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname4]
                        lea              rsi, [rip + .Lstartup_pnames4]
                        mov              edx, 1
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname4]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname4]
                        lea              rsi, [rip + proc_Push_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname4]
                        mov              esi, 1
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname4]
                        mov              esi, 16
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname4]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname5:       .string          "Pop"
                        .align           8
.Lstartup_pnames5:
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname5]
                        lea              rsi, [rip + .Lstartup_pnames5]
                        mov              edx, 0
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        lea              rsi, [rip + proc_Pop_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 16
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname6:       .string          "Unary"
.Lstartup_pp6_0:        .string          "arg"
.Lstartup_pp6_1:        .string          "op"
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
                        lea              rsi, [rip + proc_Unary_α]
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
.Lstartup_pname7:       .string          "Binary"
.Lstartup_pp7_0:        .string          "op"
.Lstartup_pp7_1:        .string          "left"
.Lstartup_pp7_2:        .string          "right"
                        .align           8
.Lstartup_pnames7:
                        .quad            .Lstartup_pp7_0
                        .quad            .Lstartup_pp7_1
                        .quad            .Lstartup_pp7_2
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname7]
                        lea              rsi, [rip + .Lstartup_pnames7]
                        mov              edx, 3
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname7]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname7]
                        lea              rsi, [rip + proc_Binary_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname7]
                        mov              esi, 3
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname7]
                        mov              esi, 16
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname7]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname8:       .string          "PAT$0"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname8]
                        lea              rsi, [rip + proc_PAT$0_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname8]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname8]
                        mov              esi, 32
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname8]
                        mov              esi, 1
                        call             rt_proc_set_zstatic@PLT
                        lea              rdi, [rip + .Lstartup_pname8]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname9:       .string          "PAT$1"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname9]
                        lea              rsi, [rip + proc_PAT$1_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname9]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname9]
                        mov              esi, 128
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname9]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname10:      .string          "PAT$2"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname10]
                        lea              rsi, [rip + proc_PAT$2_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname10]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname10]
                        mov              esi, 288
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname10]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname11:      .string          "PAT$3"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname11]
                        lea              rsi, [rip + proc_PAT$3_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname11]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname11]
                        mov              esi, 96
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname11]
                        mov              esi, 1
                        call             rt_proc_set_zstatic@PLT
                        lea              rdi, [rip + .Lstartup_pname11]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname12:      .string          "PAT$4"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname12]
                        lea              rsi, [rip + proc_PAT$4_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname12]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname12]
                        mov              esi, 96
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname12]
                        mov              esi, 1
                        call             rt_proc_set_zstatic@PLT
                        lea              rdi, [rip + .Lstartup_pname12]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname13:      .string          "PAT$5"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname13]
                        lea              rsi, [rip + proc_PAT$5_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname13]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname13]
                        mov              esi, 144
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname13]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname14:      .string          "PAT$6"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname14]
                        lea              rsi, [rip + proc_PAT$6_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname14]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname14]
                        mov              esi, 128
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname14]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname15:      .string          "PAT$7"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname15]
                        lea              rsi, [rip + proc_PAT$7_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname15]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname15]
                        mov              esi, 176
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname15]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname16:      .string          "PAT$8"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname16]
                        lea              rsi, [rip + proc_PAT$8_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname16]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname16]
                        mov              esi, 192
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname16]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname17:      .string          "PAT$9"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname17]
                        lea              rsi, [rip + proc_PAT$9_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname17]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname17]
                        mov              esi, 192
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname17]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        add              rsp, 8
                        ret
                        .section         .rodata
.Lgvan0:                .string          "Push"
.Lgvan1:                .string          "x"
.Lgvan2:                .string          "Pop"
.Lgvan3:                .string          "Unary"
.Lgvan4:                .string          "arg"
.Lgvan5:                .string          "op"
.Lgvan6:                .string          "Binary"
.Lgvan7:                .string          "left"
.Lgvan8:                .string          "right"
.Lgvan9:                .string          "stk"
.Lgvan10:               .string          "dummy"
.Lgvan11:               .string          "integer"
.Lgvan12:               .string          "exponent"
.Lgvan13:               .string          "real"
.Lgvan14:               .string          "addop"
.Lgvan15:               .string          "mulop"
.Lgvan16:               .string          "constant"
.Lgvan17:               .string          "primary"
.Lgvan18:               .string          "factor"
.Lgvan19:               .string          "term"
.Lgvan20:               .string          "expr"
.Lgvan21:               .string          "line"
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
                        .quad            .Lgvan11
                        .quad            .Lgvan12
                        .quad            .Lgvan13
                        .quad            .Lgvan14
                        .quad            .Lgvan15
                        .quad            .Lgvan16
                        .quad            .Lgvan17
                        .quad            .Lgvan18
                        .quad            .Lgvan19
                        .quad            .Lgvan20
                        .quad            .Lgvan21
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             proc_startup
                        mov              edi, 22
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 22
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
                        sub              rsp, 2552
                        mov              rdi, rsp
                        mov              ecx, 2552
                        xor              eax, eax
                        rep stosb
                        mov              [rsp + 2544], rbp
                        mov              rbp, rsp
main_α_body:
#=======================================================================================================================
#          DEFINE('Push(x)')
#          stk      =  TABLE()                       :(PushEnd)
#-----------------------------------------------------------------------------------------------------------------------
n392_call_α:
                        sub              rsp, 16
                        .section         .rodata
.Lrkfnzd534:            .string          "TABLE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd534]
                        xor              esi, esi
                        mov              edx, 0
                        call             rt_call_arr@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx533_240
                        add              rsp, 16
                                                                                        jmp   n394_lit_string_α
.Lx533_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n393_assign_α
n392_call_β:
                        add              rsp, 16
                                                                                        jmp   n394_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n393_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052432], rax
                        mov              qword ptr [1879052440], rdx
                        add              rsp, 16
                                                                                        jmp   n394_lit_string_α
#=======================================================================================================================
#          DEFINE('Pop()')                           :(PopEnd)
#          DEFINE('Unary()arg,op')                   :(UnaryEnd)
#          DEFINE('Binary()op,left,right')           :(BinaryEnd)
#          integer  =  SPAN('0123456789')
#-----------------------------------------------------------------------------------------------------------------------
n394_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx536_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n395_call_α
.Lx536_0:
                        .quad            .Lx536_0_s
.Lx536_0_s:
                        .string          "PAT$0"
#-----------------------------------------------------------------------------------------------------------------------
n395_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd538:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd538]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 99
                                                                                        jne   .Lx537_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n397_lit_string_α
.Lx537_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n396_assign_α
n395_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n397_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n396_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052464], rax
                        mov              qword ptr [1879052472], rdx
                        add              rsp, 32
                                                                                        jmp   n397_lit_string_α
#=======================================================================================================================
#          exponent =  ANY('eEdD') (ANY('+-') | epsilon) integer
#-----------------------------------------------------------------------------------------------------------------------
n397_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx540_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n398_call_α
.Lx540_0:
                        .quad            .Lx540_0_s
.Lx540_0_s:
                        .string          "PAT$1"
#-----------------------------------------------------------------------------------------------------------------------
n398_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd542:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd542]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 99
                                                                                        jne   .Lx541_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n400_lit_string_α
.Lx541_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n399_assign_α
n398_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n400_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n399_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052480], rax
                        mov              qword ptr [1879052488], rdx
                        add              rsp, 32
                                                                                        jmp   n400_lit_string_α
#=======================================================================================================================
#          real     =  integer '.' (integer | epsilon) (exponent | epsilon)
#-----------------------------------------------------------------------------------------------------------------------
n400_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx544_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n401_call_α
.Lx544_0:
                        .quad            .Lx544_0_s
.Lx544_0_s:
                        .string          "PAT$2"
#-----------------------------------------------------------------------------------------------------------------------
n401_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd546:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd546]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 99
                                                                                        jne   .Lx545_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n403_lit_string_α
.Lx545_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n402_assign_α
n401_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n403_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n402_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052496], rax
                        mov              qword ptr [1879052504], rdx
                        add              rsp, 32
                                                                                        jmp   n403_lit_string_α
#=======================================================================================================================
#          real     =  integer '.' (integer | epsilon) (exponent | epsilon)
#          addop    =  ANY('+-') . *Push()
#-----------------------------------------------------------------------------------------------------------------------
n403_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx548_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n404_call_α
.Lx548_0:
                        .quad            .Lx548_0_s
.Lx548_0_s:
                        .string          "PAT$3"
#-----------------------------------------------------------------------------------------------------------------------
n404_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd550:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd550]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 99
                                                                                        jne   .Lx549_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n406_lit_string_α
.Lx549_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n405_assign_α
n404_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n406_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n405_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052512], rax
                        mov              qword ptr [1879052520], rdx
                        add              rsp, 32
                                                                                        jmp   n406_lit_string_α
#=======================================================================================================================
#          mulop    =  ANY('*/') . *Push()
#-----------------------------------------------------------------------------------------------------------------------
n406_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx552_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n407_call_α
.Lx552_0:
                        .quad            .Lx552_0_s
.Lx552_0_s:
                        .string          "PAT$4"
#-----------------------------------------------------------------------------------------------------------------------
n407_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd554:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd554]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 99
                                                                                        jne   .Lx553_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n409_lit_string_α
.Lx553_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n408_assign_α
n407_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n409_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n408_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052528], rax
                        mov              qword ptr [1879052536], rdx
                        add              rsp, 32
                                                                                        jmp   n409_lit_string_α
#=======================================================================================================================
#          constant =  (real | integer) . *Push()
#-----------------------------------------------------------------------------------------------------------------------
n409_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx556_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n410_call_α
.Lx556_0:
                        .quad            .Lx556_0_s
.Lx556_0_s:
                        .string          "PAT$5"
#-----------------------------------------------------------------------------------------------------------------------
n410_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd558:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd558]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 99
                                                                                        jne   .Lx557_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n412_lit_string_α
.Lx557_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n411_assign_α
n410_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n412_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n411_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052544], rax
                        mov              qword ptr [1879052552], rdx
                        add              rsp, 32
                                                                                        jmp   n412_lit_string_α
#=======================================================================================================================
#          constant =  (real | integer) . *Push()
#          primary  =  constant | '(' *expr ')'
#-----------------------------------------------------------------------------------------------------------------------
n412_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx560_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n413_call_α
.Lx560_0:
                        .quad            .Lx560_0_s
.Lx560_0_s:
                        .string          "PAT$6"
#-----------------------------------------------------------------------------------------------------------------------
n413_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd562:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd562]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 99
                                                                                        jne   .Lx561_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n415_lit_string_α
.Lx561_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n414_assign_α
n413_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n415_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n414_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052560], rax
                        mov              qword ptr [1879052568], rdx
                        add              rsp, 32
                                                                                        jmp   n415_lit_string_α
#=======================================================================================================================
#          primary  =  constant | '(' *expr ')'
#          factor   =  addop *factor . *Unary()
#-----------------------------------------------------------------------------------------------------------------------
n415_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx564_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n416_call_α
.Lx564_0:
                        .quad            .Lx564_0_s
.Lx564_0_s:
                        .string          "PAT$7"
#-----------------------------------------------------------------------------------------------------------------------
n416_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd566:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd566]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 99
                                                                                        jne   .Lx565_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n418_lit_string_α
.Lx565_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n417_assign_α
n416_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n418_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n417_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052576], rax
                        mov              qword ptr [1879052584], rdx
                        add              rsp, 32
                                                                                        jmp   n418_lit_string_α
#=======================================================================================================================
#          factor   =  addop *factor . *Unary()
#          term     =  *factor mulop *term . *Binary()
#-----------------------------------------------------------------------------------------------------------------------
n418_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx568_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n419_call_α
.Lx568_0:
                        .quad            .Lx568_0_s
.Lx568_0_s:
                        .string          "PAT$8"
#-----------------------------------------------------------------------------------------------------------------------
n419_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd570:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd570]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 99
                                                                                        jne   .Lx569_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n421_lit_string_α
.Lx569_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n420_assign_α
n419_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n421_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n420_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052592], rax
                        mov              qword ptr [1879052600], rdx
                        add              rsp, 32
                                                                                        jmp   n421_lit_string_α
#=======================================================================================================================
#          term     =  *factor mulop *term . *Binary()
#          expr     =  *term addop *expr . *Binary()
#-----------------------------------------------------------------------------------------------------------------------
n421_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx572_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n422_call_α
.Lx572_0:
                        .quad            .Lx572_0_s
.Lx572_0_s:
                        .string          "PAT$9"
#-----------------------------------------------------------------------------------------------------------------------
n422_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd574:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd574]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 99
                                                                                        jne   .Lx573_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n424_lit_string_α
.Lx573_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n423_assign_α
n422_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n424_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n423_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052608], rax
                        mov              qword ptr [1879052616], rdx
                        add              rsp, 32
                                                                                        jmp   n424_lit_string_α
#=======================================================================================================================
#          expr     =  *term addop *expr . *Binary()
#          &TRIM    =  1
#-----------------------------------------------------------------------------------------------------------------------
n424_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx576_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n425_lit_integer_α
.Lx576_0:
                        .quad            .Lx576_0_s
.Lx576_0_s:
                        .string          "TRIM"
#-----------------------------------------------------------------------------------------------------------------------
n425_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx577_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n426_call_α
.Lx577_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n426_call_α:
                        sub              rsp, 16
                        sub              rsp, 32
                        mov              r10, qword ptr [rsp + 64]
                        mov              r11, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        mov              r10, qword ptr [rsp + 48]
                        mov              r11, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        .section         .rodata
.Lrkfnzd579:            .string          "SNO$KWSET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd579]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 99
                                                                                        jne   .Lx578_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n427_var_α
.Lx578_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        add              rsp, 48
                                                                                        jmp   n427_var_α
n426_call_β:
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n427_var_α
#=======================================================================================================================
# loop     line     =  INPUT                         :F(END)
#-----------------------------------------------------------------------------------------------------------------------
n427_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx580_0]
                        call             NV_GET_fn@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx580_240
                        add              rsp, 16
                                                                                        jmp   main_γ
.Lx580_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n428_assign_α
.Lx580_0:
                        .quad            .Lx580_0_s
.Lx580_0_s:
                        .string          "INPUT"
#-----------------------------------------------------------------------------------------------------------------------
n428_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052624], rax
                        mov              qword ptr [1879052632], rdx
                        add              rsp, 16
                                                                                        jmp   n429_var_α
#=======================================================================================================================
#          line     POS(0) expr RPOS(0)              :F(error)
#-----------------------------------------------------------------------------------------------------------------------
n429_var_α:
                        mov              rax, qword ptr [1879052624]
                        mov              rdx, qword ptr [1879052632]
                        mov              qword ptr [rsp + 2480], rax
                        mov              qword ptr [rsp + 2488], rdx
                                                                                        jmp   n430_match_head_α
#-----------------------------------------------------------------------------------------------------------------------
n430_match_head_α:
                        mov              qword ptr [rbp + 2384], r13
                        mov              qword ptr [rbp + 2392], r14
                        mov              qword ptr [rbp + 2400], r15
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rbp + 2408], rax
                        mov              qword ptr [rbp + 2376], rbp
                        mov              rdi, qword ptr [rbp + 2480]
                        mov              rsi, qword ptr [rbp + 2488]
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r10, qword ptr [1879048192]
                        mov              qword ptr [r10 + 0], 0
                        add              r10, 24
                        mov              qword ptr [1879048192], r10
                        mov              qword ptr [rbp + 2352], rsp
                        lea              rcx, [rip + g_patstk_sp]
                        mov              rax, qword ptr [rcx + 0]
                        mov              qword ptr [rbp + 2344], rax
                        mov              dword ptr [rbp + 2336], 0
.Lx584_0:
                        mov              r14d, dword ptr [rbp + 2336]
                                                                                        jmp   n431_match_sequence_α
n430_match_head_β:
                        add              dword ptr [rbp + 2336], 1
                        mov              eax, dword ptr [rbp + 2336]
                        cmp              eax, r15d
                                                                                        jg    .Lx584_1
                        lea              rcx, [rip + g_anchor]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx584_1
                                                                                        jmp   .Lx584_0
.Lx584_1:
                        mov              rax, qword ptr [rbp + 2344]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rbp + 2352]
                        mov              r10, qword ptr [1879048192]
.Lx584_2:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx584_2
                        mov              qword ptr [1879048192], r10
                        mov              r13, qword ptr [rbp + 2384]
                        mov              r14, qword ptr [rbp + 2392]
                        mov              r15, qword ptr [rbp + 2400]
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rbp + 2408]
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + 2376]
                                                                                        jmp   n440_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n431_match_sequence_α:
                        mov              dword ptr [rbp + 2416], r14d
                                                                                        jmp   n438_lit_integer_α
n431_match_sequence_as:
                                                                                        jmp   n432_match_release_α
n431_match_sequence_β:
                                                                                        jmp   n436_match_rpos_β
n431_match_sequence_af:
                                                                                        jmp   n430_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n432_match_release_α:
                        mov              rax, qword ptr [rbp + 2344]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rbp + 2352]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, qword ptr [1879048192]
                        mov              r10, rsi
.Lx588_5:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx588_5
                        lea              rdi, [r10 + 24]
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx588_1:
                        test             rax, rax
                                                                                        je    .Lx588_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx588_3]
                        lea              rdx, [rip + .Lx588_4]
                                                                                        jmp   rax
.Lx588_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx588_1
.Lx588_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx588_1
.Lx588_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r10, qword ptr [1879048192]
.Lx588_6:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx588_6
                        mov              qword ptr [1879048192], r10
                        mov              r13, qword ptr [rbp + 2384]
                        mov              r14, qword ptr [rbp + 2392]
                        mov              r15, qword ptr [rbp + 2400]
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rbp + 2408]
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + 2376]
                                                                                        jmp   n433_call_α
#=======================================================================================================================
#          OUTPUT   =  Pop()                         :(loop)
#-----------------------------------------------------------------------------------------------------------------------
n433_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx590_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx590_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx590_6]
                        lea              rdx, [rip + .Lx590_7]
                                                                                        jmp   rax
.Lx590_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx590_2
.Lx590_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx590_2
.Lx590_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx590_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx590_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx590_3]
                        lea              rdx, [rip + .Lx590_4]
                                                                                        jmp   rax
.Lx590_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx590_2
.Lx590_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx590_2
.Lx590_1:
                        call             rt_faildescr@PLT
.Lx590_2:
                        mov              qword ptr [rsp + 2496], rax
                        mov              qword ptr [rsp + 2504], rdx
                        cmp              eax, 99
                                                                                        je    n427_var_α
                                                                                        jmp   n434_assign_α
n433_call_β:
                                                                                        jmp   n427_var_α
.Lx590_0:
                        .quad            .Lx590_0_s
.Lx590_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n434_assign_α:
                        mov              rsi, qword ptr [rsp + 2496]
                        mov              rdx, qword ptr [rsp + 2504]
                        mov              rdi, qword ptr [rip + .Lx591_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n427_var_α
.Lx591_0:
                        .quad            .Lx591_0_s
.Lx591_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n435_lit_integer_α:
                        mov              qword ptr [rsp + 2464], 6
                        mov              rax, qword ptr [rip + .Lx592_0]
                        mov              qword ptr [rsp + 2472], rax
                                                                                        jmp   n436_match_rpos_α
n435_lit_integer_β:
                                                                                        jmp   n437_match_patref_β
.Lx592_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n436_match_rpos_α:
                        mov              rax, 0
                        mov              ecx, r15d
                        sub              ecx, eax
                        cmp              r14d, ecx
                                                                                        jne   n437_match_patref_β
                                                                                        jmp   n432_match_release_α
n436_match_rpos_β:
                                                                                        jmp   n437_match_patref_β
#-----------------------------------------------------------------------------------------------------------------------
n437_match_patref_α:
                        mov              rax, qword ptr [1879052608]
                        mov              rdx, qword ptr [1879052616]
                        cmp              eax, 3
                                                                                        jne   .Lx594_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx594_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx594_10
.Lx594_9:
                        xor              eax, eax
.Lx594_10:
                        test             rax, rax
                                                                                        jz    .Lx594_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx594_4]
                        lea              rdx, [rip + .Lx594_5]
                                                                                        jmp   rax
.Lx594_4:
                                                                                        jmp   n435_lit_integer_α
.Lx594_5:
                                                                                        jmp   n430_match_head_β
.Lx594_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S5]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx594_2:
                        test             rax, rax
                                                                                        je    .Lx594_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx594_7]
                        lea              rdx, [rip + .Lx594_8]
                                                                                        jmp   rax
.Lx594_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx594_2
.Lx594_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx594_2
.Lx594_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n430_match_head_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx594_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n435_lit_integer_α
.Lx594_6:
                        add              rsp, 16
                                                                                        jmp   n430_match_head_β
n437_match_patref_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n438_lit_integer_α:
                        mov              qword ptr [rsp + 2432], 6
                        mov              rax, qword ptr [rip + .Lx595_0]
                        mov              qword ptr [rsp + 2440], rax
                                                                                        jmp   n439_match_pos_α
n438_lit_integer_β:
                                                                                        jmp   n430_match_head_β
.Lx595_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n439_match_pos_α:
                        mov              rax, 0
                        cmp              r14d, eax
                                                                                        jne   n430_match_head_β
                                                                                        jmp   n437_match_patref_α
n439_match_pos_β:
                                                                                        jmp   n430_match_head_β
#=======================================================================================================================
# error    OUTPUT   = 'Bad input, try again'         :(loop)
#-----------------------------------------------------------------------------------------------------------------------
n440_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 20
                        mov              rax, qword ptr [rip + .Lx597_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n441_assign_α
.Lx597_0:
                        .quad            .Lx597_0_s
.Lx597_0_s:
                        .string          "Bad input, try again"
#-----------------------------------------------------------------------------------------------------------------------
n441_assign_α:
                        mov              rsi, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx598_0]
                        call             NV_SET_fn@PLT
                        add              rsp, 16
                                                                                        jmp   n427_var_α
.Lx598_0:
                        .quad            .Lx598_0_s
.Lx598_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n442_goto_α:
                                                                                        jmp   n443_var_α
n442_goto_β:
                                                                                        jmp   main_ω
#=======================================================================================================================
# Push     stk[0]   =  stk[0] + 1
#-----------------------------------------------------------------------------------------------------------------------
n443_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n444_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n444_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx601_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n445_subscript_α
.Lx601_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n445_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx602_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n453_var_α
.Lx602_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n446_var_α
#-----------------------------------------------------------------------------------------------------------------------
n446_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n447_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n447_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx604_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n448_subscript_α
.Lx604_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n448_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx605_240
                        add              rsp, 16
                        add              rsp, 80
                                                                                        jmp   n453_var_α
.Lx605_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n449_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n449_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]
                        mov              rsi, qword ptr [rsp + 24]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx606_240
                        add              rsp, 16
                        add              rsp, 96
                                                                                        jmp   n453_var_α
.Lx606_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n450_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n450_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx607_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n451_binop_α
.Lx607_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n451_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx608_240
                        add              rsp, 16
                        add              rsp, 128
                                                                                        jmp   n453_var_α
.Lx608_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n452_assign_var_α
#-----------------------------------------------------------------------------------------------------------------------
n452_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 112]
                        mov              rsi, qword ptr [rsp + 120]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx609_240
                        add              rsp, 16
                        add              rsp, 144
                                                                                        jmp   n453_var_α
.Lx609_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        add              rsp, 160
                                                                                        jmp   n453_var_α
#=======================================================================================================================
#          Push     =  .stk[stk[0]]
#-----------------------------------------------------------------------------------------------------------------------
n453_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n454_var_α
#-----------------------------------------------------------------------------------------------------------------------
n454_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n455_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n455_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx612_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n456_subscript_α
.Lx612_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n456_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx613_240
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n460_var_α
.Lx613_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n457_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n457_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]
                        mov              rsi, qword ptr [rsp + 24]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx614_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n460_var_α
.Lx614_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n458_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n458_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 80]
                        mov              rsi, qword ptr [rsp + 88]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx615_240
                        add              rsp, 16
                        add              rsp, 80
                                                                                        jmp   n460_var_α
.Lx615_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n459_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n459_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 96
                                                                                        jmp   n460_var_α
#=======================================================================================================================
#          $Push    =  x                             :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n460_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 448], rax
                        mov              qword ptr [rsp + 456], rdx
                                                                                        jmp   n461_call_α
#-----------------------------------------------------------------------------------------------------------------------
n461_call_α:
                        mov              rax, qword ptr [rsp + 448]
                        mov              qword ptr [rsp + 416], rax
                        mov              rax, qword ptr [rsp + 456]
                        mov              qword ptr [rsp + 424], rax
                        .section         .rodata
.Lrkfn619:              .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn619]
                        lea              rsi, [rsp + 416]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 400], rax
                        mov              qword ptr [rsp + 408], rdx
                        cmp              eax, 99
                                                                                        je    n464_lit_string_α
                                                                                        jmp   n462_var_α
n461_call_β:
                                                                                        jmp   n464_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n462_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 464], rax
                        mov              qword ptr [rsp + 472], rdx
                                                                                        jmp   n463_assign_var_α
#-----------------------------------------------------------------------------------------------------------------------
n463_assign_var_α:
                        mov              rdi, qword ptr [rsp + 400]
                        mov              rsi, qword ptr [rsp + 408]
                        mov              rdx, qword ptr [rsp + 464]
                        mov              rcx, qword ptr [rsp + 472]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n464_lit_string_α
                        mov              qword ptr [rsp + 480], rax
                        mov              qword ptr [rsp + 488], rdx
                                                                                        jmp   n464_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n464_lit_string_α:
                        mov              qword ptr [rsp + 32], 1
                        mov              dword ptr [rsp + 36], 0
                        mov              rax, qword ptr [rip + .Lx622_0]
                        mov              qword ptr [rsp + 40], rax
                                                                                        jmp   n465_call_α
.Lx622_0:
                        .quad            .Lx622_0_s
.Lx622_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n465_call_α:
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 72], rax
                        .section         .rodata
.Lrkfn624:              .string          "SNO$NRET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn624]
                        lea              rsi, [rsp + 64]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 48], rax
                        mov              qword ptr [rsp + 56], rdx
                        cmp              eax, 99
                                                                                        je    n467_save_restore_α
                                                                                        jmp   n466_save_restore_α
n465_call_β:
                                                                                        jmp   n467_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n466_save_restore_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n467_save_restore_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n468_goto_α:
                                                                                        jmp   n394_lit_string_α
n468_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n469_goto_α:
                                                                                        jmp   n470_var_α
n469_goto_β:
                                                                                        jmp   main_ω
#=======================================================================================================================
# Pop      Pop      =  stk[stk[0]]
#-----------------------------------------------------------------------------------------------------------------------
n470_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n471_var_α
#-----------------------------------------------------------------------------------------------------------------------
n471_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n472_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n472_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx633_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n473_subscript_α
.Lx633_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n473_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx634_240
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n478_var_α
.Lx634_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n474_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n474_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]
                        mov              rsi, qword ptr [rsp + 24]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx635_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n478_var_α
.Lx635_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n475_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n475_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 80]
                        mov              rsi, qword ptr [rsp + 88]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx636_240
                        add              rsp, 16
                        add              rsp, 80
                                                                                        jmp   n478_var_α
.Lx636_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n476_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n476_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]
                        mov              rsi, qword ptr [rsp + 24]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx637_240
                        add              rsp, 16
                        add              rsp, 96
                                                                                        jmp   n478_var_α
.Lx637_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n477_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n477_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rdx
                        add              rsp, 112
                                                                                        jmp   n478_var_α
#=======================================================================================================================
#          stk[0]   =  stk[0] - 1                    :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n478_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n479_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n479_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx640_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n480_subscript_α
.Lx640_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n480_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx641_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n466_save_restore_α
.Lx641_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n481_var_α
#-----------------------------------------------------------------------------------------------------------------------
n481_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n482_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n482_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx643_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n483_subscript_α
.Lx643_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n483_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx644_240
                        add              rsp, 16
                        add              rsp, 80
                                                                                        jmp   n466_save_restore_α
.Lx644_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n484_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n484_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]
                        mov              rsi, qword ptr [rsp + 24]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx645_240
                        add              rsp, 16
                        add              rsp, 96
                                                                                        jmp   n466_save_restore_α
.Lx645_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n485_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n485_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx646_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n486_binop_α
.Lx646_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n486_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_sub@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx647_240
                        add              rsp, 16
                        add              rsp, 128
                                                                                        jmp   n466_save_restore_α
.Lx647_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n487_assign_var_α
#-----------------------------------------------------------------------------------------------------------------------
n487_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 112]
                        mov              rsi, qword ptr [rsp + 120]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx648_240
                        add              rsp, 16
                        add              rsp, 144
                                                                                        jmp   n466_save_restore_α
.Lx648_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        add              rsp, 160
                                                                                        jmp   n466_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n488_goto_α:
                                                                                        jmp   n394_lit_string_α
n488_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n489_goto_α:
                                                                                        jmp   n490_call_α
n489_goto_β:
                                                                                        jmp   main_ω
#=======================================================================================================================
# Unary    arg      =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n490_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx652_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx652_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx652_6]
                        lea              rdx, [rip + .Lx652_7]
                                                                                        jmp   rax
.Lx652_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx652_2
.Lx652_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx652_2
.Lx652_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx652_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx652_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx652_3]
                        lea              rdx, [rip + .Lx652_4]
                                                                                        jmp   rax
.Lx652_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx652_2
.Lx652_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx652_2
.Lx652_1:
                        call             rt_faildescr@PLT
.Lx652_2:
                        mov              qword ptr [rsp + 768], rax
                        mov              qword ptr [rsp + 776], rdx
                        cmp              eax, 99
                                                                                        je    n492_call_α
                                                                                        jmp   n491_assign_α
n490_call_β:
                                                                                        jmp   n492_call_α
.Lx652_0:
                        .quad            .Lx652_0_s
.Lx652_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n491_assign_α:
                        mov              rax, qword ptr [rsp + 768]
                        mov              rdx, qword ptr [rsp + 776]
                        mov              qword ptr [1879052352], rax
                        mov              qword ptr [1879052360], rdx
                                                                                        jmp   n492_call_α
#=======================================================================================================================
#          op       =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n492_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx655_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx655_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx655_6]
                        lea              rdx, [rip + .Lx655_7]
                                                                                        jmp   rax
.Lx655_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx655_2
.Lx655_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx655_2
.Lx655_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx655_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx655_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx655_3]
                        lea              rdx, [rip + .Lx655_4]
                                                                                        jmp   rax
.Lx655_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx655_2
.Lx655_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx655_2
.Lx655_1:
                        call             rt_faildescr@PLT
.Lx655_2:
                        mov              qword ptr [rsp + 800], rax
                        mov              qword ptr [rsp + 808], rdx
                        cmp              eax, 99
                                                                                        je    n494_lit_string_α
                                                                                        jmp   n493_assign_α
n492_call_β:
                                                                                        jmp   n494_lit_string_α
.Lx655_0:
                        .quad            .Lx655_0_s
.Lx655_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n493_assign_α:
                        mov              rax, qword ptr [rsp + 800]
                        mov              rdx, qword ptr [rsp + 808]
                        mov              qword ptr [1879052368], rax
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n494_lit_string_α
#=======================================================================================================================
#          Push()   =  EVAL(op arg)
#-----------------------------------------------------------------------------------------------------------------------
n494_lit_string_α:
                        mov              qword ptr [rsp + 832], 1
                        mov              dword ptr [rsp + 836], 0
                        mov              rax, qword ptr [rip + .Lx657_0]
                        mov              qword ptr [rsp + 840], rax
                                                                                        jmp   n495_call_α
.Lx657_0:
                        .quad            .Lx657_0_s
.Lx657_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n495_call_α:
                        mov              rax, qword ptr [rsp + 832]
                        mov              qword ptr [rsp + 864], rax
                        mov              rax, qword ptr [rsp + 840]
                        mov              qword ptr [rsp + 872], rax
                        .section         .rodata
.Lrkfn659:              .string          "SNO$WANTNM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn659]
                        lea              rsi, [rsp + 864]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 848], rax
                        mov              qword ptr [rsp + 856], rdx
                        cmp              eax, 99
                                                                                        je    n502_lit_string_α
                                                                                        jmp   n496_call_α
n495_call_β:
                                                                                        jmp   n502_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n496_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx661_0]
                        mov              esi, 1
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx661_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx661_6]
                        lea              rdx, [rip + .Lx661_7]
                                                                                        jmp   rax
.Lx661_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx661_2
.Lx661_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx661_2
.Lx661_5:
                        add              rsp, 32
                        mov              rdi, qword ptr [rip + .Lx661_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx661_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx661_3]
                        lea              rdx, [rip + .Lx661_4]
                                                                                        jmp   rax
.Lx661_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx661_2
.Lx661_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx661_2
.Lx661_1:
                        call             rt_faildescr@PLT
.Lx661_2:
                        mov              qword ptr [rsp + 896], rax
                        mov              qword ptr [rsp + 904], rdx
                        cmp              eax, 99
                                                                                        je    n502_lit_string_α
                                                                                        jmp   n497_var_α
n496_call_β:
                                                                                        jmp   n502_lit_string_α
.Lx661_0:
                        .quad            .Lx661_0_s
.Lx661_0_s:
                        .string          "Push"
#-----------------------------------------------------------------------------------------------------------------------
n497_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 992], rax
                        mov              qword ptr [rsp + 1000], rdx
                                                                                        jmp   n498_var_α
#-----------------------------------------------------------------------------------------------------------------------
n498_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 1008], rax
                        mov              qword ptr [rsp + 1016], rdx
                                                                                        jmp   n499_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n499_binop_α:
                        mov              rdi, qword ptr [rsp + 992]
                        mov              rsi, qword ptr [rsp + 1000]
                        mov              rdx, qword ptr [rsp + 1008]
                        mov              rcx, qword ptr [rsp + 1016]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 976], rax
                        mov              qword ptr [rsp + 984], rdx
                                                                                        jmp   n500_call_α
#-----------------------------------------------------------------------------------------------------------------------
n500_call_α:
                        mov              rax, qword ptr [rsp + 976]
                        mov              qword ptr [rsp + 944], rax
                        mov              rax, qword ptr [rsp + 984]
                        mov              qword ptr [rsp + 952], rax
                        .section         .rodata
.Lrkfn666:              .string          "EVAL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn666]
                        lea              rsi, [rsp + 944]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 928], rax
                        mov              qword ptr [rsp + 936], rdx
                        cmp              eax, 99
                                                                                        je    n502_lit_string_α
                                                                                        jmp   n501_assign_var_α
n500_call_β:
                                                                                        jmp   n502_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n501_assign_var_α:
                        mov              rdi, qword ptr [rsp + 896]
                        mov              rsi, qword ptr [rsp + 904]
                        mov              rdx, qword ptr [rsp + 928]
                        mov              rcx, qword ptr [rsp + 936]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n502_lit_string_α
                        mov              qword ptr [rsp + 1024], rax
                        mov              qword ptr [rsp + 1032], rdx
                                                                                        jmp   n502_lit_string_α
#=======================================================================================================================
#          Unary    =  .dummy                        :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n502_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx668_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n503_call_α
.Lx668_0:
                        .quad            .Lx668_0_s
.Lx668_0_s:
                        .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n503_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd670:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd670]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 99
                                                                                        jne   .Lx669_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n464_lit_string_α
.Lx669_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n504_assign_α
n503_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n464_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n504_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052336], rax
                        mov              qword ptr [1879052344], rdx
                        add              rsp, 32
                                                                                        jmp   n464_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n505_goto_α:
                                                                                        jmp   n394_lit_string_α
n505_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n506_goto_α:
                                                                                        jmp   n507_call_α
n506_goto_β:
                                                                                        jmp   main_ω
#=======================================================================================================================
# Binary   right    =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n507_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx675_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx675_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx675_6]
                        lea              rdx, [rip + .Lx675_7]
                                                                                        jmp   rax
.Lx675_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx675_2
.Lx675_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx675_2
.Lx675_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx675_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx675_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx675_3]
                        lea              rdx, [rip + .Lx675_4]
                                                                                        jmp   rax
.Lx675_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx675_2
.Lx675_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx675_2
.Lx675_1:
                        call             rt_faildescr@PLT
.Lx675_2:
                        mov              qword ptr [rsp + 1104], rax
                        mov              qword ptr [rsp + 1112], rdx
                        cmp              eax, 99
                                                                                        je    n509_call_α
                                                                                        jmp   n508_assign_α
n507_call_β:
                                                                                        jmp   n509_call_α
.Lx675_0:
                        .quad            .Lx675_0_s
.Lx675_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n508_assign_α:
                        mov              rax, qword ptr [rsp + 1104]
                        mov              rdx, qword ptr [rsp + 1112]
                        mov              qword ptr [1879052416], rax
                        mov              qword ptr [1879052424], rdx
                                                                                        jmp   n509_call_α
#=======================================================================================================================
#          op       =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n509_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx678_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx678_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx678_6]
                        lea              rdx, [rip + .Lx678_7]
                                                                                        jmp   rax
.Lx678_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx678_2
.Lx678_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx678_2
.Lx678_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx678_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx678_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx678_3]
                        lea              rdx, [rip + .Lx678_4]
                                                                                        jmp   rax
.Lx678_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx678_2
.Lx678_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx678_2
.Lx678_1:
                        call             rt_faildescr@PLT
.Lx678_2:
                        mov              qword ptr [rsp + 1136], rax
                        mov              qword ptr [rsp + 1144], rdx
                        cmp              eax, 99
                                                                                        je    n511_call_α
                                                                                        jmp   n510_assign_α
n509_call_β:
                                                                                        jmp   n511_call_α
.Lx678_0:
                        .quad            .Lx678_0_s
.Lx678_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n510_assign_α:
                        mov              rax, qword ptr [rsp + 1136]
                        mov              rdx, qword ptr [rsp + 1144]
                        mov              qword ptr [1879052368], rax
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n511_call_α
#=======================================================================================================================
#          left     =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n511_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx681_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx681_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx681_6]
                        lea              rdx, [rip + .Lx681_7]
                                                                                        jmp   rax
.Lx681_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx681_2
.Lx681_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx681_2
.Lx681_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx681_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx681_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx681_3]
                        lea              rdx, [rip + .Lx681_4]
                                                                                        jmp   rax
.Lx681_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx681_2
.Lx681_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx681_2
.Lx681_1:
                        call             rt_faildescr@PLT
.Lx681_2:
                        mov              qword ptr [rsp + 1168], rax
                        mov              qword ptr [rsp + 1176], rdx
                        cmp              eax, 99
                                                                                        je    n513_lit_string_α
                                                                                        jmp   n512_assign_α
n511_call_β:
                                                                                        jmp   n513_lit_string_α
.Lx681_0:
                        .quad            .Lx681_0_s
.Lx681_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n512_assign_α:
                        mov              rax, qword ptr [rsp + 1168]
                        mov              rdx, qword ptr [rsp + 1176]
                        mov              qword ptr [1879052400], rax
                        mov              qword ptr [1879052408], rdx
                                                                                        jmp   n513_lit_string_α
#=======================================================================================================================
#          Push()   =  EVAL(left ' ' op ' ' right)
#-----------------------------------------------------------------------------------------------------------------------
n513_lit_string_α:
                        mov              qword ptr [rsp + 1200], 1
                        mov              dword ptr [rsp + 1204], 0
                        mov              rax, qword ptr [rip + .Lx683_0]
                        mov              qword ptr [rsp + 1208], rax
                                                                                        jmp   n514_call_α
.Lx683_0:
                        .quad            .Lx683_0_s
.Lx683_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n514_call_α:
                        mov              rax, qword ptr [rsp + 1200]
                        mov              qword ptr [rsp + 1232], rax
                        mov              rax, qword ptr [rsp + 1208]
                        mov              qword ptr [rsp + 1240], rax
                        .section         .rodata
.Lrkfn685:              .string          "SNO$WANTNM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn685]
                        lea              rsi, [rsp + 1232]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1216], rax
                        mov              qword ptr [rsp + 1224], rdx
                        cmp              eax, 99
                                                                                        je    n527_lit_string_α
                                                                                        jmp   n515_call_α
n514_call_β:
                                                                                        jmp   n527_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n515_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx687_0]
                        mov              esi, 1
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx687_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx687_6]
                        lea              rdx, [rip + .Lx687_7]
                                                                                        jmp   rax
.Lx687_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx687_2
.Lx687_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx687_2
.Lx687_5:
                        add              rsp, 32
                        mov              rdi, qword ptr [rip + .Lx687_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx687_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx687_3]
                        lea              rdx, [rip + .Lx687_4]
                                                                                        jmp   rax
.Lx687_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx687_2
.Lx687_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx687_2
.Lx687_1:
                        call             rt_faildescr@PLT
.Lx687_2:
                        mov              qword ptr [rsp + 1264], rax
                        mov              qword ptr [rsp + 1272], rdx
                        cmp              eax, 99
                                                                                        je    n527_lit_string_α
                                                                                        jmp   n516_var_α
n515_call_β:
                                                                                        jmp   n527_lit_string_α
.Lx687_0:
                        .quad            .Lx687_0_s
.Lx687_0_s:
                        .string          "Push"
#-----------------------------------------------------------------------------------------------------------------------
n516_var_α:
                        mov              rax, qword ptr [1879052400]
                        mov              rdx, qword ptr [1879052408]
                        mov              qword ptr [rsp + 1408], rax
                        mov              qword ptr [rsp + 1416], rdx
                                                                                        jmp   n517_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n517_lit_string_α:
                        mov              qword ptr [rsp + 1424], 1
                        mov              dword ptr [rsp + 1428], 1
                        mov              rax, qword ptr [rip + .Lx689_0]
                        mov              qword ptr [rsp + 1432], rax
                                                                                        jmp   n518_binop_α
.Lx689_0:
                        .quad            .Lx689_0_s
.Lx689_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n518_binop_α:
                        mov              rdi, qword ptr [rsp + 1408]
                        mov              rsi, qword ptr [rsp + 1416]
                        mov              rdx, qword ptr [rsp + 1424]
                        mov              rcx, qword ptr [rsp + 1432]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 1392], rax
                        mov              qword ptr [rsp + 1400], rdx
                                                                                        jmp   n519_var_α
#-----------------------------------------------------------------------------------------------------------------------
n519_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 1440], rax
                        mov              qword ptr [rsp + 1448], rdx
                                                                                        jmp   n520_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n520_binop_α:
                        mov              rdi, qword ptr [rsp + 1392]
                        mov              rsi, qword ptr [rsp + 1400]
                        mov              rdx, qword ptr [rsp + 1440]
                        mov              rcx, qword ptr [rsp + 1448]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 1376], rax
                        mov              qword ptr [rsp + 1384], rdx
                                                                                        jmp   n521_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n521_lit_string_α:
                        mov              qword ptr [rsp + 1456], 1
                        mov              dword ptr [rsp + 1460], 1
                        mov              rax, qword ptr [rip + .Lx693_0]
                        mov              qword ptr [rsp + 1464], rax
                                                                                        jmp   n522_binop_α
.Lx693_0:
                        .quad            .Lx693_0_s
.Lx693_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n522_binop_α:
                        mov              rdi, qword ptr [rsp + 1376]
                        mov              rsi, qword ptr [rsp + 1384]
                        mov              rdx, qword ptr [rsp + 1456]
                        mov              rcx, qword ptr [rsp + 1464]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 1360], rax
                        mov              qword ptr [rsp + 1368], rdx
                                                                                        jmp   n523_var_α
#-----------------------------------------------------------------------------------------------------------------------
n523_var_α:
                        mov              rax, qword ptr [1879052416]
                        mov              rdx, qword ptr [1879052424]
                        mov              qword ptr [rsp + 1472], rax
                        mov              qword ptr [rsp + 1480], rdx
                                                                                        jmp   n524_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n524_binop_α:
                        mov              rdi, qword ptr [rsp + 1360]
                        mov              rsi, qword ptr [rsp + 1368]
                        mov              rdx, qword ptr [rsp + 1472]
                        mov              rcx, qword ptr [rsp + 1480]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 1344], rax
                        mov              qword ptr [rsp + 1352], rdx
                                                                                        jmp   n525_call_α
#-----------------------------------------------------------------------------------------------------------------------
n525_call_α:
                        mov              rax, qword ptr [rsp + 1344]
                        mov              qword ptr [rsp + 1312], rax
                        mov              rax, qword ptr [rsp + 1352]
                        mov              qword ptr [rsp + 1320], rax
                        .section         .rodata
.Lrkfn698:              .string          "EVAL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn698]
                        lea              rsi, [rsp + 1312]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1296], rax
                        mov              qword ptr [rsp + 1304], rdx
                        cmp              eax, 99
                                                                                        je    n527_lit_string_α
                                                                                        jmp   n526_assign_var_α
n525_call_β:
                                                                                        jmp   n527_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n526_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1264]
                        mov              rsi, qword ptr [rsp + 1272]
                        mov              rdx, qword ptr [rsp + 1296]
                        mov              rcx, qword ptr [rsp + 1304]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n527_lit_string_α
                        mov              qword ptr [rsp + 1488], rax
                        mov              qword ptr [rsp + 1496], rdx
                                                                                        jmp   n527_lit_string_α
#=======================================================================================================================
#          Binary   =  .dummy                        :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n527_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx700_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n528_call_α
.Lx700_0:
                        .quad            .Lx700_0_s
.Lx700_0_s:
                        .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n528_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd702:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd702]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 99
                                                                                        jne   .Lx701_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n464_lit_string_α
.Lx701_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n529_assign_α
n528_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n464_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n529_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052384], rax
                        mov              qword ptr [1879052392], rdx
                        add              rsp, 32
                                                                                        jmp   n464_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n530_goto_α:
                                                                                        jmp   n394_lit_string_α
n530_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n531_goto_α:
                                                                                        jmp   n427_var_α
n531_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n532_goto_α:
                                                                                        jmp   n440_lit_string_α
n532_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        mov              rsp, rbp
                        mov              rbp, [rsp + 2544]
                        add              rsp, 2552
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              rsp, rbp
                        mov              eax, 99
                        xor              edx, edx
                        mov              rbp, [rsp + 2544]
                        add              rsp, 2552
                        ret
                        .section         .rodata
.S0:                    .string          "integer"
.S1:                    .string          "epsilon"
.S2:                    .string          "exponent"
.S3:                    .string          "*Push"
.S4:                    .string          "real"
.S5:                    .string          "expr"
.S6:                    .string          "constant"
.S7:                    .string          "primary"
.S8:                    .string          "factor"
.S9:                    .string          "*Unary"
.S10:                   .string          "addop"
.S11:                   .string          "term"
.S12:                   .string          "*Binary"
.S13:                   .string          "mulop"
                        .text
                        .section         .rodata
.C0:
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
.C1:
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .text
                        .section         .note.GNU-stack,"",@progbits

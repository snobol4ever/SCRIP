                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_LBL__Push_α
proc_LBL__Push_α:
                        .global          proc_LBL__Push_α
                        .global          proc_LBL__Push_β
                        .global          proc_LBL__Push_γ
                        .global          proc_LBL__Push_ω
                        sub              rsp, 2544
                        mov              [rsp + 2520], rcx
                        mov              [rsp + 2528], rdx
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
                                                                                        jmp   n4_subscript_α
.Lx28_0:
                        .quad            0
#=======================================================================================================================
#          Push     =  .stk[stk[0]]
#-----------------------------------------------------------------------------------------------------------------------
n3_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 304], rax
                        mov              qword ptr [rsp + 312], rdx
                                                                                        jmp   n5_var_α
#-----------------------------------------------------------------------------------------------------------------------
n4_subscript_α:
                        mov              rdi, qword ptr [rsp + 144]
                        mov              rsi, qword ptr [rsp + 152]
                        mov              rdx, qword ptr [rsp + 160]
                        mov              rcx, qword ptr [rsp + 168]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n3_var_α
                        mov              qword ptr [rsp + 176], rax
                        mov              qword ptr [rsp + 184], rdx
                                                                                        jmp   n7_var_α
#-----------------------------------------------------------------------------------------------------------------------
n5_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 320], rax
                        mov              qword ptr [rsp + 328], rdx
                                                                                        jmp   n8_lit_integer_α
#=======================================================================================================================
#          $Push    =  x                             :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n6_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 448], rax
                        mov              qword ptr [rsp + 456], rdx
                                                                                        jmp   n9_call_α
#-----------------------------------------------------------------------------------------------------------------------
n7_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 208], rax
                        mov              qword ptr [rsp + 216], rdx
                                                                                        jmp   n11_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n8_lit_integer_α:
                        mov              qword ptr [rsp + 336], 6
                        mov              rax, qword ptr [rip + .Lx34_0]
                        mov              qword ptr [rsp + 344], rax
                                                                                        jmp   n12_subscript_α
.Lx34_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n9_call_α:
                        mov              rax, qword ptr [rsp + 448]
                        mov              qword ptr [rsp + 416], rax
                        mov              rax, qword ptr [rsp + 456]
                        mov              qword ptr [rsp + 424], rax
                        .section         .rodata
.Lrkfn36:               .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn36]
                        lea              rsi, [rsp + 416]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 400], rax
                        mov              qword ptr [rsp + 408], rdx
                        cmp              eax, 99
                                                                                        je    n10_lit_string_α
                                                                                        jmp   n13_var_α
n9_call_β:
                                                                                        jmp   n10_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n10_lit_string_α:
                        mov              qword ptr [rsp + 32], 1
                        mov              rax, qword ptr [rip + .Lx37_0]
                        mov              qword ptr [rsp + 40], rax
                                                                                        jmp   n14_call_α
.Lx37_0:
                        .quad            .Lx37_0_s
.Lx37_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n11_lit_integer_α:
                        mov              qword ptr [rsp + 224], 6
                        mov              rax, qword ptr [rip + .Lx38_0]
                        mov              qword ptr [rsp + 232], rax
                                                                                        jmp   n15_subscript_α
.Lx38_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n12_subscript_α:
                        mov              rdi, qword ptr [rsp + 320]
                        mov              rsi, qword ptr [rsp + 328]
                        mov              rdx, qword ptr [rsp + 336]
                        mov              rcx, qword ptr [rsp + 344]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n6_var_α
                        mov              qword ptr [rsp + 352], rax
                        mov              qword ptr [rsp + 360], rdx
                                                                                        jmp   n16_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n13_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 464], rax
                        mov              qword ptr [rsp + 472], rdx
                                                                                        jmp   n17_assign_var_α
#-----------------------------------------------------------------------------------------------------------------------
n14_call_α:
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 72], rax
                        .section         .rodata
.Lrkfn42:               .string          "SNO$NRET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn42]
                        lea              rsi, [rsp + 64]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 48], rax
                        mov              qword ptr [rsp + 56], rdx
                        cmp              eax, 99
                                                                                        je    n19_op14_α
                                                                                        jmp   n18_op14_α
n14_call_β:
                                                                                        jmp   n19_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n15_subscript_α:
                        mov              rdi, qword ptr [rsp + 208]
                        mov              rsi, qword ptr [rsp + 216]
                        mov              rdx, qword ptr [rsp + 224]
                        mov              rcx, qword ptr [rsp + 232]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n3_var_α
                        mov              qword ptr [rsp + 240], rax
                        mov              qword ptr [rsp + 248], rdx
                                                                                        jmp   n20_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n16_deref_α:
                        mov              rdi, qword ptr [rsp + 352]
                        mov              rsi, qword ptr [rsp + 360]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n6_var_α
                        mov              qword ptr [rsp + 368], rax
                        mov              qword ptr [rsp + 376], rdx
                                                                                        jmp   n21_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n17_assign_var_α:
                        mov              rdi, qword ptr [rsp + 400]
                        mov              rsi, qword ptr [rsp + 408]
                        mov              rdx, qword ptr [rsp + 464]
                        mov              rcx, qword ptr [rsp + 472]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n10_lit_string_α
                        mov              qword ptr [rsp + 480], rax
                        mov              qword ptr [rsp + 488], rdx
                                                                                        jmp   n10_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n18_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n19_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n20_deref_α:
                        mov              rdi, qword ptr [rsp + 240]
                        mov              rsi, qword ptr [rsp + 248]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n3_var_α
                        mov              qword ptr [rsp + 256], rax
                        mov              qword ptr [rsp + 264], rdx
                                                                                        jmp   n22_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n21_subscript_α:
                        mov              rdi, qword ptr [rsp + 304]
                        mov              rsi, qword ptr [rsp + 312]
                        mov              rdx, qword ptr [rsp + 368]
                        mov              rcx, qword ptr [rsp + 376]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n6_var_α
                        mov              qword ptr [rsp + 384], rax
                        mov              qword ptr [rsp + 392], rdx
                                                                                        jmp   n23_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n22_lit_integer_α:
                        mov              qword ptr [rsp + 272], 6
                        mov              rax, qword ptr [rip + .Lx52_0]
                        mov              qword ptr [rsp + 280], rax
                                                                                        jmp   n24_binop_α
.Lx52_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n23_assign_α:
                        mov              rax, qword ptr [rsp + 384]
                        mov              rdx, qword ptr [rsp + 392]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n6_var_α
#-----------------------------------------------------------------------------------------------------------------------
n24_binop_α:
                        mov              rdi, qword ptr [rsp + 256]
                        mov              rsi, qword ptr [rsp + 264]
                        mov              rdx, qword ptr [rsp + 272]
                        mov              rcx, qword ptr [rsp + 280]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        je    n3_var_α
                        mov              qword ptr [rsp + 192], rax
                        mov              qword ptr [rsp + 200], rdx
                                                                                        jmp   n25_assign_var_α
#-----------------------------------------------------------------------------------------------------------------------
n25_assign_var_α:
                        mov              rdi, qword ptr [rsp + 176]
                        mov              rsi, qword ptr [rsp + 184]
                        mov              rdx, qword ptr [rsp + 192]
                        mov              rcx, qword ptr [rsp + 200]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n3_var_α
                        mov              qword ptr [rsp + 288], rax
                        mov              qword ptr [rsp + 296], rdx
                                                                                        jmp   n3_var_α
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Push_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Push_β:
                                                                                        jmp   proc_LBL__Push_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Push_γ:
                        mov              rdi, [rsp]
                        mov              rsi, [rsp + 8]
                        mov              rax, [rsp + 2520]
                        add              rsp, 2544
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Push_ω:
                        mov              rax, [rsp + 2528]
                        add              rsp, 2544
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_LBL__Pop_α
proc_LBL__Pop_α:
                        .global          proc_LBL__Pop_α
                        .global          proc_LBL__Pop_β
                        .global          proc_LBL__Pop_γ
                        .global          proc_LBL__Pop_ω
                        sub              rsp, 2544
                        mov              [rsp + 2520], rcx
                        mov              [rsp + 2528], rdx
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
                                                                                        jmp   n60_lit_integer_α
#=======================================================================================================================
#          stk[0]   =  stk[0] - 1                    :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n59_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 608], rax
                        mov              qword ptr [rsp + 616], rdx
                                                                                        jmp   n61_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n60_lit_integer_α:
                        mov              qword ptr [rsp + 528], 6
                        mov              rax, qword ptr [rip + .Lx80_0]
                        mov              qword ptr [rsp + 536], rax
                                                                                        jmp   n63_subscript_α
.Lx80_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n61_lit_integer_α:
                        mov              qword ptr [rsp + 624], 6
                        mov              rax, qword ptr [rip + .Lx81_0]
                        mov              qword ptr [rsp + 632], rax
                                                                                        jmp   n64_subscript_α
.Lx81_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n62_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n63_subscript_α:
                        mov              rdi, qword ptr [rsp + 512]
                        mov              rsi, qword ptr [rsp + 520]
                        mov              rdx, qword ptr [rsp + 528]
                        mov              rcx, qword ptr [rsp + 536]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n59_var_α
                        mov              qword ptr [rsp + 544], rax
                        mov              qword ptr [rsp + 552], rdx
                                                                                        jmp   n65_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n64_subscript_α:
                        mov              rdi, qword ptr [rsp + 608]
                        mov              rsi, qword ptr [rsp + 616]
                        mov              rdx, qword ptr [rsp + 624]
                        mov              rcx, qword ptr [rsp + 632]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n62_op14_α
                        mov              qword ptr [rsp + 640], rax
                        mov              qword ptr [rsp + 648], rdx
                                                                                        jmp   n66_var_α
#-----------------------------------------------------------------------------------------------------------------------
n65_deref_α:
                        mov              rdi, qword ptr [rsp + 544]
                        mov              rsi, qword ptr [rsp + 552]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n59_var_α
                        mov              qword ptr [rsp + 560], rax
                        mov              qword ptr [rsp + 568], rdx
                                                                                        jmp   n67_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n66_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 672], rax
                        mov              qword ptr [rsp + 680], rdx
                                                                                        jmp   n68_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n67_subscript_α:
                        mov              rdi, qword ptr [rsp + 496]
                        mov              rsi, qword ptr [rsp + 504]
                        mov              rdx, qword ptr [rsp + 560]
                        mov              rcx, qword ptr [rsp + 568]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n59_var_α
                        mov              qword ptr [rsp + 576], rax
                        mov              qword ptr [rsp + 584], rdx
                                                                                        jmp   n69_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n68_lit_integer_α:
                        mov              qword ptr [rsp + 688], 6
                        mov              rax, qword ptr [rip + .Lx89_0]
                        mov              qword ptr [rsp + 696], rax
                                                                                        jmp   n70_subscript_α
.Lx89_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n69_deref_α:
                        mov              rdi, qword ptr [rsp + 576]
                        mov              rsi, qword ptr [rsp + 584]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n59_var_α
                        mov              qword ptr [rsp + 592], rax
                        mov              qword ptr [rsp + 600], rdx
                                                                                        jmp   n71_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n70_subscript_α:
                        mov              rdi, qword ptr [rsp + 672]
                        mov              rsi, qword ptr [rsp + 680]
                        mov              rdx, qword ptr [rsp + 688]
                        mov              rcx, qword ptr [rsp + 696]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n62_op14_α
                        mov              qword ptr [rsp + 704], rax
                        mov              qword ptr [rsp + 712], rdx
                                                                                        jmp   n72_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n71_assign_α:
                        mov              rax, qword ptr [rsp + 592]
                        mov              rdx, qword ptr [rsp + 600]
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rdx
                                                                                        jmp   n59_var_α
#-----------------------------------------------------------------------------------------------------------------------
n72_deref_α:
                        mov              rdi, qword ptr [rsp + 704]
                        mov              rsi, qword ptr [rsp + 712]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n62_op14_α
                        mov              qword ptr [rsp + 720], rax
                        mov              qword ptr [rsp + 728], rdx
                                                                                        jmp   n73_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n73_lit_integer_α:
                        mov              qword ptr [rsp + 736], 6
                        mov              rax, qword ptr [rip + .Lx94_0]
                        mov              qword ptr [rsp + 744], rax
                                                                                        jmp   n74_binop_α
.Lx94_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n74_binop_α:
                        mov              rdi, qword ptr [rsp + 720]
                        mov              rsi, qword ptr [rsp + 728]
                        mov              rdx, qword ptr [rsp + 736]
                        mov              rcx, qword ptr [rsp + 744]
                        call             rt_sub@PLT
                        cmp              eax, 99
                                                                                        je    n62_op14_α
                        mov              qword ptr [rsp + 656], rax
                        mov              qword ptr [rsp + 664], rdx
                                                                                        jmp   n75_assign_var_α
#-----------------------------------------------------------------------------------------------------------------------
n75_assign_var_α:
                        mov              rdi, qword ptr [rsp + 640]
                        mov              rsi, qword ptr [rsp + 648]
                        mov              rdx, qword ptr [rsp + 656]
                        mov              rcx, qword ptr [rsp + 664]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n62_op14_α
                        mov              qword ptr [rsp + 752], rax
                        mov              qword ptr [rsp + 760], rdx
                                                                                        jmp   n62_op14_α
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Pop_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Pop_β:
                                                                                        jmp   proc_LBL__Pop_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Pop_γ:
                        mov              rdi, [rsp]
                        mov              rsi, [rsp + 8]
                        mov              rax, [rsp + 2520]
                        add              rsp, 2544
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Pop_ω:
                        mov              rax, [rsp + 2528]
                        add              rsp, 2544
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_LBL__Unary_α
proc_LBL__Unary_α:
                        .global          proc_LBL__Unary_α
                        .global          proc_LBL__Unary_β
                        .global          proc_LBL__Unary_γ
                        .global          proc_LBL__Unary_ω
                        sub              rsp, 2544
                        mov              [rsp + 2520], rcx
                        mov              [rsp + 2528], rdx
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
                                                                                        je    n105_lit_string_α
                                                                                        jmp   n104_call_α
n103_call_β:
                                                                                        jmp   n105_lit_string_α
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
                                                                                        je    n105_lit_string_α
                                                                                        jmp   n106_var_α
n104_call_β:
                                                                                        jmp   n105_lit_string_α
.Lx128_0:
                        .quad            .Lx128_0_s
.Lx128_0_s:
                        .string          "Push"
#=======================================================================================================================
#          Unary    =  .dummy                        :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n105_lit_string_α:
                        mov              qword ptr [rsp + 1088], 1
                        mov              rax, qword ptr [rip + .Lx129_0]
                        mov              qword ptr [rsp + 1096], rax
                                                                                        jmp   n107_call_α
.Lx129_0:
                        .quad            .Lx129_0_s
.Lx129_0_s:
                        .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n106_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 992], rax
                        mov              qword ptr [rsp + 1000], rdx
                                                                                        jmp   n108_var_α
#-----------------------------------------------------------------------------------------------------------------------
n107_call_α:
                        mov              rax, qword ptr [rsp + 1088]
                        mov              qword ptr [rsp + 1056], rax
                        mov              rax, qword ptr [rsp + 1096]
                        mov              qword ptr [rsp + 1064], rax
                        .section         .rodata
.Lrkfn132:              .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn132]
                        lea              rsi, [rsp + 1056]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1040], rax
                        mov              qword ptr [rsp + 1048], rdx
                        cmp              eax, 99
                                                                                        je    n110_lit_string_α
                                                                                        jmp   n109_assign_α
n107_call_β:
                                                                                        jmp   n110_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n108_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 1008], rax
                        mov              qword ptr [rsp + 1016], rdx
                                                                                        jmp   n111_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n109_assign_α:
                        mov              rax, qword ptr [rsp + 1040]
                        mov              rdx, qword ptr [rsp + 1048]
                        mov              qword ptr [1879052336], rax
                        mov              qword ptr [1879052344], rdx
                                                                                        jmp   n110_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n110_lit_string_α:
                        mov              qword ptr [rsp + 32], 1
                        mov              rax, qword ptr [rip + .Lx135_0]
                        mov              qword ptr [rsp + 40], rax
                                                                                        jmp   n112_call_α
.Lx135_0:
                        .quad            .Lx135_0_s
.Lx135_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n111_binop_α:
                        mov              rdi, qword ptr [rsp + 992]
                        mov              rsi, qword ptr [rsp + 1000]
                        mov              rdx, qword ptr [rsp + 1008]
                        mov              rcx, qword ptr [rsp + 1016]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 976], rax
                        mov              qword ptr [rsp + 984], rdx
                                                                                        jmp   n113_call_α
#-----------------------------------------------------------------------------------------------------------------------
n112_call_α:
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 72], rax
                        .section         .rodata
.Lrkfn138:              .string          "SNO$NRET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn138]
                        lea              rsi, [rsp + 64]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 48], rax
                        mov              qword ptr [rsp + 56], rdx
                        cmp              eax, 99
                                                                                        je    n115_op14_α
                                                                                        jmp   n114_op14_α
n112_call_β:
                                                                                        jmp   n115_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n113_call_α:
                        mov              rax, qword ptr [rsp + 976]
                        mov              qword ptr [rsp + 944], rax
                        mov              rax, qword ptr [rsp + 984]
                        mov              qword ptr [rsp + 952], rax
                        .section         .rodata
.Lrkfn140:              .string          "EVAL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn140]
                        lea              rsi, [rsp + 944]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 928], rax
                        mov              qword ptr [rsp + 936], rdx
                        cmp              eax, 99
                                                                                        je    n105_lit_string_α
                                                                                        jmp   n116_assign_var_α
n113_call_β:
                                                                                        jmp   n105_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n114_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n115_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n116_assign_var_α:
                        mov              rdi, qword ptr [rsp + 896]
                        mov              rsi, qword ptr [rsp + 904]
                        mov              rdx, qword ptr [rsp + 928]
                        mov              rcx, qword ptr [rsp + 936]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n105_lit_string_α
                        mov              qword ptr [rsp + 1024], rax
                        mov              qword ptr [rsp + 1032], rdx
                                                                                        jmp   n105_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Unary_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Unary_β:
                                                                                        jmp   proc_LBL__Unary_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Unary_γ:
                        mov              rdi, [rsp]
                        mov              rsi, [rsp + 8]
                        mov              rax, [rsp + 2520]
                        add              rsp, 2544
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Unary_ω:
                        mov              rax, [rsp + 2528]
                        add              rsp, 2544
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_LBL__Binary_α
proc_LBL__Binary_α:
                        .global          proc_LBL__Binary_α
                        .global          proc_LBL__Binary_β
                        .global          proc_LBL__Binary_γ
                        .global          proc_LBL__Binary_ω
                        sub              rsp, 2544
                        mov              [rsp + 2520], rcx
                        mov              [rsp + 2528], rdx
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
                                                                                        je    n156_lit_string_α
                                                                                        jmp   n155_call_α
n154_call_β:
                                                                                        jmp   n156_lit_string_α
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
                                                                                        je    n156_lit_string_α
                                                                                        jmp   n157_var_α
n155_call_β:
                                                                                        jmp   n156_lit_string_α
.Lx188_0:
                        .quad            .Lx188_0_s
.Lx188_0_s:
                        .string          "Push"
#=======================================================================================================================
#          Binary   =  .dummy                        :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n156_lit_string_α:
                        mov              qword ptr [rsp + 1552], 1
                        mov              rax, qword ptr [rip + .Lx189_0]
                        mov              qword ptr [rsp + 1560], rax
                                                                                        jmp   n158_call_α
.Lx189_0:
                        .quad            .Lx189_0_s
.Lx189_0_s:
                        .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n157_var_α:
                        mov              rax, qword ptr [1879052400]
                        mov              rdx, qword ptr [1879052408]
                        mov              qword ptr [rsp + 1408], rax
                        mov              qword ptr [rsp + 1416], rdx
                                                                                        jmp   n159_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n158_call_α:
                        mov              rax, qword ptr [rsp + 1552]
                        mov              qword ptr [rsp + 1520], rax
                        mov              rax, qword ptr [rsp + 1560]
                        mov              qword ptr [rsp + 1528], rax
                        .section         .rodata
.Lrkfn192:              .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn192]
                        lea              rsi, [rsp + 1520]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1504], rax
                        mov              qword ptr [rsp + 1512], rdx
                        cmp              eax, 99
                                                                                        je    n161_lit_string_α
                                                                                        jmp   n160_assign_α
n158_call_β:
                                                                                        jmp   n161_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n159_lit_string_α:
                        mov              qword ptr [rsp + 1424], 1
                        mov              rax, qword ptr [rip + .Lx193_0]
                        mov              qword ptr [rsp + 1432], rax
                                                                                        jmp   n162_binop_α
.Lx193_0:
                        .quad            .Lx193_0_s
.Lx193_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n160_assign_α:
                        mov              rax, qword ptr [rsp + 1504]
                        mov              rdx, qword ptr [rsp + 1512]
                        mov              qword ptr [1879052384], rax
                        mov              qword ptr [1879052392], rdx
                                                                                        jmp   n161_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n161_lit_string_α:
                        mov              qword ptr [rsp + 32], 1
                        mov              rax, qword ptr [rip + .Lx195_0]
                        mov              qword ptr [rsp + 40], rax
                                                                                        jmp   n163_call_α
.Lx195_0:
                        .quad            .Lx195_0_s
.Lx195_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n162_binop_α:
                        mov              rdi, qword ptr [rsp + 1408]
                        mov              rsi, qword ptr [rsp + 1416]
                        mov              rdx, qword ptr [rsp + 1424]
                        mov              rcx, qword ptr [rsp + 1432]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 1392], rax
                        mov              qword ptr [rsp + 1400], rdx
                                                                                        jmp   n164_var_α
#-----------------------------------------------------------------------------------------------------------------------
n163_call_α:
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 72], rax
                        .section         .rodata
.Lrkfn198:              .string          "SNO$NRET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn198]
                        lea              rsi, [rsp + 64]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 48], rax
                        mov              qword ptr [rsp + 56], rdx
                        cmp              eax, 99
                                                                                        je    n166_op14_α
                                                                                        jmp   n165_op14_α
n163_call_β:
                                                                                        jmp   n166_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n164_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 1440], rax
                        mov              qword ptr [rsp + 1448], rdx
                                                                                        jmp   n167_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n165_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n166_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n167_binop_α:
                        mov              rdi, qword ptr [rsp + 1392]
                        mov              rsi, qword ptr [rsp + 1400]
                        mov              rdx, qword ptr [rsp + 1440]
                        mov              rcx, qword ptr [rsp + 1448]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 1376], rax
                        mov              qword ptr [rsp + 1384], rdx
                                                                                        jmp   n168_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n168_lit_string_α:
                        mov              qword ptr [rsp + 1456], 1
                        mov              rax, qword ptr [rip + .Lx205_0]
                        mov              qword ptr [rsp + 1464], rax
                                                                                        jmp   n169_binop_α
.Lx205_0:
                        .quad            .Lx205_0_s
.Lx205_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n169_binop_α:
                        mov              rdi, qword ptr [rsp + 1376]
                        mov              rsi, qword ptr [rsp + 1384]
                        mov              rdx, qword ptr [rsp + 1456]
                        mov              rcx, qword ptr [rsp + 1464]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 1360], rax
                        mov              qword ptr [rsp + 1368], rdx
                                                                                        jmp   n170_var_α
#-----------------------------------------------------------------------------------------------------------------------
n170_var_α:
                        mov              rax, qword ptr [1879052416]
                        mov              rdx, qword ptr [1879052424]
                        mov              qword ptr [rsp + 1472], rax
                        mov              qword ptr [rsp + 1480], rdx
                                                                                        jmp   n171_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n171_binop_α:
                        mov              rdi, qword ptr [rsp + 1360]
                        mov              rsi, qword ptr [rsp + 1368]
                        mov              rdx, qword ptr [rsp + 1472]
                        mov              rcx, qword ptr [rsp + 1480]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 1344], rax
                        mov              qword ptr [rsp + 1352], rdx
                                                                                        jmp   n172_call_α
#-----------------------------------------------------------------------------------------------------------------------
n172_call_α:
                        mov              rax, qword ptr [rsp + 1344]
                        mov              qword ptr [rsp + 1312], rax
                        mov              rax, qword ptr [rsp + 1352]
                        mov              qword ptr [rsp + 1320], rax
                        .section         .rodata
.Lrkfn210:              .string          "EVAL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn210]
                        lea              rsi, [rsp + 1312]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1296], rax
                        mov              qword ptr [rsp + 1304], rdx
                        cmp              eax, 99
                                                                                        je    n156_lit_string_α
                                                                                        jmp   n173_assign_var_α
n172_call_β:
                                                                                        jmp   n156_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n173_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1264]
                        mov              rsi, qword ptr [rsp + 1272]
                        mov              rdx, qword ptr [rsp + 1296]
                        mov              rcx, qword ptr [rsp + 1304]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n156_lit_string_α
                        mov              qword ptr [rsp + 1488], rax
                        mov              qword ptr [rsp + 1496], rdx
                                                                                        jmp   n156_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Binary_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Binary_β:
                                                                                        jmp   proc_LBL__Binary_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Binary_γ:
                        mov              rdi, [rsp]
                        mov              rsi, [rsp + 8]
                        mov              rax, [rsp + 2520]
                        add              rsp, 2544
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__Binary_ω:
                        mov              rax, [rsp + 2528]
                        add              rsp, 2544
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_Push_α
proc_Push_α:
                        .global          proc_Push_α
                        .global          proc_Push_β
                        .global          proc_Push_γ
                        .global          proc_Push_ω
                        sub              rsp, 2544
                        mov              [rsp + 2520], rcx
                        mov              [rsp + 2528], rdx
                        mov              rdi, rsp
                        mov              ecx, 2512
                        xor              eax, eax
                        rep stosb
proc_Push_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n212_op14_α:
                        mov              rdi, qword ptr [rsp + 2520]
                        mov              rsi, qword ptr [rsp + 2528]
                        lea              rdx, [rsp + 2544]
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
                        mov              rax, [rsp + 2520]
                        add              rsp, 2544
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_Push_ω:
                        mov              rax, [rsp + 2528]
                        add              rsp, 2544
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_Pop_α
proc_Pop_α:
                        .global          proc_Pop_α
                        .global          proc_Pop_β
                        .global          proc_Pop_γ
                        .global          proc_Pop_ω
                        sub              rsp, 2544
                        mov              [rsp + 2520], rcx
                        mov              [rsp + 2528], rdx
                        mov              rdi, rsp
                        mov              ecx, 2512
                        xor              eax, eax
                        rep stosb
proc_Pop_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n218_op14_α:
                        mov              rdi, qword ptr [rsp + 2520]
                        mov              rsi, qword ptr [rsp + 2528]
                        lea              rdx, [rsp + 2544]
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
                        mov              rax, [rsp + 2520]
                        add              rsp, 2544
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_Pop_ω:
                        mov              rax, [rsp + 2528]
                        add              rsp, 2544
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_Unary_α
proc_Unary_α:
                        .global          proc_Unary_α
                        .global          proc_Unary_β
                        .global          proc_Unary_γ
                        .global          proc_Unary_ω
                        sub              rsp, 2544
                        mov              [rsp + 2520], rcx
                        mov              [rsp + 2528], rdx
                        mov              rdi, rsp
                        mov              ecx, 2512
                        xor              eax, eax
                        rep stosb
proc_Unary_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n224_op14_α:
                        mov              rdi, qword ptr [rsp + 2520]
                        mov              rsi, qword ptr [rsp + 2528]
                        lea              rdx, [rsp + 2544]
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
                        mov              rax, [rsp + 2520]
                        add              rsp, 2544
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_Unary_ω:
                        mov              rax, [rsp + 2528]
                        add              rsp, 2544
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_Binary_α
proc_Binary_α:
                        .global          proc_Binary_α
                        .global          proc_Binary_β
                        .global          proc_Binary_γ
                        .global          proc_Binary_ω
                        sub              rsp, 2544
                        mov              [rsp + 2520], rcx
                        mov              [rsp + 2528], rdx
                        mov              rdi, rsp
                        mov              ecx, 2512
                        xor              eax, eax
                        rep stosb
proc_Binary_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n230_op14_α:
                        mov              rdi, qword ptr [rsp + 2520]
                        mov              rsi, qword ptr [rsp + 2528]
                        lea              rdx, [rsp + 2544]
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
                        mov              rax, [rsp + 2520]
                        add              rsp, 2544
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_Binary_ω:
                        mov              rax, [rsp + 2528]
                        add              rsp, 2544
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$0_α
proc_PAT$0_α:
                        .global          proc_PAT$0_α
                        .global          proc_PAT$0_β
                        .global          proc_PAT$0_γ
                        .global          proc_PAT$0_ω
                        sub              rsp, 64
                        mov              [rsp + 40], rcx
                        mov              [rsp + 48], rdx
                        mov              [rsp + 56], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 16], 0
                        mov              qword ptr [rsp + 24], 0
                        mov              qword ptr [rsp + 32], r8
                        mov              dword ptr [rsp + 24], r14d
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
                        cmp              qword ptr [rbp + 32], 1
                                                                                        jne   7f
                        mov              ecx, dword ptr [rbp + 24]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx], ecx
7:
                                                                                        jmp   proc_PAT$0_γ
proc_PAT$0_scanfail:
                        cmp              qword ptr [rbp + 32], 1
                                                                                        jne   8f
                        mov              eax, dword ptr [rbp + 24]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    8f
                        lea              rcx, [rip + g_anchor]
                        cmp              qword ptr [rcx], 0
                                                                                        jne   8f
                        mov              dword ptr [rbp + 24], eax
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
                        mov              rax, [rbp + 40]
                        mov              rbp, [rbp + 56]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_ω:
                        mov              rax, [rbp + 48]
                        lea              rsp, [rbp + 64]
                        mov              rbp, [rbp + 56]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$1_α
proc_PAT$1_α:
                        .global          proc_PAT$1_α
                        .global          proc_PAT$1_β
                        .global          proc_PAT$1_γ
                        .global          proc_PAT$1_ω
                        sub              rsp, 160
                        mov              [rsp + 136], rcx
                        mov              [rsp + 144], rdx
                        mov              [rsp + 152], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 112], 0
                        mov              qword ptr [rsp + 120], 0
                        mov              qword ptr [rsp + 128], r8
                        mov              dword ptr [rsp + 120], r14d
proc_PAT$1_attempt:
proc_PAT$1_α_body:
                        lea              rax, [rip + n239_match_sequence_β]
                        mov              qword ptr [rbp + 112], rax
#-----------------------------------------------------------------------------------------------------------------------
n239_match_sequence_α:
                        mov              dword ptr [rbp + 16], r14d
                                                                                        jmp   n240_match_any_α
n239_match_sequence_as:
                                                                                        jmp   proc_PAT$1_γ
n239_match_sequence_β:
                                                                                        jmp   n242_match_defer_β
n239_match_sequence_af:
                                                                                        jmp   proc_PAT$1_ω
#-----------------------------------------------------------------------------------------------------------------------
n240_match_any_α:
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
n240_match_any_β:
                        sub              r14d, 1
                                                                                        jmp   proc_PAT$1_ω
#-----------------------------------------------------------------------------------------------------------------------
n241_match_alternate_α:
                        mov              dword ptr [rbp + 48], r14d
                        lea              rax, [rip + .Lx250_21]
                        mov              qword ptr [rbp + 64], rax
                                                                                        jmp   n243_match_any_α
.Lx250_21:
                        lea              rax, [rip + .Lx250_19]
                        mov              qword ptr [rbp + 64], rax
                                                                                        jmp   n244_match_defer_α
n241_match_alternate_s0:
                        lea              rax, [rip + .Lx250_40]
                        mov              qword ptr [rbp + 56], rax
                                                                                        jmp   n241_match_alternate_as
n241_match_alternate_s1:
                        lea              rax, [rip + .Lx250_41]
                        mov              qword ptr [rbp + 56], rax
                                                                                        jmp   n241_match_alternate_as
.Lx250_40:
                                                                                        jmp   n243_match_any_β
.Lx250_41:
                                                                                        jmp   n244_match_defer_β
n241_match_alternate_as:
                                                                                        jmp   n242_match_defer_α
n241_match_alternate_β:
                        mov              rax, qword ptr [rbp + 56]
                                                                                        jmp   rax
n241_match_alternate_af:
                        mov              r14d, dword ptr [rbp + 48]
                        mov              rax, qword ptr [rbp + 64]
                                                                                        jmp   rax
.Lx250_19:
                                                                                        jmp   n240_match_any_β
#-----------------------------------------------------------------------------------------------------------------------
n242_match_defer_α:
                        mov              rax, qword ptr [1879052464]
                        mov              rdx, qword ptr [1879052472]
                        cmp              eax, 3
                                                                                        jne   .Lx251_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx251_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx251_10
.Lx251_9:
                        xor              eax, eax
.Lx251_10:
                        test             rax, rax
                                                                                        jz    .Lx251_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx251_4]
                        lea              rdx, [rip + .Lx251_5]
                                                                                        jmp   rax
.Lx251_4:
                                                                                        jmp   proc_PAT$1_γ
.Lx251_5:
                                                                                        jmp   n241_match_alternate_β
.Lx251_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S0]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx251_2:
                        test             rax, rax
                                                                                        je    .Lx251_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx251_7]
                        lea              rdx, [rip + .Lx251_8]
                                                                                        jmp   rax
.Lx251_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx251_2
.Lx251_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx251_2
.Lx251_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n241_match_alternate_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx251_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   proc_PAT$1_γ
.Lx251_6:
                        add              rsp, 16
                                                                                        jmp   n241_match_alternate_β
n242_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n243_match_any_α:
                        mov              eax, r14d
                        cmp              eax, r15d
                                                                                        jge   n241_match_alternate_af
                        movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 43
                                                                                        je    .Lx253_0
                        cmp              esi, 45
                                                                                        je    .Lx253_0
                                                                                        jmp   n241_match_alternate_af
.Lx253_0:
                        add              r14d, 1
                                                                                        jmp   n241_match_alternate_s0
n243_match_any_β:
                        sub              r14d, 1
                                                                                        jmp   n241_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n244_match_defer_α:
                        lea              rdi, [rip + .S1]
                        xor              esi, esi
                        call             rt_defer_get_pat_fn@PLT
                        test             rax, rax
                                                                                        jz    .Lx254_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx254_4]
                        lea              rdx, [rip + .Lx254_5]
                                                                                        jmp   rax
.Lx254_4:
                                                                                        jmp   n241_match_alternate_s1
.Lx254_5:
                                                                                        jmp   n241_match_alternate_af
.Lx254_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S1]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx254_2:
                        test             rax, rax
                                                                                        je    .Lx254_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx254_7]
                        lea              rdx, [rip + .Lx254_8]
                                                                                        jmp   rax
.Lx254_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx254_2
.Lx254_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx254_2
.Lx254_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n241_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx254_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n241_match_alternate_s1
.Lx254_6:
                        add              rsp, 16
                                                                                        jmp   n241_match_alternate_af
n244_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
proc_PAT$1_scanhit:
                        cmp              qword ptr [rbp + 128], 1
                                                                                        jne   7f
                        mov              ecx, dword ptr [rbp + 120]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx], ecx
7:
                                                                                        jmp   proc_PAT$1_γ
proc_PAT$1_scanfail:
                        cmp              qword ptr [rbp + 128], 1
                                                                                        jne   8f
                        mov              eax, dword ptr [rbp + 120]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    8f
                        lea              rcx, [rip + g_anchor]
                        cmp              qword ptr [rcx], 0
                                                                                        jne   8f
                        mov              dword ptr [rbp + 120], eax
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
                        mov              rax, [rbp + 136]
                        mov              rbp, [rbp + 152]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$1_ω:
                        mov              rax, [rbp + 144]
                        lea              rsp, [rbp + 160]
                        mov              rbp, [rbp + 152]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$2_α
proc_PAT$2_α:
                        .global          proc_PAT$2_α
                        .global          proc_PAT$2_β
                        .global          proc_PAT$2_γ
                        .global          proc_PAT$2_ω
                        sub              rsp, 320
                        mov              [rsp + 296], rcx
                        mov              [rsp + 304], rdx
                        mov              [rsp + 312], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 272], 0
                        mov              qword ptr [rsp + 280], 0
                        mov              qword ptr [rsp + 288], r8
                        mov              dword ptr [rsp + 280], r14d
proc_PAT$2_attempt:
proc_PAT$2_α_body:
                        lea              rax, [rip + n255_match_alternate_β]
                        mov              qword ptr [rbp + 272], rax
#-----------------------------------------------------------------------------------------------------------------------
n255_match_alternate_α:
                        mov              dword ptr [rbp + 16], r14d
                        lea              rax, [rip + .Lx269_21]
                        mov              qword ptr [rbp + 32], rax
                                                                                        jmp   n256_match_sequence_α
.Lx269_21:
                        lea              rax, [rip + .Lx269_19]
                        mov              qword ptr [rbp + 32], rax
                                                                                        jmp   n257_match_sequence_α
n255_match_alternate_s0:
                        lea              rax, [rip + .Lx269_40]
                        mov              qword ptr [rbp + 24], rax
                                                                                        jmp   n255_match_alternate_as
n255_match_alternate_s1:
                        lea              rax, [rip + .Lx269_41]
                        mov              qword ptr [rbp + 24], rax
                                                                                        jmp   n255_match_alternate_as
.Lx269_40:
                                                                                        jmp   n256_match_sequence_β
.Lx269_41:
                                                                                        jmp   n257_match_sequence_β
n255_match_alternate_as:
                                                                                        jmp   proc_PAT$2_γ
n255_match_alternate_β:
                        mov              rax, qword ptr [rbp + 24]
                                                                                        jmp   rax
n255_match_alternate_af:
                        mov              r14d, dword ptr [rbp + 16]
                        mov              rax, qword ptr [rbp + 32]
                                                                                        jmp   rax
.Lx269_19:
                                                                                        jmp   proc_PAT$2_ω
#-----------------------------------------------------------------------------------------------------------------------
n256_match_sequence_α:
                        mov              dword ptr [rbp + 48], r14d
                                                                                        jmp   n258_match_defer_α
n256_match_sequence_as:
                                                                                        jmp   n255_match_alternate_s0
n256_match_sequence_β:
                                                                                        jmp   n261_match_alternate_β
n256_match_sequence_af:
                                                                                        jmp   n255_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n257_match_sequence_α:
                        mov              dword ptr [rbp + 224], r14d
                                                                                        jmp   n262_match_defer_α
n257_match_sequence_as:
                                                                                        jmp   n255_match_alternate_s1
n257_match_sequence_β:
                                                                                        jmp   n263_match_defer_β
n257_match_sequence_af:
                                                                                        jmp   n255_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n258_match_defer_α:
                        mov              rax, qword ptr [1879052464]
                        mov              rdx, qword ptr [1879052472]
                        cmp              eax, 3
                                                                                        jne   .Lx274_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx274_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx274_10
.Lx274_9:
                        xor              eax, eax
.Lx274_10:
                        test             rax, rax
                                                                                        jz    .Lx274_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx274_4]
                        lea              rdx, [rip + .Lx274_5]
                                                                                        jmp   rax
.Lx274_4:
                                                                                        jmp   n259_match_lit_α
.Lx274_5:
                                                                                        jmp   n255_match_alternate_af
.Lx274_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S0]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx274_2:
                        test             rax, rax
                                                                                        je    .Lx274_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx274_7]
                        lea              rdx, [rip + .Lx274_8]
                                                                                        jmp   rax
.Lx274_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx274_2
.Lx274_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx274_2
.Lx274_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n255_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx274_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n259_match_lit_α
.Lx274_6:
                        add              rsp, 16
                                                                                        jmp   n255_match_alternate_af
n258_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n259_match_lit_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jg    n258_match_defer_β
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 46
                                                                                        jne   n258_match_defer_β
                        add              r14d, 1
                                                                                        jmp   n260_match_alternate_α
n259_match_lit_β:
                        sub              r14d, 1
                                                                                        jmp   n258_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n260_match_alternate_α:
                        mov              dword ptr [rbp + 96], r14d
                        lea              rax, [rip + .Lx278_21]
                        mov              qword ptr [rbp + 112], rax
                                                                                        jmp   n264_match_defer_α
.Lx278_21:
                        lea              rax, [rip + .Lx278_19]
                        mov              qword ptr [rbp + 112], rax
                                                                                        jmp   n265_match_defer_α
n260_match_alternate_s0:
                        lea              rax, [rip + .Lx278_40]
                        mov              qword ptr [rbp + 104], rax
                                                                                        jmp   n260_match_alternate_as
n260_match_alternate_s1:
                        lea              rax, [rip + .Lx278_41]
                        mov              qword ptr [rbp + 104], rax
                                                                                        jmp   n260_match_alternate_as
.Lx278_40:
                                                                                        jmp   n264_match_defer_β
.Lx278_41:
                                                                                        jmp   n265_match_defer_β
n260_match_alternate_as:
                                                                                        jmp   n261_match_alternate_α
n260_match_alternate_β:
                        mov              rax, qword ptr [rbp + 104]
                                                                                        jmp   rax
n260_match_alternate_af:
                        mov              r14d, dword ptr [rbp + 96]
                        mov              rax, qword ptr [rbp + 112]
                                                                                        jmp   rax
.Lx278_19:
                                                                                        jmp   n259_match_lit_β
#-----------------------------------------------------------------------------------------------------------------------
n261_match_alternate_α:
                        mov              dword ptr [rbp + 160], r14d
                        lea              rax, [rip + .Lx280_21]
                        mov              qword ptr [rbp + 176], rax
                                                                                        jmp   n266_match_defer_α
.Lx280_21:
                        lea              rax, [rip + .Lx280_19]
                        mov              qword ptr [rbp + 176], rax
                                                                                        jmp   n267_match_defer_α
n261_match_alternate_s0:
                        lea              rax, [rip + .Lx280_40]
                        mov              qword ptr [rbp + 168], rax
                                                                                        jmp   n261_match_alternate_as
n261_match_alternate_s1:
                        lea              rax, [rip + .Lx280_41]
                        mov              qword ptr [rbp + 168], rax
                                                                                        jmp   n261_match_alternate_as
.Lx280_40:
                                                                                        jmp   n266_match_defer_β
.Lx280_41:
                                                                                        jmp   n267_match_defer_β
n261_match_alternate_as:
                                                                                        jmp   n255_match_alternate_s0
n261_match_alternate_β:
                        mov              rax, qword ptr [rbp + 168]
                                                                                        jmp   rax
n261_match_alternate_af:
                        mov              r14d, dword ptr [rbp + 160]
                        mov              rax, qword ptr [rbp + 176]
                                                                                        jmp   rax
.Lx280_19:
                                                                                        jmp   n260_match_alternate_β
#-----------------------------------------------------------------------------------------------------------------------
n262_match_defer_α:
                        mov              rax, qword ptr [1879052464]
                        mov              rdx, qword ptr [1879052472]
                        cmp              eax, 3
                                                                                        jne   .Lx281_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx281_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx281_10
.Lx281_9:
                        xor              eax, eax
.Lx281_10:
                        test             rax, rax
                                                                                        jz    .Lx281_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx281_4]
                        lea              rdx, [rip + .Lx281_5]
                                                                                        jmp   rax
.Lx281_4:
                                                                                        jmp   n263_match_defer_α
.Lx281_5:
                                                                                        jmp   n255_match_alternate_af
.Lx281_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S0]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx281_2:
                        test             rax, rax
                                                                                        je    .Lx281_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx281_7]
                        lea              rdx, [rip + .Lx281_8]
                                                                                        jmp   rax
.Lx281_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx281_2
.Lx281_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx281_2
.Lx281_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n255_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx281_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n263_match_defer_α
.Lx281_6:
                        add              rsp, 16
                                                                                        jmp   n255_match_alternate_af
n262_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n263_match_defer_α:
                        mov              rax, qword ptr [1879052480]
                        mov              rdx, qword ptr [1879052488]
                        cmp              eax, 3
                                                                                        jne   .Lx282_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx282_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx282_10
.Lx282_9:
                        xor              eax, eax
.Lx282_10:
                        test             rax, rax
                                                                                        jz    .Lx282_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx282_4]
                        lea              rdx, [rip + .Lx282_5]
                                                                                        jmp   rax
.Lx282_4:
                                                                                        jmp   n255_match_alternate_s1
.Lx282_5:
                                                                                        jmp   n262_match_defer_β
.Lx282_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S2]
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
                                                                                        js    n262_match_defer_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx282_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n255_match_alternate_s1
.Lx282_6:
                        add              rsp, 16
                                                                                        jmp   n262_match_defer_β
n263_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n264_match_defer_α:
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
                                                                                        jmp   n260_match_alternate_s0
.Lx283_5:
                                                                                        jmp   n260_match_alternate_af
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
                                                                                        js    n260_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx283_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n260_match_alternate_s0
.Lx283_6:
                        add              rsp, 16
                                                                                        jmp   n260_match_alternate_af
n264_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n265_match_defer_α:
                        lea              rdi, [rip + .S1]
                        xor              esi, esi
                        call             rt_defer_get_pat_fn@PLT
                        test             rax, rax
                                                                                        jz    .Lx284_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx284_4]
                        lea              rdx, [rip + .Lx284_5]
                                                                                        jmp   rax
.Lx284_4:
                                                                                        jmp   n260_match_alternate_s1
.Lx284_5:
                                                                                        jmp   n260_match_alternate_af
.Lx284_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S1]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx284_2:
                        test             rax, rax
                                                                                        je    .Lx284_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx284_7]
                        lea              rdx, [rip + .Lx284_8]
                                                                                        jmp   rax
.Lx284_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx284_2
.Lx284_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx284_2
.Lx284_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n260_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx284_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n260_match_alternate_s1
.Lx284_6:
                        add              rsp, 16
                                                                                        jmp   n260_match_alternate_af
n265_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n266_match_defer_α:
                        mov              rax, qword ptr [1879052480]
                        mov              rdx, qword ptr [1879052488]
                        cmp              eax, 3
                                                                                        jne   .Lx285_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx285_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx285_10
.Lx285_9:
                        xor              eax, eax
.Lx285_10:
                        test             rax, rax
                                                                                        jz    .Lx285_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx285_4]
                        lea              rdx, [rip + .Lx285_5]
                                                                                        jmp   rax
.Lx285_4:
                                                                                        jmp   n261_match_alternate_s0
.Lx285_5:
                                                                                        jmp   n261_match_alternate_af
.Lx285_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S2]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx285_2:
                        test             rax, rax
                                                                                        je    .Lx285_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx285_7]
                        lea              rdx, [rip + .Lx285_8]
                                                                                        jmp   rax
.Lx285_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx285_2
.Lx285_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx285_2
.Lx285_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n261_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx285_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n261_match_alternate_s0
.Lx285_6:
                        add              rsp, 16
                                                                                        jmp   n261_match_alternate_af
n266_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n267_match_defer_α:
                        lea              rdi, [rip + .S1]
                        xor              esi, esi
                        call             rt_defer_get_pat_fn@PLT
                        test             rax, rax
                                                                                        jz    .Lx286_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx286_4]
                        lea              rdx, [rip + .Lx286_5]
                                                                                        jmp   rax
.Lx286_4:
                                                                                        jmp   n261_match_alternate_s1
.Lx286_5:
                                                                                        jmp   n261_match_alternate_af
.Lx286_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S1]
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
                                                                                        js    n261_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx286_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n261_match_alternate_s1
.Lx286_6:
                        add              rsp, 16
                                                                                        jmp   n261_match_alternate_af
n267_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
proc_PAT$2_scanhit:
                        cmp              qword ptr [rbp + 288], 1
                                                                                        jne   7f
                        mov              ecx, dword ptr [rbp + 280]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx], ecx
7:
                                                                                        jmp   proc_PAT$2_γ
proc_PAT$2_scanfail:
                        cmp              qword ptr [rbp + 288], 1
                                                                                        jne   8f
                        mov              eax, dword ptr [rbp + 280]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    8f
                        lea              rcx, [rip + g_anchor]
                        cmp              qword ptr [rcx], 0
                                                                                        jne   8f
                        mov              dword ptr [rbp + 280], eax
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
                        mov              rax, [rbp + 296]
                        mov              rbp, [rbp + 312]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$2_ω:
                        mov              rax, [rbp + 304]
                        lea              rsp, [rbp + 320]
                        mov              rbp, [rbp + 312]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$3_α
proc_PAT$3_α:
                        .global          proc_PAT$3_α
                        .global          proc_PAT$3_β
                        .global          proc_PAT$3_γ
                        .global          proc_PAT$3_ω
                        sub              rsp, 128
                        mov              [rsp + 104], rcx
                        mov              [rsp + 112], rdx
                        mov              [rsp + 120], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 48], 0
                        mov              qword ptr [rsp + 56], 0
                        mov              qword ptr [rsp + 80], 0
                        mov              qword ptr [rsp + 88], 0
                        mov              qword ptr [rsp + 96], r8
                        mov              dword ptr [rsp + 88], r14d
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
                        cmp              qword ptr [rbp + 96], 1
                                                                                        jne   7f
                        mov              ecx, dword ptr [rbp + 88]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx], ecx
7:
                                                                                        jmp   proc_PAT$3_γ
proc_PAT$3_scanfail:
                        cmp              qword ptr [rbp + 96], 1
                                                                                        jne   8f
                        mov              eax, dword ptr [rbp + 88]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    8f
                        lea              rcx, [rip + g_anchor]
                        cmp              qword ptr [rcx], 0
                                                                                        jne   8f
                        mov              dword ptr [rbp + 88], eax
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
                        mov              rax, [rbp + 104]
                        mov              rbp, [rbp + 120]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$3_ω:
                        mov              rax, [rbp + 112]
                        lea              rsp, [rbp + 128]
                        mov              rbp, [rbp + 120]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$4_α
proc_PAT$4_α:
                        .global          proc_PAT$4_α
                        .global          proc_PAT$4_β
                        .global          proc_PAT$4_γ
                        .global          proc_PAT$4_ω
                        sub              rsp, 128
                        mov              [rsp + 104], rcx
                        mov              [rsp + 112], rdx
                        mov              [rsp + 120], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 48], 0
                        mov              qword ptr [rsp + 56], 0
                        mov              qword ptr [rsp + 80], 0
                        mov              qword ptr [rsp + 88], 0
                        mov              qword ptr [rsp + 96], r8
                        mov              dword ptr [rsp + 88], r14d
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
                        cmp              qword ptr [rbp + 96], 1
                                                                                        jne   7f
                        mov              ecx, dword ptr [rbp + 88]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx], ecx
7:
                                                                                        jmp   proc_PAT$4_γ
proc_PAT$4_scanfail:
                        cmp              qword ptr [rbp + 96], 1
                                                                                        jne   8f
                        mov              eax, dword ptr [rbp + 88]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    8f
                        lea              rcx, [rip + g_anchor]
                        cmp              qword ptr [rcx], 0
                                                                                        jne   8f
                        mov              dword ptr [rbp + 88], eax
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
                        mov              rax, [rbp + 104]
                        mov              rbp, [rbp + 120]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$4_ω:
                        mov              rax, [rbp + 112]
                        lea              rsp, [rbp + 128]
                        mov              rbp, [rbp + 120]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$5_α
proc_PAT$5_α:
                        .global          proc_PAT$5_α
                        .global          proc_PAT$5_β
                        .global          proc_PAT$5_γ
                        .global          proc_PAT$5_ω
                        sub              rsp, 192
                        mov              [rsp + 168], rcx
                        mov              [rsp + 176], rdx
                        mov              [rsp + 184], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 48], 0
                        mov              qword ptr [rsp + 56], 0
                        mov              qword ptr [rsp + 144], 0
                        mov              qword ptr [rsp + 152], 0
                        mov              qword ptr [rsp + 160], r8
                        mov              dword ptr [rsp + 152], r14d
proc_PAT$5_attempt:
proc_PAT$5_α_body:
                        lea              rax, [rip + n307_match_assign_cond_β]
                        mov              qword ptr [rbp + 144], rax
#-----------------------------------------------------------------------------------------------------------------------
n305_match_assign_save_α:
                        lea              rdi, [rbp + 48]
                        mov              esi, r14d
                        call             rt_cap_push@PLT
                                                                                        jmp   n306_match_alternate_α
n305_match_assign_save_β:
                        lea              rdi, [rbp + 48]
                        call             rt_cap_pop@PLT
                                                                                        jmp   proc_PAT$5_scanfail
#-----------------------------------------------------------------------------------------------------------------------
n306_match_alternate_α:
                        mov              dword ptr [rbp + 80], r14d
                        lea              rax, [rip + .Lx313_21]
                        mov              qword ptr [rbp + 96], rax
                                                                                        jmp   n308_match_defer_α
.Lx313_21:
                        lea              rax, [rip + .Lx313_19]
                        mov              qword ptr [rbp + 96], rax
                                                                                        jmp   n309_match_defer_α
n306_match_alternate_s0:
                        lea              rax, [rip + .Lx313_40]
                        mov              qword ptr [rbp + 88], rax
                                                                                        jmp   n306_match_alternate_as
n306_match_alternate_s1:
                        lea              rax, [rip + .Lx313_41]
                        mov              qword ptr [rbp + 88], rax
                                                                                        jmp   n306_match_alternate_as
.Lx313_40:
                                                                                        jmp   n308_match_defer_β
.Lx313_41:
                                                                                        jmp   n309_match_defer_β
n306_match_alternate_as:
                                                                                        jmp   n307_match_assign_cond_α
n306_match_alternate_β:
                        mov              rax, qword ptr [rbp + 88]
                                                                                        jmp   rax
n306_match_alternate_af:
                        mov              r14d, dword ptr [rbp + 80]
                        mov              rax, qword ptr [rbp + 96]
                                                                                        jmp   rax
.Lx313_19:
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
n308_match_defer_α:
                        mov              rax, qword ptr [1879052496]
                        mov              rdx, qword ptr [1879052504]
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
                                                                                        jmp   n306_match_alternate_s0
.Lx316_5:
                                                                                        jmp   n306_match_alternate_af
.Lx316_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S4]
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
                                                                                        jmp   n306_match_alternate_s0
.Lx316_6:
                        add              rsp, 16
                                                                                        jmp   n306_match_alternate_af
n308_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n309_match_defer_α:
                        mov              rax, qword ptr [1879052464]
                        mov              rdx, qword ptr [1879052472]
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
                                                                                        jmp   n306_match_alternate_s1
.Lx317_5:
                                                                                        jmp   n306_match_alternate_af
.Lx317_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S0]
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
                                                                                        jmp   n306_match_alternate_s1
.Lx317_6:
                        add              rsp, 16
                                                                                        jmp   n306_match_alternate_af
n309_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
proc_PAT$5_scanhit:
                        cmp              qword ptr [rbp + 160], 1
                                                                                        jne   7f
                        mov              ecx, dword ptr [rbp + 152]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx], ecx
7:
                                                                                        jmp   proc_PAT$5_γ
proc_PAT$5_scanfail:
                        cmp              qword ptr [rbp + 160], 1
                                                                                        jne   8f
                        mov              eax, dword ptr [rbp + 152]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    8f
                        lea              rcx, [rip + g_anchor]
                        cmp              qword ptr [rcx], 0
                                                                                        jne   8f
                        mov              dword ptr [rbp + 152], eax
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
                                                                                        jmp   qword ptr [rbp + 144]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$5_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$5_res]
                        push             rax
                        mov              rax, [rbp + 168]
                        mov              rbp, [rbp + 184]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$5_ω:
                        mov              rax, [rbp + 176]
                        lea              rsp, [rbp + 192]
                        mov              rbp, [rbp + 184]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$6_α
proc_PAT$6_α:
                        .global          proc_PAT$6_α
                        .global          proc_PAT$6_β
                        .global          proc_PAT$6_γ
                        .global          proc_PAT$6_ω
                        sub              rsp, 160
                        mov              [rsp + 136], rcx
                        mov              [rsp + 144], rdx
                        mov              [rsp + 152], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 112], 0
                        mov              qword ptr [rsp + 120], 0
                        mov              qword ptr [rsp + 128], r8
                        mov              dword ptr [rsp + 120], r14d
proc_PAT$6_attempt:
proc_PAT$6_α_body:
                        lea              rax, [rip + n318_match_alternate_β]
                        mov              qword ptr [rbp + 112], rax
#-----------------------------------------------------------------------------------------------------------------------
n318_match_alternate_α:
                        mov              dword ptr [rbp + 16], r14d
                        lea              rax, [rip + .Lx325_21]
                        mov              qword ptr [rbp + 32], rax
                                                                                        jmp   n319_match_defer_α
.Lx325_21:
                        lea              rax, [rip + .Lx325_19]
                        mov              qword ptr [rbp + 32], rax
                                                                                        jmp   n320_match_sequence_α
n318_match_alternate_s0:
                        lea              rax, [rip + .Lx325_40]
                        mov              qword ptr [rbp + 24], rax
                                                                                        jmp   n318_match_alternate_as
n318_match_alternate_s1:
                        lea              rax, [rip + .Lx325_41]
                        mov              qword ptr [rbp + 24], rax
                                                                                        jmp   n318_match_alternate_as
.Lx325_40:
                                                                                        jmp   n319_match_defer_β
.Lx325_41:
                                                                                        jmp   n320_match_sequence_β
n318_match_alternate_as:
                                                                                        jmp   proc_PAT$6_γ
n318_match_alternate_β:
                        mov              rax, qword ptr [rbp + 24]
                                                                                        jmp   rax
n318_match_alternate_af:
                        mov              r14d, dword ptr [rbp + 16]
                        mov              rax, qword ptr [rbp + 32]
                                                                                        jmp   rax
.Lx325_19:
                                                                                        jmp   proc_PAT$6_ω
#-----------------------------------------------------------------------------------------------------------------------
n319_match_defer_α:
                        mov              rax, qword ptr [1879052544]
                        mov              rdx, qword ptr [1879052552]
                        cmp              eax, 3
                                                                                        jne   .Lx326_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx326_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx326_10
.Lx326_9:
                        xor              eax, eax
.Lx326_10:
                        test             rax, rax
                                                                                        jz    .Lx326_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx326_4]
                        lea              rdx, [rip + .Lx326_5]
                                                                                        jmp   rax
.Lx326_4:
                                                                                        jmp   n318_match_alternate_s0
.Lx326_5:
                                                                                        jmp   n318_match_alternate_af
.Lx326_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S5]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx326_2:
                        test             rax, rax
                                                                                        je    .Lx326_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx326_7]
                        lea              rdx, [rip + .Lx326_8]
                                                                                        jmp   rax
.Lx326_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx326_2
.Lx326_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx326_2
.Lx326_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n318_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx326_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n318_match_alternate_s0
.Lx326_6:
                        add              rsp, 16
                                                                                        jmp   n318_match_alternate_af
n319_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n320_match_sequence_α:
                        mov              dword ptr [rbp + 64], r14d
                                                                                        jmp   n321_match_lit_α
n320_match_sequence_as:
                                                                                        jmp   n318_match_alternate_s1
n320_match_sequence_β:
                                                                                        jmp   n323_match_lit_β
n320_match_sequence_af:
                                                                                        jmp   n318_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n321_match_lit_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jg    n318_match_alternate_af
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 40
                                                                                        jne   n318_match_alternate_af
                        add              r14d, 1
                                                                                        jmp   n322_match_defer_α
n321_match_lit_β:
                        sub              r14d, 1
                                                                                        jmp   n318_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n322_match_defer_α:
                        mov              rax, qword ptr [1879052608]
                        mov              rdx, qword ptr [1879052616]
                        cmp              eax, 3
                                                                                        jne   .Lx331_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx331_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx331_10
.Lx331_9:
                        xor              eax, eax
.Lx331_10:
                        test             rax, rax
                                                                                        jz    .Lx331_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx331_4]
                        lea              rdx, [rip + .Lx331_5]
                                                                                        jmp   rax
.Lx331_4:
                                                                                        jmp   n323_match_lit_α
.Lx331_5:
                                                                                        jmp   n321_match_lit_β
.Lx331_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S6]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx331_2:
                        test             rax, rax
                                                                                        je    .Lx331_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx331_7]
                        lea              rdx, [rip + .Lx331_8]
                                                                                        jmp   rax
.Lx331_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx331_2
.Lx331_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx331_2
.Lx331_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n321_match_lit_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx331_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n323_match_lit_α
.Lx331_6:
                        add              rsp, 16
                                                                                        jmp   n321_match_lit_β
n322_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n323_match_lit_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jg    n322_match_defer_β
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 41
                                                                                        jne   n322_match_defer_β
                        add              r14d, 1
                                                                                        jmp   n318_match_alternate_s1
n323_match_lit_β:
                        sub              r14d, 1
                                                                                        jmp   n322_match_defer_β
proc_PAT$6_scanhit:
                        cmp              qword ptr [rbp + 128], 1
                                                                                        jne   7f
                        mov              ecx, dword ptr [rbp + 120]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx], ecx
7:
                                                                                        jmp   proc_PAT$6_γ
proc_PAT$6_scanfail:
                        cmp              qword ptr [rbp + 128], 1
                                                                                        jne   8f
                        mov              eax, dword ptr [rbp + 120]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    8f
                        lea              rcx, [rip + g_anchor]
                        cmp              qword ptr [rcx], 0
                                                                                        jne   8f
                        mov              dword ptr [rbp + 120], eax
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
                        mov              rax, [rbp + 136]
                        mov              rbp, [rbp + 152]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$6_ω:
                        mov              rax, [rbp + 144]
                        lea              rsp, [rbp + 160]
                        mov              rbp, [rbp + 152]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$7_α
proc_PAT$7_α:
                        .global          proc_PAT$7_α
                        .global          proc_PAT$7_β
                        .global          proc_PAT$7_γ
                        .global          proc_PAT$7_ω
                        sub              rsp, 224
                        mov              [rsp + 200], rcx
                        mov              [rsp + 208], rdx
                        mov              [rsp + 216], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 112], 0
                        mov              qword ptr [rsp + 120], 0
                        mov              qword ptr [rsp + 176], 0
                        mov              qword ptr [rsp + 184], 0
                        mov              qword ptr [rsp + 192], r8
                        mov              dword ptr [rsp + 184], r14d
proc_PAT$7_attempt:
proc_PAT$7_α_body:
                        lea              rax, [rip + n334_match_alternate_β]
                        mov              qword ptr [rbp + 176], rax
#-----------------------------------------------------------------------------------------------------------------------
n334_match_alternate_α:
                        mov              dword ptr [rbp + 16], r14d
                        lea              rax, [rip + .Lx342_21]
                        mov              qword ptr [rbp + 32], rax
                                                                                        jmp   n335_match_sequence_α
.Lx342_21:
                        lea              rax, [rip + .Lx342_19]
                        mov              qword ptr [rbp + 32], rax
                                                                                        jmp   n336_match_defer_α
n334_match_alternate_s0:
                        lea              rax, [rip + .Lx342_40]
                        mov              qword ptr [rbp + 24], rax
                                                                                        jmp   n334_match_alternate_as
n334_match_alternate_s1:
                        lea              rax, [rip + .Lx342_41]
                        mov              qword ptr [rbp + 24], rax
                                                                                        jmp   n334_match_alternate_as
.Lx342_40:
                                                                                        jmp   n335_match_sequence_β
.Lx342_41:
                                                                                        jmp   n336_match_defer_β
n334_match_alternate_as:
                                                                                        jmp   proc_PAT$7_γ
n334_match_alternate_β:
                        mov              rax, qword ptr [rbp + 24]
                                                                                        jmp   rax
n334_match_alternate_af:
                        mov              r14d, dword ptr [rbp + 16]
                        mov              rax, qword ptr [rbp + 32]
                                                                                        jmp   rax
.Lx342_19:
                                                                                        jmp   proc_PAT$7_ω
#-----------------------------------------------------------------------------------------------------------------------
n335_match_sequence_α:
                        mov              dword ptr [rbp + 48], r14d
                                                                                        jmp   n337_match_defer_α
n335_match_sequence_as:
                                                                                        jmp   n334_match_alternate_s0
n335_match_sequence_β:
                                                                                        jmp   n339_match_assign_cond_β
n335_match_sequence_af:
                                                                                        jmp   n334_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n336_match_defer_α:
                        mov              rax, qword ptr [1879052560]
                        mov              rdx, qword ptr [1879052568]
                        cmp              eax, 3
                                                                                        jne   .Lx345_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx345_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx345_10
.Lx345_9:
                        xor              eax, eax
.Lx345_10:
                        test             rax, rax
                                                                                        jz    .Lx345_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx345_4]
                        lea              rdx, [rip + .Lx345_5]
                                                                                        jmp   rax
.Lx345_4:
                                                                                        jmp   n334_match_alternate_s1
.Lx345_5:
                                                                                        jmp   n334_match_alternate_af
.Lx345_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S7]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx345_2:
                        test             rax, rax
                                                                                        je    .Lx345_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx345_7]
                        lea              rdx, [rip + .Lx345_8]
                                                                                        jmp   rax
.Lx345_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx345_2
.Lx345_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx345_2
.Lx345_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n334_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx345_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n334_match_alternate_s1
.Lx345_6:
                        add              rsp, 16
                                                                                        jmp   n334_match_alternate_af
n336_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n337_match_defer_α:
                        mov              rax, qword ptr [1879052512]
                        mov              rdx, qword ptr [1879052520]
                        cmp              eax, 3
                                                                                        jne   .Lx346_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx346_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx346_10
.Lx346_9:
                        xor              eax, eax
.Lx346_10:
                        test             rax, rax
                                                                                        jz    .Lx346_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx346_4]
                        lea              rdx, [rip + .Lx346_5]
                                                                                        jmp   rax
.Lx346_4:
                                                                                        jmp   n338_match_assign_save_α
.Lx346_5:
                                                                                        jmp   n334_match_alternate_af
.Lx346_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S8]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx346_2:
                        test             rax, rax
                                                                                        je    .Lx346_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx346_7]
                        lea              rdx, [rip + .Lx346_8]
                                                                                        jmp   rax
.Lx346_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx346_2
.Lx346_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx346_2
.Lx346_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n334_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx346_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n338_match_assign_save_α
.Lx346_6:
                        add              rsp, 16
                                                                                        jmp   n334_match_alternate_af
n337_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n338_match_assign_save_α:
                        lea              rdi, [rbp + 112]
                        mov              esi, r14d
                        call             rt_cap_push@PLT
                                                                                        jmp   n340_match_defer_α
n338_match_assign_save_β:
                        lea              rdi, [rbp + 112]
                        call             rt_cap_pop@PLT
                                                                                        jmp   n337_match_defer_β
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
                                                                                        jmp   n340_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n340_match_defer_α:
                        mov              rax, qword ptr [1879052576]
                        mov              rdx, qword ptr [1879052584]
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
                                                                                        jmp   n339_match_assign_cond_α
.Lx351_5:
                                                                                        jmp   n338_match_assign_save_β
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
                                                                                        js    n338_match_assign_save_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx351_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n339_match_assign_cond_α
.Lx351_6:
                        add              rsp, 16
                                                                                        jmp   n338_match_assign_save_β
n340_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
proc_PAT$7_scanhit:
                        cmp              qword ptr [rbp + 192], 1
                                                                                        jne   7f
                        mov              ecx, dword ptr [rbp + 184]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx], ecx
7:
                                                                                        jmp   proc_PAT$7_γ
proc_PAT$7_scanfail:
                        cmp              qword ptr [rbp + 192], 1
                                                                                        jne   8f
                        mov              eax, dword ptr [rbp + 184]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    8f
                        lea              rcx, [rip + g_anchor]
                        cmp              qword ptr [rcx], 0
                                                                                        jne   8f
                        mov              dword ptr [rbp + 184], eax
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
                                                                                        jmp   qword ptr [rbp + 176]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$7_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$7_res]
                        push             rax
                        mov              rax, [rbp + 200]
                        mov              rbp, [rbp + 216]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$7_ω:
                        mov              rax, [rbp + 208]
                        lea              rsp, [rbp + 224]
                        mov              rbp, [rbp + 216]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$8_α
proc_PAT$8_α:
                        .global          proc_PAT$8_α
                        .global          proc_PAT$8_β
                        .global          proc_PAT$8_γ
                        .global          proc_PAT$8_ω
                        sub              rsp, 240
                        mov              [rsp + 216], rcx
                        mov              [rsp + 224], rdx
                        mov              [rsp + 232], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 128], 0
                        mov              qword ptr [rsp + 136], 0
                        mov              qword ptr [rsp + 192], 0
                        mov              qword ptr [rsp + 200], 0
                        mov              qword ptr [rsp + 208], r8
                        mov              dword ptr [rsp + 200], r14d
proc_PAT$8_attempt:
proc_PAT$8_α_body:
                        lea              rax, [rip + n352_match_alternate_β]
                        mov              qword ptr [rbp + 192], rax
#-----------------------------------------------------------------------------------------------------------------------
n352_match_alternate_α:
                        mov              dword ptr [rbp + 16], r14d
                        lea              rax, [rip + .Lx361_21]
                        mov              qword ptr [rbp + 32], rax
                                                                                        jmp   n353_match_sequence_α
.Lx361_21:
                        lea              rax, [rip + .Lx361_19]
                        mov              qword ptr [rbp + 32], rax
                                                                                        jmp   n354_match_defer_α
n352_match_alternate_s0:
                        lea              rax, [rip + .Lx361_40]
                        mov              qword ptr [rbp + 24], rax
                                                                                        jmp   n352_match_alternate_as
n352_match_alternate_s1:
                        lea              rax, [rip + .Lx361_41]
                        mov              qword ptr [rbp + 24], rax
                                                                                        jmp   n352_match_alternate_as
.Lx361_40:
                                                                                        jmp   n353_match_sequence_β
.Lx361_41:
                                                                                        jmp   n354_match_defer_β
n352_match_alternate_as:
                                                                                        jmp   proc_PAT$8_γ
n352_match_alternate_β:
                        mov              rax, qword ptr [rbp + 24]
                                                                                        jmp   rax
n352_match_alternate_af:
                        mov              r14d, dword ptr [rbp + 16]
                        mov              rax, qword ptr [rbp + 32]
                                                                                        jmp   rax
.Lx361_19:
                                                                                        jmp   proc_PAT$8_ω
#-----------------------------------------------------------------------------------------------------------------------
n353_match_sequence_α:
                        mov              dword ptr [rbp + 48], r14d
                                                                                        jmp   n355_match_defer_α
n353_match_sequence_as:
                                                                                        jmp   n352_match_alternate_s0
n353_match_sequence_β:
                                                                                        jmp   n358_match_assign_cond_β
n353_match_sequence_af:
                                                                                        jmp   n352_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n354_match_defer_α:
                        mov              rax, qword ptr [1879052576]
                        mov              rdx, qword ptr [1879052584]
                        cmp              eax, 3
                                                                                        jne   .Lx364_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx364_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx364_10
.Lx364_9:
                        xor              eax, eax
.Lx364_10:
                        test             rax, rax
                                                                                        jz    .Lx364_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx364_4]
                        lea              rdx, [rip + .Lx364_5]
                                                                                        jmp   rax
.Lx364_4:
                                                                                        jmp   n352_match_alternate_s1
.Lx364_5:
                                                                                        jmp   n352_match_alternate_af
.Lx364_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S10]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx364_2:
                        test             rax, rax
                                                                                        je    .Lx364_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx364_7]
                        lea              rdx, [rip + .Lx364_8]
                                                                                        jmp   rax
.Lx364_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx364_2
.Lx364_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx364_2
.Lx364_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n352_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx364_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n352_match_alternate_s1
.Lx364_6:
                        add              rsp, 16
                                                                                        jmp   n352_match_alternate_af
n354_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n355_match_defer_α:
                        mov              rax, qword ptr [1879052576]
                        mov              rdx, qword ptr [1879052584]
                        cmp              eax, 3
                                                                                        jne   .Lx365_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx365_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx365_10
.Lx365_9:
                        xor              eax, eax
.Lx365_10:
                        test             rax, rax
                                                                                        jz    .Lx365_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx365_4]
                        lea              rdx, [rip + .Lx365_5]
                                                                                        jmp   rax
.Lx365_4:
                                                                                        jmp   n356_match_defer_α
.Lx365_5:
                                                                                        jmp   n352_match_alternate_af
.Lx365_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S10]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx365_2:
                        test             rax, rax
                                                                                        je    .Lx365_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx365_7]
                        lea              rdx, [rip + .Lx365_8]
                                                                                        jmp   rax
.Lx365_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx365_2
.Lx365_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx365_2
.Lx365_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n352_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx365_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n356_match_defer_α
.Lx365_6:
                        add              rsp, 16
                                                                                        jmp   n352_match_alternate_af
n355_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n356_match_defer_α:
                        mov              rax, qword ptr [1879052528]
                        mov              rdx, qword ptr [1879052536]
                        cmp              eax, 3
                                                                                        jne   .Lx366_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx366_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx366_10
.Lx366_9:
                        xor              eax, eax
.Lx366_10:
                        test             rax, rax
                                                                                        jz    .Lx366_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx366_4]
                        lea              rdx, [rip + .Lx366_5]
                                                                                        jmp   rax
.Lx366_4:
                                                                                        jmp   n357_match_assign_save_α
.Lx366_5:
                                                                                        jmp   n355_match_defer_β
.Lx366_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S11]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx366_2:
                        test             rax, rax
                                                                                        je    .Lx366_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx366_7]
                        lea              rdx, [rip + .Lx366_8]
                                                                                        jmp   rax
.Lx366_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx366_2
.Lx366_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx366_2
.Lx366_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n355_match_defer_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx366_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n357_match_assign_save_α
.Lx366_6:
                        add              rsp, 16
                                                                                        jmp   n355_match_defer_β
n356_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n357_match_assign_save_α:
                        lea              rdi, [rbp + 128]
                        mov              esi, r14d
                        call             rt_cap_push@PLT
                                                                                        jmp   n359_match_defer_α
n357_match_assign_save_β:
                        lea              rdi, [rbp + 128]
                        call             rt_cap_pop@PLT
                                                                                        jmp   n356_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n358_match_assign_cond_α:
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
n358_match_assign_cond_β:
                        mov              rax, qword ptr [1879048192]
                        sub              rax, 24
                        mov              qword ptr [1879048192], rax
                                                                                        jmp   n359_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n359_match_defer_α:
                        mov              rax, qword ptr [1879052592]
                        mov              rdx, qword ptr [1879052600]
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
                                                                                        jmp   n358_match_assign_cond_α
.Lx371_5:
                                                                                        jmp   n357_match_assign_save_β
.Lx371_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S13]
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
                                                                                        js    n357_match_assign_save_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx371_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n358_match_assign_cond_α
.Lx371_6:
                        add              rsp, 16
                                                                                        jmp   n357_match_assign_save_β
n359_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
proc_PAT$8_scanhit:
                        cmp              qword ptr [rbp + 208], 1
                                                                                        jne   7f
                        mov              ecx, dword ptr [rbp + 200]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx], ecx
7:
                                                                                        jmp   proc_PAT$8_γ
proc_PAT$8_scanfail:
                        cmp              qword ptr [rbp + 208], 1
                                                                                        jne   8f
                        mov              eax, dword ptr [rbp + 200]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    8f
                        lea              rcx, [rip + g_anchor]
                        cmp              qword ptr [rcx], 0
                                                                                        jne   8f
                        mov              dword ptr [rbp + 200], eax
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
                                                                                        jmp   qword ptr [rbp + 192]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$8_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$8_res]
                        push             rax
                        mov              rax, [rbp + 216]
                        mov              rbp, [rbp + 232]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$8_ω:
                        mov              rax, [rbp + 224]
                        lea              rsp, [rbp + 240]
                        mov              rbp, [rbp + 232]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$9_α
proc_PAT$9_α:
                        .global          proc_PAT$9_α
                        .global          proc_PAT$9_β
                        .global          proc_PAT$9_γ
                        .global          proc_PAT$9_ω
                        sub              rsp, 240
                        mov              [rsp + 216], rcx
                        mov              [rsp + 224], rdx
                        mov              [rsp + 232], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 128], 0
                        mov              qword ptr [rsp + 136], 0
                        mov              qword ptr [rsp + 192], 0
                        mov              qword ptr [rsp + 200], 0
                        mov              qword ptr [rsp + 208], r8
                        mov              dword ptr [rsp + 200], r14d
proc_PAT$9_attempt:
proc_PAT$9_α_body:
                        lea              rax, [rip + n372_match_alternate_β]
                        mov              qword ptr [rbp + 192], rax
#-----------------------------------------------------------------------------------------------------------------------
n372_match_alternate_α:
                        mov              dword ptr [rbp + 16], r14d
                        lea              rax, [rip + .Lx381_21]
                        mov              qword ptr [rbp + 32], rax
                                                                                        jmp   n373_match_sequence_α
.Lx381_21:
                        lea              rax, [rip + .Lx381_19]
                        mov              qword ptr [rbp + 32], rax
                                                                                        jmp   n374_match_defer_α
n372_match_alternate_s0:
                        lea              rax, [rip + .Lx381_40]
                        mov              qword ptr [rbp + 24], rax
                                                                                        jmp   n372_match_alternate_as
n372_match_alternate_s1:
                        lea              rax, [rip + .Lx381_41]
                        mov              qword ptr [rbp + 24], rax
                                                                                        jmp   n372_match_alternate_as
.Lx381_40:
                                                                                        jmp   n373_match_sequence_β
.Lx381_41:
                                                                                        jmp   n374_match_defer_β
n372_match_alternate_as:
                                                                                        jmp   proc_PAT$9_γ
n372_match_alternate_β:
                        mov              rax, qword ptr [rbp + 24]
                                                                                        jmp   rax
n372_match_alternate_af:
                        mov              r14d, dword ptr [rbp + 16]
                        mov              rax, qword ptr [rbp + 32]
                                                                                        jmp   rax
.Lx381_19:
                                                                                        jmp   proc_PAT$9_ω
#-----------------------------------------------------------------------------------------------------------------------
n373_match_sequence_α:
                        mov              dword ptr [rbp + 48], r14d
                                                                                        jmp   n375_match_defer_α
n373_match_sequence_as:
                                                                                        jmp   n372_match_alternate_s0
n373_match_sequence_β:
                                                                                        jmp   n378_match_assign_cond_β
n373_match_sequence_af:
                                                                                        jmp   n372_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n374_match_defer_α:
                        mov              rax, qword ptr [1879052592]
                        mov              rdx, qword ptr [1879052600]
                        cmp              eax, 3
                                                                                        jne   .Lx384_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx384_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx384_10
.Lx384_9:
                        xor              eax, eax
.Lx384_10:
                        test             rax, rax
                                                                                        jz    .Lx384_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx384_4]
                        lea              rdx, [rip + .Lx384_5]
                                                                                        jmp   rax
.Lx384_4:
                                                                                        jmp   n372_match_alternate_s1
.Lx384_5:
                                                                                        jmp   n372_match_alternate_af
.Lx384_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S13]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx384_2:
                        test             rax, rax
                                                                                        je    .Lx384_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx384_7]
                        lea              rdx, [rip + .Lx384_8]
                                                                                        jmp   rax
.Lx384_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx384_2
.Lx384_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx384_2
.Lx384_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n372_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx384_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n372_match_alternate_s1
.Lx384_6:
                        add              rsp, 16
                                                                                        jmp   n372_match_alternate_af
n374_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n375_match_defer_α:
                        mov              rax, qword ptr [1879052592]
                        mov              rdx, qword ptr [1879052600]
                        cmp              eax, 3
                                                                                        jne   .Lx385_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx385_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx385_10
.Lx385_9:
                        xor              eax, eax
.Lx385_10:
                        test             rax, rax
                                                                                        jz    .Lx385_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx385_4]
                        lea              rdx, [rip + .Lx385_5]
                                                                                        jmp   rax
.Lx385_4:
                                                                                        jmp   n376_match_defer_α
.Lx385_5:
                                                                                        jmp   n372_match_alternate_af
.Lx385_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S13]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx385_2:
                        test             rax, rax
                                                                                        je    .Lx385_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx385_7]
                        lea              rdx, [rip + .Lx385_8]
                                                                                        jmp   rax
.Lx385_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx385_2
.Lx385_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx385_2
.Lx385_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n372_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx385_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n376_match_defer_α
.Lx385_6:
                        add              rsp, 16
                                                                                        jmp   n372_match_alternate_af
n375_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n376_match_defer_α:
                        mov              rax, qword ptr [1879052512]
                        mov              rdx, qword ptr [1879052520]
                        cmp              eax, 3
                                                                                        jne   .Lx386_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx386_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx386_10
.Lx386_9:
                        xor              eax, eax
.Lx386_10:
                        test             rax, rax
                                                                                        jz    .Lx386_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx386_4]
                        lea              rdx, [rip + .Lx386_5]
                                                                                        jmp   rax
.Lx386_4:
                                                                                        jmp   n377_match_assign_save_α
.Lx386_5:
                                                                                        jmp   n375_match_defer_β
.Lx386_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S8]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx386_2:
                        test             rax, rax
                                                                                        je    .Lx386_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx386_7]
                        lea              rdx, [rip + .Lx386_8]
                                                                                        jmp   rax
.Lx386_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx386_2
.Lx386_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx386_2
.Lx386_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n375_match_defer_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx386_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n377_match_assign_save_α
.Lx386_6:
                        add              rsp, 16
                                                                                        jmp   n375_match_defer_β
n376_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n377_match_assign_save_α:
                        lea              rdi, [rbp + 128]
                        mov              esi, r14d
                        call             rt_cap_push@PLT
                                                                                        jmp   n379_match_defer_α
n377_match_assign_save_β:
                        lea              rdi, [rbp + 128]
                        call             rt_cap_pop@PLT
                                                                                        jmp   n376_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n378_match_assign_cond_α:
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
n378_match_assign_cond_β:
                        mov              rax, qword ptr [1879048192]
                        sub              rax, 24
                        mov              qword ptr [1879048192], rax
                                                                                        jmp   n379_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n379_match_defer_α:
                        mov              rax, qword ptr [1879052608]
                        mov              rdx, qword ptr [1879052616]
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
                                                                                        jmp   n378_match_assign_cond_α
.Lx391_5:
                                                                                        jmp   n377_match_assign_save_β
.Lx391_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S6]
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
                                                                                        js    n377_match_assign_save_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx391_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n378_match_assign_cond_α
.Lx391_6:
                        add              rsp, 16
                                                                                        jmp   n377_match_assign_save_β
n379_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
proc_PAT$9_scanhit:
                        cmp              qword ptr [rbp + 208], 1
                                                                                        jne   7f
                        mov              ecx, dword ptr [rbp + 200]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx], ecx
7:
                                                                                        jmp   proc_PAT$9_γ
proc_PAT$9_scanfail:
                        cmp              qword ptr [rbp + 208], 1
                                                                                        jne   8f
                        mov              eax, dword ptr [rbp + 200]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    8f
                        lea              rcx, [rip + g_anchor]
                        cmp              qword ptr [rcx], 0
                                                                                        jne   8f
                        mov              dword ptr [rbp + 200], eax
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
                                                                                        jmp   qword ptr [rbp + 192]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$9_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$9_res]
                        push             rax
                        mov              rax, [rbp + 216]
                        mov              rbp, [rbp + 232]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$9_ω:
                        mov              rax, [rbp + 224]
                        lea              rsp, [rbp + 240]
                        mov              rbp, [rbp + 232]
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
                        mov              esi, 2512
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
                        mov              esi, 2512
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
                        mov              esi, 2512
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
                        mov              esi, 2512
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
                        mov              esi, 160
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
                        mov              esi, 192
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
                        mov              esi, 208
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
                        mov              esi, 208
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
                        sub              rsp, 2520
                        mov              rdi, rsp
                        mov              ecx, 2520
                        xor              eax, eax
                        rep stosb
main_α_body:
#=======================================================================================================================
#          DEFINE('Push(x)')
#          stk      =  TABLE()                       :(PushEnd)
#-----------------------------------------------------------------------------------------------------------------------
n392_call_α:
                        .section         .rodata
.Lrkfn534:              .string          "TABLE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn534]
                        lea              rsi, [rsp + 128]
                        mov              edx, 0
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 112], rax
                        mov              qword ptr [rsp + 120], rdx
                        cmp              eax, 99
                                                                                        je    n404_lit_string_α
                                                                                        jmp   n403_assign_α
n392_call_β:
                                                                                        jmp   n404_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n393_goto_α:
                                                                                        jmp   n405_var_α
n393_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n394_goto_α:
                                                                                        jmp   n404_lit_string_α
n394_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n395_goto_α:
                                                                                        jmp   n406_var_α
n395_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n396_goto_α:
                                                                                        jmp   n404_lit_string_α
n396_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n397_goto_α:
                                                                                        jmp   n407_call_α
n397_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n398_goto_α:
                                                                                        jmp   n404_lit_string_α
n398_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n399_goto_α:
                                                                                        jmp   n408_call_α
n399_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n400_goto_α:
                                                                                        jmp   n404_lit_string_α
n400_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n401_goto_α:
                                                                                        jmp   n409_var_α
n401_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n402_goto_α:
                                                                                        jmp   n410_lit_string_α
n402_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n403_assign_α:
                        mov              rax, qword ptr [rsp + 112]
                        mov              rdx, qword ptr [rsp + 120]
                        mov              qword ptr [1879052432], rax
                        mov              qword ptr [1879052440], rdx
                                                                                        jmp   n404_lit_string_α
#=======================================================================================================================
#          DEFINE('Pop()')                           :(PopEnd)
#          DEFINE('Unary()arg,op')                   :(UnaryEnd)
#          DEFINE('Binary()op,left,right')           :(BinaryEnd)
#          integer  =  SPAN('0123456789')
#-----------------------------------------------------------------------------------------------------------------------
n404_lit_string_α:
                        mov              qword ptr [rsp + 1616], 1
                        mov              rax, qword ptr [rip + .Lx546_0]
                        mov              qword ptr [rsp + 1624], rax
                                                                                        jmp   n411_call_α
.Lx546_0:
                        .quad            .Lx546_0_s
.Lx546_0_s:
                        .string          "PAT$0"
#=======================================================================================================================
# Push     stk[0]   =  stk[0] + 1
#-----------------------------------------------------------------------------------------------------------------------
n405_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 144], rax
                        mov              qword ptr [rsp + 152], rdx
                                                                                        jmp   n412_lit_integer_α
#=======================================================================================================================
# Pop      Pop      =  stk[stk[0]]
#-----------------------------------------------------------------------------------------------------------------------
n406_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 496], rax
                        mov              qword ptr [rsp + 504], rdx
                                                                                        jmp   n414_var_α
#=======================================================================================================================
# Unary    arg      =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n407_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx550_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx550_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx550_6]
                        lea              rdx, [rip + .Lx550_7]
                                                                                        jmp   rax
.Lx550_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx550_2
.Lx550_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx550_2
.Lx550_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx550_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx550_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx550_3]
                        lea              rdx, [rip + .Lx550_4]
                                                                                        jmp   rax
.Lx550_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx550_2
.Lx550_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx550_2
.Lx550_1:
                        call             rt_faildescr@PLT
.Lx550_2:
                        mov              qword ptr [rsp + 768], rax
                        mov              qword ptr [rsp + 776], rdx
                        cmp              eax, 99
                                                                                        je    n417_call_α
                                                                                        jmp   n416_assign_α
n407_call_β:
                                                                                        jmp   n417_call_α
.Lx550_0:
                        .quad            .Lx550_0_s
.Lx550_0_s:
                        .string          "Pop"
#=======================================================================================================================
# Binary   right    =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n408_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx552_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx552_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx552_6]
                        lea              rdx, [rip + .Lx552_7]
                                                                                        jmp   rax
.Lx552_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx552_2
.Lx552_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx552_2
.Lx552_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx552_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx552_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx552_3]
                        lea              rdx, [rip + .Lx552_4]
                                                                                        jmp   rax
.Lx552_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx552_2
.Lx552_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx552_2
.Lx552_1:
                        call             rt_faildescr@PLT
.Lx552_2:
                        mov              qword ptr [rsp + 1104], rax
                        mov              qword ptr [rsp + 1112], rdx
                        cmp              eax, 99
                                                                                        je    n419_call_α
                                                                                        jmp   n418_assign_α
n408_call_β:
                                                                                        jmp   n419_call_α
.Lx552_0:
                        .quad            .Lx552_0_s
.Lx552_0_s:
                        .string          "Pop"
#=======================================================================================================================
# loop     line     =  INPUT                         :F(END)
#-----------------------------------------------------------------------------------------------------------------------
n409_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx553_0]
                        call             NV_GET_fn@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx553_240
                        add              rsp, 16
                                                                                        jmp   main_γ
.Lx553_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n420_assign_α
.Lx553_0:
                        .quad            .Lx553_0_s
.Lx553_0_s:
                        .string          "INPUT"
#=======================================================================================================================
# error    OUTPUT   = 'Bad input, try again'         :(loop)
#-----------------------------------------------------------------------------------------------------------------------
n410_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              rax, qword ptr [rip + .Lx554_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n421_assign_α
.Lx554_0:
                        .quad            .Lx554_0_s
.Lx554_0_s:
                        .string          "Bad input, try again"
#-----------------------------------------------------------------------------------------------------------------------
n411_call_α:
                        mov              rax, qword ptr [rsp + 1616]
                        mov              qword ptr [rsp + 1584], rax
                        mov              rax, qword ptr [rsp + 1624]
                        mov              qword ptr [rsp + 1592], rax
                        .section         .rodata
.Lrkfn556:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn556]
                        lea              rsi, [rsp + 1584]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1568], rax
                        mov              qword ptr [rsp + 1576], rdx
                        cmp              eax, 99
                                                                                        je    n423_lit_string_α
                                                                                        jmp   n422_assign_α
n411_call_β:
                                                                                        jmp   n423_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n412_lit_integer_α:
                        mov              qword ptr [rsp + 160], 6
                        mov              rax, qword ptr [rip + .Lx557_0]
                        mov              qword ptr [rsp + 168], rax
                                                                                        jmp   n424_subscript_α
.Lx557_0:
                        .quad            0
#=======================================================================================================================
#          Push     =  .stk[stk[0]]
#-----------------------------------------------------------------------------------------------------------------------
n413_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 304], rax
                        mov              qword ptr [rsp + 312], rdx
                                                                                        jmp   n425_var_α
#-----------------------------------------------------------------------------------------------------------------------
n414_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 512], rax
                        mov              qword ptr [rsp + 520], rdx
                                                                                        jmp   n427_lit_integer_α
#=======================================================================================================================
#          stk[0]   =  stk[0] - 1                    :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n415_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 608], rax
                        mov              qword ptr [rsp + 616], rdx
                                                                                        jmp   n428_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n416_assign_α:
                        mov              rax, qword ptr [rsp + 768]
                        mov              rdx, qword ptr [rsp + 776]
                        mov              qword ptr [1879052352], rax
                        mov              qword ptr [1879052360], rdx
                                                                                        jmp   n417_call_α
#=======================================================================================================================
#          op       =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n417_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx563_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx563_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx563_6]
                        lea              rdx, [rip + .Lx563_7]
                                                                                        jmp   rax
.Lx563_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx563_2
.Lx563_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx563_2
.Lx563_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx563_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx563_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx563_3]
                        lea              rdx, [rip + .Lx563_4]
                                                                                        jmp   rax
.Lx563_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx563_2
.Lx563_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx563_2
.Lx563_1:
                        call             rt_faildescr@PLT
.Lx563_2:
                        mov              qword ptr [rsp + 800], rax
                        mov              qword ptr [rsp + 808], rdx
                        cmp              eax, 99
                                                                                        je    n431_lit_string_α
                                                                                        jmp   n430_assign_α
n417_call_β:
                                                                                        jmp   n431_lit_string_α
.Lx563_0:
                        .quad            .Lx563_0_s
.Lx563_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n418_assign_α:
                        mov              rax, qword ptr [rsp + 1104]
                        mov              rdx, qword ptr [rsp + 1112]
                        mov              qword ptr [1879052416], rax
                        mov              qword ptr [1879052424], rdx
                                                                                        jmp   n419_call_α
#=======================================================================================================================
#          op       =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n419_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx566_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx566_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx566_6]
                        lea              rdx, [rip + .Lx566_7]
                                                                                        jmp   rax
.Lx566_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx566_2
.Lx566_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx566_2
.Lx566_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx566_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx566_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx566_3]
                        lea              rdx, [rip + .Lx566_4]
                                                                                        jmp   rax
.Lx566_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx566_2
.Lx566_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx566_2
.Lx566_1:
                        call             rt_faildescr@PLT
.Lx566_2:
                        mov              qword ptr [rsp + 1136], rax
                        mov              qword ptr [rsp + 1144], rdx
                        cmp              eax, 99
                                                                                        je    n433_call_α
                                                                                        jmp   n432_assign_α
n419_call_β:
                                                                                        jmp   n433_call_α
.Lx566_0:
                        .quad            .Lx566_0_s
.Lx566_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n420_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052624], rax
                        mov              qword ptr [1879052632], rdx
                                                                                        jmp   n434_var_α
#-----------------------------------------------------------------------------------------------------------------------
n421_assign_α:
                        mov              rsi, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx568_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n409_var_α
.Lx568_0:
                        .quad            .Lx568_0_s
.Lx568_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n422_assign_α:
                        mov              rax, qword ptr [rsp + 1568]
                        mov              rdx, qword ptr [rsp + 1576]
                        mov              qword ptr [1879052464], rax
                        mov              qword ptr [1879052472], rdx
                                                                                        jmp   n423_lit_string_α
#=======================================================================================================================
#          exponent =  ANY('eEdD') (ANY('+-') | epsilon) integer
#-----------------------------------------------------------------------------------------------------------------------
n423_lit_string_α:
                        mov              qword ptr [rsp + 1680], 1
                        mov              rax, qword ptr [rip + .Lx570_0]
                        mov              qword ptr [rsp + 1688], rax
                                                                                        jmp   n435_call_α
.Lx570_0:
                        .quad            .Lx570_0_s
.Lx570_0_s:
                        .string          "PAT$1"
#-----------------------------------------------------------------------------------------------------------------------
n424_subscript_α:
                        mov              rdi, qword ptr [rsp + 144]
                        mov              rsi, qword ptr [rsp + 152]
                        mov              rdx, qword ptr [rsp + 160]
                        mov              rcx, qword ptr [rsp + 168]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n413_var_α
                        mov              qword ptr [rsp + 176], rax
                        mov              qword ptr [rsp + 184], rdx
                                                                                        jmp   n436_var_α
#-----------------------------------------------------------------------------------------------------------------------
n425_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 320], rax
                        mov              qword ptr [rsp + 328], rdx
                                                                                        jmp   n437_lit_integer_α
#=======================================================================================================================
#          $Push    =  x                             :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n426_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 448], rax
                        mov              qword ptr [rsp + 456], rdx
                                                                                        jmp   n438_call_α
#-----------------------------------------------------------------------------------------------------------------------
n427_lit_integer_α:
                        mov              qword ptr [rsp + 528], 6
                        mov              rax, qword ptr [rip + .Lx574_0]
                        mov              qword ptr [rsp + 536], rax
                                                                                        jmp   n440_subscript_α
.Lx574_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n428_lit_integer_α:
                        mov              qword ptr [rsp + 624], 6
                        mov              rax, qword ptr [rip + .Lx575_0]
                        mov              qword ptr [rsp + 632], rax
                                                                                        jmp   n441_subscript_α
.Lx575_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n429_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n430_assign_α:
                        mov              rax, qword ptr [rsp + 800]
                        mov              rdx, qword ptr [rsp + 808]
                        mov              qword ptr [1879052368], rax
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n431_lit_string_α
#=======================================================================================================================
#          Push()   =  EVAL(op arg)
#-----------------------------------------------------------------------------------------------------------------------
n431_lit_string_α:
                        mov              qword ptr [rsp + 832], 1
                        mov              rax, qword ptr [rip + .Lx579_0]
                        mov              qword ptr [rsp + 840], rax
                                                                                        jmp   n442_call_α
.Lx579_0:
                        .quad            .Lx579_0_s
.Lx579_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n432_assign_α:
                        mov              rax, qword ptr [rsp + 1136]
                        mov              rdx, qword ptr [rsp + 1144]
                        mov              qword ptr [1879052368], rax
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n433_call_α
#=======================================================================================================================
#          left     =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n433_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx582_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx582_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx582_6]
                        lea              rdx, [rip + .Lx582_7]
                                                                                        jmp   rax
.Lx582_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx582_2
.Lx582_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx582_2
.Lx582_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx582_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx582_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx582_3]
                        lea              rdx, [rip + .Lx582_4]
                                                                                        jmp   rax
.Lx582_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx582_2
.Lx582_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx582_2
.Lx582_1:
                        call             rt_faildescr@PLT
.Lx582_2:
                        mov              qword ptr [rsp + 1168], rax
                        mov              qword ptr [rsp + 1176], rdx
                        cmp              eax, 99
                                                                                        je    n444_lit_string_α
                                                                                        jmp   n443_assign_α
n433_call_β:
                                                                                        jmp   n444_lit_string_α
.Lx582_0:
                        .quad            .Lx582_0_s
.Lx582_0_s:
                        .string          "Pop"
#=======================================================================================================================
#          line     POS(0) expr RPOS(0)              :F(error)
#-----------------------------------------------------------------------------------------------------------------------
n434_var_α:
                        mov              rax, qword ptr [1879052624]
                        mov              rdx, qword ptr [1879052632]
                        mov              qword ptr [rsp + 2448], rax
                        mov              qword ptr [rsp + 2456], rdx
                                                                                        jmp   n445_match_head_α
#-----------------------------------------------------------------------------------------------------------------------
n435_call_α:
                        mov              rax, qword ptr [rsp + 1680]
                        mov              qword ptr [rsp + 1648], rax
                        mov              rax, qword ptr [rsp + 1688]
                        mov              qword ptr [rsp + 1656], rax
                        .section         .rodata
.Lrkfn585:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn585]
                        lea              rsi, [rsp + 1648]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1632], rax
                        mov              qword ptr [rsp + 1640], rdx
                        cmp              eax, 99
                                                                                        je    n447_lit_string_α
                                                                                        jmp   n446_assign_α
n435_call_β:
                                                                                        jmp   n447_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n436_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 208], rax
                        mov              qword ptr [rsp + 216], rdx
                                                                                        jmp   n448_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n437_lit_integer_α:
                        mov              qword ptr [rsp + 336], 6
                        mov              rax, qword ptr [rip + .Lx587_0]
                        mov              qword ptr [rsp + 344], rax
                                                                                        jmp   n449_subscript_α
.Lx587_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n438_call_α:
                        mov              rax, qword ptr [rsp + 448]
                        mov              qword ptr [rsp + 416], rax
                        mov              rax, qword ptr [rsp + 456]
                        mov              qword ptr [rsp + 424], rax
                        .section         .rodata
.Lrkfn589:              .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn589]
                        lea              rsi, [rsp + 416]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 400], rax
                        mov              qword ptr [rsp + 408], rdx
                        cmp              eax, 99
                                                                                        je    n439_lit_string_α
                                                                                        jmp   n450_var_α
n438_call_β:
                                                                                        jmp   n439_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n439_lit_string_α:
                        mov              qword ptr [rsp + 32], 1
                        mov              rax, qword ptr [rip + .Lx590_0]
                        mov              qword ptr [rsp + 40], rax
                                                                                        jmp   n451_call_α
.Lx590_0:
                        .quad            .Lx590_0_s
.Lx590_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n440_subscript_α:
                        mov              rdi, qword ptr [rsp + 512]
                        mov              rsi, qword ptr [rsp + 520]
                        mov              rdx, qword ptr [rsp + 528]
                        mov              rcx, qword ptr [rsp + 536]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n415_var_α
                        mov              qword ptr [rsp + 544], rax
                        mov              qword ptr [rsp + 552], rdx
                                                                                        jmp   n452_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n441_subscript_α:
                        mov              rdi, qword ptr [rsp + 608]
                        mov              rsi, qword ptr [rsp + 616]
                        mov              rdx, qword ptr [rsp + 624]
                        mov              rcx, qword ptr [rsp + 632]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n429_op14_α
                        mov              qword ptr [rsp + 640], rax
                        mov              qword ptr [rsp + 648], rdx
                                                                                        jmp   n453_var_α
#-----------------------------------------------------------------------------------------------------------------------
n442_call_α:
                        mov              rax, qword ptr [rsp + 832]
                        mov              qword ptr [rsp + 864], rax
                        mov              rax, qword ptr [rsp + 840]
                        mov              qword ptr [rsp + 872], rax
                        .section         .rodata
.Lrkfn594:              .string          "SNO$WANTNM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn594]
                        lea              rsi, [rsp + 864]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 848], rax
                        mov              qword ptr [rsp + 856], rdx
                        cmp              eax, 99
                                                                                        je    n455_lit_string_α
                                                                                        jmp   n454_call_α
n442_call_β:
                                                                                        jmp   n455_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n443_assign_α:
                        mov              rax, qword ptr [rsp + 1168]
                        mov              rdx, qword ptr [rsp + 1176]
                        mov              qword ptr [1879052400], rax
                        mov              qword ptr [1879052408], rdx
                                                                                        jmp   n444_lit_string_α
#=======================================================================================================================
#          Push()   =  EVAL(left ' ' op ' ' right)
#-----------------------------------------------------------------------------------------------------------------------
n444_lit_string_α:
                        mov              qword ptr [rsp + 1200], 1
                        mov              rax, qword ptr [rip + .Lx596_0]
                        mov              qword ptr [rsp + 1208], rax
                                                                                        jmp   n456_call_α
.Lx596_0:
                        .quad            .Lx596_0_s
.Lx596_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n445_match_head_α:
                        mov              rdi, qword ptr [rsp + 2448]
                        mov              rsi, qword ptr [rsp + 2456]
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r10, qword ptr [1879048192]
                        mov              qword ptr [r10 + 0], 0
                        add              r10, 24
                        mov              qword ptr [1879048192], r10
                        mov              qword ptr [rsp + 2352], rsp
                        lea              rcx, [rip + g_patstk_sp]
                        mov              rax, qword ptr [rcx + 0]
                        mov              qword ptr [rsp + 2344], rax
                        mov              dword ptr [rsp + 2336], 0
.Lx598_0:
                        mov              r14d, dword ptr [rsp + 2336]
                                                                                        jmp   n457_match_sequence_α
n445_match_head_β:
                        add              dword ptr [rsp + 2336], 1
                        mov              eax, dword ptr [rsp + 2336]
                        cmp              eax, r15d
                                                                                        jg    .Lx598_1
                        lea              rcx, [rip + g_anchor]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx598_1
                                                                                        jmp   .Lx598_0
.Lx598_1:
                        mov              rax, qword ptr [rsp + 2344]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rsp + 2352]
                        mov              r10, qword ptr [1879048192]
.Lx598_2:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx598_2
                        mov              qword ptr [1879048192], r10
                                                                                        jmp   n410_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n446_assign_α:
                        mov              rax, qword ptr [rsp + 1632]
                        mov              rdx, qword ptr [rsp + 1640]
                        mov              qword ptr [1879052480], rax
                        mov              qword ptr [1879052488], rdx
                                                                                        jmp   n447_lit_string_α
#=======================================================================================================================
#          real     =  integer '.' (integer | epsilon) (exponent | epsilon)
#-----------------------------------------------------------------------------------------------------------------------
n447_lit_string_α:
                        mov              qword ptr [rsp + 1744], 1
                        mov              rax, qword ptr [rip + .Lx600_0]
                        mov              qword ptr [rsp + 1752], rax
                                                                                        jmp   n458_call_α
.Lx600_0:
                        .quad            .Lx600_0_s
.Lx600_0_s:
                        .string          "PAT$2"
#-----------------------------------------------------------------------------------------------------------------------
n448_lit_integer_α:
                        mov              qword ptr [rsp + 224], 6
                        mov              rax, qword ptr [rip + .Lx601_0]
                        mov              qword ptr [rsp + 232], rax
                                                                                        jmp   n459_subscript_α
.Lx601_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n449_subscript_α:
                        mov              rdi, qword ptr [rsp + 320]
                        mov              rsi, qword ptr [rsp + 328]
                        mov              rdx, qword ptr [rsp + 336]
                        mov              rcx, qword ptr [rsp + 344]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n426_var_α
                        mov              qword ptr [rsp + 352], rax
                        mov              qword ptr [rsp + 360], rdx
                                                                                        jmp   n460_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n450_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 464], rax
                        mov              qword ptr [rsp + 472], rdx
                                                                                        jmp   n461_assign_var_α
#-----------------------------------------------------------------------------------------------------------------------
n451_call_α:
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 72], rax
                        .section         .rodata
.Lrkfn605:              .string          "SNO$NRET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn605]
                        lea              rsi, [rsp + 64]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 48], rax
                        mov              qword ptr [rsp + 56], rdx
                        cmp              eax, 99
                                                                                        je    n462_op14_α
                                                                                        jmp   n429_op14_α
n451_call_β:
                                                                                        jmp   n462_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n452_deref_α:
                        mov              rdi, qword ptr [rsp + 544]
                        mov              rsi, qword ptr [rsp + 552]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n415_var_α
                        mov              qword ptr [rsp + 560], rax
                        mov              qword ptr [rsp + 568], rdx
                                                                                        jmp   n463_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n453_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 672], rax
                        mov              qword ptr [rsp + 680], rdx
                                                                                        jmp   n464_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n454_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx609_0]
                        mov              esi, 1
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx609_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx609_6]
                        lea              rdx, [rip + .Lx609_7]
                                                                                        jmp   rax
.Lx609_6:
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
                                                                                        jmp   .Lx609_2
.Lx609_7:
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
                                                                                        jmp   .Lx609_2
.Lx609_5:
                        add              rsp, 32
                        mov              rdi, qword ptr [rip + .Lx609_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx609_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx609_3]
                        lea              rdx, [rip + .Lx609_4]
                                                                                        jmp   rax
.Lx609_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx609_2
.Lx609_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx609_2
.Lx609_1:
                        call             rt_faildescr@PLT
.Lx609_2:
                        mov              qword ptr [rsp + 896], rax
                        mov              qword ptr [rsp + 904], rdx
                        cmp              eax, 99
                                                                                        je    n455_lit_string_α
                                                                                        jmp   n465_var_α
n454_call_β:
                                                                                        jmp   n455_lit_string_α
.Lx609_0:
                        .quad            .Lx609_0_s
.Lx609_0_s:
                        .string          "Push"
#=======================================================================================================================
#          Unary    =  .dummy                        :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n455_lit_string_α:
                        mov              qword ptr [rsp + 1088], 1
                        mov              rax, qword ptr [rip + .Lx610_0]
                        mov              qword ptr [rsp + 1096], rax
                                                                                        jmp   n466_call_α
.Lx610_0:
                        .quad            .Lx610_0_s
.Lx610_0_s:
                        .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n456_call_α:
                        mov              rax, qword ptr [rsp + 1200]
                        mov              qword ptr [rsp + 1232], rax
                        mov              rax, qword ptr [rsp + 1208]
                        mov              qword ptr [rsp + 1240], rax
                        .section         .rodata
.Lrkfn612:              .string          "SNO$WANTNM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn612]
                        lea              rsi, [rsp + 1232]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1216], rax
                        mov              qword ptr [rsp + 1224], rdx
                        cmp              eax, 99
                                                                                        je    n468_lit_string_α
                                                                                        jmp   n467_call_α
n456_call_β:
                                                                                        jmp   n468_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n457_match_sequence_α:
                        mov              dword ptr [rsp + 2384], r14d
                                                                                        jmp   n470_lit_integer_α
n457_match_sequence_as:
                                                                                        jmp   n469_match_release_α
n457_match_sequence_β:
                                                                                        jmp   n474_match_rpos_β
n457_match_sequence_af:
                                                                                        jmp   n445_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n458_call_α:
                        mov              rax, qword ptr [rsp + 1744]
                        mov              qword ptr [rsp + 1712], rax
                        mov              rax, qword ptr [rsp + 1752]
                        mov              qword ptr [rsp + 1720], rax
                        .section         .rodata
.Lrkfn616:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn616]
                        lea              rsi, [rsp + 1712]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1696], rax
                        mov              qword ptr [rsp + 1704], rdx
                        cmp              eax, 99
                                                                                        je    n476_lit_string_α
                                                                                        jmp   n475_assign_α
n458_call_β:
                                                                                        jmp   n476_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n459_subscript_α:
                        mov              rdi, qword ptr [rsp + 208]
                        mov              rsi, qword ptr [rsp + 216]
                        mov              rdx, qword ptr [rsp + 224]
                        mov              rcx, qword ptr [rsp + 232]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n413_var_α
                        mov              qword ptr [rsp + 240], rax
                        mov              qword ptr [rsp + 248], rdx
                                                                                        jmp   n477_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n460_deref_α:
                        mov              rdi, qword ptr [rsp + 352]
                        mov              rsi, qword ptr [rsp + 360]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n426_var_α
                        mov              qword ptr [rsp + 368], rax
                        mov              qword ptr [rsp + 376], rdx
                                                                                        jmp   n478_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n461_assign_var_α:
                        mov              rdi, qword ptr [rsp + 400]
                        mov              rsi, qword ptr [rsp + 408]
                        mov              rdx, qword ptr [rsp + 464]
                        mov              rcx, qword ptr [rsp + 472]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n439_lit_string_α
                        mov              qword ptr [rsp + 480], rax
                        mov              qword ptr [rsp + 488], rdx
                                                                                        jmp   n439_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n462_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n463_subscript_α:
                        mov              rdi, qword ptr [rsp + 496]
                        mov              rsi, qword ptr [rsp + 504]
                        mov              rdx, qword ptr [rsp + 560]
                        mov              rcx, qword ptr [rsp + 568]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n415_var_α
                        mov              qword ptr [rsp + 576], rax
                        mov              qword ptr [rsp + 584], rdx
                                                                                        jmp   n479_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n464_lit_integer_α:
                        mov              qword ptr [rsp + 688], 6
                        mov              rax, qword ptr [rip + .Lx623_0]
                        mov              qword ptr [rsp + 696], rax
                                                                                        jmp   n480_subscript_α
.Lx623_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n465_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 992], rax
                        mov              qword ptr [rsp + 1000], rdx
                                                                                        jmp   n481_var_α
#-----------------------------------------------------------------------------------------------------------------------
n466_call_α:
                        mov              rax, qword ptr [rsp + 1088]
                        mov              qword ptr [rsp + 1056], rax
                        mov              rax, qword ptr [rsp + 1096]
                        mov              qword ptr [rsp + 1064], rax
                        .section         .rodata
.Lrkfn626:              .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn626]
                        lea              rsi, [rsp + 1056]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1040], rax
                        mov              qword ptr [rsp + 1048], rdx
                        cmp              eax, 99
                                                                                        je    n439_lit_string_α
                                                                                        jmp   n482_assign_α
n466_call_β:
                                                                                        jmp   n439_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n467_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx628_0]
                        mov              esi, 1
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx628_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx628_6]
                        lea              rdx, [rip + .Lx628_7]
                                                                                        jmp   rax
.Lx628_6:
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
                                                                                        jmp   .Lx628_2
.Lx628_7:
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
                                                                                        jmp   .Lx628_2
.Lx628_5:
                        add              rsp, 32
                        mov              rdi, qword ptr [rip + .Lx628_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx628_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx628_3]
                        lea              rdx, [rip + .Lx628_4]
                                                                                        jmp   rax
.Lx628_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx628_2
.Lx628_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx628_2
.Lx628_1:
                        call             rt_faildescr@PLT
.Lx628_2:
                        mov              qword ptr [rsp + 1264], rax
                        mov              qword ptr [rsp + 1272], rdx
                        cmp              eax, 99
                                                                                        je    n468_lit_string_α
                                                                                        jmp   n483_var_α
n467_call_β:
                                                                                        jmp   n468_lit_string_α
.Lx628_0:
                        .quad            .Lx628_0_s
.Lx628_0_s:
                        .string          "Push"
#=======================================================================================================================
#          Binary   =  .dummy                        :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n468_lit_string_α:
                        mov              qword ptr [rsp + 1552], 1
                        mov              rax, qword ptr [rip + .Lx629_0]
                        mov              qword ptr [rsp + 1560], rax
                                                                                        jmp   n484_call_α
.Lx629_0:
                        .quad            .Lx629_0_s
.Lx629_0_s:
                        .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n469_match_release_α:
                        mov              rax, qword ptr [rsp + 2344]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rsp + 2352]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, qword ptr [1879048192]
                        mov              r10, rsi
.Lx631_5:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx631_5
                        lea              rdi, [r10 + 24]
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx631_1:
                        test             rax, rax
                                                                                        je    .Lx631_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx631_3]
                        lea              rdx, [rip + .Lx631_4]
                                                                                        jmp   rax
.Lx631_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx631_1
.Lx631_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx631_1
.Lx631_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r10, qword ptr [1879048192]
.Lx631_6:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx631_6
                        mov              qword ptr [1879048192], r10
                                                                                        jmp   n485_call_α
#-----------------------------------------------------------------------------------------------------------------------
n470_lit_integer_α:
                        mov              qword ptr [rsp + 2400], 6
                        mov              rax, qword ptr [rip + .Lx632_0]
                        mov              qword ptr [rsp + 2408], rax
                                                                                        jmp   n471_match_pos_α
n470_lit_integer_β:
                                                                                        jmp   n445_match_head_β
.Lx632_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n471_match_pos_α:
                        mov              rax, 0
                        cmp              r14d, eax
                                                                                        jne   n445_match_head_β
                                                                                        jmp   n472_match_defer_α
n471_match_pos_β:
                                                                                        jmp   n445_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n472_match_defer_α:
                        mov              rax, qword ptr [1879052608]
                        mov              rdx, qword ptr [1879052616]
                        cmp              eax, 3
                                                                                        jne   .Lx634_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx634_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx634_10
.Lx634_9:
                        xor              eax, eax
.Lx634_10:
                        test             rax, rax
                                                                                        jz    .Lx634_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx634_4]
                        lea              rdx, [rip + .Lx634_5]
                                                                                        jmp   rax
.Lx634_4:
                                                                                        jmp   n473_lit_integer_α
.Lx634_5:
                                                                                        jmp   n445_match_head_β
.Lx634_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S6]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx634_2:
                        test             rax, rax
                                                                                        je    .Lx634_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx634_7]
                        lea              rdx, [rip + .Lx634_8]
                                                                                        jmp   rax
.Lx634_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx634_2
.Lx634_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx634_2
.Lx634_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n445_match_head_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx634_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n473_lit_integer_α
.Lx634_6:
                        add              rsp, 16
                                                                                        jmp   n445_match_head_β
n472_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n473_lit_integer_α:
                        mov              qword ptr [rsp + 2432], 6
                        mov              rax, qword ptr [rip + .Lx635_0]
                        mov              qword ptr [rsp + 2440], rax
                                                                                        jmp   n474_match_rpos_α
n473_lit_integer_β:
                                                                                        jmp   n472_match_defer_β
.Lx635_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n474_match_rpos_α:
                        mov              rax, 0
                        mov              ecx, r15d
                        sub              ecx, eax
                        cmp              r14d, ecx
                                                                                        jne   n472_match_defer_β
                                                                                        jmp   n469_match_release_α
n474_match_rpos_β:
                                                                                        jmp   n472_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n475_assign_α:
                        mov              rax, qword ptr [rsp + 1696]
                        mov              rdx, qword ptr [rsp + 1704]
                        mov              qword ptr [1879052496], rax
                        mov              qword ptr [1879052504], rdx
                                                                                        jmp   n476_lit_string_α
#=======================================================================================================================
#          real     =  integer '.' (integer | epsilon) (exponent | epsilon)
#          addop    =  ANY('+-') . *Push()
#-----------------------------------------------------------------------------------------------------------------------
n476_lit_string_α:
                        mov              qword ptr [rsp + 1808], 1
                        mov              rax, qword ptr [rip + .Lx638_0]
                        mov              qword ptr [rsp + 1816], rax
                                                                                        jmp   n486_call_α
.Lx638_0:
                        .quad            .Lx638_0_s
.Lx638_0_s:
                        .string          "PAT$3"
#-----------------------------------------------------------------------------------------------------------------------
n477_deref_α:
                        mov              rdi, qword ptr [rsp + 240]
                        mov              rsi, qword ptr [rsp + 248]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n413_var_α
                        mov              qword ptr [rsp + 256], rax
                        mov              qword ptr [rsp + 264], rdx
                                                                                        jmp   n487_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n478_subscript_α:
                        mov              rdi, qword ptr [rsp + 304]
                        mov              rsi, qword ptr [rsp + 312]
                        mov              rdx, qword ptr [rsp + 368]
                        mov              rcx, qword ptr [rsp + 376]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n426_var_α
                        mov              qword ptr [rsp + 384], rax
                        mov              qword ptr [rsp + 392], rdx
                                                                                        jmp   n488_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n479_deref_α:
                        mov              rdi, qword ptr [rsp + 576]
                        mov              rsi, qword ptr [rsp + 584]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n415_var_α
                        mov              qword ptr [rsp + 592], rax
                        mov              qword ptr [rsp + 600], rdx
                                                                                        jmp   n489_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n480_subscript_α:
                        mov              rdi, qword ptr [rsp + 672]
                        mov              rsi, qword ptr [rsp + 680]
                        mov              rdx, qword ptr [rsp + 688]
                        mov              rcx, qword ptr [rsp + 696]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n429_op14_α
                        mov              qword ptr [rsp + 704], rax
                        mov              qword ptr [rsp + 712], rdx
                                                                                        jmp   n490_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n481_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 1008], rax
                        mov              qword ptr [rsp + 1016], rdx
                                                                                        jmp   n491_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n482_assign_α:
                        mov              rax, qword ptr [rsp + 1040]
                        mov              rdx, qword ptr [rsp + 1048]
                        mov              qword ptr [1879052336], rax
                        mov              qword ptr [1879052344], rdx
                                                                                        jmp   n439_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n483_var_α:
                        mov              rax, qword ptr [1879052400]
                        mov              rdx, qword ptr [1879052408]
                        mov              qword ptr [rsp + 1408], rax
                        mov              qword ptr [rsp + 1416], rdx
                                                                                        jmp   n492_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n484_call_α:
                        mov              rax, qword ptr [rsp + 1552]
                        mov              qword ptr [rsp + 1520], rax
                        mov              rax, qword ptr [rsp + 1560]
                        mov              qword ptr [rsp + 1528], rax
                        .section         .rodata
.Lrkfn647:              .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn647]
                        lea              rsi, [rsp + 1520]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1504], rax
                        mov              qword ptr [rsp + 1512], rdx
                        cmp              eax, 99
                                                                                        je    n439_lit_string_α
                                                                                        jmp   n493_assign_α
n484_call_β:
                                                                                        jmp   n439_lit_string_α
#=======================================================================================================================
#          OUTPUT   =  Pop()                         :(loop)
#-----------------------------------------------------------------------------------------------------------------------
n485_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx649_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx649_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx649_6]
                        lea              rdx, [rip + .Lx649_7]
                                                                                        jmp   rax
.Lx649_6:
                        mov              rdi, qword ptr [1879052320]
                        mov              rsi, qword ptr [1879052328]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx649_2
.Lx649_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052328], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx649_2
.Lx649_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx649_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx649_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx649_3]
                        lea              rdx, [rip + .Lx649_4]
                                                                                        jmp   rax
.Lx649_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx649_2
.Lx649_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx649_2
.Lx649_1:
                        call             rt_faildescr@PLT
.Lx649_2:
                        mov              qword ptr [rsp + 2464], rax
                        mov              qword ptr [rsp + 2472], rdx
                        cmp              eax, 99
                                                                                        je    n409_var_α
                                                                                        jmp   n494_assign_α
n485_call_β:
                                                                                        jmp   n409_var_α
.Lx649_0:
                        .quad            .Lx649_0_s
.Lx649_0_s:
                        .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n486_call_α:
                        mov              rax, qword ptr [rsp + 1808]
                        mov              qword ptr [rsp + 1776], rax
                        mov              rax, qword ptr [rsp + 1816]
                        mov              qword ptr [rsp + 1784], rax
                        .section         .rodata
.Lrkfn651:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn651]
                        lea              rsi, [rsp + 1776]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1760], rax
                        mov              qword ptr [rsp + 1768], rdx
                        cmp              eax, 99
                                                                                        je    n496_lit_string_α
                                                                                        jmp   n495_assign_α
n486_call_β:
                                                                                        jmp   n496_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n487_lit_integer_α:
                        mov              qword ptr [rsp + 272], 6
                        mov              rax, qword ptr [rip + .Lx652_0]
                        mov              qword ptr [rsp + 280], rax
                                                                                        jmp   n497_binop_α
.Lx652_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n488_assign_α:
                        mov              rax, qword ptr [rsp + 384]
                        mov              rdx, qword ptr [rsp + 392]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n426_var_α
#-----------------------------------------------------------------------------------------------------------------------
n489_assign_α:
                        mov              rax, qword ptr [rsp + 592]
                        mov              rdx, qword ptr [rsp + 600]
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rdx
                                                                                        jmp   n415_var_α
#-----------------------------------------------------------------------------------------------------------------------
n490_deref_α:
                        mov              rdi, qword ptr [rsp + 704]
                        mov              rsi, qword ptr [rsp + 712]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n429_op14_α
                        mov              qword ptr [rsp + 720], rax
                        mov              qword ptr [rsp + 728], rdx
                                                                                        jmp   n498_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n491_binop_α:
                        mov              rdi, qword ptr [rsp + 992]
                        mov              rsi, qword ptr [rsp + 1000]
                        mov              rdx, qword ptr [rsp + 1008]
                        mov              rcx, qword ptr [rsp + 1016]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 976], rax
                        mov              qword ptr [rsp + 984], rdx
                                                                                        jmp   n499_call_α
#-----------------------------------------------------------------------------------------------------------------------
n492_lit_string_α:
                        mov              qword ptr [rsp + 1424], 1
                        mov              rax, qword ptr [rip + .Lx657_0]
                        mov              qword ptr [rsp + 1432], rax
                                                                                        jmp   n500_binop_α
.Lx657_0:
                        .quad            .Lx657_0_s
.Lx657_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n493_assign_α:
                        mov              rax, qword ptr [rsp + 1504]
                        mov              rdx, qword ptr [rsp + 1512]
                        mov              qword ptr [1879052384], rax
                        mov              qword ptr [1879052392], rdx
                                                                                        jmp   n439_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n494_assign_α:
                        mov              rsi, qword ptr [rsp + 2464]
                        mov              rdx, qword ptr [rsp + 2472]
                        mov              rdi, qword ptr [rip + .Lx659_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n409_var_α
.Lx659_0:
                        .quad            .Lx659_0_s
.Lx659_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n495_assign_α:
                        mov              rax, qword ptr [rsp + 1760]
                        mov              rdx, qword ptr [rsp + 1768]
                        mov              qword ptr [1879052512], rax
                        mov              qword ptr [1879052520], rdx
                                                                                        jmp   n496_lit_string_α
#=======================================================================================================================
#          mulop    =  ANY('*/') . *Push()
#-----------------------------------------------------------------------------------------------------------------------
n496_lit_string_α:
                        mov              qword ptr [rsp + 1872], 1
                        mov              rax, qword ptr [rip + .Lx661_0]
                        mov              qword ptr [rsp + 1880], rax
                                                                                        jmp   n501_call_α
.Lx661_0:
                        .quad            .Lx661_0_s
.Lx661_0_s:
                        .string          "PAT$4"
#-----------------------------------------------------------------------------------------------------------------------
n497_binop_α:
                        mov              rdi, qword ptr [rsp + 256]
                        mov              rsi, qword ptr [rsp + 264]
                        mov              rdx, qword ptr [rsp + 272]
                        mov              rcx, qword ptr [rsp + 280]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        je    n413_var_α
                        mov              qword ptr [rsp + 192], rax
                        mov              qword ptr [rsp + 200], rdx
                                                                                        jmp   n502_assign_var_α
#-----------------------------------------------------------------------------------------------------------------------
n498_lit_integer_α:
                        mov              qword ptr [rsp + 736], 6
                        mov              rax, qword ptr [rip + .Lx663_0]
                        mov              qword ptr [rsp + 744], rax
                                                                                        jmp   n503_binop_α
.Lx663_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n499_call_α:
                        mov              rax, qword ptr [rsp + 976]
                        mov              qword ptr [rsp + 944], rax
                        mov              rax, qword ptr [rsp + 984]
                        mov              qword ptr [rsp + 952], rax
                        .section         .rodata
.Lrkfn665:              .string          "EVAL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn665]
                        lea              rsi, [rsp + 944]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 928], rax
                        mov              qword ptr [rsp + 936], rdx
                        cmp              eax, 99
                                                                                        je    n455_lit_string_α
                                                                                        jmp   n504_assign_var_α
n499_call_β:
                                                                                        jmp   n455_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n500_binop_α:
                        mov              rdi, qword ptr [rsp + 1408]
                        mov              rsi, qword ptr [rsp + 1416]
                        mov              rdx, qword ptr [rsp + 1424]
                        mov              rcx, qword ptr [rsp + 1432]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 1392], rax
                        mov              qword ptr [rsp + 1400], rdx
                                                                                        jmp   n505_var_α
#-----------------------------------------------------------------------------------------------------------------------
n501_call_α:
                        mov              rax, qword ptr [rsp + 1872]
                        mov              qword ptr [rsp + 1840], rax
                        mov              rax, qword ptr [rsp + 1880]
                        mov              qword ptr [rsp + 1848], rax
                        .section         .rodata
.Lrkfn668:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn668]
                        lea              rsi, [rsp + 1840]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1824], rax
                        mov              qword ptr [rsp + 1832], rdx
                        cmp              eax, 99
                                                                                        je    n507_lit_string_α
                                                                                        jmp   n506_assign_α
n501_call_β:
                                                                                        jmp   n507_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n502_assign_var_α:
                        mov              rdi, qword ptr [rsp + 176]
                        mov              rsi, qword ptr [rsp + 184]
                        mov              rdx, qword ptr [rsp + 192]
                        mov              rcx, qword ptr [rsp + 200]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n413_var_α
                        mov              qword ptr [rsp + 288], rax
                        mov              qword ptr [rsp + 296], rdx
                                                                                        jmp   n413_var_α
#-----------------------------------------------------------------------------------------------------------------------
n503_binop_α:
                        mov              rdi, qword ptr [rsp + 720]
                        mov              rsi, qword ptr [rsp + 728]
                        mov              rdx, qword ptr [rsp + 736]
                        mov              rcx, qword ptr [rsp + 744]
                        call             rt_sub@PLT
                        cmp              eax, 99
                                                                                        je    n429_op14_α
                        mov              qword ptr [rsp + 656], rax
                        mov              qword ptr [rsp + 664], rdx
                                                                                        jmp   n508_assign_var_α
#-----------------------------------------------------------------------------------------------------------------------
n504_assign_var_α:
                        mov              rdi, qword ptr [rsp + 896]
                        mov              rsi, qword ptr [rsp + 904]
                        mov              rdx, qword ptr [rsp + 928]
                        mov              rcx, qword ptr [rsp + 936]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n455_lit_string_α
                        mov              qword ptr [rsp + 1024], rax
                        mov              qword ptr [rsp + 1032], rdx
                                                                                        jmp   n455_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n505_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 1440], rax
                        mov              qword ptr [rsp + 1448], rdx
                                                                                        jmp   n509_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n506_assign_α:
                        mov              rax, qword ptr [rsp + 1824]
                        mov              rdx, qword ptr [rsp + 1832]
                        mov              qword ptr [1879052528], rax
                        mov              qword ptr [1879052536], rdx
                                                                                        jmp   n507_lit_string_α
#=======================================================================================================================
#          constant =  (real | integer) . *Push()
#-----------------------------------------------------------------------------------------------------------------------
n507_lit_string_α:
                        mov              qword ptr [rsp + 1936], 1
                        mov              rax, qword ptr [rip + .Lx674_0]
                        mov              qword ptr [rsp + 1944], rax
                                                                                        jmp   n510_call_α
.Lx674_0:
                        .quad            .Lx674_0_s
.Lx674_0_s:
                        .string          "PAT$5"
#-----------------------------------------------------------------------------------------------------------------------
n508_assign_var_α:
                        mov              rdi, qword ptr [rsp + 640]
                        mov              rsi, qword ptr [rsp + 648]
                        mov              rdx, qword ptr [rsp + 656]
                        mov              rcx, qword ptr [rsp + 664]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n429_op14_α
                        mov              qword ptr [rsp + 752], rax
                        mov              qword ptr [rsp + 760], rdx
                                                                                        jmp   n429_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n509_binop_α:
                        mov              rdi, qword ptr [rsp + 1392]
                        mov              rsi, qword ptr [rsp + 1400]
                        mov              rdx, qword ptr [rsp + 1440]
                        mov              rcx, qword ptr [rsp + 1448]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 1376], rax
                        mov              qword ptr [rsp + 1384], rdx
                                                                                        jmp   n511_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n510_call_α:
                        mov              rax, qword ptr [rsp + 1936]
                        mov              qword ptr [rsp + 1904], rax
                        mov              rax, qword ptr [rsp + 1944]
                        mov              qword ptr [rsp + 1912], rax
                        .section         .rodata
.Lrkfn678:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn678]
                        lea              rsi, [rsp + 1904]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1888], rax
                        mov              qword ptr [rsp + 1896], rdx
                        cmp              eax, 99
                                                                                        je    n513_lit_string_α
                                                                                        jmp   n512_assign_α
n510_call_β:
                                                                                        jmp   n513_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n511_lit_string_α:
                        mov              qword ptr [rsp + 1456], 1
                        mov              rax, qword ptr [rip + .Lx679_0]
                        mov              qword ptr [rsp + 1464], rax
                                                                                        jmp   n514_binop_α
.Lx679_0:
                        .quad            .Lx679_0_s
.Lx679_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n512_assign_α:
                        mov              rax, qword ptr [rsp + 1888]
                        mov              rdx, qword ptr [rsp + 1896]
                        mov              qword ptr [1879052544], rax
                        mov              qword ptr [1879052552], rdx
                                                                                        jmp   n513_lit_string_α
#=======================================================================================================================
#          constant =  (real | integer) . *Push()
#          primary  =  constant | '(' *expr ')'
#-----------------------------------------------------------------------------------------------------------------------
n513_lit_string_α:
                        mov              qword ptr [rsp + 2000], 1
                        mov              rax, qword ptr [rip + .Lx681_0]
                        mov              qword ptr [rsp + 2008], rax
                                                                                        jmp   n515_call_α
.Lx681_0:
                        .quad            .Lx681_0_s
.Lx681_0_s:
                        .string          "PAT$6"
#-----------------------------------------------------------------------------------------------------------------------
n514_binop_α:
                        mov              rdi, qword ptr [rsp + 1376]
                        mov              rsi, qword ptr [rsp + 1384]
                        mov              rdx, qword ptr [rsp + 1456]
                        mov              rcx, qword ptr [rsp + 1464]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 1360], rax
                        mov              qword ptr [rsp + 1368], rdx
                                                                                        jmp   n516_var_α
#-----------------------------------------------------------------------------------------------------------------------
n515_call_α:
                        mov              rax, qword ptr [rsp + 2000]
                        mov              qword ptr [rsp + 1968], rax
                        mov              rax, qword ptr [rsp + 2008]
                        mov              qword ptr [rsp + 1976], rax
                        .section         .rodata
.Lrkfn684:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn684]
                        lea              rsi, [rsp + 1968]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1952], rax
                        mov              qword ptr [rsp + 1960], rdx
                        cmp              eax, 99
                                                                                        je    n518_lit_string_α
                                                                                        jmp   n517_assign_α
n515_call_β:
                                                                                        jmp   n518_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n516_var_α:
                        mov              rax, qword ptr [1879052416]
                        mov              rdx, qword ptr [1879052424]
                        mov              qword ptr [rsp + 1472], rax
                        mov              qword ptr [rsp + 1480], rdx
                                                                                        jmp   n519_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n517_assign_α:
                        mov              rax, qword ptr [rsp + 1952]
                        mov              rdx, qword ptr [rsp + 1960]
                        mov              qword ptr [1879052560], rax
                        mov              qword ptr [1879052568], rdx
                                                                                        jmp   n518_lit_string_α
#=======================================================================================================================
#          primary  =  constant | '(' *expr ')'
#          factor   =  addop *factor . *Unary()
#-----------------------------------------------------------------------------------------------------------------------
n518_lit_string_α:
                        mov              qword ptr [rsp + 2064], 1
                        mov              rax, qword ptr [rip + .Lx687_0]
                        mov              qword ptr [rsp + 2072], rax
                                                                                        jmp   n520_call_α
.Lx687_0:
                        .quad            .Lx687_0_s
.Lx687_0_s:
                        .string          "PAT$7"
#-----------------------------------------------------------------------------------------------------------------------
n519_binop_α:
                        mov              rdi, qword ptr [rsp + 1360]
                        mov              rsi, qword ptr [rsp + 1368]
                        mov              rdx, qword ptr [rsp + 1472]
                        mov              rcx, qword ptr [rsp + 1480]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 1344], rax
                        mov              qword ptr [rsp + 1352], rdx
                                                                                        jmp   n521_call_α
#-----------------------------------------------------------------------------------------------------------------------
n520_call_α:
                        mov              rax, qword ptr [rsp + 2064]
                        mov              qword ptr [rsp + 2032], rax
                        mov              rax, qword ptr [rsp + 2072]
                        mov              qword ptr [rsp + 2040], rax
                        .section         .rodata
.Lrkfn690:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn690]
                        lea              rsi, [rsp + 2032]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 2016], rax
                        mov              qword ptr [rsp + 2024], rdx
                        cmp              eax, 99
                                                                                        je    n523_lit_string_α
                                                                                        jmp   n522_assign_α
n520_call_β:
                                                                                        jmp   n523_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n521_call_α:
                        mov              rax, qword ptr [rsp + 1344]
                        mov              qword ptr [rsp + 1312], rax
                        mov              rax, qword ptr [rsp + 1352]
                        mov              qword ptr [rsp + 1320], rax
                        .section         .rodata
.Lrkfn692:              .string          "EVAL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn692]
                        lea              rsi, [rsp + 1312]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1296], rax
                        mov              qword ptr [rsp + 1304], rdx
                        cmp              eax, 99
                                                                                        je    n468_lit_string_α
                                                                                        jmp   n524_assign_var_α
n521_call_β:
                                                                                        jmp   n468_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n522_assign_α:
                        mov              rax, qword ptr [rsp + 2016]
                        mov              rdx, qword ptr [rsp + 2024]
                        mov              qword ptr [1879052576], rax
                        mov              qword ptr [1879052584], rdx
                                                                                        jmp   n523_lit_string_α
#=======================================================================================================================
#          factor   =  addop *factor . *Unary()
#          term     =  *factor mulop *term . *Binary()
#-----------------------------------------------------------------------------------------------------------------------
n523_lit_string_α:
                        mov              qword ptr [rsp + 2128], 1
                        mov              rax, qword ptr [rip + .Lx694_0]
                        mov              qword ptr [rsp + 2136], rax
                                                                                        jmp   n525_call_α
.Lx694_0:
                        .quad            .Lx694_0_s
.Lx694_0_s:
                        .string          "PAT$8"
#-----------------------------------------------------------------------------------------------------------------------
n524_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1264]
                        mov              rsi, qword ptr [rsp + 1272]
                        mov              rdx, qword ptr [rsp + 1296]
                        mov              rcx, qword ptr [rsp + 1304]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n468_lit_string_α
                        mov              qword ptr [rsp + 1488], rax
                        mov              qword ptr [rsp + 1496], rdx
                                                                                        jmp   n468_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n525_call_α:
                        mov              rax, qword ptr [rsp + 2128]
                        mov              qword ptr [rsp + 2096], rax
                        mov              rax, qword ptr [rsp + 2136]
                        mov              qword ptr [rsp + 2104], rax
                        .section         .rodata
.Lrkfn697:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn697]
                        lea              rsi, [rsp + 2096]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 2080], rax
                        mov              qword ptr [rsp + 2088], rdx
                        cmp              eax, 99
                                                                                        je    n527_lit_string_α
                                                                                        jmp   n526_assign_α
n525_call_β:
                                                                                        jmp   n527_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n526_assign_α:
                        mov              rax, qword ptr [rsp + 2080]
                        mov              rdx, qword ptr [rsp + 2088]
                        mov              qword ptr [1879052592], rax
                        mov              qword ptr [1879052600], rdx
                                                                                        jmp   n527_lit_string_α
#=======================================================================================================================
#          term     =  *factor mulop *term . *Binary()
#          expr     =  *term addop *expr . *Binary()
#-----------------------------------------------------------------------------------------------------------------------
n527_lit_string_α:
                        mov              qword ptr [rsp + 2192], 1
                        mov              rax, qword ptr [rip + .Lx699_0]
                        mov              qword ptr [rsp + 2200], rax
                                                                                        jmp   n528_call_α
.Lx699_0:
                        .quad            .Lx699_0_s
.Lx699_0_s:
                        .string          "PAT$9"
#-----------------------------------------------------------------------------------------------------------------------
n528_call_α:
                        mov              rax, qword ptr [rsp + 2192]
                        mov              qword ptr [rsp + 2160], rax
                        mov              rax, qword ptr [rsp + 2200]
                        mov              qword ptr [rsp + 2168], rax
                        .section         .rodata
.Lrkfn701:              .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn701]
                        lea              rsi, [rsp + 2160]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 2144], rax
                        mov              qword ptr [rsp + 2152], rdx
                        cmp              eax, 99
                                                                                        je    n530_lit_string_α
                                                                                        jmp   n529_assign_α
n528_call_β:
                                                                                        jmp   n530_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n529_assign_α:
                        mov              rax, qword ptr [rsp + 2144]
                        mov              rdx, qword ptr [rsp + 2152]
                        mov              qword ptr [1879052608], rax
                        mov              qword ptr [1879052616], rdx
                                                                                        jmp   n530_lit_string_α
#=======================================================================================================================
#          expr     =  *term addop *expr . *Binary()
#          &TRIM    =  1
#-----------------------------------------------------------------------------------------------------------------------
n530_lit_string_α:
                        mov              qword ptr [rsp + 2272], 1
                        mov              rax, qword ptr [rip + .Lx703_0]
                        mov              qword ptr [rsp + 2280], rax
                                                                                        jmp   n531_lit_integer_α
.Lx703_0:
                        .quad            .Lx703_0_s
.Lx703_0_s:
                        .string          "TRIM"
#-----------------------------------------------------------------------------------------------------------------------
n531_lit_integer_α:
                        mov              qword ptr [rsp + 2288], 6
                        mov              rax, qword ptr [rip + .Lx704_0]
                        mov              qword ptr [rsp + 2296], rax
                                                                                        jmp   n532_call_α
.Lx704_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n532_call_α:
                        mov              rax, qword ptr [rsp + 2272]
                        mov              qword ptr [rsp + 2224], rax
                        mov              rax, qword ptr [rsp + 2280]
                        mov              qword ptr [rsp + 2232], rax
                        mov              rax, qword ptr [rsp + 2288]
                        mov              qword ptr [rsp + 2240], rax
                        mov              rax, qword ptr [rsp + 2296]
                        mov              qword ptr [rsp + 2248], rax
                        .section         .rodata
.Lrkfn706:              .string          "SNO$KWSET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn706]
                        lea              rsi, [rsp + 2224]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 2208], rax
                        mov              qword ptr [rsp + 2216], rdx
                        cmp              eax, 99
                                                                                        je    n409_var_α
                                                                                        jmp   n409_var_α
n532_call_β:
                                                                                        jmp   n409_var_α
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        add              rsp, 2520
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              eax, 99
                        xor              edx, edx
                        add              rsp, 2520
                        ret
                        .section         .rodata
.S0:                    .string          "integer"
.S1:                    .string          "epsilon"
.S2:                    .string          "exponent"
.S3:                    .string          "*Push"
.S4:                    .string          "real"
.S5:                    .string          "constant"
.S6:                    .string          "expr"
.S7:                    .string          "primary"
.S8:                    .string          "addop"
.S9:                    .string          "*Unary"
.S10:                   .string          "factor"
.S11:                   .string          "mulop"
.S12:                   .string          "*Binary"
.S13:                   .string          "term"
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

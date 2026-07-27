                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_LBL__stack_init_α
proc_LBL__stack_init_α:
                        .global          proc_LBL__stack_init_α
                        .global          proc_LBL__stack_init_β
                        .global          proc_LBL__stack_init_γ
                        .global          proc_LBL__stack_init_ω
                        sub              rsp, 2864
                        mov              [rsp + 2840], rcx
                        mov              [rsp + 2848], rdx
                        mov              [rsp + 2856], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 2480], 0
                        mov              qword ptr [rsp + 2488], 0
                        mov              qword ptr [rsp + 2560], 0
                        mov              qword ptr [rsp + 2568], 0
proc_LBL__stack_init_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n0_goto_α:
                                                                                        jmp   n1_lit_string_α
n0_goto_β:
                                                                                        jmp   proc_LBL__stack_init_ω
#-----------------------------------------------------------------------------------------------------------------------
n1_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              rax, qword ptr [rip + .Lx5_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n2_assign_α
.Lx5_0:
                        .quad            .Lx5_0_s
.Lx5_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n2_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052432], rax
                        mov              qword ptr [1879052440], rdx
                                                                                        jmp   n3_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n3_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_init_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_init_β:
                                                                                        jmp   proc_LBL__stack_init_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_init_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 2840]
                        lea              rsp, [rbp + 2864]
                        mov              rbp, [rbp + 2856]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_init_ω:
                        mov              rax, [rbp + 2848]
                        lea              rsp, [rbp + 2864]
                        mov              rbp, [rbp + 2856]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_LBL__stack_push_α
proc_LBL__stack_push_α:
                        .global          proc_LBL__stack_push_α
                        .global          proc_LBL__stack_push_β
                        .global          proc_LBL__stack_push_γ
                        .global          proc_LBL__stack_push_ω
                        sub              rsp, 2864
                        mov              [rsp + 2840], rcx
                        mov              [rsp + 2848], rdx
                        mov              [rsp + 2856], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 2480], 0
                        mov              qword ptr [rsp + 2488], 0
                        mov              qword ptr [rsp + 2560], 0
                        mov              qword ptr [rsp + 2568], 0
proc_LBL__stack_push_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n9_goto_α:
                                                                                        jmp   n10_var_α
n9_goto_β:
                                                                                        jmp   proc_LBL__stack_push_ω
#-----------------------------------------------------------------------------------------------------------------------
n10_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 256], rax
                        mov              qword ptr [rsp + 264], rdx
                                                                                        jmp   n11_var_α
#-----------------------------------------------------------------------------------------------------------------------
n11_var_α:
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 272], rax
                        mov              qword ptr [rsp + 280], rdx
                                                                                        jmp   n13_call_α
#=======================================================================================================================
#         stack_push(w1)
#-----------------------------------------------------------------------------------------------------------------------
n12_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 288], rax
                        mov              qword ptr [rsp + 296], rdx
                                                                                        jmp   n14_field_var_α
#-----------------------------------------------------------------------------------------------------------------------
n13_call_α:
                        mov              rax, qword ptr [rsp + 256]
                        mov              qword ptr [rsp + 208], rax
                        mov              rax, qword ptr [rsp + 264]
                        mov              qword ptr [rsp + 216], rax
                        mov              rax, qword ptr [rsp + 272]
                        mov              qword ptr [rsp + 224], rax
                        mov              rax, qword ptr [rsp + 280]
                        mov              qword ptr [rsp + 232], rax
                        .section         .rodata
.Lrkfn26:               .string          "slink"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn26]
                        lea              rsi, [rsp + 208]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 192], rax
                        mov              qword ptr [rsp + 200], rdx
                        cmp              eax, 99
                                                                                        je    n12_var_α
                                                                                        jmp   n16_assign_α
n13_call_β:
                                                                                        jmp   n12_var_α
#-----------------------------------------------------------------------------------------------------------------------
n14_field_var_α:
                        mov              rdi, qword ptr [rip + .Lx27_0]
                        mov              rsi, qword ptr [rsp + 288]
                        mov              rdx, qword ptr [rsp + 296]
                        call             rt_field_var@PLT
                        cmp              eax, 99
                                                                                        je    n15_lit_string_α
                        mov              qword ptr [rsp + 304], rax
                        mov              qword ptr [rsp + 312], rdx
                                                                                        jmp   n17_assign_α
.Lx27_0:
                        .quad            .Lx27_0_s
.Lx27_0_s:
                        .string          "sval"
#-----------------------------------------------------------------------------------------------------------------------
n15_lit_string_α:
                        mov              qword ptr [rsp + 32], 1
                        mov              rax, qword ptr [rip + .Lx28_0]
                        mov              qword ptr [rsp + 40], rax
                                                                                        jmp   n18_call_α
.Lx28_0:
                        .quad            .Lx28_0_s
.Lx28_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n16_assign_α:
                        mov              rax, qword ptr [rsp + 192]
                        mov              rdx, qword ptr [rsp + 200]
                        mov              qword ptr [1879052432], rax
                        mov              qword ptr [1879052440], rdx
                                                                                        jmp   n12_var_α
#-----------------------------------------------------------------------------------------------------------------------
n17_assign_α:
                        mov              rax, qword ptr [rsp + 304]
                        mov              rdx, qword ptr [rsp + 312]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   n15_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n18_call_α:
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 72], rax
                        .section         .rodata
.Lrkfn32:               .string          "SNO$NRET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn32]
                        lea              rsi, [rsp + 64]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 48], rax
                        mov              qword ptr [rsp + 56], rdx
                        cmp              eax, 99
                                                                                        je    n20_op14_α
                                                                                        jmp   n19_op14_α
n18_call_β:
                                                                                        jmp   n20_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n19_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n20_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_push_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_push_β:
                                                                                        jmp   proc_LBL__stack_push_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_push_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 2840]
                        lea              rsp, [rbp + 2864]
                        mov              rbp, [rbp + 2856]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_push_ω:
                        mov              rax, [rbp + 2848]
                        lea              rsp, [rbp + 2864]
                        mov              rbp, [rbp + 2856]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_LBL__stack_pop_α
proc_LBL__stack_pop_α:
                        .global          proc_LBL__stack_pop_α
                        .global          proc_LBL__stack_pop_β
                        .global          proc_LBL__stack_pop_γ
                        .global          proc_LBL__stack_pop_ω
                        sub              rsp, 2864
                        mov              [rsp + 2840], rcx
                        mov              [rsp + 2848], rdx
                        mov              [rsp + 2856], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 2480], 0
                        mov              qword ptr [rsp + 2488], 0
                        mov              qword ptr [rsp + 2560], 0
                        mov              qword ptr [rsp + 2568], 0
proc_LBL__stack_pop_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n37_goto_α:
                                                                                        jmp   n38_var_α
n37_goto_β:
                                                                                        jmp   proc_LBL__stack_pop_ω
#-----------------------------------------------------------------------------------------------------------------------
n38_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 368], rax
                        mov              qword ptr [rsp + 376], rdx
                                                                                        jmp   n39_call_α
#-----------------------------------------------------------------------------------------------------------------------
n39_call_α:
                        mov              rax, qword ptr [rsp + 368]
                        mov              qword ptr [rsp + 336], rax
                        mov              rax, qword ptr [rsp + 376]
                        mov              qword ptr [rsp + 344], rax
                        .section         .rodata
.Lrkfn66:               .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn66]
                        lea              rsi, [rsp + 336]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 320], rax
                        mov              qword ptr [rsp + 328], rdx
                        cmp              eax, 99
                                                                                        je    n40_op14_α
                                                                                        jmp   n41_var_α
n39_call_β:
                                                                                        jmp   n40_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n40_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n41_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 432], rax
                        mov              qword ptr [rsp + 440], rdx
                                                                                        jmp   n42_call_α
#-----------------------------------------------------------------------------------------------------------------------
n42_call_α:
                        mov              rax, qword ptr [rsp + 432]
                        mov              qword ptr [rsp + 400], rax
                        mov              rax, qword ptr [rsp + 440]
                        mov              qword ptr [rsp + 408], rax
                        .section         .rodata
.Lrkfn71:               .string          "IDENT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn71]
                        lea              rsi, [rsp + 400]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 384], rax
                        mov              qword ptr [rsp + 392], rdx
                        cmp              eax, 99
                                                                                        je    n43_var_α
                                                                                        jmp   n44_var_α
n42_call_β:
                                                                                        jmp   n43_var_α
#-----------------------------------------------------------------------------------------------------------------------
n43_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 624], rax
                        mov              qword ptr [rsp + 632], rdx
                                                                                        jmp   n45_call_α
#-----------------------------------------------------------------------------------------------------------------------
n44_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 496], rax
                        mov              qword ptr [rsp + 504], rdx
                                                                                        jmp   n47_call_α
#-----------------------------------------------------------------------------------------------------------------------
n45_call_α:
                        mov              rax, qword ptr [rsp + 624]
                        mov              qword ptr [rsp + 592], rax
                        mov              rax, qword ptr [rsp + 632]
                        mov              qword ptr [rsp + 600], rax
                        .section         .rodata
.Lrkfn75:               .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn75]
                        lea              rsi, [rsp + 592]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 576], rax
                        mov              qword ptr [rsp + 584], rdx
                        cmp              eax, 99
                                                                                        je    n46_var_α
                                                                                        jmp   n49_var_α
n45_call_β:
                                                                                        jmp   n46_var_α
#-----------------------------------------------------------------------------------------------------------------------
n46_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 768], rax
                        mov              qword ptr [rsp + 776], rdx
                                                                                        jmp   n50_call_α
#-----------------------------------------------------------------------------------------------------------------------
n47_call_α:
                        mov              rax, qword ptr [rsp + 496]
                        mov              qword ptr [rsp + 464], rax
                        mov              rax, qword ptr [rsp + 504]
                        mov              qword ptr [rsp + 472], rax
                        .section         .rodata
.Lrkfn78:               .string          "sval"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn78]
                        lea              rsi, [rsp + 464]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 448], rax
                        mov              qword ptr [rsp + 456], rdx
                        cmp              eax, 99
                                                                                        je    n48_var_α
                                                                                        jmp   n52_assign_α
n47_call_β:
                                                                                        jmp   n48_var_α
#-----------------------------------------------------------------------------------------------------------------------
n48_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 560], rax
                        mov              qword ptr [rsp + 568], rdx
                                                                                        jmp   n53_call_α
#-----------------------------------------------------------------------------------------------------------------------
n49_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 688], rax
                        mov              qword ptr [rsp + 696], rdx
                                                                                        jmp   n55_call_α
#-----------------------------------------------------------------------------------------------------------------------
n50_call_α:
                        mov              rax, qword ptr [rsp + 768]
                        mov              qword ptr [rsp + 736], rax
                        mov              rax, qword ptr [rsp + 776]
                        mov              qword ptr [rsp + 744], rax
                        .section         .rodata
.Lrkfn82:               .string          "snext"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn82]
                        lea              rsi, [rsp + 736]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 720], rax
                        mov              qword ptr [rsp + 728], rdx
                        cmp              eax, 99
                                                                                        je    n51_lit_string_α
                                                                                        jmp   n56_assign_α
n50_call_β:
                                                                                        jmp   n51_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n51_lit_string_α:
                        mov              qword ptr [rsp + 832], 1
                        mov              rax, qword ptr [rip + .Lx83_0]
                        mov              qword ptr [rsp + 840], rax
                                                                                        jmp   n57_call_α
.Lx83_0:
                        .quad            .Lx83_0_s
.Lx83_0_s:
                        .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n52_assign_α:
                        mov              rax, qword ptr [rsp + 448]
                        mov              rdx, qword ptr [rsp + 456]
                        mov              qword ptr [1879052336], rax
                        mov              qword ptr [1879052344], rdx
                                                                                        jmp   n48_var_α
#-----------------------------------------------------------------------------------------------------------------------
n53_call_α:
                        mov              rax, qword ptr [rsp + 560]
                        mov              qword ptr [rsp + 528], rax
                        mov              rax, qword ptr [rsp + 568]
                        mov              qword ptr [rsp + 536], rax
                        .section         .rodata
.Lrkfn86:               .string          "snext"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn86]
                        lea              rsi, [rsp + 528]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 512], rax
                        mov              qword ptr [rsp + 520], rdx
                        cmp              eax, 99
                                                                                        je    n54_op14_α
                                                                                        jmp   n58_assign_α
n53_call_β:
                                                                                        jmp   n54_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n54_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n55_call_α:
                        mov              rax, qword ptr [rsp + 688]
                        mov              qword ptr [rsp + 656], rax
                        mov              rax, qword ptr [rsp + 696]
                        mov              qword ptr [rsp + 664], rax
                        .section         .rodata
.Lrkfn90:               .string          "sval"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn90]
                        lea              rsi, [rsp + 656]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 640], rax
                        mov              qword ptr [rsp + 648], rdx
                        cmp              eax, 99
                                                                                        je    n46_var_α
                                                                                        jmp   n59_assign_var_α
n55_call_β:
                                                                                        jmp   n46_var_α
#-----------------------------------------------------------------------------------------------------------------------
n56_assign_α:
                        mov              rax, qword ptr [rsp + 720]
                        mov              rdx, qword ptr [rsp + 728]
                        mov              qword ptr [1879052432], rax
                        mov              qword ptr [1879052440], rdx
                                                                                        jmp   n51_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n57_call_α:
                        mov              rax, qword ptr [rsp + 832]
                        mov              qword ptr [rsp + 800], rax
                        mov              rax, qword ptr [rsp + 840]
                        mov              qword ptr [rsp + 808], rax
                        .section         .rodata
.Lrkfn93:               .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn93]
                        lea              rsi, [rsp + 800]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 784], rax
                        mov              qword ptr [rsp + 792], rdx
                        cmp              eax, 99
                                                                                        je    n61_lit_string_α
                                                                                        jmp   n60_assign_α
n57_call_β:
                                                                                        jmp   n61_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n58_assign_α:
                        mov              rax, qword ptr [rsp + 512]
                        mov              rdx, qword ptr [rsp + 520]
                        mov              qword ptr [1879052432], rax
                        mov              qword ptr [1879052440], rdx
                                                                                        jmp   n54_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n59_assign_var_α:
                        mov              rdi, qword ptr [rsp + 576]
                        mov              rsi, qword ptr [rsp + 584]
                        mov              rdx, qword ptr [rsp + 640]
                        mov              rcx, qword ptr [rsp + 648]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n46_var_α
                        mov              qword ptr [rsp + 704], rax
                        mov              qword ptr [rsp + 712], rdx
                                                                                        jmp   n46_var_α
#-----------------------------------------------------------------------------------------------------------------------
n60_assign_α:
                        mov              rax, qword ptr [rsp + 784]
                        mov              rdx, qword ptr [rsp + 792]
                        mov              qword ptr [1879052336], rax
                        mov              qword ptr [1879052344], rdx
                                                                                        jmp   n61_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n61_lit_string_α:
                        mov              qword ptr [rsp + 32], 1
                        mov              rax, qword ptr [rip + .Lx97_0]
                        mov              qword ptr [rsp + 40], rax
                                                                                        jmp   n62_call_α
.Lx97_0:
                        .quad            .Lx97_0_s
.Lx97_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n62_call_α:
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 72], rax
                        .section         .rodata
.Lrkfn99:               .string          "SNO$NRET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn99]
                        lea              rsi, [rsp + 64]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 48], rax
                        mov              qword ptr [rsp + 56], rdx
                        cmp              eax, 99
                                                                                        je    n40_op14_α
                                                                                        jmp   n54_op14_α
n62_call_β:
                                                                                        jmp   n40_op14_α
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_pop_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_pop_β:
                                                                                        jmp   proc_LBL__stack_pop_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_pop_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 2840]
                        lea              rsp, [rbp + 2864]
                        mov              rbp, [rbp + 2856]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_pop_ω:
                        mov              rax, [rbp + 2848]
                        lea              rsp, [rbp + 2864]
                        mov              rbp, [rbp + 2856]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_LBL__stack_peek_α
proc_LBL__stack_peek_α:
                        .global          proc_LBL__stack_peek_α
                        .global          proc_LBL__stack_peek_β
                        .global          proc_LBL__stack_peek_γ
                        .global          proc_LBL__stack_peek_ω
                        sub              rsp, 2864
                        mov              [rsp + 2840], rcx
                        mov              [rsp + 2848], rdx
                        mov              [rsp + 2856], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 2480], 0
                        mov              qword ptr [rsp + 2488], 0
                        mov              qword ptr [rsp + 2560], 0
                        mov              qword ptr [rsp + 2568], 0
proc_LBL__stack_peek_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n100_goto_α:
                                                                                        jmp   n101_var_α
n100_goto_β:
                                                                                        jmp   proc_LBL__stack_peek_ω
#-----------------------------------------------------------------------------------------------------------------------
n101_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 896], rax
                        mov              qword ptr [rsp + 904], rdx
                                                                                        jmp   n102_call_α
#-----------------------------------------------------------------------------------------------------------------------
n102_call_α:
                        mov              rax, qword ptr [rsp + 896]
                        mov              qword ptr [rsp + 864], rax
                        mov              rax, qword ptr [rsp + 904]
                        mov              qword ptr [rsp + 872], rax
                        .section         .rodata
.Lrkfn111:              .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn111]
                        lea              rsi, [rsp + 864]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 848], rax
                        mov              qword ptr [rsp + 856], rdx
                        cmp              eax, 99
                                                                                        je    n103_op14_α
                                                                                        jmp   n104_var_α
n102_call_β:
                                                                                        jmp   n103_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n103_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n104_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 960], rax
                        mov              qword ptr [rsp + 968], rdx
                                                                                        jmp   n105_call_α
#-----------------------------------------------------------------------------------------------------------------------
n105_call_α:
                        mov              rax, qword ptr [rsp + 960]
                        mov              qword ptr [rsp + 928], rax
                        mov              rax, qword ptr [rsp + 968]
                        mov              qword ptr [rsp + 936], rax
                        .section         .rodata
.Lrkfn116:              .string          "sval"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn116]
                        lea              rsi, [rsp + 928]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 912], rax
                        mov              qword ptr [rsp + 920], rdx
                        cmp              eax, 99
                                                                                        je    n106_op14_α
                                                                                        jmp   n107_assign_α
n105_call_β:
                                                                                        jmp   n106_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n106_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n107_assign_α:
                        mov              rax, qword ptr [rsp + 912]
                        mov              rdx, qword ptr [rsp + 920]
                        mov              qword ptr [1879052368], rax
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n106_op14_α
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_peek_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_peek_β:
                                                                                        jmp   proc_LBL__stack_peek_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_peek_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 2840]
                        lea              rsp, [rbp + 2864]
                        mov              rbp, [rbp + 2856]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_peek_ω:
                        mov              rax, [rbp + 2848]
                        lea              rsp, [rbp + 2864]
                        mov              rbp, [rbp + 2856]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_LBL__stack_top_α
proc_LBL__stack_top_α:
                        .global          proc_LBL__stack_top_α
                        .global          proc_LBL__stack_top_β
                        .global          proc_LBL__stack_top_γ
                        .global          proc_LBL__stack_top_ω
                        sub              rsp, 2864
                        mov              [rsp + 2840], rcx
                        mov              [rsp + 2848], rdx
                        mov              [rsp + 2856], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 2480], 0
                        mov              qword ptr [rsp + 2488], 0
                        mov              qword ptr [rsp + 2560], 0
                        mov              qword ptr [rsp + 2568], 0
proc_LBL__stack_top_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n120_goto_α:
                                                                                        jmp   n121_var_α
n120_goto_β:
                                                                                        jmp   proc_LBL__stack_top_ω
#-----------------------------------------------------------------------------------------------------------------------
n121_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 1024], rax
                        mov              qword ptr [rsp + 1032], rdx
                                                                                        jmp   n122_call_α
#-----------------------------------------------------------------------------------------------------------------------
n122_call_α:
                        mov              rax, qword ptr [rsp + 1024]
                        mov              qword ptr [rsp + 992], rax
                        mov              rax, qword ptr [rsp + 1032]
                        mov              qword ptr [rsp + 1000], rax
                        .section         .rodata
.Lrkfn133:              .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn133]
                        lea              rsi, [rsp + 992]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 976], rax
                        mov              qword ptr [rsp + 984], rdx
                        cmp              eax, 99
                                                                                        je    n123_op14_α
                                                                                        jmp   n124_var_α
n122_call_β:
                                                                                        jmp   n123_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n123_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n124_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 1040], rax
                        mov              qword ptr [rsp + 1048], rdx
                                                                                        jmp   n125_field_var_α
#-----------------------------------------------------------------------------------------------------------------------
n125_field_var_α:
                        mov              rdi, qword ptr [rip + .Lx137_0]
                        mov              rsi, qword ptr [rsp + 1040]
                        mov              rdx, qword ptr [rsp + 1048]
                        call             rt_field_var@PLT
                        cmp              eax, 99
                                                                                        je    n126_lit_string_α
                        mov              qword ptr [rsp + 1056], rax
                        mov              qword ptr [rsp + 1064], rdx
                                                                                        jmp   n127_assign_α
.Lx137_0:
                        .quad            .Lx137_0_s
.Lx137_0_s:
                        .string          "sval"
#-----------------------------------------------------------------------------------------------------------------------
n126_lit_string_α:
                        mov              qword ptr [rsp + 32], 1
                        mov              rax, qword ptr [rip + .Lx138_0]
                        mov              qword ptr [rsp + 40], rax
                                                                                        jmp   n128_call_α
.Lx138_0:
                        .quad            .Lx138_0_s
.Lx138_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n127_assign_α:
                        mov              rax, qword ptr [rsp + 1056]
                        mov              rdx, qword ptr [rsp + 1064]
                        mov              qword ptr [1879052384], rax
                        mov              qword ptr [1879052392], rdx
                                                                                        jmp   n126_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n128_call_α:
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
                                                                                        je    n123_op14_α
                                                                                        jmp   n129_op14_α
n128_call_β:
                                                                                        jmp   n123_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n129_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_top_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_top_β:
                                                                                        jmp   proc_LBL__stack_top_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_top_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 2840]
                        lea              rsp, [rbp + 2864]
                        mov              rbp, [rbp + 2856]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_top_ω:
                        mov              rax, [rbp + 2848]
                        lea              rsp, [rbp + 2864]
                        mov              rbp, [rbp + 2856]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_LBL__stack_depth_α
proc_LBL__stack_depth_α:
                        .global          proc_LBL__stack_depth_α
                        .global          proc_LBL__stack_depth_β
                        .global          proc_LBL__stack_depth_γ
                        .global          proc_LBL__stack_depth_ω
                        sub              rsp, 2864
                        mov              [rsp + 2840], rcx
                        mov              [rsp + 2848], rdx
                        mov              [rsp + 2856], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 2480], 0
                        mov              qword ptr [rsp + 2488], 0
                        mov              qword ptr [rsp + 2560], 0
                        mov              qword ptr [rsp + 2568], 0
proc_LBL__stack_depth_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n144_goto_α:
                                                                                        jmp   n145_lit_integer_α
n144_goto_β:
                                                                                        jmp   proc_LBL__stack_depth_ω
#-----------------------------------------------------------------------------------------------------------------------
n145_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx160_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n146_assign_α
.Lx160_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n146_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052400], rax
                        mov              qword ptr [1879052408], rdx
                                                                                        jmp   n147_var_α
#-----------------------------------------------------------------------------------------------------------------------
n147_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n148_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n148_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052416], rax
                        mov              qword ptr [1879052424], rdx
                                                                                        jmp   n149_var_α
#-----------------------------------------------------------------------------------------------------------------------
n149_var_α:
                        mov              rax, qword ptr [1879052416]
                        mov              rdx, qword ptr [1879052424]
                        mov              qword ptr [rsp + 1152], rax
                        mov              qword ptr [rsp + 1160], rdx
                                                                                        jmp   n150_call_α
#-----------------------------------------------------------------------------------------------------------------------
n150_call_α:
                        mov              rax, qword ptr [rsp + 1152]
                        mov              qword ptr [rsp + 1120], rax
                        mov              rax, qword ptr [rsp + 1160]
                        mov              qword ptr [rsp + 1128], rax
                        .section         .rodata
.Lrkfn166:              .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn166]
                        lea              rsi, [rsp + 1120]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1104], rax
                        mov              qword ptr [rsp + 1112], rdx
                        cmp              eax, 99
                                                                                        je    n151_op14_α
                                                                                        jmp   n152_var_α
n150_call_β:
                                                                                        jmp   n151_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n151_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n152_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052400]
                        mov              rdx, qword ptr [1879052408]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n153_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n153_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx170_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n155_binop_α
.Lx170_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n154_var_α:
                        mov              rax, qword ptr [1879052416]
                        mov              rdx, qword ptr [1879052424]
                        mov              qword ptr [rsp + 1264], rax
                        mov              qword ptr [rsp + 1272], rdx
                                                                                        jmp   n156_call_α
#-----------------------------------------------------------------------------------------------------------------------
n155_binop_α:
                        mov              eax, dword ptr [rsp + 16]
                        cmp              eax, 100
                                                                                        je    .Lx172_0
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, 100
                                                                                        je    .Lx172_0
                        mov              eax, dword ptr [rsp + 16]
                        cmp              eax, 6
                                                                                        jne   .Lx172_2
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, 6
                                                                                        jne   .Lx172_2
                        mov              rax, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rsp + 8]
                        add              rax, rcx
                        add              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n157_assign_α
.Lx172_0:
                        mov              rdi, qword ptr [rsp + 16]
                        mov              rsi, qword ptr [rsp + 24]
                        mov              rdx, qword ptr [rsp + 0]
                        mov              rcx, qword ptr [rsp + 8]
                        mov              r8d, 0
                        lea              r9, [rsp + 16]
                        call             rt_binop_overload@PLT
                        test             eax, eax
                                                                                        je    .Lx172_2
                        add              rsp, 16
                                                                                        jmp   n157_assign_α
.Lx172_2:
                        mov              rdi, qword ptr [rsp + 16]
                        mov              rsi, qword ptr [rsp + 24]
                        mov              rdx, qword ptr [rsp + 0]
                        mov              rcx, qword ptr [rsp + 8]
                        mov              r8d, 0
                        call             rt_num_arith@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx172_240
                        add              rsp, 32
                                                                                        jmp   n154_var_α
.Lx172_240:
                        add              rsp, 16
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n157_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n156_call_α:
                        mov              rax, qword ptr [rsp + 1264]
                        mov              qword ptr [rsp + 1232], rax
                        mov              rax, qword ptr [rsp + 1272]
                        mov              qword ptr [rsp + 1240], rax
                        .section         .rodata
.Lrkfn174:              .string          "snext"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn174]
                        lea              rsi, [rsp + 1232]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1216], rax
                        mov              qword ptr [rsp + 1224], rdx
                        cmp              eax, 99
                                                                                        je    n149_var_α
                                                                                        jmp   n158_assign_α
n156_call_β:
                                                                                        jmp   n149_var_α
#-----------------------------------------------------------------------------------------------------------------------
n157_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052400], rax
                        mov              qword ptr [1879052408], rdx
                                                                                        jmp   n154_var_α
#-----------------------------------------------------------------------------------------------------------------------
n158_assign_α:
                        mov              rax, qword ptr [rsp + 1216]
                        mov              rdx, qword ptr [rsp + 1224]
                        mov              qword ptr [1879052416], rax
                        mov              qword ptr [1879052424], rdx
                                                                                        jmp   n149_var_α
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_depth_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_depth_β:
                                                                                        jmp   proc_LBL__stack_depth_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_depth_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 2840]
                        lea              rsp, [rbp + 2864]
                        mov              rbp, [rbp + 2856]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_depth_ω:
                        mov              rax, [rbp + 2848]
                        lea              rsp, [rbp + 2864]
                        mov              rbp, [rbp + 2856]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_stack_init_α
proc_stack_init_α:
                        .global          proc_stack_init_α
                        .global          proc_stack_init_β
                        .global          proc_stack_init_γ
                        .global          proc_stack_init_ω
                        sub              rsp, 2864
                        mov              [rsp + 2840], rcx
                        mov              [rsp + 2848], rdx
                        mov              [rsp + 2856], rbp
                        mov              rbp, rsp
                        mov              rdi, rsp
                        mov              ecx, 2832
                        xor              eax, eax
                        rep stosb
proc_stack_init_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n177_op14_α:
                        mov              rdi, qword ptr [rbp + 2840]
                        mov              rsi, qword ptr [rbp + 2848]
                        lea              rdx, [rbp + 2864]
                        mov              rcx, qword ptr [rbp + 2856]
                        call             rt_flat_wire_adopt@PLT
                                                                                        jmp   n178_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n178_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx182_0]
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx182_1
.Lx182_0:
                        .quad            .Lx182_0_s
.Lx182_0_s:
                        .string          "stack_init"
.Lx182_1:
                                                                                        jmp   proc_stack_init_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_init_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_init_β:
                                                                                        jmp   proc_stack_init_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_init_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 2840]
                        lea              rsp, [rbp + 2864]
                        mov              rbp, [rbp + 2856]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_init_ω:
                        mov              rax, [rbp + 2848]
                        lea              rsp, [rbp + 2864]
                        mov              rbp, [rbp + 2856]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_stack_push_α
proc_stack_push_α:
                        .global          proc_stack_push_α
                        .global          proc_stack_push_β
                        .global          proc_stack_push_γ
                        .global          proc_stack_push_ω
                        sub              rsp, 2864
                        mov              [rsp + 2840], rcx
                        mov              [rsp + 2848], rdx
                        mov              [rsp + 2856], rbp
                        mov              rbp, rsp
                        mov              rdi, rsp
                        mov              ecx, 2832
                        xor              eax, eax
                        rep stosb
proc_stack_push_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n183_op14_α:
                        mov              rdi, qword ptr [rbp + 2840]
                        mov              rsi, qword ptr [rbp + 2848]
                        lea              rdx, [rbp + 2864]
                        mov              rcx, qword ptr [rbp + 2856]
                        call             rt_flat_wire_adopt@PLT
                                                                                        jmp   n184_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n184_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx188_0]
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx188_1
.Lx188_0:
                        .quad            .Lx188_0_s
.Lx188_0_s:
                        .string          "stack_push"
.Lx188_1:
                                                                                        jmp   proc_stack_push_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_push_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_push_β:
                                                                                        jmp   proc_stack_push_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_push_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 2840]
                        lea              rsp, [rbp + 2864]
                        mov              rbp, [rbp + 2856]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_push_ω:
                        mov              rax, [rbp + 2848]
                        lea              rsp, [rbp + 2864]
                        mov              rbp, [rbp + 2856]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_stack_pop_α
proc_stack_pop_α:
                        .global          proc_stack_pop_α
                        .global          proc_stack_pop_β
                        .global          proc_stack_pop_γ
                        .global          proc_stack_pop_ω
                        sub              rsp, 2864
                        mov              [rsp + 2840], rcx
                        mov              [rsp + 2848], rdx
                        mov              [rsp + 2856], rbp
                        mov              rbp, rsp
                        mov              rdi, rsp
                        mov              ecx, 2832
                        xor              eax, eax
                        rep stosb
proc_stack_pop_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n189_op14_α:
                        mov              rdi, qword ptr [rbp + 2840]
                        mov              rsi, qword ptr [rbp + 2848]
                        lea              rdx, [rbp + 2864]
                        mov              rcx, qword ptr [rbp + 2856]
                        call             rt_flat_wire_adopt@PLT
                                                                                        jmp   n190_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n190_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx194_0]
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx194_1
.Lx194_0:
                        .quad            .Lx194_0_s
.Lx194_0_s:
                        .string          "stack_pop"
.Lx194_1:
                                                                                        jmp   proc_stack_pop_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_pop_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_pop_β:
                                                                                        jmp   proc_stack_pop_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_pop_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 2840]
                        lea              rsp, [rbp + 2864]
                        mov              rbp, [rbp + 2856]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_pop_ω:
                        mov              rax, [rbp + 2848]
                        lea              rsp, [rbp + 2864]
                        mov              rbp, [rbp + 2856]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_stack_peek_α
proc_stack_peek_α:
                        .global          proc_stack_peek_α
                        .global          proc_stack_peek_β
                        .global          proc_stack_peek_γ
                        .global          proc_stack_peek_ω
                        sub              rsp, 2864
                        mov              [rsp + 2840], rcx
                        mov              [rsp + 2848], rdx
                        mov              [rsp + 2856], rbp
                        mov              rbp, rsp
                        mov              rdi, rsp
                        mov              ecx, 2832
                        xor              eax, eax
                        rep stosb
proc_stack_peek_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n195_op14_α:
                        mov              rdi, qword ptr [rbp + 2840]
                        mov              rsi, qword ptr [rbp + 2848]
                        lea              rdx, [rbp + 2864]
                        mov              rcx, qword ptr [rbp + 2856]
                        call             rt_flat_wire_adopt@PLT
                                                                                        jmp   n196_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n196_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx200_0]
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx200_1
.Lx200_0:
                        .quad            .Lx200_0_s
.Lx200_0_s:
                        .string          "stack_peek"
.Lx200_1:
                                                                                        jmp   proc_stack_peek_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_peek_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_peek_β:
                                                                                        jmp   proc_stack_peek_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_peek_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 2840]
                        lea              rsp, [rbp + 2864]
                        mov              rbp, [rbp + 2856]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_peek_ω:
                        mov              rax, [rbp + 2848]
                        lea              rsp, [rbp + 2864]
                        mov              rbp, [rbp + 2856]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_stack_top_α
proc_stack_top_α:
                        .global          proc_stack_top_α
                        .global          proc_stack_top_β
                        .global          proc_stack_top_γ
                        .global          proc_stack_top_ω
                        sub              rsp, 2864
                        mov              [rsp + 2840], rcx
                        mov              [rsp + 2848], rdx
                        mov              [rsp + 2856], rbp
                        mov              rbp, rsp
                        mov              rdi, rsp
                        mov              ecx, 2832
                        xor              eax, eax
                        rep stosb
proc_stack_top_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n201_op14_α:
                        mov              rdi, qword ptr [rbp + 2840]
                        mov              rsi, qword ptr [rbp + 2848]
                        lea              rdx, [rbp + 2864]
                        mov              rcx, qword ptr [rbp + 2856]
                        call             rt_flat_wire_adopt@PLT
                                                                                        jmp   n202_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n202_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx206_0]
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx206_1
.Lx206_0:
                        .quad            .Lx206_0_s
.Lx206_0_s:
                        .string          "stack_top"
.Lx206_1:
                                                                                        jmp   proc_stack_top_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_top_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_top_β:
                                                                                        jmp   proc_stack_top_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_top_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 2840]
                        lea              rsp, [rbp + 2864]
                        mov              rbp, [rbp + 2856]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_top_ω:
                        mov              rax, [rbp + 2848]
                        lea              rsp, [rbp + 2864]
                        mov              rbp, [rbp + 2856]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_stack_depth_α
proc_stack_depth_α:
                        .global          proc_stack_depth_α
                        .global          proc_stack_depth_β
                        .global          proc_stack_depth_γ
                        .global          proc_stack_depth_ω
                        sub              rsp, 2864
                        mov              [rsp + 2840], rcx
                        mov              [rsp + 2848], rdx
                        mov              [rsp + 2856], rbp
                        mov              rbp, rsp
                        mov              rdi, rsp
                        mov              ecx, 2832
                        xor              eax, eax
                        rep stosb
proc_stack_depth_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n207_op14_α:
                        mov              rdi, qword ptr [rbp + 2840]
                        mov              rsi, qword ptr [rbp + 2848]
                        lea              rdx, [rbp + 2864]
                        mov              rcx, qword ptr [rbp + 2856]
                        call             rt_flat_wire_adopt@PLT
                                                                                        jmp   n208_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n208_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx212_0]
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx212_1
.Lx212_0:
                        .quad            .Lx212_0_s
.Lx212_0_s:
                        .string          "stack_depth"
.Lx212_1:
                                                                                        jmp   proc_stack_depth_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_depth_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_depth_β:
                                                                                        jmp   proc_stack_depth_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_depth_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 2840]
                        lea              rsp, [rbp + 2864]
                        mov              rbp, [rbp + 2856]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_depth_ω:
                        mov              rax, [rbp + 2848]
                        lea              rsp, [rbp + 2864]
                        mov              rbp, [rbp + 2856]
                                                                                        jmp   rax
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lclassspec0:           .string          "slink(snext,sval)"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lclassspec0]
                        call             record_register@PLT
                        .section         .rodata
.Lstartup_pname0:       .string          "LBL__stack_init"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_LBL__stack_init_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 2832
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname1:       .string          "LBL__stack_push"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname1]
                        lea              rsi, [rip + proc_LBL__stack_push_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 2832
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname2:       .string          "LBL__stack_pop"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname2]
                        lea              rsi, [rip + proc_LBL__stack_pop_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 2832
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname3:       .string          "LBL__stack_peek"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname3]
                        lea              rsi, [rip + proc_LBL__stack_peek_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 2832
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname4:       .string          "LBL__stack_top"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname4]
                        lea              rsi, [rip + proc_LBL__stack_top_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname4]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname4]
                        mov              esi, 2832
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname4]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname5:       .string          "LBL__stack_depth"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname5]
                        lea              rsi, [rip + proc_LBL__stack_depth_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 2832
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname6:       .string          "stack_init"
                        .align           8
.Lstartup_pnames6:
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname6]
                        lea              rsi, [rip + .Lstartup_pnames6]
                        mov              edx, 0
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname6]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname6]
                        lea              rsi, [rip + proc_stack_init_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname6]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname6]
                        mov              esi, 16
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname6]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname7:       .string          "stack_push"
.Lstartup_pp7_0:        .string          "x"
                        .align           8
.Lstartup_pnames7:
                        .quad            .Lstartup_pp7_0
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname7]
                        lea              rsi, [rip + .Lstartup_pnames7]
                        mov              edx, 1
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname7]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname7]
                        lea              rsi, [rip + proc_stack_push_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname7]
                        mov              esi, 1
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname7]
                        mov              esi, 16
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname7]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname8:       .string          "stack_pop"
.Lstartup_pp8_0:        .string          "var"
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
                        lea              rsi, [rip + proc_stack_pop_α]
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
.Lstartup_pname9:       .string          "stack_peek"
                        .align           8
.Lstartup_pnames9:
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname9]
                        lea              rsi, [rip + .Lstartup_pnames9]
                        mov              edx, 0
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname9]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname9]
                        lea              rsi, [rip + proc_stack_peek_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname9]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname9]
                        mov              esi, 16
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname9]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname10:      .string          "stack_top"
                        .align           8
.Lstartup_pnames10:
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname10]
                        lea              rsi, [rip + .Lstartup_pnames10]
                        mov              edx, 0
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname10]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname10]
                        lea              rsi, [rip + proc_stack_top_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname10]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname10]
                        mov              esi, 16
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname10]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname11:      .string          "stack_depth"
.Lstartup_pp11_0:       .string          "sd"
                        .align           8
.Lstartup_pnames11:
                        .quad            .Lstartup_pp11_0
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname11]
                        lea              rsi, [rip + .Lstartup_pnames11]
                        mov              edx, 1
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname11]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname11]
                        lea              rsi, [rip + proc_stack_depth_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname11]
                        mov              esi, 1
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
.Lgvan0:                .string          "stack_init"
.Lgvan1:                .string          "stack_push"
.Lgvan2:                .string          "x"
.Lgvan3:                .string          "stack_pop"
.Lgvan4:                .string          "var"
.Lgvan5:                .string          "stack_peek"
.Lgvan6:                .string          "stack_top"
.Lgvan7:                .string          "stack_depth"
.Lgvan8:                .string          "sd"
.Lgvan9:                .string          "stk"
.Lgvan10:               .string          "dummy"
.Lgvan11:               .string          "myvar"
.Lgvan12:               .string          "subject"
.Lgvan13:               .string          "w1"
.Lgvan14:               .string          "w2"
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
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             proc_startup
                        mov              edi, 15
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 15
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
                        sub              rsp, 2840
                        mov              rdi, rsp
                        mov              ecx, 2840
                        xor              eax, eax
                        rep stosb
                        mov              [rsp + 2832], rbp
                        mov              rbp, rsp
main_α_body:
#=======================================================================================================================
#         stack_push(99)
#-----------------------------------------------------------------------------------------------------------------------
n213_lit_string_α:
                        mov              qword ptr [rsp + 144], 1
                        mov              rax, qword ptr [rip + .Lx360_0]
                        mov              qword ptr [rsp + 152], rax
                                                                                        jmp   n231_call_α
n213_lit_string_β:
                                                                                        jmp   n233_lit_string_α
.Lx360_0:
                        .quad            .Lx360_0_s
.Lx360_0_s:
                        .string          "slink(snext,sval)"
#-----------------------------------------------------------------------------------------------------------------------
n214_goto_α:
                                                                                        jmp   n232_lit_string_α
n214_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n215_goto_α:
                                                                                        jmp   n233_lit_string_α
n215_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n216_goto_α:
                                                                                        jmp   n234_var_α
n216_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n217_goto_α:
                                                                                        jmp   n233_lit_string_α
n217_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n218_goto_α:
                                                                                        jmp   n235_var_α
n218_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n219_goto_α:
                                                                                        jmp   n236_var_α
n219_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n220_goto_α:
                                                                                        jmp   n233_lit_string_α
n220_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n221_goto_α:
                                                                                        jmp   n237_var_α
n221_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n222_goto_α:
                                                                                        jmp   n233_lit_string_α
n222_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n223_goto_α:
                                                                                        jmp   n238_var_α
n223_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n224_goto_α:
                                                                                        jmp   n233_lit_string_α
n224_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n225_goto_α:
                                                                                        jmp   n239_lit_integer_α
n225_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n226_goto_α:
                                                                                        jmp   n240_var_α
n226_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n227_goto_α:
                                                                                        jmp   n233_lit_string_α
n227_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n228_goto_α:
                                                                                        jmp   n241_lit_string_α
n228_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n229_goto_α:
                                                                                        jmp   n242_call_α
n229_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n230_goto_α:
                                                                                        jmp   n243_lit_string_α
n230_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n231_call_α:
                        mov              rax, qword ptr [rsp + 144]
                        mov              qword ptr [rsp + 112], rax
                        mov              rax, qword ptr [rsp + 152]
                        mov              qword ptr [rsp + 120], rax
                        .section         .rodata
.Lrkfn379:              .string          "DATA"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn379]
                        lea              rsi, [rsp + 112]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 96], rax
                        mov              qword ptr [rsp + 104], rdx
                        cmp              eax, 99
                                                                                        je    n233_lit_string_α
                                                                                        jmp   n233_lit_string_α
n231_call_β:
                                                                                        jmp   n233_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n232_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              rax, qword ptr [rip + .Lx380_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n244_assign_α
n232_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n256_op14_α
.Lx380_0:
                        .quad            .Lx380_0_s
.Lx380_0_s:
                        .string          ""
#=======================================================================================================================
#         stack_push(99)
#         OUTPUT = myvar                  ;* 99
#         subject = 'hello world'
#         OUTPUT = stack_pop()            ;* hello
#-----------------------------------------------------------------------------------------------------------------------
n233_lit_string_α:
                        mov              qword ptr [rsp + 1344], 1
                        mov              rax, qword ptr [rip + .Lx381_0]
                        mov              qword ptr [rsp + 1352], rax
                                                                                        jmp   n245_lit_integer_α
n233_lit_string_β:
                                                                                        jmp   n273_call_α
.Lx381_0:
                        .quad            .Lx381_0_s
.Lx381_0_s:
                        .string          "TRIM"
#-----------------------------------------------------------------------------------------------------------------------
n234_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 256], rax
                        mov              qword ptr [rsp + 264], rdx
                                                                                        jmp   n246_var_α
n234_var_β:
                                                                                        jmp   n247_var_α
#-----------------------------------------------------------------------------------------------------------------------
n235_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 368], rax
                        mov              qword ptr [rsp + 376], rdx
                                                                                        jmp   n248_call_α
n235_var_β:
                                                                                        jmp   n249_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n236_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 624], rax
                        mov              qword ptr [rsp + 632], rdx
                                                                                        jmp   n250_call_α
n236_var_β:
                                                                                        jmp   n251_var_α
#-----------------------------------------------------------------------------------------------------------------------
n237_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 896], rax
                        mov              qword ptr [rsp + 904], rdx
                                                                                        jmp   n252_call_α
n237_var_β:
                                                                                        jmp   n249_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n238_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 1024], rax
                        mov              qword ptr [rsp + 1032], rdx
                                                                                        jmp   n253_call_α
n238_var_β:
                                                                                        jmp   n249_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n239_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx387_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n254_assign_α
n239_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n270_var_α
.Lx387_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n240_var_α:
                        mov              rax, qword ptr [1879052416]
                        mov              rdx, qword ptr [1879052424]
                        mov              qword ptr [rsp + 1152], rax
                        mov              qword ptr [rsp + 1160], rdx
                                                                                        jmp   n255_call_α
n240_var_β:
                                                                                        jmp   n256_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n241_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              rax, qword ptr [rip + .Lx389_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n257_assign_α
n241_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n242_call_α
.Lx389_0:
                        .quad            .Lx389_0_s
.Lx389_0_s:
                        .string          "FAIL: empty pop should FRETURN"
#-----------------------------------------------------------------------------------------------------------------------
n242_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx391_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx391_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx391_6]
                        lea              rdx, [rip + .Lx391_7]
                                                                                        jmp   rax
.Lx391_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052296], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx391_2
.Lx391_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052296], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx391_2
.Lx391_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx391_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx391_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx391_3]
                        lea              rdx, [rip + .Lx391_4]
                                                                                        jmp   rax
.Lx391_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx391_2
.Lx391_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx391_2
.Lx391_1:
                        call             rt_faildescr@PLT
.Lx391_2:
                        mov              qword ptr [rsp + 1856], rax
                        mov              qword ptr [rsp + 1864], rdx
                        cmp              eax, 99
                                                                                        je    n258_lit_string_α
                                                                                        jmp   n258_lit_string_α
n242_call_β:
                                                                                        jmp   n258_lit_string_α
.Lx391_0:
                        .quad            .Lx391_0_s
.Lx391_0_s:
                        .string          "stack_init"
#-----------------------------------------------------------------------------------------------------------------------
n243_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              rax, qword ptr [rip + .Lx392_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n259_assign_α
n243_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   main_γ
.Lx392_0:
                        .quad            .Lx392_0_s
.Lx392_0_s:
                        .string          "FAIL: pattern match failed"
#-----------------------------------------------------------------------------------------------------------------------
n244_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052432], rax
                        mov              qword ptr [1879052440], rdx
                                                                                        jmp   n256_op14_α
n244_assign_β:
                                                                                        jmp   n256_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n245_lit_integer_α:
                        mov              qword ptr [rsp + 1360], 6
                        mov              rax, qword ptr [rip + .Lx394_0]
                        mov              qword ptr [rsp + 1368], rax
                                                                                        jmp   n260_call_α
n245_lit_integer_β:
                                                                                        jmp   n273_call_α
.Lx394_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n246_var_α:
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 272], rax
                        mov              qword ptr [rsp + 280], rdx
                                                                                        jmp   n261_call_α
n246_var_β:
                                                                                        jmp   n247_var_α
#=======================================================================================================================
#         stack_push(w1)
#-----------------------------------------------------------------------------------------------------------------------
n247_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 288], rax
                        mov              qword ptr [rsp + 296], rdx
                                                                                        jmp   n262_field_var_α
n247_var_β:
                                                                                        jmp   n263_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n248_call_α:
                        mov              rax, qword ptr [rsp + 368]
                        mov              qword ptr [rsp + 336], rax
                        mov              rax, qword ptr [rsp + 376]
                        mov              qword ptr [rsp + 344], rax
                        .section         .rodata
.Lrkfn398:              .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn398]
                        lea              rsi, [rsp + 336]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 320], rax
                        mov              qword ptr [rsp + 328], rdx
                        cmp              eax, 99
                                                                                        je    n249_op14_α
                                                                                        jmp   n264_var_α
n248_call_β:
                                                                                        jmp   n249_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n249_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n250_call_α:
                        mov              rax, qword ptr [rsp + 624]
                        mov              qword ptr [rsp + 592], rax
                        mov              rax, qword ptr [rsp + 632]
                        mov              qword ptr [rsp + 600], rax
                        .section         .rodata
.Lrkfn402:              .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn402]
                        lea              rsi, [rsp + 592]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 576], rax
                        mov              qword ptr [rsp + 584], rdx
                        cmp              eax, 99
                                                                                        je    n251_var_α
                                                                                        jmp   n265_var_α
n250_call_β:
                                                                                        jmp   n251_var_α
#-----------------------------------------------------------------------------------------------------------------------
n251_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 768], rax
                        mov              qword ptr [rsp + 776], rdx
                                                                                        jmp   n266_call_α
n251_var_β:
                                                                                        jmp   n267_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n252_call_α:
                        mov              rax, qword ptr [rsp + 896]
                        mov              qword ptr [rsp + 864], rax
                        mov              rax, qword ptr [rsp + 904]
                        mov              qword ptr [rsp + 872], rax
                        .section         .rodata
.Lrkfn405:              .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn405]
                        lea              rsi, [rsp + 864]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 848], rax
                        mov              qword ptr [rsp + 856], rdx
                        cmp              eax, 99
                                                                                        je    n249_op14_α
                                                                                        jmp   n268_var_α
n252_call_β:
                                                                                        jmp   n249_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n253_call_α:
                        mov              rax, qword ptr [rsp + 1024]
                        mov              qword ptr [rsp + 992], rax
                        mov              rax, qword ptr [rsp + 1032]
                        mov              qword ptr [rsp + 1000], rax
                        .section         .rodata
.Lrkfn407:              .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn407]
                        lea              rsi, [rsp + 992]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 976], rax
                        mov              qword ptr [rsp + 984], rdx
                        cmp              eax, 99
                                                                                        je    n249_op14_α
                                                                                        jmp   n269_var_α
n253_call_β:
                                                                                        jmp   n249_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n254_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052400], rax
                        mov              qword ptr [1879052408], rdx
                                                                                        jmp   n270_var_α
n254_assign_β:
                                                                                        jmp   n270_var_α
#-----------------------------------------------------------------------------------------------------------------------
n255_call_α:
                        mov              rax, qword ptr [rsp + 1152]
                        mov              qword ptr [rsp + 1120], rax
                        mov              rax, qword ptr [rsp + 1160]
                        mov              qword ptr [rsp + 1128], rax
                        .section         .rodata
.Lrkfn410:              .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn410]
                        lea              rsi, [rsp + 1120]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1104], rax
                        mov              qword ptr [rsp + 1112], rdx
                        cmp              eax, 99
                                                                                        je    n256_op14_α
                                                                                        jmp   n271_var_α
n255_call_β:
                                                                                        jmp   n256_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n256_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n257_assign_α:
                        mov              rsi, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx413_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n242_call_α
n257_assign_β:
                                                                                        jmp   n242_call_α
.Lx413_0:
                        .quad            .Lx413_0_s
.Lx413_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n258_lit_string_α:
                        mov              qword ptr [rsp + 1936], 1
                        mov              rax, qword ptr [rip + .Lx414_0]
                        mov              qword ptr [rsp + 1944], rax
                                                                                        jmp   n272_call_α
n258_lit_string_β:
                                                                                        jmp   n286_call_α
.Lx414_0:
                        .quad            .Lx414_0_s
.Lx414_0_s:
                        .string          "x"
#-----------------------------------------------------------------------------------------------------------------------
n259_assign_α:
                        mov              rsi, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx415_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   main_γ
n259_assign_β:
                                                                                        jmp   main_γ
.Lx415_0:
                        .quad            .Lx415_0_s
.Lx415_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n260_call_α:
                        mov              rax, qword ptr [rsp + 1344]
                        mov              qword ptr [rsp + 1296], rax
                        mov              rax, qword ptr [rsp + 1352]
                        mov              qword ptr [rsp + 1304], rax
                        mov              rax, qword ptr [rsp + 1360]
                        mov              qword ptr [rsp + 1312], rax
                        mov              rax, qword ptr [rsp + 1368]
                        mov              qword ptr [rsp + 1320], rax
                        .section         .rodata
.Lrkfn417:              .string          "SNO$KWSET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn417]
                        lea              rsi, [rsp + 1296]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1280], rax
                        mov              qword ptr [rsp + 1288], rdx
                        cmp              eax, 99
                                                                                        je    n273_call_α
                                                                                        jmp   n273_call_α
n260_call_β:
                                                                                        jmp   n273_call_α
#-----------------------------------------------------------------------------------------------------------------------
n261_call_α:
                        mov              rax, qword ptr [rsp + 256]
                        mov              qword ptr [rsp + 208], rax
                        mov              rax, qword ptr [rsp + 264]
                        mov              qword ptr [rsp + 216], rax
                        mov              rax, qword ptr [rsp + 272]
                        mov              qword ptr [rsp + 224], rax
                        mov              rax, qword ptr [rsp + 280]
                        mov              qword ptr [rsp + 232], rax
                        .section         .rodata
.Lrkfn419:              .string          "slink"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn419]
                        lea              rsi, [rsp + 208]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 192], rax
                        mov              qword ptr [rsp + 200], rdx
                        cmp              eax, 99
                                                                                        je    n247_var_α
                                                                                        jmp   n274_assign_α
n261_call_β:
                                                                                        jmp   n247_var_α
#-----------------------------------------------------------------------------------------------------------------------
n262_field_var_α:
                        mov              rdi, qword ptr [rip + .Lx420_0]
                        mov              rsi, qword ptr [rsp + 288]
                        mov              rdx, qword ptr [rsp + 296]
                        call             rt_field_var@PLT
                        cmp              eax, 99
                                                                                        je    n263_lit_string_α
                        mov              qword ptr [rsp + 304], rax
                        mov              qword ptr [rsp + 312], rdx
                                                                                        jmp   n275_assign_α
n262_field_var_β:
                                                                                        jmp   n263_lit_string_α
.Lx420_0:
                        .quad            .Lx420_0_s
.Lx420_0_s:
                        .string          "sval"
#-----------------------------------------------------------------------------------------------------------------------
n263_lit_string_α:
                        mov              qword ptr [rsp + 32], 1
                        mov              rax, qword ptr [rip + .Lx421_0]
                        mov              qword ptr [rsp + 40], rax
                                                                                        jmp   n276_call_α
n263_lit_string_β:
                                                                                        jmp   n249_op14_α
.Lx421_0:
                        .quad            .Lx421_0_s
.Lx421_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n264_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 432], rax
                        mov              qword ptr [rsp + 440], rdx
                                                                                        jmp   n277_call_α
n264_var_β:
                                                                                        jmp   n236_var_α
#-----------------------------------------------------------------------------------------------------------------------
n265_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 688], rax
                        mov              qword ptr [rsp + 696], rdx
                                                                                        jmp   n278_call_α
n265_var_β:
                                                                                        jmp   n251_var_α
#-----------------------------------------------------------------------------------------------------------------------
n266_call_α:
                        mov              rax, qword ptr [rsp + 768]
                        mov              qword ptr [rsp + 736], rax
                        mov              rax, qword ptr [rsp + 776]
                        mov              qword ptr [rsp + 744], rax
                        .section         .rodata
.Lrkfn425:              .string          "snext"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn425]
                        lea              rsi, [rsp + 736]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 720], rax
                        mov              qword ptr [rsp + 728], rdx
                        cmp              eax, 99
                                                                                        je    n267_lit_string_α
                                                                                        jmp   n279_assign_α
n266_call_β:
                                                                                        jmp   n267_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n267_lit_string_α:
                        mov              qword ptr [rsp + 832], 1
                        mov              rax, qword ptr [rip + .Lx426_0]
                        mov              qword ptr [rsp + 840], rax
                                                                                        jmp   n280_call_α
n267_lit_string_β:
                                                                                        jmp   n263_lit_string_α
.Lx426_0:
                        .quad            .Lx426_0_s
.Lx426_0_s:
                        .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n268_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 960], rax
                        mov              qword ptr [rsp + 968], rdx
                                                                                        jmp   n281_call_α
n268_var_β:
                                                                                        jmp   n256_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n269_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 1040], rax
                        mov              qword ptr [rsp + 1048], rdx
                                                                                        jmp   n282_field_var_α
n269_var_β:
                                                                                        jmp   n263_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n270_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n283_assign_α
n270_var_β:
                        add              rsp, 16
                                                                                        jmp   n240_var_α
#-----------------------------------------------------------------------------------------------------------------------
n271_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052400]
                        mov              rdx, qword ptr [1879052408]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n284_lit_integer_α
n271_var_β:
                        add              rsp, 16
                                                                                        jmp   n285_var_α
#-----------------------------------------------------------------------------------------------------------------------
n272_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx432_0]
                        mov              esi, 1
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx432_5
                        mov              rax, qword ptr [rsp + 1936]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 1944]
                        mov              qword ptr [1879052328], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx432_6]
                        lea              rdx, [rip + .Lx432_7]
                                                                                        jmp   rax
.Lx432_6:
                        mov              rdi, qword ptr [1879052304]
                        mov              rsi, qword ptr [1879052312]
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
                                                                                        jmp   .Lx432_2
.Lx432_7:
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
                                                                                        jmp   .Lx432_2
.Lx432_5:
                        add              rsp, 32
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx432_20
                        mov              rax, qword ptr [rsp + 1936]
                        mov              rdx, qword ptr [rsp + 1944]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx432_21
.Lx432_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 1936]
                        mov              rdx, qword ptr [rsp + 1944]
                        call             rt_arg_stage@PLT
.Lx432_21:
                        mov              rdi, qword ptr [rip + .Lx432_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx432_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx432_3]
                        lea              rdx, [rip + .Lx432_4]
                                                                                        jmp   rax
.Lx432_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx432_2
.Lx432_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx432_2
.Lx432_1:
                        call             rt_faildescr@PLT
.Lx432_2:
                        mov              qword ptr [rsp + 1888], rax
                        mov              qword ptr [rsp + 1896], rdx
                        cmp              eax, 99
                                                                                        je    n286_call_α
                                                                                        jmp   n286_call_α
n272_call_β:
                                                                                        jmp   n286_call_α
.Lx432_0:
                        .quad            .Lx432_0_s
.Lx432_0_s:
                        .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n273_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx434_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx434_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx434_6]
                        lea              rdx, [rip + .Lx434_7]
                                                                                        jmp   rax
.Lx434_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052296], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx434_2
.Lx434_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052296], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx434_2
.Lx434_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx434_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx434_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx434_3]
                        lea              rdx, [rip + .Lx434_4]
                                                                                        jmp   rax
.Lx434_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx434_2
.Lx434_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx434_2
.Lx434_1:
                        call             rt_faildescr@PLT
.Lx434_2:
                        mov              qword ptr [rsp + 1376], rax
                        mov              qword ptr [rsp + 1384], rdx
                        cmp              eax, 99
                                                                                        je    n287_lit_string_α
                                                                                        jmp   n287_lit_string_α
n273_call_β:
                                                                                        jmp   n287_lit_string_α
.Lx434_0:
                        .quad            .Lx434_0_s
.Lx434_0_s:
                        .string          "stack_init"
#-----------------------------------------------------------------------------------------------------------------------
n274_assign_α:
                        mov              rax, qword ptr [rsp + 192]
                        mov              rdx, qword ptr [rsp + 200]
                        mov              qword ptr [1879052432], rax
                        mov              qword ptr [1879052440], rdx
                                                                                        jmp   n247_var_α
n274_assign_β:
                                                                                        jmp   n247_var_α
#-----------------------------------------------------------------------------------------------------------------------
n275_assign_α:
                        mov              rax, qword ptr [rsp + 304]
                        mov              rdx, qword ptr [rsp + 312]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   n263_lit_string_α
n275_assign_β:
                                                                                        jmp   n263_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n276_call_α:
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 72], rax
                        .section         .rodata
.Lrkfn438:              .string          "SNO$NRET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn438]
                        lea              rsi, [rsp + 64]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 48], rax
                        mov              qword ptr [rsp + 56], rdx
                        cmp              eax, 99
                                                                                        je    n249_op14_α
                                                                                        jmp   n256_op14_α
n276_call_β:
                                                                                        jmp   n249_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n277_call_α:
                        mov              rax, qword ptr [rsp + 432]
                        mov              qword ptr [rsp + 400], rax
                        mov              rax, qword ptr [rsp + 440]
                        mov              qword ptr [rsp + 408], rax
                        .section         .rodata
.Lrkfn440:              .string          "IDENT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn440]
                        lea              rsi, [rsp + 400]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 384], rax
                        mov              qword ptr [rsp + 392], rdx
                        cmp              eax, 99
                                                                                        je    n236_var_α
                                                                                        jmp   n288_var_α
n277_call_β:
                                                                                        jmp   n236_var_α
#-----------------------------------------------------------------------------------------------------------------------
n278_call_α:
                        mov              rax, qword ptr [rsp + 688]
                        mov              qword ptr [rsp + 656], rax
                        mov              rax, qword ptr [rsp + 696]
                        mov              qword ptr [rsp + 664], rax
                        .section         .rodata
.Lrkfn442:              .string          "sval"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn442]
                        lea              rsi, [rsp + 656]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 640], rax
                        mov              qword ptr [rsp + 648], rdx
                        cmp              eax, 99
                                                                                        je    n251_var_α
                                                                                        jmp   n289_assign_var_α
n278_call_β:
                                                                                        jmp   n251_var_α
#-----------------------------------------------------------------------------------------------------------------------
n279_assign_α:
                        mov              rax, qword ptr [rsp + 720]
                        mov              rdx, qword ptr [rsp + 728]
                        mov              qword ptr [1879052432], rax
                        mov              qword ptr [1879052440], rdx
                                                                                        jmp   n267_lit_string_α
n279_assign_β:
                                                                                        jmp   n267_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n280_call_α:
                        mov              rax, qword ptr [rsp + 832]
                        mov              qword ptr [rsp + 800], rax
                        mov              rax, qword ptr [rsp + 840]
                        mov              qword ptr [rsp + 808], rax
                        .section         .rodata
.Lrkfn445:              .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn445]
                        lea              rsi, [rsp + 800]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 784], rax
                        mov              qword ptr [rsp + 792], rdx
                        cmp              eax, 99
                                                                                        je    n263_lit_string_α
                                                                                        jmp   n290_assign_α
n280_call_β:
                                                                                        jmp   n263_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n281_call_α:
                        mov              rax, qword ptr [rsp + 960]
                        mov              qword ptr [rsp + 928], rax
                        mov              rax, qword ptr [rsp + 968]
                        mov              qword ptr [rsp + 936], rax
                        .section         .rodata
.Lrkfn447:              .string          "sval"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn447]
                        lea              rsi, [rsp + 928]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 912], rax
                        mov              qword ptr [rsp + 920], rdx
                        cmp              eax, 99
                                                                                        je    n256_op14_α
                                                                                        jmp   n291_assign_α
n281_call_β:
                                                                                        jmp   n256_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n282_field_var_α:
                        mov              rdi, qword ptr [rip + .Lx448_0]
                        mov              rsi, qword ptr [rsp + 1040]
                        mov              rdx, qword ptr [rsp + 1048]
                        call             rt_field_var@PLT
                        cmp              eax, 99
                                                                                        je    n263_lit_string_α
                        mov              qword ptr [rsp + 1056], rax
                        mov              qword ptr [rsp + 1064], rdx
                                                                                        jmp   n292_assign_α
n282_field_var_β:
                                                                                        jmp   n263_lit_string_α
.Lx448_0:
                        .quad            .Lx448_0_s
.Lx448_0_s:
                        .string          "sval"
#-----------------------------------------------------------------------------------------------------------------------
n283_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052416], rax
                        mov              qword ptr [1879052424], rdx
                                                                                        jmp   n240_var_α
n283_assign_β:
                                                                                        jmp   n240_var_α
#-----------------------------------------------------------------------------------------------------------------------
n284_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx450_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n293_binop_α
n284_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n285_var_α
.Lx450_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n285_var_α:
                        mov              rax, qword ptr [1879052416]
                        mov              rdx, qword ptr [1879052424]
                        mov              qword ptr [rsp + 1264], rax
                        mov              qword ptr [rsp + 1272], rdx
                                                                                        jmp   n294_call_α
n285_var_β:
                                                                                        jmp   n240_var_α
#-----------------------------------------------------------------------------------------------------------------------
n286_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052368]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052376]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx453_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx453_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx453_6]
                        lea              rdx, [rip + .Lx453_7]
                                                                                        jmp   rax
.Lx453_6:
                        mov              rdi, qword ptr [1879052368]
                        mov              rsi, qword ptr [1879052376]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052376], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx453_2
.Lx453_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052376], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx453_2
.Lx453_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx453_0]
                        mov              esi, 0
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
                        mov              qword ptr [rsp + 1952], rax
                        mov              qword ptr [rsp + 1960], rdx
                        cmp              eax, 99
                                                                                        je    n296_call_α
                                                                                        jmp   n295_assign_α
n286_call_β:
                                                                                        jmp   n296_call_α
.Lx453_0:
                        .quad            .Lx453_0_s
.Lx453_0_s:
                        .string          "stack_peek"
#-----------------------------------------------------------------------------------------------------------------------
n287_lit_string_α:
                        mov              qword ptr [rsp + 1456], 1
                        mov              rax, qword ptr [rip + .Lx454_0]
                        mov              qword ptr [rsp + 1464], rax
                                                                                        jmp   n297_call_α
n287_lit_string_β:
                                                                                        jmp   n304_lit_string_α
.Lx454_0:
                        .quad            .Lx454_0_s
.Lx454_0_s:
                        .string          "a"
#-----------------------------------------------------------------------------------------------------------------------
n288_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 496], rax
                        mov              qword ptr [rsp + 504], rdx
                                                                                        jmp   n298_call_α
n288_var_β:
                                                                                        jmp   n299_var_α
#-----------------------------------------------------------------------------------------------------------------------
n289_assign_var_α:
                        mov              rdi, qword ptr [rsp + 576]
                        mov              rsi, qword ptr [rsp + 584]
                        mov              rdx, qword ptr [rsp + 640]
                        mov              rcx, qword ptr [rsp + 648]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n251_var_α
                        mov              qword ptr [rsp + 704], rax
                        mov              qword ptr [rsp + 712], rdx
                                                                                        jmp   n251_var_α
n289_assign_var_β:
                                                                                        jmp   n251_var_α
#-----------------------------------------------------------------------------------------------------------------------
n290_assign_α:
                        mov              rax, qword ptr [rsp + 784]
                        mov              rdx, qword ptr [rsp + 792]
                        mov              qword ptr [1879052336], rax
                        mov              qword ptr [1879052344], rdx
                                                                                        jmp   n263_lit_string_α
n290_assign_β:
                                                                                        jmp   n263_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n291_assign_α:
                        mov              rax, qword ptr [rsp + 912]
                        mov              rdx, qword ptr [rsp + 920]
                        mov              qword ptr [1879052368], rax
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n256_op14_α
n291_assign_β:
                                                                                        jmp   n256_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n292_assign_α:
                        mov              rax, qword ptr [rsp + 1056]
                        mov              rdx, qword ptr [rsp + 1064]
                        mov              qword ptr [1879052384], rax
                        mov              qword ptr [1879052392], rdx
                                                                                        jmp   n263_lit_string_α
n292_assign_β:
                                                                                        jmp   n263_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n293_binop_α:
                        mov              eax, dword ptr [rsp + 16]
                        cmp              eax, 100
                                                                                        je    .Lx460_0
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, 100
                                                                                        je    .Lx460_0
                        mov              eax, dword ptr [rsp + 16]
                        cmp              eax, 6
                                                                                        jne   .Lx460_2
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, 6
                                                                                        jne   .Lx460_2
                        mov              rax, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rsp + 8]
                        add              rax, rcx
                        add              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n300_assign_α
.Lx460_0:
                        mov              rdi, qword ptr [rsp + 16]
                        mov              rsi, qword ptr [rsp + 24]
                        mov              rdx, qword ptr [rsp + 0]
                        mov              rcx, qword ptr [rsp + 8]
                        mov              r8d, 0
                        lea              r9, [rsp + 16]
                        call             rt_binop_overload@PLT
                        test             eax, eax
                                                                                        je    .Lx460_2
                        add              rsp, 16
                                                                                        jmp   n300_assign_α
.Lx460_2:
                        mov              rdi, qword ptr [rsp + 16]
                        mov              rsi, qword ptr [rsp + 24]
                        mov              rdx, qword ptr [rsp + 0]
                        mov              rcx, qword ptr [rsp + 8]
                        mov              r8d, 0
                        call             rt_num_arith@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx460_240
                        add              rsp, 32
                                                                                        jmp   n285_var_α
.Lx460_240:
                        add              rsp, 16
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n300_assign_α
n293_binop_β:
                        add              rsp, 32
                                                                                        jmp   n285_var_α
#-----------------------------------------------------------------------------------------------------------------------
n294_call_α:
                        mov              rax, qword ptr [rsp + 1264]
                        mov              qword ptr [rsp + 1232], rax
                        mov              rax, qword ptr [rsp + 1272]
                        mov              qword ptr [rsp + 1240], rax
                        .section         .rodata
.Lrkfn462:              .string          "snext"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn462]
                        lea              rsi, [rsp + 1232]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1216], rax
                        mov              qword ptr [rsp + 1224], rdx
                        cmp              eax, 99
                                                                                        je    n240_var_α
                                                                                        jmp   n301_assign_α
n294_call_β:
                                                                                        jmp   n240_var_α
#-----------------------------------------------------------------------------------------------------------------------
n295_assign_α:
                        mov              rsi, qword ptr [rsp + 1952]
                        mov              rdx, qword ptr [rsp + 1960]
                        mov              rdi, qword ptr [rip + .Lx463_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n296_call_α
n295_assign_β:
                                                                                        jmp   n296_call_α
.Lx463_0:
                        .quad            .Lx463_0_s
.Lx463_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n296_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052416]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052424]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052400]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052408]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx465_0]
                        mov              esi, 1
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx465_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx465_6]
                        lea              rdx, [rip + .Lx465_7]
                                                                                        jmp   rax
.Lx465_6:
                        mov              rdi, qword ptr [1879052400]
                        mov              rsi, qword ptr [1879052408]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052400], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052408], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052416], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052424], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx465_2
.Lx465_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052400], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052408], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052416], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052424], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx465_2
.Lx465_5:
                        add              rsp, 32
                        mov              rdi, qword ptr [rip + .Lx465_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx465_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx465_3]
                        lea              rdx, [rip + .Lx465_4]
                                                                                        jmp   rax
.Lx465_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx465_2
.Lx465_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx465_2
.Lx465_1:
                        call             rt_faildescr@PLT
.Lx465_2:
                        mov              qword ptr [rsp + 1984], rax
                        mov              qword ptr [rsp + 1992], rdx
                        cmp              eax, 99
                                                                                        je    n303_call_α
                                                                                        jmp   n302_assign_α
n296_call_β:
                                                                                        jmp   n303_call_α
.Lx465_0:
                        .quad            .Lx465_0_s
.Lx465_0_s:
                        .string          "stack_depth"
#-----------------------------------------------------------------------------------------------------------------------
n297_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx467_0]
                        mov              esi, 1
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx467_5
                        mov              rax, qword ptr [rsp + 1456]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 1464]
                        mov              qword ptr [1879052328], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx467_6]
                        lea              rdx, [rip + .Lx467_7]
                                                                                        jmp   rax
.Lx467_6:
                        mov              rdi, qword ptr [1879052304]
                        mov              rsi, qword ptr [1879052312]
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
                                                                                        jmp   .Lx467_2
.Lx467_7:
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
                                                                                        jmp   .Lx467_2
.Lx467_5:
                        add              rsp, 32
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx467_20
                        mov              rax, qword ptr [rsp + 1456]
                        mov              rdx, qword ptr [rsp + 1464]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx467_21
.Lx467_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 1456]
                        mov              rdx, qword ptr [rsp + 1464]
                        call             rt_arg_stage@PLT
.Lx467_21:
                        mov              rdi, qword ptr [rip + .Lx467_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx467_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx467_3]
                        lea              rdx, [rip + .Lx467_4]
                                                                                        jmp   rax
.Lx467_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx467_2
.Lx467_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx467_2
.Lx467_1:
                        call             rt_faildescr@PLT
.Lx467_2:
                        mov              qword ptr [rsp + 1408], rax
                        mov              qword ptr [rsp + 1416], rdx
                        cmp              eax, 99
                                                                                        je    n304_lit_string_α
                                                                                        jmp   n304_lit_string_α
n297_call_β:
                                                                                        jmp   n304_lit_string_α
.Lx467_0:
                        .quad            .Lx467_0_s
.Lx467_0_s:
                        .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n298_call_α:
                        mov              rax, qword ptr [rsp + 496]
                        mov              qword ptr [rsp + 464], rax
                        mov              rax, qword ptr [rsp + 504]
                        mov              qword ptr [rsp + 472], rax
                        .section         .rodata
.Lrkfn469:              .string          "sval"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn469]
                        lea              rsi, [rsp + 464]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 448], rax
                        mov              qword ptr [rsp + 456], rdx
                        cmp              eax, 99
                                                                                        je    n299_var_α
                                                                                        jmp   n305_assign_α
n298_call_β:
                                                                                        jmp   n299_var_α
#-----------------------------------------------------------------------------------------------------------------------
n299_var_α:
                        mov              rax, qword ptr [1879052432]
                        mov              rdx, qword ptr [1879052440]
                        mov              qword ptr [rsp + 560], rax
                        mov              qword ptr [rsp + 568], rdx
                                                                                        jmp   n306_call_α
n299_var_β:
                                                                                        jmp   n256_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n300_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052400], rax
                        mov              qword ptr [1879052408], rdx
                                                                                        jmp   n285_var_α
n300_assign_β:
                                                                                        jmp   n285_var_α
#-----------------------------------------------------------------------------------------------------------------------
n301_assign_α:
                        mov              rax, qword ptr [rsp + 1216]
                        mov              rdx, qword ptr [rsp + 1224]
                        mov              qword ptr [1879052416], rax
                        mov              qword ptr [1879052424], rdx
                                                                                        jmp   n240_var_α
n301_assign_β:
                                                                                        jmp   n240_var_α
#-----------------------------------------------------------------------------------------------------------------------
n302_assign_α:
                        mov              rsi, qword ptr [rsp + 1984]
                        mov              rdx, qword ptr [rsp + 1992]
                        mov              rdi, qword ptr [rip + .Lx473_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n303_call_α
n302_assign_β:
                                                                                        jmp   n303_call_α
.Lx473_0:
                        .quad            .Lx473_0_s
.Lx473_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n303_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052352]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052336]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx475_0]
                        mov              esi, 1
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx475_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx475_6]
                        lea              rdx, [rip + .Lx475_7]
                                                                                        jmp   rax
.Lx475_6:
                        mov              rdi, qword ptr [1879052336]
                        mov              rsi, qword ptr [1879052344]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052360], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx475_2
.Lx475_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052360], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx475_2
.Lx475_5:
                        add              rsp, 32
                        mov              rdi, qword ptr [rip + .Lx475_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx475_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx475_3]
                        lea              rdx, [rip + .Lx475_4]
                                                                                        jmp   rax
.Lx475_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx475_2
.Lx475_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx475_2
.Lx475_1:
                        call             rt_faildescr@PLT
.Lx475_2:
                        mov              qword ptr [rsp + 2016], rax
                        mov              qword ptr [rsp + 2024], rdx
                        cmp              eax, 99
                                                                                        je    n308_call_α
                                                                                        jmp   n307_assign_α
n303_call_β:
                                                                                        jmp   n308_call_α
.Lx475_0:
                        .quad            .Lx475_0_s
.Lx475_0_s:
                        .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n304_lit_string_α:
                        mov              qword ptr [rsp + 1520], 1
                        mov              rax, qword ptr [rip + .Lx476_0]
                        mov              qword ptr [rsp + 1528], rax
                                                                                        jmp   n309_call_α
n304_lit_string_β:
                                                                                        jmp   n312_lit_string_α
.Lx476_0:
                        .quad            .Lx476_0_s
.Lx476_0_s:
                        .string          "b"
#-----------------------------------------------------------------------------------------------------------------------
n305_assign_α:
                        mov              rax, qword ptr [rsp + 448]
                        mov              rdx, qword ptr [rsp + 456]
                        mov              qword ptr [1879052336], rax
                        mov              qword ptr [1879052344], rdx
                                                                                        jmp   n299_var_α
n305_assign_β:
                                                                                        jmp   n299_var_α
#-----------------------------------------------------------------------------------------------------------------------
n306_call_α:
                        mov              rax, qword ptr [rsp + 560]
                        mov              qword ptr [rsp + 528], rax
                        mov              rax, qword ptr [rsp + 568]
                        mov              qword ptr [rsp + 536], rax
                        .section         .rodata
.Lrkfn479:              .string          "snext"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn479]
                        lea              rsi, [rsp + 528]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 512], rax
                        mov              qword ptr [rsp + 520], rdx
                        cmp              eax, 99
                                                                                        je    n256_op14_α
                                                                                        jmp   n310_assign_α
n306_call_β:
                                                                                        jmp   n256_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n307_assign_α:
                        mov              rsi, qword ptr [rsp + 2016]
                        mov              rdx, qword ptr [rsp + 2024]
                        mov              rdi, qword ptr [rip + .Lx480_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n308_call_α
n307_assign_β:
                                                                                        jmp   n308_call_α
.Lx480_0:
                        .quad            .Lx480_0_s
.Lx480_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n308_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx482_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx482_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx482_6]
                        lea              rdx, [rip + .Lx482_7]
                                                                                        jmp   rax
.Lx482_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052296], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx482_2
.Lx482_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052296], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx482_2
.Lx482_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx482_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx482_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx482_3]
                        lea              rdx, [rip + .Lx482_4]
                                                                                        jmp   rax
.Lx482_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx482_2
.Lx482_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx482_2
.Lx482_1:
                        call             rt_faildescr@PLT
.Lx482_2:
                        mov              qword ptr [rsp + 2048], rax
                        mov              qword ptr [rsp + 2056], rdx
                        cmp              eax, 99
                                                                                        je    n311_lit_integer_α
                                                                                        jmp   n311_lit_integer_α
n308_call_β:
                                                                                        jmp   n311_lit_integer_α
.Lx482_0:
                        .quad            .Lx482_0_s
.Lx482_0_s:
                        .string          "stack_init"
#-----------------------------------------------------------------------------------------------------------------------
n309_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx484_0]
                        mov              esi, 1
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx484_5
                        mov              rax, qword ptr [rsp + 1520]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 1528]
                        mov              qword ptr [1879052328], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx484_6]
                        lea              rdx, [rip + .Lx484_7]
                                                                                        jmp   rax
.Lx484_6:
                        mov              rdi, qword ptr [1879052304]
                        mov              rsi, qword ptr [1879052312]
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
                                                                                        jmp   .Lx484_2
.Lx484_7:
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
                                                                                        jmp   .Lx484_2
.Lx484_5:
                        add              rsp, 32
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx484_20
                        mov              rax, qword ptr [rsp + 1520]
                        mov              rdx, qword ptr [rsp + 1528]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx484_21
.Lx484_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 1520]
                        mov              rdx, qword ptr [rsp + 1528]
                        call             rt_arg_stage@PLT
.Lx484_21:
                        mov              rdi, qword ptr [rip + .Lx484_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx484_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx484_3]
                        lea              rdx, [rip + .Lx484_4]
                                                                                        jmp   rax
.Lx484_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx484_2
.Lx484_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx484_2
.Lx484_1:
                        call             rt_faildescr@PLT
.Lx484_2:
                        mov              qword ptr [rsp + 1472], rax
                        mov              qword ptr [rsp + 1480], rdx
                        cmp              eax, 99
                                                                                        je    n312_lit_string_α
                                                                                        jmp   n312_lit_string_α
n309_call_β:
                                                                                        jmp   n312_lit_string_α
.Lx484_0:
                        .quad            .Lx484_0_s
.Lx484_0_s:
                        .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n310_assign_α:
                        mov              rax, qword ptr [rsp + 512]
                        mov              rdx, qword ptr [rsp + 520]
                        mov              qword ptr [1879052432], rax
                        mov              qword ptr [1879052440], rdx
                                                                                        jmp   n256_op14_α
n310_assign_β:
                                                                                        jmp   n256_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n311_lit_integer_α:
                        mov              qword ptr [rsp + 2128], 6
                        mov              rax, qword ptr [rip + .Lx486_0]
                        mov              qword ptr [rsp + 2136], rax
                                                                                        jmp   n313_call_α
n311_lit_integer_β:
                                                                                        jmp   n315_lit_integer_α
.Lx486_0:
                        .quad            42
#-----------------------------------------------------------------------------------------------------------------------
n312_lit_string_α:
                        mov              qword ptr [rsp + 1584], 1
                        mov              rax, qword ptr [rip + .Lx487_0]
                        mov              qword ptr [rsp + 1592], rax
                                                                                        jmp   n314_call_α
n312_lit_string_β:
                                                                                        jmp   n316_call_α
.Lx487_0:
                        .quad            .Lx487_0_s
.Lx487_0_s:
                        .string          "c"
#-----------------------------------------------------------------------------------------------------------------------
n313_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx489_0]
                        mov              esi, 1
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx489_5
                        mov              rax, qword ptr [rsp + 2128]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 2136]
                        mov              qword ptr [1879052328], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx489_6]
                        lea              rdx, [rip + .Lx489_7]
                                                                                        jmp   rax
.Lx489_6:
                        mov              rdi, qword ptr [1879052304]
                        mov              rsi, qword ptr [1879052312]
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
                                                                                        jmp   .Lx489_2
.Lx489_7:
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
                                                                                        jmp   .Lx489_2
.Lx489_5:
                        add              rsp, 32
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx489_20
                        mov              rax, qword ptr [rsp + 2128]
                        mov              rdx, qword ptr [rsp + 2136]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx489_21
.Lx489_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 2128]
                        mov              rdx, qword ptr [rsp + 2136]
                        call             rt_arg_stage@PLT
.Lx489_21:
                        mov              rdi, qword ptr [rip + .Lx489_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx489_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx489_3]
                        lea              rdx, [rip + .Lx489_4]
                                                                                        jmp   rax
.Lx489_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx489_2
.Lx489_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx489_2
.Lx489_1:
                        call             rt_faildescr@PLT
.Lx489_2:
                        mov              qword ptr [rsp + 2080], rax
                        mov              qword ptr [rsp + 2088], rdx
                        cmp              eax, 99
                                                                                        je    n315_lit_integer_α
                                                                                        jmp   n315_lit_integer_α
n313_call_β:
                                                                                        jmp   n315_lit_integer_α
.Lx489_0:
                        .quad            .Lx489_0_s
.Lx489_0_s:
                        .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n314_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx491_0]
                        mov              esi, 1
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx491_5
                        mov              rax, qword ptr [rsp + 1584]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 1592]
                        mov              qword ptr [1879052328], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx491_6]
                        lea              rdx, [rip + .Lx491_7]
                                                                                        jmp   rax
.Lx491_6:
                        mov              rdi, qword ptr [1879052304]
                        mov              rsi, qword ptr [1879052312]
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
                                                                                        jmp   .Lx491_2
.Lx491_7:
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
                                                                                        jmp   .Lx491_2
.Lx491_5:
                        add              rsp, 32
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx491_20
                        mov              rax, qword ptr [rsp + 1584]
                        mov              rdx, qword ptr [rsp + 1592]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx491_21
.Lx491_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 1584]
                        mov              rdx, qword ptr [rsp + 1592]
                        call             rt_arg_stage@PLT
.Lx491_21:
                        mov              rdi, qword ptr [rip + .Lx491_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx491_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx491_3]
                        lea              rdx, [rip + .Lx491_4]
                                                                                        jmp   rax
.Lx491_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx491_2
.Lx491_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx491_2
.Lx491_1:
                        call             rt_faildescr@PLT
.Lx491_2:
                        mov              qword ptr [rsp + 1536], rax
                        mov              qword ptr [rsp + 1544], rdx
                        cmp              eax, 99
                                                                                        je    n316_call_α
                                                                                        jmp   n316_call_α
n314_call_β:
                                                                                        jmp   n316_call_α
.Lx491_0:
                        .quad            .Lx491_0_s
.Lx491_0_s:
                        .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n315_lit_integer_α:
                        mov              qword ptr [rsp + 2192], 6
                        mov              rax, qword ptr [rip + .Lx492_0]
                        mov              qword ptr [rsp + 2200], rax
                                                                                        jmp   n317_call_α
n315_lit_integer_β:
                                                                                        jmp   n320_lit_string_α
.Lx492_0:
                        .quad            99
#-----------------------------------------------------------------------------------------------------------------------
n316_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052416]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052424]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052400]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052408]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx494_0]
                        mov              esi, 1
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx494_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx494_6]
                        lea              rdx, [rip + .Lx494_7]
                                                                                        jmp   rax
.Lx494_6:
                        mov              rdi, qword ptr [1879052400]
                        mov              rsi, qword ptr [1879052408]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052400], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052408], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052416], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052424], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx494_2
.Lx494_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052400], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052408], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052416], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052424], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx494_2
.Lx494_5:
                        add              rsp, 32
                        mov              rdi, qword ptr [rip + .Lx494_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx494_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx494_3]
                        lea              rdx, [rip + .Lx494_4]
                                                                                        jmp   rax
.Lx494_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx494_2
.Lx494_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx494_2
.Lx494_1:
                        call             rt_faildescr@PLT
.Lx494_2:
                        mov              qword ptr [rsp + 1600], rax
                        mov              qword ptr [rsp + 1608], rdx
                        cmp              eax, 99
                                                                                        je    n319_call_α
                                                                                        jmp   n318_assign_α
n316_call_β:
                                                                                        jmp   n319_call_α
.Lx494_0:
                        .quad            .Lx494_0_s
.Lx494_0_s:
                        .string          "stack_depth"
#-----------------------------------------------------------------------------------------------------------------------
n317_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx496_0]
                        mov              esi, 1
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx496_5
                        mov              rax, qword ptr [rsp + 2192]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 2200]
                        mov              qword ptr [1879052328], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx496_6]
                        lea              rdx, [rip + .Lx496_7]
                                                                                        jmp   rax
.Lx496_6:
                        mov              rdi, qword ptr [1879052304]
                        mov              rsi, qword ptr [1879052312]
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
                                                                                        jmp   .Lx496_2
.Lx496_7:
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
                                                                                        jmp   .Lx496_2
.Lx496_5:
                        add              rsp, 32
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx496_20
                        mov              rax, qword ptr [rsp + 2192]
                        mov              rdx, qword ptr [rsp + 2200]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx496_21
.Lx496_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 2192]
                        mov              rdx, qword ptr [rsp + 2200]
                        call             rt_arg_stage@PLT
.Lx496_21:
                        mov              rdi, qword ptr [rip + .Lx496_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx496_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx496_3]
                        lea              rdx, [rip + .Lx496_4]
                                                                                        jmp   rax
.Lx496_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx496_2
.Lx496_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx496_2
.Lx496_1:
                        call             rt_faildescr@PLT
.Lx496_2:
                        mov              qword ptr [rsp + 2144], rax
                        mov              qword ptr [rsp + 2152], rdx
                        cmp              eax, 99
                                                                                        je    n320_lit_string_α
                                                                                        jmp   n320_lit_string_α
n317_call_β:
                                                                                        jmp   n320_lit_string_α
.Lx496_0:
                        .quad            .Lx496_0_s
.Lx496_0_s:
                        .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n318_assign_α:
                        mov              rsi, qword ptr [rsp + 1600]
                        mov              rdx, qword ptr [rsp + 1608]
                        mov              rdi, qword ptr [rip + .Lx497_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n319_call_α
n318_assign_β:
                                                                                        jmp   n319_call_α
.Lx497_0:
                        .quad            .Lx497_0_s
.Lx497_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n319_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052352]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052336]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx499_0]
                        mov              esi, 1
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx499_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx499_6]
                        lea              rdx, [rip + .Lx499_7]
                                                                                        jmp   rax
.Lx499_6:
                        mov              rdi, qword ptr [1879052336]
                        mov              rsi, qword ptr [1879052344]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052360], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx499_2
.Lx499_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052360], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx499_2
.Lx499_5:
                        add              rsp, 32
                        mov              rdi, qword ptr [rip + .Lx499_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx499_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx499_3]
                        lea              rdx, [rip + .Lx499_4]
                                                                                        jmp   rax
.Lx499_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx499_2
.Lx499_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx499_2
.Lx499_1:
                        call             rt_faildescr@PLT
.Lx499_2:
                        mov              qword ptr [rsp + 1632], rax
                        mov              qword ptr [rsp + 1640], rdx
                        cmp              eax, 99
                                                                                        je    n322_call_α
                                                                                        jmp   n321_assign_α
n319_call_β:
                                                                                        jmp   n322_call_α
.Lx499_0:
                        .quad            .Lx499_0_s
.Lx499_0_s:
                        .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n320_lit_string_α:
                        mov              qword ptr [rsp + 2256], 1
                        mov              rax, qword ptr [rip + .Lx500_0]
                        mov              qword ptr [rsp + 2264], rax
                                                                                        jmp   n323_call_α
n320_lit_string_β:
                                                                                        jmp   n326_var_α
.Lx500_0:
                        .quad            .Lx500_0_s
.Lx500_0_s:
                        .string          "myvar"
#-----------------------------------------------------------------------------------------------------------------------
n321_assign_α:
                        mov              rsi, qword ptr [rsp + 1632]
                        mov              rdx, qword ptr [rsp + 1640]
                        mov              rdi, qword ptr [rip + .Lx501_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n322_call_α
n321_assign_β:
                                                                                        jmp   n322_call_α
.Lx501_0:
                        .quad            .Lx501_0_s
.Lx501_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n322_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052352]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052336]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx503_0]
                        mov              esi, 1
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx503_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx503_6]
                        lea              rdx, [rip + .Lx503_7]
                                                                                        jmp   rax
.Lx503_6:
                        mov              rdi, qword ptr [1879052336]
                        mov              rsi, qword ptr [1879052344]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052360], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx503_2
.Lx503_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052360], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx503_2
.Lx503_5:
                        add              rsp, 32
                        mov              rdi, qword ptr [rip + .Lx503_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx503_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx503_3]
                        lea              rdx, [rip + .Lx503_4]
                                                                                        jmp   rax
.Lx503_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx503_2
.Lx503_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx503_2
.Lx503_1:
                        call             rt_faildescr@PLT
.Lx503_2:
                        mov              qword ptr [rsp + 1664], rax
                        mov              qword ptr [rsp + 1672], rdx
                        cmp              eax, 99
                                                                                        je    n325_call_α
                                                                                        jmp   n324_assign_α
n322_call_β:
                                                                                        jmp   n325_call_α
.Lx503_0:
                        .quad            .Lx503_0_s
.Lx503_0_s:
                        .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n323_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052352]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052336]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx505_0]
                        mov              esi, 1
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx505_5
                        mov              rax, qword ptr [rsp + 2256]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 2264]
                        mov              qword ptr [1879052360], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx505_6]
                        lea              rdx, [rip + .Lx505_7]
                                                                                        jmp   rax
.Lx505_6:
                        mov              rdi, qword ptr [1879052336]
                        mov              rsi, qword ptr [1879052344]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052360], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx505_2
.Lx505_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052360], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx505_2
.Lx505_5:
                        add              rsp, 32
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx505_20
                        mov              rax, qword ptr [rsp + 2256]
                        mov              rdx, qword ptr [rsp + 2264]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx505_21
.Lx505_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 2256]
                        mov              rdx, qword ptr [rsp + 2264]
                        call             rt_arg_stage@PLT
.Lx505_21:
                        mov              rdi, qword ptr [rip + .Lx505_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx505_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx505_3]
                        lea              rdx, [rip + .Lx505_4]
                                                                                        jmp   rax
.Lx505_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx505_2
.Lx505_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx505_2
.Lx505_1:
                        call             rt_faildescr@PLT
.Lx505_2:
                        mov              qword ptr [rsp + 2208], rax
                        mov              qword ptr [rsp + 2216], rdx
                        cmp              eax, 99
                                                                                        je    n326_var_α
                                                                                        jmp   n326_var_α
n323_call_β:
                                                                                        jmp   n326_var_α
.Lx505_0:
                        .quad            .Lx505_0_s
.Lx505_0_s:
                        .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n324_assign_α:
                        mov              rsi, qword ptr [rsp + 1664]
                        mov              rdx, qword ptr [rsp + 1672]
                        mov              rdi, qword ptr [rip + .Lx506_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n325_call_α
n324_assign_β:
                                                                                        jmp   n325_call_α
.Lx506_0:
                        .quad            .Lx506_0_s
.Lx506_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n325_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052416]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052424]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052400]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052408]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx508_0]
                        mov              esi, 1
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx508_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx508_6]
                        lea              rdx, [rip + .Lx508_7]
                                                                                        jmp   rax
.Lx508_6:
                        mov              rdi, qword ptr [1879052400]
                        mov              rsi, qword ptr [1879052408]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052400], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052408], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052416], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052424], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx508_2
.Lx508_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052400], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052408], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052416], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052424], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx508_2
.Lx508_5:
                        add              rsp, 32
                        mov              rdi, qword ptr [rip + .Lx508_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx508_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx508_3]
                        lea              rdx, [rip + .Lx508_4]
                                                                                        jmp   rax
.Lx508_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx508_2
.Lx508_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx508_2
.Lx508_1:
                        call             rt_faildescr@PLT
.Lx508_2:
                        mov              qword ptr [rsp + 1696], rax
                        mov              qword ptr [rsp + 1704], rdx
                        cmp              eax, 99
                                                                                        je    n328_call_α
                                                                                        jmp   n327_assign_α
n325_call_β:
                                                                                        jmp   n328_call_α
.Lx508_0:
                        .quad            .Lx508_0_s
.Lx508_0_s:
                        .string          "stack_depth"
#-----------------------------------------------------------------------------------------------------------------------
n326_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052464]
                        mov              rdx, qword ptr [1879052472]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n329_assign_α
n326_var_β:
                        add              rsp, 16
                                                                                        jmp   n330_call_α
#-----------------------------------------------------------------------------------------------------------------------
n327_assign_α:
                        mov              rsi, qword ptr [rsp + 1696]
                        mov              rdx, qword ptr [rsp + 1704]
                        mov              rdi, qword ptr [rip + .Lx510_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n328_call_α
n327_assign_β:
                                                                                        jmp   n328_call_α
.Lx510_0:
                        .quad            .Lx510_0_s
.Lx510_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n328_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052352]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052336]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx512_0]
                        mov              esi, 1
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx512_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx512_6]
                        lea              rdx, [rip + .Lx512_7]
                                                                                        jmp   rax
.Lx512_6:
                        mov              rdi, qword ptr [1879052336]
                        mov              rsi, qword ptr [1879052344]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052360], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx512_2
.Lx512_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052360], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx512_2
.Lx512_5:
                        add              rsp, 32
                        mov              rdi, qword ptr [rip + .Lx512_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx512_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx512_3]
                        lea              rdx, [rip + .Lx512_4]
                                                                                        jmp   rax
.Lx512_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx512_2
.Lx512_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx512_2
.Lx512_1:
                        call             rt_faildescr@PLT
.Lx512_2:
                        mov              qword ptr [rsp + 1728], rax
                        mov              qword ptr [rsp + 1736], rdx
                        cmp              eax, 99
                                                                                        je    n332_call_α
                                                                                        jmp   n331_assign_α
n328_call_β:
                                                                                        jmp   n332_call_α
.Lx512_0:
                        .quad            .Lx512_0_s
.Lx512_0_s:
                        .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n329_assign_α:
                        mov              rsi, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx513_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n330_call_α
n329_assign_β:
                                                                                        jmp   n330_call_α
.Lx513_0:
                        .quad            .Lx513_0_s
.Lx513_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n330_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 8], rax
                        mov              rdi, qword ptr [rip + .Lx515_0]
                        mov              esi, 0
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx515_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx515_6]
                        lea              rdx, [rip + .Lx515_7]
                                                                                        jmp   rax
.Lx515_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052296], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx515_2
.Lx515_7:
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052296], rax
                        add              rsp, 16
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx515_2
.Lx515_5:
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx515_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx515_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx515_3]
                        lea              rdx, [rip + .Lx515_4]
                                                                                        jmp   rax
.Lx515_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx515_2
.Lx515_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx515_2
.Lx515_1:
                        call             rt_faildescr@PLT
.Lx515_2:
                        mov              qword ptr [rsp + 2288], rax
                        mov              qword ptr [rsp + 2296], rdx
                        cmp              eax, 99
                                                                                        je    n333_lit_string_α
                                                                                        jmp   n333_lit_string_α
n330_call_β:
                                                                                        jmp   n333_lit_string_α
.Lx515_0:
                        .quad            .Lx515_0_s
.Lx515_0_s:
                        .string          "stack_init"
#-----------------------------------------------------------------------------------------------------------------------
n331_assign_α:
                        mov              rsi, qword ptr [rsp + 1728]
                        mov              rdx, qword ptr [rsp + 1736]
                        mov              rdi, qword ptr [rip + .Lx516_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n332_call_α
n331_assign_β:
                                                                                        jmp   n332_call_α
.Lx516_0:
                        .quad            .Lx516_0_s
.Lx516_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n332_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052416]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052424]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052400]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052408]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx518_0]
                        mov              esi, 1
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx518_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx518_6]
                        lea              rdx, [rip + .Lx518_7]
                                                                                        jmp   rax
.Lx518_6:
                        mov              rdi, qword ptr [1879052400]
                        mov              rsi, qword ptr [1879052408]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052400], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052408], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052416], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052424], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx518_2
.Lx518_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052400], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052408], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052416], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052424], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx518_2
.Lx518_5:
                        add              rsp, 32
                        mov              rdi, qword ptr [rip + .Lx518_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx518_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx518_3]
                        lea              rdx, [rip + .Lx518_4]
                                                                                        jmp   rax
.Lx518_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx518_2
.Lx518_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx518_2
.Lx518_1:
                        call             rt_faildescr@PLT
.Lx518_2:
                        mov              qword ptr [rsp + 1760], rax
                        mov              qword ptr [rsp + 1768], rdx
                        cmp              eax, 99
                                                                                        je    n335_call_α
                                                                                        jmp   n334_assign_α
n332_call_β:
                                                                                        jmp   n335_call_α
.Lx518_0:
                        .quad            .Lx518_0_s
.Lx518_0_s:
                        .string          "stack_depth"
#-----------------------------------------------------------------------------------------------------------------------
n333_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              rax, qword ptr [rip + .Lx519_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n336_assign_α
n333_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n338_var_α
.Lx519_0:
                        .quad            .Lx519_0_s
.Lx519_0_s:
                        .string          "hello world"
#-----------------------------------------------------------------------------------------------------------------------
n334_assign_α:
                        mov              rsi, qword ptr [rsp + 1760]
                        mov              rdx, qword ptr [rsp + 1768]
                        mov              rdi, qword ptr [rip + .Lx520_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n335_call_α
n334_assign_β:
                                                                                        jmp   n335_call_α
.Lx520_0:
                        .quad            .Lx520_0_s
.Lx520_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n335_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052352]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052336]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx522_0]
                        mov              esi, 1
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx522_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx522_6]
                        lea              rdx, [rip + .Lx522_7]
                                                                                        jmp   rax
.Lx522_6:
                        mov              rdi, qword ptr [1879052336]
                        mov              rsi, qword ptr [1879052344]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052360], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx522_2
.Lx522_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052360], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx522_2
.Lx522_5:
                        add              rsp, 32
                        mov              rdi, qword ptr [rip + .Lx522_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx522_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx522_3]
                        lea              rdx, [rip + .Lx522_4]
                                                                                        jmp   rax
.Lx522_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx522_2
.Lx522_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx522_2
.Lx522_1:
                        call             rt_faildescr@PLT
.Lx522_2:
                        mov              qword ptr [rsp + 1792], rax
                        mov              qword ptr [rsp + 1800], rdx
                        cmp              eax, 99
                                                                                        je    n337_lit_string_α
                                                                                        jmp   n241_lit_string_α
n335_call_β:
                                                                                        jmp   n337_lit_string_α
.Lx522_0:
                        .quad            .Lx522_0_s
.Lx522_0_s:
                        .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n336_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052480], rax
                        mov              qword ptr [1879052488], rdx
                                                                                        jmp   n338_var_α
n336_assign_β:
                                                                                        jmp   n338_var_α
#-----------------------------------------------------------------------------------------------------------------------
n337_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              rax, qword ptr [rip + .Lx524_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n339_assign_α
n337_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n242_call_α
.Lx524_0:
                        .quad            .Lx524_0_s
.Lx524_0_s:
                        .string          "empty ok"
#-----------------------------------------------------------------------------------------------------------------------
n338_var_α:
                        mov              rax, qword ptr [1879052480]
                        mov              rdx, qword ptr [1879052488]
                        mov              qword ptr [rsp + 2608], rax
                        mov              qword ptr [rsp + 2616], rdx
                                                                                        jmp   n340_match_head_α
n338_var_β:
                                                                                        jmp   n243_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n339_assign_α:
                        mov              rsi, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx526_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n242_call_α
n339_assign_β:
                                                                                        jmp   n242_call_α
.Lx526_0:
                        .quad            .Lx526_0_s
.Lx526_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n340_match_head_α:
                        mov              qword ptr [rsp + 2392], rbp
                        mov              rdi, qword ptr [rsp + 2608]
                        mov              rsi, qword ptr [rsp + 2616]
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              qword ptr [rsp + 2384], r12
                        mov              rax, rsp
                        sub              rsp, 32
                        mov              qword ptr [rsp + 16], rax
                        lea              rcx, [rip + g_patstk_sp]
                        mov              rax, qword ptr [rcx + 0]
                        mov              qword ptr [rsp + 8], rax
                        mov              dword ptr [rsp + 0], 0
.Lx528_0:
                        mov              r14d, dword ptr [rsp + 0]
                                                                                        jmp   n341_match_sequence_α
n340_match_head_β:
                        add              dword ptr [rsp + 0], 1
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d
                                                                                        jg    .Lx528_1
                        lea              rcx, [rip + g_anchor]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx528_1
                                                                                        jmp   .Lx528_0
.Lx528_1:
                        mov              rax, qword ptr [rsp + 8]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rsp + 16]
                        mov              r12, qword ptr [rsp + 2384]
                        mov              rbp, qword ptr [rsp + 2392]
                                                                                        jmp   n243_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n341_match_sequence_α:
                                                                                        jmp   n343_match_assign_save_α
n341_match_sequence_as:
                                                                                        jmp   n342_match_release_α
n341_match_sequence_β:
                                                                                        jmp   n347_match_assign_cond_β
n341_match_sequence_af:
                                                                                        jmp   n340_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n342_match_release_α:
                        mov              rax, qword ptr [rsp + 72]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              eax, dword ptr [rsp + 64]
                        mov              dword ptr [rsp + 2448], eax
                        mov              qword ptr [rsp + 2472], r14
                        mov              rsp, qword ptr [rsp + 80]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rsp + 2416]
                        mov              rsi, r12
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx532_1:
                        test             rax, rax
                                                                                        je    .Lx532_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx532_3]
                        lea              rdx, [rip + .Lx532_4]
                                                                                        jmp   rax
.Lx532_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx532_1
.Lx532_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx532_1
.Lx532_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r12, qword ptr [rsp + 2384]
                                                                                        jmp   n348_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n343_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 48], r14d
                                                                                        jmp   n349_match_break_α
n343_match_assign_save_β:
                        add              rsp, 16
                                                                                        jmp   n340_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n344_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 48]
                        lea              rcx, [rip + .S0]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24
                                                                                        jmp   n345_match_lit_α
n344_match_assign_cond_β:
                        sub              r12, 24
                                                                                        jmp   n349_match_break_β
#-----------------------------------------------------------------------------------------------------------------------
n345_match_lit_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jg    n344_match_assign_cond_β
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 32
                                                                                        jne   n344_match_assign_cond_β
                        add              r14d, 1
                                                                                        jmp   n346_match_assign_save_α
n345_match_lit_β:
                        sub              r14d, 1
                                                                                        jmp   n344_match_assign_cond_β
#-----------------------------------------------------------------------------------------------------------------------
n346_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 80], r14d
                                                                                        jmp   n350_match_rem_α
n346_match_assign_save_β:
                        add              rsp, 16
                                                                                        jmp   n345_match_lit_β
#-----------------------------------------------------------------------------------------------------------------------
n347_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 80]
                        lea              rcx, [rip + .S1]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24
                                                                                        jmp   n342_match_release_α
n347_match_assign_cond_β:
                        sub              r12, 24
                                                                                        jmp   n350_match_rem_β
#-----------------------------------------------------------------------------------------------------------------------
n348_lit_string_α:
                        mov              qword ptr [rsp + 2416], 1
                        mov              rax, qword ptr [rip + .Lx543_0]
                        mov              qword ptr [rsp + 2424], rax
                                                                                        jmp   n351_match_replace_α
n348_lit_string_β:
                                                                                        jmp   n243_lit_string_α
.Lx543_0:
                        .quad            .Lx543_0_s
.Lx543_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n349_match_break_α:
                        sub              rsp, 16
                        movsxd           rcx, r14d
.Lx545_0:
                        cmp              ecx, r15d
                                                                                        jl    .Lx545_237
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n340_match_head_β
.Lx545_237:
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx545_1
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jl    .Lx545_238
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n340_match_head_β
.Lx545_238:
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx545_1
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jl    .Lx545_239
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n340_match_head_β
.Lx545_239:
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx545_1
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jl    .Lx545_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n340_match_head_β
.Lx545_240:
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx545_1
                        add              ecx, 1
                                                                                        jmp   .Lx545_0
.Lx545_1:
                        mov              dword ptr [rsp + 64], r14d
                        mov              r14d, ecx
                                                                                        jmp   n344_match_assign_cond_α
n349_match_break_β:
                        mov              r14d, dword ptr [rsp + 64]
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n340_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n350_match_rem_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 96], r14d
                        mov              r14d, r15d
                                                                                        jmp   n347_match_assign_cond_α
n350_match_rem_β:
                        mov              r14d, dword ptr [rsp + 96]
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n345_match_lit_β
#-----------------------------------------------------------------------------------------------------------------------
n351_match_replace_α:
                        mov              rdi, qword ptr [rip + .Lx548_0]
                        mov              rsi, qword ptr [rsp + 2608]
                        mov              rdx, qword ptr [rsp + 2616]
                        mov              ecx, dword ptr [rsp + 2352]
                        mov              r8, qword ptr [rsp + 2376]
                        lea              r9, [rsp + 2416]
                        call             rt_match_replace@PLT
                                                                                        jmp   .Lx548_1
.Lx548_0:
                        .quad            .Lx548_0_s
.Lx548_0_s:
                        .string          "subject"
.Lx548_1:
                        mov              rbp, qword ptr [rsp + 2392]
                                                                                        jmp   n352_var_α
#-----------------------------------------------------------------------------------------------------------------------
n352_var_α:
                        mov              rax, qword ptr [1879052496]
                        mov              rdx, qword ptr [1879052504]
                        mov              qword ptr [rsp + 2672], rax
                        mov              qword ptr [rsp + 2680], rdx
                                                                                        jmp   n353_call_α
n352_var_β:
                                                                                        jmp   n354_var_α
#-----------------------------------------------------------------------------------------------------------------------
n353_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx551_0]
                        mov              esi, 1
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx551_5
                        mov              rax, qword ptr [rsp + 2672]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 2680]
                        mov              qword ptr [1879052328], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx551_6]
                        lea              rdx, [rip + .Lx551_7]
                                                                                        jmp   rax
.Lx551_6:
                        mov              rdi, qword ptr [1879052304]
                        mov              rsi, qword ptr [1879052312]
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
                                                                                        jmp   .Lx551_2
.Lx551_7:
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
                                                                                        jmp   .Lx551_2
.Lx551_5:
                        add              rsp, 32
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx551_20
                        mov              rax, qword ptr [rsp + 2672]
                        mov              rdx, qword ptr [rsp + 2680]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx551_21
.Lx551_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 2672]
                        mov              rdx, qword ptr [rsp + 2680]
                        call             rt_arg_stage@PLT
.Lx551_21:
                        mov              rdi, qword ptr [rip + .Lx551_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx551_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx551_3]
                        lea              rdx, [rip + .Lx551_4]
                                                                                        jmp   rax
.Lx551_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx551_2
.Lx551_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx551_2
.Lx551_1:
                        call             rt_faildescr@PLT
.Lx551_2:
                        mov              qword ptr [rsp + 2624], rax
                        mov              qword ptr [rsp + 2632], rdx
                        cmp              eax, 99
                                                                                        je    n354_var_α
                                                                                        jmp   n354_var_α
n353_call_β:
                                                                                        jmp   n354_var_α
.Lx551_0:
                        .quad            .Lx551_0_s
.Lx551_0_s:
                        .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n354_var_α:
                        mov              rax, qword ptr [1879052512]
                        mov              rdx, qword ptr [1879052520]
                        mov              qword ptr [rsp + 2736], rax
                        mov              qword ptr [rsp + 2744], rdx
                                                                                        jmp   n355_call_α
n354_var_β:
                                                                                        jmp   n356_call_α
#-----------------------------------------------------------------------------------------------------------------------
n355_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx554_0]
                        mov              esi, 1
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx554_5
                        mov              rax, qword ptr [rsp + 2736]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 2744]
                        mov              qword ptr [1879052328], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx554_6]
                        lea              rdx, [rip + .Lx554_7]
                                                                                        jmp   rax
.Lx554_6:
                        mov              rdi, qword ptr [1879052304]
                        mov              rsi, qword ptr [1879052312]
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
                                                                                        jmp   .Lx554_2
.Lx554_7:
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
                                                                                        jmp   .Lx554_2
.Lx554_5:
                        add              rsp, 32
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx554_20
                        mov              rax, qword ptr [rsp + 2736]
                        mov              rdx, qword ptr [rsp + 2744]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx554_21
.Lx554_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 2736]
                        mov              rdx, qword ptr [rsp + 2744]
                        call             rt_arg_stage@PLT
.Lx554_21:
                        mov              rdi, qword ptr [rip + .Lx554_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx554_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx554_3]
                        lea              rdx, [rip + .Lx554_4]
                                                                                        jmp   rax
.Lx554_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx554_2
.Lx554_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx554_2
.Lx554_1:
                        call             rt_faildescr@PLT
.Lx554_2:
                        mov              qword ptr [rsp + 2688], rax
                        mov              qword ptr [rsp + 2696], rdx
                        cmp              eax, 99
                                                                                        je    n356_call_α
                                                                                        jmp   n356_call_α
n355_call_β:
                                                                                        jmp   n356_call_α
.Lx554_0:
                        .quad            .Lx554_0_s
.Lx554_0_s:
                        .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n356_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052352]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052336]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx556_0]
                        mov              esi, 1
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx556_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx556_6]
                        lea              rdx, [rip + .Lx556_7]
                                                                                        jmp   rax
.Lx556_6:
                        mov              rdi, qword ptr [1879052336]
                        mov              rsi, qword ptr [1879052344]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052360], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx556_2
.Lx556_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052360], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx556_2
.Lx556_5:
                        add              rsp, 32
                        mov              rdi, qword ptr [rip + .Lx556_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx556_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx556_3]
                        lea              rdx, [rip + .Lx556_4]
                                                                                        jmp   rax
.Lx556_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx556_2
.Lx556_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx556_2
.Lx556_1:
                        call             rt_faildescr@PLT
.Lx556_2:
                        mov              qword ptr [rsp + 2752], rax
                        mov              qword ptr [rsp + 2760], rdx
                        cmp              eax, 99
                                                                                        je    n358_call_α
                                                                                        jmp   n357_assign_α
n356_call_β:
                                                                                        jmp   n358_call_α
.Lx556_0:
                        .quad            .Lx556_0_s
.Lx556_0_s:
                        .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n357_assign_α:
                        mov              rsi, qword ptr [rsp + 2752]
                        mov              rdx, qword ptr [rsp + 2760]
                        mov              rdi, qword ptr [rip + .Lx557_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n358_call_α
n357_assign_β:
                                                                                        jmp   n358_call_α
.Lx557_0:
                        .quad            .Lx557_0_s
.Lx557_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n358_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052352]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052336]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx559_0]
                        mov              esi, 1
                        mov              edx, 0
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx559_5
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx559_6]
                        lea              rdx, [rip + .Lx559_7]
                                                                                        jmp   rax
.Lx559_6:
                        mov              rdi, qword ptr [1879052336]
                        mov              rsi, qword ptr [1879052344]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052360], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx559_2
.Lx559_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052360], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx559_2
.Lx559_5:
                        add              rsp, 32
                        mov              rdi, qword ptr [rip + .Lx559_0]
                        mov              esi, 0
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx559_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx559_3]
                        lea              rdx, [rip + .Lx559_4]
                                                                                        jmp   rax
.Lx559_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx559_2
.Lx559_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx559_2
.Lx559_1:
                        call             rt_faildescr@PLT
.Lx559_2:
                        mov              qword ptr [rsp + 2784], rax
                        mov              qword ptr [rsp + 2792], rdx
                        cmp              eax, 99
                                                                                        je    main_γ
                                                                                        jmp   n359_assign_α
n358_call_β:
                                                                                        jmp   main_γ
.Lx559_0:
                        .quad            .Lx559_0_s
.Lx559_0_s:
                        .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n359_assign_α:
                        mov              rsi, qword ptr [rsp + 2784]
                        mov              rdx, qword ptr [rsp + 2792]
                        mov              rdi, qword ptr [rip + .Lx560_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   main_γ
n359_assign_β:
                                                                                        jmp   main_γ
.Lx560_0:
                        .quad            .Lx560_0_s
.Lx560_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        mov              rsp, rbp
                        mov              rbp, [rsp + 2832]
                        add              rsp, 2840
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              rsp, rbp
                        mov              eax, 99
                        xor              edx, edx
                        mov              rbp, [rsp + 2832]
                        add              rsp, 2840
                        ret
                        .section         .rodata
.S0:                    .string          "w1"
.S1:                    .string          "w2"
                        .text
                        .section         .note.GNU-stack,"",@progbits

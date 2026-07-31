                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_LBL__roman_α
proc_LBL__roman_α:
                        .global          proc_LBL__roman_α
                        .global          proc_LBL__roman_β
                        .global          proc_LBL__roman_γ
                        .global          proc_LBL__roman_ω
                        sub              rsp, 3136
                        mov              [rsp + 3112], rcx
                        mov              [rsp + 3120], rdx
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 3088], 0
                        mov              qword ptr [rsp + 3096], 0
proc_LBL__roman_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n0_goto_α:
                                                                                        jmp   n1_lit_string_α
n0_goto_β:
                                                                                        jmp   proc_LBL__roman_ω
#=======================================================================================================================
# roman   s = ''
#-----------------------------------------------------------------------------------------------------------------------
n1_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx176_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n2_assign_α
.Lx176_0:
                        .quad            .Lx176_0_s
.Lx176_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n2_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rdx
                                                                                        jmp   n3_lit_integer_α
#=======================================================================================================================
#         v = ARRAY(13)
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_integer_α:
                        mov              qword ptr [rsp + 96], 6
                        mov              rax, qword ptr [rip + .Lx178_0]
                        mov              qword ptr [rsp + 104], rax
                                                                                        jmp   n4_call_α
.Lx178_0:
                        .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n4_call_α:
                        mov              rax, qword ptr [rsp + 96]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 72], rax
                        .section         .rodata
.Lrkfn180:              .string          "ARRAY"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn180]
                        lea              rsi, [rsp + 64]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 48], rax
                        mov              qword ptr [rsp + 56], rdx
                        cmp              eax, 99
                                                                                        je    n6_var_α
                                                                                        jmp   n5_assign_α
n4_call_β:
                                                                                        jmp   n6_var_α
#-----------------------------------------------------------------------------------------------------------------------
n5_assign_α:
                        mov              rax, qword ptr [rsp + 48]
                        mov              rdx, qword ptr [rsp + 56]
                        mov              qword ptr [1879052336], rax
                        mov              qword ptr [1879052344], rdx
                                                                                        jmp   n6_var_α
#=======================================================================================================================
#         v<1> = 1000
#-----------------------------------------------------------------------------------------------------------------------
n6_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 112], rax
                        mov              qword ptr [rsp + 120], rdx
                                                                                        jmp   n7_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n7_lit_integer_α:
                        mov              qword ptr [rsp + 128], 6
                        mov              rax, qword ptr [rip + .Lx183_0]
                        mov              qword ptr [rsp + 136], rax
                                                                                        jmp   n8_subscript_α
.Lx183_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n8_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 128]
                        mov              rsi, qword ptr [rsp + 136]
                        mov              rdx, qword ptr [rsp + 144]
                        mov              rcx, qword ptr [rsp + 152]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx184_240
                        add              rsp, 16
                                                                                        jmp   n11_var_α
.Lx184_240:
                        mov              qword ptr [rsp + 160], rax
                        mov              qword ptr [rsp + 168], rdx
                        add              rsp, 16
                                                                                        jmp   n9_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n9_lit_integer_α:
                        mov              qword ptr [rsp + 160], 6
                        mov              rax, qword ptr [rip + .Lx185_0]
                        mov              qword ptr [rsp + 168], rax
                                                                                        jmp   n10_assign_var_α
.Lx185_0:
                        .quad            1000
#-----------------------------------------------------------------------------------------------------------------------
n10_assign_var_α:
                        mov              rdi, qword ptr [rsp + 144]
                        mov              rsi, qword ptr [rsp + 152]
                        mov              rdx, qword ptr [rsp + 160]
                        mov              rcx, qword ptr [rsp + 168]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n11_var_α
                        mov              qword ptr [rsp + 176], rax
                        mov              qword ptr [rsp + 184], rdx
                                                                                        jmp   n11_var_α
#=======================================================================================================================
#         v<2> = 900
#-----------------------------------------------------------------------------------------------------------------------
n11_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 192], rax
                        mov              qword ptr [rsp + 200], rdx
                                                                                        jmp   n12_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n12_lit_integer_α:
                        mov              qword ptr [rsp + 208], 6
                        mov              rax, qword ptr [rip + .Lx188_0]
                        mov              qword ptr [rsp + 216], rax
                                                                                        jmp   n13_subscript_α
.Lx188_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n13_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 208]
                        mov              rsi, qword ptr [rsp + 216]
                        mov              rdx, qword ptr [rsp + 224]
                        mov              rcx, qword ptr [rsp + 232]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx189_240
                        add              rsp, 16
                                                                                        jmp   n16_var_α
.Lx189_240:
                        mov              qword ptr [rsp + 240], rax
                        mov              qword ptr [rsp + 248], rdx
                        add              rsp, 16
                                                                                        jmp   n14_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n14_lit_integer_α:
                        mov              qword ptr [rsp + 240], 6
                        mov              rax, qword ptr [rip + .Lx190_0]
                        mov              qword ptr [rsp + 248], rax
                                                                                        jmp   n15_assign_var_α
.Lx190_0:
                        .quad            900
#-----------------------------------------------------------------------------------------------------------------------
n15_assign_var_α:
                        mov              rdi, qword ptr [rsp + 224]
                        mov              rsi, qword ptr [rsp + 232]
                        mov              rdx, qword ptr [rsp + 240]
                        mov              rcx, qword ptr [rsp + 248]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n16_var_α
                        mov              qword ptr [rsp + 256], rax
                        mov              qword ptr [rsp + 264], rdx
                                                                                        jmp   n16_var_α
#=======================================================================================================================
#         v<3> = 500
#-----------------------------------------------------------------------------------------------------------------------
n16_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 272], rax
                        mov              qword ptr [rsp + 280], rdx
                                                                                        jmp   n17_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n17_lit_integer_α:
                        mov              qword ptr [rsp + 288], 6
                        mov              rax, qword ptr [rip + .Lx193_0]
                        mov              qword ptr [rsp + 296], rax
                                                                                        jmp   n18_subscript_α
.Lx193_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n18_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 288]
                        mov              rsi, qword ptr [rsp + 296]
                        mov              rdx, qword ptr [rsp + 304]
                        mov              rcx, qword ptr [rsp + 312]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx194_240
                        add              rsp, 16
                                                                                        jmp   n21_var_α
.Lx194_240:
                        mov              qword ptr [rsp + 320], rax
                        mov              qword ptr [rsp + 328], rdx
                        add              rsp, 16
                                                                                        jmp   n19_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n19_lit_integer_α:
                        mov              qword ptr [rsp + 320], 6
                        mov              rax, qword ptr [rip + .Lx195_0]
                        mov              qword ptr [rsp + 328], rax
                                                                                        jmp   n20_assign_var_α
.Lx195_0:
                        .quad            500
#-----------------------------------------------------------------------------------------------------------------------
n20_assign_var_α:
                        mov              rdi, qword ptr [rsp + 304]
                        mov              rsi, qword ptr [rsp + 312]
                        mov              rdx, qword ptr [rsp + 320]
                        mov              rcx, qword ptr [rsp + 328]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n21_var_α
                        mov              qword ptr [rsp + 336], rax
                        mov              qword ptr [rsp + 344], rdx
                                                                                        jmp   n21_var_α
#=======================================================================================================================
#         v<4> = 400
#-----------------------------------------------------------------------------------------------------------------------
n21_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 352], rax
                        mov              qword ptr [rsp + 360], rdx
                                                                                        jmp   n22_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n22_lit_integer_α:
                        mov              qword ptr [rsp + 368], 6
                        mov              rax, qword ptr [rip + .Lx198_0]
                        mov              qword ptr [rsp + 376], rax
                                                                                        jmp   n23_subscript_α
.Lx198_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n23_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 368]
                        mov              rsi, qword ptr [rsp + 376]
                        mov              rdx, qword ptr [rsp + 384]
                        mov              rcx, qword ptr [rsp + 392]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx199_240
                        add              rsp, 16
                                                                                        jmp   n26_var_α
.Lx199_240:
                        mov              qword ptr [rsp + 400], rax
                        mov              qword ptr [rsp + 408], rdx
                        add              rsp, 16
                                                                                        jmp   n24_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n24_lit_integer_α:
                        mov              qword ptr [rsp + 400], 6
                        mov              rax, qword ptr [rip + .Lx200_0]
                        mov              qword ptr [rsp + 408], rax
                                                                                        jmp   n25_assign_var_α
.Lx200_0:
                        .quad            400
#-----------------------------------------------------------------------------------------------------------------------
n25_assign_var_α:
                        mov              rdi, qword ptr [rsp + 384]
                        mov              rsi, qword ptr [rsp + 392]
                        mov              rdx, qword ptr [rsp + 400]
                        mov              rcx, qword ptr [rsp + 408]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n26_var_α
                        mov              qword ptr [rsp + 416], rax
                        mov              qword ptr [rsp + 424], rdx
                                                                                        jmp   n26_var_α
#=======================================================================================================================
#         v<5> = 100
#-----------------------------------------------------------------------------------------------------------------------
n26_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 432], rax
                        mov              qword ptr [rsp + 440], rdx
                                                                                        jmp   n27_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n27_lit_integer_α:
                        mov              qword ptr [rsp + 448], 6
                        mov              rax, qword ptr [rip + .Lx203_0]
                        mov              qword ptr [rsp + 456], rax
                                                                                        jmp   n28_subscript_α
.Lx203_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n28_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 448]
                        mov              rsi, qword ptr [rsp + 456]
                        mov              rdx, qword ptr [rsp + 464]
                        mov              rcx, qword ptr [rsp + 472]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx204_240
                        add              rsp, 16
                                                                                        jmp   n31_var_α
.Lx204_240:
                        mov              qword ptr [rsp + 480], rax
                        mov              qword ptr [rsp + 488], rdx
                        add              rsp, 16
                                                                                        jmp   n29_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n29_lit_integer_α:
                        mov              qword ptr [rsp + 480], 6
                        mov              rax, qword ptr [rip + .Lx205_0]
                        mov              qword ptr [rsp + 488], rax
                                                                                        jmp   n30_assign_var_α
.Lx205_0:
                        .quad            100
#-----------------------------------------------------------------------------------------------------------------------
n30_assign_var_α:
                        mov              rdi, qword ptr [rsp + 464]
                        mov              rsi, qword ptr [rsp + 472]
                        mov              rdx, qword ptr [rsp + 480]
                        mov              rcx, qword ptr [rsp + 488]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n31_var_α
                        mov              qword ptr [rsp + 496], rax
                        mov              qword ptr [rsp + 504], rdx
                                                                                        jmp   n31_var_α
#=======================================================================================================================
#         v<6> = 90
#-----------------------------------------------------------------------------------------------------------------------
n31_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 512], rax
                        mov              qword ptr [rsp + 520], rdx
                                                                                        jmp   n32_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n32_lit_integer_α:
                        mov              qword ptr [rsp + 528], 6
                        mov              rax, qword ptr [rip + .Lx208_0]
                        mov              qword ptr [rsp + 536], rax
                                                                                        jmp   n33_subscript_α
.Lx208_0:
                        .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n33_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 528]
                        mov              rsi, qword ptr [rsp + 536]
                        mov              rdx, qword ptr [rsp + 544]
                        mov              rcx, qword ptr [rsp + 552]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx209_240
                        add              rsp, 16
                                                                                        jmp   n36_var_α
.Lx209_240:
                        mov              qword ptr [rsp + 560], rax
                        mov              qword ptr [rsp + 568], rdx
                        add              rsp, 16
                                                                                        jmp   n34_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n34_lit_integer_α:
                        mov              qword ptr [rsp + 560], 6
                        mov              rax, qword ptr [rip + .Lx210_0]
                        mov              qword ptr [rsp + 568], rax
                                                                                        jmp   n35_assign_var_α
.Lx210_0:
                        .quad            90
#-----------------------------------------------------------------------------------------------------------------------
n35_assign_var_α:
                        mov              rdi, qword ptr [rsp + 544]
                        mov              rsi, qword ptr [rsp + 552]
                        mov              rdx, qword ptr [rsp + 560]
                        mov              rcx, qword ptr [rsp + 568]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n36_var_α
                        mov              qword ptr [rsp + 576], rax
                        mov              qword ptr [rsp + 584], rdx
                                                                                        jmp   n36_var_α
#=======================================================================================================================
#         v<7> = 50
#-----------------------------------------------------------------------------------------------------------------------
n36_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 592], rax
                        mov              qword ptr [rsp + 600], rdx
                                                                                        jmp   n37_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n37_lit_integer_α:
                        mov              qword ptr [rsp + 608], 6
                        mov              rax, qword ptr [rip + .Lx213_0]
                        mov              qword ptr [rsp + 616], rax
                                                                                        jmp   n38_subscript_α
.Lx213_0:
                        .quad            7
#-----------------------------------------------------------------------------------------------------------------------
n38_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 608]
                        mov              rsi, qword ptr [rsp + 616]
                        mov              rdx, qword ptr [rsp + 624]
                        mov              rcx, qword ptr [rsp + 632]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx214_240
                        add              rsp, 16
                                                                                        jmp   n41_var_α
.Lx214_240:
                        mov              qword ptr [rsp + 640], rax
                        mov              qword ptr [rsp + 648], rdx
                        add              rsp, 16
                                                                                        jmp   n39_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n39_lit_integer_α:
                        mov              qword ptr [rsp + 640], 6
                        mov              rax, qword ptr [rip + .Lx215_0]
                        mov              qword ptr [rsp + 648], rax
                                                                                        jmp   n40_assign_var_α
.Lx215_0:
                        .quad            50
#-----------------------------------------------------------------------------------------------------------------------
n40_assign_var_α:
                        mov              rdi, qword ptr [rsp + 624]
                        mov              rsi, qword ptr [rsp + 632]
                        mov              rdx, qword ptr [rsp + 640]
                        mov              rcx, qword ptr [rsp + 648]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n41_var_α
                        mov              qword ptr [rsp + 656], rax
                        mov              qword ptr [rsp + 664], rdx
                                                                                        jmp   n41_var_α
#=======================================================================================================================
#         v<8> = 40
#-----------------------------------------------------------------------------------------------------------------------
n41_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 672], rax
                        mov              qword ptr [rsp + 680], rdx
                                                                                        jmp   n42_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n42_lit_integer_α:
                        mov              qword ptr [rsp + 688], 6
                        mov              rax, qword ptr [rip + .Lx218_0]
                        mov              qword ptr [rsp + 696], rax
                                                                                        jmp   n43_subscript_α
.Lx218_0:
                        .quad            8
#-----------------------------------------------------------------------------------------------------------------------
n43_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 688]
                        mov              rsi, qword ptr [rsp + 696]
                        mov              rdx, qword ptr [rsp + 704]
                        mov              rcx, qword ptr [rsp + 712]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx219_240
                        add              rsp, 16
                                                                                        jmp   n46_var_α
.Lx219_240:
                        mov              qword ptr [rsp + 720], rax
                        mov              qword ptr [rsp + 728], rdx
                        add              rsp, 16
                                                                                        jmp   n44_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n44_lit_integer_α:
                        mov              qword ptr [rsp + 720], 6
                        mov              rax, qword ptr [rip + .Lx220_0]
                        mov              qword ptr [rsp + 728], rax
                                                                                        jmp   n45_assign_var_α
.Lx220_0:
                        .quad            40
#-----------------------------------------------------------------------------------------------------------------------
n45_assign_var_α:
                        mov              rdi, qword ptr [rsp + 704]
                        mov              rsi, qword ptr [rsp + 712]
                        mov              rdx, qword ptr [rsp + 720]
                        mov              rcx, qword ptr [rsp + 728]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n46_var_α
                        mov              qword ptr [rsp + 736], rax
                        mov              qword ptr [rsp + 744], rdx
                                                                                        jmp   n46_var_α
#=======================================================================================================================
#         v<9> = 10
#-----------------------------------------------------------------------------------------------------------------------
n46_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 752], rax
                        mov              qword ptr [rsp + 760], rdx
                                                                                        jmp   n47_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n47_lit_integer_α:
                        mov              qword ptr [rsp + 768], 6
                        mov              rax, qword ptr [rip + .Lx223_0]
                        mov              qword ptr [rsp + 776], rax
                                                                                        jmp   n48_subscript_α
.Lx223_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n48_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 768]
                        mov              rsi, qword ptr [rsp + 776]
                        mov              rdx, qword ptr [rsp + 784]
                        mov              rcx, qword ptr [rsp + 792]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx224_240
                        add              rsp, 16
                                                                                        jmp   n51_var_α
.Lx224_240:
                        mov              qword ptr [rsp + 800], rax
                        mov              qword ptr [rsp + 808], rdx
                        add              rsp, 16
                                                                                        jmp   n49_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n49_lit_integer_α:
                        mov              qword ptr [rsp + 800], 6
                        mov              rax, qword ptr [rip + .Lx225_0]
                        mov              qword ptr [rsp + 808], rax
                                                                                        jmp   n50_assign_var_α
.Lx225_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n50_assign_var_α:
                        mov              rdi, qword ptr [rsp + 784]
                        mov              rsi, qword ptr [rsp + 792]
                        mov              rdx, qword ptr [rsp + 800]
                        mov              rcx, qword ptr [rsp + 808]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n51_var_α
                        mov              qword ptr [rsp + 816], rax
                        mov              qword ptr [rsp + 824], rdx
                                                                                        jmp   n51_var_α
#=======================================================================================================================
#         v<10> = 9
#-----------------------------------------------------------------------------------------------------------------------
n51_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 832], rax
                        mov              qword ptr [rsp + 840], rdx
                                                                                        jmp   n52_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n52_lit_integer_α:
                        mov              qword ptr [rsp + 848], 6
                        mov              rax, qword ptr [rip + .Lx228_0]
                        mov              qword ptr [rsp + 856], rax
                                                                                        jmp   n53_subscript_α
.Lx228_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n53_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 848]
                        mov              rsi, qword ptr [rsp + 856]
                        mov              rdx, qword ptr [rsp + 864]
                        mov              rcx, qword ptr [rsp + 872]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx229_240
                        add              rsp, 16
                                                                                        jmp   n56_var_α
.Lx229_240:
                        mov              qword ptr [rsp + 880], rax
                        mov              qword ptr [rsp + 888], rdx
                        add              rsp, 16
                                                                                        jmp   n54_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n54_lit_integer_α:
                        mov              qword ptr [rsp + 880], 6
                        mov              rax, qword ptr [rip + .Lx230_0]
                        mov              qword ptr [rsp + 888], rax
                                                                                        jmp   n55_assign_var_α
.Lx230_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n55_assign_var_α:
                        mov              rdi, qword ptr [rsp + 864]
                        mov              rsi, qword ptr [rsp + 872]
                        mov              rdx, qword ptr [rsp + 880]
                        mov              rcx, qword ptr [rsp + 888]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n56_var_α
                        mov              qword ptr [rsp + 896], rax
                        mov              qword ptr [rsp + 904], rdx
                                                                                        jmp   n56_var_α
#=======================================================================================================================
#         v<11> = 5
#-----------------------------------------------------------------------------------------------------------------------
n56_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 912], rax
                        mov              qword ptr [rsp + 920], rdx
                                                                                        jmp   n57_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n57_lit_integer_α:
                        mov              qword ptr [rsp + 928], 6
                        mov              rax, qword ptr [rip + .Lx233_0]
                        mov              qword ptr [rsp + 936], rax
                                                                                        jmp   n58_subscript_α
.Lx233_0:
                        .quad            11
#-----------------------------------------------------------------------------------------------------------------------
n58_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 928]
                        mov              rsi, qword ptr [rsp + 936]
                        mov              rdx, qword ptr [rsp + 944]
                        mov              rcx, qword ptr [rsp + 952]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx234_240
                        add              rsp, 16
                                                                                        jmp   n61_var_α
.Lx234_240:
                        mov              qword ptr [rsp + 960], rax
                        mov              qword ptr [rsp + 968], rdx
                        add              rsp, 16
                                                                                        jmp   n59_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n59_lit_integer_α:
                        mov              qword ptr [rsp + 960], 6
                        mov              rax, qword ptr [rip + .Lx235_0]
                        mov              qword ptr [rsp + 968], rax
                                                                                        jmp   n60_assign_var_α
.Lx235_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n60_assign_var_α:
                        mov              rdi, qword ptr [rsp + 944]
                        mov              rsi, qword ptr [rsp + 952]
                        mov              rdx, qword ptr [rsp + 960]
                        mov              rcx, qword ptr [rsp + 968]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n61_var_α
                        mov              qword ptr [rsp + 976], rax
                        mov              qword ptr [rsp + 984], rdx
                                                                                        jmp   n61_var_α
#=======================================================================================================================
#         v<12> = 4
#-----------------------------------------------------------------------------------------------------------------------
n61_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 992], rax
                        mov              qword ptr [rsp + 1000], rdx
                                                                                        jmp   n62_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n62_lit_integer_α:
                        mov              qword ptr [rsp + 1008], 6
                        mov              rax, qword ptr [rip + .Lx238_0]
                        mov              qword ptr [rsp + 1016], rax
                                                                                        jmp   n63_subscript_α
.Lx238_0:
                        .quad            12
#-----------------------------------------------------------------------------------------------------------------------
n63_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1008]
                        mov              rsi, qword ptr [rsp + 1016]
                        mov              rdx, qword ptr [rsp + 1024]
                        mov              rcx, qword ptr [rsp + 1032]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx239_240
                        add              rsp, 16
                                                                                        jmp   n66_var_α
.Lx239_240:
                        mov              qword ptr [rsp + 1040], rax
                        mov              qword ptr [rsp + 1048], rdx
                        add              rsp, 16
                                                                                        jmp   n64_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n64_lit_integer_α:
                        mov              qword ptr [rsp + 1040], 6
                        mov              rax, qword ptr [rip + .Lx240_0]
                        mov              qword ptr [rsp + 1048], rax
                                                                                        jmp   n65_assign_var_α
.Lx240_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n65_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1024]
                        mov              rsi, qword ptr [rsp + 1032]
                        mov              rdx, qword ptr [rsp + 1040]
                        mov              rcx, qword ptr [rsp + 1048]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n66_var_α
                        mov              qword ptr [rsp + 1056], rax
                        mov              qword ptr [rsp + 1064], rdx
                                                                                        jmp   n66_var_α
#=======================================================================================================================
#         v<13> = 1
#-----------------------------------------------------------------------------------------------------------------------
n66_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 1072], rax
                        mov              qword ptr [rsp + 1080], rdx
                                                                                        jmp   n67_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n67_lit_integer_α:
                        mov              qword ptr [rsp + 1088], 6
                        mov              rax, qword ptr [rip + .Lx243_0]
                        mov              qword ptr [rsp + 1096], rax
                                                                                        jmp   n68_subscript_α
.Lx243_0:
                        .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n68_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1088]
                        mov              rsi, qword ptr [rsp + 1096]
                        mov              rdx, qword ptr [rsp + 1104]
                        mov              rcx, qword ptr [rsp + 1112]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx244_240
                        add              rsp, 16
                                                                                        jmp   n71_lit_integer_α
.Lx244_240:
                        mov              qword ptr [rsp + 1120], rax
                        mov              qword ptr [rsp + 1128], rdx
                        add              rsp, 16
                                                                                        jmp   n69_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n69_lit_integer_α:
                        mov              qword ptr [rsp + 1120], 6
                        mov              rax, qword ptr [rip + .Lx245_0]
                        mov              qword ptr [rsp + 1128], rax
                                                                                        jmp   n70_assign_var_α
.Lx245_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n70_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1104]
                        mov              rsi, qword ptr [rsp + 1112]
                        mov              rdx, qword ptr [rsp + 1120]
                        mov              rcx, qword ptr [rsp + 1128]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n71_lit_integer_α
                        mov              qword ptr [rsp + 1136], rax
                        mov              qword ptr [rsp + 1144], rdx
                                                                                        jmp   n71_lit_integer_α
#=======================================================================================================================
#         r = ARRAY(13)
#-----------------------------------------------------------------------------------------------------------------------
n71_lit_integer_α:
                        mov              qword ptr [rsp + 1200], 6
                        mov              rax, qword ptr [rip + .Lx247_0]
                        mov              qword ptr [rsp + 1208], rax
                                                                                        jmp   n72_call_α
.Lx247_0:
                        .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n72_call_α:
                        mov              rax, qword ptr [rsp + 1200]
                        mov              qword ptr [rsp + 1168], rax
                        mov              rax, qword ptr [rsp + 1208]
                        mov              qword ptr [rsp + 1176], rax
                        .section         .rodata
.Lrkfn249:              .string          "ARRAY"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn249]
                        lea              rsi, [rsp + 1168]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1152], rax
                        mov              qword ptr [rsp + 1160], rdx
                        cmp              eax, 99
                                                                                        je    n74_var_α
                                                                                        jmp   n73_assign_α
n72_call_β:
                                                                                        jmp   n74_var_α
#-----------------------------------------------------------------------------------------------------------------------
n73_assign_α:
                        mov              rax, qword ptr [rsp + 1152]
                        mov              rdx, qword ptr [rsp + 1160]
                        mov              qword ptr [1879052352], rax
                        mov              qword ptr [1879052360], rdx
                                                                                        jmp   n74_var_α
#=======================================================================================================================
#         r<1> = 'M'
#-----------------------------------------------------------------------------------------------------------------------
n74_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 1216], rax
                        mov              qword ptr [rsp + 1224], rdx
                                                                                        jmp   n75_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n75_lit_integer_α:
                        mov              qword ptr [rsp + 1232], 6
                        mov              rax, qword ptr [rip + .Lx252_0]
                        mov              qword ptr [rsp + 1240], rax
                                                                                        jmp   n76_subscript_α
.Lx252_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n76_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1232]
                        mov              rsi, qword ptr [rsp + 1240]
                        mov              rdx, qword ptr [rsp + 1248]
                        mov              rcx, qword ptr [rsp + 1256]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx253_240
                        add              rsp, 16
                                                                                        jmp   n79_var_α
.Lx253_240:
                        mov              qword ptr [rsp + 1264], rax
                        mov              qword ptr [rsp + 1272], rdx
                        add              rsp, 16
                                                                                        jmp   n77_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n77_lit_string_α:
                        mov              qword ptr [rsp + 1264], 1
                        mov              dword ptr [rsp + 1268], 1
                        mov              rax, qword ptr [rip + .Lx254_0]
                        mov              qword ptr [rsp + 1272], rax
                                                                                        jmp   n78_assign_var_α
.Lx254_0:
                        .quad            .Lx254_0_s
.Lx254_0_s:
                        .string          "M"
#-----------------------------------------------------------------------------------------------------------------------
n78_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1248]
                        mov              rsi, qword ptr [rsp + 1256]
                        mov              rdx, qword ptr [rsp + 1264]
                        mov              rcx, qword ptr [rsp + 1272]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n79_var_α
                        mov              qword ptr [rsp + 1280], rax
                        mov              qword ptr [rsp + 1288], rdx
                                                                                        jmp   n79_var_α
#=======================================================================================================================
#         r<2> = 'CM'
#-----------------------------------------------------------------------------------------------------------------------
n79_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 1296], rax
                        mov              qword ptr [rsp + 1304], rdx
                                                                                        jmp   n80_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n80_lit_integer_α:
                        mov              qword ptr [rsp + 1312], 6
                        mov              rax, qword ptr [rip + .Lx257_0]
                        mov              qword ptr [rsp + 1320], rax
                                                                                        jmp   n81_subscript_α
.Lx257_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n81_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1312]
                        mov              rsi, qword ptr [rsp + 1320]
                        mov              rdx, qword ptr [rsp + 1328]
                        mov              rcx, qword ptr [rsp + 1336]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx258_240
                        add              rsp, 16
                                                                                        jmp   n84_var_α
.Lx258_240:
                        mov              qword ptr [rsp + 1344], rax
                        mov              qword ptr [rsp + 1352], rdx
                        add              rsp, 16
                                                                                        jmp   n82_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n82_lit_string_α:
                        mov              qword ptr [rsp + 1344], 1
                        mov              dword ptr [rsp + 1348], 2
                        mov              rax, qword ptr [rip + .Lx259_0]
                        mov              qword ptr [rsp + 1352], rax
                                                                                        jmp   n83_assign_var_α
.Lx259_0:
                        .quad            .Lx259_0_s
.Lx259_0_s:
                        .string          "CM"
#-----------------------------------------------------------------------------------------------------------------------
n83_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1328]
                        mov              rsi, qword ptr [rsp + 1336]
                        mov              rdx, qword ptr [rsp + 1344]
                        mov              rcx, qword ptr [rsp + 1352]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n84_var_α
                        mov              qword ptr [rsp + 1360], rax
                        mov              qword ptr [rsp + 1368], rdx
                                                                                        jmp   n84_var_α
#=======================================================================================================================
#         r<3> = 'D'
#-----------------------------------------------------------------------------------------------------------------------
n84_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 1376], rax
                        mov              qword ptr [rsp + 1384], rdx
                                                                                        jmp   n85_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n85_lit_integer_α:
                        mov              qword ptr [rsp + 1392], 6
                        mov              rax, qword ptr [rip + .Lx262_0]
                        mov              qword ptr [rsp + 1400], rax
                                                                                        jmp   n86_subscript_α
.Lx262_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n86_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1392]
                        mov              rsi, qword ptr [rsp + 1400]
                        mov              rdx, qword ptr [rsp + 1408]
                        mov              rcx, qword ptr [rsp + 1416]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx263_240
                        add              rsp, 16
                                                                                        jmp   n89_var_α
.Lx263_240:
                        mov              qword ptr [rsp + 1424], rax
                        mov              qword ptr [rsp + 1432], rdx
                        add              rsp, 16
                                                                                        jmp   n87_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n87_lit_string_α:
                        mov              qword ptr [rsp + 1424], 1
                        mov              dword ptr [rsp + 1428], 1
                        mov              rax, qword ptr [rip + .Lx264_0]
                        mov              qword ptr [rsp + 1432], rax
                                                                                        jmp   n88_assign_var_α
.Lx264_0:
                        .quad            .Lx264_0_s
.Lx264_0_s:
                        .string          "D"
#-----------------------------------------------------------------------------------------------------------------------
n88_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1408]
                        mov              rsi, qword ptr [rsp + 1416]
                        mov              rdx, qword ptr [rsp + 1424]
                        mov              rcx, qword ptr [rsp + 1432]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n89_var_α
                        mov              qword ptr [rsp + 1440], rax
                        mov              qword ptr [rsp + 1448], rdx
                                                                                        jmp   n89_var_α
#=======================================================================================================================
#         r<4> = 'CD'
#-----------------------------------------------------------------------------------------------------------------------
n89_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 1456], rax
                        mov              qword ptr [rsp + 1464], rdx
                                                                                        jmp   n90_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n90_lit_integer_α:
                        mov              qword ptr [rsp + 1472], 6
                        mov              rax, qword ptr [rip + .Lx267_0]
                        mov              qword ptr [rsp + 1480], rax
                                                                                        jmp   n91_subscript_α
.Lx267_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n91_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1472]
                        mov              rsi, qword ptr [rsp + 1480]
                        mov              rdx, qword ptr [rsp + 1488]
                        mov              rcx, qword ptr [rsp + 1496]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx268_240
                        add              rsp, 16
                                                                                        jmp   n94_var_α
.Lx268_240:
                        mov              qword ptr [rsp + 1504], rax
                        mov              qword ptr [rsp + 1512], rdx
                        add              rsp, 16
                                                                                        jmp   n92_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n92_lit_string_α:
                        mov              qword ptr [rsp + 1504], 1
                        mov              dword ptr [rsp + 1508], 2
                        mov              rax, qword ptr [rip + .Lx269_0]
                        mov              qword ptr [rsp + 1512], rax
                                                                                        jmp   n93_assign_var_α
.Lx269_0:
                        .quad            .Lx269_0_s
.Lx269_0_s:
                        .string          "CD"
#-----------------------------------------------------------------------------------------------------------------------
n93_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1488]
                        mov              rsi, qword ptr [rsp + 1496]
                        mov              rdx, qword ptr [rsp + 1504]
                        mov              rcx, qword ptr [rsp + 1512]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n94_var_α
                        mov              qword ptr [rsp + 1520], rax
                        mov              qword ptr [rsp + 1528], rdx
                                                                                        jmp   n94_var_α
#=======================================================================================================================
#         r<5> = 'C'
#-----------------------------------------------------------------------------------------------------------------------
n94_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 1536], rax
                        mov              qword ptr [rsp + 1544], rdx
                                                                                        jmp   n95_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n95_lit_integer_α:
                        mov              qword ptr [rsp + 1552], 6
                        mov              rax, qword ptr [rip + .Lx272_0]
                        mov              qword ptr [rsp + 1560], rax
                                                                                        jmp   n96_subscript_α
.Lx272_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n96_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1552]
                        mov              rsi, qword ptr [rsp + 1560]
                        mov              rdx, qword ptr [rsp + 1568]
                        mov              rcx, qword ptr [rsp + 1576]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx273_240
                        add              rsp, 16
                                                                                        jmp   n99_var_α
.Lx273_240:
                        mov              qword ptr [rsp + 1584], rax
                        mov              qword ptr [rsp + 1592], rdx
                        add              rsp, 16
                                                                                        jmp   n97_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n97_lit_string_α:
                        mov              qword ptr [rsp + 1584], 1
                        mov              dword ptr [rsp + 1588], 1
                        mov              rax, qword ptr [rip + .Lx274_0]
                        mov              qword ptr [rsp + 1592], rax
                                                                                        jmp   n98_assign_var_α
.Lx274_0:
                        .quad            .Lx274_0_s
.Lx274_0_s:
                        .string          "C"
#-----------------------------------------------------------------------------------------------------------------------
n98_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1568]
                        mov              rsi, qword ptr [rsp + 1576]
                        mov              rdx, qword ptr [rsp + 1584]
                        mov              rcx, qword ptr [rsp + 1592]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n99_var_α
                        mov              qword ptr [rsp + 1600], rax
                        mov              qword ptr [rsp + 1608], rdx
                                                                                        jmp   n99_var_α
#=======================================================================================================================
#         r<6> = 'XC'
#-----------------------------------------------------------------------------------------------------------------------
n99_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 1616], rax
                        mov              qword ptr [rsp + 1624], rdx
                                                                                        jmp   n100_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n100_lit_integer_α:
                        mov              qword ptr [rsp + 1632], 6
                        mov              rax, qword ptr [rip + .Lx277_0]
                        mov              qword ptr [rsp + 1640], rax
                                                                                        jmp   n101_subscript_α
.Lx277_0:
                        .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n101_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1632]
                        mov              rsi, qword ptr [rsp + 1640]
                        mov              rdx, qword ptr [rsp + 1648]
                        mov              rcx, qword ptr [rsp + 1656]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx278_240
                        add              rsp, 16
                                                                                        jmp   n104_var_α
.Lx278_240:
                        mov              qword ptr [rsp + 1664], rax
                        mov              qword ptr [rsp + 1672], rdx
                        add              rsp, 16
                                                                                        jmp   n102_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n102_lit_string_α:
                        mov              qword ptr [rsp + 1664], 1
                        mov              dword ptr [rsp + 1668], 2
                        mov              rax, qword ptr [rip + .Lx279_0]
                        mov              qword ptr [rsp + 1672], rax
                                                                                        jmp   n103_assign_var_α
.Lx279_0:
                        .quad            .Lx279_0_s
.Lx279_0_s:
                        .string          "XC"
#-----------------------------------------------------------------------------------------------------------------------
n103_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1648]
                        mov              rsi, qword ptr [rsp + 1656]
                        mov              rdx, qword ptr [rsp + 1664]
                        mov              rcx, qword ptr [rsp + 1672]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n104_var_α
                        mov              qword ptr [rsp + 1680], rax
                        mov              qword ptr [rsp + 1688], rdx
                                                                                        jmp   n104_var_α
#=======================================================================================================================
#         r<7> = 'L'
#-----------------------------------------------------------------------------------------------------------------------
n104_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 1696], rax
                        mov              qword ptr [rsp + 1704], rdx
                                                                                        jmp   n105_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n105_lit_integer_α:
                        mov              qword ptr [rsp + 1712], 6
                        mov              rax, qword ptr [rip + .Lx282_0]
                        mov              qword ptr [rsp + 1720], rax
                                                                                        jmp   n106_subscript_α
.Lx282_0:
                        .quad            7
#-----------------------------------------------------------------------------------------------------------------------
n106_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1712]
                        mov              rsi, qword ptr [rsp + 1720]
                        mov              rdx, qword ptr [rsp + 1728]
                        mov              rcx, qword ptr [rsp + 1736]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx283_240
                        add              rsp, 16
                                                                                        jmp   n109_var_α
.Lx283_240:
                        mov              qword ptr [rsp + 1744], rax
                        mov              qword ptr [rsp + 1752], rdx
                        add              rsp, 16
                                                                                        jmp   n107_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n107_lit_string_α:
                        mov              qword ptr [rsp + 1744], 1
                        mov              dword ptr [rsp + 1748], 1
                        mov              rax, qword ptr [rip + .Lx284_0]
                        mov              qword ptr [rsp + 1752], rax
                                                                                        jmp   n108_assign_var_α
.Lx284_0:
                        .quad            .Lx284_0_s
.Lx284_0_s:
                        .string          "L"
#-----------------------------------------------------------------------------------------------------------------------
n108_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1728]
                        mov              rsi, qword ptr [rsp + 1736]
                        mov              rdx, qword ptr [rsp + 1744]
                        mov              rcx, qword ptr [rsp + 1752]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n109_var_α
                        mov              qword ptr [rsp + 1760], rax
                        mov              qword ptr [rsp + 1768], rdx
                                                                                        jmp   n109_var_α
#=======================================================================================================================
#         r<8> = 'XL'
#-----------------------------------------------------------------------------------------------------------------------
n109_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 1776], rax
                        mov              qword ptr [rsp + 1784], rdx
                                                                                        jmp   n110_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n110_lit_integer_α:
                        mov              qword ptr [rsp + 1792], 6
                        mov              rax, qword ptr [rip + .Lx287_0]
                        mov              qword ptr [rsp + 1800], rax
                                                                                        jmp   n111_subscript_α
.Lx287_0:
                        .quad            8
#-----------------------------------------------------------------------------------------------------------------------
n111_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1792]
                        mov              rsi, qword ptr [rsp + 1800]
                        mov              rdx, qword ptr [rsp + 1808]
                        mov              rcx, qword ptr [rsp + 1816]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx288_240
                        add              rsp, 16
                                                                                        jmp   n114_var_α
.Lx288_240:
                        mov              qword ptr [rsp + 1824], rax
                        mov              qword ptr [rsp + 1832], rdx
                        add              rsp, 16
                                                                                        jmp   n112_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n112_lit_string_α:
                        mov              qword ptr [rsp + 1824], 1
                        mov              dword ptr [rsp + 1828], 2
                        mov              rax, qword ptr [rip + .Lx289_0]
                        mov              qword ptr [rsp + 1832], rax
                                                                                        jmp   n113_assign_var_α
.Lx289_0:
                        .quad            .Lx289_0_s
.Lx289_0_s:
                        .string          "XL"
#-----------------------------------------------------------------------------------------------------------------------
n113_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1808]
                        mov              rsi, qword ptr [rsp + 1816]
                        mov              rdx, qword ptr [rsp + 1824]
                        mov              rcx, qword ptr [rsp + 1832]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n114_var_α
                        mov              qword ptr [rsp + 1840], rax
                        mov              qword ptr [rsp + 1848], rdx
                                                                                        jmp   n114_var_α
#=======================================================================================================================
#         r<9> = 'X'
#-----------------------------------------------------------------------------------------------------------------------
n114_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 1856], rax
                        mov              qword ptr [rsp + 1864], rdx
                                                                                        jmp   n115_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n115_lit_integer_α:
                        mov              qword ptr [rsp + 1872], 6
                        mov              rax, qword ptr [rip + .Lx292_0]
                        mov              qword ptr [rsp + 1880], rax
                                                                                        jmp   n116_subscript_α
.Lx292_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n116_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1872]
                        mov              rsi, qword ptr [rsp + 1880]
                        mov              rdx, qword ptr [rsp + 1888]
                        mov              rcx, qword ptr [rsp + 1896]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx293_240
                        add              rsp, 16
                                                                                        jmp   n119_var_α
.Lx293_240:
                        mov              qword ptr [rsp + 1904], rax
                        mov              qword ptr [rsp + 1912], rdx
                        add              rsp, 16
                                                                                        jmp   n117_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n117_lit_string_α:
                        mov              qword ptr [rsp + 1904], 1
                        mov              dword ptr [rsp + 1908], 1
                        mov              rax, qword ptr [rip + .Lx294_0]
                        mov              qword ptr [rsp + 1912], rax
                                                                                        jmp   n118_assign_var_α
.Lx294_0:
                        .quad            .Lx294_0_s
.Lx294_0_s:
                        .string          "X"
#-----------------------------------------------------------------------------------------------------------------------
n118_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1888]
                        mov              rsi, qword ptr [rsp + 1896]
                        mov              rdx, qword ptr [rsp + 1904]
                        mov              rcx, qword ptr [rsp + 1912]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n119_var_α
                        mov              qword ptr [rsp + 1920], rax
                        mov              qword ptr [rsp + 1928], rdx
                                                                                        jmp   n119_var_α
#=======================================================================================================================
#         r<10> = 'IX'
#-----------------------------------------------------------------------------------------------------------------------
n119_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 1936], rax
                        mov              qword ptr [rsp + 1944], rdx
                                                                                        jmp   n120_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n120_lit_integer_α:
                        mov              qword ptr [rsp + 1952], 6
                        mov              rax, qword ptr [rip + .Lx297_0]
                        mov              qword ptr [rsp + 1960], rax
                                                                                        jmp   n121_subscript_α
.Lx297_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n121_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1952]
                        mov              rsi, qword ptr [rsp + 1960]
                        mov              rdx, qword ptr [rsp + 1968]
                        mov              rcx, qword ptr [rsp + 1976]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx298_240
                        add              rsp, 16
                                                                                        jmp   n124_var_α
.Lx298_240:
                        mov              qword ptr [rsp + 1984], rax
                        mov              qword ptr [rsp + 1992], rdx
                        add              rsp, 16
                                                                                        jmp   n122_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n122_lit_string_α:
                        mov              qword ptr [rsp + 1984], 1
                        mov              dword ptr [rsp + 1988], 2
                        mov              rax, qword ptr [rip + .Lx299_0]
                        mov              qword ptr [rsp + 1992], rax
                                                                                        jmp   n123_assign_var_α
.Lx299_0:
                        .quad            .Lx299_0_s
.Lx299_0_s:
                        .string          "IX"
#-----------------------------------------------------------------------------------------------------------------------
n123_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1968]
                        mov              rsi, qword ptr [rsp + 1976]
                        mov              rdx, qword ptr [rsp + 1984]
                        mov              rcx, qword ptr [rsp + 1992]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n124_var_α
                        mov              qword ptr [rsp + 2000], rax
                        mov              qword ptr [rsp + 2008], rdx
                                                                                        jmp   n124_var_α
#=======================================================================================================================
#         r<11> = 'V'
#-----------------------------------------------------------------------------------------------------------------------
n124_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 2016], rax
                        mov              qword ptr [rsp + 2024], rdx
                                                                                        jmp   n125_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n125_lit_integer_α:
                        mov              qword ptr [rsp + 2032], 6
                        mov              rax, qword ptr [rip + .Lx302_0]
                        mov              qword ptr [rsp + 2040], rax
                                                                                        jmp   n126_subscript_α
.Lx302_0:
                        .quad            11
#-----------------------------------------------------------------------------------------------------------------------
n126_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 2032]
                        mov              rsi, qword ptr [rsp + 2040]
                        mov              rdx, qword ptr [rsp + 2048]
                        mov              rcx, qword ptr [rsp + 2056]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx303_240
                        add              rsp, 16
                                                                                        jmp   n129_var_α
.Lx303_240:
                        mov              qword ptr [rsp + 2064], rax
                        mov              qword ptr [rsp + 2072], rdx
                        add              rsp, 16
                                                                                        jmp   n127_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n127_lit_string_α:
                        mov              qword ptr [rsp + 2064], 1
                        mov              dword ptr [rsp + 2068], 1
                        mov              rax, qword ptr [rip + .Lx304_0]
                        mov              qword ptr [rsp + 2072], rax
                                                                                        jmp   n128_assign_var_α
.Lx304_0:
                        .quad            .Lx304_0_s
.Lx304_0_s:
                        .string          "V"
#-----------------------------------------------------------------------------------------------------------------------
n128_assign_var_α:
                        mov              rdi, qword ptr [rsp + 2048]
                        mov              rsi, qword ptr [rsp + 2056]
                        mov              rdx, qword ptr [rsp + 2064]
                        mov              rcx, qword ptr [rsp + 2072]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n129_var_α
                        mov              qword ptr [rsp + 2080], rax
                        mov              qword ptr [rsp + 2088], rdx
                                                                                        jmp   n129_var_α
#=======================================================================================================================
#         r<12> = 'IV'
#-----------------------------------------------------------------------------------------------------------------------
n129_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 2096], rax
                        mov              qword ptr [rsp + 2104], rdx
                                                                                        jmp   n130_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n130_lit_integer_α:
                        mov              qword ptr [rsp + 2112], 6
                        mov              rax, qword ptr [rip + .Lx307_0]
                        mov              qword ptr [rsp + 2120], rax
                                                                                        jmp   n131_subscript_α
.Lx307_0:
                        .quad            12
#-----------------------------------------------------------------------------------------------------------------------
n131_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 2112]
                        mov              rsi, qword ptr [rsp + 2120]
                        mov              rdx, qword ptr [rsp + 2128]
                        mov              rcx, qword ptr [rsp + 2136]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx308_240
                        add              rsp, 16
                                                                                        jmp   n134_var_α
.Lx308_240:
                        mov              qword ptr [rsp + 2144], rax
                        mov              qword ptr [rsp + 2152], rdx
                        add              rsp, 16
                                                                                        jmp   n132_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n132_lit_string_α:
                        mov              qword ptr [rsp + 2144], 1
                        mov              dword ptr [rsp + 2148], 2
                        mov              rax, qword ptr [rip + .Lx309_0]
                        mov              qword ptr [rsp + 2152], rax
                                                                                        jmp   n133_assign_var_α
.Lx309_0:
                        .quad            .Lx309_0_s
.Lx309_0_s:
                        .string          "IV"
#-----------------------------------------------------------------------------------------------------------------------
n133_assign_var_α:
                        mov              rdi, qword ptr [rsp + 2128]
                        mov              rsi, qword ptr [rsp + 2136]
                        mov              rdx, qword ptr [rsp + 2144]
                        mov              rcx, qword ptr [rsp + 2152]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n134_var_α
                        mov              qword ptr [rsp + 2160], rax
                        mov              qword ptr [rsp + 2168], rdx
                                                                                        jmp   n134_var_α
#=======================================================================================================================
#         r<13> = 'I'
#-----------------------------------------------------------------------------------------------------------------------
n134_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 2176], rax
                        mov              qword ptr [rsp + 2184], rdx
                                                                                        jmp   n135_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n135_lit_integer_α:
                        mov              qword ptr [rsp + 2192], 6
                        mov              rax, qword ptr [rip + .Lx312_0]
                        mov              qword ptr [rsp + 2200], rax
                                                                                        jmp   n136_subscript_α
.Lx312_0:
                        .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n136_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 2192]
                        mov              rsi, qword ptr [rsp + 2200]
                        mov              rdx, qword ptr [rsp + 2208]
                        mov              rcx, qword ptr [rsp + 2216]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx313_240
                        add              rsp, 16
                                                                                        jmp   n139_lit_integer_α
.Lx313_240:
                        mov              qword ptr [rsp + 2224], rax
                        mov              qword ptr [rsp + 2232], rdx
                        add              rsp, 16
                                                                                        jmp   n137_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n137_lit_string_α:
                        mov              qword ptr [rsp + 2224], 1
                        mov              dword ptr [rsp + 2228], 1
                        mov              rax, qword ptr [rip + .Lx314_0]
                        mov              qword ptr [rsp + 2232], rax
                                                                                        jmp   n138_assign_var_α
.Lx314_0:
                        .quad            .Lx314_0_s
.Lx314_0_s:
                        .string          "I"
#-----------------------------------------------------------------------------------------------------------------------
n138_assign_var_α:
                        mov              rdi, qword ptr [rsp + 2208]
                        mov              rsi, qword ptr [rsp + 2216]
                        mov              rdx, qword ptr [rsp + 2224]
                        mov              rcx, qword ptr [rsp + 2232]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n139_lit_integer_α
                        mov              qword ptr [rsp + 2240], rax
                        mov              qword ptr [rsp + 2248], rdx
                                                                                        jmp   n139_lit_integer_α
#=======================================================================================================================
#         i = 1
#-----------------------------------------------------------------------------------------------------------------------
n139_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx316_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n140_assign_α
.Lx316_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n140_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052368], rax
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n141_var_α
#=======================================================================================================================
# RLOOP   GT(n, 0)                                                       :F(RDONE)
#-----------------------------------------------------------------------------------------------------------------------
n141_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 2320], rax
                        mov              qword ptr [rsp + 2328], rdx
                                                                                        jmp   n142_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n142_lit_integer_α:
                        mov              qword ptr [rsp + 2304], 6
                        mov              rax, qword ptr [rip + .Lx319_0]
                        mov              qword ptr [rsp + 2312], rax
                                                                                        jmp   n143_coerce_numeric_α
.Lx319_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n143_coerce_numeric_α:
                        mov              eax, dword ptr [rsp + 2320]
                        cmp              eax, 7
                                                                                        je    .Lx321_1
                        cmp              eax, 6
                                                                                        jne   .Lx321_0
                        mov              eax, dword ptr [rsp + 2304]
                        cmp              eax, 6
                                                                                        jne   .Lx321_0
.Lx321_1:
                        mov              rax, qword ptr [rsp + 2320]
                        mov              qword ptr [rsp + 2288], rax
                        mov              rax, qword ptr [rsp + 2328]
                        mov              qword ptr [rsp + 2296], rax
                                                                                        jmp   n144_coerce_numeric_α
.Lx321_0:
                        lea              rdi, [rsp + 2320]
                        lea              rsi, [rsp + 2304]
                        lea              rdx, [rsp + 2288]
                        mov              rcx, 111
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n144_coerce_numeric_α
#-----------------------------------------------------------------------------------------------------------------------
n144_coerce_numeric_α:
                        mov              eax, dword ptr [rsp + 2304]
                        cmp              eax, 7
                                                                                        je    .Lx323_1
                        cmp              eax, 6
                                                                                        jne   .Lx323_0
                        mov              eax, dword ptr [rsp + 2320]
                        cmp              eax, 6
                                                                                        jne   .Lx323_0
.Lx323_1:
                        mov              rax, qword ptr [rsp + 2304]
                        mov              qword ptr [rsp + 2272], rax
                        mov              rax, qword ptr [rsp + 2312]
                        mov              qword ptr [rsp + 2280], rax
                                                                                        jmp   n145_cmp_test_α
.Lx323_0:
                        lea              rdi, [rsp + 2304]
                        lea              rsi, [rsp + 2320]
                        lea              rdx, [rsp + 2272]
                        mov              rcx, 112
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n145_cmp_test_α
#-----------------------------------------------------------------------------------------------------------------------
n145_cmp_test_α:
                        lea              rdi, [rsp + 2288]
                        lea              rsi, [rsp + 2272]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jle   n172_var_α
                                                                                        jmp   n146_var_α
#=======================================================================================================================
#         GE(n, v<i>)                                                    :F(RNEXT)
#-----------------------------------------------------------------------------------------------------------------------
n146_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 2432], rax
                        mov              qword ptr [rsp + 2440], rdx
                                                                                        jmp   n147_var_α
#-----------------------------------------------------------------------------------------------------------------------
n147_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 2368], rax
                        mov              qword ptr [rsp + 2376], rdx
                                                                                        jmp   n148_var_α
#-----------------------------------------------------------------------------------------------------------------------
n148_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 2384], rax
                        mov              qword ptr [rsp + 2392], rdx
                                                                                        jmp   n149_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n149_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 2384]
                        mov              rsi, qword ptr [rsp + 2392]
                        mov              rdx, qword ptr [rsp + 2400]
                        mov              rcx, qword ptr [rsp + 2408]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx329_240
                        add              rsp, 16
                                                                                        jmp   n168_var_α
.Lx329_240:
                        mov              qword ptr [rsp + 2416], rax
                        mov              qword ptr [rsp + 2424], rdx
                        add              rsp, 16
                                                                                        jmp   n150_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n150_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 2416]
                        mov              rsi, qword ptr [rsp + 2424]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx330_240
                        add              rsp, 16
                                                                                        jmp   n168_var_α
.Lx330_240:
                        mov              qword ptr [rsp + 2432], rax
                        mov              qword ptr [rsp + 2440], rdx
                        add              rsp, 16
                                                                                        jmp   n151_coerce_numeric_α
#-----------------------------------------------------------------------------------------------------------------------
n151_coerce_numeric_α:
                        mov              eax, dword ptr [rsp + 2432]
                        cmp              eax, 7
                                                                                        je    .Lx332_1
                        cmp              eax, 6
                                                                                        jne   .Lx332_0
                        mov              eax, dword ptr [rsp + 2416]
                        cmp              eax, 6
                                                                                        jne   .Lx332_0
.Lx332_1:
                        mov              rax, qword ptr [rsp + 2432]
                        mov              qword ptr [rsp + 2352], rax
                        mov              rax, qword ptr [rsp + 2440]
                        mov              qword ptr [rsp + 2360], rax
                                                                                        jmp   n152_coerce_numeric_α
.Lx332_0:
                        lea              rdi, [rsp + 2432]
                        lea              rsi, [rsp + 2416]
                        lea              rdx, [rsp + 2352]
                        mov              rcx, 109
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n152_coerce_numeric_α
#-----------------------------------------------------------------------------------------------------------------------
n152_coerce_numeric_α:
                        mov              eax, dword ptr [rsp + 2416]
                        cmp              eax, 7
                                                                                        je    .Lx334_1
                        cmp              eax, 6
                                                                                        jne   .Lx334_0
                        mov              eax, dword ptr [rsp + 2432]
                        cmp              eax, 6
                                                                                        jne   .Lx334_0
.Lx334_1:
                        mov              rax, qword ptr [rsp + 2416]
                        mov              qword ptr [rsp + 2336], rax
                        mov              rax, qword ptr [rsp + 2424]
                        mov              qword ptr [rsp + 2344], rax
                                                                                        jmp   n153_cmp_test_α
.Lx334_0:
                        lea              rdi, [rsp + 2416]
                        lea              rsi, [rsp + 2432]
                        lea              rdx, [rsp + 2336]
                        mov              rcx, 110
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n153_cmp_test_α
#-----------------------------------------------------------------------------------------------------------------------
n153_cmp_test_α:
                        lea              rdi, [rsp + 2352]
                        lea              rsi, [rsp + 2336]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        js    n168_var_α
                                                                                        jmp   n154_var_α
#=======================================================================================================================
#         s = s r<i>
#-----------------------------------------------------------------------------------------------------------------------
n154_var_α:
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 2464], rax
                        mov              qword ptr [rsp + 2472], rdx
                                                                                        jmp   n155_var_α
#-----------------------------------------------------------------------------------------------------------------------
n155_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 2480], rax
                        mov              qword ptr [rsp + 2488], rdx
                                                                                        jmp   n156_var_α
#-----------------------------------------------------------------------------------------------------------------------
n156_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 2496], rax
                        mov              qword ptr [rsp + 2504], rdx
                                                                                        jmp   n157_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n157_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 2496]
                        mov              rsi, qword ptr [rsp + 2504]
                        mov              rdx, qword ptr [rsp + 2512]
                        mov              rcx, qword ptr [rsp + 2520]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx340_240
                        add              rsp, 16
                                                                                        jmp   n161_var_α
.Lx340_240:
                        mov              qword ptr [rsp + 2528], rax
                        mov              qword ptr [rsp + 2536], rdx
                        add              rsp, 16
                                                                                        jmp   n158_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n158_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 2528]
                        mov              rsi, qword ptr [rsp + 2536]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx341_240
                        add              rsp, 16
                                                                                        jmp   n161_var_α
.Lx341_240:
                        mov              qword ptr [rsp + 2544], rax
                        mov              qword ptr [rsp + 2552], rdx
                        add              rsp, 16
                                                                                        jmp   n159_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n159_binop_α:
                        mov              rdi, qword ptr [rsp + 2464]
                        mov              rsi, qword ptr [rsp + 2472]
                        mov              rdx, qword ptr [rsp + 2528]
                        mov              rcx, qword ptr [rsp + 2536]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 2448], rax
                        mov              qword ptr [rsp + 2456], rdx
                                                                                        jmp   n160_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n160_assign_α:
                        mov              rax, qword ptr [rsp + 2448]
                        mov              rdx, qword ptr [rsp + 2456]
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rdx
                                                                                        jmp   n161_var_α
#=======================================================================================================================
#         n = n - v<i>                                                   :(RLOOP)
#-----------------------------------------------------------------------------------------------------------------------
n161_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 2560], rax
                        mov              qword ptr [rsp + 2568], rdx
                                                                                        jmp   n162_var_α
#-----------------------------------------------------------------------------------------------------------------------
n162_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 2576], rax
                        mov              qword ptr [rsp + 2584], rdx
                                                                                        jmp   n163_var_α
#-----------------------------------------------------------------------------------------------------------------------
n163_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 2592], rax
                        mov              qword ptr [rsp + 2600], rdx
                                                                                        jmp   n164_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n164_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 2592]
                        mov              rsi, qword ptr [rsp + 2600]
                        mov              rdx, qword ptr [rsp + 2608]
                        mov              rcx, qword ptr [rsp + 2616]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx347_240
                        add              rsp, 16
                                                                                        jmp   n141_var_α
.Lx347_240:
                        mov              qword ptr [rsp + 2624], rax
                        mov              qword ptr [rsp + 2632], rdx
                        add              rsp, 16
                                                                                        jmp   n165_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n165_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 2624]
                        mov              rsi, qword ptr [rsp + 2632]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx348_240
                        add              rsp, 16
                                                                                        jmp   n141_var_α
.Lx348_240:
                        mov              qword ptr [rsp + 2640], rax
                        mov              qword ptr [rsp + 2648], rdx
                        add              rsp, 16
                                                                                        jmp   n166_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n166_binop_α:
                        mov              eax, dword ptr [rsp + 2560]
                        cmp              eax, 6
                                                                                        jne   .Lx349_0
                        mov              eax, dword ptr [rsp + 2624]
                        cmp              eax, 6
                                                                                        jne   .Lx349_0
                        mov              rax, qword ptr [rsp + 2568]
                        mov              rcx, qword ptr [rsp + 2632]
                        sub              rax, rcx
                        mov              qword ptr [rsp + 2544], 6
                        mov              qword ptr [rsp + 2552], rax
                                                                                        jmp   n167_assign_α
.Lx349_0:
                        mov              rdi, qword ptr [rsp + 2560]
                        mov              rsi, qword ptr [rsp + 2568]
                        mov              rdx, qword ptr [rsp + 2624]
                        mov              rcx, qword ptr [rsp + 2632]
                        call             rt_sub@PLT
                        cmp              eax, 99
                                                                                        je    n141_var_α
                        mov              qword ptr [rsp + 2544], rax
                        mov              qword ptr [rsp + 2552], rdx
                                                                                        jmp   n167_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n167_assign_α:
                        mov              rax, qword ptr [rsp + 2544]
                        mov              rdx, qword ptr [rsp + 2552]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   n141_var_α
#=======================================================================================================================
# RNEXT   i = i + 1                                                      :(RLOOP)
#-----------------------------------------------------------------------------------------------------------------------
n168_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n169_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n169_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx352_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n170_binop_α
.Lx352_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n170_binop_α:
                        mov              eax, dword ptr [rsp + 16]
                        cmp              eax, 6
                                                                                        jne   .Lx353_0
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, 6
                                                                                        jne   .Lx353_0
                        mov              rax, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rsp + 8]
                        add              rax, rcx
                        add              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n171_assign_α
.Lx353_0:
                        mov              rdi, qword ptr [rsp + 16]
                        mov              rsi, qword ptr [rsp + 24]
                        mov              rdx, qword ptr [rsp + 0]
                        mov              rcx, qword ptr [rsp + 8]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx353_240
                        add              rsp, 32
                                                                                        jmp   n141_var_α
.Lx353_240:
                        add              rsp, 16
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n171_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n171_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052368], rax
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n141_var_α
#=======================================================================================================================
# RDONE   roman = s                                                      :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n172_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n173_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n173_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n174_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n174_save_restore_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__roman_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__roman_β:
                                                                                        jmp   proc_LBL__roman_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__roman_γ:
                        mov              rdi, [rsp]
                        mov              rsi, [rsp + 8]
                        mov              rax, [rsp + 3112]
                        add              rsp, 3136
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__roman_ω:
                        mov              rax, [rsp + 3120]
                        add              rsp, 3136
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_roman_α
proc_roman_α:
                        .global          proc_roman_α
                        .global          proc_roman_β
                        .global          proc_roman_γ
                        .global          proc_roman_ω
                        sub              rsp, 64
                        mov              [rsp + 40], rcx
                        mov              [rsp + 48], rdx
                        mov              rdi, rsp
                        mov              ecx, 32
                        xor              eax, eax
                        rep stosb
proc_roman_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n359_save_restore_α:
                        mov              rdi, qword ptr [rsp + 40]
                        mov              rsi, qword ptr [rsp + 48]
                        lea              rdx, [rsp + 64]
                        mov              rcx, rbp
                        call             rt_flat_wire_adopt@PLT
                                                                                        jmp   n360_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n360_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx364_0]
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx364_1
.Lx364_0:
                        .quad            .Lx364_0_s
.Lx364_0_s:
                        .string          "roman"
.Lx364_1:
                                                                                        jmp   proc_roman_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_roman_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_roman_β:
                                                                                        jmp   proc_roman_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_roman_γ:
                        mov              rdi, [rsp]
                        mov              rsi, [rsp + 8]
                        mov              rax, [rsp + 40]
                        add              rsp, 64
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_roman_ω:
                        mov              rax, [rsp + 48]
                        add              rsp, 64
                                                                                        jmp   rax
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lstartup_pname0:       .string          "LBL__roman"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_LBL__roman_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 3088
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname1:       .string          "roman"
.Lstartup_pp1_0:        .string          "n"
.Lstartup_pp1_1:        .string          "s"
.Lstartup_pp1_2:        .string          "v"
.Lstartup_pp1_3:        .string          "r"
.Lstartup_pp1_4:        .string          "i"
                        .align           8
.Lstartup_pnames1:
                        .quad            .Lstartup_pp1_0
                        .quad            .Lstartup_pp1_1
                        .quad            .Lstartup_pp1_2
                        .quad            .Lstartup_pp1_3
                        .quad            .Lstartup_pp1_4
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname1]
                        lea              rsi, [rip + .Lstartup_pnames1]
                        mov              edx, 5
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        lea              rsi, [rip + proc_roman_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 5
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 16
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        add              rsp, 8
                        ret
                        .section         .rodata
.Lgvan0:                .string          "roman"
.Lgvan1:                .string          "n"
.Lgvan2:                .string          "s"
.Lgvan3:                .string          "v"
.Lgvan4:                .string          "r"
.Lgvan5:                .string          "i"
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
                        sub              rsp, 3096
                        mov              rdi, rsp
                        mov              ecx, 3096
                        xor              eax, eax
                        rep stosb
main_α_body:
#=======================================================================================================================
#         DEFINE('roman(n)s,v,r,i')                                     :(roman_end)
#         OUTPUT = roman(1)
#-----------------------------------------------------------------------------------------------------------------------
n365_lit_integer_α:
                        mov              qword ptr [rsp + 2752], 6
                        mov              rax, qword ptr [rip + .Lx562_0]
                        mov              qword ptr [rsp + 2760], rax
                                                                                        jmp   n366_call_α
.Lx562_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n366_call_α:
                        sub              rsp, 96
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [1879052336]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [1879052352]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 56], rax
                        mov              rax, qword ptr [1879052368]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [1879052376]
                        mov              qword ptr [rsp + 72], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 88], rax
                        mov              rdi, qword ptr [rip + .Lx564_0]
                        mov              esi, 5
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx564_5
                        mov              rax, qword ptr [rsp + 2848]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 2856]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx564_6]
                        lea              rdx, [rip + .Lx564_7]
                                                                                        jmp   rax
.Lx564_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx564_2
.Lx564_7:
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx564_2
.Lx564_5:
                        add              rsp, 96
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx564_20
                        mov              rax, qword ptr [rsp + 2752]
                        mov              rdx, qword ptr [rsp + 2760]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx564_21
.Lx564_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 2752]
                        mov              rdx, qword ptr [rsp + 2760]
                        call             rt_arg_stage@PLT
.Lx564_21:
                        mov              rdi, qword ptr [rip + .Lx564_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx564_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx564_3]
                        lea              rdx, [rip + .Lx564_4]
                                                                                        jmp   rax
.Lx564_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx564_2
.Lx564_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx564_2
.Lx564_1:
                        call             rt_faildescr@PLT
.Lx564_2:
                        mov              qword ptr [rsp + 2704], rax
                        mov              qword ptr [rsp + 2712], rdx
                        cmp              eax, 99
                                                                                        je    n368_lit_integer_α
                                                                                        jmp   n367_assign_α
n366_call_β:
                                                                                        jmp   n368_lit_integer_α
.Lx564_0:
                        .quad            .Lx564_0_s
.Lx564_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n367_assign_α:
                        mov              rsi, qword ptr [rsp + 2704]
                        mov              rdx, qword ptr [rsp + 2712]
                        mov              rdi, qword ptr [rip + .Lx565_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n368_lit_integer_α
.Lx565_0:
                        .quad            .Lx565_0_s
.Lx565_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#         OUTPUT = roman(4)
#-----------------------------------------------------------------------------------------------------------------------
n368_lit_integer_α:
                        mov              qword ptr [rsp + 2816], 6
                        mov              rax, qword ptr [rip + .Lx566_0]
                        mov              qword ptr [rsp + 2824], rax
                                                                                        jmp   n369_call_α
.Lx566_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n369_call_α:
                        sub              rsp, 96
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [1879052336]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [1879052352]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 56], rax
                        mov              rax, qword ptr [1879052368]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [1879052376]
                        mov              qword ptr [rsp + 72], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 88], rax
                        mov              rdi, qword ptr [rip + .Lx568_0]
                        mov              esi, 5
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx568_5
                        mov              rax, qword ptr [rsp + 2912]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 2920]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx568_6]
                        lea              rdx, [rip + .Lx568_7]
                                                                                        jmp   rax
.Lx568_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx568_2
.Lx568_7:
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx568_2
.Lx568_5:
                        add              rsp, 96
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx568_20
                        mov              rax, qword ptr [rsp + 2816]
                        mov              rdx, qword ptr [rsp + 2824]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx568_21
.Lx568_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 2816]
                        mov              rdx, qword ptr [rsp + 2824]
                        call             rt_arg_stage@PLT
.Lx568_21:
                        mov              rdi, qword ptr [rip + .Lx568_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx568_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx568_3]
                        lea              rdx, [rip + .Lx568_4]
                                                                                        jmp   rax
.Lx568_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx568_2
.Lx568_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx568_2
.Lx568_1:
                        call             rt_faildescr@PLT
.Lx568_2:
                        mov              qword ptr [rsp + 2768], rax
                        mov              qword ptr [rsp + 2776], rdx
                        cmp              eax, 99
                                                                                        je    n371_lit_integer_α
                                                                                        jmp   n370_assign_α
n369_call_β:
                                                                                        jmp   n371_lit_integer_α
.Lx568_0:
                        .quad            .Lx568_0_s
.Lx568_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n370_assign_α:
                        mov              rsi, qword ptr [rsp + 2768]
                        mov              rdx, qword ptr [rsp + 2776]
                        mov              rdi, qword ptr [rip + .Lx569_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n371_lit_integer_α
.Lx569_0:
                        .quad            .Lx569_0_s
.Lx569_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#         OUTPUT = roman(9)
#-----------------------------------------------------------------------------------------------------------------------
n371_lit_integer_α:
                        mov              qword ptr [rsp + 2880], 6
                        mov              rax, qword ptr [rip + .Lx570_0]
                        mov              qword ptr [rsp + 2888], rax
                                                                                        jmp   n372_call_α
.Lx570_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n372_call_α:
                        sub              rsp, 96
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [1879052336]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [1879052352]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 56], rax
                        mov              rax, qword ptr [1879052368]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [1879052376]
                        mov              qword ptr [rsp + 72], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 88], rax
                        mov              rdi, qword ptr [rip + .Lx572_0]
                        mov              esi, 5
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx572_5
                        mov              rax, qword ptr [rsp + 2976]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 2984]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx572_6]
                        lea              rdx, [rip + .Lx572_7]
                                                                                        jmp   rax
.Lx572_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx572_2
.Lx572_7:
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx572_2
.Lx572_5:
                        add              rsp, 96
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx572_20
                        mov              rax, qword ptr [rsp + 2880]
                        mov              rdx, qword ptr [rsp + 2888]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx572_21
.Lx572_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 2880]
                        mov              rdx, qword ptr [rsp + 2888]
                        call             rt_arg_stage@PLT
.Lx572_21:
                        mov              rdi, qword ptr [rip + .Lx572_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx572_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx572_3]
                        lea              rdx, [rip + .Lx572_4]
                                                                                        jmp   rax
.Lx572_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx572_2
.Lx572_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx572_2
.Lx572_1:
                        call             rt_faildescr@PLT
.Lx572_2:
                        mov              qword ptr [rsp + 2832], rax
                        mov              qword ptr [rsp + 2840], rdx
                        cmp              eax, 99
                                                                                        je    n374_lit_integer_α
                                                                                        jmp   n373_assign_α
n372_call_β:
                                                                                        jmp   n374_lit_integer_α
.Lx572_0:
                        .quad            .Lx572_0_s
.Lx572_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n373_assign_α:
                        mov              rsi, qword ptr [rsp + 2832]
                        mov              rdx, qword ptr [rsp + 2840]
                        mov              rdi, qword ptr [rip + .Lx573_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n374_lit_integer_α
.Lx573_0:
                        .quad            .Lx573_0_s
.Lx573_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#         OUTPUT = roman(42)
#-----------------------------------------------------------------------------------------------------------------------
n374_lit_integer_α:
                        mov              qword ptr [rsp + 2944], 6
                        mov              rax, qword ptr [rip + .Lx574_0]
                        mov              qword ptr [rsp + 2952], rax
                                                                                        jmp   n375_call_α
.Lx574_0:
                        .quad            42
#-----------------------------------------------------------------------------------------------------------------------
n375_call_α:
                        sub              rsp, 96
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [1879052336]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [1879052352]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 56], rax
                        mov              rax, qword ptr [1879052368]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [1879052376]
                        mov              qword ptr [rsp + 72], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 88], rax
                        mov              rdi, qword ptr [rip + .Lx576_0]
                        mov              esi, 5
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx576_5
                        mov              rax, qword ptr [rsp + 3040]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 3048]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx576_6]
                        lea              rdx, [rip + .Lx576_7]
                                                                                        jmp   rax
.Lx576_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx576_2
.Lx576_7:
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx576_2
.Lx576_5:
                        add              rsp, 96
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx576_20
                        mov              rax, qword ptr [rsp + 2944]
                        mov              rdx, qword ptr [rsp + 2952]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx576_21
.Lx576_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 2944]
                        mov              rdx, qword ptr [rsp + 2952]
                        call             rt_arg_stage@PLT
.Lx576_21:
                        mov              rdi, qword ptr [rip + .Lx576_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx576_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx576_3]
                        lea              rdx, [rip + .Lx576_4]
                                                                                        jmp   rax
.Lx576_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx576_2
.Lx576_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx576_2
.Lx576_1:
                        call             rt_faildescr@PLT
.Lx576_2:
                        mov              qword ptr [rsp + 2896], rax
                        mov              qword ptr [rsp + 2904], rdx
                        cmp              eax, 99
                                                                                        je    n377_lit_integer_α
                                                                                        jmp   n376_assign_α
n375_call_β:
                                                                                        jmp   n377_lit_integer_α
.Lx576_0:
                        .quad            .Lx576_0_s
.Lx576_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n376_assign_α:
                        mov              rsi, qword ptr [rsp + 2896]
                        mov              rdx, qword ptr [rsp + 2904]
                        mov              rdi, qword ptr [rip + .Lx577_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n377_lit_integer_α
.Lx577_0:
                        .quad            .Lx577_0_s
.Lx577_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#         OUTPUT = roman(1999)
#-----------------------------------------------------------------------------------------------------------------------
n377_lit_integer_α:
                        mov              qword ptr [rsp + 3008], 6
                        mov              rax, qword ptr [rip + .Lx578_0]
                        mov              qword ptr [rsp + 3016], rax
                                                                                        jmp   n378_call_α
.Lx578_0:
                        .quad            1999
#-----------------------------------------------------------------------------------------------------------------------
n378_call_α:
                        sub              rsp, 96
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [1879052336]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [1879052352]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 56], rax
                        mov              rax, qword ptr [1879052368]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [1879052376]
                        mov              qword ptr [rsp + 72], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 88], rax
                        mov              rdi, qword ptr [rip + .Lx580_0]
                        mov              esi, 5
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx580_5
                        mov              rax, qword ptr [rsp + 3104]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 3112]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx580_6]
                        lea              rdx, [rip + .Lx580_7]
                                                                                        jmp   rax
.Lx580_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx580_2
.Lx580_7:
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx580_2
.Lx580_5:
                        add              rsp, 96
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx580_20
                        mov              rax, qword ptr [rsp + 3008]
                        mov              rdx, qword ptr [rsp + 3016]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx580_21
.Lx580_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 3008]
                        mov              rdx, qword ptr [rsp + 3016]
                        call             rt_arg_stage@PLT
.Lx580_21:
                        mov              rdi, qword ptr [rip + .Lx580_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx580_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx580_3]
                        lea              rdx, [rip + .Lx580_4]
                                                                                        jmp   rax
.Lx580_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx580_2
.Lx580_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx580_2
.Lx580_1:
                        call             rt_faildescr@PLT
.Lx580_2:
                        mov              qword ptr [rsp + 2960], rax
                        mov              qword ptr [rsp + 2968], rdx
                        cmp              eax, 99
                                                                                        je    n380_lit_integer_α
                                                                                        jmp   n379_assign_α
n378_call_β:
                                                                                        jmp   n380_lit_integer_α
.Lx580_0:
                        .quad            .Lx580_0_s
.Lx580_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n379_assign_α:
                        mov              rsi, qword ptr [rsp + 2960]
                        mov              rdx, qword ptr [rsp + 2968]
                        mov              rdi, qword ptr [rip + .Lx581_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n380_lit_integer_α
.Lx581_0:
                        .quad            .Lx581_0_s
.Lx581_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#         OUTPUT = roman(2024)
#-----------------------------------------------------------------------------------------------------------------------
n380_lit_integer_α:
                        mov              qword ptr [rsp + 3072], 6
                        mov              rax, qword ptr [rip + .Lx582_0]
                        mov              qword ptr [rsp + 3080], rax
                                                                                        jmp   n381_call_α
.Lx582_0:
                        .quad            2024
#-----------------------------------------------------------------------------------------------------------------------
n381_call_α:
                        sub              rsp, 96
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [1879052336]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [1879052352]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 56], rax
                        mov              rax, qword ptr [1879052368]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [1879052376]
                        mov              qword ptr [rsp + 72], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 88], rax
                        mov              rdi, qword ptr [rip + .Lx584_0]
                        mov              esi, 5
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx584_5
                        mov              rax, qword ptr [rsp + 3168]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 3176]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx584_6]
                        lea              rdx, [rip + .Lx584_7]
                                                                                        jmp   rax
.Lx584_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx584_2
.Lx584_7:
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx584_2
.Lx584_5:
                        add              rsp, 96
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx584_20
                        mov              rax, qword ptr [rsp + 3072]
                        mov              rdx, qword ptr [rsp + 3080]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx584_21
.Lx584_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 3072]
                        mov              rdx, qword ptr [rsp + 3080]
                        call             rt_arg_stage@PLT
.Lx584_21:
                        mov              rdi, qword ptr [rip + .Lx584_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx584_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx584_3]
                        lea              rdx, [rip + .Lx584_4]
                                                                                        jmp   rax
.Lx584_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx584_2
.Lx584_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx584_2
.Lx584_1:
                        call             rt_faildescr@PLT
.Lx584_2:
                        mov              qword ptr [rsp + 3024], rax
                        mov              qword ptr [rsp + 3032], rdx
                        cmp              eax, 99
                                                                                        je    main_γ
                                                                                        jmp   n382_assign_α
n381_call_β:
                                                                                        jmp   main_γ
.Lx584_0:
                        .quad            .Lx584_0_s
.Lx584_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n382_assign_α:
                        mov              rsi, qword ptr [rsp + 3024]
                        mov              rdx, qword ptr [rsp + 3032]
                        mov              rdi, qword ptr [rip + .Lx585_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   main_γ
.Lx585_0:
                        .quad            .Lx585_0_s
.Lx585_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n383_goto_α:
                                                                                        jmp   n384_lit_string_α
n383_goto_β:
                                                                                        jmp   main_ω
#=======================================================================================================================
# roman   s = ''
#-----------------------------------------------------------------------------------------------------------------------
n384_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx587_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n385_assign_α
.Lx587_0:
                        .quad            .Lx587_0_s
.Lx587_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n385_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rdx
                        add              rsp, 16
                                                                                        jmp   n386_lit_integer_α
#=======================================================================================================================
#         v = ARRAY(13)
#-----------------------------------------------------------------------------------------------------------------------
n386_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx589_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n387_call_α
.Lx589_0:
                        .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n387_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd591:            .string          "ARRAY"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd591]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 99
                                                                                        jne   .Lx590_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n389_var_α
.Lx590_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n388_assign_α
n387_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n389_var_α
#-----------------------------------------------------------------------------------------------------------------------
n388_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052336], rax
                        mov              qword ptr [1879052344], rdx
                        add              rsp, 32
                                                                                        jmp   n389_var_α
#=======================================================================================================================
#         v<1> = 1000
#-----------------------------------------------------------------------------------------------------------------------
n389_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n390_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n390_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx594_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n391_subscript_α
.Lx594_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n391_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx595_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n394_var_α
.Lx595_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n392_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n392_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx596_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n393_assign_var_α
.Lx596_0:
                        .quad            1000
#-----------------------------------------------------------------------------------------------------------------------
n393_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx597_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n394_var_α
.Lx597_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        add              rsp, 80
                                                                                        jmp   n394_var_α
#=======================================================================================================================
#         v<2> = 900
#-----------------------------------------------------------------------------------------------------------------------
n394_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n395_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n395_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx599_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n396_subscript_α
.Lx599_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n396_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx600_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n399_var_α
.Lx600_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n397_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n397_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx601_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n398_assign_var_α
.Lx601_0:
                        .quad            900
#-----------------------------------------------------------------------------------------------------------------------
n398_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx602_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n399_var_α
.Lx602_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        add              rsp, 80
                                                                                        jmp   n399_var_α
#=======================================================================================================================
#         v<3> = 500
#-----------------------------------------------------------------------------------------------------------------------
n399_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n400_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n400_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx604_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n401_subscript_α
.Lx604_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n401_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx605_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n404_var_α
.Lx605_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n402_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n402_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx606_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n403_assign_var_α
.Lx606_0:
                        .quad            500
#-----------------------------------------------------------------------------------------------------------------------
n403_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx607_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n404_var_α
.Lx607_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        add              rsp, 80
                                                                                        jmp   n404_var_α
#=======================================================================================================================
#         v<4> = 400
#-----------------------------------------------------------------------------------------------------------------------
n404_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n405_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n405_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx609_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n406_subscript_α
.Lx609_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n406_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx610_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n409_var_α
.Lx610_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n407_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n407_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx611_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n408_assign_var_α
.Lx611_0:
                        .quad            400
#-----------------------------------------------------------------------------------------------------------------------
n408_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx612_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n409_var_α
.Lx612_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        add              rsp, 80
                                                                                        jmp   n409_var_α
#=======================================================================================================================
#         v<5> = 100
#-----------------------------------------------------------------------------------------------------------------------
n409_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n410_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n410_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx614_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n411_subscript_α
.Lx614_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n411_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx615_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n414_var_α
.Lx615_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n412_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n412_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx616_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n413_assign_var_α
.Lx616_0:
                        .quad            100
#-----------------------------------------------------------------------------------------------------------------------
n413_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx617_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n414_var_α
.Lx617_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        add              rsp, 80
                                                                                        jmp   n414_var_α
#=======================================================================================================================
#         v<6> = 90
#-----------------------------------------------------------------------------------------------------------------------
n414_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n415_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n415_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx619_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n416_subscript_α
.Lx619_0:
                        .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n416_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx620_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n419_var_α
.Lx620_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n417_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n417_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx621_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n418_assign_var_α
.Lx621_0:
                        .quad            90
#-----------------------------------------------------------------------------------------------------------------------
n418_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx622_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n419_var_α
.Lx622_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        add              rsp, 80
                                                                                        jmp   n419_var_α
#=======================================================================================================================
#         v<7> = 50
#-----------------------------------------------------------------------------------------------------------------------
n419_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n420_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n420_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx624_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n421_subscript_α
.Lx624_0:
                        .quad            7
#-----------------------------------------------------------------------------------------------------------------------
n421_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx625_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n424_var_α
.Lx625_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n422_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n422_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx626_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n423_assign_var_α
.Lx626_0:
                        .quad            50
#-----------------------------------------------------------------------------------------------------------------------
n423_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx627_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n424_var_α
.Lx627_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        add              rsp, 80
                                                                                        jmp   n424_var_α
#=======================================================================================================================
#         v<8> = 40
#-----------------------------------------------------------------------------------------------------------------------
n424_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n425_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n425_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx629_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n426_subscript_α
.Lx629_0:
                        .quad            8
#-----------------------------------------------------------------------------------------------------------------------
n426_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx630_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n429_var_α
.Lx630_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n427_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n427_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx631_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n428_assign_var_α
.Lx631_0:
                        .quad            40
#-----------------------------------------------------------------------------------------------------------------------
n428_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx632_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n429_var_α
.Lx632_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        add              rsp, 80
                                                                                        jmp   n429_var_α
#=======================================================================================================================
#         v<9> = 10
#-----------------------------------------------------------------------------------------------------------------------
n429_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n430_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n430_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx634_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n431_subscript_α
.Lx634_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n431_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx635_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n434_var_α
.Lx635_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n432_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n432_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx636_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n433_assign_var_α
.Lx636_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n433_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx637_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n434_var_α
.Lx637_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        add              rsp, 80
                                                                                        jmp   n434_var_α
#=======================================================================================================================
#         v<10> = 9
#-----------------------------------------------------------------------------------------------------------------------
n434_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n435_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n435_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx639_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n436_subscript_α
.Lx639_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n436_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx640_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n439_var_α
.Lx640_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n437_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n437_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx641_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n438_assign_var_α
.Lx641_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n438_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx642_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n439_var_α
.Lx642_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        add              rsp, 80
                                                                                        jmp   n439_var_α
#=======================================================================================================================
#         v<11> = 5
#-----------------------------------------------------------------------------------------------------------------------
n439_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n440_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n440_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx644_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n441_subscript_α
.Lx644_0:
                        .quad            11
#-----------------------------------------------------------------------------------------------------------------------
n441_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx645_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n444_var_α
.Lx645_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n442_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n442_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx646_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n443_assign_var_α
.Lx646_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n443_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx647_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n444_var_α
.Lx647_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        add              rsp, 80
                                                                                        jmp   n444_var_α
#=======================================================================================================================
#         v<12> = 4
#-----------------------------------------------------------------------------------------------------------------------
n444_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n445_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n445_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx649_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n446_subscript_α
.Lx649_0:
                        .quad            12
#-----------------------------------------------------------------------------------------------------------------------
n446_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx650_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n449_var_α
.Lx650_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n447_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n447_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx651_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n448_assign_var_α
.Lx651_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n448_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx652_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n449_var_α
.Lx652_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        add              rsp, 80
                                                                                        jmp   n449_var_α
#=======================================================================================================================
#         v<13> = 1
#-----------------------------------------------------------------------------------------------------------------------
n449_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n450_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n450_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx654_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n451_subscript_α
.Lx654_0:
                        .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n451_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx655_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n454_lit_integer_α
.Lx655_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n452_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n452_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx656_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n453_assign_var_α
.Lx656_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n453_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx657_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n454_lit_integer_α
.Lx657_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        add              rsp, 80
                                                                                        jmp   n454_lit_integer_α
#=======================================================================================================================
#         r = ARRAY(13)
#-----------------------------------------------------------------------------------------------------------------------
n454_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx658_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n455_call_α
.Lx658_0:
                        .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n455_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd660:            .string          "ARRAY"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd660]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 99
                                                                                        jne   .Lx659_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n457_var_α
.Lx659_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n456_assign_α
n455_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n457_var_α
#-----------------------------------------------------------------------------------------------------------------------
n456_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052352], rax
                        mov              qword ptr [1879052360], rdx
                        add              rsp, 32
                                                                                        jmp   n457_var_α
#=======================================================================================================================
#         r<1> = 'M'
#-----------------------------------------------------------------------------------------------------------------------
n457_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n458_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n458_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx663_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n459_subscript_α
.Lx663_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n459_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx664_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n462_var_α
.Lx664_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n460_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n460_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx665_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n461_assign_var_α
.Lx665_0:
                        .quad            .Lx665_0_s
.Lx665_0_s:
                        .string          "M"
#-----------------------------------------------------------------------------------------------------------------------
n461_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx666_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n462_var_α
.Lx666_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        add              rsp, 80
                                                                                        jmp   n462_var_α
#=======================================================================================================================
#         r<2> = 'CM'
#-----------------------------------------------------------------------------------------------------------------------
n462_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n463_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n463_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx668_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n464_subscript_α
.Lx668_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n464_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx669_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n467_var_α
.Lx669_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n465_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n465_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx670_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n466_assign_var_α
.Lx670_0:
                        .quad            .Lx670_0_s
.Lx670_0_s:
                        .string          "CM"
#-----------------------------------------------------------------------------------------------------------------------
n466_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx671_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n467_var_α
.Lx671_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        add              rsp, 80
                                                                                        jmp   n467_var_α
#=======================================================================================================================
#         r<3> = 'D'
#-----------------------------------------------------------------------------------------------------------------------
n467_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n468_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n468_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx673_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n469_subscript_α
.Lx673_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n469_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx674_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n472_var_α
.Lx674_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n470_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n470_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx675_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n471_assign_var_α
.Lx675_0:
                        .quad            .Lx675_0_s
.Lx675_0_s:
                        .string          "D"
#-----------------------------------------------------------------------------------------------------------------------
n471_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx676_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n472_var_α
.Lx676_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        add              rsp, 80
                                                                                        jmp   n472_var_α
#=======================================================================================================================
#         r<4> = 'CD'
#-----------------------------------------------------------------------------------------------------------------------
n472_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n473_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n473_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx678_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n474_subscript_α
.Lx678_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n474_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx679_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n477_var_α
.Lx679_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n475_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n475_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx680_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n476_assign_var_α
.Lx680_0:
                        .quad            .Lx680_0_s
.Lx680_0_s:
                        .string          "CD"
#-----------------------------------------------------------------------------------------------------------------------
n476_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx681_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n477_var_α
.Lx681_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        add              rsp, 80
                                                                                        jmp   n477_var_α
#=======================================================================================================================
#         r<5> = 'C'
#-----------------------------------------------------------------------------------------------------------------------
n477_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n478_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n478_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx683_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n479_subscript_α
.Lx683_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n479_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx684_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n482_var_α
.Lx684_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n480_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n480_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx685_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n481_assign_var_α
.Lx685_0:
                        .quad            .Lx685_0_s
.Lx685_0_s:
                        .string          "C"
#-----------------------------------------------------------------------------------------------------------------------
n481_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx686_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n482_var_α
.Lx686_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        add              rsp, 80
                                                                                        jmp   n482_var_α
#=======================================================================================================================
#         r<6> = 'XC'
#-----------------------------------------------------------------------------------------------------------------------
n482_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n483_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n483_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx688_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n484_subscript_α
.Lx688_0:
                        .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n484_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx689_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n487_var_α
.Lx689_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n485_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n485_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx690_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n486_assign_var_α
.Lx690_0:
                        .quad            .Lx690_0_s
.Lx690_0_s:
                        .string          "XC"
#-----------------------------------------------------------------------------------------------------------------------
n486_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx691_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n487_var_α
.Lx691_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        add              rsp, 80
                                                                                        jmp   n487_var_α
#=======================================================================================================================
#         r<7> = 'L'
#-----------------------------------------------------------------------------------------------------------------------
n487_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n488_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n488_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx693_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n489_subscript_α
.Lx693_0:
                        .quad            7
#-----------------------------------------------------------------------------------------------------------------------
n489_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx694_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n492_var_α
.Lx694_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n490_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n490_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx695_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n491_assign_var_α
.Lx695_0:
                        .quad            .Lx695_0_s
.Lx695_0_s:
                        .string          "L"
#-----------------------------------------------------------------------------------------------------------------------
n491_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx696_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n492_var_α
.Lx696_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        add              rsp, 80
                                                                                        jmp   n492_var_α
#=======================================================================================================================
#         r<8> = 'XL'
#-----------------------------------------------------------------------------------------------------------------------
n492_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n493_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n493_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx698_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n494_subscript_α
.Lx698_0:
                        .quad            8
#-----------------------------------------------------------------------------------------------------------------------
n494_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx699_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n497_var_α
.Lx699_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n495_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n495_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx700_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n496_assign_var_α
.Lx700_0:
                        .quad            .Lx700_0_s
.Lx700_0_s:
                        .string          "XL"
#-----------------------------------------------------------------------------------------------------------------------
n496_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx701_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n497_var_α
.Lx701_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        add              rsp, 80
                                                                                        jmp   n497_var_α
#=======================================================================================================================
#         r<9> = 'X'
#-----------------------------------------------------------------------------------------------------------------------
n497_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n498_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n498_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx703_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n499_subscript_α
.Lx703_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n499_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx704_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n502_var_α
.Lx704_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n500_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n500_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx705_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n501_assign_var_α
.Lx705_0:
                        .quad            .Lx705_0_s
.Lx705_0_s:
                        .string          "X"
#-----------------------------------------------------------------------------------------------------------------------
n501_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx706_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n502_var_α
.Lx706_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        add              rsp, 80
                                                                                        jmp   n502_var_α
#=======================================================================================================================
#         r<10> = 'IX'
#-----------------------------------------------------------------------------------------------------------------------
n502_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n503_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n503_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx708_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n504_subscript_α
.Lx708_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n504_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx709_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n507_var_α
.Lx709_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n505_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n505_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx710_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n506_assign_var_α
.Lx710_0:
                        .quad            .Lx710_0_s
.Lx710_0_s:
                        .string          "IX"
#-----------------------------------------------------------------------------------------------------------------------
n506_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx711_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n507_var_α
.Lx711_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        add              rsp, 80
                                                                                        jmp   n507_var_α
#=======================================================================================================================
#         r<11> = 'V'
#-----------------------------------------------------------------------------------------------------------------------
n507_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n508_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n508_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx713_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n509_subscript_α
.Lx713_0:
                        .quad            11
#-----------------------------------------------------------------------------------------------------------------------
n509_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx714_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n512_var_α
.Lx714_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n510_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n510_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx715_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n511_assign_var_α
.Lx715_0:
                        .quad            .Lx715_0_s
.Lx715_0_s:
                        .string          "V"
#-----------------------------------------------------------------------------------------------------------------------
n511_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx716_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n512_var_α
.Lx716_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        add              rsp, 80
                                                                                        jmp   n512_var_α
#=======================================================================================================================
#         r<12> = 'IV'
#-----------------------------------------------------------------------------------------------------------------------
n512_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n513_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n513_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx718_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n514_subscript_α
.Lx718_0:
                        .quad            12
#-----------------------------------------------------------------------------------------------------------------------
n514_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx719_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n517_var_α
.Lx719_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n515_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n515_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx720_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n516_assign_var_α
.Lx720_0:
                        .quad            .Lx720_0_s
.Lx720_0_s:
                        .string          "IV"
#-----------------------------------------------------------------------------------------------------------------------
n516_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx721_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n517_var_α
.Lx721_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        add              rsp, 80
                                                                                        jmp   n517_var_α
#=======================================================================================================================
#         r<13> = 'I'
#-----------------------------------------------------------------------------------------------------------------------
n517_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n518_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n518_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx723_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n519_subscript_α
.Lx723_0:
                        .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n519_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx724_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n522_lit_integer_α
.Lx724_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n520_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n520_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx725_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n521_assign_var_α
.Lx725_0:
                        .quad            .Lx725_0_s
.Lx725_0_s:
                        .string          "I"
#-----------------------------------------------------------------------------------------------------------------------
n521_assign_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx726_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n522_lit_integer_α
.Lx726_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        add              rsp, 80
                                                                                        jmp   n522_lit_integer_α
#=======================================================================================================================
#         i = 1
#-----------------------------------------------------------------------------------------------------------------------
n522_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx727_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n523_assign_α
.Lx727_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n523_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052368], rax
                        mov              qword ptr [1879052376], rdx
                        add              rsp, 16
                                                                                        jmp   n524_var_α
#=======================================================================================================================
# RLOOP   GT(n, 0)                                                       :F(RDONE)
#-----------------------------------------------------------------------------------------------------------------------
n524_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n525_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n525_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx730_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n526_coerce_numeric_α
.Lx730_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n526_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]
                        cmp              eax, 7
                                                                                        je    .Lx732_1
                        cmp              eax, 6
                                                                                        jne   .Lx732_0
                        mov              eax, dword ptr [rsp + 16]
                        cmp              eax, 6
                                                                                        jne   .Lx732_0
.Lx732_1:
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n527_coerce_numeric_α
.Lx732_0:
                        lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        lea              rdx, [rsp + 0]
                        mov              rcx, 111
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n527_coerce_numeric_α
#-----------------------------------------------------------------------------------------------------------------------
n527_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]
                        cmp              eax, 7
                                                                                        je    .Lx734_1
                        cmp              eax, 6
                                                                                        jne   .Lx734_0
                        mov              eax, dword ptr [rsp + 48]
                        cmp              eax, 6
                                                                                        jne   .Lx734_0
.Lx734_1:
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n528_cmp_test_α
.Lx734_0:
                        lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]
                        lea              rdx, [rsp + 0]
                        mov              rcx, 112
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n528_cmp_test_α
#-----------------------------------------------------------------------------------------------------------------------
n528_cmp_test_α:
                        sub              rsp, 16
                        lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jg    .Lx736_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n555_var_α
.Lx736_240:
                        add              rsp, 80
                                                                                        jmp   n529_var_α
#=======================================================================================================================
#         GE(n, v<i>)                                                    :F(RNEXT)
#-----------------------------------------------------------------------------------------------------------------------
n529_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n530_var_α
#-----------------------------------------------------------------------------------------------------------------------
n530_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n531_var_α
#-----------------------------------------------------------------------------------------------------------------------
n531_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n532_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n532_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx740_240
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n551_var_α
.Lx740_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n533_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n533_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]
                        mov              rsi, qword ptr [rsp + 24]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx741_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n551_var_α
.Lx741_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n534_coerce_numeric_α
#-----------------------------------------------------------------------------------------------------------------------
n534_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 80]
                        cmp              eax, 7
                                                                                        je    .Lx743_1
                        cmp              eax, 6
                                                                                        jne   .Lx743_0
                        mov              eax, dword ptr [rsp + 16]
                        cmp              eax, 6
                                                                                        jne   .Lx743_0
.Lx743_1:
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n535_coerce_numeric_α
.Lx743_0:
                        lea              rdi, [rsp + 80]
                        lea              rsi, [rsp + 16]
                        lea              rdx, [rsp + 0]
                        mov              rcx, 109
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n535_coerce_numeric_α
#-----------------------------------------------------------------------------------------------------------------------
n535_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]
                        cmp              eax, 7
                                                                                        je    .Lx745_1
                        cmp              eax, 6
                                                                                        jne   .Lx745_0
                        mov              eax, dword ptr [rsp + 96]
                        cmp              eax, 6
                                                                                        jne   .Lx745_0
.Lx745_1:
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n536_cmp_test_α
.Lx745_0:
                        lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 96]
                        lea              rdx, [rsp + 0]
                        mov              rcx, 110
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n536_cmp_test_α
#-----------------------------------------------------------------------------------------------------------------------
n536_cmp_test_α:
                        sub              rsp, 16
                        lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jns   .Lx747_240
                        add              rsp, 16
                        add              rsp, 112
                                                                                        jmp   n551_var_α
.Lx747_240:
                        add              rsp, 128
                                                                                        jmp   n537_var_α
#=======================================================================================================================
#         s = s r<i>
#-----------------------------------------------------------------------------------------------------------------------
n537_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n538_var_α
#-----------------------------------------------------------------------------------------------------------------------
n538_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n539_var_α
#-----------------------------------------------------------------------------------------------------------------------
n539_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n540_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n540_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx751_240
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n544_var_α
.Lx751_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n541_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n541_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]
                        mov              rsi, qword ptr [rsp + 24]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx752_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n544_var_α
.Lx752_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n542_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n542_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 80]
                        mov              rsi, qword ptr [rsp + 88]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n543_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n543_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rdx
                        add              rsp, 96
                                                                                        jmp   n544_var_α
#=======================================================================================================================
#         n = n - v<i>                                                   :(RLOOP)
#-----------------------------------------------------------------------------------------------------------------------
n544_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n545_var_α
#-----------------------------------------------------------------------------------------------------------------------
n545_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n546_var_α
#-----------------------------------------------------------------------------------------------------------------------
n546_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n547_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n547_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx758_240
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n524_var_α
.Lx758_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n548_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n548_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]
                        mov              rsi, qword ptr [rsp + 24]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx759_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n524_var_α
.Lx759_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n549_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n549_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 80]
                        mov              rsi, qword ptr [rsp + 88]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_sub@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx760_240
                        add              rsp, 16
                        add              rsp, 80
                                                                                        jmp   n524_var_α
.Lx760_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n550_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n550_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        add              rsp, 96
                                                                                        jmp   n524_var_α
#=======================================================================================================================
# RNEXT   i = i + 1                                                      :(RLOOP)
#-----------------------------------------------------------------------------------------------------------------------
n551_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n552_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n552_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx763_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n553_binop_α
.Lx763_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n553_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx764_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n524_var_α
.Lx764_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n554_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n554_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052368], rax
                        mov              qword ptr [1879052376], rdx
                        add              rsp, 48
                                                                                        jmp   n524_var_α
#=======================================================================================================================
# RDONE   roman = s                                                      :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n555_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n556_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n556_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n557_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n557_save_restore_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n558_goto_α:
                                                                                        jmp   n524_var_α
n558_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n559_goto_α:
                                                                                        jmp   n551_var_α
n559_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n560_goto_α:
                                                                                        jmp   n555_var_α
n560_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n561_goto_α:
                                                                                        jmp   n365_lit_integer_α
n561_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        add              rsp, 3096
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              eax, 99
                        xor              edx, edx
                        add              rsp, 3096
                        ret
                        .section         .note.GNU-stack,"",@progbits

                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_LBL__roman_α
proc_LBL__roman_α:
                        .global          proc_LBL__roman_α
                        .global          proc_LBL__roman_β
                        .global          proc_LBL__roman_γ
                        .global          proc_LBL__roman_ω
                        sub              rsp, 3120
                        mov              [rsp + 3096], rcx
                        mov              [rsp + 3104], rdx
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
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
                                                                                        jmp   n9_subscript_α
.Lx183_0:
                        .quad            1
#=======================================================================================================================
#         v<2> = 900
#-----------------------------------------------------------------------------------------------------------------------
n8_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 192], rax
                        mov              qword ptr [rsp + 200], rdx
                                                                                        jmp   n10_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n9_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 112]
                        mov              rsi, qword ptr [rsp + 120]
                        mov              rdx, qword ptr [rsp + 128]
                        mov              rcx, qword ptr [rsp + 136]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx185_240
                        add              rsp, 16
                                                                                        jmp   n8_var_α
.Lx185_240:
                        mov              qword ptr [rsp + 144], rax
                        mov              qword ptr [rsp + 152], rdx
                        add              rsp, 16
                                                                                        jmp   n12_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n10_lit_integer_α:
                        mov              qword ptr [rsp + 208], 6
                        mov              rax, qword ptr [rip + .Lx186_0]
                        mov              qword ptr [rsp + 216], rax
                                                                                        jmp   n13_subscript_α
.Lx186_0:
                        .quad            2
#=======================================================================================================================
#         v<3> = 500
#-----------------------------------------------------------------------------------------------------------------------
n11_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 272], rax
                        mov              qword ptr [rsp + 280], rdx
                                                                                        jmp   n14_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n12_lit_integer_α:
                        mov              qword ptr [rsp + 160], 6
                        mov              rax, qword ptr [rip + .Lx188_0]
                        mov              qword ptr [rsp + 168], rax
                                                                                        jmp   n16_assign_var_α
.Lx188_0:
                        .quad            1000
#-----------------------------------------------------------------------------------------------------------------------
n13_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 192]
                        mov              rsi, qword ptr [rsp + 200]
                        mov              rdx, qword ptr [rsp + 208]
                        mov              rcx, qword ptr [rsp + 216]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx189_240
                        add              rsp, 16
                                                                                        jmp   n11_var_α
.Lx189_240:
                        mov              qword ptr [rsp + 224], rax
                        mov              qword ptr [rsp + 232], rdx
                        add              rsp, 16
                                                                                        jmp   n17_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n14_lit_integer_α:
                        mov              qword ptr [rsp + 288], 6
                        mov              rax, qword ptr [rip + .Lx190_0]
                        mov              qword ptr [rsp + 296], rax
                                                                                        jmp   n18_subscript_α
.Lx190_0:
                        .quad            3
#=======================================================================================================================
#         v<4> = 400
#-----------------------------------------------------------------------------------------------------------------------
n15_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 352], rax
                        mov              qword ptr [rsp + 360], rdx
                                                                                        jmp   n19_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n16_assign_var_α:
                        mov              rdi, qword ptr [rsp + 144]
                        mov              rsi, qword ptr [rsp + 152]
                        mov              rdx, qword ptr [rsp + 160]
                        mov              rcx, qword ptr [rsp + 168]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n8_var_α
                        mov              qword ptr [rsp + 176], rax
                        mov              qword ptr [rsp + 184], rdx
                                                                                        jmp   n8_var_α
#-----------------------------------------------------------------------------------------------------------------------
n17_lit_integer_α:
                        mov              qword ptr [rsp + 240], 6
                        mov              rax, qword ptr [rip + .Lx193_0]
                        mov              qword ptr [rsp + 248], rax
                                                                                        jmp   n21_assign_var_α
.Lx193_0:
                        .quad            900
#-----------------------------------------------------------------------------------------------------------------------
n18_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 272]
                        mov              rsi, qword ptr [rsp + 280]
                        mov              rdx, qword ptr [rsp + 288]
                        mov              rcx, qword ptr [rsp + 296]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx194_240
                        add              rsp, 16
                                                                                        jmp   n15_var_α
.Lx194_240:
                        mov              qword ptr [rsp + 304], rax
                        mov              qword ptr [rsp + 312], rdx
                        add              rsp, 16
                                                                                        jmp   n22_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n19_lit_integer_α:
                        mov              qword ptr [rsp + 368], 6
                        mov              rax, qword ptr [rip + .Lx195_0]
                        mov              qword ptr [rsp + 376], rax
                                                                                        jmp   n23_subscript_α
.Lx195_0:
                        .quad            4
#=======================================================================================================================
#         v<5> = 100
#-----------------------------------------------------------------------------------------------------------------------
n20_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 432], rax
                        mov              qword ptr [rsp + 440], rdx
                                                                                        jmp   n24_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n21_assign_var_α:
                        mov              rdi, qword ptr [rsp + 224]
                        mov              rsi, qword ptr [rsp + 232]
                        mov              rdx, qword ptr [rsp + 240]
                        mov              rcx, qword ptr [rsp + 248]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n11_var_α
                        mov              qword ptr [rsp + 256], rax
                        mov              qword ptr [rsp + 264], rdx
                                                                                        jmp   n11_var_α
#-----------------------------------------------------------------------------------------------------------------------
n22_lit_integer_α:
                        mov              qword ptr [rsp + 320], 6
                        mov              rax, qword ptr [rip + .Lx198_0]
                        mov              qword ptr [rsp + 328], rax
                                                                                        jmp   n26_assign_var_α
.Lx198_0:
                        .quad            500
#-----------------------------------------------------------------------------------------------------------------------
n23_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 352]
                        mov              rsi, qword ptr [rsp + 360]
                        mov              rdx, qword ptr [rsp + 368]
                        mov              rcx, qword ptr [rsp + 376]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx199_240
                        add              rsp, 16
                                                                                        jmp   n20_var_α
.Lx199_240:
                        mov              qword ptr [rsp + 384], rax
                        mov              qword ptr [rsp + 392], rdx
                        add              rsp, 16
                                                                                        jmp   n27_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n24_lit_integer_α:
                        mov              qword ptr [rsp + 448], 6
                        mov              rax, qword ptr [rip + .Lx200_0]
                        mov              qword ptr [rsp + 456], rax
                                                                                        jmp   n28_subscript_α
.Lx200_0:
                        .quad            5
#=======================================================================================================================
#         v<6> = 90
#-----------------------------------------------------------------------------------------------------------------------
n25_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 512], rax
                        mov              qword ptr [rsp + 520], rdx
                                                                                        jmp   n29_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n26_assign_var_α:
                        mov              rdi, qword ptr [rsp + 304]
                        mov              rsi, qword ptr [rsp + 312]
                        mov              rdx, qword ptr [rsp + 320]
                        mov              rcx, qword ptr [rsp + 328]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n15_var_α
                        mov              qword ptr [rsp + 336], rax
                        mov              qword ptr [rsp + 344], rdx
                                                                                        jmp   n15_var_α
#-----------------------------------------------------------------------------------------------------------------------
n27_lit_integer_α:
                        mov              qword ptr [rsp + 400], 6
                        mov              rax, qword ptr [rip + .Lx203_0]
                        mov              qword ptr [rsp + 408], rax
                                                                                        jmp   n31_assign_var_α
.Lx203_0:
                        .quad            400
#-----------------------------------------------------------------------------------------------------------------------
n28_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 432]
                        mov              rsi, qword ptr [rsp + 440]
                        mov              rdx, qword ptr [rsp + 448]
                        mov              rcx, qword ptr [rsp + 456]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx204_240
                        add              rsp, 16
                                                                                        jmp   n25_var_α
.Lx204_240:
                        mov              qword ptr [rsp + 464], rax
                        mov              qword ptr [rsp + 472], rdx
                        add              rsp, 16
                                                                                        jmp   n32_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n29_lit_integer_α:
                        mov              qword ptr [rsp + 528], 6
                        mov              rax, qword ptr [rip + .Lx205_0]
                        mov              qword ptr [rsp + 536], rax
                                                                                        jmp   n33_subscript_α
.Lx205_0:
                        .quad            6
#=======================================================================================================================
#         v<7> = 50
#-----------------------------------------------------------------------------------------------------------------------
n30_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 592], rax
                        mov              qword ptr [rsp + 600], rdx
                                                                                        jmp   n34_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n31_assign_var_α:
                        mov              rdi, qword ptr [rsp + 384]
                        mov              rsi, qword ptr [rsp + 392]
                        mov              rdx, qword ptr [rsp + 400]
                        mov              rcx, qword ptr [rsp + 408]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n20_var_α
                        mov              qword ptr [rsp + 416], rax
                        mov              qword ptr [rsp + 424], rdx
                                                                                        jmp   n20_var_α
#-----------------------------------------------------------------------------------------------------------------------
n32_lit_integer_α:
                        mov              qword ptr [rsp + 480], 6
                        mov              rax, qword ptr [rip + .Lx208_0]
                        mov              qword ptr [rsp + 488], rax
                                                                                        jmp   n36_assign_var_α
.Lx208_0:
                        .quad            100
#-----------------------------------------------------------------------------------------------------------------------
n33_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 512]
                        mov              rsi, qword ptr [rsp + 520]
                        mov              rdx, qword ptr [rsp + 528]
                        mov              rcx, qword ptr [rsp + 536]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx209_240
                        add              rsp, 16
                                                                                        jmp   n30_var_α
.Lx209_240:
                        mov              qword ptr [rsp + 544], rax
                        mov              qword ptr [rsp + 552], rdx
                        add              rsp, 16
                                                                                        jmp   n37_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n34_lit_integer_α:
                        mov              qword ptr [rsp + 608], 6
                        mov              rax, qword ptr [rip + .Lx210_0]
                        mov              qword ptr [rsp + 616], rax
                                                                                        jmp   n38_subscript_α
.Lx210_0:
                        .quad            7
#=======================================================================================================================
#         v<8> = 40
#-----------------------------------------------------------------------------------------------------------------------
n35_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 672], rax
                        mov              qword ptr [rsp + 680], rdx
                                                                                        jmp   n39_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n36_assign_var_α:
                        mov              rdi, qword ptr [rsp + 464]
                        mov              rsi, qword ptr [rsp + 472]
                        mov              rdx, qword ptr [rsp + 480]
                        mov              rcx, qword ptr [rsp + 488]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n25_var_α
                        mov              qword ptr [rsp + 496], rax
                        mov              qword ptr [rsp + 504], rdx
                                                                                        jmp   n25_var_α
#-----------------------------------------------------------------------------------------------------------------------
n37_lit_integer_α:
                        mov              qword ptr [rsp + 560], 6
                        mov              rax, qword ptr [rip + .Lx213_0]
                        mov              qword ptr [rsp + 568], rax
                                                                                        jmp   n41_assign_var_α
.Lx213_0:
                        .quad            90
#-----------------------------------------------------------------------------------------------------------------------
n38_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 592]
                        mov              rsi, qword ptr [rsp + 600]
                        mov              rdx, qword ptr [rsp + 608]
                        mov              rcx, qword ptr [rsp + 616]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx214_240
                        add              rsp, 16
                                                                                        jmp   n35_var_α
.Lx214_240:
                        mov              qword ptr [rsp + 624], rax
                        mov              qword ptr [rsp + 632], rdx
                        add              rsp, 16
                                                                                        jmp   n42_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n39_lit_integer_α:
                        mov              qword ptr [rsp + 688], 6
                        mov              rax, qword ptr [rip + .Lx215_0]
                        mov              qword ptr [rsp + 696], rax
                                                                                        jmp   n43_subscript_α
.Lx215_0:
                        .quad            8
#=======================================================================================================================
#         v<9> = 10
#-----------------------------------------------------------------------------------------------------------------------
n40_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 752], rax
                        mov              qword ptr [rsp + 760], rdx
                                                                                        jmp   n44_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n41_assign_var_α:
                        mov              rdi, qword ptr [rsp + 544]
                        mov              rsi, qword ptr [rsp + 552]
                        mov              rdx, qword ptr [rsp + 560]
                        mov              rcx, qword ptr [rsp + 568]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n30_var_α
                        mov              qword ptr [rsp + 576], rax
                        mov              qword ptr [rsp + 584], rdx
                                                                                        jmp   n30_var_α
#-----------------------------------------------------------------------------------------------------------------------
n42_lit_integer_α:
                        mov              qword ptr [rsp + 640], 6
                        mov              rax, qword ptr [rip + .Lx218_0]
                        mov              qword ptr [rsp + 648], rax
                                                                                        jmp   n46_assign_var_α
.Lx218_0:
                        .quad            50
#-----------------------------------------------------------------------------------------------------------------------
n43_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 672]
                        mov              rsi, qword ptr [rsp + 680]
                        mov              rdx, qword ptr [rsp + 688]
                        mov              rcx, qword ptr [rsp + 696]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx219_240
                        add              rsp, 16
                                                                                        jmp   n40_var_α
.Lx219_240:
                        mov              qword ptr [rsp + 704], rax
                        mov              qword ptr [rsp + 712], rdx
                        add              rsp, 16
                                                                                        jmp   n47_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n44_lit_integer_α:
                        mov              qword ptr [rsp + 768], 6
                        mov              rax, qword ptr [rip + .Lx220_0]
                        mov              qword ptr [rsp + 776], rax
                                                                                        jmp   n48_subscript_α
.Lx220_0:
                        .quad            9
#=======================================================================================================================
#         v<10> = 9
#-----------------------------------------------------------------------------------------------------------------------
n45_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 832], rax
                        mov              qword ptr [rsp + 840], rdx
                                                                                        jmp   n49_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n46_assign_var_α:
                        mov              rdi, qword ptr [rsp + 624]
                        mov              rsi, qword ptr [rsp + 632]
                        mov              rdx, qword ptr [rsp + 640]
                        mov              rcx, qword ptr [rsp + 648]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n35_var_α
                        mov              qword ptr [rsp + 656], rax
                        mov              qword ptr [rsp + 664], rdx
                                                                                        jmp   n35_var_α
#-----------------------------------------------------------------------------------------------------------------------
n47_lit_integer_α:
                        mov              qword ptr [rsp + 720], 6
                        mov              rax, qword ptr [rip + .Lx223_0]
                        mov              qword ptr [rsp + 728], rax
                                                                                        jmp   n51_assign_var_α
.Lx223_0:
                        .quad            40
#-----------------------------------------------------------------------------------------------------------------------
n48_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 752]
                        mov              rsi, qword ptr [rsp + 760]
                        mov              rdx, qword ptr [rsp + 768]
                        mov              rcx, qword ptr [rsp + 776]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx224_240
                        add              rsp, 16
                                                                                        jmp   n45_var_α
.Lx224_240:
                        mov              qword ptr [rsp + 784], rax
                        mov              qword ptr [rsp + 792], rdx
                        add              rsp, 16
                                                                                        jmp   n52_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n49_lit_integer_α:
                        mov              qword ptr [rsp + 848], 6
                        mov              rax, qword ptr [rip + .Lx225_0]
                        mov              qword ptr [rsp + 856], rax
                                                                                        jmp   n53_subscript_α
.Lx225_0:
                        .quad            10
#=======================================================================================================================
#         v<11> = 5
#-----------------------------------------------------------------------------------------------------------------------
n50_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 912], rax
                        mov              qword ptr [rsp + 920], rdx
                                                                                        jmp   n54_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n51_assign_var_α:
                        mov              rdi, qword ptr [rsp + 704]
                        mov              rsi, qword ptr [rsp + 712]
                        mov              rdx, qword ptr [rsp + 720]
                        mov              rcx, qword ptr [rsp + 728]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n40_var_α
                        mov              qword ptr [rsp + 736], rax
                        mov              qword ptr [rsp + 744], rdx
                                                                                        jmp   n40_var_α
#-----------------------------------------------------------------------------------------------------------------------
n52_lit_integer_α:
                        mov              qword ptr [rsp + 800], 6
                        mov              rax, qword ptr [rip + .Lx228_0]
                        mov              qword ptr [rsp + 808], rax
                                                                                        jmp   n56_assign_var_α
.Lx228_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n53_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 832]
                        mov              rsi, qword ptr [rsp + 840]
                        mov              rdx, qword ptr [rsp + 848]
                        mov              rcx, qword ptr [rsp + 856]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx229_240
                        add              rsp, 16
                                                                                        jmp   n50_var_α
.Lx229_240:
                        mov              qword ptr [rsp + 864], rax
                        mov              qword ptr [rsp + 872], rdx
                        add              rsp, 16
                                                                                        jmp   n57_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n54_lit_integer_α:
                        mov              qword ptr [rsp + 928], 6
                        mov              rax, qword ptr [rip + .Lx230_0]
                        mov              qword ptr [rsp + 936], rax
                                                                                        jmp   n58_subscript_α
.Lx230_0:
                        .quad            11
#=======================================================================================================================
#         v<12> = 4
#-----------------------------------------------------------------------------------------------------------------------
n55_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 992], rax
                        mov              qword ptr [rsp + 1000], rdx
                                                                                        jmp   n59_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n56_assign_var_α:
                        mov              rdi, qword ptr [rsp + 784]
                        mov              rsi, qword ptr [rsp + 792]
                        mov              rdx, qword ptr [rsp + 800]
                        mov              rcx, qword ptr [rsp + 808]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n45_var_α
                        mov              qword ptr [rsp + 816], rax
                        mov              qword ptr [rsp + 824], rdx
                                                                                        jmp   n45_var_α
#-----------------------------------------------------------------------------------------------------------------------
n57_lit_integer_α:
                        mov              qword ptr [rsp + 880], 6
                        mov              rax, qword ptr [rip + .Lx233_0]
                        mov              qword ptr [rsp + 888], rax
                                                                                        jmp   n61_assign_var_α
.Lx233_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n58_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 912]
                        mov              rsi, qword ptr [rsp + 920]
                        mov              rdx, qword ptr [rsp + 928]
                        mov              rcx, qword ptr [rsp + 936]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx234_240
                        add              rsp, 16
                                                                                        jmp   n55_var_α
.Lx234_240:
                        mov              qword ptr [rsp + 944], rax
                        mov              qword ptr [rsp + 952], rdx
                        add              rsp, 16
                                                                                        jmp   n62_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n59_lit_integer_α:
                        mov              qword ptr [rsp + 1008], 6
                        mov              rax, qword ptr [rip + .Lx235_0]
                        mov              qword ptr [rsp + 1016], rax
                                                                                        jmp   n63_subscript_α
.Lx235_0:
                        .quad            12
#=======================================================================================================================
#         v<13> = 1
#-----------------------------------------------------------------------------------------------------------------------
n60_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 1072], rax
                        mov              qword ptr [rsp + 1080], rdx
                                                                                        jmp   n64_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n61_assign_var_α:
                        mov              rdi, qword ptr [rsp + 864]
                        mov              rsi, qword ptr [rsp + 872]
                        mov              rdx, qword ptr [rsp + 880]
                        mov              rcx, qword ptr [rsp + 888]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n50_var_α
                        mov              qword ptr [rsp + 896], rax
                        mov              qword ptr [rsp + 904], rdx
                                                                                        jmp   n50_var_α
#-----------------------------------------------------------------------------------------------------------------------
n62_lit_integer_α:
                        mov              qword ptr [rsp + 960], 6
                        mov              rax, qword ptr [rip + .Lx238_0]
                        mov              qword ptr [rsp + 968], rax
                                                                                        jmp   n66_assign_var_α
.Lx238_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n63_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 992]
                        mov              rsi, qword ptr [rsp + 1000]
                        mov              rdx, qword ptr [rsp + 1008]
                        mov              rcx, qword ptr [rsp + 1016]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx239_240
                        add              rsp, 16
                                                                                        jmp   n60_var_α
.Lx239_240:
                        mov              qword ptr [rsp + 1024], rax
                        mov              qword ptr [rsp + 1032], rdx
                        add              rsp, 16
                                                                                        jmp   n67_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n64_lit_integer_α:
                        mov              qword ptr [rsp + 1088], 6
                        mov              rax, qword ptr [rip + .Lx240_0]
                        mov              qword ptr [rsp + 1096], rax
                                                                                        jmp   n68_subscript_α
.Lx240_0:
                        .quad            13
#=======================================================================================================================
#         r = ARRAY(13)
#-----------------------------------------------------------------------------------------------------------------------
n65_lit_integer_α:
                        mov              qword ptr [rsp + 1200], 6
                        mov              rax, qword ptr [rip + .Lx241_0]
                        mov              qword ptr [rsp + 1208], rax
                                                                                        jmp   n69_call_α
.Lx241_0:
                        .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n66_assign_var_α:
                        mov              rdi, qword ptr [rsp + 944]
                        mov              rsi, qword ptr [rsp + 952]
                        mov              rdx, qword ptr [rsp + 960]
                        mov              rcx, qword ptr [rsp + 968]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n55_var_α
                        mov              qword ptr [rsp + 976], rax
                        mov              qword ptr [rsp + 984], rdx
                                                                                        jmp   n55_var_α
#-----------------------------------------------------------------------------------------------------------------------
n67_lit_integer_α:
                        mov              qword ptr [rsp + 1040], 6
                        mov              rax, qword ptr [rip + .Lx243_0]
                        mov              qword ptr [rsp + 1048], rax
                                                                                        jmp   n70_assign_var_α
.Lx243_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n68_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1072]
                        mov              rsi, qword ptr [rsp + 1080]
                        mov              rdx, qword ptr [rsp + 1088]
                        mov              rcx, qword ptr [rsp + 1096]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx244_240
                        add              rsp, 16
                                                                                        jmp   n65_lit_integer_α
.Lx244_240:
                        mov              qword ptr [rsp + 1104], rax
                        mov              qword ptr [rsp + 1112], rdx
                        add              rsp, 16
                                                                                        jmp   n71_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n69_call_α:
                        mov              rax, qword ptr [rsp + 1200]
                        mov              qword ptr [rsp + 1168], rax
                        mov              rax, qword ptr [rsp + 1208]
                        mov              qword ptr [rsp + 1176], rax
                        .section         .rodata
.Lrkfn246:              .string          "ARRAY"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn246]
                        lea              rsi, [rsp + 1168]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1152], rax
                        mov              qword ptr [rsp + 1160], rdx
                        cmp              eax, 99
                                                                                        je    n73_var_α
                                                                                        jmp   n72_assign_α
n69_call_β:
                                                                                        jmp   n73_var_α
#-----------------------------------------------------------------------------------------------------------------------
n70_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1024]
                        mov              rsi, qword ptr [rsp + 1032]
                        mov              rdx, qword ptr [rsp + 1040]
                        mov              rcx, qword ptr [rsp + 1048]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n60_var_α
                        mov              qword ptr [rsp + 1056], rax
                        mov              qword ptr [rsp + 1064], rdx
                                                                                        jmp   n60_var_α
#-----------------------------------------------------------------------------------------------------------------------
n71_lit_integer_α:
                        mov              qword ptr [rsp + 1120], 6
                        mov              rax, qword ptr [rip + .Lx248_0]
                        mov              qword ptr [rsp + 1128], rax
                                                                                        jmp   n74_assign_var_α
.Lx248_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n72_assign_α:
                        mov              rax, qword ptr [rsp + 1152]
                        mov              rdx, qword ptr [rsp + 1160]
                        mov              qword ptr [1879052352], rax
                        mov              qword ptr [1879052360], rdx
                                                                                        jmp   n73_var_α
#=======================================================================================================================
#         r<1> = 'M'
#-----------------------------------------------------------------------------------------------------------------------
n73_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 1216], rax
                        mov              qword ptr [rsp + 1224], rdx
                                                                                        jmp   n75_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n74_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1104]
                        mov              rsi, qword ptr [rsp + 1112]
                        mov              rdx, qword ptr [rsp + 1120]
                        mov              rcx, qword ptr [rsp + 1128]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n65_lit_integer_α
                        mov              qword ptr [rsp + 1136], rax
                        mov              qword ptr [rsp + 1144], rdx
                                                                                        jmp   n65_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n75_lit_integer_α:
                        mov              qword ptr [rsp + 1232], 6
                        mov              rax, qword ptr [rip + .Lx252_0]
                        mov              qword ptr [rsp + 1240], rax
                                                                                        jmp   n77_subscript_α
.Lx252_0:
                        .quad            1
#=======================================================================================================================
#         r<2> = 'CM'
#-----------------------------------------------------------------------------------------------------------------------
n76_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 1296], rax
                        mov              qword ptr [rsp + 1304], rdx
                                                                                        jmp   n78_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n77_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1216]
                        mov              rsi, qword ptr [rsp + 1224]
                        mov              rdx, qword ptr [rsp + 1232]
                        mov              rcx, qword ptr [rsp + 1240]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx254_240
                        add              rsp, 16
                                                                                        jmp   n76_var_α
.Lx254_240:
                        mov              qword ptr [rsp + 1248], rax
                        mov              qword ptr [rsp + 1256], rdx
                        add              rsp, 16
                                                                                        jmp   n80_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n78_lit_integer_α:
                        mov              qword ptr [rsp + 1312], 6
                        mov              rax, qword ptr [rip + .Lx255_0]
                        mov              qword ptr [rsp + 1320], rax
                                                                                        jmp   n81_subscript_α
.Lx255_0:
                        .quad            2
#=======================================================================================================================
#         r<3> = 'D'
#-----------------------------------------------------------------------------------------------------------------------
n79_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 1376], rax
                        mov              qword ptr [rsp + 1384], rdx
                                                                                        jmp   n82_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n80_lit_string_α:
                        mov              qword ptr [rsp + 1264], 1
                        mov              dword ptr [rsp + 1268], 1
                        mov              rax, qword ptr [rip + .Lx257_0]
                        mov              qword ptr [rsp + 1272], rax
                                                                                        jmp   n84_assign_var_α
.Lx257_0:
                        .quad            .Lx257_0_s
.Lx257_0_s:
                        .string          "M"
#-----------------------------------------------------------------------------------------------------------------------
n81_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1296]
                        mov              rsi, qword ptr [rsp + 1304]
                        mov              rdx, qword ptr [rsp + 1312]
                        mov              rcx, qword ptr [rsp + 1320]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx258_240
                        add              rsp, 16
                                                                                        jmp   n79_var_α
.Lx258_240:
                        mov              qword ptr [rsp + 1328], rax
                        mov              qword ptr [rsp + 1336], rdx
                        add              rsp, 16
                                                                                        jmp   n85_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n82_lit_integer_α:
                        mov              qword ptr [rsp + 1392], 6
                        mov              rax, qword ptr [rip + .Lx259_0]
                        mov              qword ptr [rsp + 1400], rax
                                                                                        jmp   n86_subscript_α
.Lx259_0:
                        .quad            3
#=======================================================================================================================
#         r<4> = 'CD'
#-----------------------------------------------------------------------------------------------------------------------
n83_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 1456], rax
                        mov              qword ptr [rsp + 1464], rdx
                                                                                        jmp   n87_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n84_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1248]
                        mov              rsi, qword ptr [rsp + 1256]
                        mov              rdx, qword ptr [rsp + 1264]
                        mov              rcx, qword ptr [rsp + 1272]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n76_var_α
                        mov              qword ptr [rsp + 1280], rax
                        mov              qword ptr [rsp + 1288], rdx
                                                                                        jmp   n76_var_α
#-----------------------------------------------------------------------------------------------------------------------
n85_lit_string_α:
                        mov              qword ptr [rsp + 1344], 1
                        mov              dword ptr [rsp + 1348], 2
                        mov              rax, qword ptr [rip + .Lx262_0]
                        mov              qword ptr [rsp + 1352], rax
                                                                                        jmp   n89_assign_var_α
.Lx262_0:
                        .quad            .Lx262_0_s
.Lx262_0_s:
                        .string          "CM"
#-----------------------------------------------------------------------------------------------------------------------
n86_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1376]
                        mov              rsi, qword ptr [rsp + 1384]
                        mov              rdx, qword ptr [rsp + 1392]
                        mov              rcx, qword ptr [rsp + 1400]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx263_240
                        add              rsp, 16
                                                                                        jmp   n83_var_α
.Lx263_240:
                        mov              qword ptr [rsp + 1408], rax
                        mov              qword ptr [rsp + 1416], rdx
                        add              rsp, 16
                                                                                        jmp   n90_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n87_lit_integer_α:
                        mov              qword ptr [rsp + 1472], 6
                        mov              rax, qword ptr [rip + .Lx264_0]
                        mov              qword ptr [rsp + 1480], rax
                                                                                        jmp   n91_subscript_α
.Lx264_0:
                        .quad            4
#=======================================================================================================================
#         r<5> = 'C'
#-----------------------------------------------------------------------------------------------------------------------
n88_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 1536], rax
                        mov              qword ptr [rsp + 1544], rdx
                                                                                        jmp   n92_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n89_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1328]
                        mov              rsi, qword ptr [rsp + 1336]
                        mov              rdx, qword ptr [rsp + 1344]
                        mov              rcx, qword ptr [rsp + 1352]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n79_var_α
                        mov              qword ptr [rsp + 1360], rax
                        mov              qword ptr [rsp + 1368], rdx
                                                                                        jmp   n79_var_α
#-----------------------------------------------------------------------------------------------------------------------
n90_lit_string_α:
                        mov              qword ptr [rsp + 1424], 1
                        mov              dword ptr [rsp + 1428], 1
                        mov              rax, qword ptr [rip + .Lx267_0]
                        mov              qword ptr [rsp + 1432], rax
                                                                                        jmp   n94_assign_var_α
.Lx267_0:
                        .quad            .Lx267_0_s
.Lx267_0_s:
                        .string          "D"
#-----------------------------------------------------------------------------------------------------------------------
n91_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1456]
                        mov              rsi, qword ptr [rsp + 1464]
                        mov              rdx, qword ptr [rsp + 1472]
                        mov              rcx, qword ptr [rsp + 1480]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx268_240
                        add              rsp, 16
                                                                                        jmp   n88_var_α
.Lx268_240:
                        mov              qword ptr [rsp + 1488], rax
                        mov              qword ptr [rsp + 1496], rdx
                        add              rsp, 16
                                                                                        jmp   n95_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n92_lit_integer_α:
                        mov              qword ptr [rsp + 1552], 6
                        mov              rax, qword ptr [rip + .Lx269_0]
                        mov              qword ptr [rsp + 1560], rax
                                                                                        jmp   n96_subscript_α
.Lx269_0:
                        .quad            5
#=======================================================================================================================
#         r<6> = 'XC'
#-----------------------------------------------------------------------------------------------------------------------
n93_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 1616], rax
                        mov              qword ptr [rsp + 1624], rdx
                                                                                        jmp   n97_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n94_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1408]
                        mov              rsi, qword ptr [rsp + 1416]
                        mov              rdx, qword ptr [rsp + 1424]
                        mov              rcx, qword ptr [rsp + 1432]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n83_var_α
                        mov              qword ptr [rsp + 1440], rax
                        mov              qword ptr [rsp + 1448], rdx
                                                                                        jmp   n83_var_α
#-----------------------------------------------------------------------------------------------------------------------
n95_lit_string_α:
                        mov              qword ptr [rsp + 1504], 1
                        mov              dword ptr [rsp + 1508], 2
                        mov              rax, qword ptr [rip + .Lx272_0]
                        mov              qword ptr [rsp + 1512], rax
                                                                                        jmp   n99_assign_var_α
.Lx272_0:
                        .quad            .Lx272_0_s
.Lx272_0_s:
                        .string          "CD"
#-----------------------------------------------------------------------------------------------------------------------
n96_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1536]
                        mov              rsi, qword ptr [rsp + 1544]
                        mov              rdx, qword ptr [rsp + 1552]
                        mov              rcx, qword ptr [rsp + 1560]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx273_240
                        add              rsp, 16
                                                                                        jmp   n93_var_α
.Lx273_240:
                        mov              qword ptr [rsp + 1568], rax
                        mov              qword ptr [rsp + 1576], rdx
                        add              rsp, 16
                                                                                        jmp   n100_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n97_lit_integer_α:
                        mov              qword ptr [rsp + 1632], 6
                        mov              rax, qword ptr [rip + .Lx274_0]
                        mov              qword ptr [rsp + 1640], rax
                                                                                        jmp   n101_subscript_α
.Lx274_0:
                        .quad            6
#=======================================================================================================================
#         r<7> = 'L'
#-----------------------------------------------------------------------------------------------------------------------
n98_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 1696], rax
                        mov              qword ptr [rsp + 1704], rdx
                                                                                        jmp   n102_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n99_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1488]
                        mov              rsi, qword ptr [rsp + 1496]
                        mov              rdx, qword ptr [rsp + 1504]
                        mov              rcx, qword ptr [rsp + 1512]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n88_var_α
                        mov              qword ptr [rsp + 1520], rax
                        mov              qword ptr [rsp + 1528], rdx
                                                                                        jmp   n88_var_α
#-----------------------------------------------------------------------------------------------------------------------
n100_lit_string_α:
                        mov              qword ptr [rsp + 1584], 1
                        mov              dword ptr [rsp + 1588], 1
                        mov              rax, qword ptr [rip + .Lx277_0]
                        mov              qword ptr [rsp + 1592], rax
                                                                                        jmp   n104_assign_var_α
.Lx277_0:
                        .quad            .Lx277_0_s
.Lx277_0_s:
                        .string          "C"
#-----------------------------------------------------------------------------------------------------------------------
n101_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1616]
                        mov              rsi, qword ptr [rsp + 1624]
                        mov              rdx, qword ptr [rsp + 1632]
                        mov              rcx, qword ptr [rsp + 1640]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx278_240
                        add              rsp, 16
                                                                                        jmp   n98_var_α
.Lx278_240:
                        mov              qword ptr [rsp + 1648], rax
                        mov              qword ptr [rsp + 1656], rdx
                        add              rsp, 16
                                                                                        jmp   n105_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n102_lit_integer_α:
                        mov              qword ptr [rsp + 1712], 6
                        mov              rax, qword ptr [rip + .Lx279_0]
                        mov              qword ptr [rsp + 1720], rax
                                                                                        jmp   n106_subscript_α
.Lx279_0:
                        .quad            7
#=======================================================================================================================
#         r<8> = 'XL'
#-----------------------------------------------------------------------------------------------------------------------
n103_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 1776], rax
                        mov              qword ptr [rsp + 1784], rdx
                                                                                        jmp   n107_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n104_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1568]
                        mov              rsi, qword ptr [rsp + 1576]
                        mov              rdx, qword ptr [rsp + 1584]
                        mov              rcx, qword ptr [rsp + 1592]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n93_var_α
                        mov              qword ptr [rsp + 1600], rax
                        mov              qword ptr [rsp + 1608], rdx
                                                                                        jmp   n93_var_α
#-----------------------------------------------------------------------------------------------------------------------
n105_lit_string_α:
                        mov              qword ptr [rsp + 1664], 1
                        mov              dword ptr [rsp + 1668], 2
                        mov              rax, qword ptr [rip + .Lx282_0]
                        mov              qword ptr [rsp + 1672], rax
                                                                                        jmp   n109_assign_var_α
.Lx282_0:
                        .quad            .Lx282_0_s
.Lx282_0_s:
                        .string          "XC"
#-----------------------------------------------------------------------------------------------------------------------
n106_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1696]
                        mov              rsi, qword ptr [rsp + 1704]
                        mov              rdx, qword ptr [rsp + 1712]
                        mov              rcx, qword ptr [rsp + 1720]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx283_240
                        add              rsp, 16
                                                                                        jmp   n103_var_α
.Lx283_240:
                        mov              qword ptr [rsp + 1728], rax
                        mov              qword ptr [rsp + 1736], rdx
                        add              rsp, 16
                                                                                        jmp   n110_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n107_lit_integer_α:
                        mov              qword ptr [rsp + 1792], 6
                        mov              rax, qword ptr [rip + .Lx284_0]
                        mov              qword ptr [rsp + 1800], rax
                                                                                        jmp   n111_subscript_α
.Lx284_0:
                        .quad            8
#=======================================================================================================================
#         r<9> = 'X'
#-----------------------------------------------------------------------------------------------------------------------
n108_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 1856], rax
                        mov              qword ptr [rsp + 1864], rdx
                                                                                        jmp   n112_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n109_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1648]
                        mov              rsi, qword ptr [rsp + 1656]
                        mov              rdx, qword ptr [rsp + 1664]
                        mov              rcx, qword ptr [rsp + 1672]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n98_var_α
                        mov              qword ptr [rsp + 1680], rax
                        mov              qword ptr [rsp + 1688], rdx
                                                                                        jmp   n98_var_α
#-----------------------------------------------------------------------------------------------------------------------
n110_lit_string_α:
                        mov              qword ptr [rsp + 1744], 1
                        mov              dword ptr [rsp + 1748], 1
                        mov              rax, qword ptr [rip + .Lx287_0]
                        mov              qword ptr [rsp + 1752], rax
                                                                                        jmp   n114_assign_var_α
.Lx287_0:
                        .quad            .Lx287_0_s
.Lx287_0_s:
                        .string          "L"
#-----------------------------------------------------------------------------------------------------------------------
n111_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1776]
                        mov              rsi, qword ptr [rsp + 1784]
                        mov              rdx, qword ptr [rsp + 1792]
                        mov              rcx, qword ptr [rsp + 1800]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx288_240
                        add              rsp, 16
                                                                                        jmp   n108_var_α
.Lx288_240:
                        mov              qword ptr [rsp + 1808], rax
                        mov              qword ptr [rsp + 1816], rdx
                        add              rsp, 16
                                                                                        jmp   n115_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n112_lit_integer_α:
                        mov              qword ptr [rsp + 1872], 6
                        mov              rax, qword ptr [rip + .Lx289_0]
                        mov              qword ptr [rsp + 1880], rax
                                                                                        jmp   n116_subscript_α
.Lx289_0:
                        .quad            9
#=======================================================================================================================
#         r<10> = 'IX'
#-----------------------------------------------------------------------------------------------------------------------
n113_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 1936], rax
                        mov              qword ptr [rsp + 1944], rdx
                                                                                        jmp   n117_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n114_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1728]
                        mov              rsi, qword ptr [rsp + 1736]
                        mov              rdx, qword ptr [rsp + 1744]
                        mov              rcx, qword ptr [rsp + 1752]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n103_var_α
                        mov              qword ptr [rsp + 1760], rax
                        mov              qword ptr [rsp + 1768], rdx
                                                                                        jmp   n103_var_α
#-----------------------------------------------------------------------------------------------------------------------
n115_lit_string_α:
                        mov              qword ptr [rsp + 1824], 1
                        mov              dword ptr [rsp + 1828], 2
                        mov              rax, qword ptr [rip + .Lx292_0]
                        mov              qword ptr [rsp + 1832], rax
                                                                                        jmp   n119_assign_var_α
.Lx292_0:
                        .quad            .Lx292_0_s
.Lx292_0_s:
                        .string          "XL"
#-----------------------------------------------------------------------------------------------------------------------
n116_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1856]
                        mov              rsi, qword ptr [rsp + 1864]
                        mov              rdx, qword ptr [rsp + 1872]
                        mov              rcx, qword ptr [rsp + 1880]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx293_240
                        add              rsp, 16
                                                                                        jmp   n113_var_α
.Lx293_240:
                        mov              qword ptr [rsp + 1888], rax
                        mov              qword ptr [rsp + 1896], rdx
                        add              rsp, 16
                                                                                        jmp   n120_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n117_lit_integer_α:
                        mov              qword ptr [rsp + 1952], 6
                        mov              rax, qword ptr [rip + .Lx294_0]
                        mov              qword ptr [rsp + 1960], rax
                                                                                        jmp   n121_subscript_α
.Lx294_0:
                        .quad            10
#=======================================================================================================================
#         r<11> = 'V'
#-----------------------------------------------------------------------------------------------------------------------
n118_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 2016], rax
                        mov              qword ptr [rsp + 2024], rdx
                                                                                        jmp   n122_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n119_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1808]
                        mov              rsi, qword ptr [rsp + 1816]
                        mov              rdx, qword ptr [rsp + 1824]
                        mov              rcx, qword ptr [rsp + 1832]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n108_var_α
                        mov              qword ptr [rsp + 1840], rax
                        mov              qword ptr [rsp + 1848], rdx
                                                                                        jmp   n108_var_α
#-----------------------------------------------------------------------------------------------------------------------
n120_lit_string_α:
                        mov              qword ptr [rsp + 1904], 1
                        mov              dword ptr [rsp + 1908], 1
                        mov              rax, qword ptr [rip + .Lx297_0]
                        mov              qword ptr [rsp + 1912], rax
                                                                                        jmp   n124_assign_var_α
.Lx297_0:
                        .quad            .Lx297_0_s
.Lx297_0_s:
                        .string          "X"
#-----------------------------------------------------------------------------------------------------------------------
n121_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1936]
                        mov              rsi, qword ptr [rsp + 1944]
                        mov              rdx, qword ptr [rsp + 1952]
                        mov              rcx, qword ptr [rsp + 1960]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx298_240
                        add              rsp, 16
                                                                                        jmp   n118_var_α
.Lx298_240:
                        mov              qword ptr [rsp + 1968], rax
                        mov              qword ptr [rsp + 1976], rdx
                        add              rsp, 16
                                                                                        jmp   n125_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n122_lit_integer_α:
                        mov              qword ptr [rsp + 2032], 6
                        mov              rax, qword ptr [rip + .Lx299_0]
                        mov              qword ptr [rsp + 2040], rax
                                                                                        jmp   n126_subscript_α
.Lx299_0:
                        .quad            11
#=======================================================================================================================
#         r<12> = 'IV'
#-----------------------------------------------------------------------------------------------------------------------
n123_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 2096], rax
                        mov              qword ptr [rsp + 2104], rdx
                                                                                        jmp   n127_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n124_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1888]
                        mov              rsi, qword ptr [rsp + 1896]
                        mov              rdx, qword ptr [rsp + 1904]
                        mov              rcx, qword ptr [rsp + 1912]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n113_var_α
                        mov              qword ptr [rsp + 1920], rax
                        mov              qword ptr [rsp + 1928], rdx
                                                                                        jmp   n113_var_α
#-----------------------------------------------------------------------------------------------------------------------
n125_lit_string_α:
                        mov              qword ptr [rsp + 1984], 1
                        mov              dword ptr [rsp + 1988], 2
                        mov              rax, qword ptr [rip + .Lx302_0]
                        mov              qword ptr [rsp + 1992], rax
                                                                                        jmp   n129_assign_var_α
.Lx302_0:
                        .quad            .Lx302_0_s
.Lx302_0_s:
                        .string          "IX"
#-----------------------------------------------------------------------------------------------------------------------
n126_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 2016]
                        mov              rsi, qword ptr [rsp + 2024]
                        mov              rdx, qword ptr [rsp + 2032]
                        mov              rcx, qword ptr [rsp + 2040]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx303_240
                        add              rsp, 16
                                                                                        jmp   n123_var_α
.Lx303_240:
                        mov              qword ptr [rsp + 2048], rax
                        mov              qword ptr [rsp + 2056], rdx
                        add              rsp, 16
                                                                                        jmp   n130_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n127_lit_integer_α:
                        mov              qword ptr [rsp + 2112], 6
                        mov              rax, qword ptr [rip + .Lx304_0]
                        mov              qword ptr [rsp + 2120], rax
                                                                                        jmp   n131_subscript_α
.Lx304_0:
                        .quad            12
#=======================================================================================================================
#         r<13> = 'I'
#-----------------------------------------------------------------------------------------------------------------------
n128_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 2176], rax
                        mov              qword ptr [rsp + 2184], rdx
                                                                                        jmp   n132_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n129_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1968]
                        mov              rsi, qword ptr [rsp + 1976]
                        mov              rdx, qword ptr [rsp + 1984]
                        mov              rcx, qword ptr [rsp + 1992]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n118_var_α
                        mov              qword ptr [rsp + 2000], rax
                        mov              qword ptr [rsp + 2008], rdx
                                                                                        jmp   n118_var_α
#-----------------------------------------------------------------------------------------------------------------------
n130_lit_string_α:
                        mov              qword ptr [rsp + 2064], 1
                        mov              dword ptr [rsp + 2068], 1
                        mov              rax, qword ptr [rip + .Lx307_0]
                        mov              qword ptr [rsp + 2072], rax
                                                                                        jmp   n134_assign_var_α
.Lx307_0:
                        .quad            .Lx307_0_s
.Lx307_0_s:
                        .string          "V"
#-----------------------------------------------------------------------------------------------------------------------
n131_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 2096]
                        mov              rsi, qword ptr [rsp + 2104]
                        mov              rdx, qword ptr [rsp + 2112]
                        mov              rcx, qword ptr [rsp + 2120]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx308_240
                        add              rsp, 16
                                                                                        jmp   n128_var_α
.Lx308_240:
                        mov              qword ptr [rsp + 2128], rax
                        mov              qword ptr [rsp + 2136], rdx
                        add              rsp, 16
                                                                                        jmp   n135_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n132_lit_integer_α:
                        mov              qword ptr [rsp + 2192], 6
                        mov              rax, qword ptr [rip + .Lx309_0]
                        mov              qword ptr [rsp + 2200], rax
                                                                                        jmp   n136_subscript_α
.Lx309_0:
                        .quad            13
#=======================================================================================================================
#         i = 1
#-----------------------------------------------------------------------------------------------------------------------
n133_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx310_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n137_assign_α
.Lx310_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n134_assign_var_α:
                        mov              rdi, qword ptr [rsp + 2048]
                        mov              rsi, qword ptr [rsp + 2056]
                        mov              rdx, qword ptr [rsp + 2064]
                        mov              rcx, qword ptr [rsp + 2072]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n123_var_α
                        mov              qword ptr [rsp + 2080], rax
                        mov              qword ptr [rsp + 2088], rdx
                                                                                        jmp   n123_var_α
#-----------------------------------------------------------------------------------------------------------------------
n135_lit_string_α:
                        mov              qword ptr [rsp + 2144], 1
                        mov              dword ptr [rsp + 2148], 2
                        mov              rax, qword ptr [rip + .Lx312_0]
                        mov              qword ptr [rsp + 2152], rax
                                                                                        jmp   n138_assign_var_α
.Lx312_0:
                        .quad            .Lx312_0_s
.Lx312_0_s:
                        .string          "IV"
#-----------------------------------------------------------------------------------------------------------------------
n136_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 2176]
                        mov              rsi, qword ptr [rsp + 2184]
                        mov              rdx, qword ptr [rsp + 2192]
                        mov              rcx, qword ptr [rsp + 2200]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx313_240
                        add              rsp, 16
                                                                                        jmp   n133_lit_integer_α
.Lx313_240:
                        mov              qword ptr [rsp + 2208], rax
                        mov              qword ptr [rsp + 2216], rdx
                        add              rsp, 16
                                                                                        jmp   n139_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n137_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052368], rax
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n140_var_α
#-----------------------------------------------------------------------------------------------------------------------
n138_assign_var_α:
                        mov              rdi, qword ptr [rsp + 2128]
                        mov              rsi, qword ptr [rsp + 2136]
                        mov              rdx, qword ptr [rsp + 2144]
                        mov              rcx, qword ptr [rsp + 2152]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n128_var_α
                        mov              qword ptr [rsp + 2160], rax
                        mov              qword ptr [rsp + 2168], rdx
                                                                                        jmp   n128_var_α
#-----------------------------------------------------------------------------------------------------------------------
n139_lit_string_α:
                        mov              qword ptr [rsp + 2224], 1
                        mov              dword ptr [rsp + 2228], 1
                        mov              rax, qword ptr [rip + .Lx316_0]
                        mov              qword ptr [rsp + 2232], rax
                                                                                        jmp   n141_assign_var_α
.Lx316_0:
                        .quad            .Lx316_0_s
.Lx316_0_s:
                        .string          "I"
#=======================================================================================================================
# RLOOP   GT(n, 0)                                                       :F(RDONE)
#-----------------------------------------------------------------------------------------------------------------------
n140_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 2320], rax
                        mov              qword ptr [rsp + 2328], rdx
                                                                                        jmp   n142_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n141_assign_var_α:
                        mov              rdi, qword ptr [rsp + 2208]
                        mov              rsi, qword ptr [rsp + 2216]
                        mov              rdx, qword ptr [rsp + 2224]
                        mov              rcx, qword ptr [rsp + 2232]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n133_lit_integer_α
                        mov              qword ptr [rsp + 2240], rax
                        mov              qword ptr [rsp + 2248], rdx
                                                                                        jmp   n133_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n142_lit_integer_α:
                        mov              qword ptr [rsp + 2304], 6
                        mov              rax, qword ptr [rip + .Lx319_0]
                        mov              qword ptr [rsp + 2312], rax
                                                                                        jmp   n144_op75_α
.Lx319_0:
                        .quad            0
#=======================================================================================================================
# RDONE   roman = s                                                      :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n143_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n145_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n144_op75_α:
                        mov              eax, dword ptr [rsp + 2320]
                        cmp              eax, 7
                                                                                        je    .Lx322_1
                        cmp              eax, 6
                                                                                        jne   .Lx322_0
                        mov              eax, dword ptr [rsp + 2304]
                        cmp              eax, 6
                                                                                        jne   .Lx322_0
.Lx322_1:
                        mov              rax, qword ptr [rsp + 2320]
                        mov              qword ptr [rsp + 2288], rax
                        mov              rax, qword ptr [rsp + 2328]
                        mov              qword ptr [rsp + 2296], rax
                                                                                        jmp   n147_op75_α
.Lx322_0:
                        lea              rdi, [rsp + 2320]
                        lea              rsi, [rsp + 2304]
                        lea              rdx, [rsp + 2288]
                        mov              rcx, 111
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n147_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n145_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n146_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n146_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n147_op75_α:
                        mov              eax, dword ptr [rsp + 2304]
                        cmp              eax, 7
                                                                                        je    .Lx327_1
                        cmp              eax, 6
                                                                                        jne   .Lx327_0
                        mov              eax, dword ptr [rsp + 2320]
                        cmp              eax, 6
                                                                                        jne   .Lx327_0
.Lx327_1:
                        mov              rax, qword ptr [rsp + 2304]
                        mov              qword ptr [rsp + 2272], rax
                        mov              rax, qword ptr [rsp + 2312]
                        mov              qword ptr [rsp + 2280], rax
                                                                                        jmp   n148_op77_α
.Lx327_0:
                        lea              rdi, [rsp + 2304]
                        lea              rsi, [rsp + 2320]
                        lea              rdx, [rsp + 2272]
                        mov              rcx, 112
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n148_op77_α
#-----------------------------------------------------------------------------------------------------------------------
n148_op77_α:
                        lea              rdi, [rsp + 2288]
                        lea              rsi, [rsp + 2272]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jle   n143_var_α
                                                                                        jmp   n149_var_α
#=======================================================================================================================
#         GE(n, v<i>)                                                    :F(RNEXT)
#-----------------------------------------------------------------------------------------------------------------------
n149_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 2432], rax
                        mov              qword ptr [rsp + 2440], rdx
                                                                                        jmp   n150_var_α
#-----------------------------------------------------------------------------------------------------------------------
n150_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 2368], rax
                        mov              qword ptr [rsp + 2376], rdx
                                                                                        jmp   n152_var_α
#=======================================================================================================================
# RNEXT   i = i + 1                                                      :(RLOOP)
#-----------------------------------------------------------------------------------------------------------------------
n151_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n153_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n152_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 2384], rax
                        mov              qword ptr [rsp + 2392], rdx
                                                                                        jmp   n154_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n153_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx334_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n155_binop_α
.Lx334_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n154_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 2368]
                        mov              rsi, qword ptr [rsp + 2376]
                        mov              rdx, qword ptr [rsp + 2384]
                        mov              rcx, qword ptr [rsp + 2392]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx335_240
                        add              rsp, 16
                                                                                        jmp   n151_var_α
.Lx335_240:
                        mov              qword ptr [rsp + 2400], rax
                        mov              qword ptr [rsp + 2408], rdx
                        add              rsp, 16
                                                                                        jmp   n156_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n155_binop_α:
                        mov              eax, dword ptr [rsp + 16]
                        cmp              eax, 6
                                                                                        jne   .Lx336_0
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, 6
                                                                                        jne   .Lx336_0
                        mov              rax, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rsp + 8]
                        add              rax, rcx
                        add              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n157_assign_α
.Lx336_0:
                        mov              rdi, qword ptr [rsp + 16]
                        mov              rsi, qword ptr [rsp + 24]
                        mov              rdx, qword ptr [rsp + 0]
                        mov              rcx, qword ptr [rsp + 8]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx336_240
                        add              rsp, 32
                                                                                        jmp   n140_var_α
.Lx336_240:
                        add              rsp, 16
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n157_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n156_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 2400]
                        mov              rsi, qword ptr [rsp + 2408]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx337_240
                        add              rsp, 16
                                                                                        jmp   n151_var_α
.Lx337_240:
                        mov              qword ptr [rsp + 2416], rax
                        mov              qword ptr [rsp + 2424], rdx
                        add              rsp, 16
                                                                                        jmp   n158_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n157_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052368], rax
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n140_var_α
#-----------------------------------------------------------------------------------------------------------------------
n158_op75_α:
                        mov              eax, dword ptr [rsp + 2432]
                        cmp              eax, 7
                                                                                        je    .Lx340_1
                        cmp              eax, 6
                                                                                        jne   .Lx340_0
                        mov              eax, dword ptr [rsp + 2416]
                        cmp              eax, 6
                                                                                        jne   .Lx340_0
.Lx340_1:
                        mov              rax, qword ptr [rsp + 2432]
                        mov              qword ptr [rsp + 2352], rax
                        mov              rax, qword ptr [rsp + 2440]
                        mov              qword ptr [rsp + 2360], rax
                                                                                        jmp   n159_op75_α
.Lx340_0:
                        lea              rdi, [rsp + 2432]
                        lea              rsi, [rsp + 2416]
                        lea              rdx, [rsp + 2352]
                        mov              rcx, 109
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n159_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n159_op75_α:
                        mov              eax, dword ptr [rsp + 2416]
                        cmp              eax, 7
                                                                                        je    .Lx342_1
                        cmp              eax, 6
                                                                                        jne   .Lx342_0
                        mov              eax, dword ptr [rsp + 2432]
                        cmp              eax, 6
                                                                                        jne   .Lx342_0
.Lx342_1:
                        mov              rax, qword ptr [rsp + 2416]
                        mov              qword ptr [rsp + 2336], rax
                        mov              rax, qword ptr [rsp + 2424]
                        mov              qword ptr [rsp + 2344], rax
                                                                                        jmp   n160_op77_α
.Lx342_0:
                        lea              rdi, [rsp + 2416]
                        lea              rsi, [rsp + 2432]
                        lea              rdx, [rsp + 2336]
                        mov              rcx, 110
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n160_op77_α
#-----------------------------------------------------------------------------------------------------------------------
n160_op77_α:
                        lea              rdi, [rsp + 2352]
                        lea              rsi, [rsp + 2336]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        js    n151_var_α
                                                                                        jmp   n161_var_α
#=======================================================================================================================
#         s = s r<i>
#-----------------------------------------------------------------------------------------------------------------------
n161_var_α:
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 2464], rax
                        mov              qword ptr [rsp + 2472], rdx
                                                                                        jmp   n162_var_α
#-----------------------------------------------------------------------------------------------------------------------
n162_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 2480], rax
                        mov              qword ptr [rsp + 2488], rdx
                                                                                        jmp   n164_var_α
#=======================================================================================================================
#         n = n - v<i>                                                   :(RLOOP)
#-----------------------------------------------------------------------------------------------------------------------
n163_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 2560], rax
                        mov              qword ptr [rsp + 2568], rdx
                                                                                        jmp   n165_var_α
#-----------------------------------------------------------------------------------------------------------------------
n164_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 2496], rax
                        mov              qword ptr [rsp + 2504], rdx
                                                                                        jmp   n166_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n165_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 2576], rax
                        mov              qword ptr [rsp + 2584], rdx
                                                                                        jmp   n167_var_α
#-----------------------------------------------------------------------------------------------------------------------
n166_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 2480]
                        mov              rsi, qword ptr [rsp + 2488]
                        mov              rdx, qword ptr [rsp + 2496]
                        mov              rcx, qword ptr [rsp + 2504]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx350_240
                        add              rsp, 16
                                                                                        jmp   n163_var_α
.Lx350_240:
                        mov              qword ptr [rsp + 2512], rax
                        mov              qword ptr [rsp + 2520], rdx
                        add              rsp, 16
                                                                                        jmp   n168_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n167_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 2592], rax
                        mov              qword ptr [rsp + 2600], rdx
                                                                                        jmp   n169_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n168_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 2512]
                        mov              rsi, qword ptr [rsp + 2520]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx352_240
                        add              rsp, 16
                                                                                        jmp   n163_var_α
.Lx352_240:
                        mov              qword ptr [rsp + 2528], rax
                        mov              qword ptr [rsp + 2536], rdx
                        add              rsp, 16
                                                                                        jmp   n170_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n169_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 2576]
                        mov              rsi, qword ptr [rsp + 2584]
                        mov              rdx, qword ptr [rsp + 2592]
                        mov              rcx, qword ptr [rsp + 2600]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx353_240
                        add              rsp, 16
                                                                                        jmp   n140_var_α
.Lx353_240:
                        mov              qword ptr [rsp + 2608], rax
                        mov              qword ptr [rsp + 2616], rdx
                        add              rsp, 16
                                                                                        jmp   n171_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n170_binop_α:
                        mov              rdi, qword ptr [rsp + 2464]
                        mov              rsi, qword ptr [rsp + 2472]
                        mov              rdx, qword ptr [rsp + 2528]
                        mov              rcx, qword ptr [rsp + 2536]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 2448], rax
                        mov              qword ptr [rsp + 2456], rdx
                                                                                        jmp   n172_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n171_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 2608]
                        mov              rsi, qword ptr [rsp + 2616]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx355_240
                        add              rsp, 16
                                                                                        jmp   n140_var_α
.Lx355_240:
                        mov              qword ptr [rsp + 2624], rax
                        mov              qword ptr [rsp + 2632], rdx
                        add              rsp, 16
                                                                                        jmp   n173_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n172_assign_α:
                        mov              rax, qword ptr [rsp + 2448]
                        mov              rdx, qword ptr [rsp + 2456]
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rdx
                                                                                        jmp   n163_var_α
#-----------------------------------------------------------------------------------------------------------------------
n173_binop_α:
                        mov              eax, dword ptr [rsp + 2560]
                        cmp              eax, 6
                                                                                        jne   .Lx357_0
                        mov              eax, dword ptr [rsp + 2624]
                        cmp              eax, 6
                                                                                        jne   .Lx357_0
                        mov              rax, qword ptr [rsp + 2568]
                        mov              rcx, qword ptr [rsp + 2632]
                        sub              rax, rcx
                        mov              qword ptr [rsp + 2544], 6
                        mov              qword ptr [rsp + 2552], rax
                                                                                        jmp   n174_assign_α
.Lx357_0:
                        mov              rdi, qword ptr [rsp + 2560]
                        mov              rsi, qword ptr [rsp + 2568]
                        mov              rdx, qword ptr [rsp + 2624]
                        mov              rcx, qword ptr [rsp + 2632]
                        call             rt_sub@PLT
                        cmp              eax, 99
                                                                                        je    n140_var_α
                        mov              qword ptr [rsp + 2544], rax
                        mov              qword ptr [rsp + 2552], rdx
                                                                                        jmp   n174_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n174_assign_α:
                        mov              rax, qword ptr [rsp + 2544]
                        mov              rdx, qword ptr [rsp + 2552]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   n140_var_α
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
                        mov              rax, [rsp + 3096]
                        add              rsp, 3120
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__roman_ω:
                        mov              rax, [rsp + 3104]
                        add              rsp, 3120
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_roman_α
proc_roman_α:
                        .global          proc_roman_α
                        .global          proc_roman_β
                        .global          proc_roman_γ
                        .global          proc_roman_ω
                        sub              rsp, 3120
                        mov              [rsp + 3096], rcx
                        mov              [rsp + 3104], rdx
                        mov              rdi, rsp
                        mov              ecx, 3088
                        xor              eax, eax
                        rep stosb
proc_roman_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n359_op14_α:
                        mov              rdi, qword ptr [rsp + 3096]
                        mov              rsi, qword ptr [rsp + 3104]
                        lea              rdx, [rsp + 3120]
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
                        mov              rax, [rsp + 3096]
                        add              rsp, 3120
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_roman_ω:
                        mov              rax, [rsp + 3104]
                        add              rsp, 3120
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
                                                                                        jmp   n371_call_α
.Lx562_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n366_goto_α:
                                                                                        jmp   n372_lit_string_α
n366_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n367_goto_α:
                                                                                        jmp   n373_var_α
n367_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n368_goto_α:
                                                                                        jmp   n374_var_α
n368_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n369_goto_α:
                                                                                        jmp   n375_var_α
n369_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n370_goto_α:
                                                                                        jmp   n365_lit_integer_α
n370_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n371_call_α:
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
                        mov              rdi, qword ptr [rip + .Lx569_0]
                        mov              esi, 5
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx569_5
                        mov              rax, qword ptr [rsp + 2848]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 2856]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx569_6]
                        lea              rdx, [rip + .Lx569_7]
                                                                                        jmp   rax
.Lx569_6:
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
                                                                                        jmp   .Lx569_2
.Lx569_7:
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
                                                                                        jmp   .Lx569_2
.Lx569_5:
                        add              rsp, 96
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx569_20
                        mov              rax, qword ptr [rsp + 2752]
                        mov              rdx, qword ptr [rsp + 2760]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx569_21
.Lx569_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 2752]
                        mov              rdx, qword ptr [rsp + 2760]
                        call             rt_arg_stage@PLT
.Lx569_21:
                        mov              rdi, qword ptr [rip + .Lx569_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx569_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx569_3]
                        lea              rdx, [rip + .Lx569_4]
                                                                                        jmp   rax
.Lx569_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx569_2
.Lx569_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx569_2
.Lx569_1:
                        call             rt_faildescr@PLT
.Lx569_2:
                        mov              qword ptr [rsp + 2704], rax
                        mov              qword ptr [rsp + 2712], rdx
                        cmp              eax, 99
                                                                                        je    n377_lit_integer_α
                                                                                        jmp   n376_assign_α
n371_call_β:
                                                                                        jmp   n377_lit_integer_α
.Lx569_0:
                        .quad            .Lx569_0_s
.Lx569_0_s:
                        .string          "roman"
#=======================================================================================================================
# roman   s = ''
#-----------------------------------------------------------------------------------------------------------------------
n372_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx570_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n378_assign_α
.Lx570_0:
                        .quad            .Lx570_0_s
.Lx570_0_s:
                        .string          ""
#=======================================================================================================================
# RLOOP   GT(n, 0)                                                       :F(RDONE)
#-----------------------------------------------------------------------------------------------------------------------
n373_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 2320], rax
                        mov              qword ptr [rsp + 2328], rdx
                                                                                        jmp   n379_lit_integer_α
#=======================================================================================================================
# RNEXT   i = i + 1                                                      :(RLOOP)
#-----------------------------------------------------------------------------------------------------------------------
n374_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n380_lit_integer_α
#=======================================================================================================================
# RDONE   roman = s                                                      :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n375_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n381_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n376_assign_α:
                        mov              rsi, qword ptr [rsp + 2704]
                        mov              rdx, qword ptr [rsp + 2712]
                        mov              rdi, qword ptr [rip + .Lx574_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n377_lit_integer_α
.Lx574_0:
                        .quad            .Lx574_0_s
.Lx574_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#         OUTPUT = roman(4)
#-----------------------------------------------------------------------------------------------------------------------
n377_lit_integer_α:
                        mov              qword ptr [rsp + 2816], 6
                        mov              rax, qword ptr [rip + .Lx575_0]
                        mov              qword ptr [rsp + 2824], rax
                                                                                        jmp   n383_call_α
.Lx575_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n378_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rdx
                                                                                        jmp   n384_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n379_lit_integer_α:
                        mov              qword ptr [rsp + 2304], 6
                        mov              rax, qword ptr [rip + .Lx577_0]
                        mov              qword ptr [rsp + 2312], rax
                                                                                        jmp   n385_op75_α
.Lx577_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n380_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx578_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n386_binop_α
.Lx578_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n381_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n382_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n382_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n383_call_α:
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
                        mov              rdi, qword ptr [rip + .Lx583_0]
                        mov              esi, 5
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx583_5
                        mov              rax, qword ptr [rsp + 2912]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 2920]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx583_6]
                        lea              rdx, [rip + .Lx583_7]
                                                                                        jmp   rax
.Lx583_6:
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
                                                                                        jmp   .Lx583_2
.Lx583_7:
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
                                                                                        jmp   .Lx583_2
.Lx583_5:
                        add              rsp, 96
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx583_20
                        mov              rax, qword ptr [rsp + 2816]
                        mov              rdx, qword ptr [rsp + 2824]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx583_21
.Lx583_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 2816]
                        mov              rdx, qword ptr [rsp + 2824]
                        call             rt_arg_stage@PLT
.Lx583_21:
                        mov              rdi, qword ptr [rip + .Lx583_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx583_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx583_3]
                        lea              rdx, [rip + .Lx583_4]
                                                                                        jmp   rax
.Lx583_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx583_2
.Lx583_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx583_2
.Lx583_1:
                        call             rt_faildescr@PLT
.Lx583_2:
                        mov              qword ptr [rsp + 2768], rax
                        mov              qword ptr [rsp + 2776], rdx
                        cmp              eax, 99
                                                                                        je    n388_lit_integer_α
                                                                                        jmp   n387_assign_α
n383_call_β:
                                                                                        jmp   n388_lit_integer_α
.Lx583_0:
                        .quad            .Lx583_0_s
.Lx583_0_s:
                        .string          "roman"
#=======================================================================================================================
#         v = ARRAY(13)
#-----------------------------------------------------------------------------------------------------------------------
n384_lit_integer_α:
                        mov              qword ptr [rsp + 96], 6
                        mov              rax, qword ptr [rip + .Lx584_0]
                        mov              qword ptr [rsp + 104], rax
                                                                                        jmp   n389_call_α
.Lx584_0:
                        .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n385_op75_α:
                        mov              eax, dword ptr [rsp + 2320]
                        cmp              eax, 7
                                                                                        je    .Lx586_1
                        cmp              eax, 6
                                                                                        jne   .Lx586_0
                        mov              eax, dword ptr [rsp + 2304]
                        cmp              eax, 6
                                                                                        jne   .Lx586_0
.Lx586_1:
                        mov              rax, qword ptr [rsp + 2320]
                        mov              qword ptr [rsp + 2288], rax
                        mov              rax, qword ptr [rsp + 2328]
                        mov              qword ptr [rsp + 2296], rax
                                                                                        jmp   n390_op75_α
.Lx586_0:
                        lea              rdi, [rsp + 2320]
                        lea              rsi, [rsp + 2304]
                        lea              rdx, [rsp + 2288]
                        mov              rcx, 111
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n390_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n386_binop_α:
                        mov              eax, dword ptr [rsp + 16]
                        cmp              eax, 6
                                                                                        jne   .Lx587_0
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, 6
                                                                                        jne   .Lx587_0
                        mov              rax, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rsp + 8]
                        add              rax, rcx
                        add              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n391_assign_α
.Lx587_0:
                        mov              rdi, qword ptr [rsp + 16]
                        mov              rsi, qword ptr [rsp + 24]
                        mov              rdx, qword ptr [rsp + 0]
                        mov              rcx, qword ptr [rsp + 8]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx587_240
                        add              rsp, 32
                                                                                        jmp   n373_var_α
.Lx587_240:
                        add              rsp, 16
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n391_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n387_assign_α:
                        mov              rsi, qword ptr [rsp + 2768]
                        mov              rdx, qword ptr [rsp + 2776]
                        mov              rdi, qword ptr [rip + .Lx588_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n388_lit_integer_α
.Lx588_0:
                        .quad            .Lx588_0_s
.Lx588_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#         OUTPUT = roman(9)
#-----------------------------------------------------------------------------------------------------------------------
n388_lit_integer_α:
                        mov              qword ptr [rsp + 2880], 6
                        mov              rax, qword ptr [rip + .Lx589_0]
                        mov              qword ptr [rsp + 2888], rax
                                                                                        jmp   n392_call_α
.Lx589_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n389_call_α:
                        mov              rax, qword ptr [rsp + 96]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 72], rax
                        .section         .rodata
.Lrkfn591:              .string          "ARRAY"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn591]
                        lea              rsi, [rsp + 64]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 48], rax
                        mov              qword ptr [rsp + 56], rdx
                        cmp              eax, 99
                                                                                        je    n394_var_α
                                                                                        jmp   n393_assign_α
n389_call_β:
                                                                                        jmp   n394_var_α
#-----------------------------------------------------------------------------------------------------------------------
n390_op75_α:
                        mov              eax, dword ptr [rsp + 2304]
                        cmp              eax, 7
                                                                                        je    .Lx593_1
                        cmp              eax, 6
                                                                                        jne   .Lx593_0
                        mov              eax, dword ptr [rsp + 2320]
                        cmp              eax, 6
                                                                                        jne   .Lx593_0
.Lx593_1:
                        mov              rax, qword ptr [rsp + 2304]
                        mov              qword ptr [rsp + 2272], rax
                        mov              rax, qword ptr [rsp + 2312]
                        mov              qword ptr [rsp + 2280], rax
                                                                                        jmp   n395_op77_α
.Lx593_0:
                        lea              rdi, [rsp + 2304]
                        lea              rsi, [rsp + 2320]
                        lea              rdx, [rsp + 2272]
                        mov              rcx, 112
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n395_op77_α
#-----------------------------------------------------------------------------------------------------------------------
n391_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052368], rax
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n373_var_α
#-----------------------------------------------------------------------------------------------------------------------
n392_call_α:
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
                        mov              rdi, qword ptr [rip + .Lx596_0]
                        mov              esi, 5
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx596_5
                        mov              rax, qword ptr [rsp + 2976]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 2984]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx596_6]
                        lea              rdx, [rip + .Lx596_7]
                                                                                        jmp   rax
.Lx596_6:
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
                                                                                        jmp   .Lx596_2
.Lx596_7:
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
                                                                                        jmp   .Lx596_2
.Lx596_5:
                        add              rsp, 96
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx596_20
                        mov              rax, qword ptr [rsp + 2880]
                        mov              rdx, qword ptr [rsp + 2888]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx596_21
.Lx596_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 2880]
                        mov              rdx, qword ptr [rsp + 2888]
                        call             rt_arg_stage@PLT
.Lx596_21:
                        mov              rdi, qword ptr [rip + .Lx596_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx596_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx596_3]
                        lea              rdx, [rip + .Lx596_4]
                                                                                        jmp   rax
.Lx596_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx596_2
.Lx596_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx596_2
.Lx596_1:
                        call             rt_faildescr@PLT
.Lx596_2:
                        mov              qword ptr [rsp + 2832], rax
                        mov              qword ptr [rsp + 2840], rdx
                        cmp              eax, 99
                                                                                        je    n397_lit_integer_α
                                                                                        jmp   n396_assign_α
n392_call_β:
                                                                                        jmp   n397_lit_integer_α
.Lx596_0:
                        .quad            .Lx596_0_s
.Lx596_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n393_assign_α:
                        mov              rax, qword ptr [rsp + 48]
                        mov              rdx, qword ptr [rsp + 56]
                        mov              qword ptr [1879052336], rax
                        mov              qword ptr [1879052344], rdx
                                                                                        jmp   n394_var_α
#=======================================================================================================================
#         v<1> = 1000
#-----------------------------------------------------------------------------------------------------------------------
n394_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 112], rax
                        mov              qword ptr [rsp + 120], rdx
                                                                                        jmp   n398_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n395_op77_α:
                        lea              rdi, [rsp + 2288]
                        lea              rsi, [rsp + 2272]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jle   n375_var_α
                                                                                        jmp   n400_var_α
#-----------------------------------------------------------------------------------------------------------------------
n396_assign_α:
                        mov              rsi, qword ptr [rsp + 2832]
                        mov              rdx, qword ptr [rsp + 2840]
                        mov              rdi, qword ptr [rip + .Lx601_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n397_lit_integer_α
.Lx601_0:
                        .quad            .Lx601_0_s
.Lx601_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#         OUTPUT = roman(42)
#-----------------------------------------------------------------------------------------------------------------------
n397_lit_integer_α:
                        mov              qword ptr [rsp + 2944], 6
                        mov              rax, qword ptr [rip + .Lx602_0]
                        mov              qword ptr [rsp + 2952], rax
                                                                                        jmp   n401_call_α
.Lx602_0:
                        .quad            42
#-----------------------------------------------------------------------------------------------------------------------
n398_lit_integer_α:
                        mov              qword ptr [rsp + 128], 6
                        mov              rax, qword ptr [rip + .Lx603_0]
                        mov              qword ptr [rsp + 136], rax
                                                                                        jmp   n402_subscript_α
.Lx603_0:
                        .quad            1
#=======================================================================================================================
#         v<2> = 900
#-----------------------------------------------------------------------------------------------------------------------
n399_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 192], rax
                        mov              qword ptr [rsp + 200], rdx
                                                                                        jmp   n403_lit_integer_α
#=======================================================================================================================
#         GE(n, v<i>)                                                    :F(RNEXT)
#-----------------------------------------------------------------------------------------------------------------------
n400_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 2432], rax
                        mov              qword ptr [rsp + 2440], rdx
                                                                                        jmp   n405_var_α
#-----------------------------------------------------------------------------------------------------------------------
n401_call_α:
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
                        mov              rdi, qword ptr [rip + .Lx607_0]
                        mov              esi, 5
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx607_5
                        mov              rax, qword ptr [rsp + 3040]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 3048]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx607_6]
                        lea              rdx, [rip + .Lx607_7]
                                                                                        jmp   rax
.Lx607_6:
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
                                                                                        jmp   .Lx607_2
.Lx607_7:
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
                                                                                        jmp   .Lx607_2
.Lx607_5:
                        add              rsp, 96
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx607_20
                        mov              rax, qword ptr [rsp + 2944]
                        mov              rdx, qword ptr [rsp + 2952]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx607_21
.Lx607_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 2944]
                        mov              rdx, qword ptr [rsp + 2952]
                        call             rt_arg_stage@PLT
.Lx607_21:
                        mov              rdi, qword ptr [rip + .Lx607_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx607_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx607_3]
                        lea              rdx, [rip + .Lx607_4]
                                                                                        jmp   rax
.Lx607_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx607_2
.Lx607_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx607_2
.Lx607_1:
                        call             rt_faildescr@PLT
.Lx607_2:
                        mov              qword ptr [rsp + 2896], rax
                        mov              qword ptr [rsp + 2904], rdx
                        cmp              eax, 99
                                                                                        je    n407_lit_integer_α
                                                                                        jmp   n406_assign_α
n401_call_β:
                                                                                        jmp   n407_lit_integer_α
.Lx607_0:
                        .quad            .Lx607_0_s
.Lx607_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n402_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 112]
                        mov              rsi, qword ptr [rsp + 120]
                        mov              rdx, qword ptr [rsp + 128]
                        mov              rcx, qword ptr [rsp + 136]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx608_240
                        add              rsp, 16
                                                                                        jmp   n399_var_α
.Lx608_240:
                        mov              qword ptr [rsp + 144], rax
                        mov              qword ptr [rsp + 152], rdx
                        add              rsp, 16
                                                                                        jmp   n408_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n403_lit_integer_α:
                        mov              qword ptr [rsp + 208], 6
                        mov              rax, qword ptr [rip + .Lx609_0]
                        mov              qword ptr [rsp + 216], rax
                                                                                        jmp   n409_subscript_α
.Lx609_0:
                        .quad            2
#=======================================================================================================================
#         v<3> = 500
#-----------------------------------------------------------------------------------------------------------------------
n404_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 272], rax
                        mov              qword ptr [rsp + 280], rdx
                                                                                        jmp   n410_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n405_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 2368], rax
                        mov              qword ptr [rsp + 2376], rdx
                                                                                        jmp   n412_var_α
#-----------------------------------------------------------------------------------------------------------------------
n406_assign_α:
                        mov              rsi, qword ptr [rsp + 2896]
                        mov              rdx, qword ptr [rsp + 2904]
                        mov              rdi, qword ptr [rip + .Lx612_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n407_lit_integer_α
.Lx612_0:
                        .quad            .Lx612_0_s
.Lx612_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#         OUTPUT = roman(1999)
#-----------------------------------------------------------------------------------------------------------------------
n407_lit_integer_α:
                        mov              qword ptr [rsp + 3008], 6
                        mov              rax, qword ptr [rip + .Lx613_0]
                        mov              qword ptr [rsp + 3016], rax
                                                                                        jmp   n413_call_α
.Lx613_0:
                        .quad            1999
#-----------------------------------------------------------------------------------------------------------------------
n408_lit_integer_α:
                        mov              qword ptr [rsp + 160], 6
                        mov              rax, qword ptr [rip + .Lx614_0]
                        mov              qword ptr [rsp + 168], rax
                                                                                        jmp   n414_assign_var_α
.Lx614_0:
                        .quad            1000
#-----------------------------------------------------------------------------------------------------------------------
n409_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 192]
                        mov              rsi, qword ptr [rsp + 200]
                        mov              rdx, qword ptr [rsp + 208]
                        mov              rcx, qword ptr [rsp + 216]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx615_240
                        add              rsp, 16
                                                                                        jmp   n404_var_α
.Lx615_240:
                        mov              qword ptr [rsp + 224], rax
                        mov              qword ptr [rsp + 232], rdx
                        add              rsp, 16
                                                                                        jmp   n415_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n410_lit_integer_α:
                        mov              qword ptr [rsp + 288], 6
                        mov              rax, qword ptr [rip + .Lx616_0]
                        mov              qword ptr [rsp + 296], rax
                                                                                        jmp   n416_subscript_α
.Lx616_0:
                        .quad            3
#=======================================================================================================================
#         v<4> = 400
#-----------------------------------------------------------------------------------------------------------------------
n411_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 352], rax
                        mov              qword ptr [rsp + 360], rdx
                                                                                        jmp   n417_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n412_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 2384], rax
                        mov              qword ptr [rsp + 2392], rdx
                                                                                        jmp   n419_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n413_call_α:
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
                        mov              rdi, qword ptr [rip + .Lx620_0]
                        mov              esi, 5
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx620_5
                        mov              rax, qword ptr [rsp + 3104]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 3112]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx620_6]
                        lea              rdx, [rip + .Lx620_7]
                                                                                        jmp   rax
.Lx620_6:
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
                                                                                        jmp   .Lx620_2
.Lx620_7:
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
                                                                                        jmp   .Lx620_2
.Lx620_5:
                        add              rsp, 96
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx620_20
                        mov              rax, qword ptr [rsp + 3008]
                        mov              rdx, qword ptr [rsp + 3016]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx620_21
.Lx620_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 3008]
                        mov              rdx, qword ptr [rsp + 3016]
                        call             rt_arg_stage@PLT
.Lx620_21:
                        mov              rdi, qword ptr [rip + .Lx620_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx620_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx620_3]
                        lea              rdx, [rip + .Lx620_4]
                                                                                        jmp   rax
.Lx620_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx620_2
.Lx620_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx620_2
.Lx620_1:
                        call             rt_faildescr@PLT
.Lx620_2:
                        mov              qword ptr [rsp + 2960], rax
                        mov              qword ptr [rsp + 2968], rdx
                        cmp              eax, 99
                                                                                        je    n421_lit_integer_α
                                                                                        jmp   n420_assign_α
n413_call_β:
                                                                                        jmp   n421_lit_integer_α
.Lx620_0:
                        .quad            .Lx620_0_s
.Lx620_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n414_assign_var_α:
                        mov              rdi, qword ptr [rsp + 144]
                        mov              rsi, qword ptr [rsp + 152]
                        mov              rdx, qword ptr [rsp + 160]
                        mov              rcx, qword ptr [rsp + 168]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n399_var_α
                        mov              qword ptr [rsp + 176], rax
                        mov              qword ptr [rsp + 184], rdx
                                                                                        jmp   n399_var_α
#-----------------------------------------------------------------------------------------------------------------------
n415_lit_integer_α:
                        mov              qword ptr [rsp + 240], 6
                        mov              rax, qword ptr [rip + .Lx622_0]
                        mov              qword ptr [rsp + 248], rax
                                                                                        jmp   n422_assign_var_α
.Lx622_0:
                        .quad            900
#-----------------------------------------------------------------------------------------------------------------------
n416_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 272]
                        mov              rsi, qword ptr [rsp + 280]
                        mov              rdx, qword ptr [rsp + 288]
                        mov              rcx, qword ptr [rsp + 296]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx623_240
                        add              rsp, 16
                                                                                        jmp   n411_var_α
.Lx623_240:
                        mov              qword ptr [rsp + 304], rax
                        mov              qword ptr [rsp + 312], rdx
                        add              rsp, 16
                                                                                        jmp   n423_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n417_lit_integer_α:
                        mov              qword ptr [rsp + 368], 6
                        mov              rax, qword ptr [rip + .Lx624_0]
                        mov              qword ptr [rsp + 376], rax
                                                                                        jmp   n424_subscript_α
.Lx624_0:
                        .quad            4
#=======================================================================================================================
#         v<5> = 100
#-----------------------------------------------------------------------------------------------------------------------
n418_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 432], rax
                        mov              qword ptr [rsp + 440], rdx
                                                                                        jmp   n425_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n419_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 2368]
                        mov              rsi, qword ptr [rsp + 2376]
                        mov              rdx, qword ptr [rsp + 2384]
                        mov              rcx, qword ptr [rsp + 2392]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx626_240
                        add              rsp, 16
                                                                                        jmp   n374_var_α
.Lx626_240:
                        mov              qword ptr [rsp + 2400], rax
                        mov              qword ptr [rsp + 2408], rdx
                        add              rsp, 16
                                                                                        jmp   n427_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n420_assign_α:
                        mov              rsi, qword ptr [rsp + 2960]
                        mov              rdx, qword ptr [rsp + 2968]
                        mov              rdi, qword ptr [rip + .Lx627_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n421_lit_integer_α
.Lx627_0:
                        .quad            .Lx627_0_s
.Lx627_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#         OUTPUT = roman(2024)
#-----------------------------------------------------------------------------------------------------------------------
n421_lit_integer_α:
                        mov              qword ptr [rsp + 3072], 6
                        mov              rax, qword ptr [rip + .Lx628_0]
                        mov              qword ptr [rsp + 3080], rax
                                                                                        jmp   n428_call_α
.Lx628_0:
                        .quad            2024
#-----------------------------------------------------------------------------------------------------------------------
n422_assign_var_α:
                        mov              rdi, qword ptr [rsp + 224]
                        mov              rsi, qword ptr [rsp + 232]
                        mov              rdx, qword ptr [rsp + 240]
                        mov              rcx, qword ptr [rsp + 248]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n404_var_α
                        mov              qword ptr [rsp + 256], rax
                        mov              qword ptr [rsp + 264], rdx
                                                                                        jmp   n404_var_α
#-----------------------------------------------------------------------------------------------------------------------
n423_lit_integer_α:
                        mov              qword ptr [rsp + 320], 6
                        mov              rax, qword ptr [rip + .Lx630_0]
                        mov              qword ptr [rsp + 328], rax
                                                                                        jmp   n429_assign_var_α
.Lx630_0:
                        .quad            500
#-----------------------------------------------------------------------------------------------------------------------
n424_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 352]
                        mov              rsi, qword ptr [rsp + 360]
                        mov              rdx, qword ptr [rsp + 368]
                        mov              rcx, qword ptr [rsp + 376]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx631_240
                        add              rsp, 16
                                                                                        jmp   n418_var_α
.Lx631_240:
                        mov              qword ptr [rsp + 384], rax
                        mov              qword ptr [rsp + 392], rdx
                        add              rsp, 16
                                                                                        jmp   n430_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n425_lit_integer_α:
                        mov              qword ptr [rsp + 448], 6
                        mov              rax, qword ptr [rip + .Lx632_0]
                        mov              qword ptr [rsp + 456], rax
                                                                                        jmp   n431_subscript_α
.Lx632_0:
                        .quad            5
#=======================================================================================================================
#         v<6> = 90
#-----------------------------------------------------------------------------------------------------------------------
n426_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 512], rax
                        mov              qword ptr [rsp + 520], rdx
                                                                                        jmp   n432_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n427_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 2400]
                        mov              rsi, qword ptr [rsp + 2408]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx634_240
                        add              rsp, 16
                                                                                        jmp   n374_var_α
.Lx634_240:
                        mov              qword ptr [rsp + 2416], rax
                        mov              qword ptr [rsp + 2424], rdx
                        add              rsp, 16
                                                                                        jmp   n434_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n428_call_α:
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
                        mov              rdi, qword ptr [rip + .Lx636_0]
                        mov              esi, 5
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx636_5
                        mov              rax, qword ptr [rsp + 3168]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 3176]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx636_6]
                        lea              rdx, [rip + .Lx636_7]
                                                                                        jmp   rax
.Lx636_6:
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
                                                                                        jmp   .Lx636_2
.Lx636_7:
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
                                                                                        jmp   .Lx636_2
.Lx636_5:
                        add              rsp, 96
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx636_20
                        mov              rax, qword ptr [rsp + 3072]
                        mov              rdx, qword ptr [rsp + 3080]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx636_21
.Lx636_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 3072]
                        mov              rdx, qword ptr [rsp + 3080]
                        call             rt_arg_stage@PLT
.Lx636_21:
                        mov              rdi, qword ptr [rip + .Lx636_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx636_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx636_3]
                        lea              rdx, [rip + .Lx636_4]
                                                                                        jmp   rax
.Lx636_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx636_2
.Lx636_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx636_2
.Lx636_1:
                        call             rt_faildescr@PLT
.Lx636_2:
                        mov              qword ptr [rsp + 3024], rax
                        mov              qword ptr [rsp + 3032], rdx
                        cmp              eax, 99
                                                                                        je    main_γ
                                                                                        jmp   n435_assign_α
n428_call_β:
                                                                                        jmp   main_γ
.Lx636_0:
                        .quad            .Lx636_0_s
.Lx636_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n429_assign_var_α:
                        mov              rdi, qword ptr [rsp + 304]
                        mov              rsi, qword ptr [rsp + 312]
                        mov              rdx, qword ptr [rsp + 320]
                        mov              rcx, qword ptr [rsp + 328]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n411_var_α
                        mov              qword ptr [rsp + 336], rax
                        mov              qword ptr [rsp + 344], rdx
                                                                                        jmp   n411_var_α
#-----------------------------------------------------------------------------------------------------------------------
n430_lit_integer_α:
                        mov              qword ptr [rsp + 400], 6
                        mov              rax, qword ptr [rip + .Lx638_0]
                        mov              qword ptr [rsp + 408], rax
                                                                                        jmp   n436_assign_var_α
.Lx638_0:
                        .quad            400
#-----------------------------------------------------------------------------------------------------------------------
n431_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 432]
                        mov              rsi, qword ptr [rsp + 440]
                        mov              rdx, qword ptr [rsp + 448]
                        mov              rcx, qword ptr [rsp + 456]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx639_240
                        add              rsp, 16
                                                                                        jmp   n426_var_α
.Lx639_240:
                        mov              qword ptr [rsp + 464], rax
                        mov              qword ptr [rsp + 472], rdx
                        add              rsp, 16
                                                                                        jmp   n437_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n432_lit_integer_α:
                        mov              qword ptr [rsp + 528], 6
                        mov              rax, qword ptr [rip + .Lx640_0]
                        mov              qword ptr [rsp + 536], rax
                                                                                        jmp   n438_subscript_α
.Lx640_0:
                        .quad            6
#=======================================================================================================================
#         v<7> = 50
#-----------------------------------------------------------------------------------------------------------------------
n433_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 592], rax
                        mov              qword ptr [rsp + 600], rdx
                                                                                        jmp   n439_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n434_op75_α:
                        mov              eax, dword ptr [rsp + 2432]
                        cmp              eax, 7
                                                                                        je    .Lx643_1
                        cmp              eax, 6
                                                                                        jne   .Lx643_0
                        mov              eax, dword ptr [rsp + 2416]
                        cmp              eax, 6
                                                                                        jne   .Lx643_0
.Lx643_1:
                        mov              rax, qword ptr [rsp + 2432]
                        mov              qword ptr [rsp + 2352], rax
                        mov              rax, qword ptr [rsp + 2440]
                        mov              qword ptr [rsp + 2360], rax
                                                                                        jmp   n441_op75_α
.Lx643_0:
                        lea              rdi, [rsp + 2432]
                        lea              rsi, [rsp + 2416]
                        lea              rdx, [rsp + 2352]
                        mov              rcx, 109
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n441_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n435_assign_α:
                        mov              rsi, qword ptr [rsp + 3024]
                        mov              rdx, qword ptr [rsp + 3032]
                        mov              rdi, qword ptr [rip + .Lx644_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   main_γ
.Lx644_0:
                        .quad            .Lx644_0_s
.Lx644_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n436_assign_var_α:
                        mov              rdi, qword ptr [rsp + 384]
                        mov              rsi, qword ptr [rsp + 392]
                        mov              rdx, qword ptr [rsp + 400]
                        mov              rcx, qword ptr [rsp + 408]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n418_var_α
                        mov              qword ptr [rsp + 416], rax
                        mov              qword ptr [rsp + 424], rdx
                                                                                        jmp   n418_var_α
#-----------------------------------------------------------------------------------------------------------------------
n437_lit_integer_α:
                        mov              qword ptr [rsp + 480], 6
                        mov              rax, qword ptr [rip + .Lx646_0]
                        mov              qword ptr [rsp + 488], rax
                                                                                        jmp   n442_assign_var_α
.Lx646_0:
                        .quad            100
#-----------------------------------------------------------------------------------------------------------------------
n438_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 512]
                        mov              rsi, qword ptr [rsp + 520]
                        mov              rdx, qword ptr [rsp + 528]
                        mov              rcx, qword ptr [rsp + 536]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx647_240
                        add              rsp, 16
                                                                                        jmp   n433_var_α
.Lx647_240:
                        mov              qword ptr [rsp + 544], rax
                        mov              qword ptr [rsp + 552], rdx
                        add              rsp, 16
                                                                                        jmp   n443_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n439_lit_integer_α:
                        mov              qword ptr [rsp + 608], 6
                        mov              rax, qword ptr [rip + .Lx648_0]
                        mov              qword ptr [rsp + 616], rax
                                                                                        jmp   n444_subscript_α
.Lx648_0:
                        .quad            7
#=======================================================================================================================
#         v<8> = 40
#-----------------------------------------------------------------------------------------------------------------------
n440_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 672], rax
                        mov              qword ptr [rsp + 680], rdx
                                                                                        jmp   n445_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n441_op75_α:
                        mov              eax, dword ptr [rsp + 2416]
                        cmp              eax, 7
                                                                                        je    .Lx651_1
                        cmp              eax, 6
                                                                                        jne   .Lx651_0
                        mov              eax, dword ptr [rsp + 2432]
                        cmp              eax, 6
                                                                                        jne   .Lx651_0
.Lx651_1:
                        mov              rax, qword ptr [rsp + 2416]
                        mov              qword ptr [rsp + 2336], rax
                        mov              rax, qword ptr [rsp + 2424]
                        mov              qword ptr [rsp + 2344], rax
                                                                                        jmp   n447_op77_α
.Lx651_0:
                        lea              rdi, [rsp + 2416]
                        lea              rsi, [rsp + 2432]
                        lea              rdx, [rsp + 2336]
                        mov              rcx, 110
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n447_op77_α
#-----------------------------------------------------------------------------------------------------------------------
n442_assign_var_α:
                        mov              rdi, qword ptr [rsp + 464]
                        mov              rsi, qword ptr [rsp + 472]
                        mov              rdx, qword ptr [rsp + 480]
                        mov              rcx, qword ptr [rsp + 488]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n426_var_α
                        mov              qword ptr [rsp + 496], rax
                        mov              qword ptr [rsp + 504], rdx
                                                                                        jmp   n426_var_α
#-----------------------------------------------------------------------------------------------------------------------
n443_lit_integer_α:
                        mov              qword ptr [rsp + 560], 6
                        mov              rax, qword ptr [rip + .Lx653_0]
                        mov              qword ptr [rsp + 568], rax
                                                                                        jmp   n448_assign_var_α
.Lx653_0:
                        .quad            90
#-----------------------------------------------------------------------------------------------------------------------
n444_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 592]
                        mov              rsi, qword ptr [rsp + 600]
                        mov              rdx, qword ptr [rsp + 608]
                        mov              rcx, qword ptr [rsp + 616]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx654_240
                        add              rsp, 16
                                                                                        jmp   n440_var_α
.Lx654_240:
                        mov              qword ptr [rsp + 624], rax
                        mov              qword ptr [rsp + 632], rdx
                        add              rsp, 16
                                                                                        jmp   n449_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n445_lit_integer_α:
                        mov              qword ptr [rsp + 688], 6
                        mov              rax, qword ptr [rip + .Lx655_0]
                        mov              qword ptr [rsp + 696], rax
                                                                                        jmp   n450_subscript_α
.Lx655_0:
                        .quad            8
#=======================================================================================================================
#         v<9> = 10
#-----------------------------------------------------------------------------------------------------------------------
n446_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 752], rax
                        mov              qword ptr [rsp + 760], rdx
                                                                                        jmp   n451_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n447_op77_α:
                        lea              rdi, [rsp + 2352]
                        lea              rsi, [rsp + 2336]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        js    n374_var_α
                                                                                        jmp   n453_var_α
#-----------------------------------------------------------------------------------------------------------------------
n448_assign_var_α:
                        mov              rdi, qword ptr [rsp + 544]
                        mov              rsi, qword ptr [rsp + 552]
                        mov              rdx, qword ptr [rsp + 560]
                        mov              rcx, qword ptr [rsp + 568]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n433_var_α
                        mov              qword ptr [rsp + 576], rax
                        mov              qword ptr [rsp + 584], rdx
                                                                                        jmp   n433_var_α
#-----------------------------------------------------------------------------------------------------------------------
n449_lit_integer_α:
                        mov              qword ptr [rsp + 640], 6
                        mov              rax, qword ptr [rip + .Lx660_0]
                        mov              qword ptr [rsp + 648], rax
                                                                                        jmp   n454_assign_var_α
.Lx660_0:
                        .quad            50
#-----------------------------------------------------------------------------------------------------------------------
n450_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 672]
                        mov              rsi, qword ptr [rsp + 680]
                        mov              rdx, qword ptr [rsp + 688]
                        mov              rcx, qword ptr [rsp + 696]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx661_240
                        add              rsp, 16
                                                                                        jmp   n446_var_α
.Lx661_240:
                        mov              qword ptr [rsp + 704], rax
                        mov              qword ptr [rsp + 712], rdx
                        add              rsp, 16
                                                                                        jmp   n455_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n451_lit_integer_α:
                        mov              qword ptr [rsp + 768], 6
                        mov              rax, qword ptr [rip + .Lx662_0]
                        mov              qword ptr [rsp + 776], rax
                                                                                        jmp   n456_subscript_α
.Lx662_0:
                        .quad            9
#=======================================================================================================================
#         v<10> = 9
#-----------------------------------------------------------------------------------------------------------------------
n452_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 832], rax
                        mov              qword ptr [rsp + 840], rdx
                                                                                        jmp   n457_lit_integer_α
#=======================================================================================================================
#         s = s r<i>
#-----------------------------------------------------------------------------------------------------------------------
n453_var_α:
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 2464], rax
                        mov              qword ptr [rsp + 2472], rdx
                                                                                        jmp   n459_var_α
#-----------------------------------------------------------------------------------------------------------------------
n454_assign_var_α:
                        mov              rdi, qword ptr [rsp + 624]
                        mov              rsi, qword ptr [rsp + 632]
                        mov              rdx, qword ptr [rsp + 640]
                        mov              rcx, qword ptr [rsp + 648]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n440_var_α
                        mov              qword ptr [rsp + 656], rax
                        mov              qword ptr [rsp + 664], rdx
                                                                                        jmp   n440_var_α
#-----------------------------------------------------------------------------------------------------------------------
n455_lit_integer_α:
                        mov              qword ptr [rsp + 720], 6
                        mov              rax, qword ptr [rip + .Lx666_0]
                        mov              qword ptr [rsp + 728], rax
                                                                                        jmp   n461_assign_var_α
.Lx666_0:
                        .quad            40
#-----------------------------------------------------------------------------------------------------------------------
n456_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 752]
                        mov              rsi, qword ptr [rsp + 760]
                        mov              rdx, qword ptr [rsp + 768]
                        mov              rcx, qword ptr [rsp + 776]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx667_240
                        add              rsp, 16
                                                                                        jmp   n452_var_α
.Lx667_240:
                        mov              qword ptr [rsp + 784], rax
                        mov              qword ptr [rsp + 792], rdx
                        add              rsp, 16
                                                                                        jmp   n462_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n457_lit_integer_α:
                        mov              qword ptr [rsp + 848], 6
                        mov              rax, qword ptr [rip + .Lx668_0]
                        mov              qword ptr [rsp + 856], rax
                                                                                        jmp   n463_subscript_α
.Lx668_0:
                        .quad            10
#=======================================================================================================================
#         v<11> = 5
#-----------------------------------------------------------------------------------------------------------------------
n458_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 912], rax
                        mov              qword ptr [rsp + 920], rdx
                                                                                        jmp   n464_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n459_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 2480], rax
                        mov              qword ptr [rsp + 2488], rdx
                                                                                        jmp   n466_var_α
#=======================================================================================================================
#         n = n - v<i>                                                   :(RLOOP)
#-----------------------------------------------------------------------------------------------------------------------
n460_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 2560], rax
                        mov              qword ptr [rsp + 2568], rdx
                                                                                        jmp   n467_var_α
#-----------------------------------------------------------------------------------------------------------------------
n461_assign_var_α:
                        mov              rdi, qword ptr [rsp + 704]
                        mov              rsi, qword ptr [rsp + 712]
                        mov              rdx, qword ptr [rsp + 720]
                        mov              rcx, qword ptr [rsp + 728]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n446_var_α
                        mov              qword ptr [rsp + 736], rax
                        mov              qword ptr [rsp + 744], rdx
                                                                                        jmp   n446_var_α
#-----------------------------------------------------------------------------------------------------------------------
n462_lit_integer_α:
                        mov              qword ptr [rsp + 800], 6
                        mov              rax, qword ptr [rip + .Lx673_0]
                        mov              qword ptr [rsp + 808], rax
                                                                                        jmp   n468_assign_var_α
.Lx673_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n463_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 832]
                        mov              rsi, qword ptr [rsp + 840]
                        mov              rdx, qword ptr [rsp + 848]
                        mov              rcx, qword ptr [rsp + 856]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx674_240
                        add              rsp, 16
                                                                                        jmp   n458_var_α
.Lx674_240:
                        mov              qword ptr [rsp + 864], rax
                        mov              qword ptr [rsp + 872], rdx
                        add              rsp, 16
                                                                                        jmp   n469_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n464_lit_integer_α:
                        mov              qword ptr [rsp + 928], 6
                        mov              rax, qword ptr [rip + .Lx675_0]
                        mov              qword ptr [rsp + 936], rax
                                                                                        jmp   n470_subscript_α
.Lx675_0:
                        .quad            11
#=======================================================================================================================
#         v<12> = 4
#-----------------------------------------------------------------------------------------------------------------------
n465_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 992], rax
                        mov              qword ptr [rsp + 1000], rdx
                                                                                        jmp   n471_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n466_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 2496], rax
                        mov              qword ptr [rsp + 2504], rdx
                                                                                        jmp   n473_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n467_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 2576], rax
                        mov              qword ptr [rsp + 2584], rdx
                                                                                        jmp   n474_var_α
#-----------------------------------------------------------------------------------------------------------------------
n468_assign_var_α:
                        mov              rdi, qword ptr [rsp + 784]
                        mov              rsi, qword ptr [rsp + 792]
                        mov              rdx, qword ptr [rsp + 800]
                        mov              rcx, qword ptr [rsp + 808]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n452_var_α
                        mov              qword ptr [rsp + 816], rax
                        mov              qword ptr [rsp + 824], rdx
                                                                                        jmp   n452_var_α
#-----------------------------------------------------------------------------------------------------------------------
n469_lit_integer_α:
                        mov              qword ptr [rsp + 880], 6
                        mov              rax, qword ptr [rip + .Lx680_0]
                        mov              qword ptr [rsp + 888], rax
                                                                                        jmp   n475_assign_var_α
.Lx680_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n470_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 912]
                        mov              rsi, qword ptr [rsp + 920]
                        mov              rdx, qword ptr [rsp + 928]
                        mov              rcx, qword ptr [rsp + 936]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx681_240
                        add              rsp, 16
                                                                                        jmp   n465_var_α
.Lx681_240:
                        mov              qword ptr [rsp + 944], rax
                        mov              qword ptr [rsp + 952], rdx
                        add              rsp, 16
                                                                                        jmp   n476_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n471_lit_integer_α:
                        mov              qword ptr [rsp + 1008], 6
                        mov              rax, qword ptr [rip + .Lx682_0]
                        mov              qword ptr [rsp + 1016], rax
                                                                                        jmp   n477_subscript_α
.Lx682_0:
                        .quad            12
#=======================================================================================================================
#         v<13> = 1
#-----------------------------------------------------------------------------------------------------------------------
n472_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 1072], rax
                        mov              qword ptr [rsp + 1080], rdx
                                                                                        jmp   n478_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n473_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 2480]
                        mov              rsi, qword ptr [rsp + 2488]
                        mov              rdx, qword ptr [rsp + 2496]
                        mov              rcx, qword ptr [rsp + 2504]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx684_240
                        add              rsp, 16
                                                                                        jmp   n460_var_α
.Lx684_240:
                        mov              qword ptr [rsp + 2512], rax
                        mov              qword ptr [rsp + 2520], rdx
                        add              rsp, 16
                                                                                        jmp   n480_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n474_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 2592], rax
                        mov              qword ptr [rsp + 2600], rdx
                                                                                        jmp   n481_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n475_assign_var_α:
                        mov              rdi, qword ptr [rsp + 864]
                        mov              rsi, qword ptr [rsp + 872]
                        mov              rdx, qword ptr [rsp + 880]
                        mov              rcx, qword ptr [rsp + 888]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n458_var_α
                        mov              qword ptr [rsp + 896], rax
                        mov              qword ptr [rsp + 904], rdx
                                                                                        jmp   n458_var_α
#-----------------------------------------------------------------------------------------------------------------------
n476_lit_integer_α:
                        mov              qword ptr [rsp + 960], 6
                        mov              rax, qword ptr [rip + .Lx687_0]
                        mov              qword ptr [rsp + 968], rax
                                                                                        jmp   n482_assign_var_α
.Lx687_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n477_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 992]
                        mov              rsi, qword ptr [rsp + 1000]
                        mov              rdx, qword ptr [rsp + 1008]
                        mov              rcx, qword ptr [rsp + 1016]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx688_240
                        add              rsp, 16
                                                                                        jmp   n472_var_α
.Lx688_240:
                        mov              qword ptr [rsp + 1024], rax
                        mov              qword ptr [rsp + 1032], rdx
                        add              rsp, 16
                                                                                        jmp   n483_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n478_lit_integer_α:
                        mov              qword ptr [rsp + 1088], 6
                        mov              rax, qword ptr [rip + .Lx689_0]
                        mov              qword ptr [rsp + 1096], rax
                                                                                        jmp   n484_subscript_α
.Lx689_0:
                        .quad            13
#=======================================================================================================================
#         r = ARRAY(13)
#-----------------------------------------------------------------------------------------------------------------------
n479_lit_integer_α:
                        mov              qword ptr [rsp + 1200], 6
                        mov              rax, qword ptr [rip + .Lx690_0]
                        mov              qword ptr [rsp + 1208], rax
                                                                                        jmp   n485_call_α
.Lx690_0:
                        .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n480_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 2512]
                        mov              rsi, qword ptr [rsp + 2520]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx691_240
                        add              rsp, 16
                                                                                        jmp   n460_var_α
.Lx691_240:
                        mov              qword ptr [rsp + 2528], rax
                        mov              qword ptr [rsp + 2536], rdx
                        add              rsp, 16
                                                                                        jmp   n486_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n481_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 2576]
                        mov              rsi, qword ptr [rsp + 2584]
                        mov              rdx, qword ptr [rsp + 2592]
                        mov              rcx, qword ptr [rsp + 2600]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx692_240
                        add              rsp, 16
                                                                                        jmp   n373_var_α
.Lx692_240:
                        mov              qword ptr [rsp + 2608], rax
                        mov              qword ptr [rsp + 2616], rdx
                        add              rsp, 16
                                                                                        jmp   n487_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n482_assign_var_α:
                        mov              rdi, qword ptr [rsp + 944]
                        mov              rsi, qword ptr [rsp + 952]
                        mov              rdx, qword ptr [rsp + 960]
                        mov              rcx, qword ptr [rsp + 968]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n465_var_α
                        mov              qword ptr [rsp + 976], rax
                        mov              qword ptr [rsp + 984], rdx
                                                                                        jmp   n465_var_α
#-----------------------------------------------------------------------------------------------------------------------
n483_lit_integer_α:
                        mov              qword ptr [rsp + 1040], 6
                        mov              rax, qword ptr [rip + .Lx694_0]
                        mov              qword ptr [rsp + 1048], rax
                                                                                        jmp   n488_assign_var_α
.Lx694_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n484_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1072]
                        mov              rsi, qword ptr [rsp + 1080]
                        mov              rdx, qword ptr [rsp + 1088]
                        mov              rcx, qword ptr [rsp + 1096]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx695_240
                        add              rsp, 16
                                                                                        jmp   n479_lit_integer_α
.Lx695_240:
                        mov              qword ptr [rsp + 1104], rax
                        mov              qword ptr [rsp + 1112], rdx
                        add              rsp, 16
                                                                                        jmp   n489_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n485_call_α:
                        mov              rax, qword ptr [rsp + 1200]
                        mov              qword ptr [rsp + 1168], rax
                        mov              rax, qword ptr [rsp + 1208]
                        mov              qword ptr [rsp + 1176], rax
                        .section         .rodata
.Lrkfn697:              .string          "ARRAY"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn697]
                        lea              rsi, [rsp + 1168]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1152], rax
                        mov              qword ptr [rsp + 1160], rdx
                        cmp              eax, 99
                                                                                        je    n491_var_α
                                                                                        jmp   n490_assign_α
n485_call_β:
                                                                                        jmp   n491_var_α
#-----------------------------------------------------------------------------------------------------------------------
n486_binop_α:
                        mov              rdi, qword ptr [rsp + 2464]
                        mov              rsi, qword ptr [rsp + 2472]
                        mov              rdx, qword ptr [rsp + 2528]
                        mov              rcx, qword ptr [rsp + 2536]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 2448], rax
                        mov              qword ptr [rsp + 2456], rdx
                                                                                        jmp   n492_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n487_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 2608]
                        mov              rsi, qword ptr [rsp + 2616]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx699_240
                        add              rsp, 16
                                                                                        jmp   n373_var_α
.Lx699_240:
                        mov              qword ptr [rsp + 2624], rax
                        mov              qword ptr [rsp + 2632], rdx
                        add              rsp, 16
                                                                                        jmp   n493_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n488_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1024]
                        mov              rsi, qword ptr [rsp + 1032]
                        mov              rdx, qword ptr [rsp + 1040]
                        mov              rcx, qword ptr [rsp + 1048]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n472_var_α
                        mov              qword ptr [rsp + 1056], rax
                        mov              qword ptr [rsp + 1064], rdx
                                                                                        jmp   n472_var_α
#-----------------------------------------------------------------------------------------------------------------------
n489_lit_integer_α:
                        mov              qword ptr [rsp + 1120], 6
                        mov              rax, qword ptr [rip + .Lx701_0]
                        mov              qword ptr [rsp + 1128], rax
                                                                                        jmp   n494_assign_var_α
.Lx701_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n490_assign_α:
                        mov              rax, qword ptr [rsp + 1152]
                        mov              rdx, qword ptr [rsp + 1160]
                        mov              qword ptr [1879052352], rax
                        mov              qword ptr [1879052360], rdx
                                                                                        jmp   n491_var_α
#=======================================================================================================================
#         r<1> = 'M'
#-----------------------------------------------------------------------------------------------------------------------
n491_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 1216], rax
                        mov              qword ptr [rsp + 1224], rdx
                                                                                        jmp   n495_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n492_assign_α:
                        mov              rax, qword ptr [rsp + 2448]
                        mov              rdx, qword ptr [rsp + 2456]
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rdx
                                                                                        jmp   n460_var_α
#-----------------------------------------------------------------------------------------------------------------------
n493_binop_α:
                        mov              eax, dword ptr [rsp + 2560]
                        cmp              eax, 6
                                                                                        jne   .Lx705_0
                        mov              eax, dword ptr [rsp + 2624]
                        cmp              eax, 6
                                                                                        jne   .Lx705_0
                        mov              rax, qword ptr [rsp + 2568]
                        mov              rcx, qword ptr [rsp + 2632]
                        sub              rax, rcx
                        mov              qword ptr [rsp + 2544], 6
                        mov              qword ptr [rsp + 2552], rax
                                                                                        jmp   n497_assign_α
.Lx705_0:
                        mov              rdi, qword ptr [rsp + 2560]
                        mov              rsi, qword ptr [rsp + 2568]
                        mov              rdx, qword ptr [rsp + 2624]
                        mov              rcx, qword ptr [rsp + 2632]
                        call             rt_sub@PLT
                        cmp              eax, 99
                                                                                        je    n373_var_α
                        mov              qword ptr [rsp + 2544], rax
                        mov              qword ptr [rsp + 2552], rdx
                                                                                        jmp   n497_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n494_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1104]
                        mov              rsi, qword ptr [rsp + 1112]
                        mov              rdx, qword ptr [rsp + 1120]
                        mov              rcx, qword ptr [rsp + 1128]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n479_lit_integer_α
                        mov              qword ptr [rsp + 1136], rax
                        mov              qword ptr [rsp + 1144], rdx
                                                                                        jmp   n479_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n495_lit_integer_α:
                        mov              qword ptr [rsp + 1232], 6
                        mov              rax, qword ptr [rip + .Lx707_0]
                        mov              qword ptr [rsp + 1240], rax
                                                                                        jmp   n498_subscript_α
.Lx707_0:
                        .quad            1
#=======================================================================================================================
#         r<2> = 'CM'
#-----------------------------------------------------------------------------------------------------------------------
n496_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 1296], rax
                        mov              qword ptr [rsp + 1304], rdx
                                                                                        jmp   n499_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n497_assign_α:
                        mov              rax, qword ptr [rsp + 2544]
                        mov              rdx, qword ptr [rsp + 2552]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   n373_var_α
#-----------------------------------------------------------------------------------------------------------------------
n498_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1216]
                        mov              rsi, qword ptr [rsp + 1224]
                        mov              rdx, qword ptr [rsp + 1232]
                        mov              rcx, qword ptr [rsp + 1240]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx710_240
                        add              rsp, 16
                                                                                        jmp   n496_var_α
.Lx710_240:
                        mov              qword ptr [rsp + 1248], rax
                        mov              qword ptr [rsp + 1256], rdx
                        add              rsp, 16
                                                                                        jmp   n501_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n499_lit_integer_α:
                        mov              qword ptr [rsp + 1312], 6
                        mov              rax, qword ptr [rip + .Lx711_0]
                        mov              qword ptr [rsp + 1320], rax
                                                                                        jmp   n502_subscript_α
.Lx711_0:
                        .quad            2
#=======================================================================================================================
#         r<3> = 'D'
#-----------------------------------------------------------------------------------------------------------------------
n500_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 1376], rax
                        mov              qword ptr [rsp + 1384], rdx
                                                                                        jmp   n503_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n501_lit_string_α:
                        mov              qword ptr [rsp + 1264], 1
                        mov              dword ptr [rsp + 1268], 1
                        mov              rax, qword ptr [rip + .Lx713_0]
                        mov              qword ptr [rsp + 1272], rax
                                                                                        jmp   n505_assign_var_α
.Lx713_0:
                        .quad            .Lx713_0_s
.Lx713_0_s:
                        .string          "M"
#-----------------------------------------------------------------------------------------------------------------------
n502_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1296]
                        mov              rsi, qword ptr [rsp + 1304]
                        mov              rdx, qword ptr [rsp + 1312]
                        mov              rcx, qword ptr [rsp + 1320]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx714_240
                        add              rsp, 16
                                                                                        jmp   n500_var_α
.Lx714_240:
                        mov              qword ptr [rsp + 1328], rax
                        mov              qword ptr [rsp + 1336], rdx
                        add              rsp, 16
                                                                                        jmp   n506_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n503_lit_integer_α:
                        mov              qword ptr [rsp + 1392], 6
                        mov              rax, qword ptr [rip + .Lx715_0]
                        mov              qword ptr [rsp + 1400], rax
                                                                                        jmp   n507_subscript_α
.Lx715_0:
                        .quad            3
#=======================================================================================================================
#         r<4> = 'CD'
#-----------------------------------------------------------------------------------------------------------------------
n504_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 1456], rax
                        mov              qword ptr [rsp + 1464], rdx
                                                                                        jmp   n508_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n505_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1248]
                        mov              rsi, qword ptr [rsp + 1256]
                        mov              rdx, qword ptr [rsp + 1264]
                        mov              rcx, qword ptr [rsp + 1272]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n496_var_α
                        mov              qword ptr [rsp + 1280], rax
                        mov              qword ptr [rsp + 1288], rdx
                                                                                        jmp   n496_var_α
#-----------------------------------------------------------------------------------------------------------------------
n506_lit_string_α:
                        mov              qword ptr [rsp + 1344], 1
                        mov              dword ptr [rsp + 1348], 2
                        mov              rax, qword ptr [rip + .Lx718_0]
                        mov              qword ptr [rsp + 1352], rax
                                                                                        jmp   n510_assign_var_α
.Lx718_0:
                        .quad            .Lx718_0_s
.Lx718_0_s:
                        .string          "CM"
#-----------------------------------------------------------------------------------------------------------------------
n507_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1376]
                        mov              rsi, qword ptr [rsp + 1384]
                        mov              rdx, qword ptr [rsp + 1392]
                        mov              rcx, qword ptr [rsp + 1400]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx719_240
                        add              rsp, 16
                                                                                        jmp   n504_var_α
.Lx719_240:
                        mov              qword ptr [rsp + 1408], rax
                        mov              qword ptr [rsp + 1416], rdx
                        add              rsp, 16
                                                                                        jmp   n511_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n508_lit_integer_α:
                        mov              qword ptr [rsp + 1472], 6
                        mov              rax, qword ptr [rip + .Lx720_0]
                        mov              qword ptr [rsp + 1480], rax
                                                                                        jmp   n512_subscript_α
.Lx720_0:
                        .quad            4
#=======================================================================================================================
#         r<5> = 'C'
#-----------------------------------------------------------------------------------------------------------------------
n509_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 1536], rax
                        mov              qword ptr [rsp + 1544], rdx
                                                                                        jmp   n513_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n510_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1328]
                        mov              rsi, qword ptr [rsp + 1336]
                        mov              rdx, qword ptr [rsp + 1344]
                        mov              rcx, qword ptr [rsp + 1352]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n500_var_α
                        mov              qword ptr [rsp + 1360], rax
                        mov              qword ptr [rsp + 1368], rdx
                                                                                        jmp   n500_var_α
#-----------------------------------------------------------------------------------------------------------------------
n511_lit_string_α:
                        mov              qword ptr [rsp + 1424], 1
                        mov              dword ptr [rsp + 1428], 1
                        mov              rax, qword ptr [rip + .Lx723_0]
                        mov              qword ptr [rsp + 1432], rax
                                                                                        jmp   n515_assign_var_α
.Lx723_0:
                        .quad            .Lx723_0_s
.Lx723_0_s:
                        .string          "D"
#-----------------------------------------------------------------------------------------------------------------------
n512_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1456]
                        mov              rsi, qword ptr [rsp + 1464]
                        mov              rdx, qword ptr [rsp + 1472]
                        mov              rcx, qword ptr [rsp + 1480]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx724_240
                        add              rsp, 16
                                                                                        jmp   n509_var_α
.Lx724_240:
                        mov              qword ptr [rsp + 1488], rax
                        mov              qword ptr [rsp + 1496], rdx
                        add              rsp, 16
                                                                                        jmp   n516_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n513_lit_integer_α:
                        mov              qword ptr [rsp + 1552], 6
                        mov              rax, qword ptr [rip + .Lx725_0]
                        mov              qword ptr [rsp + 1560], rax
                                                                                        jmp   n517_subscript_α
.Lx725_0:
                        .quad            5
#=======================================================================================================================
#         r<6> = 'XC'
#-----------------------------------------------------------------------------------------------------------------------
n514_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 1616], rax
                        mov              qword ptr [rsp + 1624], rdx
                                                                                        jmp   n518_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n515_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1408]
                        mov              rsi, qword ptr [rsp + 1416]
                        mov              rdx, qword ptr [rsp + 1424]
                        mov              rcx, qword ptr [rsp + 1432]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n504_var_α
                        mov              qword ptr [rsp + 1440], rax
                        mov              qword ptr [rsp + 1448], rdx
                                                                                        jmp   n504_var_α
#-----------------------------------------------------------------------------------------------------------------------
n516_lit_string_α:
                        mov              qword ptr [rsp + 1504], 1
                        mov              dword ptr [rsp + 1508], 2
                        mov              rax, qword ptr [rip + .Lx728_0]
                        mov              qword ptr [rsp + 1512], rax
                                                                                        jmp   n520_assign_var_α
.Lx728_0:
                        .quad            .Lx728_0_s
.Lx728_0_s:
                        .string          "CD"
#-----------------------------------------------------------------------------------------------------------------------
n517_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1536]
                        mov              rsi, qword ptr [rsp + 1544]
                        mov              rdx, qword ptr [rsp + 1552]
                        mov              rcx, qword ptr [rsp + 1560]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx729_240
                        add              rsp, 16
                                                                                        jmp   n514_var_α
.Lx729_240:
                        mov              qword ptr [rsp + 1568], rax
                        mov              qword ptr [rsp + 1576], rdx
                        add              rsp, 16
                                                                                        jmp   n521_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n518_lit_integer_α:
                        mov              qword ptr [rsp + 1632], 6
                        mov              rax, qword ptr [rip + .Lx730_0]
                        mov              qword ptr [rsp + 1640], rax
                                                                                        jmp   n522_subscript_α
.Lx730_0:
                        .quad            6
#=======================================================================================================================
#         r<7> = 'L'
#-----------------------------------------------------------------------------------------------------------------------
n519_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 1696], rax
                        mov              qword ptr [rsp + 1704], rdx
                                                                                        jmp   n523_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n520_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1488]
                        mov              rsi, qword ptr [rsp + 1496]
                        mov              rdx, qword ptr [rsp + 1504]
                        mov              rcx, qword ptr [rsp + 1512]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n509_var_α
                        mov              qword ptr [rsp + 1520], rax
                        mov              qword ptr [rsp + 1528], rdx
                                                                                        jmp   n509_var_α
#-----------------------------------------------------------------------------------------------------------------------
n521_lit_string_α:
                        mov              qword ptr [rsp + 1584], 1
                        mov              dword ptr [rsp + 1588], 1
                        mov              rax, qword ptr [rip + .Lx733_0]
                        mov              qword ptr [rsp + 1592], rax
                                                                                        jmp   n525_assign_var_α
.Lx733_0:
                        .quad            .Lx733_0_s
.Lx733_0_s:
                        .string          "C"
#-----------------------------------------------------------------------------------------------------------------------
n522_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1616]
                        mov              rsi, qword ptr [rsp + 1624]
                        mov              rdx, qword ptr [rsp + 1632]
                        mov              rcx, qword ptr [rsp + 1640]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx734_240
                        add              rsp, 16
                                                                                        jmp   n519_var_α
.Lx734_240:
                        mov              qword ptr [rsp + 1648], rax
                        mov              qword ptr [rsp + 1656], rdx
                        add              rsp, 16
                                                                                        jmp   n526_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n523_lit_integer_α:
                        mov              qword ptr [rsp + 1712], 6
                        mov              rax, qword ptr [rip + .Lx735_0]
                        mov              qword ptr [rsp + 1720], rax
                                                                                        jmp   n527_subscript_α
.Lx735_0:
                        .quad            7
#=======================================================================================================================
#         r<8> = 'XL'
#-----------------------------------------------------------------------------------------------------------------------
n524_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 1776], rax
                        mov              qword ptr [rsp + 1784], rdx
                                                                                        jmp   n528_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n525_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1568]
                        mov              rsi, qword ptr [rsp + 1576]
                        mov              rdx, qword ptr [rsp + 1584]
                        mov              rcx, qword ptr [rsp + 1592]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n514_var_α
                        mov              qword ptr [rsp + 1600], rax
                        mov              qword ptr [rsp + 1608], rdx
                                                                                        jmp   n514_var_α
#-----------------------------------------------------------------------------------------------------------------------
n526_lit_string_α:
                        mov              qword ptr [rsp + 1664], 1
                        mov              dword ptr [rsp + 1668], 2
                        mov              rax, qword ptr [rip + .Lx738_0]
                        mov              qword ptr [rsp + 1672], rax
                                                                                        jmp   n530_assign_var_α
.Lx738_0:
                        .quad            .Lx738_0_s
.Lx738_0_s:
                        .string          "XC"
#-----------------------------------------------------------------------------------------------------------------------
n527_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1696]
                        mov              rsi, qword ptr [rsp + 1704]
                        mov              rdx, qword ptr [rsp + 1712]
                        mov              rcx, qword ptr [rsp + 1720]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx739_240
                        add              rsp, 16
                                                                                        jmp   n524_var_α
.Lx739_240:
                        mov              qword ptr [rsp + 1728], rax
                        mov              qword ptr [rsp + 1736], rdx
                        add              rsp, 16
                                                                                        jmp   n531_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n528_lit_integer_α:
                        mov              qword ptr [rsp + 1792], 6
                        mov              rax, qword ptr [rip + .Lx740_0]
                        mov              qword ptr [rsp + 1800], rax
                                                                                        jmp   n532_subscript_α
.Lx740_0:
                        .quad            8
#=======================================================================================================================
#         r<9> = 'X'
#-----------------------------------------------------------------------------------------------------------------------
n529_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 1856], rax
                        mov              qword ptr [rsp + 1864], rdx
                                                                                        jmp   n533_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n530_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1648]
                        mov              rsi, qword ptr [rsp + 1656]
                        mov              rdx, qword ptr [rsp + 1664]
                        mov              rcx, qword ptr [rsp + 1672]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n519_var_α
                        mov              qword ptr [rsp + 1680], rax
                        mov              qword ptr [rsp + 1688], rdx
                                                                                        jmp   n519_var_α
#-----------------------------------------------------------------------------------------------------------------------
n531_lit_string_α:
                        mov              qword ptr [rsp + 1744], 1
                        mov              dword ptr [rsp + 1748], 1
                        mov              rax, qword ptr [rip + .Lx743_0]
                        mov              qword ptr [rsp + 1752], rax
                                                                                        jmp   n535_assign_var_α
.Lx743_0:
                        .quad            .Lx743_0_s
.Lx743_0_s:
                        .string          "L"
#-----------------------------------------------------------------------------------------------------------------------
n532_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1776]
                        mov              rsi, qword ptr [rsp + 1784]
                        mov              rdx, qword ptr [rsp + 1792]
                        mov              rcx, qword ptr [rsp + 1800]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx744_240
                        add              rsp, 16
                                                                                        jmp   n529_var_α
.Lx744_240:
                        mov              qword ptr [rsp + 1808], rax
                        mov              qword ptr [rsp + 1816], rdx
                        add              rsp, 16
                                                                                        jmp   n536_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n533_lit_integer_α:
                        mov              qword ptr [rsp + 1872], 6
                        mov              rax, qword ptr [rip + .Lx745_0]
                        mov              qword ptr [rsp + 1880], rax
                                                                                        jmp   n537_subscript_α
.Lx745_0:
                        .quad            9
#=======================================================================================================================
#         r<10> = 'IX'
#-----------------------------------------------------------------------------------------------------------------------
n534_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 1936], rax
                        mov              qword ptr [rsp + 1944], rdx
                                                                                        jmp   n538_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n535_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1728]
                        mov              rsi, qword ptr [rsp + 1736]
                        mov              rdx, qword ptr [rsp + 1744]
                        mov              rcx, qword ptr [rsp + 1752]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n524_var_α
                        mov              qword ptr [rsp + 1760], rax
                        mov              qword ptr [rsp + 1768], rdx
                                                                                        jmp   n524_var_α
#-----------------------------------------------------------------------------------------------------------------------
n536_lit_string_α:
                        mov              qword ptr [rsp + 1824], 1
                        mov              dword ptr [rsp + 1828], 2
                        mov              rax, qword ptr [rip + .Lx748_0]
                        mov              qword ptr [rsp + 1832], rax
                                                                                        jmp   n540_assign_var_α
.Lx748_0:
                        .quad            .Lx748_0_s
.Lx748_0_s:
                        .string          "XL"
#-----------------------------------------------------------------------------------------------------------------------
n537_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1856]
                        mov              rsi, qword ptr [rsp + 1864]
                        mov              rdx, qword ptr [rsp + 1872]
                        mov              rcx, qword ptr [rsp + 1880]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx749_240
                        add              rsp, 16
                                                                                        jmp   n534_var_α
.Lx749_240:
                        mov              qword ptr [rsp + 1888], rax
                        mov              qword ptr [rsp + 1896], rdx
                        add              rsp, 16
                                                                                        jmp   n541_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n538_lit_integer_α:
                        mov              qword ptr [rsp + 1952], 6
                        mov              rax, qword ptr [rip + .Lx750_0]
                        mov              qword ptr [rsp + 1960], rax
                                                                                        jmp   n542_subscript_α
.Lx750_0:
                        .quad            10
#=======================================================================================================================
#         r<11> = 'V'
#-----------------------------------------------------------------------------------------------------------------------
n539_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 2016], rax
                        mov              qword ptr [rsp + 2024], rdx
                                                                                        jmp   n543_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n540_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1808]
                        mov              rsi, qword ptr [rsp + 1816]
                        mov              rdx, qword ptr [rsp + 1824]
                        mov              rcx, qword ptr [rsp + 1832]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n529_var_α
                        mov              qword ptr [rsp + 1840], rax
                        mov              qword ptr [rsp + 1848], rdx
                                                                                        jmp   n529_var_α
#-----------------------------------------------------------------------------------------------------------------------
n541_lit_string_α:
                        mov              qword ptr [rsp + 1904], 1
                        mov              dword ptr [rsp + 1908], 1
                        mov              rax, qword ptr [rip + .Lx753_0]
                        mov              qword ptr [rsp + 1912], rax
                                                                                        jmp   n545_assign_var_α
.Lx753_0:
                        .quad            .Lx753_0_s
.Lx753_0_s:
                        .string          "X"
#-----------------------------------------------------------------------------------------------------------------------
n542_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 1936]
                        mov              rsi, qword ptr [rsp + 1944]
                        mov              rdx, qword ptr [rsp + 1952]
                        mov              rcx, qword ptr [rsp + 1960]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx754_240
                        add              rsp, 16
                                                                                        jmp   n539_var_α
.Lx754_240:
                        mov              qword ptr [rsp + 1968], rax
                        mov              qword ptr [rsp + 1976], rdx
                        add              rsp, 16
                                                                                        jmp   n546_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n543_lit_integer_α:
                        mov              qword ptr [rsp + 2032], 6
                        mov              rax, qword ptr [rip + .Lx755_0]
                        mov              qword ptr [rsp + 2040], rax
                                                                                        jmp   n547_subscript_α
.Lx755_0:
                        .quad            11
#=======================================================================================================================
#         r<12> = 'IV'
#-----------------------------------------------------------------------------------------------------------------------
n544_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 2096], rax
                        mov              qword ptr [rsp + 2104], rdx
                                                                                        jmp   n548_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n545_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1888]
                        mov              rsi, qword ptr [rsp + 1896]
                        mov              rdx, qword ptr [rsp + 1904]
                        mov              rcx, qword ptr [rsp + 1912]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n534_var_α
                        mov              qword ptr [rsp + 1920], rax
                        mov              qword ptr [rsp + 1928], rdx
                                                                                        jmp   n534_var_α
#-----------------------------------------------------------------------------------------------------------------------
n546_lit_string_α:
                        mov              qword ptr [rsp + 1984], 1
                        mov              dword ptr [rsp + 1988], 2
                        mov              rax, qword ptr [rip + .Lx758_0]
                        mov              qword ptr [rsp + 1992], rax
                                                                                        jmp   n550_assign_var_α
.Lx758_0:
                        .quad            .Lx758_0_s
.Lx758_0_s:
                        .string          "IX"
#-----------------------------------------------------------------------------------------------------------------------
n547_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 2016]
                        mov              rsi, qword ptr [rsp + 2024]
                        mov              rdx, qword ptr [rsp + 2032]
                        mov              rcx, qword ptr [rsp + 2040]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx759_240
                        add              rsp, 16
                                                                                        jmp   n544_var_α
.Lx759_240:
                        mov              qword ptr [rsp + 2048], rax
                        mov              qword ptr [rsp + 2056], rdx
                        add              rsp, 16
                                                                                        jmp   n551_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n548_lit_integer_α:
                        mov              qword ptr [rsp + 2112], 6
                        mov              rax, qword ptr [rip + .Lx760_0]
                        mov              qword ptr [rsp + 2120], rax
                                                                                        jmp   n552_subscript_α
.Lx760_0:
                        .quad            12
#=======================================================================================================================
#         r<13> = 'I'
#-----------------------------------------------------------------------------------------------------------------------
n549_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 2176], rax
                        mov              qword ptr [rsp + 2184], rdx
                                                                                        jmp   n553_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n550_assign_var_α:
                        mov              rdi, qword ptr [rsp + 1968]
                        mov              rsi, qword ptr [rsp + 1976]
                        mov              rdx, qword ptr [rsp + 1984]
                        mov              rcx, qword ptr [rsp + 1992]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n539_var_α
                        mov              qword ptr [rsp + 2000], rax
                        mov              qword ptr [rsp + 2008], rdx
                                                                                        jmp   n539_var_α
#-----------------------------------------------------------------------------------------------------------------------
n551_lit_string_α:
                        mov              qword ptr [rsp + 2064], 1
                        mov              dword ptr [rsp + 2068], 1
                        mov              rax, qword ptr [rip + .Lx763_0]
                        mov              qword ptr [rsp + 2072], rax
                                                                                        jmp   n555_assign_var_α
.Lx763_0:
                        .quad            .Lx763_0_s
.Lx763_0_s:
                        .string          "V"
#-----------------------------------------------------------------------------------------------------------------------
n552_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 2096]
                        mov              rsi, qword ptr [rsp + 2104]
                        mov              rdx, qword ptr [rsp + 2112]
                        mov              rcx, qword ptr [rsp + 2120]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx764_240
                        add              rsp, 16
                                                                                        jmp   n549_var_α
.Lx764_240:
                        mov              qword ptr [rsp + 2128], rax
                        mov              qword ptr [rsp + 2136], rdx
                        add              rsp, 16
                                                                                        jmp   n556_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n553_lit_integer_α:
                        mov              qword ptr [rsp + 2192], 6
                        mov              rax, qword ptr [rip + .Lx765_0]
                        mov              qword ptr [rsp + 2200], rax
                                                                                        jmp   n557_subscript_α
.Lx765_0:
                        .quad            13
#=======================================================================================================================
#         i = 1
#-----------------------------------------------------------------------------------------------------------------------
n554_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx766_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n558_assign_α
.Lx766_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n555_assign_var_α:
                        mov              rdi, qword ptr [rsp + 2048]
                        mov              rsi, qword ptr [rsp + 2056]
                        mov              rdx, qword ptr [rsp + 2064]
                        mov              rcx, qword ptr [rsp + 2072]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n544_var_α
                        mov              qword ptr [rsp + 2080], rax
                        mov              qword ptr [rsp + 2088], rdx
                                                                                        jmp   n544_var_α
#-----------------------------------------------------------------------------------------------------------------------
n556_lit_string_α:
                        mov              qword ptr [rsp + 2144], 1
                        mov              dword ptr [rsp + 2148], 2
                        mov              rax, qword ptr [rip + .Lx768_0]
                        mov              qword ptr [rsp + 2152], rax
                                                                                        jmp   n559_assign_var_α
.Lx768_0:
                        .quad            .Lx768_0_s
.Lx768_0_s:
                        .string          "IV"
#-----------------------------------------------------------------------------------------------------------------------
n557_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 2176]
                        mov              rsi, qword ptr [rsp + 2184]
                        mov              rdx, qword ptr [rsp + 2192]
                        mov              rcx, qword ptr [rsp + 2200]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx769_240
                        add              rsp, 16
                                                                                        jmp   n554_lit_integer_α
.Lx769_240:
                        mov              qword ptr [rsp + 2208], rax
                        mov              qword ptr [rsp + 2216], rdx
                        add              rsp, 16
                                                                                        jmp   n560_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n558_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052368], rax
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n373_var_α
#-----------------------------------------------------------------------------------------------------------------------
n559_assign_var_α:
                        mov              rdi, qword ptr [rsp + 2128]
                        mov              rsi, qword ptr [rsp + 2136]
                        mov              rdx, qword ptr [rsp + 2144]
                        mov              rcx, qword ptr [rsp + 2152]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n549_var_α
                        mov              qword ptr [rsp + 2160], rax
                        mov              qword ptr [rsp + 2168], rdx
                                                                                        jmp   n549_var_α
#-----------------------------------------------------------------------------------------------------------------------
n560_lit_string_α:
                        mov              qword ptr [rsp + 2224], 1
                        mov              dword ptr [rsp + 2228], 1
                        mov              rax, qword ptr [rip + .Lx772_0]
                        mov              qword ptr [rsp + 2232], rax
                                                                                        jmp   n561_assign_var_α
.Lx772_0:
                        .quad            .Lx772_0_s
.Lx772_0_s:
                        .string          "I"
#-----------------------------------------------------------------------------------------------------------------------
n561_assign_var_α:
                        mov              rdi, qword ptr [rsp + 2208]
                        mov              rsi, qword ptr [rsp + 2216]
                        mov              rdx, qword ptr [rsp + 2224]
                        mov              rcx, qword ptr [rsp + 2232]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n554_lit_integer_α
                        mov              qword ptr [rsp + 2240], rax
                        mov              qword ptr [rsp + 2248], rdx
                                                                                        jmp   n554_lit_integer_α
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

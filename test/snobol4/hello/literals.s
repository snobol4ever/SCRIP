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
                        sub              rsp, 65544
                        mov              rdi, rsp
                        mov              ecx, 65544
                        xor              eax, eax
                        rep stosb
                        mov              qword ptr [rsp + 1240], rsp
                        mov              r12, qword ptr [1879048192]
                        mov              [rsp + 65536], rbp
                        mov              rbp, rsp
main_α_body:
#=======================================================================================================================
#       OUTPUT =
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n0_α:
                        mov              qword ptr [rbp + 112], 1
                        mov              rax, qword ptr [rip + .Lx1_0]
                        mov              qword ptr [rbp + 120], rax
                                                                                        jmp   xchain0_n1_α
.Lx1_0:
                        .quad            .Lx1_0_s
.Lx1_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN global
xchain0_n1_α:
                        mov              rsi, qword ptr [rbp + 112]
                        mov              rdx, qword ptr [rbp + 120]
                        mov              rdi, qword ptr [rip + .Lx2_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 96], rax
                        mov              qword ptr [rbp + 104], rdx
                                                                                        jmp   xchain0_n2_α
.Lx2_0:
                        .quad            .Lx2_0_s
.Lx2_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#       OUTPUT = ""
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n2_α:
                        mov              qword ptr [rbp + 160], 1
                        mov              rax, qword ptr [rip + .Lx3_0]
                        mov              qword ptr [rbp + 168], rax
                                                                                        jmp   xchain0_n3_α
.Lx3_0:
                        .quad            .Lx3_0_s
.Lx3_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN global
xchain0_n3_α:
                        mov              rsi, qword ptr [rbp + 160]
                        mov              rdx, qword ptr [rbp + 168]
                        mov              rdi, qword ptr [rip + .Lx4_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 144], rax
                        mov              qword ptr [rbp + 152], rdx
                                                                                        jmp   xchain0_n4_α
.Lx4_0:
                        .quad            .Lx4_0_s
.Lx4_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#       OUTPUT = "Hello World!"
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n4_α:
                        mov              qword ptr [rbp + 208], 1
                        mov              rax, qword ptr [rip + .Lx5_0]
                        mov              qword ptr [rbp + 216], rax
                                                                                        jmp   xchain0_n5_α
.Lx5_0:
                        .quad            .Lx5_0_s
.Lx5_0_s:
                        .string          "Hello World!"
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN global
xchain0_n5_α:
                        mov              rsi, qword ptr [rbp + 208]
                        mov              rdx, qword ptr [rbp + 216]
                        mov              rdi, qword ptr [rip + .Lx6_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 192], rax
                        mov              qword ptr [rbp + 200], rdx
                                                                                        jmp   xchain0_n6_α
.Lx6_0:
                        .quad            .Lx6_0_s
.Lx6_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#       OUTPUT = 0
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_INTEGER
xchain0_n6_α:
                        mov              qword ptr [rbp + 256], 6
                        mov              rax, qword ptr [rip + .Lx7_0]
                        mov              qword ptr [rbp + 264], rax
                                                                                        jmp   xchain0_n7_α
.Lx7_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN global
xchain0_n7_α:
                        mov              rsi, qword ptr [rbp + 256]
                        mov              rdx, qword ptr [rbp + 264]
                        mov              rdi, qword ptr [rip + .Lx8_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 240], rax
                        mov              qword ptr [rbp + 248], rdx
                                                                                        jmp   xchain0_n8_α
.Lx8_0:
                        .quad            .Lx8_0_s
.Lx8_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#       OUTPUT = 1
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_INTEGER
xchain0_n8_α:
                        mov              qword ptr [rbp + 304], 6
                        mov              rax, qword ptr [rip + .Lx9_0]
                        mov              qword ptr [rbp + 312], rax
                                                                                        jmp   xchain0_n9_α
.Lx9_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN global
xchain0_n9_α:
                        mov              rsi, qword ptr [rbp + 304]
                        mov              rdx, qword ptr [rbp + 312]
                        mov              rdi, qword ptr [rip + .Lx10_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 288], rax
                        mov              qword ptr [rbp + 296], rdx
                                                                                        jmp   xchain0_n10_α
.Lx10_0:
                        .quad            .Lx10_0_s
.Lx10_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#       OUTPUT = -1
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_INTEGER
xchain0_n10_α:
                        mov              qword ptr [rbp + 384], 6
                        mov              rax, qword ptr [rip + .Lx11_0]
                        mov              qword ptr [rbp + 392], rax
                                                                                        jmp   xchain0_n11_α
.Lx11_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
# IR_UNOP
xchain0_n11_α:
                        mov              rdi, qword ptr [rbp + 384]
                        mov              rsi, qword ptr [rbp + 392]
                        call             rt_num_neg@PLT
                        mov              qword ptr [rbp + 352], rax
                        mov              qword ptr [rbp + 360], rdx
                                                                                        jmp   xchain0_n12_α
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN global
xchain0_n12_α:
                        mov              rsi, qword ptr [rbp + 352]
                        mov              rdx, qword ptr [rbp + 360]
                        mov              rdi, qword ptr [rip + .Lx13_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 336], rax
                        mov              qword ptr [rbp + 344], rdx
                                                                                        jmp   xchain0_n13_α
.Lx13_0:
                        .quad            .Lx13_0_s
.Lx13_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#       OUTPUT = 1.0
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_REAL
xchain0_n13_α:
                        mov              qword ptr [rbp + 432], 7
                        mov              rax, qword ptr [rip + .Lx14_0]
                        mov              qword ptr [rbp + 440], rax
                                                                                        jmp   xchain0_n14_α
.Lx14_0:
                        .quad            4607182418800017408
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN global
xchain0_n14_α:
                        mov              rsi, qword ptr [rbp + 432]
                        mov              rdx, qword ptr [rbp + 440]
                        mov              rdi, qword ptr [rip + .Lx15_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 416], rax
                        mov              qword ptr [rbp + 424], rdx
                                                                                        jmp   xchain0_n15_α
.Lx15_0:
                        .quad            .Lx15_0_s
.Lx15_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#       OUTPUT = '1'
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n15_α:
                        mov              qword ptr [rbp + 480], 1
                        mov              rax, qword ptr [rip + .Lx16_0]
                        mov              qword ptr [rbp + 488], rax
                                                                                        jmp   xchain0_n16_α
.Lx16_0:
                        .quad            .Lx16_0_s
.Lx16_0_s:
                        .string          "1"
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN global
xchain0_n16_α:
                        mov              rsi, qword ptr [rbp + 480]
                        mov              rdx, qword ptr [rbp + 488]
                        mov              rdi, qword ptr [rip + .Lx17_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 464], rax
                        mov              qword ptr [rbp + 472], rdx
                                                                                        jmp   xchain0_n17_α
.Lx17_0:
                        .quad            .Lx17_0_s
.Lx17_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#       OUTPUT = '1'
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n17_α:
                        mov              qword ptr [rbp + 528], 1
                        mov              rax, qword ptr [rip + .Lx18_0]
                        mov              qword ptr [rbp + 536], rax
                                                                                        jmp   xchain0_n18_α
.Lx18_0:
                        .quad            .Lx18_0_s
.Lx18_0_s:
                        .string          "1"
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN global
xchain0_n18_α:
                        mov              rsi, qword ptr [rbp + 528]
                        mov              rdx, qword ptr [rbp + 536]
                        mov              rdi, qword ptr [rip + .Lx19_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 512], rax
                        mov              qword ptr [rbp + 520], rdx
                                                                                        jmp   xchain0_n19_α
.Lx19_0:
                        .quad            .Lx19_0_s
.Lx19_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#       OUTPUT = '1.0'
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n19_α:
                        mov              qword ptr [rbp + 576], 1
                        mov              rax, qword ptr [rip + .Lx20_0]
                        mov              qword ptr [rbp + 584], rax
                                                                                        jmp   xchain0_n20_α
.Lx20_0:
                        .quad            .Lx20_0_s
.Lx20_0_s:
                        .string          "1.0"
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN global
xchain0_n20_α:
                        mov              rsi, qword ptr [rbp + 576]
                        mov              rdx, qword ptr [rbp + 584]
                        mov              rdi, qword ptr [rip + .Lx21_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 560], rax
                        mov              qword ptr [rbp + 568], rdx
                                                                                        jmp   xchain0_n21_α
.Lx21_0:
                        .quad            .Lx21_0_s
.Lx21_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#       OUTPUT = "I'm here"
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n21_α:
                        mov              qword ptr [rbp + 624], 1
                        mov              rax, qword ptr [rip + .Lx22_0]
                        mov              qword ptr [rbp + 632], rax
                                                                                        jmp   xchain0_n22_α
.Lx22_0:
                        .quad            .Lx22_0_s
.Lx22_0_s:
                        .string          "I'm here"
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN global
xchain0_n22_α:
                        mov              rsi, qword ptr [rbp + 624]
                        mov              rdx, qword ptr [rbp + 632]
                        mov              rdi, qword ptr [rip + .Lx23_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 608], rax
                        mov              qword ptr [rbp + 616], rdx
                                                                                        jmp   xchain0_n23_α
.Lx23_0:
                        .quad            .Lx23_0_s
.Lx23_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#       OUTPUT = '"Quote of the day"'
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n23_α:
                        mov              qword ptr [rbp + 672], 1
                        mov              rax, qword ptr [rip + .Lx24_0]
                        mov              qword ptr [rbp + 680], rax
                                                                                        jmp   xchain0_n24_α
.Lx24_0:
                        .quad            .Lx24_0_s
.Lx24_0_s:
                        .string          "\"Quote of the day\""
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN global
xchain0_n24_α:
                        mov              rsi, qword ptr [rbp + 672]
                        mov              rdx, qword ptr [rbp + 680]
                        mov              rdi, qword ptr [rip + .Lx25_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 656], rax
                        mov              qword ptr [rbp + 664], rdx
                                                                                        jmp   xchain0_n25_α
.Lx25_0:
                        .quad            .Lx25_0_s
.Lx25_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_INTEGER
xchain0_n25_α:
                        mov              qword ptr [rbp + 720], 6
                        mov              rax, qword ptr [rip + .Lx26_0]
                        mov              qword ptr [rbp + 728], rax
                                                                                        jmp   xchain0_n26_α
.Lx26_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN global
xchain0_n26_α:
                        mov              rsi, qword ptr [rbp + 720]
                        mov              rdx, qword ptr [rbp + 728]
                        mov              rdi, qword ptr [rip + .Lx27_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 704], rax
                        mov              qword ptr [rbp + 712], rdx
                                                                                        jmp   xchain0_n27_α
.Lx27_0:
                        .quad            .Lx27_0_s
.Lx27_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_INTEGER
xchain0_n27_α:
                        mov              qword ptr [rbp + 768], 6
                        mov              rax, qword ptr [rip + .Lx28_0]
                        mov              qword ptr [rbp + 776], rax
                                                                                        jmp   xchain0_n28_α
.Lx28_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN global
xchain0_n28_α:
                        mov              rsi, qword ptr [rbp + 768]
                        mov              rdx, qword ptr [rbp + 776]
                        mov              rdi, qword ptr [rip + .Lx29_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 752], rax
                        mov              qword ptr [rbp + 760], rdx
                                                                                        jmp   xchain0_n29_α
.Lx29_0:
                        .quad            .Lx29_0_s
.Lx29_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_INTEGER
xchain0_n29_α:
                        mov              qword ptr [rbp + 816], 6
                        mov              rax, qword ptr [rip + .Lx30_0]
                        mov              qword ptr [rbp + 824], rax
                                                                                        jmp   xchain0_n30_α
.Lx30_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN global
xchain0_n30_α:
                        mov              rsi, qword ptr [rbp + 816]
                        mov              rdx, qword ptr [rbp + 824]
                        mov              rdi, qword ptr [rip + .Lx31_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 800], rax
                        mov              qword ptr [rbp + 808], rdx
                                                                                        jmp   xchain0_n31_α
.Lx31_0:
                        .quad            .Lx31_0_s
.Lx31_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n31_α:
                        mov              qword ptr [rbp + 864], 1
                        mov              rax, qword ptr [rip + .Lx32_0]
                        mov              qword ptr [rbp + 872], rax
                                                                                        jmp   xchain0_n32_α
.Lx32_0:
                        .quad            .Lx32_0_s
.Lx32_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN global
xchain0_n32_α:
                        mov              rsi, qword ptr [rbp + 864]
                        mov              rdx, qword ptr [rbp + 872]
                        mov              rdi, qword ptr [rip + .Lx33_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 848], rax
                        mov              qword ptr [rbp + 856], rdx
                                                                                        jmp   xchain0_n33_α
.Lx33_0:
                        .quad            .Lx33_0_s
.Lx33_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n33_α:
                        mov              qword ptr [rbp + 912], 1
                        mov              rax, qword ptr [rip + .Lx34_0]
                        mov              qword ptr [rbp + 920], rax
                                                                                        jmp   xchain0_n34_α
.Lx34_0:
                        .quad            .Lx34_0_s
.Lx34_0_s:
                        .string          "Z"
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN global
xchain0_n34_α:
                        mov              rsi, qword ptr [rbp + 912]
                        mov              rdx, qword ptr [rbp + 920]
                        mov              rdi, qword ptr [rip + .Lx35_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 896], rax
                        mov              qword ptr [rbp + 904], rdx
                                                                                        jmp   xchain0_n35_α
.Lx35_0:
                        .quad            .Lx35_0_s
.Lx35_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n35_α:
                        mov              qword ptr [rbp + 960], 1
                        mov              rax, qword ptr [rip + .Lx36_0]
                        mov              qword ptr [rbp + 968], rax
                                                                                        jmp   xchain0_n36_α
.Lx36_0:
                        .quad            .Lx36_0_s
.Lx36_0_s:
                        .string          "A"
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN global
xchain0_n36_α:
                        mov              rsi, qword ptr [rbp + 960]
                        mov              rdx, qword ptr [rbp + 968]
                        mov              rdi, qword ptr [rip + .Lx37_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 944], rax
                        mov              qword ptr [rbp + 952], rdx
                                                                                        jmp   xchain0_n37_α
.Lx37_0:
                        .quad            .Lx37_0_s
.Lx37_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n37_α:
                        mov              qword ptr [rbp + 1008], 1
                        mov              rax, qword ptr [rip + .Lx38_0]
                        mov              qword ptr [rbp + 1016], rax
                                                                                        jmp   xchain0_n38_α
.Lx38_0:
                        .quad            .Lx38_0_s
.Lx38_0_s:
                        .string          "AZ"
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN global
xchain0_n38_α:
                        mov              rsi, qword ptr [rbp + 1008]
                        mov              rdx, qword ptr [rbp + 1016]
                        mov              rdi, qword ptr [rip + .Lx39_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 992], rax
                        mov              qword ptr [rbp + 1000], rdx
                                                                                        jmp   xchain0_n39_α
.Lx39_0:
                        .quad            .Lx39_0_s
.Lx39_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_INTEGER
xchain0_n39_α:
                        mov              qword ptr [rbp + 1056], 6
                        mov              rax, qword ptr [rip + .Lx40_0]
                        mov              qword ptr [rbp + 1064], rax
                                                                                        jmp   xchain0_n40_α
.Lx40_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN global
xchain0_n40_α:
                        mov              rsi, qword ptr [rbp + 1056]
                        mov              rdx, qword ptr [rbp + 1064]
                        mov              rdi, qword ptr [rip + .Lx41_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 1040], rax
                        mov              qword ptr [rbp + 1048], rdx
                                                                                        jmp   xchain0_n41_α
.Lx41_0:
                        .quad            .Lx41_0_s
.Lx41_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_INTEGER
xchain0_n41_α:
                        mov              qword ptr [rbp + 1104], 6
                        mov              rax, qword ptr [rip + .Lx42_0]
                        mov              qword ptr [rbp + 1112], rax
                                                                                        jmp   xchain0_n42_α
.Lx42_0:
                        .quad            7
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN global
xchain0_n42_α:
                        mov              rsi, qword ptr [rbp + 1104]
                        mov              rdx, qword ptr [rbp + 1112]
                        mov              rdi, qword ptr [rip + .Lx43_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 1088], rax
                        mov              qword ptr [rbp + 1096], rdx
                                                                                        jmp   xchain0_n43_α
.Lx43_0:
                        .quad            .Lx43_0_s
.Lx43_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_INTEGER
xchain0_n43_α:
                        mov              qword ptr [rbp + 1152], 6
                        mov              rax, qword ptr [rip + .Lx44_0]
                        mov              qword ptr [rbp + 1160], rax
                                                                                        jmp   xchain0_n44_α
.Lx44_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN global
xchain0_n44_α:
                        mov              rsi, qword ptr [rbp + 1152]
                        mov              rdx, qword ptr [rbp + 1160]
                        mov              rdi, qword ptr [rip + .Lx45_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 1136], rax
                        mov              qword ptr [rbp + 1144], rdx
                                                                                        jmp   xchain0_n45_α
.Lx45_0:
                        .quad            .Lx45_0_s
.Lx45_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_INTEGER
xchain0_n45_α:
                        mov              qword ptr [rbp + 1200], 6
                        mov              rax, qword ptr [rip + .Lx46_0]
                        mov              qword ptr [rbp + 1208], rax
                                                                                        jmp   xchain0_n46_α
.Lx46_0:
                        .quad            7
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN global
xchain0_n46_α:
                        mov              rsi, qword ptr [rbp + 1200]
                        mov              rdx, qword ptr [rbp + 1208]
                        mov              rdi, qword ptr [rip + .Lx47_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 1184], rax
                        mov              qword ptr [rbp + 1192], rdx
                                                                                        jmp   main_γ
.Lx47_0:
                        .quad            .Lx47_0_s
.Lx47_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        mov              rsp, qword ptr [rbp + 1240]
                        mov              rbp, [rsp + 65536]
                        add              rsp, 65544
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              rsp, qword ptr [rbp + 1240]
                        mov              dword ptr [rsp+0], 99
                        mov              dword ptr [rsp+4], 0
                        mov              qword ptr [rsp+8], 0
                        mov              eax, 99
                        xor              edx, edx
                        mov              rbp, [rsp + 65536]
                        add              rsp, 65544
                        ret
                        .section         .note.GNU-stack,"",@progbits

                        .intel_syntax    noprefix
                        .text
                        .section         .rodata
.Lgvan0:                .string          "output"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        mov              edi, 1
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 1
                        call             gva_register@PLT
                        xor              esi, esi
                        call             main_α
                        xor              eax, eax
                        add              rsp, 24
                        ret
main_α:
#=======================================================================================================================
                        .global          main_α
                        .global          main_β
                        .global          main_γ
                        .global          main_ω
                        sub              rsp, 65544
                        mov              rdi, rsp
                        mov              ecx, 65544
                        xor              eax, eax
                        rep stosb
                        mov              qword ptr [rsp + 1800], rsp
                        mov              r12, qword ptr [1879048192]
                        mov              [rsp + 65536], rbp
                        mov              rbp, rsp
main_α_body:
# IR_LIT_INTEGER
xchain0_n0_α:
                        mov              qword ptr [rbp + 160], 6
                        mov              rax, qword ptr [rip + .Lx1_0]
                        mov              qword ptr [rbp + 168], rax
                                                                                        jmp   xchain0_n1_α
.Lx1_0:
                        .quad            5
# IR_LIT_INTEGER
xchain0_n1_α:
                        mov              qword ptr [rbp + 192], 6
                        mov              rax, qword ptr [rip + .Lx2_0]
                        mov              qword ptr [rbp + 200], rax
                                                                                        jmp   xchain0_n2_α
.Lx2_0:
                        .quad            5
xchain0_n2_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+160] -> [zr+112]
                        mov              rax, qword ptr [rbp + 160]
                        mov              qword ptr [rbp + 112], rax
                        mov              rax, qword ptr [rbp + 168]
                        mov              qword ptr [rbp + 120], rax
# marshal arg1 = producer-box slot [zr+192] -> [zr+128]
                        mov              rax, qword ptr [rbp + 192]
                        mov              qword ptr [rbp + 128], rax
                        mov              rax, qword ptr [rbp + 200]
                        mov              qword ptr [rbp + 136], rax
                        .section         .rodata
.Lbynamefn3:            .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn3]
                        lea              rsi, [rbp + 112]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 96], rax
                        mov              qword ptr [rbp + 104], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n4_α
                                                                                        jmp   xchain0_n3_α
xchain0_n2_β:
                                                                                        jmp   xchain0_n4_α
#         output = 'FAIL 410/001: 3+2'                   :(end)
# IR_LIT_STRING
xchain0_n3_α:
                        mov              qword ptr [rbp + 240], 1
                        mov              rax, qword ptr [rip + .Lx4_0]
                        mov              qword ptr [rbp + 248], rax
                                                                                        jmp   xchain0_n5_α
.Lx4_0:
                        .quad            .Lx4_0_s
.Lx4_0_s:
                        .string          "FAIL 410/001: 3+2"
# IR_LIT_INTEGER
xchain0_n4_α:
                        mov              qword ptr [rbp + 336], 6
                        mov              rax, qword ptr [rip + .Lx5_0]
                        mov              qword ptr [rbp + 344], rax
                                                                                        jmp   xchain0_n6_α
.Lx5_0:
                        .quad            1
# IR_ASSIGN gva
xchain0_n5_α:
                        mov              rax, qword ptr [rbp + 240]
                        mov              rdx, qword ptr [rbp + 248]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        mov              qword ptr [rbp + 224], rax
                        mov              qword ptr [rbp + 232], rdx
                                                                                        jmp   main_γ
# IR_LIT_INTEGER
xchain0_n6_α:
                        mov              qword ptr [rbp + 368], 6
                        mov              rax, qword ptr [rip + .Lx7_0]
                        mov              qword ptr [rbp + 376], rax
                                                                                        jmp   xchain0_n7_α
.Lx7_0:
                        .quad            1
xchain0_n7_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+336] -> [zr+288]
                        mov              rax, qword ptr [rbp + 336]
                        mov              qword ptr [rbp + 288], rax
                        mov              rax, qword ptr [rbp + 344]
                        mov              qword ptr [rbp + 296], rax
# marshal arg1 = producer-box slot [zr+368] -> [zr+304]
                        mov              rax, qword ptr [rbp + 368]
                        mov              qword ptr [rbp + 304], rax
                        mov              rax, qword ptr [rbp + 376]
                        mov              qword ptr [rbp + 312], rax
                        .section         .rodata
.Lbynamefn8:            .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn8]
                        lea              rsi, [rbp + 288]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 272], rax
                        mov              qword ptr [rbp + 280], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n9_α
                                                                                        jmp   xchain0_n8_α
xchain0_n7_β:
                                                                                        jmp   xchain0_n9_α
#         output = 'FAIL 410/002: 3-2'                   :(end)
# IR_LIT_STRING
xchain0_n8_α:
                        mov              qword ptr [rbp + 416], 1
                        mov              rax, qword ptr [rip + .Lx9_0]
                        mov              qword ptr [rbp + 424], rax
                                                                                        jmp   xchain0_n10_α
.Lx9_0:
                        .quad            .Lx9_0_s
.Lx9_0_s:
                        .string          "FAIL 410/002: 3-2"
# IR_LIT_INTEGER
xchain0_n9_α:
                        mov              qword ptr [rbp + 512], 6
                        mov              rax, qword ptr [rip + .Lx10_0]
                        mov              qword ptr [rbp + 520], rax
                                                                                        jmp   xchain0_n11_α
.Lx10_0:
                        .quad            6
# IR_ASSIGN gva
xchain0_n10_α:
                        mov              rax, qword ptr [rbp + 416]
                        mov              rdx, qword ptr [rbp + 424]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        mov              qword ptr [rbp + 400], rax
                        mov              qword ptr [rbp + 408], rdx
                                                                                        jmp   main_γ
# IR_LIT_INTEGER
xchain0_n11_α:
                        mov              qword ptr [rbp + 544], 6
                        mov              rax, qword ptr [rip + .Lx12_0]
                        mov              qword ptr [rbp + 552], rax
                                                                                        jmp   xchain0_n12_α
.Lx12_0:
                        .quad            6
xchain0_n12_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+512] -> [zr+464]
                        mov              rax, qword ptr [rbp + 512]
                        mov              qword ptr [rbp + 464], rax
                        mov              rax, qword ptr [rbp + 520]
                        mov              qword ptr [rbp + 472], rax
# marshal arg1 = producer-box slot [zr+544] -> [zr+480]
                        mov              rax, qword ptr [rbp + 544]
                        mov              qword ptr [rbp + 480], rax
                        mov              rax, qword ptr [rbp + 552]
                        mov              qword ptr [rbp + 488], rax
                        .section         .rodata
.Lbynamefn13:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn13]
                        lea              rsi, [rbp + 464]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 448], rax
                        mov              qword ptr [rbp + 456], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n14_α
                                                                                        jmp   xchain0_n13_α
xchain0_n12_β:
                                                                                        jmp   xchain0_n14_α
#         output = 'FAIL 410/003: 3*2'                   :(end)
# IR_LIT_STRING
xchain0_n13_α:
                        mov              qword ptr [rbp + 592], 1
                        mov              rax, qword ptr [rip + .Lx14_0]
                        mov              qword ptr [rbp + 600], rax
                                                                                        jmp   xchain0_n15_α
.Lx14_0:
                        .quad            .Lx14_0_s
.Lx14_0_s:
                        .string          "FAIL 410/003: 3*2"
# IR_LIT_INTEGER
xchain0_n14_α:
                        mov              qword ptr [rbp + 688], 6
                        mov              rax, qword ptr [rip + .Lx15_0]
                        mov              qword ptr [rbp + 696], rax
                                                                                        jmp   xchain0_n16_α
.Lx15_0:
                        .quad            2
# IR_ASSIGN gva
xchain0_n15_α:
                        mov              rax, qword ptr [rbp + 592]
                        mov              rdx, qword ptr [rbp + 600]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        mov              qword ptr [rbp + 576], rax
                        mov              qword ptr [rbp + 584], rdx
                                                                                        jmp   main_γ
# IR_LIT_INTEGER
xchain0_n16_α:
                        mov              qword ptr [rbp + 720], 6
                        mov              rax, qword ptr [rip + .Lx17_0]
                        mov              qword ptr [rbp + 728], rax
                                                                                        jmp   xchain0_n17_α
.Lx17_0:
                        .quad            2
xchain0_n17_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+688] -> [zr+640]
                        mov              rax, qword ptr [rbp + 688]
                        mov              qword ptr [rbp + 640], rax
                        mov              rax, qword ptr [rbp + 696]
                        mov              qword ptr [rbp + 648], rax
# marshal arg1 = producer-box slot [zr+720] -> [zr+656]
                        mov              rax, qword ptr [rbp + 720]
                        mov              qword ptr [rbp + 656], rax
                        mov              rax, qword ptr [rbp + 728]
                        mov              qword ptr [rbp + 664], rax
                        .section         .rodata
.Lbynamefn18:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn18]
                        lea              rsi, [rbp + 640]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 624], rax
                        mov              qword ptr [rbp + 632], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n19_α
                                                                                        jmp   xchain0_n18_α
xchain0_n17_β:
                                                                                        jmp   xchain0_n19_α
#         output = 'FAIL 410/004: 5/2 integer division'  :(end)
# IR_LIT_STRING
xchain0_n18_α:
                        mov              qword ptr [rbp + 768], 1
                        mov              rax, qword ptr [rip + .Lx19_0]
                        mov              qword ptr [rbp + 776], rax
                                                                                        jmp   xchain0_n20_α
.Lx19_0:
                        .quad            .Lx19_0_s
.Lx19_0_s:
                        .string          "FAIL 410/004: 5/2 integer division"
#         differ(2 ** 3, 8)                   :f(e005)
# IR_LIT_INTEGER
xchain0_n19_α:
                        mov              qword ptr [rbp + 896], 6
                        mov              rax, qword ptr [rip + .Lx20_0]
                        mov              qword ptr [rbp + 904], rax
                                                                                        jmp   xchain0_n21_α
.Lx20_0:
                        .quad            2
# IR_ASSIGN gva
xchain0_n20_α:
                        mov              rax, qword ptr [rbp + 768]
                        mov              rdx, qword ptr [rbp + 776]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        mov              qword ptr [rbp + 752], rax
                        mov              qword ptr [rbp + 760], rdx
                                                                                        jmp   main_γ
# IR_LIT_INTEGER
xchain0_n21_α:
                        mov              qword ptr [rbp + 928], 6
                        mov              rax, qword ptr [rip + .Lx22_0]
                        mov              qword ptr [rbp + 936], rax
                                                                                        jmp   xchain0_n22_α
.Lx22_0:
                        .quad            3
xchain0_n22_α:
# IR_BINOP_ARITH_REAL
                        mov              rdi, qword ptr [rbp + 896]
                        mov              rsi, qword ptr [rbp + 904]
                        mov              rdx, qword ptr [rbp + 928]
                        mov              rcx, qword ptr [rbp + 936]
                        mov              r8d, 18
                        call             rt_num_arith@PLT
                        cmp              eax, 99
                                                                                        je    xchain0_n24_α
                        mov              qword ptr [rbp + 864], rax
                        mov              qword ptr [rbp + 872], rdx
                                                                                        jmp   xchain0_n23_α
# IR_LIT_INTEGER
xchain0_n23_α:
                        mov              qword ptr [rbp + 960], 6
                        mov              rax, qword ptr [rip + .Lx24_0]
                        mov              qword ptr [rbp + 968], rax
                                                                                        jmp   xchain0_n25_α
.Lx24_0:
                        .quad            8
# IR_LIT_INTEGER
xchain0_n24_α:
                        mov              qword ptr [rbp + 1104], 6
                        mov              rax, qword ptr [rip + .Lx25_0]
                        mov              qword ptr [rbp + 1112], rax
                                                                                        jmp   xchain0_n26_α
.Lx25_0:
                        .quad            5
xchain0_n25_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+864] -> [zr+816]
                        mov              rax, qword ptr [rbp + 864]
                        mov              qword ptr [rbp + 816], rax
                        mov              rax, qword ptr [rbp + 872]
                        mov              qword ptr [rbp + 824], rax
# marshal arg1 = producer-box slot [zr+960] -> [zr+832]
                        mov              rax, qword ptr [rbp + 960]
                        mov              qword ptr [rbp + 832], rax
                        mov              rax, qword ptr [rbp + 968]
                        mov              qword ptr [rbp + 840], rax
                        .section         .rodata
.Lbynamefn26:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn26]
                        lea              rsi, [rbp + 816]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 800], rax
                        mov              qword ptr [rbp + 808], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n24_α
                                                                                        jmp   xchain0_n27_α
xchain0_n25_β:
                                                                                        jmp   xchain0_n24_α
# IR_LIT_INTEGER
xchain0_n26_α:
                        mov              qword ptr [rbp + 1136], 6
                        mov              rax, qword ptr [rip + .Lx27_0]
                        mov              qword ptr [rbp + 1144], rax
                                                                                        jmp   xchain0_n28_α
.Lx27_0:
                        .quad            5
#         output = 'FAIL 410/005: 2**3'                  :(end)
# IR_LIT_STRING
xchain0_n27_α:
                        mov              qword ptr [rbp + 1008], 1
                        mov              rax, qword ptr [rip + .Lx28_0]
                        mov              qword ptr [rbp + 1016], rax
                                                                                        jmp   xchain0_n29_α
.Lx28_0:
                        .quad            .Lx28_0_s
.Lx28_0_s:
                        .string          "FAIL 410/005: 2**3"
xchain0_n28_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1104] -> [zr+1056]
                        mov              rax, qword ptr [rbp + 1104]
                        mov              qword ptr [rbp + 1056], rax
                        mov              rax, qword ptr [rbp + 1112]
                        mov              qword ptr [rbp + 1064], rax
# marshal arg1 = producer-box slot [zr+1136] -> [zr+1072]
                        mov              rax, qword ptr [rbp + 1136]
                        mov              qword ptr [rbp + 1072], rax
                        mov              rax, qword ptr [rbp + 1144]
                        mov              qword ptr [rbp + 1080], rax
                        .section         .rodata
.Lbynamefn29:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn29]
                        lea              rsi, [rbp + 1056]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1040], rax
                        mov              qword ptr [rbp + 1048], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n31_α
                                                                                        jmp   xchain0_n30_α
xchain0_n28_β:
                                                                                        jmp   xchain0_n31_α
# IR_ASSIGN gva
xchain0_n29_α:
                        mov              rax, qword ptr [rbp + 1008]
                        mov              rdx, qword ptr [rbp + 1016]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        mov              qword ptr [rbp + 992], rax
                        mov              qword ptr [rbp + 1000], rdx
                                                                                        jmp   main_γ
#         output = 'FAIL 410/006: string+int coerce'     :(end)
# IR_LIT_STRING
xchain0_n30_α:
                        mov              qword ptr [rbp + 1184], 1
                        mov              rax, qword ptr [rip + .Lx31_0]
                        mov              qword ptr [rbp + 1192], rax
                                                                                        jmp   xchain0_n32_α
.Lx31_0:
                        .quad            .Lx31_0_s
.Lx31_0_s:
                        .string          "FAIL 410/006: string+int coerce"
# IR_LIT_INTEGER
xchain0_n31_α:
                        mov              qword ptr [rbp + 1280], 6
                        mov              rax, qword ptr [rip + .Lx32_0]
                        mov              qword ptr [rbp + 1288], rax
                                                                                        jmp   xchain0_n33_α
.Lx32_0:
                        .quad            1
# IR_ASSIGN gva
xchain0_n32_α:
                        mov              rax, qword ptr [rbp + 1184]
                        mov              rdx, qword ptr [rbp + 1192]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        mov              qword ptr [rbp + 1168], rax
                        mov              qword ptr [rbp + 1176], rdx
                                                                                        jmp   main_γ
# IR_LIT_INTEGER
xchain0_n33_α:
                        mov              qword ptr [rbp + 1312], 6
                        mov              rax, qword ptr [rip + .Lx34_0]
                        mov              qword ptr [rbp + 1320], rax
                                                                                        jmp   xchain0_n34_α
.Lx34_0:
                        .quad            1
xchain0_n34_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1280] -> [zr+1232]
                        mov              rax, qword ptr [rbp + 1280]
                        mov              qword ptr [rbp + 1232], rax
                        mov              rax, qword ptr [rbp + 1288]
                        mov              qword ptr [rbp + 1240], rax
# marshal arg1 = producer-box slot [zr+1312] -> [zr+1248]
                        mov              rax, qword ptr [rbp + 1312]
                        mov              qword ptr [rbp + 1248], rax
                        mov              rax, qword ptr [rbp + 1320]
                        mov              qword ptr [rbp + 1256], rax
                        .section         .rodata
.Lbynamefn35:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn35]
                        lea              rsi, [rbp + 1232]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1216], rax
                        mov              qword ptr [rbp + 1224], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n36_α
                                                                                        jmp   xchain0_n35_α
xchain0_n34_β:
                                                                                        jmp   xchain0_n36_α
#         output = 'FAIL 410/007: int+neg-string'        :(end)
# IR_LIT_STRING
xchain0_n35_α:
                        mov              qword ptr [rbp + 1360], 1
                        mov              rax, qword ptr [rip + .Lx36_0]
                        mov              qword ptr [rbp + 1368], rax
                                                                                        jmp   xchain0_n37_α
.Lx36_0:
                        .quad            .Lx36_0_s
.Lx36_0_s:
                        .string          "FAIL 410/007: int+neg-string"
# IR_LIT_INTEGER
xchain0_n36_α:
                        mov              qword ptr [rbp + 1456], 6
                        mov              rax, qword ptr [rip + .Lx37_0]
                        mov              qword ptr [rbp + 1464], rax
                                                                                        jmp   xchain0_n38_α
.Lx37_0:
                        .quad            1
# IR_ASSIGN gva
xchain0_n37_α:
                        mov              rax, qword ptr [rbp + 1360]
                        mov              rdx, qword ptr [rbp + 1368]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        mov              qword ptr [rbp + 1344], rax
                        mov              qword ptr [rbp + 1352], rdx
                                                                                        jmp   main_γ
# IR_LIT_INTEGER
xchain0_n38_α:
                        mov              qword ptr [rbp + 1488], 6
                        mov              rax, qword ptr [rip + .Lx39_0]
                        mov              qword ptr [rbp + 1496], rax
                                                                                        jmp   xchain0_n39_α
.Lx39_0:
                        .quad            1
xchain0_n39_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1456] -> [zr+1408]
                        mov              rax, qword ptr [rbp + 1456]
                        mov              qword ptr [rbp + 1408], rax
                        mov              rax, qword ptr [rbp + 1464]
                        mov              qword ptr [rbp + 1416], rax
# marshal arg1 = producer-box slot [zr+1488] -> [zr+1424]
                        mov              rax, qword ptr [rbp + 1488]
                        mov              qword ptr [rbp + 1424], rax
                        mov              rax, qword ptr [rbp + 1496]
                        mov              qword ptr [rbp + 1432], rax
                        .section         .rodata
.Lbynamefn40:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn40]
                        lea              rsi, [rbp + 1408]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1392], rax
                        mov              qword ptr [rbp + 1400], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n41_α
                                                                                        jmp   xchain0_n40_α
xchain0_n39_β:
                                                                                        jmp   xchain0_n41_α
#         output = 'FAIL 410/008: string+string coerce'  :(end)
# IR_LIT_STRING
xchain0_n40_α:
                        mov              qword ptr [rbp + 1536], 1
                        mov              rax, qword ptr [rip + .Lx41_0]
                        mov              qword ptr [rbp + 1544], rax
                                                                                        jmp   xchain0_n42_α
.Lx41_0:
                        .quad            .Lx41_0_s
.Lx41_0_s:
                        .string          "FAIL 410/008: string+string coerce"
# IR_LIT_INTEGER
xchain0_n41_α:
                        mov              qword ptr [rbp + 1632], 6
                        mov              rax, qword ptr [rip + .Lx42_0]
                        mov              qword ptr [rbp + 1640], rax
                                                                                        jmp   xchain0_n43_α
.Lx42_0:
                        .quad            5
# IR_ASSIGN gva
xchain0_n42_α:
                        mov              rax, qword ptr [rbp + 1536]
                        mov              rdx, qword ptr [rbp + 1544]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        mov              qword ptr [rbp + 1520], rax
                        mov              qword ptr [rbp + 1528], rdx
                                                                                        jmp   main_γ
# IR_LIT_INTEGER
xchain0_n43_α:
                        mov              qword ptr [rbp + 1664], 6
                        mov              rax, qword ptr [rip + .Lx44_0]
                        mov              qword ptr [rbp + 1672], rax
                                                                                        jmp   xchain0_n44_α
.Lx44_0:
                        .quad            5
xchain0_n44_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1632] -> [zr+1584]
                        mov              rax, qword ptr [rbp + 1632]
                        mov              qword ptr [rbp + 1584], rax
                        mov              rax, qword ptr [rbp + 1640]
                        mov              qword ptr [rbp + 1592], rax
# marshal arg1 = producer-box slot [zr+1664] -> [zr+1600]
                        mov              rax, qword ptr [rbp + 1664]
                        mov              qword ptr [rbp + 1600], rax
                        mov              rax, qword ptr [rbp + 1672]
                        mov              qword ptr [rbp + 1608], rax
                        .section         .rodata
.Lbynamefn45:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn45]
                        lea              rsi, [rbp + 1584]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1568], rax
                        mov              qword ptr [rbp + 1576], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n46_α
                                                                                        jmp   xchain0_n45_α
xchain0_n44_β:
                                                                                        jmp   xchain0_n46_α
#         output = 'FAIL 410/009: null addend is zero'   :(end)
# IR_LIT_STRING
xchain0_n45_α:
                        mov              qword ptr [rbp + 1712], 1
                        mov              rax, qword ptr [rip + .Lx46_0]
                        mov              qword ptr [rbp + 1720], rax
                                                                                        jmp   xchain0_n47_α
.Lx46_0:
                        .quad            .Lx46_0_s
.Lx46_0_s:
                        .string          "FAIL 410/009: null addend is zero"
#         output = 'PASS 410_arith_int (9/9)'
# IR_LIT_STRING
xchain0_n46_α:
                        mov              qword ptr [rbp + 1760], 1
                        mov              rax, qword ptr [rip + .Lx47_0]
                        mov              qword ptr [rbp + 1768], rax
                                                                                        jmp   xchain0_n48_α
.Lx47_0:
                        .quad            .Lx47_0_s
.Lx47_0_s:
                        .string          "PASS 410_arith_int (9/9)"
# IR_ASSIGN gva
xchain0_n47_α:
                        mov              rax, qword ptr [rbp + 1712]
                        mov              rdx, qword ptr [rbp + 1720]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        mov              qword ptr [rbp + 1696], rax
                        mov              qword ptr [rbp + 1704], rdx
                                                                                        jmp   main_γ
# IR_ASSIGN gva
xchain0_n48_α:
                        mov              rax, qword ptr [rbp + 1760]
                        mov              rdx, qword ptr [rbp + 1768]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        mov              qword ptr [rbp + 1744], rax
                        mov              qword ptr [rbp + 1752], rdx
                                                                                        jmp   main_γ
main_β:
                                                                                        jmp   main_ω
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        mov              rsp, qword ptr [rbp + 1800]
                        mov              rbp, [rsp + 65536]
                        add              rsp, 65544
                        ret
main_ω:
                        mov              rsp, qword ptr [rbp + 1800]
                        mov              dword ptr [rsp+0], 99
                        mov              dword ptr [rsp+4], 0
                        mov              qword ptr [rsp+8], 0
                        mov              eax, 99
                        xor              edx, edx
                        mov              rbp, [rsp + 65536]
                        add              rsp, 65544
                        ret

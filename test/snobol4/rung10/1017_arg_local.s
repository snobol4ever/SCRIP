                        .intel_syntax    noprefix
                        .text
                        .section         .rodata
.Lgvan0:                .string          "jlab"
.Lgvan1:                .string          "output"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .quad            .Lgvan1
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        mov              edi, 2
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 2
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
                        sub              rsp, 2408
                        mov              rdi, rsp
                        mov              ecx, 2408
                        xor              eax, eax
                        rep stosb
                        mov              r12, qword ptr [1879048192]
                        mov              [rsp + 2400], rbp
                        mov              rbp, rsp
main_α_body:
#=======================================================================================================================
# jlab    define('jlab(a,b,c)d,e,f')
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n0_α:
                        mov              qword ptr [rbp + 144], 1
                        mov              rax, qword ptr [rip + .Lx1_0]
                        mov              qword ptr [rbp + 152], rax
                                                                                        jmp   xchain0_n1_α
.Lx1_0:
                        .quad            .Lx1_0_s
.Lx1_0_s:
                        .string          "jlab(a,b,c)d,e,f"
#-----------------------------------------------------------------------------------------------------------------------
xchain0_n1_α:
# BOX CALL define(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+144] -> [zr+112]
                        mov              rax, qword ptr [rbp + 144]
                        mov              qword ptr [rbp + 112], rax
                        mov              rax, qword ptr [rbp + 152]
                        mov              qword ptr [rbp + 120], rax
                        .section         .rodata
.Lbynamefn2:            .string          "define"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn2]
                        lea              rsi, [rbp + 112]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 96], rax
                        mov              qword ptr [rbp + 104], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n2_α
                                                                                        jmp   xchain0_n2_α
xchain0_n1_β:
                                                                                        jmp   xchain0_n2_α
#=======================================================================================================================
#         differ(arg(.jlab, 1), 'A')                   :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n2_α:
                        mov              qword ptr [rbp + 352], 1
                        mov              rax, qword ptr [rip + .Lx3_0]
                        mov              qword ptr [rbp + 360], rax
                                                                                        jmp   xchain0_n3_α
.Lx3_0:
                        .quad            .Lx3_0_s
.Lx3_0_s:
                        .string          "jlab"
#-----------------------------------------------------------------------------------------------------------------------
xchain0_n3_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+352] -> [zr+320]
                        mov              rax, qword ptr [rbp + 352]
                        mov              qword ptr [rbp + 320], rax
                        mov              rax, qword ptr [rbp + 360]
                        mov              qword ptr [rbp + 328], rax
                        .section         .rodata
.Lrkfn5:                .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn5]
                        lea              rsi, [rbp + 320]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 304], rax
                        mov              qword ptr [rbp + 312], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n5_α
                                                                                        jmp   xchain0_n4_α
xchain0_n3_β:
                                                                                        jmp   xchain0_n5_α
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_INTEGER
xchain0_n4_α:
                        mov              qword ptr [rbp + 384], 6
                        mov              rax, qword ptr [rip + .Lx6_0]
                        mov              qword ptr [rbp + 392], rax
                                                                                        jmp   xchain0_n6_α
.Lx6_0:
                        .quad            1
#=======================================================================================================================
#         differ(arg(.jlab, 3), 'C')                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n5_α:
                        mov              qword ptr [rbp + 672], 1
                        mov              rax, qword ptr [rip + .Lx7_0]
                        mov              qword ptr [rbp + 680], rax
                                                                                        jmp   xchain0_n7_α
.Lx7_0:
                        .quad            .Lx7_0_s
.Lx7_0_s:
                        .string          "jlab"
#-----------------------------------------------------------------------------------------------------------------------
xchain0_n6_α:
# BOX CALL arg(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+304] -> [zr+256]
                        mov              rax, qword ptr [rbp + 304]
                        mov              qword ptr [rbp + 256], rax
                        mov              rax, qword ptr [rbp + 312]
                        mov              qword ptr [rbp + 264], rax
# marshal arg1 = producer-box slot [zr+384] -> [zr+272]
                        mov              rax, qword ptr [rbp + 384]
                        mov              qword ptr [rbp + 272], rax
                        mov              rax, qword ptr [rbp + 392]
                        mov              qword ptr [rbp + 280], rax
                        .section         .rodata
.Lbynamefn7:            .string          "arg"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn7]
                        lea              rsi, [rbp + 256]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 240], rax
                        mov              qword ptr [rbp + 248], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n5_α
                                                                                        jmp   xchain0_n8_α
xchain0_n6_β:
                                                                                        jmp   xchain0_n5_α
#-----------------------------------------------------------------------------------------------------------------------
xchain0_n7_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+672] -> [zr+640]
                        mov              rax, qword ptr [rbp + 672]
                        mov              qword ptr [rbp + 640], rax
                        mov              rax, qword ptr [rbp + 680]
                        mov              qword ptr [rbp + 648], rax
                        .section         .rodata
.Lrkfn10:               .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn10]
                        lea              rsi, [rbp + 640]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 624], rax
                        mov              qword ptr [rbp + 632], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n10_α
                                                                                        jmp   xchain0_n9_α
xchain0_n7_β:
                                                                                        jmp   xchain0_n10_α
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n8_α:
                        mov              qword ptr [rbp + 416], 1
                        mov              rax, qword ptr [rip + .Lx11_0]
                        mov              qword ptr [rbp + 424], rax
                                                                                        jmp   xchain0_n11_α
.Lx11_0:
                        .quad            .Lx11_0_s
.Lx11_0_s:
                        .string          "A"
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_INTEGER
xchain0_n9_α:
                        mov              qword ptr [rbp + 704], 6
                        mov              rax, qword ptr [rip + .Lx12_0]
                        mov              qword ptr [rbp + 712], rax
                                                                                        jmp   xchain0_n12_α
.Lx12_0:
                        .quad            3
#=======================================================================================================================
#         arg(.jlab, 0)              :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n10_α:
                        mov              qword ptr [rbp + 928], 1
                        mov              rax, qword ptr [rip + .Lx13_0]
                        mov              qword ptr [rbp + 936], rax
                                                                                        jmp   xchain0_n13_α
.Lx13_0:
                        .quad            .Lx13_0_s
.Lx13_0_s:
                        .string          "jlab"
#-----------------------------------------------------------------------------------------------------------------------
xchain0_n11_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+240] -> [zr+192]
                        mov              rax, qword ptr [rbp + 240]
                        mov              qword ptr [rbp + 192], rax
                        mov              rax, qword ptr [rbp + 248]
                        mov              qword ptr [rbp + 200], rax
# marshal arg1 = producer-box slot [zr+416] -> [zr+208]
                        mov              rax, qword ptr [rbp + 416]
                        mov              qword ptr [rbp + 208], rax
                        mov              rax, qword ptr [rbp + 424]
                        mov              qword ptr [rbp + 216], rax
                        .section         .rodata
.Lbynamefn12:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn12]
                        lea              rsi, [rbp + 192]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 176], rax
                        mov              qword ptr [rbp + 184], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n5_α
                                                                                        jmp   xchain0_n14_α
xchain0_n11_β:
                                                                                        jmp   xchain0_n5_α
#-----------------------------------------------------------------------------------------------------------------------
xchain0_n12_α:
# BOX CALL arg(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+624] -> [zr+576]
                        mov              rax, qword ptr [rbp + 624]
                        mov              qword ptr [rbp + 576], rax
                        mov              rax, qword ptr [rbp + 632]
                        mov              qword ptr [rbp + 584], rax
# marshal arg1 = producer-box slot [zr+704] -> [zr+592]
                        mov              rax, qword ptr [rbp + 704]
                        mov              qword ptr [rbp + 592], rax
                        mov              rax, qword ptr [rbp + 712]
                        mov              qword ptr [rbp + 600], rax
                        .section         .rodata
.Lbynamefn13:           .string          "arg"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn13]
                        lea              rsi, [rbp + 576]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 560], rax
                        mov              qword ptr [rbp + 568], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n10_α
                                                                                        jmp   xchain0_n15_α
xchain0_n12_β:
                                                                                        jmp   xchain0_n10_α
#-----------------------------------------------------------------------------------------------------------------------
xchain0_n13_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+928] -> [zr+896]
                        mov              rax, qword ptr [rbp + 928]
                        mov              qword ptr [rbp + 896], rax
                        mov              rax, qword ptr [rbp + 936]
                        mov              qword ptr [rbp + 904], rax
                        .section         .rodata
.Lrkfn17:               .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn17]
                        lea              rsi, [rbp + 896]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 880], rax
                        mov              qword ptr [rbp + 888], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n17_α
                                                                                        jmp   xchain0_n16_α
xchain0_n13_β:
                                                                                        jmp   xchain0_n17_α
#=======================================================================================================================
#         output = 'FAIL 1017/001: arg(.jlab,1) = A'     :(end)
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n14_α:
                        mov              qword ptr [rbp + 464], 1
                        mov              rax, qword ptr [rip + .Lx18_0]
                        mov              qword ptr [rbp + 472], rax
                                                                                        jmp   xchain0_n18_α
.Lx18_0:
                        .quad            .Lx18_0_s
.Lx18_0_s:
                        .string          "FAIL 1017/001: arg(.jlab,1) = A"
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n15_α:
                        mov              qword ptr [rbp + 736], 1
                        mov              rax, qword ptr [rip + .Lx19_0]
                        mov              qword ptr [rbp + 744], rax
                                                                                        jmp   xchain0_n19_α
.Lx19_0:
                        .quad            .Lx19_0_s
.Lx19_0_s:
                        .string          "C"
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_INTEGER
xchain0_n16_α:
                        mov              qword ptr [rbp + 960], 6
                        mov              rax, qword ptr [rip + .Lx20_0]
                        mov              qword ptr [rbp + 968], rax
                                                                                        jmp   xchain0_n20_α
.Lx20_0:
                        .quad            0
#=======================================================================================================================
#         arg(.jlab, 4)              :f(e004)
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n17_α:
                        mov              qword ptr [rbp + 1152], 1
                        mov              rax, qword ptr [rip + .Lx21_0]
                        mov              qword ptr [rbp + 1160], rax
                                                                                        jmp   xchain0_n21_α
.Lx21_0:
                        .quad            .Lx21_0_s
.Lx21_0_s:
                        .string          "jlab"
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN gva
xchain0_n18_α:
                        mov              rax, qword ptr [rbp + 464]
                        mov              rdx, qword ptr [rbp + 472]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        mov              qword ptr [rbp + 448], rax
                        mov              qword ptr [rbp + 456], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
xchain0_n19_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+560] -> [zr+512]
                        mov              rax, qword ptr [rbp + 560]
                        mov              qword ptr [rbp + 512], rax
                        mov              rax, qword ptr [rbp + 568]
                        mov              qword ptr [rbp + 520], rax
# marshal arg1 = producer-box slot [zr+736] -> [zr+528]
                        mov              rax, qword ptr [rbp + 736]
                        mov              qword ptr [rbp + 528], rax
                        mov              rax, qword ptr [rbp + 744]
                        mov              qword ptr [rbp + 536], rax
                        .section         .rodata
.Lbynamefn20:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn20]
                        lea              rsi, [rbp + 512]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 496], rax
                        mov              qword ptr [rbp + 504], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n10_α
                                                                                        jmp   xchain0_n22_α
xchain0_n19_β:
                                                                                        jmp   xchain0_n10_α
#-----------------------------------------------------------------------------------------------------------------------
xchain0_n20_α:
# BOX CALL arg(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+880] -> [zr+832]
                        mov              rax, qword ptr [rbp + 880]
                        mov              qword ptr [rbp + 832], rax
                        mov              rax, qword ptr [rbp + 888]
                        mov              qword ptr [rbp + 840], rax
# marshal arg1 = producer-box slot [zr+960] -> [zr+848]
                        mov              rax, qword ptr [rbp + 960]
                        mov              qword ptr [rbp + 848], rax
                        mov              rax, qword ptr [rbp + 968]
                        mov              qword ptr [rbp + 856], rax
                        .section         .rodata
.Lbynamefn21:           .string          "arg"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn21]
                        lea              rsi, [rbp + 832]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 816], rax
                        mov              qword ptr [rbp + 824], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n17_α
                                                                                        jmp   xchain0_n23_α
xchain0_n20_β:
                                                                                        jmp   xchain0_n17_α
#-----------------------------------------------------------------------------------------------------------------------
xchain0_n21_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1152] -> [zr+1120]
                        mov              rax, qword ptr [rbp + 1152]
                        mov              qword ptr [rbp + 1120], rax
                        mov              rax, qword ptr [rbp + 1160]
                        mov              qword ptr [rbp + 1128], rax
                        .section         .rodata
.Lrkfn26:               .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn26]
                        lea              rsi, [rbp + 1120]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1104], rax
                        mov              qword ptr [rbp + 1112], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n25_α
                                                                                        jmp   xchain0_n24_α
xchain0_n21_β:
                                                                                        jmp   xchain0_n25_α
#=======================================================================================================================
#         output = 'FAIL 1017/002: arg(.jlab,3) = C'     :(end)
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n22_α:
                        mov              qword ptr [rbp + 784], 1
                        mov              rax, qword ptr [rip + .Lx27_0]
                        mov              qword ptr [rbp + 792], rax
                                                                                        jmp   xchain0_n26_α
.Lx27_0:
                        .quad            .Lx27_0_s
.Lx27_0_s:
                        .string          "FAIL 1017/002: arg(.jlab,3) = C"
#=======================================================================================================================
#         output = 'FAIL 1017/003: arg(.jlab,0) OOB should fail' :(end)
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n23_α:
                        mov              qword ptr [rbp + 1008], 1
                        mov              rax, qword ptr [rip + .Lx28_0]
                        mov              qword ptr [rbp + 1016], rax
                                                                                        jmp   xchain0_n27_α
.Lx28_0:
                        .quad            .Lx28_0_s
.Lx28_0_s:
                        .string          "FAIL 1017/003: arg(.jlab,0) OOB should fail"
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_INTEGER
xchain0_n24_α:
                        mov              qword ptr [rbp + 1184], 6
                        mov              rax, qword ptr [rip + .Lx29_0]
                        mov              qword ptr [rbp + 1192], rax
                                                                                        jmp   xchain0_n28_α
.Lx29_0:
                        .quad            4
#=======================================================================================================================
#         differ(local(.jlab, 1), 'D')                   :f(e005)
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n25_α:
                        mov              qword ptr [rbp + 1440], 1
                        mov              rax, qword ptr [rip + .Lx30_0]
                        mov              qword ptr [rbp + 1448], rax
                                                                                        jmp   xchain0_n29_α
.Lx30_0:
                        .quad            .Lx30_0_s
.Lx30_0_s:
                        .string          "jlab"
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN gva
xchain0_n26_α:
                        mov              rax, qword ptr [rbp + 784]
                        mov              rdx, qword ptr [rbp + 792]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        mov              qword ptr [rbp + 768], rax
                        mov              qword ptr [rbp + 776], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN gva
xchain0_n27_α:
                        mov              rax, qword ptr [rbp + 1008]
                        mov              rdx, qword ptr [rbp + 1016]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        mov              qword ptr [rbp + 992], rax
                        mov              qword ptr [rbp + 1000], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
xchain0_n28_α:
# BOX CALL arg(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1104] -> [zr+1056]
                        mov              rax, qword ptr [rbp + 1104]
                        mov              qword ptr [rbp + 1056], rax
                        mov              rax, qword ptr [rbp + 1112]
                        mov              qword ptr [rbp + 1064], rax
# marshal arg1 = producer-box slot [zr+1184] -> [zr+1072]
                        mov              rax, qword ptr [rbp + 1184]
                        mov              qword ptr [rbp + 1072], rax
                        mov              rax, qword ptr [rbp + 1192]
                        mov              qword ptr [rbp + 1080], rax
                        .section         .rodata
.Lbynamefn29:           .string          "arg"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn29]
                        lea              rsi, [rbp + 1056]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1040], rax
                        mov              qword ptr [rbp + 1048], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n25_α
                                                                                        jmp   xchain0_n30_α
xchain0_n28_β:
                                                                                        jmp   xchain0_n25_α
#-----------------------------------------------------------------------------------------------------------------------
xchain0_n29_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1440] -> [zr+1408]
                        mov              rax, qword ptr [rbp + 1440]
                        mov              qword ptr [rbp + 1408], rax
                        mov              rax, qword ptr [rbp + 1448]
                        mov              qword ptr [rbp + 1416], rax
                        .section         .rodata
.Lrkfn35:               .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn35]
                        lea              rsi, [rbp + 1408]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1392], rax
                        mov              qword ptr [rbp + 1400], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n32_α
                                                                                        jmp   xchain0_n31_α
xchain0_n29_β:
                                                                                        jmp   xchain0_n32_α
#=======================================================================================================================
#         output = 'FAIL 1017/004: arg(.jlab,4) OOB should fail' :(end)
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n30_α:
                        mov              qword ptr [rbp + 1232], 1
                        mov              rax, qword ptr [rip + .Lx36_0]
                        mov              qword ptr [rbp + 1240], rax
                                                                                        jmp   xchain0_n33_α
.Lx36_0:
                        .quad            .Lx36_0_s
.Lx36_0_s:
                        .string          "FAIL 1017/004: arg(.jlab,4) OOB should fail"
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_INTEGER
xchain0_n31_α:
                        mov              qword ptr [rbp + 1472], 6
                        mov              rax, qword ptr [rip + .Lx37_0]
                        mov              qword ptr [rbp + 1480], rax
                                                                                        jmp   xchain0_n34_α
.Lx37_0:
                        .quad            1
#=======================================================================================================================
#         differ(local(.jlab, 3), 'F')                   :f(e006)
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n32_α:
                        mov              qword ptr [rbp + 1760], 1
                        mov              rax, qword ptr [rip + .Lx38_0]
                        mov              qword ptr [rbp + 1768], rax
                                                                                        jmp   xchain0_n35_α
.Lx38_0:
                        .quad            .Lx38_0_s
.Lx38_0_s:
                        .string          "jlab"
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN gva
xchain0_n33_α:
                        mov              rax, qword ptr [rbp + 1232]
                        mov              rdx, qword ptr [rbp + 1240]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        mov              qword ptr [rbp + 1216], rax
                        mov              qword ptr [rbp + 1224], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
xchain0_n34_α:
# BOX CALL local(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1392] -> [zr+1344]
                        mov              rax, qword ptr [rbp + 1392]
                        mov              qword ptr [rbp + 1344], rax
                        mov              rax, qword ptr [rbp + 1400]
                        mov              qword ptr [rbp + 1352], rax
# marshal arg1 = producer-box slot [zr+1472] -> [zr+1360]
                        mov              rax, qword ptr [rbp + 1472]
                        mov              qword ptr [rbp + 1360], rax
                        mov              rax, qword ptr [rbp + 1480]
                        mov              qword ptr [rbp + 1368], rax
                        .section         .rodata
.Lbynamefn35:           .string          "local"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn35]
                        lea              rsi, [rbp + 1344]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1328], rax
                        mov              qword ptr [rbp + 1336], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n32_α
                                                                                        jmp   xchain0_n36_α
xchain0_n34_β:
                                                                                        jmp   xchain0_n32_α
#-----------------------------------------------------------------------------------------------------------------------
xchain0_n35_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1760] -> [zr+1728]
                        mov              rax, qword ptr [rbp + 1760]
                        mov              qword ptr [rbp + 1728], rax
                        mov              rax, qword ptr [rbp + 1768]
                        mov              qword ptr [rbp + 1736], rax
                        .section         .rodata
.Lrkfn42:               .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn42]
                        lea              rsi, [rbp + 1728]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1712], rax
                        mov              qword ptr [rbp + 1720], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n38_α
                                                                                        jmp   xchain0_n37_α
xchain0_n35_β:
                                                                                        jmp   xchain0_n38_α
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n36_α:
                        mov              qword ptr [rbp + 1504], 1
                        mov              rax, qword ptr [rip + .Lx43_0]
                        mov              qword ptr [rbp + 1512], rax
                                                                                        jmp   xchain0_n39_α
.Lx43_0:
                        .quad            .Lx43_0_s
.Lx43_0_s:
                        .string          "D"
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_INTEGER
xchain0_n37_α:
                        mov              qword ptr [rbp + 1792], 6
                        mov              rax, qword ptr [rip + .Lx44_0]
                        mov              qword ptr [rbp + 1800], rax
                                                                                        jmp   xchain0_n40_α
.Lx44_0:
                        .quad            3
#=======================================================================================================================
#         local(.jlab, 0)            :f(e007)
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n38_α:
                        mov              qword ptr [rbp + 2016], 1
                        mov              rax, qword ptr [rip + .Lx45_0]
                        mov              qword ptr [rbp + 2024], rax
                                                                                        jmp   xchain0_n41_α
.Lx45_0:
                        .quad            .Lx45_0_s
.Lx45_0_s:
                        .string          "jlab"
#-----------------------------------------------------------------------------------------------------------------------
xchain0_n39_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1328] -> [zr+1280]
                        mov              rax, qword ptr [rbp + 1328]
                        mov              qword ptr [rbp + 1280], rax
                        mov              rax, qword ptr [rbp + 1336]
                        mov              qword ptr [rbp + 1288], rax
# marshal arg1 = producer-box slot [zr+1504] -> [zr+1296]
                        mov              rax, qword ptr [rbp + 1504]
                        mov              qword ptr [rbp + 1296], rax
                        mov              rax, qword ptr [rbp + 1512]
                        mov              qword ptr [rbp + 1304], rax
                        .section         .rodata
.Lbynamefn40:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn40]
                        lea              rsi, [rbp + 1280]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1264], rax
                        mov              qword ptr [rbp + 1272], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n32_α
                                                                                        jmp   xchain0_n42_α
xchain0_n39_β:
                                                                                        jmp   xchain0_n32_α
#-----------------------------------------------------------------------------------------------------------------------
xchain0_n40_α:
# BOX CALL local(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1712] -> [zr+1664]
                        mov              rax, qword ptr [rbp + 1712]
                        mov              qword ptr [rbp + 1664], rax
                        mov              rax, qword ptr [rbp + 1720]
                        mov              qword ptr [rbp + 1672], rax
# marshal arg1 = producer-box slot [zr+1792] -> [zr+1680]
                        mov              rax, qword ptr [rbp + 1792]
                        mov              qword ptr [rbp + 1680], rax
                        mov              rax, qword ptr [rbp + 1800]
                        mov              qword ptr [rbp + 1688], rax
                        .section         .rodata
.Lbynamefn41:           .string          "local"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn41]
                        lea              rsi, [rbp + 1664]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1648], rax
                        mov              qword ptr [rbp + 1656], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n38_α
                                                                                        jmp   xchain0_n43_α
xchain0_n40_β:
                                                                                        jmp   xchain0_n38_α
#-----------------------------------------------------------------------------------------------------------------------
xchain0_n41_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2016] -> [zr+1984]
                        mov              rax, qword ptr [rbp + 2016]
                        mov              qword ptr [rbp + 1984], rax
                        mov              rax, qword ptr [rbp + 2024]
                        mov              qword ptr [rbp + 1992], rax
                        .section         .rodata
.Lrkfn49:               .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn49]
                        lea              rsi, [rbp + 1984]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1968], rax
                        mov              qword ptr [rbp + 1976], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n45_α
                                                                                        jmp   xchain0_n44_α
xchain0_n41_β:
                                                                                        jmp   xchain0_n45_α
#=======================================================================================================================
#         output = 'FAIL 1017/005: local(.jlab,1) = D'   :(end)
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n42_α:
                        mov              qword ptr [rbp + 1552], 1
                        mov              rax, qword ptr [rip + .Lx50_0]
                        mov              qword ptr [rbp + 1560], rax
                                                                                        jmp   xchain0_n46_α
.Lx50_0:
                        .quad            .Lx50_0_s
.Lx50_0_s:
                        .string          "FAIL 1017/005: local(.jlab,1) = D"
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n43_α:
                        mov              qword ptr [rbp + 1824], 1
                        mov              rax, qword ptr [rip + .Lx51_0]
                        mov              qword ptr [rbp + 1832], rax
                                                                                        jmp   xchain0_n47_α
.Lx51_0:
                        .quad            .Lx51_0_s
.Lx51_0_s:
                        .string          "F"
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_INTEGER
xchain0_n44_α:
                        mov              qword ptr [rbp + 2048], 6
                        mov              rax, qword ptr [rip + .Lx52_0]
                        mov              qword ptr [rbp + 2056], rax
                                                                                        jmp   xchain0_n48_α
.Lx52_0:
                        .quad            0
#=======================================================================================================================
#         local(.jlab, 4)            :f(e008)
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n45_α:
                        mov              qword ptr [rbp + 2240], 1
                        mov              rax, qword ptr [rip + .Lx53_0]
                        mov              qword ptr [rbp + 2248], rax
                                                                                        jmp   xchain0_n49_α
.Lx53_0:
                        .quad            .Lx53_0_s
.Lx53_0_s:
                        .string          "jlab"
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN gva
xchain0_n46_α:
                        mov              rax, qword ptr [rbp + 1552]
                        mov              rdx, qword ptr [rbp + 1560]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        mov              qword ptr [rbp + 1536], rax
                        mov              qword ptr [rbp + 1544], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
xchain0_n47_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1648] -> [zr+1600]
                        mov              rax, qword ptr [rbp + 1648]
                        mov              qword ptr [rbp + 1600], rax
                        mov              rax, qword ptr [rbp + 1656]
                        mov              qword ptr [rbp + 1608], rax
# marshal arg1 = producer-box slot [zr+1824] -> [zr+1616]
                        mov              rax, qword ptr [rbp + 1824]
                        mov              qword ptr [rbp + 1616], rax
                        mov              rax, qword ptr [rbp + 1832]
                        mov              qword ptr [rbp + 1624], rax
                        .section         .rodata
.Lbynamefn48:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn48]
                        lea              rsi, [rbp + 1600]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1584], rax
                        mov              qword ptr [rbp + 1592], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n38_α
                                                                                        jmp   xchain0_n50_α
xchain0_n47_β:
                                                                                        jmp   xchain0_n38_α
#-----------------------------------------------------------------------------------------------------------------------
xchain0_n48_α:
# BOX CALL local(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1968] -> [zr+1920]
                        mov              rax, qword ptr [rbp + 1968]
                        mov              qword ptr [rbp + 1920], rax
                        mov              rax, qword ptr [rbp + 1976]
                        mov              qword ptr [rbp + 1928], rax
# marshal arg1 = producer-box slot [zr+2048] -> [zr+1936]
                        mov              rax, qword ptr [rbp + 2048]
                        mov              qword ptr [rbp + 1936], rax
                        mov              rax, qword ptr [rbp + 2056]
                        mov              qword ptr [rbp + 1944], rax
                        .section         .rodata
.Lbynamefn49:           .string          "local"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn49]
                        lea              rsi, [rbp + 1920]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1904], rax
                        mov              qword ptr [rbp + 1912], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n45_α
                                                                                        jmp   xchain0_n51_α
xchain0_n48_β:
                                                                                        jmp   xchain0_n45_α
#-----------------------------------------------------------------------------------------------------------------------
xchain0_n49_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2240] -> [zr+2208]
                        mov              rax, qword ptr [rbp + 2240]
                        mov              qword ptr [rbp + 2208], rax
                        mov              rax, qword ptr [rbp + 2248]
                        mov              qword ptr [rbp + 2216], rax
                        .section         .rodata
.Lrkfn58:               .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn58]
                        lea              rsi, [rbp + 2208]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 2192], rax
                        mov              qword ptr [rbp + 2200], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n53_α
                                                                                        jmp   xchain0_n52_α
xchain0_n49_β:
                                                                                        jmp   xchain0_n53_α
#=======================================================================================================================
#         output = 'FAIL 1017/006: local(.jlab,3) = F'   :(end)
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n50_α:
                        mov              qword ptr [rbp + 1872], 1
                        mov              rax, qword ptr [rip + .Lx59_0]
                        mov              qword ptr [rbp + 1880], rax
                                                                                        jmp   xchain0_n54_α
.Lx59_0:
                        .quad            .Lx59_0_s
.Lx59_0_s:
                        .string          "FAIL 1017/006: local(.jlab,3) = F"
#=======================================================================================================================
#         output = 'FAIL 1017/007: local(.jlab,0) OOB should fail' :(end)
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n51_α:
                        mov              qword ptr [rbp + 2096], 1
                        mov              rax, qword ptr [rip + .Lx60_0]
                        mov              qword ptr [rbp + 2104], rax
                                                                                        jmp   xchain0_n55_α
.Lx60_0:
                        .quad            .Lx60_0_s
.Lx60_0_s:
                        .string          "FAIL 1017/007: local(.jlab,0) OOB should fail"
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_INTEGER
xchain0_n52_α:
                        mov              qword ptr [rbp + 2272], 6
                        mov              rax, qword ptr [rip + .Lx61_0]
                        mov              qword ptr [rbp + 2280], rax
                                                                                        jmp   xchain0_n56_α
.Lx61_0:
                        .quad            4
#=======================================================================================================================
#         output = 'PASS 1017_arg_local (8/8)'
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n53_α:
                        mov              qword ptr [rbp + 2368], 1
                        mov              rax, qword ptr [rip + .Lx62_0]
                        mov              qword ptr [rbp + 2376], rax
                                                                                        jmp   xchain0_n57_α
.Lx62_0:
                        .quad            .Lx62_0_s
.Lx62_0_s:
                        .string          "PASS 1017_arg_local (8/8)"
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN gva
xchain0_n54_α:
                        mov              rax, qword ptr [rbp + 1872]
                        mov              rdx, qword ptr [rbp + 1880]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        mov              qword ptr [rbp + 1856], rax
                        mov              qword ptr [rbp + 1864], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN gva
xchain0_n55_α:
                        mov              rax, qword ptr [rbp + 2096]
                        mov              rdx, qword ptr [rbp + 2104]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        mov              qword ptr [rbp + 2080], rax
                        mov              qword ptr [rbp + 2088], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
xchain0_n56_α:
# BOX CALL local(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2192] -> [zr+2144]
                        mov              rax, qword ptr [rbp + 2192]
                        mov              qword ptr [rbp + 2144], rax
                        mov              rax, qword ptr [rbp + 2200]
                        mov              qword ptr [rbp + 2152], rax
# marshal arg1 = producer-box slot [zr+2272] -> [zr+2160]
                        mov              rax, qword ptr [rbp + 2272]
                        mov              qword ptr [rbp + 2160], rax
                        mov              rax, qword ptr [rbp + 2280]
                        mov              qword ptr [rbp + 2168], rax
                        .section         .rodata
.Lbynamefn57:           .string          "local"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn57]
                        lea              rsi, [rbp + 2144]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 2128], rax
                        mov              qword ptr [rbp + 2136], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n53_α
                                                                                        jmp   xchain0_n58_α
xchain0_n56_β:
                                                                                        jmp   xchain0_n53_α
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN gva
xchain0_n57_α:
                        mov              rax, qword ptr [rbp + 2368]
                        mov              rdx, qword ptr [rbp + 2376]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        mov              qword ptr [rbp + 2352], rax
                        mov              qword ptr [rbp + 2360], rdx
                                                                                        jmp   main_γ
#=======================================================================================================================
#         output = 'FAIL 1017/008: local(.jlab,4) OOB should fail' :(end)
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n58_α:
                        mov              qword ptr [rbp + 2320], 1
                        mov              rax, qword ptr [rip + .Lx67_0]
                        mov              qword ptr [rbp + 2328], rax
                                                                                        jmp   xchain0_n59_α
.Lx67_0:
                        .quad            .Lx67_0_s
.Lx67_0_s:
                        .string          "FAIL 1017/008: local(.jlab,4) OOB should fail"
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN gva
xchain0_n59_α:
                        mov              rax, qword ptr [rbp + 2320]
                        mov              rdx, qword ptr [rbp + 2328]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        mov              qword ptr [rbp + 2304], rax
                        mov              qword ptr [rbp + 2312], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        mov              rsp, rbp
                        mov              rbp, [rsp + 2400]
                        add              rsp, 2408
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              rsp, rbp
                        mov              dword ptr [rsp+0], 99
                        mov              dword ptr [rsp+4], 0
                        mov              qword ptr [rsp+8], 0
                        mov              eax, 99
                        xor              edx, edx
                        mov              rbp, [rsp + 2400]
                        add              rsp, 2408
                        ret
                        .section         .note.GNU-stack,"",@progbits

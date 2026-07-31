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
main_α_body:
#=======================================================================================================================
#         lt(5, 4)                   :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n0_lit_integer_α:
                        mov              qword ptr [rsp + 64], 6
                        mov              rax, qword ptr [rip + .Lx113_0]
                        mov              qword ptr [rsp + 72], rax
                                                                                        jmp   n20_lit_integer_α
.Lx113_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n1_goto_α:
                                                                                        jmp   n21_lit_integer_α
n1_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n2_goto_α:
                                                                                        jmp   n22_lit_integer_α
n2_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n3_goto_α:
                                                                                        jmp   n23_lit_integer_α
n3_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n4_goto_α:
                                                                                        jmp   n24_lit_integer_α
n4_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n5_goto_α:
                                                                                        jmp   n25_lit_integer_α
n5_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n6_goto_α:
                                                                                        jmp   n26_lit_integer_α
n6_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n7_goto_α:
                                                                                        jmp   n27_lit_integer_α
n7_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n8_goto_α:
                                                                                        jmp   n28_lit_integer_α
n8_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n9_goto_α:
                                                                                        jmp   n29_lit_integer_α
n9_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n10_goto_α:
                                                                                        jmp   n30_lit_integer_α
n10_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n11_goto_α:
                                                                                        jmp   n31_lit_integer_α
n11_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n12_goto_α:
                                                                                        jmp   n32_lit_integer_α
n12_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n13_goto_α:
                                                                                        jmp   n33_lit_integer_α
n13_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n14_goto_α:
                                                                                        jmp   n34_lit_integer_α
n14_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n15_goto_α:
                                                                                        jmp   n35_lit_integer_α
n15_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n16_goto_α:
                                                                                        jmp   n36_lit_integer_α
n16_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n17_goto_α:
                                                                                        jmp   n37_lit_string_α
n17_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n18_goto_α:
                                                                                        jmp   n38_lit_string_α
n18_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n19_goto_α:
                                                                                        jmp   main_γ
n19_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n20_lit_integer_α:
                        mov              qword ptr [rsp + 80], 6
                        mov              rax, qword ptr [rip + .Lx133_0]
                        mov              qword ptr [rsp + 88], rax
                                                                                        jmp   n39_call_α
.Lx133_0:
                        .quad            4
#=======================================================================================================================
# e001    lt(4, 4)                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n21_lit_integer_α:
                        mov              qword ptr [rsp + 192], 6
                        mov              rax, qword ptr [rip + .Lx134_0]
                        mov              qword ptr [rsp + 200], rax
                                                                                        jmp   n40_lit_integer_α
.Lx134_0:
                        .quad            4
#=======================================================================================================================
# e002    lt(4, 5)                   :s(e003)
#-----------------------------------------------------------------------------------------------------------------------
n22_lit_integer_α:
                        mov              qword ptr [rsp + 304], 6
                        mov              rax, qword ptr [rip + .Lx135_0]
                        mov              qword ptr [rsp + 312], rax
                                                                                        jmp   n41_lit_integer_α
.Lx135_0:
                        .quad            4
#=======================================================================================================================
#         le(5, 2)                   :f(e004)
#-----------------------------------------------------------------------------------------------------------------------
n23_lit_integer_α:
                        mov              qword ptr [rsp + 416], 6
                        mov              rax, qword ptr [rip + .Lx136_0]
                        mov              qword ptr [rsp + 424], rax
                                                                                        jmp   n42_lit_integer_α
.Lx136_0:
                        .quad            5
#=======================================================================================================================
# e004    le(4, 4)                   :s(e005)
#-----------------------------------------------------------------------------------------------------------------------
n24_lit_integer_α:
                        mov              qword ptr [rsp + 528], 6
                        mov              rax, qword ptr [rip + .Lx137_0]
                        mov              qword ptr [rsp + 536], rax
                                                                                        jmp   n43_lit_integer_α
.Lx137_0:
                        .quad            4
#=======================================================================================================================
# e005    le(4, 10)                  :s(e006)
#-----------------------------------------------------------------------------------------------------------------------
n25_lit_integer_α:
                        mov              qword ptr [rsp + 640], 6
                        mov              rax, qword ptr [rip + .Lx138_0]
                        mov              qword ptr [rsp + 648], rax
                                                                                        jmp   n44_lit_integer_α
.Lx138_0:
                        .quad            4
#=======================================================================================================================
#         eq(4, 5)                   :f(e007)
#-----------------------------------------------------------------------------------------------------------------------
n26_lit_integer_α:
                        mov              qword ptr [rsp + 752], 6
                        mov              rax, qword ptr [rip + .Lx139_0]
                        mov              qword ptr [rsp + 760], rax
                                                                                        jmp   n45_lit_integer_α
.Lx139_0:
                        .quad            4
#=======================================================================================================================
# e007    eq(5, 5)                   :s(e008)
#-----------------------------------------------------------------------------------------------------------------------
n27_lit_integer_α:
                        mov              qword ptr [rsp + 864], 6
                        mov              rax, qword ptr [rip + .Lx140_0]
                        mov              qword ptr [rsp + 872], rax
                                                                                        jmp   n46_lit_integer_α
.Lx140_0:
                        .quad            5
#=======================================================================================================================
#         ne(4, 4)                   :f(e009)
#-----------------------------------------------------------------------------------------------------------------------
n28_lit_integer_α:
                        mov              qword ptr [rsp + 976], 6
                        mov              rax, qword ptr [rip + .Lx141_0]
                        mov              qword ptr [rsp + 984], rax
                                                                                        jmp   n47_lit_integer_α
.Lx141_0:
                        .quad            4
#=======================================================================================================================
# e009    ne(4, 6)                   :s(e010)
#-----------------------------------------------------------------------------------------------------------------------
n29_lit_integer_α:
                        mov              qword ptr [rsp + 1088], 6
                        mov              rax, qword ptr [rip + .Lx142_0]
                        mov              qword ptr [rsp + 1096], rax
                                                                                        jmp   n48_lit_integer_α
.Lx142_0:
                        .quad            4
#=======================================================================================================================
#         gt(4, 6)                   :f(e011)
#-----------------------------------------------------------------------------------------------------------------------
n30_lit_integer_α:
                        mov              qword ptr [rsp + 1200], 6
                        mov              rax, qword ptr [rip + .Lx143_0]
                        mov              qword ptr [rsp + 1208], rax
                                                                                        jmp   n49_lit_integer_α
.Lx143_0:
                        .quad            4
#=======================================================================================================================
# e011    gt(4, 4)                   :f(e012)
#-----------------------------------------------------------------------------------------------------------------------
n31_lit_integer_α:
                        mov              qword ptr [rsp + 1312], 6
                        mov              rax, qword ptr [rip + .Lx144_0]
                        mov              qword ptr [rsp + 1320], rax
                                                                                        jmp   n50_lit_integer_α
.Lx144_0:
                        .quad            4
#=======================================================================================================================
# e012    gt(5, 2)                   :s(e013)
#-----------------------------------------------------------------------------------------------------------------------
n32_lit_integer_α:
                        mov              qword ptr [rsp + 1424], 6
                        mov              rax, qword ptr [rip + .Lx145_0]
                        mov              qword ptr [rsp + 1432], rax
                                                                                        jmp   n51_lit_integer_α
.Lx145_0:
                        .quad            5
#=======================================================================================================================
#         ge(5, 7)                   :f(e014)
#-----------------------------------------------------------------------------------------------------------------------
n33_lit_integer_α:
                        mov              qword ptr [rsp + 1536], 6
                        mov              rax, qword ptr [rip + .Lx146_0]
                        mov              qword ptr [rsp + 1544], rax
                                                                                        jmp   n52_lit_integer_α
.Lx146_0:
                        .quad            5
#=======================================================================================================================
# e014    ge(4, 4)                   :s(e015)
#-----------------------------------------------------------------------------------------------------------------------
n34_lit_integer_α:
                        mov              qword ptr [rsp + 1648], 6
                        mov              rax, qword ptr [rip + .Lx147_0]
                        mov              qword ptr [rsp + 1656], rax
                                                                                        jmp   n53_lit_integer_α
.Lx147_0:
                        .quad            4
#=======================================================================================================================
# e015    ge(7, 5)                   :s(e016)
#-----------------------------------------------------------------------------------------------------------------------
n35_lit_integer_α:
                        mov              qword ptr [rsp + 1760], 6
                        mov              rax, qword ptr [rip + .Lx148_0]
                        mov              qword ptr [rsp + 1768], rax
                                                                                        jmp   n54_lit_integer_α
.Lx148_0:
                        .quad            7
#=======================================================================================================================
#         ne(4, 5 - 1)               :f(e017)
#-----------------------------------------------------------------------------------------------------------------------
n36_lit_integer_α:
                        mov              qword ptr [rsp + 1872], 6
                        mov              rax, qword ptr [rip + .Lx149_0]
                        mov              qword ptr [rsp + 1880], rax
                                                                                        jmp   n55_lit_integer_α
.Lx149_0:
                        .quad            4
#=======================================================================================================================
#         ne('12', 12)                   :f(e018)
#-----------------------------------------------------------------------------------------------------------------------
n37_lit_string_α:
                        mov              qword ptr [rsp + 2016], 1
                        mov              dword ptr [rsp + 2020], 2
                        mov              rax, qword ptr [rip + .Lx150_0]
                        mov              qword ptr [rsp + 2024], rax
                                                                                        jmp   n56_lit_integer_α
.Lx150_0:
                        .quad            .Lx150_0_s
.Lx150_0_s:
                        .string          "12"
#=======================================================================================================================
#         output = 'PASS 912_num_pred (18/18)'
#-----------------------------------------------------------------------------------------------------------------------
n38_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 25
                        mov              rax, qword ptr [rip + .Lx151_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n57_assign_α
.Lx151_0:
                        .quad            .Lx151_0_s
.Lx151_0_s:
                        .string          "PASS 912_num_pred (18/18)"
#-----------------------------------------------------------------------------------------------------------------------
n39_call_α:
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 40], rax
                        .section         .rodata
.Lbynamefn40:           .string          "lt"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn40]
                        lea              rsi, [rsp + 16]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 99
                                                                                        je    n21_lit_integer_α
                                                                                        jmp   n58_lit_string_α
n39_call_β:
                                                                                        jmp   n21_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n40_lit_integer_α:
                        mov              qword ptr [rsp + 208], 6
                        mov              rax, qword ptr [rip + .Lx153_0]
                        mov              qword ptr [rsp + 216], rax
                                                                                        jmp   n59_call_α
.Lx153_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n41_lit_integer_α:
                        mov              qword ptr [rsp + 320], 6
                        mov              rax, qword ptr [rip + .Lx154_0]
                        mov              qword ptr [rsp + 328], rax
                                                                                        jmp   n60_call_α
.Lx154_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n42_lit_integer_α:
                        mov              qword ptr [rsp + 432], 6
                        mov              rax, qword ptr [rip + .Lx155_0]
                        mov              qword ptr [rsp + 440], rax
                                                                                        jmp   n61_call_α
.Lx155_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n43_lit_integer_α:
                        mov              qword ptr [rsp + 544], 6
                        mov              rax, qword ptr [rip + .Lx156_0]
                        mov              qword ptr [rsp + 552], rax
                                                                                        jmp   n62_call_α
.Lx156_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n44_lit_integer_α:
                        mov              qword ptr [rsp + 656], 6
                        mov              rax, qword ptr [rip + .Lx157_0]
                        mov              qword ptr [rsp + 664], rax
                                                                                        jmp   n63_call_α
.Lx157_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n45_lit_integer_α:
                        mov              qword ptr [rsp + 768], 6
                        mov              rax, qword ptr [rip + .Lx158_0]
                        mov              qword ptr [rsp + 776], rax
                                                                                        jmp   n64_call_α
.Lx158_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n46_lit_integer_α:
                        mov              qword ptr [rsp + 880], 6
                        mov              rax, qword ptr [rip + .Lx159_0]
                        mov              qword ptr [rsp + 888], rax
                                                                                        jmp   n65_call_α
.Lx159_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n47_lit_integer_α:
                        mov              qword ptr [rsp + 992], 6
                        mov              rax, qword ptr [rip + .Lx160_0]
                        mov              qword ptr [rsp + 1000], rax
                                                                                        jmp   n66_call_α
.Lx160_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n48_lit_integer_α:
                        mov              qword ptr [rsp + 1104], 6
                        mov              rax, qword ptr [rip + .Lx161_0]
                        mov              qword ptr [rsp + 1112], rax
                                                                                        jmp   n67_call_α
.Lx161_0:
                        .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n49_lit_integer_α:
                        mov              qword ptr [rsp + 1216], 6
                        mov              rax, qword ptr [rip + .Lx162_0]
                        mov              qword ptr [rsp + 1224], rax
                                                                                        jmp   n68_call_α
.Lx162_0:
                        .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n50_lit_integer_α:
                        mov              qword ptr [rsp + 1328], 6
                        mov              rax, qword ptr [rip + .Lx163_0]
                        mov              qword ptr [rsp + 1336], rax
                                                                                        jmp   n69_call_α
.Lx163_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n51_lit_integer_α:
                        mov              qword ptr [rsp + 1440], 6
                        mov              rax, qword ptr [rip + .Lx164_0]
                        mov              qword ptr [rsp + 1448], rax
                                                                                        jmp   n70_call_α
.Lx164_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n52_lit_integer_α:
                        mov              qword ptr [rsp + 1552], 6
                        mov              rax, qword ptr [rip + .Lx165_0]
                        mov              qword ptr [rsp + 1560], rax
                                                                                        jmp   n71_call_α
.Lx165_0:
                        .quad            7
#-----------------------------------------------------------------------------------------------------------------------
n53_lit_integer_α:
                        mov              qword ptr [rsp + 1664], 6
                        mov              rax, qword ptr [rip + .Lx166_0]
                        mov              qword ptr [rsp + 1672], rax
                                                                                        jmp   n72_call_α
.Lx166_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n54_lit_integer_α:
                        mov              qword ptr [rsp + 1776], 6
                        mov              rax, qword ptr [rip + .Lx167_0]
                        mov              qword ptr [rsp + 1784], rax
                                                                                        jmp   n73_call_α
.Lx167_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n55_lit_integer_α:
                        mov              qword ptr [rsp + 1904], 6
                        mov              rax, qword ptr [rip + .Lx168_0]
                        mov              qword ptr [rsp + 1912], rax
                                                                                        jmp   n74_lit_integer_α
.Lx168_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n56_lit_integer_α:
                        mov              qword ptr [rsp + 2032], 6
                        mov              rax, qword ptr [rip + .Lx169_0]
                        mov              qword ptr [rsp + 2040], rax
                                                                                        jmp   n75_call_α
.Lx169_0:
                        .quad            12
#-----------------------------------------------------------------------------------------------------------------------
n57_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         output = 'FAIL 912/001: lt(5,4) should fail'   :(end)
#-----------------------------------------------------------------------------------------------------------------------
n58_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx171_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n76_assign_α
.Lx171_0:
                        .quad            .Lx171_0_s
.Lx171_0_s:
                        .string          "FAIL 912/001: lt(5,4) should fail"
#-----------------------------------------------------------------------------------------------------------------------
n59_call_α:
                        mov              rax, qword ptr [rsp + 192]
                        mov              qword ptr [rsp + 144], rax
                        mov              rax, qword ptr [rsp + 200]
                        mov              qword ptr [rsp + 152], rax
                        mov              rax, qword ptr [rsp + 208]
                        mov              qword ptr [rsp + 160], rax
                        mov              rax, qword ptr [rsp + 216]
                        mov              qword ptr [rsp + 168], rax
                        .section         .rodata
.Lbynamefn60:           .string          "lt"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn60]
                        lea              rsi, [rsp + 144]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 128], rax
                        mov              qword ptr [rsp + 136], rdx
                        cmp              eax, 99
                                                                                        je    n22_lit_integer_α
                                                                                        jmp   n77_lit_string_α
n59_call_β:
                                                                                        jmp   n22_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n60_call_α:
                        mov              rax, qword ptr [rsp + 304]
                        mov              qword ptr [rsp + 256], rax
                        mov              rax, qword ptr [rsp + 312]
                        mov              qword ptr [rsp + 264], rax
                        mov              rax, qword ptr [rsp + 320]
                        mov              qword ptr [rsp + 272], rax
                        mov              rax, qword ptr [rsp + 328]
                        mov              qword ptr [rsp + 280], rax
                        .section         .rodata
.Lbynamefn61:           .string          "lt"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn61]
                        lea              rsi, [rsp + 256]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 240], rax
                        mov              qword ptr [rsp + 248], rdx
                        cmp              eax, 99
                                                                                        je    n78_lit_string_α
                                                                                        jmp   n23_lit_integer_α
n60_call_β:
                                                                                        jmp   n78_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n61_call_α:
                        mov              rax, qword ptr [rsp + 416]
                        mov              qword ptr [rsp + 368], rax
                        mov              rax, qword ptr [rsp + 424]
                        mov              qword ptr [rsp + 376], rax
                        mov              rax, qword ptr [rsp + 432]
                        mov              qword ptr [rsp + 384], rax
                        mov              rax, qword ptr [rsp + 440]
                        mov              qword ptr [rsp + 392], rax
                        .section         .rodata
.Lbynamefn62:           .string          "le"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn62]
                        lea              rsi, [rsp + 368]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 352], rax
                        mov              qword ptr [rsp + 360], rdx
                        cmp              eax, 99
                                                                                        je    n24_lit_integer_α
                                                                                        jmp   n79_lit_string_α
n61_call_β:
                                                                                        jmp   n24_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n62_call_α:
                        mov              rax, qword ptr [rsp + 528]
                        mov              qword ptr [rsp + 480], rax
                        mov              rax, qword ptr [rsp + 536]
                        mov              qword ptr [rsp + 488], rax
                        mov              rax, qword ptr [rsp + 544]
                        mov              qword ptr [rsp + 496], rax
                        mov              rax, qword ptr [rsp + 552]
                        mov              qword ptr [rsp + 504], rax
                        .section         .rodata
.Lbynamefn63:           .string          "le"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn63]
                        lea              rsi, [rsp + 480]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 464], rax
                        mov              qword ptr [rsp + 472], rdx
                        cmp              eax, 99
                                                                                        je    n80_lit_string_α
                                                                                        jmp   n25_lit_integer_α
n62_call_β:
                                                                                        jmp   n80_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n63_call_α:
                        mov              rax, qword ptr [rsp + 640]
                        mov              qword ptr [rsp + 592], rax
                        mov              rax, qword ptr [rsp + 648]
                        mov              qword ptr [rsp + 600], rax
                        mov              rax, qword ptr [rsp + 656]
                        mov              qword ptr [rsp + 608], rax
                        mov              rax, qword ptr [rsp + 664]
                        mov              qword ptr [rsp + 616], rax
                        .section         .rodata
.Lbynamefn64:           .string          "le"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn64]
                        lea              rsi, [rsp + 592]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 576], rax
                        mov              qword ptr [rsp + 584], rdx
                        cmp              eax, 99
                                                                                        je    n81_lit_string_α
                                                                                        jmp   n26_lit_integer_α
n63_call_β:
                                                                                        jmp   n81_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n64_call_α:
                        mov              rax, qword ptr [rsp + 752]
                        mov              qword ptr [rsp + 704], rax
                        mov              rax, qword ptr [rsp + 760]
                        mov              qword ptr [rsp + 712], rax
                        mov              rax, qword ptr [rsp + 768]
                        mov              qword ptr [rsp + 720], rax
                        mov              rax, qword ptr [rsp + 776]
                        mov              qword ptr [rsp + 728], rax
                        .section         .rodata
.Lbynamefn65:           .string          "eq"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn65]
                        lea              rsi, [rsp + 704]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 688], rax
                        mov              qword ptr [rsp + 696], rdx
                        cmp              eax, 99
                                                                                        je    n27_lit_integer_α
                                                                                        jmp   n82_lit_string_α
n64_call_β:
                                                                                        jmp   n27_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n65_call_α:
                        mov              rax, qword ptr [rsp + 864]
                        mov              qword ptr [rsp + 816], rax
                        mov              rax, qword ptr [rsp + 872]
                        mov              qword ptr [rsp + 824], rax
                        mov              rax, qword ptr [rsp + 880]
                        mov              qword ptr [rsp + 832], rax
                        mov              rax, qword ptr [rsp + 888]
                        mov              qword ptr [rsp + 840], rax
                        .section         .rodata
.Lbynamefn66:           .string          "eq"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn66]
                        lea              rsi, [rsp + 816]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 800], rax
                        mov              qword ptr [rsp + 808], rdx
                        cmp              eax, 99
                                                                                        je    n83_lit_string_α
                                                                                        jmp   n28_lit_integer_α
n65_call_β:
                                                                                        jmp   n83_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n66_call_α:
                        mov              rax, qword ptr [rsp + 976]
                        mov              qword ptr [rsp + 928], rax
                        mov              rax, qword ptr [rsp + 984]
                        mov              qword ptr [rsp + 936], rax
                        mov              rax, qword ptr [rsp + 992]
                        mov              qword ptr [rsp + 944], rax
                        mov              rax, qword ptr [rsp + 1000]
                        mov              qword ptr [rsp + 952], rax
                        .section         .rodata
.Lbynamefn67:           .string          "ne"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn67]
                        lea              rsi, [rsp + 928]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 912], rax
                        mov              qword ptr [rsp + 920], rdx
                        cmp              eax, 99
                                                                                        je    n29_lit_integer_α
                                                                                        jmp   n84_lit_string_α
n66_call_β:
                                                                                        jmp   n29_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n67_call_α:
                        mov              rax, qword ptr [rsp + 1088]
                        mov              qword ptr [rsp + 1040], rax
                        mov              rax, qword ptr [rsp + 1096]
                        mov              qword ptr [rsp + 1048], rax
                        mov              rax, qword ptr [rsp + 1104]
                        mov              qword ptr [rsp + 1056], rax
                        mov              rax, qword ptr [rsp + 1112]
                        mov              qword ptr [rsp + 1064], rax
                        .section         .rodata
.Lbynamefn68:           .string          "ne"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn68]
                        lea              rsi, [rsp + 1040]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1024], rax
                        mov              qword ptr [rsp + 1032], rdx
                        cmp              eax, 99
                                                                                        je    n85_lit_string_α
                                                                                        jmp   n30_lit_integer_α
n67_call_β:
                                                                                        jmp   n85_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n68_call_α:
                        mov              rax, qword ptr [rsp + 1200]
                        mov              qword ptr [rsp + 1152], rax
                        mov              rax, qword ptr [rsp + 1208]
                        mov              qword ptr [rsp + 1160], rax
                        mov              rax, qword ptr [rsp + 1216]
                        mov              qword ptr [rsp + 1168], rax
                        mov              rax, qword ptr [rsp + 1224]
                        mov              qword ptr [rsp + 1176], rax
                        .section         .rodata
.Lbynamefn69:           .string          "gt"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn69]
                        lea              rsi, [rsp + 1152]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1136], rax
                        mov              qword ptr [rsp + 1144], rdx
                        cmp              eax, 99
                                                                                        je    n31_lit_integer_α
                                                                                        jmp   n86_lit_string_α
n68_call_β:
                                                                                        jmp   n31_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n69_call_α:
                        mov              rax, qword ptr [rsp + 1312]
                        mov              qword ptr [rsp + 1264], rax
                        mov              rax, qword ptr [rsp + 1320]
                        mov              qword ptr [rsp + 1272], rax
                        mov              rax, qword ptr [rsp + 1328]
                        mov              qword ptr [rsp + 1280], rax
                        mov              rax, qword ptr [rsp + 1336]
                        mov              qword ptr [rsp + 1288], rax
                        .section         .rodata
.Lbynamefn70:           .string          "gt"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn70]
                        lea              rsi, [rsp + 1264]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1248], rax
                        mov              qword ptr [rsp + 1256], rdx
                        cmp              eax, 99
                                                                                        je    n32_lit_integer_α
                                                                                        jmp   n87_lit_string_α
n69_call_β:
                                                                                        jmp   n32_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n70_call_α:
                        mov              rax, qword ptr [rsp + 1424]
                        mov              qword ptr [rsp + 1376], rax
                        mov              rax, qword ptr [rsp + 1432]
                        mov              qword ptr [rsp + 1384], rax
                        mov              rax, qword ptr [rsp + 1440]
                        mov              qword ptr [rsp + 1392], rax
                        mov              rax, qword ptr [rsp + 1448]
                        mov              qword ptr [rsp + 1400], rax
                        .section         .rodata
.Lbynamefn71:           .string          "gt"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn71]
                        lea              rsi, [rsp + 1376]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1360], rax
                        mov              qword ptr [rsp + 1368], rdx
                        cmp              eax, 99
                                                                                        je    n88_lit_string_α
                                                                                        jmp   n33_lit_integer_α
n70_call_β:
                                                                                        jmp   n88_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n71_call_α:
                        mov              rax, qword ptr [rsp + 1536]
                        mov              qword ptr [rsp + 1488], rax
                        mov              rax, qword ptr [rsp + 1544]
                        mov              qword ptr [rsp + 1496], rax
                        mov              rax, qword ptr [rsp + 1552]
                        mov              qword ptr [rsp + 1504], rax
                        mov              rax, qword ptr [rsp + 1560]
                        mov              qword ptr [rsp + 1512], rax
                        .section         .rodata
.Lbynamefn72:           .string          "ge"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn72]
                        lea              rsi, [rsp + 1488]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1472], rax
                        mov              qword ptr [rsp + 1480], rdx
                        cmp              eax, 99
                                                                                        je    n34_lit_integer_α
                                                                                        jmp   n89_lit_string_α
n71_call_β:
                                                                                        jmp   n34_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n72_call_α:
                        mov              rax, qword ptr [rsp + 1648]
                        mov              qword ptr [rsp + 1600], rax
                        mov              rax, qword ptr [rsp + 1656]
                        mov              qword ptr [rsp + 1608], rax
                        mov              rax, qword ptr [rsp + 1664]
                        mov              qword ptr [rsp + 1616], rax
                        mov              rax, qword ptr [rsp + 1672]
                        mov              qword ptr [rsp + 1624], rax
                        .section         .rodata
.Lbynamefn73:           .string          "ge"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn73]
                        lea              rsi, [rsp + 1600]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1584], rax
                        mov              qword ptr [rsp + 1592], rdx
                        cmp              eax, 99
                                                                                        je    n90_lit_string_α
                                                                                        jmp   n35_lit_integer_α
n72_call_β:
                                                                                        jmp   n90_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n73_call_α:
                        mov              rax, qword ptr [rsp + 1760]
                        mov              qword ptr [rsp + 1712], rax
                        mov              rax, qword ptr [rsp + 1768]
                        mov              qword ptr [rsp + 1720], rax
                        mov              rax, qword ptr [rsp + 1776]
                        mov              qword ptr [rsp + 1728], rax
                        mov              rax, qword ptr [rsp + 1784]
                        mov              qword ptr [rsp + 1736], rax
                        .section         .rodata
.Lbynamefn74:           .string          "ge"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn74]
                        lea              rsi, [rsp + 1712]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1696], rax
                        mov              qword ptr [rsp + 1704], rdx
                        cmp              eax, 99
                                                                                        je    n91_lit_string_α
                                                                                        jmp   n36_lit_integer_α
n73_call_β:
                                                                                        jmp   n91_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n74_lit_integer_α:
                        mov              qword ptr [rsp + 1920], 6
                        mov              rax, qword ptr [rip + .Lx187_0]
                        mov              qword ptr [rsp + 1928], rax
                                                                                        jmp   n92_binop_α
.Lx187_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n75_call_α:
                        mov              rax, qword ptr [rsp + 2016]
                        mov              qword ptr [rsp + 1968], rax
                        mov              rax, qword ptr [rsp + 2024]
                        mov              qword ptr [rsp + 1976], rax
                        mov              rax, qword ptr [rsp + 2032]
                        mov              qword ptr [rsp + 1984], rax
                        mov              rax, qword ptr [rsp + 2040]
                        mov              qword ptr [rsp + 1992], rax
                        .section         .rodata
.Lbynamefn76:           .string          "ne"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn76]
                        lea              rsi, [rsp + 1968]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1952], rax
                        mov              qword ptr [rsp + 1960], rdx
                        cmp              eax, 99
                                                                                        je    n38_lit_string_α
                                                                                        jmp   n93_lit_string_α
n75_call_β:
                                                                                        jmp   n38_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n76_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         output = 'FAIL 912/002: lt(4,4) should fail'   :(end)
#-----------------------------------------------------------------------------------------------------------------------
n77_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx190_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n94_assign_α
.Lx190_0:
                        .quad            .Lx190_0_s
.Lx190_0_s:
                        .string          "FAIL 912/002: lt(4,4) should fail"
#=======================================================================================================================
#         output = 'FAIL 912/003: lt(4,5) should succeed' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n78_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 36
                        mov              rax, qword ptr [rip + .Lx191_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n95_assign_α
.Lx191_0:
                        .quad            .Lx191_0_s
.Lx191_0_s:
                        .string          "FAIL 912/003: lt(4,5) should succeed"
#=======================================================================================================================
#         output = 'FAIL 912/004: le(5,2) should fail'   :(end)
#-----------------------------------------------------------------------------------------------------------------------
n79_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx192_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n96_assign_α
.Lx192_0:
                        .quad            .Lx192_0_s
.Lx192_0_s:
                        .string          "FAIL 912/004: le(5,2) should fail"
#=======================================================================================================================
#         output = 'FAIL 912/005: le(4,4) should succeed' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n80_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 36
                        mov              rax, qword ptr [rip + .Lx193_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n97_assign_α
.Lx193_0:
                        .quad            .Lx193_0_s
.Lx193_0_s:
                        .string          "FAIL 912/005: le(4,4) should succeed"
#=======================================================================================================================
#         output = 'FAIL 912/006: le(4,10) should succeed' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n81_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 37
                        mov              rax, qword ptr [rip + .Lx194_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n98_assign_α
.Lx194_0:
                        .quad            .Lx194_0_s
.Lx194_0_s:
                        .string          "FAIL 912/006: le(4,10) should succeed"
#=======================================================================================================================
#         output = 'FAIL 912/007: eq(4,5) should fail'   :(end)
#-----------------------------------------------------------------------------------------------------------------------
n82_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx195_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n99_assign_α
.Lx195_0:
                        .quad            .Lx195_0_s
.Lx195_0_s:
                        .string          "FAIL 912/007: eq(4,5) should fail"
#=======================================================================================================================
#         output = 'FAIL 912/008: eq(5,5) should succeed' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n83_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 36
                        mov              rax, qword ptr [rip + .Lx196_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n100_assign_α
.Lx196_0:
                        .quad            .Lx196_0_s
.Lx196_0_s:
                        .string          "FAIL 912/008: eq(5,5) should succeed"
#=======================================================================================================================
#         output = 'FAIL 912/009: ne(4,4) should fail'   :(end)
#-----------------------------------------------------------------------------------------------------------------------
n84_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx197_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n101_assign_α
.Lx197_0:
                        .quad            .Lx197_0_s
.Lx197_0_s:
                        .string          "FAIL 912/009: ne(4,4) should fail"
#=======================================================================================================================
#         output = 'FAIL 912/010: ne(4,6) should succeed' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n85_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 36
                        mov              rax, qword ptr [rip + .Lx198_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n102_assign_α
.Lx198_0:
                        .quad            .Lx198_0_s
.Lx198_0_s:
                        .string          "FAIL 912/010: ne(4,6) should succeed"
#=======================================================================================================================
#         output = 'FAIL 912/011: gt(4,6) should fail'   :(end)
#-----------------------------------------------------------------------------------------------------------------------
n86_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx199_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n103_assign_α
.Lx199_0:
                        .quad            .Lx199_0_s
.Lx199_0_s:
                        .string          "FAIL 912/011: gt(4,6) should fail"
#=======================================================================================================================
#         output = 'FAIL 912/012: gt(4,4) should fail'   :(end)
#-----------------------------------------------------------------------------------------------------------------------
n87_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx200_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n104_assign_α
.Lx200_0:
                        .quad            .Lx200_0_s
.Lx200_0_s:
                        .string          "FAIL 912/012: gt(4,4) should fail"
#=======================================================================================================================
#         output = 'FAIL 912/013: gt(5,2) should succeed' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n88_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 36
                        mov              rax, qword ptr [rip + .Lx201_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n105_assign_α
.Lx201_0:
                        .quad            .Lx201_0_s
.Lx201_0_s:
                        .string          "FAIL 912/013: gt(5,2) should succeed"
#=======================================================================================================================
#         output = 'FAIL 912/014: ge(5,7) should fail'   :(end)
#-----------------------------------------------------------------------------------------------------------------------
n89_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx202_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n106_assign_α
.Lx202_0:
                        .quad            .Lx202_0_s
.Lx202_0_s:
                        .string          "FAIL 912/014: ge(5,7) should fail"
#=======================================================================================================================
#         output = 'FAIL 912/015: ge(4,4) should succeed' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n90_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 36
                        mov              rax, qword ptr [rip + .Lx203_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n107_assign_α
.Lx203_0:
                        .quad            .Lx203_0_s
.Lx203_0_s:
                        .string          "FAIL 912/015: ge(4,4) should succeed"
#=======================================================================================================================
#         output = 'FAIL 912/016: ge(7,5) should succeed' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n91_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 36
                        mov              rax, qword ptr [rip + .Lx204_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n108_assign_α
.Lx204_0:
                        .quad            .Lx204_0_s
.Lx204_0_s:
                        .string          "FAIL 912/016: ge(7,5) should succeed"
#-----------------------------------------------------------------------------------------------------------------------
n92_binop_α:
                        mov              rdi, qword ptr [rsp + 1904]
                        mov              rsi, qword ptr [rsp + 1912]
                        mov              rdx, qword ptr [rsp + 1920]
                        mov              rcx, qword ptr [rsp + 1928]
                        call             rt_sub@PLT
                        cmp              eax, 99
                                                                                        je    n37_lit_string_α
                        mov              qword ptr [rsp + 1888], rax
                        mov              qword ptr [rsp + 1896], rdx
                                                                                        jmp   n109_call_α
#=======================================================================================================================
#         output = 'FAIL 912/018: ne(string,int) type mismatch' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n93_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 42
                        mov              rax, qword ptr [rip + .Lx206_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n110_assign_α
.Lx206_0:
                        .quad            .Lx206_0_s
.Lx206_0_s:
                        .string          "FAIL 912/018: ne(string,int) type mismatch"
#-----------------------------------------------------------------------------------------------------------------------
n94_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n95_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n96_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n97_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n98_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n99_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n100_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n101_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n102_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n103_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n104_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n105_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n106_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n107_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n108_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n109_call_α:
                        mov              rax, qword ptr [rsp + 1872]
                        mov              qword ptr [rsp + 1824], rax
                        mov              rax, qword ptr [rsp + 1880]
                        mov              qword ptr [rsp + 1832], rax
                        mov              rax, qword ptr [rsp + 1888]
                        mov              qword ptr [rsp + 1840], rax
                        mov              rax, qword ptr [rsp + 1896]
                        mov              qword ptr [rsp + 1848], rax
                        .section         .rodata
.Lbynamefn110:          .string          "ne"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn110]
                        lea              rsi, [rsp + 1824]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1808], rax
                        mov              qword ptr [rsp + 1816], rdx
                        cmp              eax, 99
                                                                                        je    n37_lit_string_α
                                                                                        jmp   n111_lit_string_α
n109_call_β:
                                                                                        jmp   n37_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n110_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         output = 'FAIL 912/017: ne(4,5-1) should fail (both=4)' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n111_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 44
                        mov              rax, qword ptr [rip + .Lx224_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n112_assign_α
.Lx224_0:
                        .quad            .Lx224_0_s
.Lx224_0_s:
                        .string          "FAIL 912/017: ne(4,5-1) should fail (both=4)"
#-----------------------------------------------------------------------------------------------------------------------
n112_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        add              rsp, 2088
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              eax, 99
                        xor              edx, edx
                        add              rsp, 2088
                        ret
                        .section         .note.GNU-stack,"",@progbits

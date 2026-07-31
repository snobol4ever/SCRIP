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
                                                                                        jmp   n1_lit_integer_α
.Lx113_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n1_lit_integer_α:
                        mov              qword ptr [rsp + 80], 6
                        mov              rax, qword ptr [rip + .Lx114_0]
                        mov              qword ptr [rsp + 88], rax
                                                                                        jmp   n2_call_α
.Lx114_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n2_call_α:
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 40], rax
                        .section         .rodata
.Lbynamefn3:            .string          "lt"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn3]
                        lea              rsi, [rsp + 16]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 99
                                                                                        je    n5_lit_integer_α
                                                                                        jmp   n3_lit_string_α
n2_call_β:
                                                                                        jmp   n5_lit_integer_α
#=======================================================================================================================
#         output = 'FAIL 912/001: lt(5,4) should fail'   :(end)
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx116_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n4_assign_α
.Lx116_0:
                        .quad            .Lx116_0_s
.Lx116_0_s:
                        .string          "FAIL 912/001: lt(5,4) should fail"
#-----------------------------------------------------------------------------------------------------------------------
n4_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
# e001    lt(4, 4)                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n5_lit_integer_α:
                        mov              qword ptr [rsp + 192], 6
                        mov              rax, qword ptr [rip + .Lx118_0]
                        mov              qword ptr [rsp + 200], rax
                                                                                        jmp   n6_lit_integer_α
.Lx118_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n6_lit_integer_α:
                        mov              qword ptr [rsp + 208], 6
                        mov              rax, qword ptr [rip + .Lx119_0]
                        mov              qword ptr [rsp + 216], rax
                                                                                        jmp   n7_call_α
.Lx119_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n7_call_α:
                        mov              rax, qword ptr [rsp + 192]
                        mov              qword ptr [rsp + 144], rax
                        mov              rax, qword ptr [rsp + 200]
                        mov              qword ptr [rsp + 152], rax
                        mov              rax, qword ptr [rsp + 208]
                        mov              qword ptr [rsp + 160], rax
                        mov              rax, qword ptr [rsp + 216]
                        mov              qword ptr [rsp + 168], rax
                        .section         .rodata
.Lbynamefn8:            .string          "lt"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn8]
                        lea              rsi, [rsp + 144]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 128], rax
                        mov              qword ptr [rsp + 136], rdx
                        cmp              eax, 99
                                                                                        je    n10_lit_integer_α
                                                                                        jmp   n8_lit_string_α
n7_call_β:
                                                                                        jmp   n10_lit_integer_α
#=======================================================================================================================
#         output = 'FAIL 912/002: lt(4,4) should fail'   :(end)
#-----------------------------------------------------------------------------------------------------------------------
n8_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx121_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n9_assign_α
.Lx121_0:
                        .quad            .Lx121_0_s
.Lx121_0_s:
                        .string          "FAIL 912/002: lt(4,4) should fail"
#-----------------------------------------------------------------------------------------------------------------------
n9_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
# e002    lt(4, 5)                   :s(e003)
#-----------------------------------------------------------------------------------------------------------------------
n10_lit_integer_α:
                        mov              qword ptr [rsp + 304], 6
                        mov              rax, qword ptr [rip + .Lx123_0]
                        mov              qword ptr [rsp + 312], rax
                                                                                        jmp   n11_lit_integer_α
.Lx123_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n11_lit_integer_α:
                        mov              qword ptr [rsp + 320], 6
                        mov              rax, qword ptr [rip + .Lx124_0]
                        mov              qword ptr [rsp + 328], rax
                                                                                        jmp   n12_call_α
.Lx124_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n12_call_α:
                        mov              rax, qword ptr [rsp + 304]
                        mov              qword ptr [rsp + 256], rax
                        mov              rax, qword ptr [rsp + 312]
                        mov              qword ptr [rsp + 264], rax
                        mov              rax, qword ptr [rsp + 320]
                        mov              qword ptr [rsp + 272], rax
                        mov              rax, qword ptr [rsp + 328]
                        mov              qword ptr [rsp + 280], rax
                        .section         .rodata
.Lbynamefn13:           .string          "lt"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn13]
                        lea              rsi, [rsp + 256]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 240], rax
                        mov              qword ptr [rsp + 248], rdx
                        cmp              eax, 99
                                                                                        je    n92_lit_string_α
                                                                                        jmp   n13_lit_integer_α
n12_call_β:
                                                                                        jmp   n92_lit_string_α
#=======================================================================================================================
#         le(5, 2)                   :f(e004)
#-----------------------------------------------------------------------------------------------------------------------
n13_lit_integer_α:
                        mov              qword ptr [rsp + 416], 6
                        mov              rax, qword ptr [rip + .Lx126_0]
                        mov              qword ptr [rsp + 424], rax
                                                                                        jmp   n14_lit_integer_α
.Lx126_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n14_lit_integer_α:
                        mov              qword ptr [rsp + 432], 6
                        mov              rax, qword ptr [rip + .Lx127_0]
                        mov              qword ptr [rsp + 440], rax
                                                                                        jmp   n15_call_α
.Lx127_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n15_call_α:
                        mov              rax, qword ptr [rsp + 416]
                        mov              qword ptr [rsp + 368], rax
                        mov              rax, qword ptr [rsp + 424]
                        mov              qword ptr [rsp + 376], rax
                        mov              rax, qword ptr [rsp + 432]
                        mov              qword ptr [rsp + 384], rax
                        mov              rax, qword ptr [rsp + 440]
                        mov              qword ptr [rsp + 392], rax
                        .section         .rodata
.Lbynamefn16:           .string          "le"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn16]
                        lea              rsi, [rsp + 368]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 352], rax
                        mov              qword ptr [rsp + 360], rdx
                        cmp              eax, 99
                                                                                        je    n18_lit_integer_α
                                                                                        jmp   n16_lit_string_α
n15_call_β:
                                                                                        jmp   n18_lit_integer_α
#=======================================================================================================================
#         output = 'FAIL 912/004: le(5,2) should fail'   :(end)
#-----------------------------------------------------------------------------------------------------------------------
n16_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx129_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n17_assign_α
.Lx129_0:
                        .quad            .Lx129_0_s
.Lx129_0_s:
                        .string          "FAIL 912/004: le(5,2) should fail"
#-----------------------------------------------------------------------------------------------------------------------
n17_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
# e004    le(4, 4)                   :s(e005)
#-----------------------------------------------------------------------------------------------------------------------
n18_lit_integer_α:
                        mov              qword ptr [rsp + 528], 6
                        mov              rax, qword ptr [rip + .Lx131_0]
                        mov              qword ptr [rsp + 536], rax
                                                                                        jmp   n19_lit_integer_α
.Lx131_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n19_lit_integer_α:
                        mov              qword ptr [rsp + 544], 6
                        mov              rax, qword ptr [rip + .Lx132_0]
                        mov              qword ptr [rsp + 552], rax
                                                                                        jmp   n20_call_α
.Lx132_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n20_call_α:
                        mov              rax, qword ptr [rsp + 528]
                        mov              qword ptr [rsp + 480], rax
                        mov              rax, qword ptr [rsp + 536]
                        mov              qword ptr [rsp + 488], rax
                        mov              rax, qword ptr [rsp + 544]
                        mov              qword ptr [rsp + 496], rax
                        mov              rax, qword ptr [rsp + 552]
                        mov              qword ptr [rsp + 504], rax
                        .section         .rodata
.Lbynamefn21:           .string          "le"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn21]
                        lea              rsi, [rsp + 480]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 464], rax
                        mov              qword ptr [rsp + 472], rdx
                        cmp              eax, 99
                                                                                        je    n90_lit_string_α
                                                                                        jmp   n21_lit_integer_α
n20_call_β:
                                                                                        jmp   n90_lit_string_α
#=======================================================================================================================
# e005    le(4, 10)                  :s(e006)
#-----------------------------------------------------------------------------------------------------------------------
n21_lit_integer_α:
                        mov              qword ptr [rsp + 640], 6
                        mov              rax, qword ptr [rip + .Lx134_0]
                        mov              qword ptr [rsp + 648], rax
                                                                                        jmp   n22_lit_integer_α
.Lx134_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n22_lit_integer_α:
                        mov              qword ptr [rsp + 656], 6
                        mov              rax, qword ptr [rip + .Lx135_0]
                        mov              qword ptr [rsp + 664], rax
                                                                                        jmp   n23_call_α
.Lx135_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n23_call_α:
                        mov              rax, qword ptr [rsp + 640]
                        mov              qword ptr [rsp + 592], rax
                        mov              rax, qword ptr [rsp + 648]
                        mov              qword ptr [rsp + 600], rax
                        mov              rax, qword ptr [rsp + 656]
                        mov              qword ptr [rsp + 608], rax
                        mov              rax, qword ptr [rsp + 664]
                        mov              qword ptr [rsp + 616], rax
                        .section         .rodata
.Lbynamefn24:           .string          "le"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn24]
                        lea              rsi, [rsp + 592]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 576], rax
                        mov              qword ptr [rsp + 584], rdx
                        cmp              eax, 99
                                                                                        je    n88_lit_string_α
                                                                                        jmp   n24_lit_integer_α
n23_call_β:
                                                                                        jmp   n88_lit_string_α
#=======================================================================================================================
#         eq(4, 5)                   :f(e007)
#-----------------------------------------------------------------------------------------------------------------------
n24_lit_integer_α:
                        mov              qword ptr [rsp + 752], 6
                        mov              rax, qword ptr [rip + .Lx137_0]
                        mov              qword ptr [rsp + 760], rax
                                                                                        jmp   n25_lit_integer_α
.Lx137_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n25_lit_integer_α:
                        mov              qword ptr [rsp + 768], 6
                        mov              rax, qword ptr [rip + .Lx138_0]
                        mov              qword ptr [rsp + 776], rax
                                                                                        jmp   n26_call_α
.Lx138_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n26_call_α:
                        mov              rax, qword ptr [rsp + 752]
                        mov              qword ptr [rsp + 704], rax
                        mov              rax, qword ptr [rsp + 760]
                        mov              qword ptr [rsp + 712], rax
                        mov              rax, qword ptr [rsp + 768]
                        mov              qword ptr [rsp + 720], rax
                        mov              rax, qword ptr [rsp + 776]
                        mov              qword ptr [rsp + 728], rax
                        .section         .rodata
.Lbynamefn27:           .string          "eq"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn27]
                        lea              rsi, [rsp + 704]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 688], rax
                        mov              qword ptr [rsp + 696], rdx
                        cmp              eax, 99
                                                                                        je    n29_lit_integer_α
                                                                                        jmp   n27_lit_string_α
n26_call_β:
                                                                                        jmp   n29_lit_integer_α
#=======================================================================================================================
#         output = 'FAIL 912/007: eq(4,5) should fail'   :(end)
#-----------------------------------------------------------------------------------------------------------------------
n27_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx140_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n28_assign_α
.Lx140_0:
                        .quad            .Lx140_0_s
.Lx140_0_s:
                        .string          "FAIL 912/007: eq(4,5) should fail"
#-----------------------------------------------------------------------------------------------------------------------
n28_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
# e007    eq(5, 5)                   :s(e008)
#-----------------------------------------------------------------------------------------------------------------------
n29_lit_integer_α:
                        mov              qword ptr [rsp + 864], 6
                        mov              rax, qword ptr [rip + .Lx142_0]
                        mov              qword ptr [rsp + 872], rax
                                                                                        jmp   n30_lit_integer_α
.Lx142_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n30_lit_integer_α:
                        mov              qword ptr [rsp + 880], 6
                        mov              rax, qword ptr [rip + .Lx143_0]
                        mov              qword ptr [rsp + 888], rax
                                                                                        jmp   n31_call_α
.Lx143_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n31_call_α:
                        mov              rax, qword ptr [rsp + 864]
                        mov              qword ptr [rsp + 816], rax
                        mov              rax, qword ptr [rsp + 872]
                        mov              qword ptr [rsp + 824], rax
                        mov              rax, qword ptr [rsp + 880]
                        mov              qword ptr [rsp + 832], rax
                        mov              rax, qword ptr [rsp + 888]
                        mov              qword ptr [rsp + 840], rax
                        .section         .rodata
.Lbynamefn32:           .string          "eq"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn32]
                        lea              rsi, [rsp + 816]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 800], rax
                        mov              qword ptr [rsp + 808], rdx
                        cmp              eax, 99
                                                                                        je    n86_lit_string_α
                                                                                        jmp   n32_lit_integer_α
n31_call_β:
                                                                                        jmp   n86_lit_string_α
#=======================================================================================================================
#         ne(4, 4)                   :f(e009)
#-----------------------------------------------------------------------------------------------------------------------
n32_lit_integer_α:
                        mov              qword ptr [rsp + 976], 6
                        mov              rax, qword ptr [rip + .Lx145_0]
                        mov              qword ptr [rsp + 984], rax
                                                                                        jmp   n33_lit_integer_α
.Lx145_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n33_lit_integer_α:
                        mov              qword ptr [rsp + 992], 6
                        mov              rax, qword ptr [rip + .Lx146_0]
                        mov              qword ptr [rsp + 1000], rax
                                                                                        jmp   n34_call_α
.Lx146_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n34_call_α:
                        mov              rax, qword ptr [rsp + 976]
                        mov              qword ptr [rsp + 928], rax
                        mov              rax, qword ptr [rsp + 984]
                        mov              qword ptr [rsp + 936], rax
                        mov              rax, qword ptr [rsp + 992]
                        mov              qword ptr [rsp + 944], rax
                        mov              rax, qword ptr [rsp + 1000]
                        mov              qword ptr [rsp + 952], rax
                        .section         .rodata
.Lbynamefn35:           .string          "ne"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn35]
                        lea              rsi, [rsp + 928]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 912], rax
                        mov              qword ptr [rsp + 920], rdx
                        cmp              eax, 99
                                                                                        je    n37_lit_integer_α
                                                                                        jmp   n35_lit_string_α
n34_call_β:
                                                                                        jmp   n37_lit_integer_α
#=======================================================================================================================
#         output = 'FAIL 912/009: ne(4,4) should fail'   :(end)
#-----------------------------------------------------------------------------------------------------------------------
n35_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx148_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n36_assign_α
.Lx148_0:
                        .quad            .Lx148_0_s
.Lx148_0_s:
                        .string          "FAIL 912/009: ne(4,4) should fail"
#-----------------------------------------------------------------------------------------------------------------------
n36_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
# e009    ne(4, 6)                   :s(e010)
#-----------------------------------------------------------------------------------------------------------------------
n37_lit_integer_α:
                        mov              qword ptr [rsp + 1088], 6
                        mov              rax, qword ptr [rip + .Lx150_0]
                        mov              qword ptr [rsp + 1096], rax
                                                                                        jmp   n38_lit_integer_α
.Lx150_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n38_lit_integer_α:
                        mov              qword ptr [rsp + 1104], 6
                        mov              rax, qword ptr [rip + .Lx151_0]
                        mov              qword ptr [rsp + 1112], rax
                                                                                        jmp   n39_call_α
.Lx151_0:
                        .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n39_call_α:
                        mov              rax, qword ptr [rsp + 1088]
                        mov              qword ptr [rsp + 1040], rax
                        mov              rax, qword ptr [rsp + 1096]
                        mov              qword ptr [rsp + 1048], rax
                        mov              rax, qword ptr [rsp + 1104]
                        mov              qword ptr [rsp + 1056], rax
                        mov              rax, qword ptr [rsp + 1112]
                        mov              qword ptr [rsp + 1064], rax
                        .section         .rodata
.Lbynamefn40:           .string          "ne"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn40]
                        lea              rsi, [rsp + 1040]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1024], rax
                        mov              qword ptr [rsp + 1032], rdx
                        cmp              eax, 99
                                                                                        je    n84_lit_string_α
                                                                                        jmp   n40_lit_integer_α
n39_call_β:
                                                                                        jmp   n84_lit_string_α
#=======================================================================================================================
#         gt(4, 6)                   :f(e011)
#-----------------------------------------------------------------------------------------------------------------------
n40_lit_integer_α:
                        mov              qword ptr [rsp + 1200], 6
                        mov              rax, qword ptr [rip + .Lx153_0]
                        mov              qword ptr [rsp + 1208], rax
                                                                                        jmp   n41_lit_integer_α
.Lx153_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n41_lit_integer_α:
                        mov              qword ptr [rsp + 1216], 6
                        mov              rax, qword ptr [rip + .Lx154_0]
                        mov              qword ptr [rsp + 1224], rax
                                                                                        jmp   n42_call_α
.Lx154_0:
                        .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n42_call_α:
                        mov              rax, qword ptr [rsp + 1200]
                        mov              qword ptr [rsp + 1152], rax
                        mov              rax, qword ptr [rsp + 1208]
                        mov              qword ptr [rsp + 1160], rax
                        mov              rax, qword ptr [rsp + 1216]
                        mov              qword ptr [rsp + 1168], rax
                        mov              rax, qword ptr [rsp + 1224]
                        mov              qword ptr [rsp + 1176], rax
                        .section         .rodata
.Lbynamefn43:           .string          "gt"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn43]
                        lea              rsi, [rsp + 1152]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1136], rax
                        mov              qword ptr [rsp + 1144], rdx
                        cmp              eax, 99
                                                                                        je    n45_lit_integer_α
                                                                                        jmp   n43_lit_string_α
n42_call_β:
                                                                                        jmp   n45_lit_integer_α
#=======================================================================================================================
#         output = 'FAIL 912/011: gt(4,6) should fail'   :(end)
#-----------------------------------------------------------------------------------------------------------------------
n43_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx156_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n44_assign_α
.Lx156_0:
                        .quad            .Lx156_0_s
.Lx156_0_s:
                        .string          "FAIL 912/011: gt(4,6) should fail"
#-----------------------------------------------------------------------------------------------------------------------
n44_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
# e011    gt(4, 4)                   :f(e012)
#-----------------------------------------------------------------------------------------------------------------------
n45_lit_integer_α:
                        mov              qword ptr [rsp + 1312], 6
                        mov              rax, qword ptr [rip + .Lx158_0]
                        mov              qword ptr [rsp + 1320], rax
                                                                                        jmp   n46_lit_integer_α
.Lx158_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n46_lit_integer_α:
                        mov              qword ptr [rsp + 1328], 6
                        mov              rax, qword ptr [rip + .Lx159_0]
                        mov              qword ptr [rsp + 1336], rax
                                                                                        jmp   n47_call_α
.Lx159_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n47_call_α:
                        mov              rax, qword ptr [rsp + 1312]
                        mov              qword ptr [rsp + 1264], rax
                        mov              rax, qword ptr [rsp + 1320]
                        mov              qword ptr [rsp + 1272], rax
                        mov              rax, qword ptr [rsp + 1328]
                        mov              qword ptr [rsp + 1280], rax
                        mov              rax, qword ptr [rsp + 1336]
                        mov              qword ptr [rsp + 1288], rax
                        .section         .rodata
.Lbynamefn48:           .string          "gt"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn48]
                        lea              rsi, [rsp + 1264]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1248], rax
                        mov              qword ptr [rsp + 1256], rdx
                        cmp              eax, 99
                                                                                        je    n50_lit_integer_α
                                                                                        jmp   n48_lit_string_α
n47_call_β:
                                                                                        jmp   n50_lit_integer_α
#=======================================================================================================================
#         output = 'FAIL 912/012: gt(4,4) should fail'   :(end)
#-----------------------------------------------------------------------------------------------------------------------
n48_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx161_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n49_assign_α
.Lx161_0:
                        .quad            .Lx161_0_s
.Lx161_0_s:
                        .string          "FAIL 912/012: gt(4,4) should fail"
#-----------------------------------------------------------------------------------------------------------------------
n49_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
# e012    gt(5, 2)                   :s(e013)
#-----------------------------------------------------------------------------------------------------------------------
n50_lit_integer_α:
                        mov              qword ptr [rsp + 1424], 6
                        mov              rax, qword ptr [rip + .Lx163_0]
                        mov              qword ptr [rsp + 1432], rax
                                                                                        jmp   n51_lit_integer_α
.Lx163_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n51_lit_integer_α:
                        mov              qword ptr [rsp + 1440], 6
                        mov              rax, qword ptr [rip + .Lx164_0]
                        mov              qword ptr [rsp + 1448], rax
                                                                                        jmp   n52_call_α
.Lx164_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n52_call_α:
                        mov              rax, qword ptr [rsp + 1424]
                        mov              qword ptr [rsp + 1376], rax
                        mov              rax, qword ptr [rsp + 1432]
                        mov              qword ptr [rsp + 1384], rax
                        mov              rax, qword ptr [rsp + 1440]
                        mov              qword ptr [rsp + 1392], rax
                        mov              rax, qword ptr [rsp + 1448]
                        mov              qword ptr [rsp + 1400], rax
                        .section         .rodata
.Lbynamefn53:           .string          "gt"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn53]
                        lea              rsi, [rsp + 1376]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1360], rax
                        mov              qword ptr [rsp + 1368], rdx
                        cmp              eax, 99
                                                                                        je    n82_lit_string_α
                                                                                        jmp   n53_lit_integer_α
n52_call_β:
                                                                                        jmp   n82_lit_string_α
#=======================================================================================================================
#         ge(5, 7)                   :f(e014)
#-----------------------------------------------------------------------------------------------------------------------
n53_lit_integer_α:
                        mov              qword ptr [rsp + 1536], 6
                        mov              rax, qword ptr [rip + .Lx166_0]
                        mov              qword ptr [rsp + 1544], rax
                                                                                        jmp   n54_lit_integer_α
.Lx166_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n54_lit_integer_α:
                        mov              qword ptr [rsp + 1552], 6
                        mov              rax, qword ptr [rip + .Lx167_0]
                        mov              qword ptr [rsp + 1560], rax
                                                                                        jmp   n55_call_α
.Lx167_0:
                        .quad            7
#-----------------------------------------------------------------------------------------------------------------------
n55_call_α:
                        mov              rax, qword ptr [rsp + 1536]
                        mov              qword ptr [rsp + 1488], rax
                        mov              rax, qword ptr [rsp + 1544]
                        mov              qword ptr [rsp + 1496], rax
                        mov              rax, qword ptr [rsp + 1552]
                        mov              qword ptr [rsp + 1504], rax
                        mov              rax, qword ptr [rsp + 1560]
                        mov              qword ptr [rsp + 1512], rax
                        .section         .rodata
.Lbynamefn56:           .string          "ge"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn56]
                        lea              rsi, [rsp + 1488]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1472], rax
                        mov              qword ptr [rsp + 1480], rdx
                        cmp              eax, 99
                                                                                        je    n58_lit_integer_α
                                                                                        jmp   n56_lit_string_α
n55_call_β:
                                                                                        jmp   n58_lit_integer_α
#=======================================================================================================================
#         output = 'FAIL 912/014: ge(5,7) should fail'   :(end)
#-----------------------------------------------------------------------------------------------------------------------
n56_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx169_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n57_assign_α
.Lx169_0:
                        .quad            .Lx169_0_s
.Lx169_0_s:
                        .string          "FAIL 912/014: ge(5,7) should fail"
#-----------------------------------------------------------------------------------------------------------------------
n57_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
# e014    ge(4, 4)                   :s(e015)
#-----------------------------------------------------------------------------------------------------------------------
n58_lit_integer_α:
                        mov              qword ptr [rsp + 1648], 6
                        mov              rax, qword ptr [rip + .Lx171_0]
                        mov              qword ptr [rsp + 1656], rax
                                                                                        jmp   n59_lit_integer_α
.Lx171_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n59_lit_integer_α:
                        mov              qword ptr [rsp + 1664], 6
                        mov              rax, qword ptr [rip + .Lx172_0]
                        mov              qword ptr [rsp + 1672], rax
                                                                                        jmp   n60_call_α
.Lx172_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n60_call_α:
                        mov              rax, qword ptr [rsp + 1648]
                        mov              qword ptr [rsp + 1600], rax
                        mov              rax, qword ptr [rsp + 1656]
                        mov              qword ptr [rsp + 1608], rax
                        mov              rax, qword ptr [rsp + 1664]
                        mov              qword ptr [rsp + 1616], rax
                        mov              rax, qword ptr [rsp + 1672]
                        mov              qword ptr [rsp + 1624], rax
                        .section         .rodata
.Lbynamefn61:           .string          "ge"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn61]
                        lea              rsi, [rsp + 1600]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1584], rax
                        mov              qword ptr [rsp + 1592], rdx
                        cmp              eax, 99
                                                                                        je    n80_lit_string_α
                                                                                        jmp   n61_lit_integer_α
n60_call_β:
                                                                                        jmp   n80_lit_string_α
#=======================================================================================================================
# e015    ge(7, 5)                   :s(e016)
#-----------------------------------------------------------------------------------------------------------------------
n61_lit_integer_α:
                        mov              qword ptr [rsp + 1760], 6
                        mov              rax, qword ptr [rip + .Lx174_0]
                        mov              qword ptr [rsp + 1768], rax
                                                                                        jmp   n62_lit_integer_α
.Lx174_0:
                        .quad            7
#-----------------------------------------------------------------------------------------------------------------------
n62_lit_integer_α:
                        mov              qword ptr [rsp + 1776], 6
                        mov              rax, qword ptr [rip + .Lx175_0]
                        mov              qword ptr [rsp + 1784], rax
                                                                                        jmp   n63_call_α
.Lx175_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n63_call_α:
                        mov              rax, qword ptr [rsp + 1760]
                        mov              qword ptr [rsp + 1712], rax
                        mov              rax, qword ptr [rsp + 1768]
                        mov              qword ptr [rsp + 1720], rax
                        mov              rax, qword ptr [rsp + 1776]
                        mov              qword ptr [rsp + 1728], rax
                        mov              rax, qword ptr [rsp + 1784]
                        mov              qword ptr [rsp + 1736], rax
                        .section         .rodata
.Lbynamefn64:           .string          "ge"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn64]
                        lea              rsi, [rsp + 1712]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1696], rax
                        mov              qword ptr [rsp + 1704], rdx
                        cmp              eax, 99
                                                                                        je    n78_lit_string_α
                                                                                        jmp   n64_lit_integer_α
n63_call_β:
                                                                                        jmp   n78_lit_string_α
#=======================================================================================================================
#         ne(4, 5 - 1)               :f(e017)
#-----------------------------------------------------------------------------------------------------------------------
n64_lit_integer_α:
                        mov              qword ptr [rsp + 1872], 6
                        mov              rax, qword ptr [rip + .Lx177_0]
                        mov              qword ptr [rsp + 1880], rax
                                                                                        jmp   n65_lit_integer_α
.Lx177_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n65_lit_integer_α:
                        mov              qword ptr [rsp + 1904], 6
                        mov              rax, qword ptr [rip + .Lx178_0]
                        mov              qword ptr [rsp + 1912], rax
                                                                                        jmp   n66_lit_integer_α
.Lx178_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n66_lit_integer_α:
                        mov              qword ptr [rsp + 1920], 6
                        mov              rax, qword ptr [rip + .Lx179_0]
                        mov              qword ptr [rsp + 1928], rax
                                                                                        jmp   n67_binop_α
.Lx179_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n67_binop_α:
                        mov              rdi, qword ptr [rsp + 1904]
                        mov              rsi, qword ptr [rsp + 1912]
                        mov              rdx, qword ptr [rsp + 1920]
                        mov              rcx, qword ptr [rsp + 1928]
                        call             rt_sub@PLT
                        cmp              eax, 99
                                                                                        je    n71_lit_string_α
                        mov              qword ptr [rsp + 1888], rax
                        mov              qword ptr [rsp + 1896], rdx
                                                                                        jmp   n68_call_α
#-----------------------------------------------------------------------------------------------------------------------
n68_call_α:
                        mov              rax, qword ptr [rsp + 1872]
                        mov              qword ptr [rsp + 1824], rax
                        mov              rax, qword ptr [rsp + 1880]
                        mov              qword ptr [rsp + 1832], rax
                        mov              rax, qword ptr [rsp + 1888]
                        mov              qword ptr [rsp + 1840], rax
                        mov              rax, qword ptr [rsp + 1896]
                        mov              qword ptr [rsp + 1848], rax
                        .section         .rodata
.Lbynamefn69:           .string          "ne"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn69]
                        lea              rsi, [rsp + 1824]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1808], rax
                        mov              qword ptr [rsp + 1816], rdx
                        cmp              eax, 99
                                                                                        je    n71_lit_string_α
                                                                                        jmp   n69_lit_string_α
n68_call_β:
                                                                                        jmp   n71_lit_string_α
#=======================================================================================================================
#         output = 'FAIL 912/017: ne(4,5-1) should fail (both=4)' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n69_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 44
                        mov              rax, qword ptr [rip + .Lx182_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n70_assign_α
.Lx182_0:
                        .quad            .Lx182_0_s
.Lx182_0_s:
                        .string          "FAIL 912/017: ne(4,5-1) should fail (both=4)"
#-----------------------------------------------------------------------------------------------------------------------
n70_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         ne('12', 12)                   :f(e018)
#-----------------------------------------------------------------------------------------------------------------------
n71_lit_string_α:
                        mov              qword ptr [rsp + 2016], 1
                        mov              dword ptr [rsp + 2020], 2
                        mov              rax, qword ptr [rip + .Lx184_0]
                        mov              qword ptr [rsp + 2024], rax
                                                                                        jmp   n72_lit_integer_α
.Lx184_0:
                        .quad            .Lx184_0_s
.Lx184_0_s:
                        .string          "12"
#-----------------------------------------------------------------------------------------------------------------------
n72_lit_integer_α:
                        mov              qword ptr [rsp + 2032], 6
                        mov              rax, qword ptr [rip + .Lx185_0]
                        mov              qword ptr [rsp + 2040], rax
                                                                                        jmp   n73_call_α
.Lx185_0:
                        .quad            12
#-----------------------------------------------------------------------------------------------------------------------
n73_call_α:
                        mov              rax, qword ptr [rsp + 2016]
                        mov              qword ptr [rsp + 1968], rax
                        mov              rax, qword ptr [rsp + 2024]
                        mov              qword ptr [rsp + 1976], rax
                        mov              rax, qword ptr [rsp + 2032]
                        mov              qword ptr [rsp + 1984], rax
                        mov              rax, qword ptr [rsp + 2040]
                        mov              qword ptr [rsp + 1992], rax
                        .section         .rodata
.Lbynamefn74:           .string          "ne"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn74]
                        lea              rsi, [rsp + 1968]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1952], rax
                        mov              qword ptr [rsp + 1960], rdx
                        cmp              eax, 99
                                                                                        je    n76_lit_string_α
                                                                                        jmp   n74_lit_string_α
n73_call_β:
                                                                                        jmp   n76_lit_string_α
#=======================================================================================================================
#         output = 'FAIL 912/018: ne(string,int) type mismatch' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n74_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 42
                        mov              rax, qword ptr [rip + .Lx187_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n75_assign_α
.Lx187_0:
                        .quad            .Lx187_0_s
.Lx187_0_s:
                        .string          "FAIL 912/018: ne(string,int) type mismatch"
#-----------------------------------------------------------------------------------------------------------------------
n75_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         output = 'PASS 912_num_pred (18/18)'
#-----------------------------------------------------------------------------------------------------------------------
n76_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 25
                        mov              rax, qword ptr [rip + .Lx189_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n77_assign_α
.Lx189_0:
                        .quad            .Lx189_0_s
.Lx189_0_s:
                        .string          "PASS 912_num_pred (18/18)"
#-----------------------------------------------------------------------------------------------------------------------
n77_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         output = 'FAIL 912/016: ge(7,5) should succeed' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n78_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 36
                        mov              rax, qword ptr [rip + .Lx191_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n79_assign_α
.Lx191_0:
                        .quad            .Lx191_0_s
.Lx191_0_s:
                        .string          "FAIL 912/016: ge(7,5) should succeed"
#-----------------------------------------------------------------------------------------------------------------------
n79_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         output = 'FAIL 912/015: ge(4,4) should succeed' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n80_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 36
                        mov              rax, qword ptr [rip + .Lx193_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n81_assign_α
.Lx193_0:
                        .quad            .Lx193_0_s
.Lx193_0_s:
                        .string          "FAIL 912/015: ge(4,4) should succeed"
#-----------------------------------------------------------------------------------------------------------------------
n81_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         output = 'FAIL 912/013: gt(5,2) should succeed' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n82_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 36
                        mov              rax, qword ptr [rip + .Lx195_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n83_assign_α
.Lx195_0:
                        .quad            .Lx195_0_s
.Lx195_0_s:
                        .string          "FAIL 912/013: gt(5,2) should succeed"
#-----------------------------------------------------------------------------------------------------------------------
n83_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         output = 'FAIL 912/010: ne(4,6) should succeed' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n84_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 36
                        mov              rax, qword ptr [rip + .Lx197_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n85_assign_α
.Lx197_0:
                        .quad            .Lx197_0_s
.Lx197_0_s:
                        .string          "FAIL 912/010: ne(4,6) should succeed"
#-----------------------------------------------------------------------------------------------------------------------
n85_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         output = 'FAIL 912/008: eq(5,5) should succeed' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n86_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 36
                        mov              rax, qword ptr [rip + .Lx199_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n87_assign_α
.Lx199_0:
                        .quad            .Lx199_0_s
.Lx199_0_s:
                        .string          "FAIL 912/008: eq(5,5) should succeed"
#-----------------------------------------------------------------------------------------------------------------------
n87_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         output = 'FAIL 912/006: le(4,10) should succeed' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n88_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 37
                        mov              rax, qword ptr [rip + .Lx201_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n89_assign_α
.Lx201_0:
                        .quad            .Lx201_0_s
.Lx201_0_s:
                        .string          "FAIL 912/006: le(4,10) should succeed"
#-----------------------------------------------------------------------------------------------------------------------
n89_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         output = 'FAIL 912/005: le(4,4) should succeed' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n90_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 36
                        mov              rax, qword ptr [rip + .Lx203_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n91_assign_α
.Lx203_0:
                        .quad            .Lx203_0_s
.Lx203_0_s:
                        .string          "FAIL 912/005: le(4,4) should succeed"
#-----------------------------------------------------------------------------------------------------------------------
n91_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         output = 'FAIL 912/003: lt(4,5) should succeed' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n92_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 36
                        mov              rax, qword ptr [rip + .Lx205_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n93_assign_α
.Lx205_0:
                        .quad            .Lx205_0_s
.Lx205_0_s:
                        .string          "FAIL 912/003: lt(4,5) should succeed"
#-----------------------------------------------------------------------------------------------------------------------
n93_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n94_goto_α:
                                                                                        jmp   n5_lit_integer_α
n94_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n95_goto_α:
                                                                                        jmp   n10_lit_integer_α
n95_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n96_goto_α:
                                                                                        jmp   n13_lit_integer_α
n96_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n97_goto_α:
                                                                                        jmp   n18_lit_integer_α
n97_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n98_goto_α:
                                                                                        jmp   n21_lit_integer_α
n98_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n99_goto_α:
                                                                                        jmp   n24_lit_integer_α
n99_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n100_goto_α:
                                                                                        jmp   n29_lit_integer_α
n100_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n101_goto_α:
                                                                                        jmp   n32_lit_integer_α
n101_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n102_goto_α:
                                                                                        jmp   n37_lit_integer_α
n102_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n103_goto_α:
                                                                                        jmp   n40_lit_integer_α
n103_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n104_goto_α:
                                                                                        jmp   n45_lit_integer_α
n104_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n105_goto_α:
                                                                                        jmp   n50_lit_integer_α
n105_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n106_goto_α:
                                                                                        jmp   n53_lit_integer_α
n106_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n107_goto_α:
                                                                                        jmp   n58_lit_integer_α
n107_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n108_goto_α:
                                                                                        jmp   n61_lit_integer_α
n108_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n109_goto_α:
                                                                                        jmp   n64_lit_integer_α
n109_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n110_goto_α:
                                                                                        jmp   n71_lit_string_α
n110_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n111_goto_α:
                                                                                        jmp   n76_lit_string_α
n111_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n112_goto_α:
                                                                                        jmp   main_γ
n112_goto_β:
                                                                                        jmp   main_ω
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

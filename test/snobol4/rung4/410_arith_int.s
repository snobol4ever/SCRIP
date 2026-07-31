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
                        sub              rsp, 1336
                        mov              rdi, rsp
                        mov              ecx, 1336
                        xor              eax, eax
                        rep stosb
main_α_body:
#=======================================================================================================================
#         differ(3 + 2, 5)                   :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n0_lit_integer_α:
                        mov              qword ptr [rsp + 80], 6
                        mov              rax, qword ptr [rip + .Lx75_0]
                        mov              qword ptr [rsp + 88], rax
                                                                                        jmp   n1_lit_integer_α
.Lx75_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n1_lit_integer_α:
                        mov              qword ptr [rsp + 96], 6
                        mov              rax, qword ptr [rip + .Lx76_0]
                        mov              qword ptr [rsp + 104], rax
                                                                                        jmp   n2_binop_α
.Lx76_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n2_binop_α:
                        mov              rdi, qword ptr [rsp + 80]
                        mov              rsi, qword ptr [rsp + 88]
                        mov              rdx, qword ptr [rsp + 96]
                        mov              rcx, qword ptr [rsp + 104]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        je    n7_lit_integer_α
                        mov              qword ptr [rsp + 64], rax
                        mov              qword ptr [rsp + 72], rdx
                                                                                        jmp   n3_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_integer_α:
                        mov              qword ptr [rsp + 112], 6
                        mov              rax, qword ptr [rip + .Lx78_0]
                        mov              qword ptr [rsp + 120], rax
                                                                                        jmp   n4_call_α
.Lx78_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n4_call_α:
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [rsp + 112]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 120]
                        mov              qword ptr [rsp + 40], rax
                        .section         .rodata
.Lbynamefn5:            .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn5]
                        lea              rsi, [rsp + 16]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 99
                                                                                        je    n7_lit_integer_α
                                                                                        jmp   n5_lit_string_α
n4_call_β:
                                                                                        jmp   n7_lit_integer_α
#=======================================================================================================================
#         output = 'FAIL 410/001: 3+2'                   :(end)
#-----------------------------------------------------------------------------------------------------------------------
n5_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 17
                        mov              rax, qword ptr [rip + .Lx80_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n6_assign_α
.Lx80_0:
                        .quad            .Lx80_0_s
.Lx80_0_s:
                        .string          "FAIL 410/001: 3+2"
#-----------------------------------------------------------------------------------------------------------------------
n6_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         differ(3 - 2, 1)                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n7_lit_integer_α:
                        mov              qword ptr [rsp + 240], 6
                        mov              rax, qword ptr [rip + .Lx82_0]
                        mov              qword ptr [rsp + 248], rax
                                                                                        jmp   n8_lit_integer_α
.Lx82_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n8_lit_integer_α:
                        mov              qword ptr [rsp + 256], 6
                        mov              rax, qword ptr [rip + .Lx83_0]
                        mov              qword ptr [rsp + 264], rax
                                                                                        jmp   n9_binop_α
.Lx83_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n9_binop_α:
                        mov              rdi, qword ptr [rsp + 240]
                        mov              rsi, qword ptr [rsp + 248]
                        mov              rdx, qword ptr [rsp + 256]
                        mov              rcx, qword ptr [rsp + 264]
                        call             rt_sub@PLT
                        cmp              eax, 99
                                                                                        je    n14_lit_integer_α
                        mov              qword ptr [rsp + 224], rax
                        mov              qword ptr [rsp + 232], rdx
                                                                                        jmp   n10_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n10_lit_integer_α:
                        mov              qword ptr [rsp + 272], 6
                        mov              rax, qword ptr [rip + .Lx85_0]
                        mov              qword ptr [rsp + 280], rax
                                                                                        jmp   n11_call_α
.Lx85_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n11_call_α:
                        mov              rax, qword ptr [rsp + 224]
                        mov              qword ptr [rsp + 176], rax
                        mov              rax, qword ptr [rsp + 232]
                        mov              qword ptr [rsp + 184], rax
                        mov              rax, qword ptr [rsp + 272]
                        mov              qword ptr [rsp + 192], rax
                        mov              rax, qword ptr [rsp + 280]
                        mov              qword ptr [rsp + 200], rax
                        .section         .rodata
.Lbynamefn12:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn12]
                        lea              rsi, [rsp + 176]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 160], rax
                        mov              qword ptr [rsp + 168], rdx
                        cmp              eax, 99
                                                                                        je    n14_lit_integer_α
                                                                                        jmp   n12_lit_string_α
n11_call_β:
                                                                                        jmp   n14_lit_integer_α
#=======================================================================================================================
#         output = 'FAIL 410/002: 3-2'                   :(end)
#-----------------------------------------------------------------------------------------------------------------------
n12_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 17
                        mov              rax, qword ptr [rip + .Lx87_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n13_assign_α
.Lx87_0:
                        .quad            .Lx87_0_s
.Lx87_0_s:
                        .string          "FAIL 410/002: 3-2"
#-----------------------------------------------------------------------------------------------------------------------
n13_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         differ(3 * 2, 6)                   :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n14_lit_integer_α:
                        mov              qword ptr [rsp + 384], 6
                        mov              rax, qword ptr [rip + .Lx89_0]
                        mov              qword ptr [rsp + 392], rax
                                                                                        jmp   n15_lit_integer_α
.Lx89_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n15_lit_integer_α:
                        mov              qword ptr [rsp + 400], 6
                        mov              rax, qword ptr [rip + .Lx90_0]
                        mov              qword ptr [rsp + 408], rax
                                                                                        jmp   n16_binop_α
.Lx90_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n16_binop_α:
                        mov              rdi, qword ptr [rsp + 384]
                        mov              rsi, qword ptr [rsp + 392]
                        mov              rdx, qword ptr [rsp + 400]
                        mov              rcx, qword ptr [rsp + 408]
                        call             rt_mul@PLT
                        cmp              eax, 99
                                                                                        je    n21_lit_integer_α
                        mov              qword ptr [rsp + 368], rax
                        mov              qword ptr [rsp + 376], rdx
                                                                                        jmp   n17_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n17_lit_integer_α:
                        mov              qword ptr [rsp + 416], 6
                        mov              rax, qword ptr [rip + .Lx92_0]
                        mov              qword ptr [rsp + 424], rax
                                                                                        jmp   n18_call_α
.Lx92_0:
                        .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n18_call_α:
                        mov              rax, qword ptr [rsp + 368]
                        mov              qword ptr [rsp + 320], rax
                        mov              rax, qword ptr [rsp + 376]
                        mov              qword ptr [rsp + 328], rax
                        mov              rax, qword ptr [rsp + 416]
                        mov              qword ptr [rsp + 336], rax
                        mov              rax, qword ptr [rsp + 424]
                        mov              qword ptr [rsp + 344], rax
                        .section         .rodata
.Lbynamefn19:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn19]
                        lea              rsi, [rsp + 320]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 304], rax
                        mov              qword ptr [rsp + 312], rdx
                        cmp              eax, 99
                                                                                        je    n21_lit_integer_α
                                                                                        jmp   n19_lit_string_α
n18_call_β:
                                                                                        jmp   n21_lit_integer_α
#=======================================================================================================================
#         output = 'FAIL 410/003: 3*2'                   :(end)
#-----------------------------------------------------------------------------------------------------------------------
n19_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 17
                        mov              rax, qword ptr [rip + .Lx94_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n20_assign_α
.Lx94_0:
                        .quad            .Lx94_0_s
.Lx94_0_s:
                        .string          "FAIL 410/003: 3*2"
#-----------------------------------------------------------------------------------------------------------------------
n20_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         differ(5 / 2, 2)                   :f(e004)
#-----------------------------------------------------------------------------------------------------------------------
n21_lit_integer_α:
                        mov              qword ptr [rsp + 528], 6
                        mov              rax, qword ptr [rip + .Lx96_0]
                        mov              qword ptr [rsp + 536], rax
                                                                                        jmp   n22_lit_integer_α
.Lx96_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n22_lit_integer_α:
                        mov              qword ptr [rsp + 544], 6
                        mov              rax, qword ptr [rip + .Lx97_0]
                        mov              qword ptr [rsp + 552], rax
                                                                                        jmp   n23_binop_α
.Lx97_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n23_binop_α:
                        mov              rdi, qword ptr [rsp + 528]
                        mov              rsi, qword ptr [rsp + 536]
                        mov              rdx, qword ptr [rsp + 544]
                        mov              rcx, qword ptr [rsp + 552]
                        call             rt_div@PLT
                        cmp              eax, 99
                                                                                        je    n28_lit_integer_α
                        mov              qword ptr [rsp + 512], rax
                        mov              qword ptr [rsp + 520], rdx
                                                                                        jmp   n24_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n24_lit_integer_α:
                        mov              qword ptr [rsp + 560], 6
                        mov              rax, qword ptr [rip + .Lx99_0]
                        mov              qword ptr [rsp + 568], rax
                                                                                        jmp   n25_call_α
.Lx99_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n25_call_α:
                        mov              rax, qword ptr [rsp + 512]
                        mov              qword ptr [rsp + 464], rax
                        mov              rax, qword ptr [rsp + 520]
                        mov              qword ptr [rsp + 472], rax
                        mov              rax, qword ptr [rsp + 560]
                        mov              qword ptr [rsp + 480], rax
                        mov              rax, qword ptr [rsp + 568]
                        mov              qword ptr [rsp + 488], rax
                        .section         .rodata
.Lbynamefn26:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn26]
                        lea              rsi, [rsp + 464]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 448], rax
                        mov              qword ptr [rsp + 456], rdx
                        cmp              eax, 99
                                                                                        je    n28_lit_integer_α
                                                                                        jmp   n26_lit_string_α
n25_call_β:
                                                                                        jmp   n28_lit_integer_α
#=======================================================================================================================
#         output = 'FAIL 410/004: 5/2 integer division'  :(end)
#-----------------------------------------------------------------------------------------------------------------------
n26_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 34
                        mov              rax, qword ptr [rip + .Lx101_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n27_assign_α
.Lx101_0:
                        .quad            .Lx101_0_s
.Lx101_0_s:
                        .string          "FAIL 410/004: 5/2 integer division"
#-----------------------------------------------------------------------------------------------------------------------
n27_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         differ(2 ** 3, 8)                   :f(e005)
#-----------------------------------------------------------------------------------------------------------------------
n28_lit_integer_α:
                        mov              qword ptr [rsp + 672], 6
                        mov              rax, qword ptr [rip + .Lx103_0]
                        mov              qword ptr [rsp + 680], rax
                                                                                        jmp   n29_lit_integer_α
.Lx103_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n29_lit_integer_α:
                        mov              qword ptr [rsp + 688], 6
                        mov              rax, qword ptr [rip + .Lx104_0]
                        mov              qword ptr [rsp + 696], rax
                                                                                        jmp   n30_binop_α
.Lx104_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n30_binop_α:
                        mov              rdi, qword ptr [rsp + 672]
                        mov              rsi, qword ptr [rsp + 680]
                        mov              rdx, qword ptr [rsp + 688]
                        mov              rcx, qword ptr [rsp + 696]
                        call             rt_pow@PLT
                        cmp              eax, 99
                                                                                        je    n35_lit_string_α
                        mov              qword ptr [rsp + 656], rax
                        mov              qword ptr [rsp + 664], rdx
                                                                                        jmp   n31_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n31_lit_integer_α:
                        mov              qword ptr [rsp + 704], 6
                        mov              rax, qword ptr [rip + .Lx106_0]
                        mov              qword ptr [rsp + 712], rax
                                                                                        jmp   n32_call_α
.Lx106_0:
                        .quad            8
#-----------------------------------------------------------------------------------------------------------------------
n32_call_α:
                        mov              rax, qword ptr [rsp + 656]
                        mov              qword ptr [rsp + 608], rax
                        mov              rax, qword ptr [rsp + 664]
                        mov              qword ptr [rsp + 616], rax
                        mov              rax, qword ptr [rsp + 704]
                        mov              qword ptr [rsp + 624], rax
                        mov              rax, qword ptr [rsp + 712]
                        mov              qword ptr [rsp + 632], rax
                        .section         .rodata
.Lbynamefn33:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn33]
                        lea              rsi, [rsp + 608]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 592], rax
                        mov              qword ptr [rsp + 600], rdx
                        cmp              eax, 99
                                                                                        je    n35_lit_string_α
                                                                                        jmp   n33_lit_string_α
n32_call_β:
                                                                                        jmp   n35_lit_string_α
#=======================================================================================================================
#         output = 'FAIL 410/005: 2**3'                  :(end)
#-----------------------------------------------------------------------------------------------------------------------
n33_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 18
                        mov              rax, qword ptr [rip + .Lx108_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n34_assign_α
.Lx108_0:
                        .quad            .Lx108_0_s
.Lx108_0_s:
                        .string          "FAIL 410/005: 2**3"
#-----------------------------------------------------------------------------------------------------------------------
n34_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         differ('3' + 2, 5)                   :f(e006)
#-----------------------------------------------------------------------------------------------------------------------
n35_lit_string_α:
                        mov              qword ptr [rsp + 816], 1
                        mov              dword ptr [rsp + 820], 1
                        mov              rax, qword ptr [rip + .Lx110_0]
                        mov              qword ptr [rsp + 824], rax
                                                                                        jmp   n36_lit_integer_α
.Lx110_0:
                        .quad            .Lx110_0_s
.Lx110_0_s:
                        .string          "3"
#-----------------------------------------------------------------------------------------------------------------------
n36_lit_integer_α:
                        mov              qword ptr [rsp + 832], 6
                        mov              rax, qword ptr [rip + .Lx111_0]
                        mov              qword ptr [rsp + 840], rax
                                                                                        jmp   n37_binop_α
.Lx111_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n37_binop_α:
                        mov              eax, dword ptr [rsp + 816]
                        cmp              eax, 6
                                                                                        jne   .Lx112_0
                        mov              rax, qword ptr [rsp + 824]
                        mov              rcx, 2
                        add              rax, rcx
                        mov              qword ptr [rsp + 800], 6
                        mov              qword ptr [rsp + 808], rax
                                                                                        jmp   n38_lit_integer_α
.Lx112_0:
                        mov              rdi, qword ptr [rsp + 816]
                        mov              rsi, qword ptr [rsp + 824]
                        mov              rdx, qword ptr [rsp + 832]
                        mov              rcx, qword ptr [rsp + 840]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        je    n42_lit_integer_α
                        mov              qword ptr [rsp + 800], rax
                        mov              qword ptr [rsp + 808], rdx
                                                                                        jmp   n38_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n38_lit_integer_α:
                        mov              qword ptr [rsp + 848], 6
                        mov              rax, qword ptr [rip + .Lx113_0]
                        mov              qword ptr [rsp + 856], rax
                                                                                        jmp   n39_call_α
.Lx113_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n39_call_α:
                        mov              rax, qword ptr [rsp + 800]
                        mov              qword ptr [rsp + 752], rax
                        mov              rax, qword ptr [rsp + 808]
                        mov              qword ptr [rsp + 760], rax
                        mov              rax, qword ptr [rsp + 848]
                        mov              qword ptr [rsp + 768], rax
                        mov              rax, qword ptr [rsp + 856]
                        mov              qword ptr [rsp + 776], rax
                        .section         .rodata
.Lbynamefn40:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn40]
                        lea              rsi, [rsp + 752]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 736], rax
                        mov              qword ptr [rsp + 744], rdx
                        cmp              eax, 99
                                                                                        je    n42_lit_integer_α
                                                                                        jmp   n40_lit_string_α
n39_call_β:
                                                                                        jmp   n42_lit_integer_α
#=======================================================================================================================
#         output = 'FAIL 410/006: string+int coerce'     :(end)
#-----------------------------------------------------------------------------------------------------------------------
n40_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 31
                        mov              rax, qword ptr [rip + .Lx115_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n41_assign_α
.Lx115_0:
                        .quad            .Lx115_0_s
.Lx115_0_s:
                        .string          "FAIL 410/006: string+int coerce"
#-----------------------------------------------------------------------------------------------------------------------
n41_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         differ(3 + '-2', 1)                   :f(e007)
#-----------------------------------------------------------------------------------------------------------------------
n42_lit_integer_α:
                        mov              qword ptr [rsp + 960], 6
                        mov              rax, qword ptr [rip + .Lx117_0]
                        mov              qword ptr [rsp + 968], rax
                                                                                        jmp   n43_lit_string_α
.Lx117_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n43_lit_string_α:
                        mov              qword ptr [rsp + 976], 1
                        mov              dword ptr [rsp + 980], 2
                        mov              rax, qword ptr [rip + .Lx118_0]
                        mov              qword ptr [rsp + 984], rax
                                                                                        jmp   n44_binop_α
.Lx118_0:
                        .quad            .Lx118_0_s
.Lx118_0_s:
                        .string          "-2"
#-----------------------------------------------------------------------------------------------------------------------
n44_binop_α:
                        mov              eax, dword ptr [rsp + 976]
                        cmp              eax, 6
                                                                                        jne   .Lx119_0
                        mov              rax, 3
                        mov              rcx, qword ptr [rsp + 984]
                        add              rax, rcx
                        mov              qword ptr [rsp + 944], 6
                        mov              qword ptr [rsp + 952], rax
                                                                                        jmp   n45_lit_integer_α
.Lx119_0:
                        mov              rdi, qword ptr [rsp + 960]
                        mov              rsi, qword ptr [rsp + 968]
                        mov              rdx, qword ptr [rsp + 976]
                        mov              rcx, qword ptr [rsp + 984]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        je    n49_lit_string_α
                        mov              qword ptr [rsp + 944], rax
                        mov              qword ptr [rsp + 952], rdx
                                                                                        jmp   n45_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n45_lit_integer_α:
                        mov              qword ptr [rsp + 992], 6
                        mov              rax, qword ptr [rip + .Lx120_0]
                        mov              qword ptr [rsp + 1000], rax
                                                                                        jmp   n46_call_α
.Lx120_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n46_call_α:
                        mov              rax, qword ptr [rsp + 944]
                        mov              qword ptr [rsp + 896], rax
                        mov              rax, qword ptr [rsp + 952]
                        mov              qword ptr [rsp + 904], rax
                        mov              rax, qword ptr [rsp + 992]
                        mov              qword ptr [rsp + 912], rax
                        mov              rax, qword ptr [rsp + 1000]
                        mov              qword ptr [rsp + 920], rax
                        .section         .rodata
.Lbynamefn47:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn47]
                        lea              rsi, [rsp + 896]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 880], rax
                        mov              qword ptr [rsp + 888], rdx
                        cmp              eax, 99
                                                                                        je    n49_lit_string_α
                                                                                        jmp   n47_lit_string_α
n46_call_β:
                                                                                        jmp   n49_lit_string_α
#=======================================================================================================================
#         output = 'FAIL 410/007: int+neg-string'        :(end)
#-----------------------------------------------------------------------------------------------------------------------
n47_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 28
                        mov              rax, qword ptr [rip + .Lx122_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n48_assign_α
.Lx122_0:
                        .quad            .Lx122_0_s
.Lx122_0_s:
                        .string          "FAIL 410/007: int+neg-string"
#-----------------------------------------------------------------------------------------------------------------------
n48_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         differ('1' + '0', 1)                   :f(e008)
#-----------------------------------------------------------------------------------------------------------------------
n49_lit_string_α:
                        mov              qword ptr [rsp + 1104], 1
                        mov              dword ptr [rsp + 1108], 1
                        mov              rax, qword ptr [rip + .Lx124_0]
                        mov              qword ptr [rsp + 1112], rax
                                                                                        jmp   n50_lit_string_α
.Lx124_0:
                        .quad            .Lx124_0_s
.Lx124_0_s:
                        .string          "1"
#-----------------------------------------------------------------------------------------------------------------------
n50_lit_string_α:
                        mov              qword ptr [rsp + 1120], 1
                        mov              dword ptr [rsp + 1124], 1
                        mov              rax, qword ptr [rip + .Lx125_0]
                        mov              qword ptr [rsp + 1128], rax
                                                                                        jmp   n51_binop_α
.Lx125_0:
                        .quad            .Lx125_0_s
.Lx125_0_s:
                        .string          "0"
#-----------------------------------------------------------------------------------------------------------------------
n51_binop_α:
                        mov              eax, dword ptr [rsp + 1104]
                        cmp              eax, 6
                                                                                        jne   .Lx126_0
                        mov              eax, dword ptr [rsp + 1120]
                        cmp              eax, 6
                                                                                        jne   .Lx126_0
                        mov              rax, qword ptr [rsp + 1112]
                        mov              rcx, qword ptr [rsp + 1128]
                        add              rax, rcx
                        mov              qword ptr [rsp + 1088], 6
                        mov              qword ptr [rsp + 1096], rax
                                                                                        jmp   n52_lit_integer_α
.Lx126_0:
                        mov              rdi, qword ptr [rsp + 1104]
                        mov              rsi, qword ptr [rsp + 1112]
                        mov              rdx, qword ptr [rsp + 1120]
                        mov              rcx, qword ptr [rsp + 1128]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        je    n56_lit_integer_α
                        mov              qword ptr [rsp + 1088], rax
                        mov              qword ptr [rsp + 1096], rdx
                                                                                        jmp   n52_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n52_lit_integer_α:
                        mov              qword ptr [rsp + 1136], 6
                        mov              rax, qword ptr [rip + .Lx127_0]
                        mov              qword ptr [rsp + 1144], rax
                                                                                        jmp   n53_call_α
.Lx127_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n53_call_α:
                        mov              rax, qword ptr [rsp + 1088]
                        mov              qword ptr [rsp + 1040], rax
                        mov              rax, qword ptr [rsp + 1096]
                        mov              qword ptr [rsp + 1048], rax
                        mov              rax, qword ptr [rsp + 1136]
                        mov              qword ptr [rsp + 1056], rax
                        mov              rax, qword ptr [rsp + 1144]
                        mov              qword ptr [rsp + 1064], rax
                        .section         .rodata
.Lbynamefn54:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn54]
                        lea              rsi, [rsp + 1040]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1024], rax
                        mov              qword ptr [rsp + 1032], rdx
                        cmp              eax, 99
                                                                                        je    n56_lit_integer_α
                                                                                        jmp   n54_lit_string_α
n53_call_β:
                                                                                        jmp   n56_lit_integer_α
#=======================================================================================================================
#         output = 'FAIL 410/008: string+string coerce'  :(end)
#-----------------------------------------------------------------------------------------------------------------------
n54_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 34
                        mov              rax, qword ptr [rip + .Lx129_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n55_assign_α
.Lx129_0:
                        .quad            .Lx129_0_s
.Lx129_0_s:
                        .string          "FAIL 410/008: string+string coerce"
#-----------------------------------------------------------------------------------------------------------------------
n55_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         differ(5 + '', 5)                   :f(e009)
#-----------------------------------------------------------------------------------------------------------------------
n56_lit_integer_α:
                        mov              qword ptr [rsp + 1248], 6
                        mov              rax, qword ptr [rip + .Lx131_0]
                        mov              qword ptr [rsp + 1256], rax
                                                                                        jmp   n57_lit_string_α
.Lx131_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n57_lit_string_α:
                        mov              qword ptr [rsp + 1264], 1
                        mov              dword ptr [rsp + 1268], 0
                        mov              rax, qword ptr [rip + .Lx132_0]
                        mov              qword ptr [rsp + 1272], rax
                                                                                        jmp   n58_binop_α
.Lx132_0:
                        .quad            .Lx132_0_s
.Lx132_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n58_binop_α:
                        mov              eax, dword ptr [rsp + 1264]
                        cmp              eax, 6
                                                                                        jne   .Lx133_0
                        mov              rax, 5
                        mov              rcx, qword ptr [rsp + 1272]
                        add              rax, rcx
                        mov              qword ptr [rsp + 1232], 6
                        mov              qword ptr [rsp + 1240], rax
                                                                                        jmp   n59_lit_integer_α
.Lx133_0:
                        mov              rdi, qword ptr [rsp + 1248]
                        mov              rsi, qword ptr [rsp + 1256]
                        mov              rdx, qword ptr [rsp + 1264]
                        mov              rcx, qword ptr [rsp + 1272]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        je    n63_lit_string_α
                        mov              qword ptr [rsp + 1232], rax
                        mov              qword ptr [rsp + 1240], rdx
                                                                                        jmp   n59_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n59_lit_integer_α:
                        mov              qword ptr [rsp + 1280], 6
                        mov              rax, qword ptr [rip + .Lx134_0]
                        mov              qword ptr [rsp + 1288], rax
                                                                                        jmp   n60_call_α
.Lx134_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n60_call_α:
                        mov              rax, qword ptr [rsp + 1232]
                        mov              qword ptr [rsp + 1184], rax
                        mov              rax, qword ptr [rsp + 1240]
                        mov              qword ptr [rsp + 1192], rax
                        mov              rax, qword ptr [rsp + 1280]
                        mov              qword ptr [rsp + 1200], rax
                        mov              rax, qword ptr [rsp + 1288]
                        mov              qword ptr [rsp + 1208], rax
                        .section         .rodata
.Lbynamefn61:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn61]
                        lea              rsi, [rsp + 1184]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1168], rax
                        mov              qword ptr [rsp + 1176], rdx
                        cmp              eax, 99
                                                                                        je    n63_lit_string_α
                                                                                        jmp   n61_lit_string_α
n60_call_β:
                                                                                        jmp   n63_lit_string_α
#=======================================================================================================================
#         output = 'FAIL 410/009: null addend is zero'   :(end)
#-----------------------------------------------------------------------------------------------------------------------
n61_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx136_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n62_assign_α
.Lx136_0:
                        .quad            .Lx136_0_s
.Lx136_0_s:
                        .string          "FAIL 410/009: null addend is zero"
#-----------------------------------------------------------------------------------------------------------------------
n62_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         output = 'PASS 410_arith_int (9/9)'
#-----------------------------------------------------------------------------------------------------------------------
n63_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 24
                        mov              rax, qword ptr [rip + .Lx138_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n64_assign_α
.Lx138_0:
                        .quad            .Lx138_0_s
.Lx138_0_s:
                        .string          "PASS 410_arith_int (9/9)"
#-----------------------------------------------------------------------------------------------------------------------
n64_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n65_goto_α:
                                                                                        jmp   n7_lit_integer_α
n65_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n66_goto_α:
                                                                                        jmp   n14_lit_integer_α
n66_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n67_goto_α:
                                                                                        jmp   n21_lit_integer_α
n67_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n68_goto_α:
                                                                                        jmp   n28_lit_integer_α
n68_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n69_goto_α:
                                                                                        jmp   n35_lit_string_α
n69_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n70_goto_α:
                                                                                        jmp   n42_lit_integer_α
n70_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n71_goto_α:
                                                                                        jmp   n49_lit_string_α
n71_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n72_goto_α:
                                                                                        jmp   n56_lit_integer_α
n72_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n73_goto_α:
                                                                                        jmp   n63_lit_string_α
n73_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n74_goto_α:
                                                                                        jmp   main_γ
n74_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        add              rsp, 1336
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              eax, 99
                        xor              edx, edx
                        add              rsp, 1336
                        ret
                        .section         .note.GNU-stack,"",@progbits

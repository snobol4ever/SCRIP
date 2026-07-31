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
                        sub              rsp, 1704
                        mov              rdi, rsp
                        mov              ecx, 1704
                        xor              eax, eax
                        rep stosb
main_α_body:
#=======================================================================================================================
# jlab    define('jlab(a,b,c)d,e,f')
#-----------------------------------------------------------------------------------------------------------------------
n0_lit_string_α:
                        mov              qword ptr [rsp + 48], 1
                        mov              dword ptr [rsp + 52], 16
                        mov              rax, qword ptr [rip + .Lx70_0]
                        mov              qword ptr [rsp + 56], rax
                                                                                        jmp   n1_call_α
.Lx70_0:
                        .quad            .Lx70_0_s
.Lx70_0_s:
                        .string          "jlab(a,b,c)d,e,f"
#-----------------------------------------------------------------------------------------------------------------------
n1_call_α:
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 24], rax
                        .section         .rodata
.Lbynamefn2:            .string          "define"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn2]
                        lea              rsi, [rsp + 16]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 99
                                                                                        je    n2_lit_string_α
                                                                                        jmp   n2_lit_string_α
n1_call_β:
                                                                                        jmp   n2_lit_string_α
#=======================================================================================================================
#         differ(arg(.jlab, 1), 'A')                   :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n2_lit_string_α:
                        mov              qword ptr [rsp + 240], 1
                        mov              dword ptr [rsp + 244], 4
                        mov              rax, qword ptr [rip + .Lx72_0]
                        mov              qword ptr [rsp + 248], rax
                                                                                        jmp   n3_call_α
.Lx72_0:
                        .quad            .Lx72_0_s
.Lx72_0_s:
                        .string          "jlab"
#-----------------------------------------------------------------------------------------------------------------------
n3_call_α:
                        mov              rax, qword ptr [rsp + 240]
                        mov              qword ptr [rsp + 208], rax
                        mov              rax, qword ptr [rsp + 248]
                        mov              qword ptr [rsp + 216], rax
                        .section         .rodata
.Lrkfn74:               .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn74]
                        lea              rsi, [rsp + 208]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 192], rax
                        mov              qword ptr [rsp + 200], rdx
                        cmp              eax, 99
                                                                                        je    n10_lit_string_α
                                                                                        jmp   n4_lit_integer_α
n3_call_β:
                                                                                        jmp   n10_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n4_lit_integer_α:
                        mov              qword ptr [rsp + 256], 6
                        mov              rax, qword ptr [rip + .Lx75_0]
                        mov              qword ptr [rsp + 264], rax
                                                                                        jmp   n5_call_α
.Lx75_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n5_call_α:
                        mov              rax, qword ptr [rsp + 192]
                        mov              qword ptr [rsp + 144], rax
                        mov              rax, qword ptr [rsp + 200]
                        mov              qword ptr [rsp + 152], rax
                        mov              rax, qword ptr [rsp + 256]
                        mov              qword ptr [rsp + 160], rax
                        mov              rax, qword ptr [rsp + 264]
                        mov              qword ptr [rsp + 168], rax
                        .section         .rodata
.Lbynamefn6:            .string          "arg"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn6]
                        lea              rsi, [rsp + 144]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 128], rax
                        mov              qword ptr [rsp + 136], rdx
                        cmp              eax, 99
                                                                                        je    n10_lit_string_α
                                                                                        jmp   n6_lit_string_α
n5_call_β:
                                                                                        jmp   n10_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n6_lit_string_α:
                        mov              qword ptr [rsp + 272], 1
                        mov              dword ptr [rsp + 276], 1
                        mov              rax, qword ptr [rip + .Lx77_0]
                        mov              qword ptr [rsp + 280], rax
                                                                                        jmp   n7_call_α
.Lx77_0:
                        .quad            .Lx77_0_s
.Lx77_0_s:
                        .string          "A"
#-----------------------------------------------------------------------------------------------------------------------
n7_call_α:
                        mov              rax, qword ptr [rsp + 128]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [rsp + 136]
                        mov              qword ptr [rsp + 88], rax
                        mov              rax, qword ptr [rsp + 272]
                        mov              qword ptr [rsp + 96], rax
                        mov              rax, qword ptr [rsp + 280]
                        mov              qword ptr [rsp + 104], rax
                        .section         .rodata
.Lbynamefn8:            .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn8]
                        lea              rsi, [rsp + 80]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 64], rax
                        mov              qword ptr [rsp + 72], rdx
                        cmp              eax, 99
                                                                                        je    n10_lit_string_α
                                                                                        jmp   n8_lit_string_α
n7_call_β:
                                                                                        jmp   n10_lit_string_α
#=======================================================================================================================
#         output = 'FAIL 1017/001: arg(.jlab,1) = A'     :(end)
#-----------------------------------------------------------------------------------------------------------------------
n8_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 31
                        mov              rax, qword ptr [rip + .Lx79_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n9_assign_α
.Lx79_0:
                        .quad            .Lx79_0_s
.Lx79_0_s:
                        .string          "FAIL 1017/001: arg(.jlab,1) = A"
#-----------------------------------------------------------------------------------------------------------------------
n9_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         differ(arg(.jlab, 3), 'C')                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n10_lit_string_α:
                        mov              qword ptr [rsp + 496], 1
                        mov              dword ptr [rsp + 500], 4
                        mov              rax, qword ptr [rip + .Lx81_0]
                        mov              qword ptr [rsp + 504], rax
                                                                                        jmp   n11_call_α
.Lx81_0:
                        .quad            .Lx81_0_s
.Lx81_0_s:
                        .string          "jlab"
#-----------------------------------------------------------------------------------------------------------------------
n11_call_α:
                        mov              rax, qword ptr [rsp + 496]
                        mov              qword ptr [rsp + 464], rax
                        mov              rax, qword ptr [rsp + 504]
                        mov              qword ptr [rsp + 472], rax
                        .section         .rodata
.Lrkfn83:               .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn83]
                        lea              rsi, [rsp + 464]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 448], rax
                        mov              qword ptr [rsp + 456], rdx
                        cmp              eax, 99
                                                                                        je    n18_lit_string_α
                                                                                        jmp   n12_lit_integer_α
n11_call_β:
                                                                                        jmp   n18_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n12_lit_integer_α:
                        mov              qword ptr [rsp + 512], 6
                        mov              rax, qword ptr [rip + .Lx84_0]
                        mov              qword ptr [rsp + 520], rax
                                                                                        jmp   n13_call_α
.Lx84_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n13_call_α:
                        mov              rax, qword ptr [rsp + 448]
                        mov              qword ptr [rsp + 400], rax
                        mov              rax, qword ptr [rsp + 456]
                        mov              qword ptr [rsp + 408], rax
                        mov              rax, qword ptr [rsp + 512]
                        mov              qword ptr [rsp + 416], rax
                        mov              rax, qword ptr [rsp + 520]
                        mov              qword ptr [rsp + 424], rax
                        .section         .rodata
.Lbynamefn14:           .string          "arg"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn14]
                        lea              rsi, [rsp + 400]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 384], rax
                        mov              qword ptr [rsp + 392], rdx
                        cmp              eax, 99
                                                                                        je    n18_lit_string_α
                                                                                        jmp   n14_lit_string_α
n13_call_β:
                                                                                        jmp   n18_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n14_lit_string_α:
                        mov              qword ptr [rsp + 528], 1
                        mov              dword ptr [rsp + 532], 1
                        mov              rax, qword ptr [rip + .Lx86_0]
                        mov              qword ptr [rsp + 536], rax
                                                                                        jmp   n15_call_α
.Lx86_0:
                        .quad            .Lx86_0_s
.Lx86_0_s:
                        .string          "C"
#-----------------------------------------------------------------------------------------------------------------------
n15_call_α:
                        mov              rax, qword ptr [rsp + 384]
                        mov              qword ptr [rsp + 336], rax
                        mov              rax, qword ptr [rsp + 392]
                        mov              qword ptr [rsp + 344], rax
                        mov              rax, qword ptr [rsp + 528]
                        mov              qword ptr [rsp + 352], rax
                        mov              rax, qword ptr [rsp + 536]
                        mov              qword ptr [rsp + 360], rax
                        .section         .rodata
.Lbynamefn16:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn16]
                        lea              rsi, [rsp + 336]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 320], rax
                        mov              qword ptr [rsp + 328], rdx
                        cmp              eax, 99
                                                                                        je    n18_lit_string_α
                                                                                        jmp   n16_lit_string_α
n15_call_β:
                                                                                        jmp   n18_lit_string_α
#=======================================================================================================================
#         output = 'FAIL 1017/002: arg(.jlab,3) = C'     :(end)
#-----------------------------------------------------------------------------------------------------------------------
n16_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 31
                        mov              rax, qword ptr [rip + .Lx88_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n17_assign_α
.Lx88_0:
                        .quad            .Lx88_0_s
.Lx88_0_s:
                        .string          "FAIL 1017/002: arg(.jlab,3) = C"
#-----------------------------------------------------------------------------------------------------------------------
n17_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         arg(.jlab, 0)              :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n18_lit_string_α:
                        mov              qword ptr [rsp + 672], 1
                        mov              dword ptr [rsp + 676], 4
                        mov              rax, qword ptr [rip + .Lx90_0]
                        mov              qword ptr [rsp + 680], rax
                                                                                        jmp   n19_call_α
.Lx90_0:
                        .quad            .Lx90_0_s
.Lx90_0_s:
                        .string          "jlab"
#-----------------------------------------------------------------------------------------------------------------------
n19_call_α:
                        mov              rax, qword ptr [rsp + 672]
                        mov              qword ptr [rsp + 640], rax
                        mov              rax, qword ptr [rsp + 680]
                        mov              qword ptr [rsp + 648], rax
                        .section         .rodata
.Lrkfn92:               .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn92]
                        lea              rsi, [rsp + 640]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 624], rax
                        mov              qword ptr [rsp + 632], rdx
                        cmp              eax, 99
                                                                                        je    n24_lit_string_α
                                                                                        jmp   n20_lit_integer_α
n19_call_β:
                                                                                        jmp   n24_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n20_lit_integer_α:
                        mov              qword ptr [rsp + 688], 6
                        mov              rax, qword ptr [rip + .Lx93_0]
                        mov              qword ptr [rsp + 696], rax
                                                                                        jmp   n21_call_α
.Lx93_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n21_call_α:
                        mov              rax, qword ptr [rsp + 624]
                        mov              qword ptr [rsp + 576], rax
                        mov              rax, qword ptr [rsp + 632]
                        mov              qword ptr [rsp + 584], rax
                        mov              rax, qword ptr [rsp + 688]
                        mov              qword ptr [rsp + 592], rax
                        mov              rax, qword ptr [rsp + 696]
                        mov              qword ptr [rsp + 600], rax
                        .section         .rodata
.Lbynamefn22:           .string          "arg"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn22]
                        lea              rsi, [rsp + 576]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 560], rax
                        mov              qword ptr [rsp + 568], rdx
                        cmp              eax, 99
                                                                                        je    n24_lit_string_α
                                                                                        jmp   n22_lit_string_α
n21_call_β:
                                                                                        jmp   n24_lit_string_α
#=======================================================================================================================
#         output = 'FAIL 1017/003: arg(.jlab,0) OOB should fail' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n22_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 43
                        mov              rax, qword ptr [rip + .Lx95_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n23_assign_α
.Lx95_0:
                        .quad            .Lx95_0_s
.Lx95_0_s:
                        .string          "FAIL 1017/003: arg(.jlab,0) OOB should fail"
#-----------------------------------------------------------------------------------------------------------------------
n23_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         arg(.jlab, 4)              :f(e004)
#-----------------------------------------------------------------------------------------------------------------------
n24_lit_string_α:
                        mov              qword ptr [rsp + 832], 1
                        mov              dword ptr [rsp + 836], 4
                        mov              rax, qword ptr [rip + .Lx97_0]
                        mov              qword ptr [rsp + 840], rax
                                                                                        jmp   n25_call_α
.Lx97_0:
                        .quad            .Lx97_0_s
.Lx97_0_s:
                        .string          "jlab"
#-----------------------------------------------------------------------------------------------------------------------
n25_call_α:
                        mov              rax, qword ptr [rsp + 832]
                        mov              qword ptr [rsp + 800], rax
                        mov              rax, qword ptr [rsp + 840]
                        mov              qword ptr [rsp + 808], rax
                        .section         .rodata
.Lrkfn99:               .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn99]
                        lea              rsi, [rsp + 800]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 784], rax
                        mov              qword ptr [rsp + 792], rdx
                        cmp              eax, 99
                                                                                        je    n30_lit_string_α
                                                                                        jmp   n26_lit_integer_α
n25_call_β:
                                                                                        jmp   n30_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n26_lit_integer_α:
                        mov              qword ptr [rsp + 848], 6
                        mov              rax, qword ptr [rip + .Lx100_0]
                        mov              qword ptr [rsp + 856], rax
                                                                                        jmp   n27_call_α
.Lx100_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n27_call_α:
                        mov              rax, qword ptr [rsp + 784]
                        mov              qword ptr [rsp + 736], rax
                        mov              rax, qword ptr [rsp + 792]
                        mov              qword ptr [rsp + 744], rax
                        mov              rax, qword ptr [rsp + 848]
                        mov              qword ptr [rsp + 752], rax
                        mov              rax, qword ptr [rsp + 856]
                        mov              qword ptr [rsp + 760], rax
                        .section         .rodata
.Lbynamefn28:           .string          "arg"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn28]
                        lea              rsi, [rsp + 736]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 720], rax
                        mov              qword ptr [rsp + 728], rdx
                        cmp              eax, 99
                                                                                        je    n30_lit_string_α
                                                                                        jmp   n28_lit_string_α
n27_call_β:
                                                                                        jmp   n30_lit_string_α
#=======================================================================================================================
#         output = 'FAIL 1017/004: arg(.jlab,4) OOB should fail' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n28_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 43
                        mov              rax, qword ptr [rip + .Lx102_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n29_assign_α
.Lx102_0:
                        .quad            .Lx102_0_s
.Lx102_0_s:
                        .string          "FAIL 1017/004: arg(.jlab,4) OOB should fail"
#-----------------------------------------------------------------------------------------------------------------------
n29_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         differ(local(.jlab, 1), 'D')                   :f(e005)
#-----------------------------------------------------------------------------------------------------------------------
n30_lit_string_α:
                        mov              qword ptr [rsp + 1056], 1
                        mov              dword ptr [rsp + 1060], 4
                        mov              rax, qword ptr [rip + .Lx104_0]
                        mov              qword ptr [rsp + 1064], rax
                                                                                        jmp   n31_call_α
.Lx104_0:
                        .quad            .Lx104_0_s
.Lx104_0_s:
                        .string          "jlab"
#-----------------------------------------------------------------------------------------------------------------------
n31_call_α:
                        mov              rax, qword ptr [rsp + 1056]
                        mov              qword ptr [rsp + 1024], rax
                        mov              rax, qword ptr [rsp + 1064]
                        mov              qword ptr [rsp + 1032], rax
                        .section         .rodata
.Lrkfn106:              .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn106]
                        lea              rsi, [rsp + 1024]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1008], rax
                        mov              qword ptr [rsp + 1016], rdx
                        cmp              eax, 99
                                                                                        je    n38_lit_string_α
                                                                                        jmp   n32_lit_integer_α
n31_call_β:
                                                                                        jmp   n38_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n32_lit_integer_α:
                        mov              qword ptr [rsp + 1072], 6
                        mov              rax, qword ptr [rip + .Lx107_0]
                        mov              qword ptr [rsp + 1080], rax
                                                                                        jmp   n33_call_α
.Lx107_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n33_call_α:
                        mov              rax, qword ptr [rsp + 1008]
                        mov              qword ptr [rsp + 960], rax
                        mov              rax, qword ptr [rsp + 1016]
                        mov              qword ptr [rsp + 968], rax
                        mov              rax, qword ptr [rsp + 1072]
                        mov              qword ptr [rsp + 976], rax
                        mov              rax, qword ptr [rsp + 1080]
                        mov              qword ptr [rsp + 984], rax
                        .section         .rodata
.Lbynamefn34:           .string          "local"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn34]
                        lea              rsi, [rsp + 960]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 944], rax
                        mov              qword ptr [rsp + 952], rdx
                        cmp              eax, 99
                                                                                        je    n38_lit_string_α
                                                                                        jmp   n34_lit_string_α
n33_call_β:
                                                                                        jmp   n38_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n34_lit_string_α:
                        mov              qword ptr [rsp + 1088], 1
                        mov              dword ptr [rsp + 1092], 1
                        mov              rax, qword ptr [rip + .Lx109_0]
                        mov              qword ptr [rsp + 1096], rax
                                                                                        jmp   n35_call_α
.Lx109_0:
                        .quad            .Lx109_0_s
.Lx109_0_s:
                        .string          "D"
#-----------------------------------------------------------------------------------------------------------------------
n35_call_α:
                        mov              rax, qword ptr [rsp + 944]
                        mov              qword ptr [rsp + 896], rax
                        mov              rax, qword ptr [rsp + 952]
                        mov              qword ptr [rsp + 904], rax
                        mov              rax, qword ptr [rsp + 1088]
                        mov              qword ptr [rsp + 912], rax
                        mov              rax, qword ptr [rsp + 1096]
                        mov              qword ptr [rsp + 920], rax
                        .section         .rodata
.Lbynamefn36:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn36]
                        lea              rsi, [rsp + 896]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 880], rax
                        mov              qword ptr [rsp + 888], rdx
                        cmp              eax, 99
                                                                                        je    n38_lit_string_α
                                                                                        jmp   n36_lit_string_α
n35_call_β:
                                                                                        jmp   n38_lit_string_α
#=======================================================================================================================
#         output = 'FAIL 1017/005: local(.jlab,1) = D'   :(end)
#-----------------------------------------------------------------------------------------------------------------------
n36_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx111_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n37_assign_α
.Lx111_0:
                        .quad            .Lx111_0_s
.Lx111_0_s:
                        .string          "FAIL 1017/005: local(.jlab,1) = D"
#-----------------------------------------------------------------------------------------------------------------------
n37_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         differ(local(.jlab, 3), 'F')                   :f(e006)
#-----------------------------------------------------------------------------------------------------------------------
n38_lit_string_α:
                        mov              qword ptr [rsp + 1296], 1
                        mov              dword ptr [rsp + 1300], 4
                        mov              rax, qword ptr [rip + .Lx113_0]
                        mov              qword ptr [rsp + 1304], rax
                                                                                        jmp   n39_call_α
.Lx113_0:
                        .quad            .Lx113_0_s
.Lx113_0_s:
                        .string          "jlab"
#-----------------------------------------------------------------------------------------------------------------------
n39_call_α:
                        mov              rax, qword ptr [rsp + 1296]
                        mov              qword ptr [rsp + 1264], rax
                        mov              rax, qword ptr [rsp + 1304]
                        mov              qword ptr [rsp + 1272], rax
                        .section         .rodata
.Lrkfn115:              .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn115]
                        lea              rsi, [rsp + 1264]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1248], rax
                        mov              qword ptr [rsp + 1256], rdx
                        cmp              eax, 99
                                                                                        je    n46_lit_string_α
                                                                                        jmp   n40_lit_integer_α
n39_call_β:
                                                                                        jmp   n46_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n40_lit_integer_α:
                        mov              qword ptr [rsp + 1312], 6
                        mov              rax, qword ptr [rip + .Lx116_0]
                        mov              qword ptr [rsp + 1320], rax
                                                                                        jmp   n41_call_α
.Lx116_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n41_call_α:
                        mov              rax, qword ptr [rsp + 1248]
                        mov              qword ptr [rsp + 1200], rax
                        mov              rax, qword ptr [rsp + 1256]
                        mov              qword ptr [rsp + 1208], rax
                        mov              rax, qword ptr [rsp + 1312]
                        mov              qword ptr [rsp + 1216], rax
                        mov              rax, qword ptr [rsp + 1320]
                        mov              qword ptr [rsp + 1224], rax
                        .section         .rodata
.Lbynamefn42:           .string          "local"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn42]
                        lea              rsi, [rsp + 1200]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1184], rax
                        mov              qword ptr [rsp + 1192], rdx
                        cmp              eax, 99
                                                                                        je    n46_lit_string_α
                                                                                        jmp   n42_lit_string_α
n41_call_β:
                                                                                        jmp   n46_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n42_lit_string_α:
                        mov              qword ptr [rsp + 1328], 1
                        mov              dword ptr [rsp + 1332], 1
                        mov              rax, qword ptr [rip + .Lx118_0]
                        mov              qword ptr [rsp + 1336], rax
                                                                                        jmp   n43_call_α
.Lx118_0:
                        .quad            .Lx118_0_s
.Lx118_0_s:
                        .string          "F"
#-----------------------------------------------------------------------------------------------------------------------
n43_call_α:
                        mov              rax, qword ptr [rsp + 1184]
                        mov              qword ptr [rsp + 1136], rax
                        mov              rax, qword ptr [rsp + 1192]
                        mov              qword ptr [rsp + 1144], rax
                        mov              rax, qword ptr [rsp + 1328]
                        mov              qword ptr [rsp + 1152], rax
                        mov              rax, qword ptr [rsp + 1336]
                        mov              qword ptr [rsp + 1160], rax
                        .section         .rodata
.Lbynamefn44:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn44]
                        lea              rsi, [rsp + 1136]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1120], rax
                        mov              qword ptr [rsp + 1128], rdx
                        cmp              eax, 99
                                                                                        je    n46_lit_string_α
                                                                                        jmp   n44_lit_string_α
n43_call_β:
                                                                                        jmp   n46_lit_string_α
#=======================================================================================================================
#         output = 'FAIL 1017/006: local(.jlab,3) = F'   :(end)
#-----------------------------------------------------------------------------------------------------------------------
n44_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx120_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n45_assign_α
.Lx120_0:
                        .quad            .Lx120_0_s
.Lx120_0_s:
                        .string          "FAIL 1017/006: local(.jlab,3) = F"
#-----------------------------------------------------------------------------------------------------------------------
n45_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         local(.jlab, 0)            :f(e007)
#-----------------------------------------------------------------------------------------------------------------------
n46_lit_string_α:
                        mov              qword ptr [rsp + 1472], 1
                        mov              dword ptr [rsp + 1476], 4
                        mov              rax, qword ptr [rip + .Lx122_0]
                        mov              qword ptr [rsp + 1480], rax
                                                                                        jmp   n47_call_α
.Lx122_0:
                        .quad            .Lx122_0_s
.Lx122_0_s:
                        .string          "jlab"
#-----------------------------------------------------------------------------------------------------------------------
n47_call_α:
                        mov              rax, qword ptr [rsp + 1472]
                        mov              qword ptr [rsp + 1440], rax
                        mov              rax, qword ptr [rsp + 1480]
                        mov              qword ptr [rsp + 1448], rax
                        .section         .rodata
.Lrkfn124:              .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn124]
                        lea              rsi, [rsp + 1440]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1424], rax
                        mov              qword ptr [rsp + 1432], rdx
                        cmp              eax, 99
                                                                                        je    n52_lit_string_α
                                                                                        jmp   n48_lit_integer_α
n47_call_β:
                                                                                        jmp   n52_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n48_lit_integer_α:
                        mov              qword ptr [rsp + 1488], 6
                        mov              rax, qword ptr [rip + .Lx125_0]
                        mov              qword ptr [rsp + 1496], rax
                                                                                        jmp   n49_call_α
.Lx125_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n49_call_α:
                        mov              rax, qword ptr [rsp + 1424]
                        mov              qword ptr [rsp + 1376], rax
                        mov              rax, qword ptr [rsp + 1432]
                        mov              qword ptr [rsp + 1384], rax
                        mov              rax, qword ptr [rsp + 1488]
                        mov              qword ptr [rsp + 1392], rax
                        mov              rax, qword ptr [rsp + 1496]
                        mov              qword ptr [rsp + 1400], rax
                        .section         .rodata
.Lbynamefn50:           .string          "local"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn50]
                        lea              rsi, [rsp + 1376]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1360], rax
                        mov              qword ptr [rsp + 1368], rdx
                        cmp              eax, 99
                                                                                        je    n52_lit_string_α
                                                                                        jmp   n50_lit_string_α
n49_call_β:
                                                                                        jmp   n52_lit_string_α
#=======================================================================================================================
#         output = 'FAIL 1017/007: local(.jlab,0) OOB should fail' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n50_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 45
                        mov              rax, qword ptr [rip + .Lx127_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n51_assign_α
.Lx127_0:
                        .quad            .Lx127_0_s
.Lx127_0_s:
                        .string          "FAIL 1017/007: local(.jlab,0) OOB should fail"
#-----------------------------------------------------------------------------------------------------------------------
n51_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         local(.jlab, 4)            :f(e008)
#-----------------------------------------------------------------------------------------------------------------------
n52_lit_string_α:
                        mov              qword ptr [rsp + 1632], 1
                        mov              dword ptr [rsp + 1636], 4
                        mov              rax, qword ptr [rip + .Lx129_0]
                        mov              qword ptr [rsp + 1640], rax
                                                                                        jmp   n53_call_α
.Lx129_0:
                        .quad            .Lx129_0_s
.Lx129_0_s:
                        .string          "jlab"
#-----------------------------------------------------------------------------------------------------------------------
n53_call_α:
                        mov              rax, qword ptr [rsp + 1632]
                        mov              qword ptr [rsp + 1600], rax
                        mov              rax, qword ptr [rsp + 1640]
                        mov              qword ptr [rsp + 1608], rax
                        .section         .rodata
.Lrkfn131:              .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn131]
                        lea              rsi, [rsp + 1600]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1584], rax
                        mov              qword ptr [rsp + 1592], rdx
                        cmp              eax, 99
                                                                                        je    n58_lit_string_α
                                                                                        jmp   n54_lit_integer_α
n53_call_β:
                                                                                        jmp   n58_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n54_lit_integer_α:
                        mov              qword ptr [rsp + 1648], 6
                        mov              rax, qword ptr [rip + .Lx132_0]
                        mov              qword ptr [rsp + 1656], rax
                                                                                        jmp   n55_call_α
.Lx132_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n55_call_α:
                        mov              rax, qword ptr [rsp + 1584]
                        mov              qword ptr [rsp + 1536], rax
                        mov              rax, qword ptr [rsp + 1592]
                        mov              qword ptr [rsp + 1544], rax
                        mov              rax, qword ptr [rsp + 1648]
                        mov              qword ptr [rsp + 1552], rax
                        mov              rax, qword ptr [rsp + 1656]
                        mov              qword ptr [rsp + 1560], rax
                        .section         .rodata
.Lbynamefn56:           .string          "local"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn56]
                        lea              rsi, [rsp + 1536]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1520], rax
                        mov              qword ptr [rsp + 1528], rdx
                        cmp              eax, 99
                                                                                        je    n58_lit_string_α
                                                                                        jmp   n56_lit_string_α
n55_call_β:
                                                                                        jmp   n58_lit_string_α
#=======================================================================================================================
#         output = 'FAIL 1017/008: local(.jlab,4) OOB should fail' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n56_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 45
                        mov              rax, qword ptr [rip + .Lx134_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n57_assign_α
.Lx134_0:
                        .quad            .Lx134_0_s
.Lx134_0_s:
                        .string          "FAIL 1017/008: local(.jlab,4) OOB should fail"
#-----------------------------------------------------------------------------------------------------------------------
n57_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         output = 'PASS 1017_arg_local (8/8)'
#-----------------------------------------------------------------------------------------------------------------------
n58_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 25
                        mov              rax, qword ptr [rip + .Lx136_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n59_assign_α
.Lx136_0:
                        .quad            .Lx136_0_s
.Lx136_0_s:
                        .string          "PASS 1017_arg_local (8/8)"
#-----------------------------------------------------------------------------------------------------------------------
n59_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n60_goto_α:
                                                                                        jmp   n0_lit_string_α
n60_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n61_goto_α:
                                                                                        jmp   n10_lit_string_α
n61_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n62_goto_α:
                                                                                        jmp   n18_lit_string_α
n62_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n63_goto_α:
                                                                                        jmp   n24_lit_string_α
n63_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n64_goto_α:
                                                                                        jmp   n30_lit_string_α
n64_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n65_goto_α:
                                                                                        jmp   n38_lit_string_α
n65_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n66_goto_α:
                                                                                        jmp   n46_lit_string_α
n66_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n67_goto_α:
                                                                                        jmp   n52_lit_string_α
n67_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n68_goto_α:
                                                                                        jmp   n58_lit_string_α
n68_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n69_goto_α:
                                                                                        jmp   main_γ
n69_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        add              rsp, 1704
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              eax, 99
                        xor              edx, edx
                        add              rsp, 1704
                        ret
                        .section         .note.GNU-stack,"",@progbits

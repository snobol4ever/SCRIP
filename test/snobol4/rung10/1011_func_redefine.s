                        .intel_syntax    noprefix
                        .text
                        .section         .rodata
.Lgvan0:                .string          "myfunc"
.Lgvan1:                .string          "n"
.Lgvan2:                .string          "output"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .quad            .Lgvan1
                        .quad            .Lgvan2
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        mov              edi, 3
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 3
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
                        sub              rsp, 968
                        mov              rdi, rsp
                        mov              ecx, 968
                        xor              eax, eax
                        rep stosb
main_α_body:
#=======================================================================================================================
#         define('myfunc(n)')                            :(myfunc_end)
#-----------------------------------------------------------------------------------------------------------------------
n0_lit_string_α:
                        mov              qword ptr [rsp + 144], 1
                        mov              dword ptr [rsp + 148], 9
                        mov              rax, qword ptr [rip + .Lx49_0]
                        mov              qword ptr [rsp + 152], rax
                                                                                        jmp   n9_call_α
.Lx49_0:
                        .quad            .Lx49_0_s
.Lx49_0_s:
                        .string          "myfunc(n)"
#-----------------------------------------------------------------------------------------------------------------------
n1_goto_α:
                                                                                        jmp   n10_var_α
n1_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n2_goto_α:
                                                                                        jmp   n11_lit_integer_α
n2_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n3_goto_α:
                                                                                        jmp   n12_lit_string_α
n3_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n4_goto_α:
                                                                                        jmp   n13_lit_integer_α
n4_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n5_goto_α:
                                                                                        jmp   n14_var_α
n5_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n6_goto_α:
                                                                                        jmp   n13_lit_integer_α
n6_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n7_goto_α:
                                                                                        jmp   n15_lit_string_α
n7_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n8_goto_α:
                                                                                        jmp   main_γ
n8_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n9_call_α:
                        mov              rax, qword ptr [rsp + 144]
                        mov              qword ptr [rsp + 112], rax
                        mov              rax, qword ptr [rsp + 152]
                        mov              qword ptr [rsp + 120], rax
                        .section         .rodata
.Lbynamefn10:           .string          "define"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn10]
                        lea              rsi, [rsp + 112]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 96], rax
                        mov              qword ptr [rsp + 104], rdx
                        cmp              eax, 99
                                                                                        je    n11_lit_integer_α
                                                                                        jmp   n11_lit_integer_α
n9_call_β:
                                                                                        jmp   n11_lit_integer_α
#=======================================================================================================================
# myfunc  myfunc = n * 2                                 :(return)
#-----------------------------------------------------------------------------------------------------------------------
n10_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n16_lit_integer_α
#=======================================================================================================================
#         ne(myfunc(3), 6)           :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n11_lit_integer_α:
                        mov              qword ptr [rsp + 336], 6
                        mov              rax, qword ptr [rip + .Lx60_0]
                        mov              qword ptr [rsp + 344], rax
                                                                                        jmp   n18_call_α
.Lx60_0:
                        .quad            3
#=======================================================================================================================
#         differ(define('myfunc(myfunc)', 'myfunc2'))                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n12_lit_string_α:
                        mov              qword ptr [rsp + 496], 1
                        mov              dword ptr [rsp + 500], 14
                        mov              rax, qword ptr [rip + .Lx61_0]
                        mov              qword ptr [rsp + 504], rax
                                                                                        jmp   n19_lit_string_α
.Lx61_0:
                        .quad            .Lx61_0_s
.Lx61_0_s:
                        .string          "myfunc(myfunc)"
#=======================================================================================================================
# e002    :(myfunc2_end)
#         ne(myfunc(4), 24)          :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n13_lit_integer_α:
                        mov              qword ptr [rsp + 896], 6
                        mov              rax, qword ptr [rip + .Lx62_0]
                        mov              qword ptr [rsp + 904], rax
                                                                                        jmp   n20_call_α
.Lx62_0:
                        .quad            4
#=======================================================================================================================
# myfunc2 myfunc = ne(myfunc, 1) myfunc * myfunc(myfunc - 1) :(return)
#-----------------------------------------------------------------------------------------------------------------------
n14_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 624], rax
                        mov              qword ptr [rsp + 632], rdx
                                                                                        jmp   n21_lit_integer_α
#=======================================================================================================================
#         output = 'PASS 1011_func_redefine (3/3)'
#-----------------------------------------------------------------------------------------------------------------------
n15_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 29
                        mov              rax, qword ptr [rip + .Lx64_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n23_assign_α
.Lx64_0:
                        .quad            .Lx64_0_s
.Lx64_0_s:
                        .string          "PASS 1011_func_redefine (3/3)"
#-----------------------------------------------------------------------------------------------------------------------
n16_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx65_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n24_binop_α
.Lx65_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n17_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx67_0]
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx67_1
.Lx67_0:
                        .quad            .Lx67_0_s
.Lx67_0_s:
                        .string          "return"
.Lx67_1:
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n18_call_α:
                        mov              rax, qword ptr [rsp + 336]
                        mov              qword ptr [rsp + 304], rax
                        mov              rax, qword ptr [rsp + 344]
                        mov              qword ptr [rsp + 312], rax
                        .section         .rodata
.Lbynamefn19:           .string          "myfunc"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn19]
                        lea              rsi, [rsp + 304]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 288], rax
                        mov              qword ptr [rsp + 296], rdx
                        cmp              eax, 99
                                                                                        je    n12_lit_string_α
                                                                                        jmp   n25_lit_integer_α
n18_call_β:
                                                                                        jmp   n12_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n19_lit_string_α:
                        mov              qword ptr [rsp + 512], 1
                        mov              dword ptr [rsp + 516], 7
                        mov              rax, qword ptr [rip + .Lx69_0]
                        mov              qword ptr [rsp + 520], rax
                                                                                        jmp   n26_call_α
.Lx69_0:
                        .quad            .Lx69_0_s
.Lx69_0_s:
                        .string          "myfunc2"
#-----------------------------------------------------------------------------------------------------------------------
n20_call_α:
                        mov              rax, qword ptr [rsp + 896]
                        mov              qword ptr [rsp + 864], rax
                        mov              rax, qword ptr [rsp + 904]
                        mov              qword ptr [rsp + 872], rax
                        .section         .rodata
.Lbynamefn21:           .string          "myfunc"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn21]
                        lea              rsi, [rsp + 864]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 848], rax
                        mov              qword ptr [rsp + 856], rdx
                        cmp              eax, 99
                                                                                        je    n15_lit_string_α
                                                                                        jmp   n27_lit_integer_α
n20_call_β:
                                                                                        jmp   n15_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n21_lit_integer_α:
                        mov              qword ptr [rsp + 640], 6
                        mov              rax, qword ptr [rip + .Lx71_0]
                        mov              qword ptr [rsp + 648], rax
                                                                                        jmp   n28_call_α
.Lx71_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n22_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx73_0]
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx73_1
.Lx73_0:
                        .quad            .Lx73_0_s
.Lx73_0_s:
                        .string          "return"
.Lx73_1:
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n23_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n24_binop_α:
                        mov              eax, dword ptr [rsp + 16]
                        cmp              eax, 6
                                                                                        jne   .Lx75_0
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, 6
                                                                                        jne   .Lx75_0
                        mov              rax, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rsp + 8]
                        imul             rax, rcx
                        add              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n29_assign_α
.Lx75_0:
                        mov              rdi, qword ptr [rsp + 16]
                        mov              rsi, qword ptr [rsp + 24]
                        mov              rdx, qword ptr [rsp + 0]
                        mov              rcx, qword ptr [rsp + 8]
                        call             rt_mul@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx75_240
                        add              rsp, 32
                                                                                        jmp   n17_goto_deferred_α
.Lx75_240:
                        add              rsp, 16
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n29_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n25_lit_integer_α:
                        mov              qword ptr [rsp + 352], 6
                        mov              rax, qword ptr [rip + .Lx76_0]
                        mov              qword ptr [rsp + 360], rax
                                                                                        jmp   n30_call_α
.Lx76_0:
                        .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n26_call_α:
                        mov              rax, qword ptr [rsp + 496]
                        mov              qword ptr [rsp + 448], rax
                        mov              rax, qword ptr [rsp + 504]
                        mov              qword ptr [rsp + 456], rax
                        mov              rax, qword ptr [rsp + 512]
                        mov              qword ptr [rsp + 464], rax
                        mov              rax, qword ptr [rsp + 520]
                        mov              qword ptr [rsp + 472], rax
                        .section         .rodata
.Lbynamefn27:           .string          "define"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn27]
                        lea              rsi, [rsp + 448]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 432], rax
                        mov              qword ptr [rsp + 440], rdx
                        cmp              eax, 99
                                                                                        je    n13_lit_integer_α
                                                                                        jmp   n31_call_α
n26_call_β:
                                                                                        jmp   n13_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n27_lit_integer_α:
                        mov              qword ptr [rsp + 912], 6
                        mov              rax, qword ptr [rip + .Lx78_0]
                        mov              qword ptr [rsp + 920], rax
                                                                                        jmp   n32_call_α
.Lx78_0:
                        .quad            24
#-----------------------------------------------------------------------------------------------------------------------
n28_call_α:
                        mov              rax, qword ptr [rsp + 624]
                        mov              qword ptr [rsp + 576], rax
                        mov              rax, qword ptr [rsp + 632]
                        mov              qword ptr [rsp + 584], rax
                        mov              rax, qword ptr [rsp + 640]
                        mov              qword ptr [rsp + 592], rax
                        mov              rax, qword ptr [rsp + 648]
                        mov              qword ptr [rsp + 600], rax
                        .section         .rodata
.Lbynamefn29:           .string          "ne"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn29]
                        lea              rsi, [rsp + 576]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 560], rax
                        mov              qword ptr [rsp + 568], rdx
                        cmp              eax, 99
                                                                                        je    n22_goto_deferred_α
                                                                                        jmp   n33_var_α
n28_call_β:
                                                                                        jmp   n22_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n29_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n34_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n30_call_α:
                        mov              rax, qword ptr [rsp + 288]
                        mov              qword ptr [rsp + 240], rax
                        mov              rax, qword ptr [rsp + 296]
                        mov              qword ptr [rsp + 248], rax
                        mov              rax, qword ptr [rsp + 352]
                        mov              qword ptr [rsp + 256], rax
                        mov              rax, qword ptr [rsp + 360]
                        mov              qword ptr [rsp + 264], rax
                        .section         .rodata
.Lbynamefn31:           .string          "ne"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn31]
                        lea              rsi, [rsp + 240]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 224], rax
                        mov              qword ptr [rsp + 232], rdx
                        cmp              eax, 99
                                                                                        je    n12_lit_string_α
                                                                                        jmp   n35_lit_string_α
n30_call_β:
                                                                                        jmp   n12_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n31_call_α:
                        mov              rax, qword ptr [rsp + 432]
                        mov              qword ptr [rsp + 400], rax
                        mov              rax, qword ptr [rsp + 440]
                        mov              qword ptr [rsp + 408], rax
                        .section         .rodata
.Lbynamefn32:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn32]
                        lea              rsi, [rsp + 400]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 384], rax
                        mov              qword ptr [rsp + 392], rdx
                        cmp              eax, 99
                                                                                        je    n13_lit_integer_α
                                                                                        jmp   n36_lit_string_α
n31_call_β:
                                                                                        jmp   n13_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n32_call_α:
                        mov              rax, qword ptr [rsp + 848]
                        mov              qword ptr [rsp + 800], rax
                        mov              rax, qword ptr [rsp + 856]
                        mov              qword ptr [rsp + 808], rax
                        mov              rax, qword ptr [rsp + 912]
                        mov              qword ptr [rsp + 816], rax
                        mov              rax, qword ptr [rsp + 920]
                        mov              qword ptr [rsp + 824], rax
                        .section         .rodata
.Lbynamefn33:           .string          "ne"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn33]
                        lea              rsi, [rsp + 800]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 784], rax
                        mov              qword ptr [rsp + 792], rdx
                        cmp              eax, 99
                                                                                        je    n15_lit_string_α
                                                                                        jmp   n37_lit_string_α
n32_call_β:
                                                                                        jmp   n15_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n33_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 672], rax
                        mov              qword ptr [rsp + 680], rdx
                                                                                        jmp   n38_var_α
#-----------------------------------------------------------------------------------------------------------------------
n34_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx86_0]
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx86_1
.Lx86_0:
                        .quad            .Lx86_0_s
.Lx86_0_s:
                        .string          "return"
.Lx86_1:
                                                                                        jmp   main_γ
#=======================================================================================================================
#         output = 'FAIL 1011/001: first definition myfunc(3)=6' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n35_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 43
                        mov              rax, qword ptr [rip + .Lx87_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n39_assign_α
.Lx87_0:
                        .quad            .Lx87_0_s
.Lx87_0_s:
                        .string          "FAIL 1011/001: first definition myfunc(3)=6"
#=======================================================================================================================
#         output = 'FAIL 1011/002: define returns function name' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n36_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 43
                        mov              rax, qword ptr [rip + .Lx88_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n40_assign_α
.Lx88_0:
                        .quad            .Lx88_0_s
.Lx88_0_s:
                        .string          "FAIL 1011/002: define returns function name"
#=======================================================================================================================
#         output = 'FAIL 1011/003: redefined myfunc(4)=24'   :(end)
#-----------------------------------------------------------------------------------------------------------------------
n37_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 37
                        mov              rax, qword ptr [rip + .Lx89_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n41_assign_α
.Lx89_0:
                        .quad            .Lx89_0_s
.Lx89_0_s:
                        .string          "FAIL 1011/003: redefined myfunc(4)=24"
#-----------------------------------------------------------------------------------------------------------------------
n38_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 752], rax
                        mov              qword ptr [rsp + 760], rdx
                                                                                        jmp   n42_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n39_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n40_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n41_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n42_lit_integer_α:
                        mov              qword ptr [rsp + 768], 6
                        mov              rax, qword ptr [rip + .Lx94_0]
                        mov              qword ptr [rsp + 776], rax
                                                                                        jmp   n43_binop_α
.Lx94_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n43_binop_α:
                        mov              eax, dword ptr [rsp + 752]
                        cmp              eax, 6
                                                                                        jne   .Lx95_0
                        mov              rax, qword ptr [rsp + 760]
                        mov              rcx, 1
                        sub              rax, rcx
                        mov              qword ptr [rsp + 736], 6
                        mov              qword ptr [rsp + 744], rax
                                                                                        jmp   n44_call_α
.Lx95_0:
                        mov              rdi, qword ptr [rsp + 752]
                        mov              rsi, qword ptr [rsp + 760]
                        mov              rdx, qword ptr [rsp + 768]
                        mov              rcx, qword ptr [rsp + 776]
                        call             rt_sub@PLT
                        cmp              eax, 99
                                                                                        je    n22_goto_deferred_α
                        mov              qword ptr [rsp + 736], rax
                        mov              qword ptr [rsp + 744], rdx
                                                                                        jmp   n44_call_α
#-----------------------------------------------------------------------------------------------------------------------
n44_call_α:
                        mov              rax, qword ptr [rsp + 736]
                        mov              qword ptr [rsp + 704], rax
                        mov              rax, qword ptr [rsp + 744]
                        mov              qword ptr [rsp + 712], rax
                        .section         .rodata
.Lbynamefn45:           .string          "myfunc"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn45]
                        lea              rsi, [rsp + 704]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 688], rax
                        mov              qword ptr [rsp + 696], rdx
                        cmp              eax, 99
                                                                                        je    n22_goto_deferred_α
                                                                                        jmp   n45_binop_α
n44_call_β:
                                                                                        jmp   n22_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n45_binop_α:
                        mov              eax, dword ptr [rsp + 672]
                        cmp              eax, 6
                                                                                        jne   .Lx97_0
                        mov              eax, dword ptr [rsp + 688]
                        cmp              eax, 6
                                                                                        jne   .Lx97_0
                        mov              rax, qword ptr [rsp + 680]
                        mov              rcx, qword ptr [rsp + 696]
                        imul             rax, rcx
                        mov              qword ptr [rsp + 656], 6
                        mov              qword ptr [rsp + 664], rax
                                                                                        jmp   n46_binop_α
.Lx97_0:
                        mov              rdi, qword ptr [rsp + 672]
                        mov              rsi, qword ptr [rsp + 680]
                        mov              rdx, qword ptr [rsp + 688]
                        mov              rcx, qword ptr [rsp + 696]
                        call             rt_mul@PLT
                        cmp              eax, 99
                                                                                        je    n22_goto_deferred_α
                        mov              qword ptr [rsp + 656], rax
                        mov              qword ptr [rsp + 664], rdx
                                                                                        jmp   n46_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n46_binop_α:
                        mov              rdi, qword ptr [rsp + 560]
                        mov              rsi, qword ptr [rsp + 568]
                        mov              rdx, qword ptr [rsp + 656]
                        mov              rcx, qword ptr [rsp + 664]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 544], rax
                        mov              qword ptr [rsp + 552], rdx
                                                                                        jmp   n47_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n47_assign_α:
                        mov              rax, qword ptr [rsp + 544]
                        mov              rdx, qword ptr [rsp + 552]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n48_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n48_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx101_0]
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx101_1
.Lx101_0:
                        .quad            .Lx101_0_s
.Lx101_0_s:
                        .string          "return"
.Lx101_1:
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        add              rsp, 968
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              eax, 99
                        xor              edx, edx
                        add              rsp, 968
                        ret
                        .section         .note.GNU-stack,"",@progbits

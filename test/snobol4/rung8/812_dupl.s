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
                        sub              rsp, 808
                        mov              rdi, rsp
                        mov              ecx, 808
                        xor              eax, eax
                        rep stosb
main_α_body:
#=======================================================================================================================
#         differ(dupl('abc', 2), 'abcabc')                   :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n0_lit_string_α:
                        mov              qword ptr [rsp + 128], 1
                        mov              dword ptr [rsp + 132], 3
                        mov              rax, qword ptr [rip + .Lx35_0]
                        mov              qword ptr [rsp + 136], rax
                                                                                        jmp   n6_lit_integer_α
.Lx35_0:
                        .quad            .Lx35_0_s
.Lx35_0_s:
                        .string          "abc"
#-----------------------------------------------------------------------------------------------------------------------
n1_goto_α:
                                                                                        jmp   n7_lit_string_α
n1_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n2_goto_α:
                                                                                        jmp   n8_lit_string_α
n2_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n3_goto_α:
                                                                                        jmp   n9_lit_integer_α
n3_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n4_goto_α:
                                                                                        jmp   n10_lit_string_α
n4_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n5_goto_α:
                                                                                        jmp   main_γ
n5_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n6_lit_integer_α:
                        mov              qword ptr [rsp + 144], 6
                        mov              rax, qword ptr [rip + .Lx41_0]
                        mov              qword ptr [rsp + 152], rax
                                                                                        jmp   n11_call_α
.Lx41_0:
                        .quad            2
#=======================================================================================================================
#         differ(dupl('', 10), '')                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n7_lit_string_α:
                        mov              qword ptr [rsp + 336], 1
                        mov              dword ptr [rsp + 340], 0
                        mov              rax, qword ptr [rip + .Lx42_0]
                        mov              qword ptr [rsp + 344], rax
                                                                                        jmp   n12_lit_integer_α
.Lx42_0:
                        .quad            .Lx42_0_s
.Lx42_0_s:
                        .string          ""
#=======================================================================================================================
#         differ(dupl('abcdefg', 0), '')                   :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n8_lit_string_α:
                        mov              qword ptr [rsp + 528], 1
                        mov              dword ptr [rsp + 532], 7
                        mov              rax, qword ptr [rip + .Lx43_0]
                        mov              qword ptr [rsp + 536], rax
                                                                                        jmp   n13_lit_integer_α
.Lx43_0:
                        .quad            .Lx43_0_s
.Lx43_0_s:
                        .string          "abcdefg"
#=======================================================================================================================
#         differ(dupl(1, 10), '1111111111')                   :f(e004)
#-----------------------------------------------------------------------------------------------------------------------
n9_lit_integer_α:
                        mov              qword ptr [rsp + 720], 6
                        mov              rax, qword ptr [rip + .Lx44_0]
                        mov              qword ptr [rsp + 728], rax
                                                                                        jmp   n14_lit_integer_α
.Lx44_0:
                        .quad            1
#=======================================================================================================================
#         output = 'PASS 812_dupl (4/4)'
#-----------------------------------------------------------------------------------------------------------------------
n10_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 19
                        mov              rax, qword ptr [rip + .Lx45_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n15_assign_α
.Lx45_0:
                        .quad            .Lx45_0_s
.Lx45_0_s:
                        .string          "PASS 812_dupl (4/4)"
#-----------------------------------------------------------------------------------------------------------------------
n11_call_α:
                        mov              rax, qword ptr [rsp + 128]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [rsp + 136]
                        mov              qword ptr [rsp + 88], rax
                        mov              rax, qword ptr [rsp + 144]
                        mov              qword ptr [rsp + 96], rax
                        mov              rax, qword ptr [rsp + 152]
                        mov              qword ptr [rsp + 104], rax
                        .section         .rodata
.Lbynamefn12:           .string          "dupl"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn12]
                        lea              rsi, [rsp + 80]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 64], rax
                        mov              qword ptr [rsp + 72], rdx
                        cmp              eax, 99
                                                                                        je    n7_lit_string_α
                                                                                        jmp   n16_lit_string_α
n11_call_β:
                                                                                        jmp   n7_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n12_lit_integer_α:
                        mov              qword ptr [rsp + 352], 6
                        mov              rax, qword ptr [rip + .Lx47_0]
                        mov              qword ptr [rsp + 360], rax
                                                                                        jmp   n17_call_α
.Lx47_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n13_lit_integer_α:
                        mov              qword ptr [rsp + 544], 6
                        mov              rax, qword ptr [rip + .Lx48_0]
                        mov              qword ptr [rsp + 552], rax
                                                                                        jmp   n18_call_α
.Lx48_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n14_lit_integer_α:
                        mov              qword ptr [rsp + 736], 6
                        mov              rax, qword ptr [rip + .Lx49_0]
                        mov              qword ptr [rsp + 744], rax
                                                                                        jmp   n19_call_α
.Lx49_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n15_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n16_lit_string_α:
                        mov              qword ptr [rsp + 160], 1
                        mov              dword ptr [rsp + 164], 6
                        mov              rax, qword ptr [rip + .Lx51_0]
                        mov              qword ptr [rsp + 168], rax
                                                                                        jmp   n20_call_α
.Lx51_0:
                        .quad            .Lx51_0_s
.Lx51_0_s:
                        .string          "abcabc"
#-----------------------------------------------------------------------------------------------------------------------
n17_call_α:
                        mov              rax, qword ptr [rsp + 336]
                        mov              qword ptr [rsp + 288], rax
                        mov              rax, qword ptr [rsp + 344]
                        mov              qword ptr [rsp + 296], rax
                        mov              rax, qword ptr [rsp + 352]
                        mov              qword ptr [rsp + 304], rax
                        mov              rax, qword ptr [rsp + 360]
                        mov              qword ptr [rsp + 312], rax
                        .section         .rodata
.Lbynamefn18:           .string          "dupl"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn18]
                        lea              rsi, [rsp + 288]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 272], rax
                        mov              qword ptr [rsp + 280], rdx
                        cmp              eax, 99
                                                                                        je    n8_lit_string_α
                                                                                        jmp   n21_lit_string_α
n17_call_β:
                                                                                        jmp   n8_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n18_call_α:
                        mov              rax, qword ptr [rsp + 528]
                        mov              qword ptr [rsp + 480], rax
                        mov              rax, qword ptr [rsp + 536]
                        mov              qword ptr [rsp + 488], rax
                        mov              rax, qword ptr [rsp + 544]
                        mov              qword ptr [rsp + 496], rax
                        mov              rax, qword ptr [rsp + 552]
                        mov              qword ptr [rsp + 504], rax
                        .section         .rodata
.Lbynamefn19:           .string          "dupl"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn19]
                        lea              rsi, [rsp + 480]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 464], rax
                        mov              qword ptr [rsp + 472], rdx
                        cmp              eax, 99
                                                                                        je    n9_lit_integer_α
                                                                                        jmp   n22_lit_string_α
n18_call_β:
                                                                                        jmp   n9_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n19_call_α:
                        mov              rax, qword ptr [rsp + 720]
                        mov              qword ptr [rsp + 672], rax
                        mov              rax, qword ptr [rsp + 728]
                        mov              qword ptr [rsp + 680], rax
                        mov              rax, qword ptr [rsp + 736]
                        mov              qword ptr [rsp + 688], rax
                        mov              rax, qword ptr [rsp + 744]
                        mov              qword ptr [rsp + 696], rax
                        .section         .rodata
.Lbynamefn20:           .string          "dupl"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn20]
                        lea              rsi, [rsp + 672]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 656], rax
                        mov              qword ptr [rsp + 664], rdx
                        cmp              eax, 99
                                                                                        je    n10_lit_string_α
                                                                                        jmp   n23_lit_string_α
n19_call_β:
                                                                                        jmp   n10_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n20_call_α:
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [rsp + 160]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 168]
                        mov              qword ptr [rsp + 40], rax
                        .section         .rodata
.Lbynamefn21:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn21]
                        lea              rsi, [rsp + 16]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 99
                                                                                        je    n7_lit_string_α
                                                                                        jmp   n24_lit_string_α
n20_call_β:
                                                                                        jmp   n7_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n21_lit_string_α:
                        mov              qword ptr [rsp + 368], 1
                        mov              dword ptr [rsp + 372], 0
                        mov              rax, qword ptr [rip + .Lx56_0]
                        mov              qword ptr [rsp + 376], rax
                                                                                        jmp   n25_call_α
.Lx56_0:
                        .quad            .Lx56_0_s
.Lx56_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n22_lit_string_α:
                        mov              qword ptr [rsp + 560], 1
                        mov              dword ptr [rsp + 564], 0
                        mov              rax, qword ptr [rip + .Lx57_0]
                        mov              qword ptr [rsp + 568], rax
                                                                                        jmp   n26_call_α
.Lx57_0:
                        .quad            .Lx57_0_s
.Lx57_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n23_lit_string_α:
                        mov              qword ptr [rsp + 752], 1
                        mov              dword ptr [rsp + 756], 10
                        mov              rax, qword ptr [rip + .Lx58_0]
                        mov              qword ptr [rsp + 760], rax
                                                                                        jmp   n27_call_α
.Lx58_0:
                        .quad            .Lx58_0_s
.Lx58_0_s:
                        .string          "1111111111"
#=======================================================================================================================
#         output = 'FAIL 812/001: dupl string x2'         :(end)
#-----------------------------------------------------------------------------------------------------------------------
n24_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 28
                        mov              rax, qword ptr [rip + .Lx59_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n28_assign_α
.Lx59_0:
                        .quad            .Lx59_0_s
.Lx59_0_s:
                        .string          "FAIL 812/001: dupl string x2"
#-----------------------------------------------------------------------------------------------------------------------
n25_call_α:
                        mov              rax, qword ptr [rsp + 272]
                        mov              qword ptr [rsp + 224], rax
                        mov              rax, qword ptr [rsp + 280]
                        mov              qword ptr [rsp + 232], rax
                        mov              rax, qword ptr [rsp + 368]
                        mov              qword ptr [rsp + 240], rax
                        mov              rax, qword ptr [rsp + 376]
                        mov              qword ptr [rsp + 248], rax
                        .section         .rodata
.Lbynamefn26:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn26]
                        lea              rsi, [rsp + 224]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 208], rax
                        mov              qword ptr [rsp + 216], rdx
                        cmp              eax, 99
                                                                                        je    n8_lit_string_α
                                                                                        jmp   n29_lit_string_α
n25_call_β:
                                                                                        jmp   n8_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n26_call_α:
                        mov              rax, qword ptr [rsp + 464]
                        mov              qword ptr [rsp + 416], rax
                        mov              rax, qword ptr [rsp + 472]
                        mov              qword ptr [rsp + 424], rax
                        mov              rax, qword ptr [rsp + 560]
                        mov              qword ptr [rsp + 432], rax
                        mov              rax, qword ptr [rsp + 568]
                        mov              qword ptr [rsp + 440], rax
                        .section         .rodata
.Lbynamefn27:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn27]
                        lea              rsi, [rsp + 416]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 400], rax
                        mov              qword ptr [rsp + 408], rdx
                        cmp              eax, 99
                                                                                        je    n9_lit_integer_α
                                                                                        jmp   n30_lit_string_α
n26_call_β:
                                                                                        jmp   n9_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n27_call_α:
                        mov              rax, qword ptr [rsp + 656]
                        mov              qword ptr [rsp + 608], rax
                        mov              rax, qword ptr [rsp + 664]
                        mov              qword ptr [rsp + 616], rax
                        mov              rax, qword ptr [rsp + 752]
                        mov              qword ptr [rsp + 624], rax
                        mov              rax, qword ptr [rsp + 760]
                        mov              qword ptr [rsp + 632], rax
                        .section         .rodata
.Lbynamefn28:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn28]
                        lea              rsi, [rsp + 608]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 592], rax
                        mov              qword ptr [rsp + 600], rdx
                        cmp              eax, 99
                                                                                        je    n10_lit_string_α
                                                                                        jmp   n31_lit_string_α
n27_call_β:
                                                                                        jmp   n10_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n28_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         output = 'FAIL 812/002: dupl null is null'      :(end)
#-----------------------------------------------------------------------------------------------------------------------
n29_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 31
                        mov              rax, qword ptr [rip + .Lx64_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n32_assign_α
.Lx64_0:
                        .quad            .Lx64_0_s
.Lx64_0_s:
                        .string          "FAIL 812/002: dupl null is null"
#=======================================================================================================================
#         output = 'FAIL 812/003: dupl x0 is null'        :(end)
#-----------------------------------------------------------------------------------------------------------------------
n30_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 29
                        mov              rax, qword ptr [rip + .Lx65_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n33_assign_α
.Lx65_0:
                        .quad            .Lx65_0_s
.Lx65_0_s:
                        .string          "FAIL 812/003: dupl x0 is null"
#=======================================================================================================================
#         output = 'FAIL 812/004: dupl integer coerce'    :(end)
#-----------------------------------------------------------------------------------------------------------------------
n31_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx66_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n34_assign_α
.Lx66_0:
                        .quad            .Lx66_0_s
.Lx66_0_s:
                        .string          "FAIL 812/004: dupl integer coerce"
#-----------------------------------------------------------------------------------------------------------------------
n32_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n33_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n34_assign_α:
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
                        add              rsp, 808
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              eax, 99
                        xor              edx, edx
                        add              rsp, 808
                        ret
                        .section         .note.GNU-stack,"",@progbits

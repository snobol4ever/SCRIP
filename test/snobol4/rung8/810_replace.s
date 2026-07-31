                        .intel_syntax    noprefix
                        .text
                        .section         .rodata
.Lgvan0:                .string          "output"
.Lgvan1:                .string          "a"
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
                        sub              rsp, 840
                        mov              rdi, rsp
                        mov              ecx, 840
                        xor              eax, eax
                        rep stosb
main_α_body:
#=======================================================================================================================
#         differ(replace('axxbyyy', 'xy', '01'), 'a00b111')                   :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n0_lit_string_α:
                        mov              qword ptr [rsp + 144], 1
                        mov              dword ptr [rsp + 148], 7
                        mov              rax, qword ptr [rip + .Lx35_0]
                        mov              qword ptr [rsp + 152], rax
                                                                                        jmp   n5_lit_string_α
.Lx35_0:
                        .quad            .Lx35_0_s
.Lx35_0_s:
                        .string          "axxbyyy"
#-----------------------------------------------------------------------------------------------------------------------
n1_goto_α:
                                                                                        jmp   n6_keyword_snobol4_α
n1_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n2_goto_α:
                                                                                        jmp   n7_lit_string_α
n2_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n3_goto_α:
                                                                                        jmp   n8_lit_string_α
n3_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n4_goto_α:
                                                                                        jmp   main_γ
n4_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n5_lit_string_α:
                        mov              qword ptr [rsp + 160], 1
                        mov              dword ptr [rsp + 164], 2
                        mov              rax, qword ptr [rip + .Lx40_0]
                        mov              qword ptr [rsp + 168], rax
                                                                                        jmp   n9_lit_string_α
.Lx40_0:
                        .quad            .Lx40_0_s
.Lx40_0_s:
                        .string          "xy"
#=======================================================================================================================
#         a = replace(&alphabet, 'xy', 'ab')
#-----------------------------------------------------------------------------------------------------------------------
n6_keyword_snobol4_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx41_0]
                        call             rt_keyword_read_snobol4@PLT
                        mov              qword ptr [rsp + 336], rax
                        mov              qword ptr [rsp + 344], rdx
                        add              rsp, 16
                                                                                        jmp   n10_lit_string_α
.Lx41_0:
                        .quad            .Lx41_0_s
.Lx41_0_s:
                        .string          "alphabet"
#=======================================================================================================================
#         differ(replace('hello', 'aeiou', 'aeiou'), 'hello')                   :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n7_lit_string_α:
                        mov              qword ptr [rsp + 736], 1
                        mov              dword ptr [rsp + 740], 5
                        mov              rax, qword ptr [rip + .Lx42_0]
                        mov              qword ptr [rsp + 744], rax
                                                                                        jmp   n11_lit_string_α
.Lx42_0:
                        .quad            .Lx42_0_s
.Lx42_0_s:
                        .string          "hello"
#=======================================================================================================================
#         output = 'PASS 810_replace (3/3)'
#-----------------------------------------------------------------------------------------------------------------------
n8_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 22
                        mov              rax, qword ptr [rip + .Lx43_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n12_assign_α
.Lx43_0:
                        .quad            .Lx43_0_s
.Lx43_0_s:
                        .string          "PASS 810_replace (3/3)"
#-----------------------------------------------------------------------------------------------------------------------
n9_lit_string_α:
                        mov              qword ptr [rsp + 176], 1
                        mov              dword ptr [rsp + 180], 2
                        mov              rax, qword ptr [rip + .Lx44_0]
                        mov              qword ptr [rsp + 184], rax
                                                                                        jmp   n13_call_α
.Lx44_0:
                        .quad            .Lx44_0_s
.Lx44_0_s:
                        .string          "01"
#-----------------------------------------------------------------------------------------------------------------------
n10_lit_string_α:
                        mov              qword ptr [rsp + 336], 1
                        mov              dword ptr [rsp + 340], 2
                        mov              rax, qword ptr [rip + .Lx45_0]
                        mov              qword ptr [rsp + 344], rax
                                                                                        jmp   n14_lit_string_α
.Lx45_0:
                        .quad            .Lx45_0_s
.Lx45_0_s:
                        .string          "xy"
#-----------------------------------------------------------------------------------------------------------------------
n11_lit_string_α:
                        mov              qword ptr [rsp + 752], 1
                        mov              dword ptr [rsp + 756], 5
                        mov              rax, qword ptr [rip + .Lx46_0]
                        mov              qword ptr [rsp + 760], rax
                                                                                        jmp   n15_lit_string_α
.Lx46_0:
                        .quad            .Lx46_0_s
.Lx46_0_s:
                        .string          "aeiou"
#-----------------------------------------------------------------------------------------------------------------------
n12_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n13_call_α:
                        mov              rax, qword ptr [rsp + 144]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [rsp + 152]
                        mov              qword ptr [rsp + 88], rax
                        mov              rax, qword ptr [rsp + 160]
                        mov              qword ptr [rsp + 96], rax
                        mov              rax, qword ptr [rsp + 168]
                        mov              qword ptr [rsp + 104], rax
                        mov              rax, qword ptr [rsp + 176]
                        mov              qword ptr [rsp + 112], rax
                        mov              rax, qword ptr [rsp + 184]
                        mov              qword ptr [rsp + 120], rax
                        .section         .rodata
.Lbynamefn14:           .string          "replace"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn14]
                        lea              rsi, [rsp + 80]
                        mov              edx, 3
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 64], rax
                        mov              qword ptr [rsp + 72], rdx
                        cmp              eax, 99
                                                                                        je    n6_keyword_snobol4_α
                                                                                        jmp   n16_lit_string_α
n13_call_β:
                                                                                        jmp   n6_keyword_snobol4_α
#-----------------------------------------------------------------------------------------------------------------------
n14_lit_string_α:
                        mov              qword ptr [rsp + 352], 1
                        mov              dword ptr [rsp + 356], 2
                        mov              rax, qword ptr [rip + .Lx49_0]
                        mov              qword ptr [rsp + 360], rax
                                                                                        jmp   n17_call_α
.Lx49_0:
                        .quad            .Lx49_0_s
.Lx49_0_s:
                        .string          "ab"
#-----------------------------------------------------------------------------------------------------------------------
n15_lit_string_α:
                        mov              qword ptr [rsp + 768], 1
                        mov              dword ptr [rsp + 772], 5
                        mov              rax, qword ptr [rip + .Lx50_0]
                        mov              qword ptr [rsp + 776], rax
                                                                                        jmp   n18_call_α
.Lx50_0:
                        .quad            .Lx50_0_s
.Lx50_0_s:
                        .string          "aeiou"
#-----------------------------------------------------------------------------------------------------------------------
n16_lit_string_α:
                        mov              qword ptr [rsp + 192], 1
                        mov              dword ptr [rsp + 196], 7
                        mov              rax, qword ptr [rip + .Lx51_0]
                        mov              qword ptr [rsp + 200], rax
                                                                                        jmp   n19_call_α
.Lx51_0:
                        .quad            .Lx51_0_s
.Lx51_0_s:
                        .string          "a00b111"
#-----------------------------------------------------------------------------------------------------------------------
n17_call_α:
                        mov              rax, qword ptr [rsp + 320]
                        mov              qword ptr [rsp + 256], rax
                        mov              rax, qword ptr [rsp + 328]
                        mov              qword ptr [rsp + 264], rax
                        mov              rax, qword ptr [rsp + 336]
                        mov              qword ptr [rsp + 272], rax
                        mov              rax, qword ptr [rsp + 344]
                        mov              qword ptr [rsp + 280], rax
                        mov              rax, qword ptr [rsp + 352]
                        mov              qword ptr [rsp + 288], rax
                        mov              rax, qword ptr [rsp + 360]
                        mov              qword ptr [rsp + 296], rax
                        .section         .rodata
.Lbynamefn18:           .string          "replace"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn18]
                        lea              rsi, [rsp + 256]
                        mov              edx, 3
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 240], rax
                        mov              qword ptr [rsp + 248], rdx
                        cmp              eax, 99
                                                                                        je    n21_lit_string_α
                                                                                        jmp   n20_assign_α
n17_call_β:
                                                                                        jmp   n21_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n18_call_α:
                        mov              rax, qword ptr [rsp + 736]
                        mov              qword ptr [rsp + 672], rax
                        mov              rax, qword ptr [rsp + 744]
                        mov              qword ptr [rsp + 680], rax
                        mov              rax, qword ptr [rsp + 752]
                        mov              qword ptr [rsp + 688], rax
                        mov              rax, qword ptr [rsp + 760]
                        mov              qword ptr [rsp + 696], rax
                        mov              rax, qword ptr [rsp + 768]
                        mov              qword ptr [rsp + 704], rax
                        mov              rax, qword ptr [rsp + 776]
                        mov              qword ptr [rsp + 712], rax
                        .section         .rodata
.Lbynamefn19:           .string          "replace"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn19]
                        lea              rsi, [rsp + 672]
                        mov              edx, 3
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 656], rax
                        mov              qword ptr [rsp + 664], rdx
                        cmp              eax, 99
                                                                                        je    n8_lit_string_α
                                                                                        jmp   n22_lit_string_α
n18_call_β:
                                                                                        jmp   n8_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n19_call_α:
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [rsp + 192]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 200]
                        mov              qword ptr [rsp + 40], rax
                        .section         .rodata
.Lbynamefn20:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn20]
                        lea              rsi, [rsp + 16]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 99
                                                                                        je    n6_keyword_snobol4_α
                                                                                        jmp   n23_lit_string_α
n19_call_β:
                                                                                        jmp   n6_keyword_snobol4_α
#-----------------------------------------------------------------------------------------------------------------------
n20_assign_α:
                        mov              rax, qword ptr [rsp + 240]
                        mov              rdx, qword ptr [rsp + 248]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   n21_lit_string_α
#=======================================================================================================================
#         differ(replace('axy', &alphabet, a), 'aab')                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n21_lit_string_α:
                        mov              qword ptr [rsp + 512], 1
                        mov              dword ptr [rsp + 516], 3
                        mov              rax, qword ptr [rip + .Lx56_0]
                        mov              qword ptr [rsp + 520], rax
                                                                                        jmp   n24_keyword_snobol4_α
.Lx56_0:
                        .quad            .Lx56_0_s
.Lx56_0_s:
                        .string          "axy"
#-----------------------------------------------------------------------------------------------------------------------
n22_lit_string_α:
                        mov              qword ptr [rsp + 784], 1
                        mov              dword ptr [rsp + 788], 5
                        mov              rax, qword ptr [rip + .Lx57_0]
                        mov              qword ptr [rsp + 792], rax
                                                                                        jmp   n25_call_α
.Lx57_0:
                        .quad            .Lx57_0_s
.Lx57_0_s:
                        .string          "hello"
#=======================================================================================================================
#         output = 'FAIL 810/001: xy->01 mapping'             :(end)
#-----------------------------------------------------------------------------------------------------------------------
n23_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 28
                        mov              rax, qword ptr [rip + .Lx58_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n26_assign_α
.Lx58_0:
                        .quad            .Lx58_0_s
.Lx58_0_s:
                        .string          "FAIL 810/001: xy->01 mapping"
#-----------------------------------------------------------------------------------------------------------------------
n24_keyword_snobol4_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx59_0]
                        call             rt_keyword_read_snobol4@PLT
                        mov              qword ptr [rsp + 544], rax
                        mov              qword ptr [rsp + 552], rdx
                        add              rsp, 16
                                                                                        jmp   n27_var_α
.Lx59_0:
                        .quad            .Lx59_0_s
.Lx59_0_s:
                        .string          "alphabet"
#-----------------------------------------------------------------------------------------------------------------------
n25_call_α:
                        mov              rax, qword ptr [rsp + 656]
                        mov              qword ptr [rsp + 608], rax
                        mov              rax, qword ptr [rsp + 664]
                        mov              qword ptr [rsp + 616], rax
                        mov              rax, qword ptr [rsp + 784]
                        mov              qword ptr [rsp + 624], rax
                        mov              rax, qword ptr [rsp + 792]
                        mov              qword ptr [rsp + 632], rax
                        .section         .rodata
.Lbynamefn26:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn26]
                        lea              rsi, [rsp + 608]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 592], rax
                        mov              qword ptr [rsp + 600], rdx
                        cmp              eax, 99
                                                                                        je    n8_lit_string_α
                                                                                        jmp   n28_lit_string_α
n25_call_β:
                                                                                        jmp   n8_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n26_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n27_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 544], rax
                        mov              qword ptr [rsp + 552], rdx
                                                                                        jmp   n29_call_α
#=======================================================================================================================
#         output = 'FAIL 810/003: identity replace'           :(end)
#-----------------------------------------------------------------------------------------------------------------------
n28_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 30
                        mov              rax, qword ptr [rip + .Lx63_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n30_assign_α
.Lx63_0:
                        .quad            .Lx63_0_s
.Lx63_0_s:
                        .string          "FAIL 810/003: identity replace"
#-----------------------------------------------------------------------------------------------------------------------
n29_call_α:
                        mov              rax, qword ptr [rsp + 512]
                        mov              qword ptr [rsp + 448], rax
                        mov              rax, qword ptr [rsp + 520]
                        mov              qword ptr [rsp + 456], rax
                        mov              rax, qword ptr [rsp + 528]
                        mov              qword ptr [rsp + 464], rax
                        mov              rax, qword ptr [rsp + 536]
                        mov              qword ptr [rsp + 472], rax
                        mov              rax, qword ptr [rsp + 544]
                        mov              qword ptr [rsp + 480], rax
                        mov              rax, qword ptr [rsp + 552]
                        mov              qword ptr [rsp + 488], rax
                        .section         .rodata
.Lbynamefn30:           .string          "replace"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn30]
                        lea              rsi, [rsp + 448]
                        mov              edx, 3
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 432], rax
                        mov              qword ptr [rsp + 440], rdx
                        cmp              eax, 99
                                                                                        je    n7_lit_string_α
                                                                                        jmp   n31_lit_string_α
n29_call_β:
                                                                                        jmp   n7_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n30_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n31_lit_string_α:
                        mov              qword ptr [rsp + 560], 1
                        mov              dword ptr [rsp + 564], 3
                        mov              rax, qword ptr [rip + .Lx66_0]
                        mov              qword ptr [rsp + 568], rax
                                                                                        jmp   n32_call_α
.Lx66_0:
                        .quad            .Lx66_0_s
.Lx66_0_s:
                        .string          "aab"
#-----------------------------------------------------------------------------------------------------------------------
n32_call_α:
                        mov              rax, qword ptr [rsp + 432]
                        mov              qword ptr [rsp + 384], rax
                        mov              rax, qword ptr [rsp + 440]
                        mov              qword ptr [rsp + 392], rax
                        mov              rax, qword ptr [rsp + 560]
                        mov              qword ptr [rsp + 400], rax
                        mov              rax, qword ptr [rsp + 568]
                        mov              qword ptr [rsp + 408], rax
                        .section         .rodata
.Lbynamefn33:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn33]
                        lea              rsi, [rsp + 384]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 368], rax
                        mov              qword ptr [rsp + 376], rdx
                        cmp              eax, 99
                                                                                        je    n7_lit_string_α
                                                                                        jmp   n33_lit_string_α
n32_call_β:
                                                                                        jmp   n7_lit_string_α
#=======================================================================================================================
#         output = 'FAIL 810/002: alphabet translation'       :(end)
#-----------------------------------------------------------------------------------------------------------------------
n33_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 34
                        mov              rax, qword ptr [rip + .Lx68_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n34_assign_α
.Lx68_0:
                        .quad            .Lx68_0_s
.Lx68_0_s:
                        .string          "FAIL 810/002: alphabet translation"
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
                        add              rsp, 840
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              eax, 99
                        xor              edx, edx
                        add              rsp, 840
                        ret
                        .section         .note.GNU-stack,"",@progbits

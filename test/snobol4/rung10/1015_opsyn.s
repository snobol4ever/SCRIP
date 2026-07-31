                        .intel_syntax    noprefix
                        .text
                        .section         .rodata
.Lgvan0:                .string          "dupl"
.Lgvan1:                .string          "output"
.Lgvan2:                .string          "size"
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
                        sub              rsp, 744
                        mov              rdi, rsp
                        mov              ecx, 744
                        xor              eax, eax
                        rep stosb
main_α_body:
#=======================================================================================================================
#         opsyn('@', .dupl, 2)
#-----------------------------------------------------------------------------------------------------------------------
n0_lit_string_α:
                        mov              qword ptr [rsp + 80], 1
                        mov              dword ptr [rsp + 84], 1
                        mov              rax, qword ptr [rip + .Lx28_0]
                        mov              qword ptr [rsp + 88], rax
                                                                                        jmp   n4_lit_string_α
.Lx28_0:
                        .quad            .Lx28_0_s
.Lx28_0_s:
                        .string          "@"
#-----------------------------------------------------------------------------------------------------------------------
n1_goto_α:
                                                                                        jmp   n5_lit_string_α
n1_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n2_goto_α:
                                                                                        jmp   n6_lit_string_α
n2_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n3_goto_α:
                                                                                        jmp   main_γ
n3_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n4_lit_string_α:
                        mov              qword ptr [rsp + 144], 1
                        mov              dword ptr [rsp + 148], 4
                        mov              rax, qword ptr [rip + .Lx32_0]
                        mov              qword ptr [rsp + 152], rax
                                                                                        jmp   n7_call_α
.Lx32_0:
                        .quad            .Lx32_0_s
.Lx32_0_s:
                        .string          "dupl"
#=======================================================================================================================
#         opsyn('|', .size, 1)
#-----------------------------------------------------------------------------------------------------------------------
n5_lit_string_α:
                        mov              qword ptr [rsp + 464], 1
                        mov              dword ptr [rsp + 468], 1
                        mov              rax, qword ptr [rip + .Lx33_0]
                        mov              qword ptr [rsp + 472], rax
                                                                                        jmp   n8_lit_string_α
.Lx33_0:
                        .quad            .Lx33_0_s
.Lx33_0_s:
                        .string          "|"
#=======================================================================================================================
#         output = 'PASS 1015_opsyn (2/2)'
#-----------------------------------------------------------------------------------------------------------------------
n6_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 21
                        mov              rax, qword ptr [rip + .Lx34_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n9_assign_α
.Lx34_0:
                        .quad            .Lx34_0_s
.Lx34_0_s:
                        .string          "PASS 1015_opsyn (2/2)"
#-----------------------------------------------------------------------------------------------------------------------
n7_call_α:
                        mov              rax, qword ptr [rsp + 144]
                        mov              qword ptr [rsp + 112], rax
                        mov              rax, qword ptr [rsp + 152]
                        mov              qword ptr [rsp + 120], rax
                        .section         .rodata
.Lrkfn36:               .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn36]
                        lea              rsi, [rsp + 112]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 96], rax
                        mov              qword ptr [rsp + 104], rdx
                        cmp              eax, 99
                                                                                        je    n11_lit_string_α
                                                                                        jmp   n10_lit_integer_α
n7_call_β:
                                                                                        jmp   n11_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n8_lit_string_α:
                        mov              qword ptr [rsp + 528], 1
                        mov              dword ptr [rsp + 532], 4
                        mov              rax, qword ptr [rip + .Lx37_0]
                        mov              qword ptr [rsp + 536], rax
                                                                                        jmp   n12_call_α
.Lx37_0:
                        .quad            .Lx37_0_s
.Lx37_0_s:
                        .string          "size"
#-----------------------------------------------------------------------------------------------------------------------
n9_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n10_lit_integer_α:
                        mov              qword ptr [rsp + 160], 6
                        mov              rax, qword ptr [rip + .Lx39_0]
                        mov              qword ptr [rsp + 168], rax
                                                                                        jmp   n13_call_α
.Lx39_0:
                        .quad            2
#=======================================================================================================================
#         opsyn('@', .dupl, 2)
#         differ('a' @ 4, 'aaaa')                   :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n11_lit_string_α:
                        mov              qword ptr [rsp + 304], 1
                        mov              dword ptr [rsp + 308], 1
                        mov              rax, qword ptr [rip + .Lx40_0]
                        mov              qword ptr [rsp + 312], rax
                                                                                        jmp   n14_lit_integer_α
.Lx40_0:
                        .quad            .Lx40_0_s
.Lx40_0_s:
                        .string          "a"
#-----------------------------------------------------------------------------------------------------------------------
n12_call_α:
                        mov              rax, qword ptr [rsp + 528]
                        mov              qword ptr [rsp + 496], rax
                        mov              rax, qword ptr [rsp + 536]
                        mov              qword ptr [rsp + 504], rax
                        .section         .rodata
.Lrkfn42:               .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn42]
                        lea              rsi, [rsp + 496]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 480], rax
                        mov              qword ptr [rsp + 488], rdx
                        cmp              eax, 99
                                                                                        je    n16_lit_string_α
                                                                                        jmp   n15_lit_integer_α
n12_call_β:
                                                                                        jmp   n16_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n13_call_α:
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [rsp + 96]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [rsp + 160]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 168]
                        mov              qword ptr [rsp + 56], rax
                        .section         .rodata
.Lbynamefn14:           .string          "opsyn"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn14]
                        lea              rsi, [rsp + 16]
                        mov              edx, 3
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 99
                                                                                        je    n11_lit_string_α
                                                                                        jmp   n11_lit_string_α
n13_call_β:
                                                                                        jmp   n11_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n14_lit_integer_α:
                        mov              qword ptr [rsp + 320], 6
                        mov              rax, qword ptr [rip + .Lx44_0]
                        mov              qword ptr [rsp + 328], rax
                                                                                        jmp   n17_call_α
.Lx44_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n15_lit_integer_α:
                        mov              qword ptr [rsp + 544], 6
                        mov              rax, qword ptr [rip + .Lx45_0]
                        mov              qword ptr [rsp + 552], rax
                                                                                        jmp   n18_call_α
.Lx45_0:
                        .quad            1
#=======================================================================================================================
#         opsyn('|', .size, 1)
#         differ(|'string', 6)                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n16_lit_string_α:
                        mov              qword ptr [rsp + 672], 1
                        mov              dword ptr [rsp + 676], 6
                        mov              rax, qword ptr [rip + .Lx46_0]
                        mov              qword ptr [rsp + 680], rax
                                                                                        jmp   n19_call_α
.Lx46_0:
                        .quad            .Lx46_0_s
.Lx46_0_s:
                        .string          "string"
#-----------------------------------------------------------------------------------------------------------------------
n17_call_α:
                        mov              rax, qword ptr [rsp + 304]
                        mov              qword ptr [rsp + 256], rax
                        mov              rax, qword ptr [rsp + 312]
                        mov              qword ptr [rsp + 264], rax
                        mov              rax, qword ptr [rsp + 320]
                        mov              qword ptr [rsp + 272], rax
                        mov              rax, qword ptr [rsp + 328]
                        mov              qword ptr [rsp + 280], rax
                        .section         .rodata
.Lbynamefn18:           .string          "@"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn18]
                        lea              rsi, [rsp + 256]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 240], rax
                        mov              qword ptr [rsp + 248], rdx
                        cmp              eax, 99
                                                                                        je    n5_lit_string_α
                                                                                        jmp   n20_lit_string_α
n17_call_β:
                                                                                        jmp   n5_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n18_call_α:
                        mov              rax, qword ptr [rsp + 464]
                        mov              qword ptr [rsp + 400], rax
                        mov              rax, qword ptr [rsp + 472]
                        mov              qword ptr [rsp + 408], rax
                        mov              rax, qword ptr [rsp + 480]
                        mov              qword ptr [rsp + 416], rax
                        mov              rax, qword ptr [rsp + 488]
                        mov              qword ptr [rsp + 424], rax
                        mov              rax, qword ptr [rsp + 544]
                        mov              qword ptr [rsp + 432], rax
                        mov              rax, qword ptr [rsp + 552]
                        mov              qword ptr [rsp + 440], rax
                        .section         .rodata
.Lbynamefn19:           .string          "opsyn"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn19]
                        lea              rsi, [rsp + 400]
                        mov              edx, 3
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 384], rax
                        mov              qword ptr [rsp + 392], rdx
                        cmp              eax, 99
                                                                                        je    n16_lit_string_α
                                                                                        jmp   n16_lit_string_α
n18_call_β:
                                                                                        jmp   n16_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n19_call_α:
                        mov              rax, qword ptr [rsp + 672]
                        mov              qword ptr [rsp + 640], rax
                        mov              rax, qword ptr [rsp + 680]
                        mov              qword ptr [rsp + 648], rax
                        .section         .rodata
.Lbynamefn20:           .string          "|"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn20]
                        lea              rsi, [rsp + 640]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 624], rax
                        mov              qword ptr [rsp + 632], rdx
                        cmp              eax, 99
                                                                                        je    n6_lit_string_α
                                                                                        jmp   n21_lit_integer_α
n19_call_β:
                                                                                        jmp   n6_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n20_lit_string_α:
                        mov              qword ptr [rsp + 336], 1
                        mov              dword ptr [rsp + 340], 4
                        mov              rax, qword ptr [rip + .Lx50_0]
                        mov              qword ptr [rsp + 344], rax
                                                                                        jmp   n22_call_α
.Lx50_0:
                        .quad            .Lx50_0_s
.Lx50_0_s:
                        .string          "aaaa"
#-----------------------------------------------------------------------------------------------------------------------
n21_lit_integer_α:
                        mov              qword ptr [rsp + 688], 6
                        mov              rax, qword ptr [rip + .Lx51_0]
                        mov              qword ptr [rsp + 696], rax
                                                                                        jmp   n23_call_α
.Lx51_0:
                        .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n22_call_α:
                        mov              rax, qword ptr [rsp + 240]
                        mov              qword ptr [rsp + 192], rax
                        mov              rax, qword ptr [rsp + 248]
                        mov              qword ptr [rsp + 200], rax
                        mov              rax, qword ptr [rsp + 336]
                        mov              qword ptr [rsp + 208], rax
                        mov              rax, qword ptr [rsp + 344]
                        mov              qword ptr [rsp + 216], rax
                        .section         .rodata
.Lbynamefn23:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn23]
                        lea              rsi, [rsp + 192]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 176], rax
                        mov              qword ptr [rsp + 184], rdx
                        cmp              eax, 99
                                                                                        je    n5_lit_string_α
                                                                                        jmp   n24_lit_string_α
n22_call_β:
                                                                                        jmp   n5_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n23_call_α:
                        mov              rax, qword ptr [rsp + 624]
                        mov              qword ptr [rsp + 576], rax
                        mov              rax, qword ptr [rsp + 632]
                        mov              qword ptr [rsp + 584], rax
                        mov              rax, qword ptr [rsp + 688]
                        mov              qword ptr [rsp + 592], rax
                        mov              rax, qword ptr [rsp + 696]
                        mov              qword ptr [rsp + 600], rax
                        .section         .rodata
.Lbynamefn24:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn24]
                        lea              rsi, [rsp + 576]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 560], rax
                        mov              qword ptr [rsp + 568], rdx
                        cmp              eax, 99
                                                                                        je    n6_lit_string_α
                                                                                        jmp   n25_lit_string_α
n23_call_β:
                                                                                        jmp   n6_lit_string_α
#=======================================================================================================================
#         output = 'FAIL 1015/001: @ as binary dupl'     :(end)
#-----------------------------------------------------------------------------------------------------------------------
n24_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 31
                        mov              rax, qword ptr [rip + .Lx54_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n26_assign_α
.Lx54_0:
                        .quad            .Lx54_0_s
.Lx54_0_s:
                        .string          "FAIL 1015/001: @ as binary dupl"
#=======================================================================================================================
#         output = 'FAIL 1015/002: | as unary size'      :(end)
#-----------------------------------------------------------------------------------------------------------------------
n25_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 30
                        mov              rax, qword ptr [rip + .Lx55_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n27_assign_α
.Lx55_0:
                        .quad            .Lx55_0_s
.Lx55_0_s:
                        .string          "FAIL 1015/002: | as unary size"
#-----------------------------------------------------------------------------------------------------------------------
n26_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n27_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        add              rsp, 744
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              eax, 99
                        xor              edx, edx
                        add              rsp, 744
                        ret
                        .section         .note.GNU-stack,"",@progbits

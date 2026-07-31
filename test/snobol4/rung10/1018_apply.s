                        .intel_syntax    noprefix
                        .text
                        .section         .rodata
.Lgvan0:                .string          "eq"
.Lgvan1:                .string          "output"
.Lgvan2:                .string          "trim"
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
                        sub              rsp, 664
                        mov              rdi, rsp
                        mov              ecx, 664
                        xor              eax, eax
                        rep stosb
main_α_body:
#=======================================================================================================================
#         apply(.eq, 1, 2)           :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n0_lit_string_α:
                        mov              qword ptr [rsp + 128], 1
                        mov              dword ptr [rsp + 132], 2
                        mov              rax, qword ptr [rip + .Lx28_0]
                        mov              qword ptr [rsp + 136], rax
                                                                                        jmp   n1_call_α
.Lx28_0:
                        .quad            .Lx28_0_s
.Lx28_0_s:
                        .string          "eq"
#-----------------------------------------------------------------------------------------------------------------------
n1_call_α:
                        mov              rax, qword ptr [rsp + 128]
                        mov              qword ptr [rsp + 96], rax
                        mov              rax, qword ptr [rsp + 136]
                        mov              qword ptr [rsp + 104], rax
                        .section         .rodata
.Lrkfn30:               .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn30]
                        lea              rsi, [rsp + 96]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 80], rax
                        mov              qword ptr [rsp + 88], rdx
                        cmp              eax, 99
                                                                                        je    n7_lit_string_α
                                                                                        jmp   n2_lit_integer_α
n1_call_β:
                                                                                        jmp   n7_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n2_lit_integer_α:
                        mov              qword ptr [rsp + 144], 6
                        mov              rax, qword ptr [rip + .Lx31_0]
                        mov              qword ptr [rsp + 152], rax
                                                                                        jmp   n3_lit_integer_α
.Lx31_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_integer_α:
                        mov              qword ptr [rsp + 160], 6
                        mov              rax, qword ptr [rip + .Lx32_0]
                        mov              qword ptr [rsp + 168], rax
                                                                                        jmp   n4_call_α
.Lx32_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n4_call_α:
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [rsp + 144]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 152]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [rsp + 160]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 168]
                        mov              qword ptr [rsp + 56], rax
                        .section         .rodata
.Lbynamefn5:            .string          "apply"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn5]
                        lea              rsi, [rsp + 16]
                        mov              edx, 3
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 99
                                                                                        je    n7_lit_string_α
                                                                                        jmp   n5_lit_string_α
n4_call_β:
                                                                                        jmp   n7_lit_string_α
#=======================================================================================================================
#         output = 'FAIL 1018/001: apply(.eq,1,2) should fail' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n5_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 41
                        mov              rax, qword ptr [rip + .Lx34_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n6_assign_α
.Lx34_0:
                        .quad            .Lx34_0_s
.Lx34_0_s:
                        .string          "FAIL 1018/001: apply(.eq,1,2) should fail"
#-----------------------------------------------------------------------------------------------------------------------
n6_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         apply(.eq, 1, 1)           :s(e002)
#-----------------------------------------------------------------------------------------------------------------------
n7_lit_string_α:
                        mov              qword ptr [rsp + 336], 1
                        mov              dword ptr [rsp + 340], 2
                        mov              rax, qword ptr [rip + .Lx36_0]
                        mov              qword ptr [rsp + 344], rax
                                                                                        jmp   n8_call_α
.Lx36_0:
                        .quad            .Lx36_0_s
.Lx36_0_s:
                        .string          "eq"
#-----------------------------------------------------------------------------------------------------------------------
n8_call_α:
                        mov              rax, qword ptr [rsp + 336]
                        mov              qword ptr [rsp + 304], rax
                        mov              rax, qword ptr [rsp + 344]
                        mov              qword ptr [rsp + 312], rax
                        .section         .rodata
.Lrkfn38:               .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn38]
                        lea              rsi, [rsp + 304]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 288], rax
                        mov              qword ptr [rsp + 296], rdx
                        cmp              eax, 99
                                                                                        je    n22_lit_string_α
                                                                                        jmp   n9_lit_integer_α
n8_call_β:
                                                                                        jmp   n22_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n9_lit_integer_α:
                        mov              qword ptr [rsp + 352], 6
                        mov              rax, qword ptr [rip + .Lx39_0]
                        mov              qword ptr [rsp + 360], rax
                                                                                        jmp   n10_lit_integer_α
.Lx39_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n10_lit_integer_α:
                        mov              qword ptr [rsp + 368], 6
                        mov              rax, qword ptr [rip + .Lx40_0]
                        mov              qword ptr [rsp + 376], rax
                                                                                        jmp   n11_call_α
.Lx40_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n11_call_α:
                        mov              rax, qword ptr [rsp + 288]
                        mov              qword ptr [rsp + 224], rax
                        mov              rax, qword ptr [rsp + 296]
                        mov              qword ptr [rsp + 232], rax
                        mov              rax, qword ptr [rsp + 352]
                        mov              qword ptr [rsp + 240], rax
                        mov              rax, qword ptr [rsp + 360]
                        mov              qword ptr [rsp + 248], rax
                        mov              rax, qword ptr [rsp + 368]
                        mov              qword ptr [rsp + 256], rax
                        mov              rax, qword ptr [rsp + 376]
                        mov              qword ptr [rsp + 264], rax
                        .section         .rodata
.Lbynamefn12:           .string          "apply"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn12]
                        lea              rsi, [rsp + 224]
                        mov              edx, 3
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 208], rax
                        mov              qword ptr [rsp + 216], rdx
                        cmp              eax, 99
                                                                                        je    n22_lit_string_α
                                                                                        jmp   n12_lit_string_α
n11_call_β:
                                                                                        jmp   n22_lit_string_α
#=======================================================================================================================
#         differ(apply(.trim, 'abc   '), 'abc')                   :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n12_lit_string_α:
                        mov              qword ptr [rsp + 576], 1
                        mov              dword ptr [rsp + 580], 4
                        mov              rax, qword ptr [rip + .Lx42_0]
                        mov              qword ptr [rsp + 584], rax
                                                                                        jmp   n13_call_α
.Lx42_0:
                        .quad            .Lx42_0_s
.Lx42_0_s:
                        .string          "trim"
#-----------------------------------------------------------------------------------------------------------------------
n13_call_α:
                        mov              rax, qword ptr [rsp + 576]
                        mov              qword ptr [rsp + 544], rax
                        mov              rax, qword ptr [rsp + 584]
                        mov              qword ptr [rsp + 552], rax
                        .section         .rodata
.Lrkfn44:               .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn44]
                        lea              rsi, [rsp + 544]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 528], rax
                        mov              qword ptr [rsp + 536], rdx
                        cmp              eax, 99
                                                                                        je    n20_lit_string_α
                                                                                        jmp   n14_lit_string_α
n13_call_β:
                                                                                        jmp   n20_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n14_lit_string_α:
                        mov              qword ptr [rsp + 592], 1
                        mov              dword ptr [rsp + 596], 6
                        mov              rax, qword ptr [rip + .Lx45_0]
                        mov              qword ptr [rsp + 600], rax
                                                                                        jmp   n15_call_α
.Lx45_0:
                        .quad            .Lx45_0_s
.Lx45_0_s:
                        .string          "abc   "
#-----------------------------------------------------------------------------------------------------------------------
n15_call_α:
                        mov              rax, qword ptr [rsp + 528]
                        mov              qword ptr [rsp + 480], rax
                        mov              rax, qword ptr [rsp + 536]
                        mov              qword ptr [rsp + 488], rax
                        mov              rax, qword ptr [rsp + 592]
                        mov              qword ptr [rsp + 496], rax
                        mov              rax, qword ptr [rsp + 600]
                        mov              qword ptr [rsp + 504], rax
                        .section         .rodata
.Lbynamefn16:           .string          "apply"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn16]
                        lea              rsi, [rsp + 480]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 464], rax
                        mov              qword ptr [rsp + 472], rdx
                        cmp              eax, 99
                                                                                        je    n20_lit_string_α
                                                                                        jmp   n16_lit_string_α
n15_call_β:
                                                                                        jmp   n20_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n16_lit_string_α:
                        mov              qword ptr [rsp + 608], 1
                        mov              dword ptr [rsp + 612], 3
                        mov              rax, qword ptr [rip + .Lx47_0]
                        mov              qword ptr [rsp + 616], rax
                                                                                        jmp   n17_call_α
.Lx47_0:
                        .quad            .Lx47_0_s
.Lx47_0_s:
                        .string          "abc"
#-----------------------------------------------------------------------------------------------------------------------
n17_call_α:
                        mov              rax, qword ptr [rsp + 464]
                        mov              qword ptr [rsp + 416], rax
                        mov              rax, qword ptr [rsp + 472]
                        mov              qword ptr [rsp + 424], rax
                        mov              rax, qword ptr [rsp + 608]
                        mov              qword ptr [rsp + 432], rax
                        mov              rax, qword ptr [rsp + 616]
                        mov              qword ptr [rsp + 440], rax
                        .section         .rodata
.Lbynamefn18:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn18]
                        lea              rsi, [rsp + 416]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 400], rax
                        mov              qword ptr [rsp + 408], rdx
                        cmp              eax, 99
                                                                                        je    n20_lit_string_α
                                                                                        jmp   n18_lit_string_α
n17_call_β:
                                                                                        jmp   n20_lit_string_α
#=======================================================================================================================
#         output = 'FAIL 1018/003: apply(.trim,...)'     :(end)
#-----------------------------------------------------------------------------------------------------------------------
n18_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 31
                        mov              rax, qword ptr [rip + .Lx49_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n19_assign_α
.Lx49_0:
                        .quad            .Lx49_0_s
.Lx49_0_s:
                        .string          "FAIL 1018/003: apply(.trim,...)"
#-----------------------------------------------------------------------------------------------------------------------
n19_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         output = 'PASS 1018_apply (3/3)'
#-----------------------------------------------------------------------------------------------------------------------
n20_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 21
                        mov              rax, qword ptr [rip + .Lx51_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n21_assign_α
.Lx51_0:
                        .quad            .Lx51_0_s
.Lx51_0_s:
                        .string          "PASS 1018_apply (3/3)"
#-----------------------------------------------------------------------------------------------------------------------
n21_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#         output = 'FAIL 1018/002: apply(.eq,1,1) should succeed' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n22_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 44
                        mov              rax, qword ptr [rip + .Lx53_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n23_assign_α
.Lx53_0:
                        .quad            .Lx53_0_s
.Lx53_0_s:
                        .string          "FAIL 1018/002: apply(.eq,1,1) should succeed"
#-----------------------------------------------------------------------------------------------------------------------
n23_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n24_goto_α:
                                                                                        jmp   n7_lit_string_α
n24_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n25_goto_α:
                                                                                        jmp   n12_lit_string_α
n25_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n26_goto_α:
                                                                                        jmp   n20_lit_string_α
n26_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n27_goto_α:
                                                                                        jmp   main_γ
n27_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        add              rsp, 664
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              eax, 99
                        xor              edx, edx
                        add              rsp, 664
                        ret
                        .section         .note.GNU-stack,"",@progbits

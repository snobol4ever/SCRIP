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
                        sub              rsp, 328
                        mov              rdi, rsp
                        mov              ecx, 328
                        xor              eax, eax
                        rep stosb
main_α_body:
#=======================================================================================================================
#         differ(-5, 0 - 5)                   :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n0_lit_integer_α:
                        mov              qword ptr [rsp + 80], 6
                        mov              rax, qword ptr [rip + .Lx19_0]
                        mov              qword ptr [rsp + 88], rax
                                                                                        jmp   n4_unop_α
.Lx19_0:
                        .quad            5
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
n4_unop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 96]
                        mov              rsi, qword ptr [rsp + 104]
                        call             rt_num_neg@PLT
                        mov              qword ptr [rsp + 80], rax
                        mov              qword ptr [rsp + 88], rdx
                        add              rsp, 16
                                                                                        jmp   n7_lit_integer_α
#=======================================================================================================================
#         differ(+'4', 4)                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n5_lit_string_α:
                        mov              qword ptr [rsp + 256], 1
                        mov              dword ptr [rsp + 260], 1
                        mov              rax, qword ptr [rip + .Lx24_0]
                        mov              qword ptr [rsp + 264], rax
                                                                                        jmp   n8_unop_α
.Lx24_0:
                        .quad            .Lx24_0_s
.Lx24_0_s:
                        .string          "4"
#=======================================================================================================================
#         output = 'PASS 411_arith_unary (2/2)'
#-----------------------------------------------------------------------------------------------------------------------
n6_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 26
                        mov              rax, qword ptr [rip + .Lx25_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n9_assign_α
.Lx25_0:
                        .quad            .Lx25_0_s
.Lx25_0_s:
                        .string          "PASS 411_arith_unary (2/2)"
#-----------------------------------------------------------------------------------------------------------------------
n7_lit_integer_α:
                        mov              qword ptr [rsp + 112], 6
                        mov              rax, qword ptr [rip + .Lx26_0]
                        mov              qword ptr [rsp + 120], rax
                                                                                        jmp   n10_lit_integer_α
.Lx26_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n8_unop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 272]
                        mov              rsi, qword ptr [rsp + 280]
                        call             rt_num_pos@PLT
                        mov              qword ptr [rsp + 256], rax
                        mov              qword ptr [rsp + 264], rdx
                        add              rsp, 16
                                                                                        jmp   n11_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n9_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n10_lit_integer_α:
                        mov              qword ptr [rsp + 128], 6
                        mov              rax, qword ptr [rip + .Lx29_0]
                        mov              qword ptr [rsp + 136], rax
                                                                                        jmp   n12_binop_α
.Lx29_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n11_lit_integer_α:
                        mov              qword ptr [rsp + 272], 6
                        mov              rax, qword ptr [rip + .Lx30_0]
                        mov              qword ptr [rsp + 280], rax
                                                                                        jmp   n13_call_α
.Lx30_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n12_binop_α:
                        mov              rdi, qword ptr [rsp + 112]
                        mov              rsi, qword ptr [rsp + 120]
                        mov              rdx, qword ptr [rsp + 128]
                        mov              rcx, qword ptr [rsp + 136]
                        call             rt_sub@PLT
                        cmp              eax, 99
                                                                                        je    n5_lit_string_α
                        mov              qword ptr [rsp + 96], rax
                        mov              qword ptr [rsp + 104], rdx
                                                                                        jmp   n14_call_α
#-----------------------------------------------------------------------------------------------------------------------
n13_call_α:
                        mov              rax, qword ptr [rsp + 240]
                        mov              qword ptr [rsp + 192], rax
                        mov              rax, qword ptr [rsp + 248]
                        mov              qword ptr [rsp + 200], rax
                        mov              rax, qword ptr [rsp + 272]
                        mov              qword ptr [rsp + 208], rax
                        mov              rax, qword ptr [rsp + 280]
                        mov              qword ptr [rsp + 216], rax
                        .section         .rodata
.Lbynamefn14:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn14]
                        lea              rsi, [rsp + 192]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 176], rax
                        mov              qword ptr [rsp + 184], rdx
                        cmp              eax, 99
                                                                                        je    n6_lit_string_α
                                                                                        jmp   n15_lit_string_α
n13_call_β:
                                                                                        jmp   n6_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n14_call_α:
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [rsp + 96]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 40], rax
                        .section         .rodata
.Lbynamefn15:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn15]
                        lea              rsi, [rsp + 16]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 99
                                                                                        je    n5_lit_string_α
                                                                                        jmp   n16_lit_string_α
n14_call_β:
                                                                                        jmp   n5_lit_string_α
#=======================================================================================================================
#         output = 'FAIL 411/002: unary plus string->int' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n15_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 36
                        mov              rax, qword ptr [rip + .Lx34_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n17_assign_α
.Lx34_0:
                        .quad            .Lx34_0_s
.Lx34_0_s:
                        .string          "FAIL 411/002: unary plus string->int"
#=======================================================================================================================
#         output = 'FAIL 411/001: unary minus'           :(end)
#-----------------------------------------------------------------------------------------------------------------------
n16_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 25
                        mov              rax, qword ptr [rip + .Lx35_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n18_assign_α
.Lx35_0:
                        .quad            .Lx35_0_s
.Lx35_0_s:
                        .string          "FAIL 411/001: unary minus"
#-----------------------------------------------------------------------------------------------------------------------
n17_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n18_assign_α:
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
                        add              rsp, 328
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              eax, 99
                        xor              edx, edx
                        add              rsp, 328
                        ret
                        .section         .note.GNU-stack,"",@progbits

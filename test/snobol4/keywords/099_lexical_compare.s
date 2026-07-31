                        .intel_syntax    noprefix
                        .text
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
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
                        sub              rsp, 216
                        mov              rdi, rsp
                        mov              ecx, 216
                        xor              eax, eax
                        rep stosb
main_α_body:
#=======================================================================================================================
#         LGT('b', 'a')                                               :S(A)F(END)
#-----------------------------------------------------------------------------------------------------------------------
n0_lit_string_α:
                        mov              qword ptr [rsp + 32], 1
                        mov              dword ptr [rsp + 36], 1
                        mov              rax, qword ptr [rip + .Lx24_0]
                        mov              qword ptr [rsp + 40], rax
                                                                                        jmp   n5_lit_string_α
.Lx24_0:
                        .quad            .Lx24_0_s
.Lx24_0_s:
                        .string          "b"
#-----------------------------------------------------------------------------------------------------------------------
n1_goto_α:
                                                                                        jmp   n6_lit_string_α
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
                                                                                        jmp   n9_lit_string_α
n4_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n5_lit_string_α:
                        mov              qword ptr [rsp + 16], 1
                        mov              dword ptr [rsp + 20], 1
                        mov              rax, qword ptr [rip + .Lx29_0]
                        mov              qword ptr [rsp + 24], rax
                                                                                        jmp   n10_op77_α
.Lx29_0:
                        .quad            .Lx29_0_s
.Lx29_0_s:
                        .string          "a"
#=======================================================================================================================
# A       OUTPUT = 'b > a'
#-----------------------------------------------------------------------------------------------------------------------
n6_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx30_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n11_assign_α
.Lx30_0:
                        .quad            .Lx30_0_s
.Lx30_0_s:
                        .string          "b > a"
#=======================================================================================================================
# B       OUTPUT = 'a < b'
#-----------------------------------------------------------------------------------------------------------------------
n7_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx31_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n12_assign_α
.Lx31_0:
                        .quad            .Lx31_0_s
.Lx31_0_s:
                        .string          "a < b"
#=======================================================================================================================
# C       OUTPUT = 'cat = cat'
#-----------------------------------------------------------------------------------------------------------------------
n8_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 9
                        mov              rax, qword ptr [rip + .Lx32_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n13_assign_α
.Lx32_0:
                        .quad            .Lx32_0_s
.Lx32_0_s:
                        .string          "cat = cat"
#=======================================================================================================================
# D       OUTPUT = 'cat != dog'
#-----------------------------------------------------------------------------------------------------------------------
n9_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 10
                        mov              rax, qword ptr [rip + .Lx33_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n14_assign_α
.Lx33_0:
                        .quad            .Lx33_0_s
.Lx33_0_s:
                        .string          "cat != dog"
#-----------------------------------------------------------------------------------------------------------------------
n10_op77_α:
                        lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jle   main_γ
                                                                                        jmp   n6_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n11_assign_α:
                        mov              rsi, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx36_0]
                        call             NV_SET_fn@PLT
                        add              rsp, 16
                                                                                        jmp   n15_lit_string_α
.Lx36_0:
                        .quad            .Lx36_0_s
.Lx36_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n12_assign_α:
                        mov              rsi, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx37_0]
                        call             NV_SET_fn@PLT
                        add              rsp, 16
                                                                                        jmp   n16_lit_string_α
.Lx37_0:
                        .quad            .Lx37_0_s
.Lx37_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n13_assign_α:
                        mov              rsi, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx38_0]
                        call             NV_SET_fn@PLT
                        add              rsp, 16
                                                                                        jmp   n17_lit_string_α
.Lx38_0:
                        .quad            .Lx38_0_s
.Lx38_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n14_assign_α:
                        mov              rsi, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx39_0]
                        call             NV_SET_fn@PLT
                        add              rsp, 16
                                                                                        jmp   main_γ
.Lx39_0:
                        .quad            .Lx39_0_s
.Lx39_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#         LLT('a', 'b')                                               :S(B)F(END)
#-----------------------------------------------------------------------------------------------------------------------
n15_lit_string_α:
                        mov              qword ptr [rsp + 80], 1
                        mov              dword ptr [rsp + 84], 1
                        mov              rax, qword ptr [rip + .Lx40_0]
                        mov              qword ptr [rsp + 88], rax
                                                                                        jmp   n18_lit_string_α
.Lx40_0:
                        .quad            .Lx40_0_s
.Lx40_0_s:
                        .string          "a"
#=======================================================================================================================
#         LEQ('cat', 'cat')                                           :S(C)F(END)
#-----------------------------------------------------------------------------------------------------------------------
n16_lit_string_α:
                        mov              qword ptr [rsp + 128], 1
                        mov              dword ptr [rsp + 132], 3
                        mov              rax, qword ptr [rip + .Lx41_0]
                        mov              qword ptr [rsp + 136], rax
                                                                                        jmp   n19_lit_string_α
.Lx41_0:
                        .quad            .Lx41_0_s
.Lx41_0_s:
                        .string          "cat"
#=======================================================================================================================
#         LNE('cat', 'dog')                                           :S(D)F(END)
#-----------------------------------------------------------------------------------------------------------------------
n17_lit_string_α:
                        mov              qword ptr [rsp + 176], 1
                        mov              dword ptr [rsp + 180], 3
                        mov              rax, qword ptr [rip + .Lx42_0]
                        mov              qword ptr [rsp + 184], rax
                                                                                        jmp   n20_lit_string_α
.Lx42_0:
                        .quad            .Lx42_0_s
.Lx42_0_s:
                        .string          "cat"
#-----------------------------------------------------------------------------------------------------------------------
n18_lit_string_α:
                        mov              qword ptr [rsp + 64], 1
                        mov              dword ptr [rsp + 68], 1
                        mov              rax, qword ptr [rip + .Lx43_0]
                        mov              qword ptr [rsp + 72], rax
                                                                                        jmp   n21_op77_α
.Lx43_0:
                        .quad            .Lx43_0_s
.Lx43_0_s:
                        .string          "b"
#-----------------------------------------------------------------------------------------------------------------------
n19_lit_string_α:
                        mov              qword ptr [rsp + 112], 1
                        mov              dword ptr [rsp + 116], 3
                        mov              rax, qword ptr [rip + .Lx44_0]
                        mov              qword ptr [rsp + 120], rax
                                                                                        jmp   n22_op77_α
.Lx44_0:
                        .quad            .Lx44_0_s
.Lx44_0_s:
                        .string          "cat"
#-----------------------------------------------------------------------------------------------------------------------
n20_lit_string_α:
                        mov              qword ptr [rsp + 160], 1
                        mov              dword ptr [rsp + 164], 3
                        mov              rax, qword ptr [rip + .Lx45_0]
                        mov              qword ptr [rsp + 168], rax
                                                                                        jmp   n23_op77_α
.Lx45_0:
                        .quad            .Lx45_0_s
.Lx45_0_s:
                        .string          "dog"
#-----------------------------------------------------------------------------------------------------------------------
n21_op77_α:
                        lea              rdi, [rsp + 80]
                        lea              rsi, [rsp + 64]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jns   main_γ
                                                                                        jmp   n7_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n22_op77_α:
                        lea              rdi, [rsp + 128]
                        lea              rsi, [rsp + 112]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jne   main_γ
                                                                                        jmp   n8_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n23_op77_α:
                        lea              rdi, [rsp + 176]
                        lea              rsi, [rsp + 160]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        je    main_γ
                                                                                        jmp   n9_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        add              rsp, 216
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              eax, 99
                        xor              edx, edx
                        add              rsp, 216
                        ret
                        .section         .note.GNU-stack,"",@progbits

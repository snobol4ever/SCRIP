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
                        sub              rsp, 264
                        mov              rdi, rsp
                        mov              ecx, 264
                        xor              eax, eax
                        rep stosb
                        mov              [rsp + 256], rbp
                        mov              rbp, rsp
main_α_body:
#=======================================================================================================================
#         LT(3, 5)                                                    :S(A)F(END)
#-----------------------------------------------------------------------------------------------------------------------
n0_lit_integer_α:
                        mov              qword ptr [rbp + 64], 6
                        mov              rax, qword ptr [rip + .Lx24_0]
                        mov              qword ptr [rbp + 72], rax
                                                                                        jmp   n4_lit_integer_α
.Lx24_0:
                        .quad            3
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
                                                                                        jmp   n7_lit_string_α
n3_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n4_lit_integer_α:
                        mov              qword ptr [rbp + 48], 6
                        mov              rax, qword ptr [rip + .Lx28_0]
                        mov              qword ptr [rbp + 56], rax
                                                                                        jmp   n8_op75_α
.Lx28_0:
                        .quad            5
#=======================================================================================================================
# A       OUTPUT = '3 < 5'
#-----------------------------------------------------------------------------------------------------------------------
n5_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              rax, qword ptr [rip + .Lx29_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n9_assign_α
.Lx29_0:
                        .quad            .Lx29_0_s
.Lx29_0_s:
                        .string          "3 < 5"
#=======================================================================================================================
# B       OUTPUT = '5 <= 5'
#-----------------------------------------------------------------------------------------------------------------------
n6_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              rax, qword ptr [rip + .Lx30_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n10_assign_α
.Lx30_0:
                        .quad            .Lx30_0_s
.Lx30_0_s:
                        .string          "5 <= 5"
#=======================================================================================================================
# C       OUTPUT = '7 >= 5'
#-----------------------------------------------------------------------------------------------------------------------
n7_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              rax, qword ptr [rip + .Lx31_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n11_assign_α
.Lx31_0:
                        .quad            .Lx31_0_s
.Lx31_0_s:
                        .string          "7 >= 5"
#-----------------------------------------------------------------------------------------------------------------------
n8_op75_α:
                        mov              eax, dword ptr [rbp + 64]
                        cmp              eax, 7
                                                                                        je    .Lx33_1
                        cmp              eax, 6
                                                                                        jne   .Lx33_0
                        mov              eax, dword ptr [rbp + 48]
                        cmp              eax, 6
                                                                                        jne   .Lx33_0
.Lx33_1:
                        mov              rax, qword ptr [rbp + 64]
                        mov              qword ptr [rbp + 32], rax
                        mov              rax, qword ptr [rbp + 72]
                        mov              qword ptr [rbp + 40], rax
                                                                                        jmp   n12_op75_α
.Lx33_0:
                        lea              rdi, [rbp + 64]
                        lea              rsi, [rbp + 48]
                        lea              rdx, [rbp + 32]
                        mov              rcx, 147
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n12_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n9_assign_α:
                        mov              rsi, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx34_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n13_lit_integer_α
.Lx34_0:
                        .quad            .Lx34_0_s
.Lx34_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n10_assign_α:
                        mov              rsi, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx35_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n14_lit_integer_α
.Lx35_0:
                        .quad            .Lx35_0_s
.Lx35_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n11_assign_α:
                        mov              rsi, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx36_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   main_γ
.Lx36_0:
                        .quad            .Lx36_0_s
.Lx36_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n12_op75_α:
                        mov              eax, dword ptr [rbp + 48]
                        cmp              eax, 7
                                                                                        je    .Lx38_1
                        cmp              eax, 6
                                                                                        jne   .Lx38_0
                        mov              eax, dword ptr [rbp + 64]
                        cmp              eax, 6
                                                                                        jne   .Lx38_0
.Lx38_1:
                        mov              rax, qword ptr [rbp + 48]
                        mov              qword ptr [rbp + 16], rax
                        mov              rax, qword ptr [rbp + 56]
                        mov              qword ptr [rbp + 24], rax
                                                                                        jmp   n15_op77_α
.Lx38_0:
                        lea              rdi, [rbp + 48]
                        lea              rsi, [rbp + 64]
                        lea              rdx, [rbp + 16]
                        mov              rcx, 148
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n15_op77_α
#=======================================================================================================================
#         LE(5, 5)                                                    :S(B)F(END)
#-----------------------------------------------------------------------------------------------------------------------
n13_lit_integer_α:
                        mov              qword ptr [rbp + 144], 6
                        mov              rax, qword ptr [rip + .Lx39_0]
                        mov              qword ptr [rbp + 152], rax
                                                                                        jmp   n16_lit_integer_α
.Lx39_0:
                        .quad            5
#=======================================================================================================================
#         GE(7, 5)                                                    :S(C)F(END)
#-----------------------------------------------------------------------------------------------------------------------
n14_lit_integer_α:
                        mov              qword ptr [rbp + 224], 6
                        mov              rax, qword ptr [rip + .Lx40_0]
                        mov              qword ptr [rbp + 232], rax
                                                                                        jmp   n17_lit_integer_α
.Lx40_0:
                        .quad            7
#-----------------------------------------------------------------------------------------------------------------------
n15_op77_α:
                        lea              rdi, [rbp + 32]
                        lea              rsi, [rbp + 16]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jns   main_γ
                                                                                        jmp   n5_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n16_lit_integer_α:
                        mov              qword ptr [rbp + 128], 6
                        mov              rax, qword ptr [rip + .Lx43_0]
                        mov              qword ptr [rbp + 136], rax
                                                                                        jmp   n18_op75_α
.Lx43_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n17_lit_integer_α:
                        mov              qword ptr [rbp + 208], 6
                        mov              rax, qword ptr [rip + .Lx44_0]
                        mov              qword ptr [rbp + 216], rax
                                                                                        jmp   n19_op75_α
.Lx44_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n18_op75_α:
                        mov              eax, dword ptr [rbp + 144]
                        cmp              eax, 7
                                                                                        je    .Lx46_1
                        cmp              eax, 6
                                                                                        jne   .Lx46_0
                        mov              eax, dword ptr [rbp + 128]
                        cmp              eax, 6
                                                                                        jne   .Lx46_0
.Lx46_1:
                        mov              rax, qword ptr [rbp + 144]
                        mov              qword ptr [rbp + 112], rax
                        mov              rax, qword ptr [rbp + 152]
                        mov              qword ptr [rbp + 120], rax
                                                                                        jmp   n20_op75_α
.Lx46_0:
                        lea              rdi, [rbp + 144]
                        lea              rsi, [rbp + 128]
                        lea              rdx, [rbp + 112]
                        mov              rcx, 118
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n20_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n19_op75_α:
                        mov              eax, dword ptr [rbp + 224]
                        cmp              eax, 7
                                                                                        je    .Lx48_1
                        cmp              eax, 6
                                                                                        jne   .Lx48_0
                        mov              eax, dword ptr [rbp + 208]
                        cmp              eax, 6
                                                                                        jne   .Lx48_0
.Lx48_1:
                        mov              rax, qword ptr [rbp + 224]
                        mov              qword ptr [rbp + 192], rax
                        mov              rax, qword ptr [rbp + 232]
                        mov              qword ptr [rbp + 200], rax
                                                                                        jmp   n21_op75_α
.Lx48_0:
                        lea              rdi, [rbp + 224]
                        lea              rsi, [rbp + 208]
                        lea              rdx, [rbp + 192]
                        mov              rcx, 109
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n21_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n20_op75_α:
                        mov              eax, dword ptr [rbp + 128]
                        cmp              eax, 7
                                                                                        je    .Lx50_1
                        cmp              eax, 6
                                                                                        jne   .Lx50_0
                        mov              eax, dword ptr [rbp + 144]
                        cmp              eax, 6
                                                                                        jne   .Lx50_0
.Lx50_1:
                        mov              rax, qword ptr [rbp + 128]
                        mov              qword ptr [rbp + 96], rax
                        mov              rax, qword ptr [rbp + 136]
                        mov              qword ptr [rbp + 104], rax
                                                                                        jmp   n22_op77_α
.Lx50_0:
                        lea              rdi, [rbp + 128]
                        lea              rsi, [rbp + 144]
                        lea              rdx, [rbp + 96]
                        mov              rcx, 119
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n22_op77_α
#-----------------------------------------------------------------------------------------------------------------------
n21_op75_α:
                        mov              eax, dword ptr [rbp + 208]
                        cmp              eax, 7
                                                                                        je    .Lx52_1
                        cmp              eax, 6
                                                                                        jne   .Lx52_0
                        mov              eax, dword ptr [rbp + 224]
                        cmp              eax, 6
                                                                                        jne   .Lx52_0
.Lx52_1:
                        mov              rax, qword ptr [rbp + 208]
                        mov              qword ptr [rbp + 176], rax
                        mov              rax, qword ptr [rbp + 216]
                        mov              qword ptr [rbp + 184], rax
                                                                                        jmp   n23_op77_α
.Lx52_0:
                        lea              rdi, [rbp + 208]
                        lea              rsi, [rbp + 224]
                        lea              rdx, [rbp + 176]
                        mov              rcx, 110
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n23_op77_α
#-----------------------------------------------------------------------------------------------------------------------
n22_op77_α:
                        lea              rdi, [rbp + 112]
                        lea              rsi, [rbp + 96]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jg    main_γ
                                                                                        jmp   n6_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n23_op77_α:
                        lea              rdi, [rbp + 192]
                        lea              rsi, [rbp + 176]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        js    main_γ
                                                                                        jmp   n7_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        mov              rsp, rbp
                        mov              rbp, [rsp + 256]
                        add              rsp, 264
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              rsp, rbp
                        mov              eax, 99
                        xor              edx, edx
                        mov              rbp, [rsp + 256]
                        add              rsp, 264
                        ret
                        .section         .note.GNU-stack,"",@progbits

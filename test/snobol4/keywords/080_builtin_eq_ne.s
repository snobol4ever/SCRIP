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
#         EQ(42, 42)                                                  :S(YES)F(NO)
#-----------------------------------------------------------------------------------------------------------------------
n0_lit_integer_α:
                        mov              qword ptr [rsp + 64], 6
                        mov              rax, qword ptr [rip + .Lx23_0]
                        mov              qword ptr [rsp + 72], rax
                                                                                        jmp   n6_lit_integer_α
.Lx23_0:
                        .quad            42
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
                                                                                        jmp   n11_lit_string_α
n5_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n6_lit_integer_α:
                        mov              qword ptr [rsp + 48], 6
                        mov              rax, qword ptr [rip + .Lx29_0]
                        mov              qword ptr [rsp + 56], rax
                                                                                        jmp   n12_op75_α
.Lx29_0:
                        .quad            42
#=======================================================================================================================
# YES     OUTPUT = '42 = 42'
#-----------------------------------------------------------------------------------------------------------------------
n7_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 7
                        mov              rax, qword ptr [rip + .Lx30_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n13_assign_α
.Lx30_0:
                        .quad            .Lx30_0_s
.Lx30_0_s:
                        .string          "42 = 42"
#=======================================================================================================================
# NO      OUTPUT = 'wrong'
#-----------------------------------------------------------------------------------------------------------------------
n8_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx31_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n14_assign_α
.Lx31_0:
                        .quad            .Lx31_0_s
.Lx31_0_s:
                        .string          "wrong"
#=======================================================================================================================
#         :(NEXT)
# NEXT    NE(42, 99)                                                  :S(YES2)F(NO2)
#-----------------------------------------------------------------------------------------------------------------------
n9_lit_integer_α:
                        mov              qword ptr [rsp + 160], 6
                        mov              rax, qword ptr [rip + .Lx32_0]
                        mov              qword ptr [rsp + 168], rax
                                                                                        jmp   n15_lit_integer_α
.Lx32_0:
                        .quad            42
#=======================================================================================================================
# YES2    OUTPUT = '42 != 99'
#-----------------------------------------------------------------------------------------------------------------------
n10_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 8
                        mov              rax, qword ptr [rip + .Lx33_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n16_assign_α
.Lx33_0:
                        .quad            .Lx33_0_s
.Lx33_0_s:
                        .string          "42 != 99"
#=======================================================================================================================
# NO2     OUTPUT = 'wrong'
#-----------------------------------------------------------------------------------------------------------------------
n11_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx34_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n17_assign_α
.Lx34_0:
                        .quad            .Lx34_0_s
.Lx34_0_s:
                        .string          "wrong"
#-----------------------------------------------------------------------------------------------------------------------
n12_op75_α:
                        mov              eax, dword ptr [rsp + 64]
                        cmp              eax, 7
                                                                                        je    .Lx36_1
                        cmp              eax, 6
                                                                                        jne   .Lx36_0
                        mov              eax, dword ptr [rsp + 48]
                        cmp              eax, 6
                                                                                        jne   .Lx36_0
.Lx36_1:
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                                                                                        jmp   n18_op75_α
.Lx36_0:
                        lea              rdi, [rsp + 64]
                        lea              rsi, [rsp + 48]
                        lea              rdx, [rsp + 32]
                        mov              rcx, 101
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n18_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n13_assign_α:
                        mov              rsi, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx37_0]
                        call             NV_SET_fn@PLT
                        add              rsp, 16
                                                                                        jmp   n9_lit_integer_α
.Lx37_0:
                        .quad            .Lx37_0_s
.Lx37_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n14_assign_α:
                        mov              rsi, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx38_0]
                        call             NV_SET_fn@PLT
                        add              rsp, 16
                                                                                        jmp   n9_lit_integer_α
.Lx38_0:
                        .quad            .Lx38_0_s
.Lx38_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n15_lit_integer_α:
                        mov              qword ptr [rsp + 144], 6
                        mov              rax, qword ptr [rip + .Lx39_0]
                        mov              qword ptr [rsp + 152], rax
                                                                                        jmp   n19_op75_α
.Lx39_0:
                        .quad            99
#-----------------------------------------------------------------------------------------------------------------------
n16_assign_α:
                        mov              rsi, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx40_0]
                        call             NV_SET_fn@PLT
                        add              rsp, 16
                                                                                        jmp   main_γ
.Lx40_0:
                        .quad            .Lx40_0_s
.Lx40_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n17_assign_α:
                        mov              rsi, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx41_0]
                        call             NV_SET_fn@PLT
                        add              rsp, 16
                                                                                        jmp   main_γ
.Lx41_0:
                        .quad            .Lx41_0_s
.Lx41_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n18_op75_α:
                        mov              eax, dword ptr [rsp + 48]
                        cmp              eax, 7
                                                                                        je    .Lx43_1
                        cmp              eax, 6
                                                                                        jne   .Lx43_0
                        mov              eax, dword ptr [rsp + 64]
                        cmp              eax, 6
                                                                                        jne   .Lx43_0
.Lx43_1:
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 24], rax
                                                                                        jmp   n20_op77_α
.Lx43_0:
                        lea              rdi, [rsp + 48]
                        lea              rsi, [rsp + 64]
                        lea              rdx, [rsp + 16]
                        mov              rcx, 102
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n20_op77_α
#-----------------------------------------------------------------------------------------------------------------------
n19_op75_α:
                        mov              eax, dword ptr [rsp + 160]
                        cmp              eax, 7
                                                                                        je    .Lx45_1
                        cmp              eax, 6
                                                                                        jne   .Lx45_0
                        mov              eax, dword ptr [rsp + 144]
                        cmp              eax, 6
                                                                                        jne   .Lx45_0
.Lx45_1:
                        mov              rax, qword ptr [rsp + 160]
                        mov              qword ptr [rsp + 128], rax
                        mov              rax, qword ptr [rsp + 168]
                        mov              qword ptr [rsp + 136], rax
                                                                                        jmp   n21_op75_α
.Lx45_0:
                        lea              rdi, [rsp + 160]
                        lea              rsi, [rsp + 144]
                        lea              rdx, [rsp + 128]
                        mov              rcx, 149
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n21_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n20_op77_α:
                        lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jne   n8_lit_string_α
                                                                                        jmp   n7_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n21_op75_α:
                        mov              eax, dword ptr [rsp + 144]
                        cmp              eax, 7
                                                                                        je    .Lx49_1
                        cmp              eax, 6
                                                                                        jne   .Lx49_0
                        mov              eax, dword ptr [rsp + 160]
                        cmp              eax, 6
                                                                                        jne   .Lx49_0
.Lx49_1:
                        mov              rax, qword ptr [rsp + 144]
                        mov              qword ptr [rsp + 112], rax
                        mov              rax, qword ptr [rsp + 152]
                        mov              qword ptr [rsp + 120], rax
                                                                                        jmp   n22_op77_α
.Lx49_0:
                        lea              rdi, [rsp + 144]
                        lea              rsi, [rsp + 160]
                        lea              rdx, [rsp + 112]
                        mov              rcx, 150
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n22_op77_α
#-----------------------------------------------------------------------------------------------------------------------
n22_op77_α:
                        lea              rdi, [rsp + 128]
                        lea              rsi, [rsp + 112]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        je    n11_lit_string_α
                                                                                        jmp   n10_lit_string_α
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

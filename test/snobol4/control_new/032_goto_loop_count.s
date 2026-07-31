                        .intel_syntax    noprefix
                        .text
                        .section         .rodata
.Lgvan0:                .string          "N"
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
                        sub              rsp, 168
                        mov              rdi, rsp
                        mov              ecx, 168
                        xor              eax, eax
                        rep stosb
main_α_body:
#=======================================================================================================================
#         N = 0
#-----------------------------------------------------------------------------------------------------------------------
n0_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx15_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n3_assign_α
.Lx15_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n1_goto_α:
                                                                                        jmp   n4_var_α
n1_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n2_goto_α:
                                                                                        jmp   n5_var_α
n2_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n3_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   n4_var_α
#=======================================================================================================================
# LOOP    N = N + 1
#         :(LOOP)
#-----------------------------------------------------------------------------------------------------------------------
n4_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n6_lit_integer_α
#=======================================================================================================================
# DONE    OUTPUT = N
#-----------------------------------------------------------------------------------------------------------------------
n5_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n8_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n6_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx21_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n9_binop_α
.Lx21_0:
                        .quad            1
#=======================================================================================================================
#         GT(N, 5)                                                    :S(DONE)
#-----------------------------------------------------------------------------------------------------------------------
n7_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 128], rax
                        mov              qword ptr [rsp + 136], rdx
                                                                                        jmp   n10_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n8_assign_α:
                        mov              rsi, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx23_0]
                        call             NV_SET_fn@PLT
                        add              rsp, 16
                                                                                        jmp   main_γ
.Lx23_0:
                        .quad            .Lx23_0_s
.Lx23_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n9_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx24_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n7_var_α
.Lx24_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n11_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n10_lit_integer_α:
                        mov              qword ptr [rsp + 112], 6
                        mov              rax, qword ptr [rip + .Lx25_0]
                        mov              qword ptr [rsp + 120], rax
                                                                                        jmp   n12_op75_α
.Lx25_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n11_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 48
                                                                                        jmp   n7_var_α
#-----------------------------------------------------------------------------------------------------------------------
n12_op75_α:
                        mov              eax, dword ptr [rsp + 128]
                        cmp              eax, 7
                                                                                        je    .Lx28_1
                        cmp              eax, 6
                                                                                        jne   .Lx28_0
                        mov              eax, dword ptr [rsp + 112]
                        cmp              eax, 6
                                                                                        jne   .Lx28_0
.Lx28_1:
                        mov              rax, qword ptr [rsp + 128]
                        mov              qword ptr [rsp + 96], rax
                        mov              rax, qword ptr [rsp + 136]
                        mov              qword ptr [rsp + 104], rax
                                                                                        jmp   n13_op75_α
.Lx28_0:
                        lea              rdi, [rsp + 128]
                        lea              rsi, [rsp + 112]
                        lea              rdx, [rsp + 96]
                        mov              rcx, 111
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n13_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n13_op75_α:
                        mov              eax, dword ptr [rsp + 112]
                        cmp              eax, 7
                                                                                        je    .Lx30_1
                        cmp              eax, 6
                                                                                        jne   .Lx30_0
                        mov              eax, dword ptr [rsp + 128]
                        cmp              eax, 6
                                                                                        jne   .Lx30_0
.Lx30_1:
                        mov              rax, qword ptr [rsp + 112]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [rsp + 120]
                        mov              qword ptr [rsp + 88], rax
                                                                                        jmp   n14_op77_α
.Lx30_0:
                        lea              rdi, [rsp + 112]
                        lea              rsi, [rsp + 128]
                        lea              rdx, [rsp + 80]
                        mov              rcx, 112
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n14_op77_α
#-----------------------------------------------------------------------------------------------------------------------
n14_op77_α:
                        lea              rdi, [rsp + 96]
                        lea              rsi, [rsp + 80]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jle   n4_var_α
                                                                                        jmp   n5_var_α
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        add              rsp, 168
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              eax, 99
                        xor              edx, edx
                        add              rsp, 168
                        ret
                        .section         .note.GNU-stack,"",@progbits

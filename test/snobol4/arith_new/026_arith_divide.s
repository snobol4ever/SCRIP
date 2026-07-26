                        .intel_syntax    noprefix
                        .text
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        mov              r12, qword ptr [1879048192]
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
                        sub              rsp, 72
                        mov              rdi, rsp
                        mov              ecx, 72
                        xor              eax, eax
                        rep stosb
                        mov              [rsp + 64], rbp
                        mov              rbp, rsp
main_α_body:
#=======================================================================================================================
#         OUTPUT = 10 / 4
#-----------------------------------------------------------------------------------------------------------------------
n0_lit_integer_α:
                        mov              qword ptr [rbp + 32], 6
                        mov              rax, qword ptr [rip + .Lx4_0]
                        mov              qword ptr [rbp + 40], rax
                                                                                        jmp   n1_lit_integer_α
.Lx4_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n1_lit_integer_α:
                        mov              qword ptr [rbp + 48], 6
                        mov              rax, qword ptr [rip + .Lx5_0]
                        mov              qword ptr [rbp + 56], rax
                                                                                        jmp   n2_binop_α
.Lx5_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n2_binop_α:
.Lx6_1:
                        mov              rax, 10
                        mov              rcx, 4
                        cqo
                        idiv             rcx
                        mov              qword ptr [rbp + 16], 6
                        mov              qword ptr [rbp + 24], rax
                                                                                        jmp   n3_assign_α
.Lx6_0:
                        mov              rdi, qword ptr [rbp + 32]
                        mov              rsi, qword ptr [rbp + 40]
                        mov              rdx, qword ptr [rbp + 48]
                        mov              rcx, qword ptr [rbp + 56]
                        mov              r8d, 3
                        lea              r9, [rbp + 16]
                        call             rt_binop_overload@PLT
                        test             eax, eax
                                                                                        jne   n3_assign_α
.Lx6_2:
                        mov              rdi, qword ptr [rbp + 32]
                        mov              rsi, qword ptr [rbp + 40]
                        mov              rdx, qword ptr [rbp + 48]
                        mov              rcx, qword ptr [rbp + 56]
                        mov              r8d, 3
                        call             rt_num_arith@PLT
                        cmp              eax, 99
                                                                                        je    main_γ
                        mov              qword ptr [rbp + 16], rax
                        mov              qword ptr [rbp + 24], rdx
                                                                                        jmp   n3_assign_α
n2_binop_β:
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n3_assign_α:
                        mov              rsi, qword ptr [rbp + 16]
                        mov              rdx, qword ptr [rbp + 24]
                        mov              rdi, qword ptr [rip + .Lx7_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 0], rax
                        mov              qword ptr [rbp + 8], rdx
                                                                                        jmp   main_γ
.Lx7_0:
                        .quad            .Lx7_0_s
.Lx7_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        mov              rsp, rbp
                        mov              rbp, [rsp + 64]
                        add              rsp, 72
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              rsp, rbp
                        mov              eax, 99
                        xor              edx, edx
                        mov              rbp, [rsp + 64]
                        add              rsp, 72
                        ret
                        .section         .note.GNU-stack,"",@progbits

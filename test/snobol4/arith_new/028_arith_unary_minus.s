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
                        sub              rsp, 56
                        mov              rdi, rsp
                        mov              ecx, 56
                        xor              eax, eax
                        rep stosb
main_α_body:
#=======================================================================================================================
#         OUTPUT = -5
#-----------------------------------------------------------------------------------------------------------------------
n0_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx3_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n1_unop_α
.Lx3_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n1_unop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]
                        mov              rsi, qword ptr [rsp + 24]
                        call             rt_num_neg@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n2_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n2_assign_α:
                        mov              rsi, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx5_0]
                        call             NV_SET_fn@PLT
                        add              rsp, 32
                                                                                        jmp   main_γ
.Lx5_0:
                        .quad            .Lx5_0_s
.Lx5_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        add              rsp, 56
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              eax, 99
                        xor              edx, edx
                        add              rsp, 56
                        ret
                        .section         .note.GNU-stack,"",@progbits

                        .intel_syntax    noprefix
                        .text
                        .section         .rodata
.Lgvan0:                .string          "X"
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
                        sub              rsp, 8
main_α_body:
n0_var_st:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 8
#=======================================================================================================================
#         OUTPUT = X
#-----------------------------------------------------------------------------------------------------------------------
n0_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n1_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n1_assign_α:
                        mov              rsi, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx3_0]
                        call             NV_SET_fn@PLT
                        add              rsp, 16
                                                                                        jmp   main_stγ
.Lx3_0:
                        .quad            .Lx3_0_s
.Lx3_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_stγ:
                        mov              rsp, rbp
                        pop              rbp
                                                                                        jmp   main_γ
main_stω:
                        mov              rsp, rbp
                        pop              rbp
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        add              rsp, 8
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              eax, 99
                        xor              edx, edx
                        add              rsp, 8
                        ret
                        .section         .note.GNU-stack,"",@progbits

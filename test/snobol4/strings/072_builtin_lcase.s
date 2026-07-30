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
                        sub              rsp, 40
                        mov              rdi, rsp
                        mov              ecx, 40
                        xor              eax, eax
                        rep stosb
main_α_body:
#=======================================================================================================================
#         OUTPUT = &LCASE
#-----------------------------------------------------------------------------------------------------------------------
n0_keyword_snobol4_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx2_0]
                        call             rt_keyword_read_snobol4@PLT
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rdx
                        add              rsp, 16
                                                                                        jmp   n1_assign_α
.Lx2_0:
                        .quad            .Lx2_0_s
.Lx2_0_s:
                        .string          "LCASE"
#-----------------------------------------------------------------------------------------------------------------------
n1_assign_α:
                        mov              rsi, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rsp + 24]
                        mov              rdi, qword ptr [rip + .Lx3_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   main_γ
.Lx3_0:
                        .quad            .Lx3_0_s
.Lx3_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        add              rsp, 40
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              eax, 99
                        xor              edx, edx
                        add              rsp, 40
                        ret
                        .section         .note.GNU-stack,"",@progbits

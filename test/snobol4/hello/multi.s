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
                        sub              rsp, 65544
                        mov              rdi, rsp
                        mov              ecx, 65544
                        xor              eax, eax
                        rep stosb
                        mov              qword ptr [rsp + 248], rsp
                        mov              r12, qword ptr [1879048192]
                        mov              [rsp + 65536], rbp
                        mov              rbp, rsp
main_α_body:
#=======================================================================================================================
#         OUTPUT = 'LINE ONE'
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n0_α:
                        mov              qword ptr [rbp + 112], 1
                        mov              rax, qword ptr [rip + .Lx1_0]
                        mov              qword ptr [rbp + 120], rax
                                                                                        jmp   xchain0_n1_α
.Lx1_0:
                        .quad            .Lx1_0_s
.Lx1_0_s:
                        .string          "LINE ONE"
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN global
xchain0_n1_α:
                        mov              rsi, qword ptr [rbp + 112]
                        mov              rdx, qword ptr [rbp + 120]
                        mov              rdi, qword ptr [rip + .Lx2_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 96], rax
                        mov              qword ptr [rbp + 104], rdx
                                                                                        jmp   xchain0_n2_α
.Lx2_0:
                        .quad            .Lx2_0_s
.Lx2_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#         OUTPUT = 'LINE TWO'
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n2_α:
                        mov              qword ptr [rbp + 160], 1
                        mov              rax, qword ptr [rip + .Lx3_0]
                        mov              qword ptr [rbp + 168], rax
                                                                                        jmp   xchain0_n3_α
.Lx3_0:
                        .quad            .Lx3_0_s
.Lx3_0_s:
                        .string          "LINE TWO"
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN global
xchain0_n3_α:
                        mov              rsi, qword ptr [rbp + 160]
                        mov              rdx, qword ptr [rbp + 168]
                        mov              rdi, qword ptr [rip + .Lx4_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 144], rax
                        mov              qword ptr [rbp + 152], rdx
                                                                                        jmp   xchain0_n4_α
.Lx4_0:
                        .quad            .Lx4_0_s
.Lx4_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#         OUTPUT = 'LINE THREE'
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n4_α:
                        mov              qword ptr [rbp + 208], 1
                        mov              rax, qword ptr [rip + .Lx5_0]
                        mov              qword ptr [rbp + 216], rax
                                                                                        jmp   xchain0_n5_α
.Lx5_0:
                        .quad            .Lx5_0_s
.Lx5_0_s:
                        .string          "LINE THREE"
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN global
xchain0_n5_α:
                        mov              rsi, qword ptr [rbp + 208]
                        mov              rdx, qword ptr [rbp + 216]
                        mov              rdi, qword ptr [rip + .Lx6_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 192], rax
                        mov              qword ptr [rbp + 200], rdx
                                                                                        jmp   main_γ
.Lx6_0:
                        .quad            .Lx6_0_s
.Lx6_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        mov              rsp, qword ptr [rbp + 248]
                        mov              rbp, [rsp + 65536]
                        add              rsp, 65544
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              rsp, qword ptr [rbp + 248]
                        mov              dword ptr [rsp+0], 99
                        mov              dword ptr [rsp+4], 0
                        mov              qword ptr [rsp+8], 0
                        mov              eax, 99
                        xor              edx, edx
                        mov              rbp, [rsp + 65536]
                        add              rsp, 65544
                        ret
                        .section         .note.GNU-stack,"",@progbits

                        .intel_syntax    noprefix
                        .text
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        mov              r12, qword ptr [0x70000000]
                        xor              esi, esi
                                                                                        jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
#=======================================================================================================================
#         OUTPUT = 'a' 'b' 'c' 'd'
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:
                                                                                        jmp   n1_lit_string_α
n0_statement_begin_β:
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n1_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx12_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n2_lit_string_α
.Lx12_0:
                        .quad            .Lx12_0_s
.Lx12_0_s:
                        .string          "a"
#-----------------------------------------------------------------------------------------------------------------------
n2_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx13_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n3_binop_α
n2_lit_string_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n0_statement_begin_β
.Lx13_0:
                        .quad            .Lx13_0_s
.Lx13_0_s:
                        .string          "b"
#-----------------------------------------------------------------------------------------------------------------------
n3_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # lit_string
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # b
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n4_lit_string_α
n3_binop_β:
                        add              rsp, 16
                                                                                        jmp   n2_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n4_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx15_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n5_binop_α
n4_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n3_binop_β
.Lx15_0:
                        .quad            .Lx15_0_s
.Lx15_0_s:
                        .string          "c"
#-----------------------------------------------------------------------------------------------------------------------
n5_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # binop
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n6_lit_string_α
n5_binop_β:
                        add              rsp, 16
                                                                                        jmp   n4_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n6_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx17_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n7_binop_α
n6_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n5_binop_β
.Lx17_0:
                        .quad            .Lx17_0_s
.Lx17_0_s:
                        .string          "d"
#-----------------------------------------------------------------------------------------------------------------------
n7_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # binop
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n8_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n8_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx19_0]                 # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n9_statement_end_α
.Lx19_0:
                        .quad            .Lx19_0_s
.Lx19_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n9_statement_end_α:
                        add              rsp, 112
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              edi, 1
                        call             exit@PLT
                        .section         .note.GNU-stack,"",@progbits

                        .intel_syntax    noprefix
                        .text
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        mov              r12, qword ptr [0x70000000]
                        call             rtcc_load_all@PLT
                        xor              esi, esi
                                                                              jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
#=======================================================================================================================
#         OUTPUT = 2 + 3 * 4
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:                                                         jmp   n1_lit_integer_α
n0_statement_begin_β:                                                         jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n1_lit_integer_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx10_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n2_lit_integer_α
.Lx10_0:                .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n2_lit_integer_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx11_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n3_lit_integer_α
n2_lit_integer_β:       add              rsp, 16
                        add              rsp, 16;                             jmp   n0_statement_begin_β
.Lx11_0:                .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_integer_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx12_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n4_binop_α
n3_lit_integer_β:       add              rsp, 16;                             jmp   n2_lit_integer_β
.Lx12_0:                .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n4_binop_α:             sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              edx, 3;                              jne   .Lx13_2
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        imul             rax, rdx
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              qword ptr [rsp + 8], rax;            jmp   .Lx13_7
.Lx13_2:                and              edx, 1;                              jz    .Lx13_0
                        mov              rsi, qword ptr [rsp + 40]            # lit_integer
                        mov              rdi, qword ptr [rsp + 24]
                        cmp              eax, 5;                              je    .Lx13_3
                        cvtsi2sd         xmm0, rsi;                           jmp   .Lx13_4
.Lx13_3:                movq             xmm0, rsi
.Lx13_4:                cmp              ecx, 5;                              je    .Lx13_5
                        cvtsi2sd         xmm1, rdi;                           jmp   .Lx13_6
.Lx13_5:                movq             xmm1, rdi
.Lx13_6:                mulsd            xmm0, xmm1
                        movq             rax, xmm0
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              qword ptr [rsp + 8], rax
.Lx13_7:                                                                      jmp   n5_binop_α
.Lx13_0:                mov              rdi, qword ptr [rsp + 32]            # lit_integer
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_mul@PLT
                        cmp              eax, 104;                            jne   .Lx13_240
                        add              rsp, 16;                             jmp   n3_lit_integer_β
.Lx13_240:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n5_binop_α
n4_binop_β:             add              rsp, 16;                             jmp   n3_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n5_binop_α:             sub              rsp, 16
                        mov              eax, dword ptr [rsp + 64]            # lit_integer
                        mov              ecx, dword ptr [rsp + 16]            # binop
                        mov              edx, eax
                        and              edx, ecx
                        cmp              edx, 3;                              jne   .Lx14_2
                        mov              rax, qword ptr [rsp + 72]            # lit_integer
                        mov              rdx, qword ptr [rsp + 24]            # binop
                        add              rax, rdx
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              qword ptr [rsp + 8], rax;            jmp   .Lx14_7
.Lx14_2:                and              edx, 1;                              jz    .Lx14_0
                        mov              rsi, qword ptr [rsp + 72]            # lit_integer
                        mov              rdi, qword ptr [rsp + 24]            # binop
                        cmp              eax, 5;                              je    .Lx14_3
                        cvtsi2sd         xmm0, rsi;                           jmp   .Lx14_4
.Lx14_3:                movq             xmm0, rsi
.Lx14_4:                cmp              ecx, 5;                              je    .Lx14_5
                        cvtsi2sd         xmm1, rdi;                           jmp   .Lx14_6
.Lx14_5:                movq             xmm1, rdi
.Lx14_6:                addsd            xmm0, xmm1
                        movq             rax, xmm0
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              qword ptr [rsp + 8], rax
.Lx14_7:                                                                      jmp   n6_assign_α
.Lx14_0:                mov              rdi, qword ptr [rsp + 64]            # lit_integer
                        mov              rsi, qword ptr [rsp + 72]
                        mov              rdx, qword ptr [rsp + 16]            # binop
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_add@PLT
                        cmp              eax, 104;                            jne   .Lx14_240
                        add              rsp, 16;                             jmp   n4_binop_β
.Lx14_240:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n6_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n6_assign_α:            mov              rsi, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx15_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n7_statement_end_α
.Lx15_0:                .quad            .Lx15_0_s
.Lx15_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n7_statement_end_α:     add              rsp, 80;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                              jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        xor              edi, edi
                        call             exit@PLT
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              edi, 1
                        call             exit@PLT
                        .section         .note.GNU-stack,"",@progbits

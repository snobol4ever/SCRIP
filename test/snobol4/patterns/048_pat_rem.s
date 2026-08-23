                        .intel_syntax    noprefix
                        .text
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        mov              edi, 2
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 2
                        call             gva_register@PLT
                        mov              r12, qword ptr [0x70000000]
                        call             rtcc_load_all@PLT
                        xor              esi, esi
                                                                              jmp   main_α
                        .section         .rodata
.Lgvan0:                .string          "X"
.Lgvan1:                .string          "V"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .quad            .Lgvan1
                        .section         .text
                        .intel_syntax    noprefix
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
#=======================================================================================================================
#         X = 'hello world'
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:   mov              r11, 1
                        mov              r10, 1;                              jmp   n1_lit_string_α
n0_statement_begin_β:   mov              r11, 1;                              jmp   n4_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n1_lit_string_α:        sub              rsp, 16
                        mov              r11, 2
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 11
                        mov              rax, qword ptr [rip + .Lx26_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n2_assign_α
.Lx26_0:                .quad            .Lx26_0_s
.Lx26_0_s:              .string          "hello world"
#-----------------------------------------------------------------------------------------------------------------------
n2_assign_α:            mov              r11, 3
                        mov              rax, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax              # X
                        mov              qword ptr [r9 + 8], rdx;             jmp   n3_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n3_statement_end_α:     mov              r11, 4
                        mov              r10, 1
                        add              rsp, 16;                             jmp   n4_statement_begin_α
#=======================================================================================================================
#         X BREAK(' ') LEN(1) REM . V                                :S(YES)
#-----------------------------------------------------------------------------------------------------------------------
n4_statement_begin_α:   mov              r11, 5
                        mov              r10, 2;                              jmp   n5_var_α
n4_statement_begin_β:   mov              r11, 5;                              jmp   n14_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n5_var_α:               sub              rsp, 16
                        mov              r11, 6
                        mov              rax, qword ptr [r9 + 0]              # X
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n6_match_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n6_match_begin_α:       mov              r11, 7
                        mov              rdi, qword ptr [rsp + 0]             # var
                        mov              rsi, qword ptr [rsp + 8]
                        push             rbp
                        mov              rbp, rsp
                        push             r12                                  # cas_mark
                        push             r13                                  # outer_Σ
                        push             r14                                  # outer_δ
                        push             r15                                  # outer_Δ
                        sub              rsp, 24
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              dword ptr [rbp + -40], 0             # start_δ
.Lx34_0:                mov              r14d, dword ptr [rbp + -40]
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # match_beta_cont
                        mov              rax, qword ptr [rcx + 248]
                        mov              qword ptr [rbp + -48], rax
                        lea              rax, [rip + .Lx34_13]
                        mov              qword ptr [rcx + 248], rax;          jmp   n7_match_break_α
n6_match_begin_β:       mov              r11, 7
.Lx34_13:               lea              rsp, [rbp + -56]                     # retry_whack
                        add              dword ptr [rbp + -40], 1             # start_δ
                        mov              eax, dword ptr [rbp + -40]
                        cmp              eax, r15d;                           jg    .Lx34_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                              jne   .Lx34_1
                                                                              jmp   .Lx34_0
.Lx34_1:
n6_match_begin_af:      mov              r11, 7
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # mbc_restore
                        mov              rax, qword ptr [rbp + -48]
                        mov              qword ptr [rcx + 248], rax
                        mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              qword ptr [rip + rtccb+56], r10
                        call             rt_match_ctx_restore@PLT
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              rsp, rbp
                        pop              rbp
                        add              rsp, 16;                             jmp   n14_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n7_match_break_α:       sub              rsp, 16
                        mov              r11, 8
                        movsxd           rcx, r14d
.Lx36_0:                cmp              ecx, r15d;                           jl    .Lx36_240
                        add              rsp, 16;                             jmp   n6_match_begin_β
.Lx36_240:              movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32;                             je    .Lx36_1
                        add              ecx, 1;                              jmp   .Lx36_0
.Lx36_1:                mov              dword ptr [rsp + 0], r14d
                        mov              r14d, ecx;                           jmp   n8_match_len_α
n7_match_break_β:       mov              r11, 8
                        mov              r14d, dword ptr [rsp + 0]
                        add              rsp, 16;                             jmp   n6_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n8_match_len_α:         mov              r11, 9
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d;                           jg    n7_match_break_β
                        add              r14d, 1;                             jmp   n9_match_assign_save_α
n8_match_len_β:         mov              r11, 9
                        sub              r14d, 1;                             jmp   n7_match_break_β
#-----------------------------------------------------------------------------------------------------------------------
n9_match_assign_save_α: sub              rsp, 16
                        mov              r11, 10
                        mov              dword ptr [rsp + 0], r14d;           jmp   n10_match_rem_α
n9_match_assign_save_β: mov              r11, 10
                        add              rsp, 16;                             jmp   n8_match_len_β
#-----------------------------------------------------------------------------------------------------------------------
n10_match_rem_α:        sub              rsp, 16
                        mov              r11, 11
                        mov              dword ptr [rsp + 0], r14d
                        mov              r14d, r15d;                          jmp   n11_match_assign_cond_α
n10_match_rem_β:        mov              r11, 11
                        mov              r14d, dword ptr [rsp + 0]
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n8_match_len_β
#-----------------------------------------------------------------------------------------------------------------------
n11_match_assign_cond_α:
                        mov              r11, 12
                        mov              eax, dword ptr [rsp + 16]
                        lea              rcx, [rip + .S0]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                             jmp   n12_match_end_α
n11_match_assign_cond_β:
                        mov              r11, 12
                        sub              r12, 24;                             jmp   n10_match_rem_β
#-----------------------------------------------------------------------------------------------------------------------
n12_match_end_α:        mov              r11, 13
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # mbc_restore
                        mov              rax, qword ptr [rbp + -48]
                        mov              qword ptr [rcx + 248], rax
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rbp + -8]            # cas_mark
                        mov              rax, qword ptr [rbp + -32]           # outer_Δ
                        mov              rcx, qword ptr [rbp + -16]           # outer_Σ
                        push             rax
                        push             rcx
                        lea              rcx, [rsp + 0]
                        mov              rsi, r12
                        mov              rdx, r13
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_match_end_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             rax, rax;                            je    .Lx44_13
                        add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14;                                 jmp   n6_match_begin_af
.Lx44_13:               add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rsp, rbp                             # frame_whack
                        pop              rbp;                                 jmp   n13_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n13_statement_end_α:    mov              r11, 14
                        mov              r10, 2
                        add              rsp, 16;                             jmp   n20_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'fail'
#-----------------------------------------------------------------------------------------------------------------------
n14_statement_begin_α:  mov              r11, 15
                        mov              r10, 3;                              jmp   n15_lit_string_α
n14_statement_begin_β:  mov              r11, 15;                             jmp   n18_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n15_lit_string_α:       sub              rsp, 16
                        mov              r11, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx49_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n16_assign_α
.Lx49_0:                .quad            .Lx49_0_s
.Lx49_0_s:              .string          "fail"
#-----------------------------------------------------------------------------------------------------------------------
n16_assign_α:           mov              r11, 17
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx50_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n17_statement_end_α
.Lx50_0:                .quad            .Lx50_0_s
.Lx50_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n17_statement_end_α:    mov              r11, 18
                        mov              r10, 3
                        add              rsp, 16;                             jmp   n18_statement_begin_α
#=======================================================================================================================
#         :(END)
#-----------------------------------------------------------------------------------------------------------------------
n18_statement_begin_α:  mov              r11, 19
                        mov              r10, 4;                              jmp   n19_statement_end_α
n18_statement_begin_β:  mov              r11, 19;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n19_statement_end_α:    mov              r11, 20
                        mov              r10, 4;                              jmp   main_γ
#=======================================================================================================================
# YES     OUTPUT = V
#-----------------------------------------------------------------------------------------------------------------------
n20_statement_begin_α:  mov              r11, 21
                        mov              r10, 5;                              jmp   n21_var_α
n20_statement_begin_β:  mov              r11, 21;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n21_var_α:              sub              rsp, 16
                        mov              r11, 22
                        mov              rax, qword ptr [r9 + 16]             # V
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n22_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n22_assign_α:           mov              r11, 23
                        mov              rsi, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx60_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n23_statement_end_α
.Lx60_0:                .quad            .Lx60_0_s
.Lx60_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n23_statement_end_α:    mov              r11, 24
                        mov              r10, 5
                        add              rsp, 16;                             jmp   main_γ
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
                        .section         .rodata
.S0:                    .string          "V"
                        .text
                        .section         .note.GNU-stack,"",@progbits

                        .intel_syntax    noprefix
                        .text
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        mov              edi, 3
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 3
                        call             gva_register@PLT
                        mov              r12, qword ptr [0x70000000]
                        call             rtcc_load_all@PLT
                        xor              esi, esi
                                                                              jmp   main_α
                        .section         .rodata
.Lgvan0:                .string          "P"
.Lgvan1:                .string          "X"
.Lgvan2:                .string          "V"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .quad            .Lgvan1
                        .quad            .Lgvan2
                        .section         .text
                        .intel_syntax    noprefix
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
#=======================================================================================================================
#         P = ('a' | 'b' | 'c')
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:                                                         jmp   n1_statement_end_α
n0_statement_begin_β:                                                         jmp   n2_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n1_statement_end_α:                                                           jmp   n2_statement_begin_α
n1_statement_end_β:                                                           jmp   n2_statement_begin_α
#=======================================================================================================================
#         X = 'b'
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_begin_α:                                                         jmp   n3_lit_string_α
n2_statement_begin_β:                                                         jmp   n6_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_string_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx33_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n4_assign_α
n3_lit_string_β:        add              rsp, 16;                             jmp   n2_statement_begin_β
.Lx33_0:                .quad            .Lx33_0_s
.Lx33_0_s:              .string          "b"
#-----------------------------------------------------------------------------------------------------------------------
n4_assign_α:            mov              rax, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 16], rax             # X
                        mov              qword ptr [r9 + 24], rdx;            jmp   n5_statement_end_α
n4_assign_β:                                                                  jmp   n2_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n5_statement_end_α:     add              rsp, 16;                             jmp   n6_statement_begin_α
n5_statement_end_β:     add              rsp, 16;                             jmp   n6_statement_begin_α
#=======================================================================================================================
#         X P . V                                                     :S(YES)F(NO)
#-----------------------------------------------------------------------------------------------------------------------
n6_statement_begin_α:                                                         jmp   n7_var_α
n6_statement_begin_β:                                                         jmp   n23_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n7_var_α:               sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]             # X
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n8_match_begin_α
n7_var_β:               add              rsp, 16;                             jmp   n23_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n8_match_begin_α:       mov              rdi, qword ptr [rsp + 0]             # var
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
.Lx41_0:                mov              r14d, dword ptr [rbp + -40];         jmp   n9_match_assign_save_α
n8_match_begin_β:       lea              rsp, [rbp + -56]                     # retry_whack
                        add              dword ptr [rbp + -40], 1             # start_δ
                        mov              eax, dword ptr [rbp + -40]
                        cmp              eax, r15d;                           jg    .Lx41_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                              jne   .Lx41_1
                                                                              jmp   .Lx41_0
.Lx41_1:
n8_match_begin_af:      mov              r12, qword ptr [rbp + -8]            # cas_mark
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
                        add              rsp, 16;                             jmp   n23_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n9_match_assign_save_α: sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d;           jmp   n10_match_alternate_α
n9_match_assign_save_β: add              rsp, 16;                             jmp   n8_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n10_match_alternate_α:  sub              rsp, 32
                        mov              dword ptr [rsp + 0], r14d
                        lea              rax, [rip + .Lx45_21]
                        mov              qword ptr [rsp + 16], rax;           jmp   n16_match_lit_α
.Lx45_21:               lea              rax, [rip + .Lx45_22]
                        mov              qword ptr [rsp + 16], rax;           jmp   n15_match_lit_α
.Lx45_22:               lea              rax, [rip + .Lx45_19]
                        mov              qword ptr [rsp + 16], rax;           jmp   n14_match_lit_α
n10_match_alternate_s0: lea              rax, [rip + .Lx45_40]
                        mov              qword ptr [rsp + 8], rax;            jmp   n10_match_alternate_as
n10_match_alternate_s1: lea              rax, [rip + .Lx45_41]
                        mov              qword ptr [rsp + 8], rax;            jmp   n10_match_alternate_as
n10_match_alternate_s2: lea              rax, [rip + .Lx45_42]
                        mov              qword ptr [rsp + 8], rax;            jmp   n10_match_alternate_as
.Lx45_40:                                                                     jmp   n16_match_lit_β
.Lx45_41:                                                                     jmp   n15_match_lit_β
.Lx45_42:                                                                     jmp   n14_match_lit_β
n10_match_alternate_as:                                                       jmp   n11_match_assign_cond_α
n10_match_alternate_β:  mov              rax, qword ptr [rsp + 8];            jmp   rax
n10_match_alternate_af: mov              r14d, dword ptr [rsp + 0]
                        mov              rax, qword ptr [rsp + 16];           jmp   rax
.Lx45_19:               add              rsp, 32;                             jmp   n9_match_assign_save_β
#-----------------------------------------------------------------------------------------------------------------------
n11_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 0]
                        lea              rcx, [rip + .S0]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                             jmp   n12_match_end_α
n11_match_assign_cond_β:
                        sub              r12, 24;                             jmp   n10_match_alternate_β
#-----------------------------------------------------------------------------------------------------------------------
n12_match_end_α:        push             r14
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
                        add              rsp, 16
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
n13_statement_end_α:    add              rsp, 16;                             jmp   n17_statement_begin_α
n13_statement_end_β:    add              rsp, 32;                             jmp   n23_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n14_match_lit_α:        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d;                           jg    n10_match_alternate_af
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 99;                             jne   n10_match_alternate_af
                        add              r14d, 1;                             jmp   n10_match_alternate_s2
n14_match_lit_β:        sub              r14d, 1;                             jmp   n10_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n15_match_lit_α:        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d;                           jg    n10_match_alternate_af
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 98;                             jne   n10_match_alternate_af
                        add              r14d, 1;                             jmp   n10_match_alternate_s1
n15_match_lit_β:        sub              r14d, 1;                             jmp   n10_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n16_match_lit_α:        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d;                           jg    n10_match_alternate_af
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 97;                             jne   n10_match_alternate_af
                        add              r14d, 1;                             jmp   n10_match_alternate_s0
n16_match_lit_β:        sub              r14d, 1;                             jmp   n10_match_alternate_af
#=======================================================================================================================
# YES     OUTPUT = V
#-----------------------------------------------------------------------------------------------------------------------
n17_statement_begin_α:                                                        jmp   n18_var_α
n17_statement_begin_β:                                                        jmp   n21_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n18_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]             # V
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n19_assign_α
n18_var_β:              add              rsp, 16;                             jmp   n17_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n19_assign_α:           mov              rsi, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx61_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n20_statement_end_α
n19_assign_β:                                                                 jmp   n17_statement_begin_β
.Lx61_0:                .quad            .Lx61_0_s
.Lx61_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n20_statement_end_α:    add              rsp, 16;                             jmp   n21_statement_begin_α
n20_statement_end_β:    add              rsp, 16;                             jmp   n21_statement_begin_α
#=======================================================================================================================
#         :(END)
#-----------------------------------------------------------------------------------------------------------------------
n21_statement_begin_α:                                                        jmp   n22_statement_end_α
n21_statement_begin_β:                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n22_statement_end_α:                                                          jmp   main_γ
n22_statement_end_β:                                                          jmp   main_γ
#=======================================================================================================================
# NO      OUTPUT = 'no match'
#-----------------------------------------------------------------------------------------------------------------------
n23_statement_begin_α:                                                        jmp   n24_lit_string_α
n23_statement_begin_β:                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n24_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 8
                        mov              rax, qword ptr [rip + .Lx70_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n25_assign_α
n24_lit_string_β:       add              rsp, 16;                             jmp   n23_statement_begin_β
.Lx70_0:                .quad            .Lx70_0_s
.Lx70_0_s:              .string          "no match"
#-----------------------------------------------------------------------------------------------------------------------
n25_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx71_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n26_statement_end_α
n25_assign_β:                                                                 jmp   n23_statement_begin_β
.Lx71_0:                .quad            .Lx71_0_s
.Lx71_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n26_statement_end_α:    add              rsp, 16;                             jmp   main_γ
n26_statement_end_β:    add              rsp, 16;                             jmp   main_γ
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

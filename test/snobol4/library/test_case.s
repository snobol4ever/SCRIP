                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
FN__PAT$0:
PAT$0_α_body:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 72
                        mov              rcx, qword ptr [rbp + 8]
                        mov              qword ptr [rbp + -8], rcx
                        mov              rcx, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + -16], rcx
                        mov              qword ptr [rbp + -24], rdx
                        mov              qword ptr [rbp + -32], r12
#-----------------------------------------------------------------------------------------------------------------------
n0_match_defer_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 0
                        lea              rdx, [rip + .S0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_patv_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx4_16
                        mov              rax, qword ptr [rdx + 0]
.Lx4_16:                test             rax, rax;                            jz    .Lx4_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx4_5]
                        push             rcx
                        lea              rcx, [rip + .Lx4_4]
                        push             rcx;                                 jmp   rax
.Lx4_4:                                                                       jmp   n1_match_alternate_α
.Lx4_5:                 add              rsp, 16;                             jmp   PAT$0_ω
.Lx4_0:                 push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 0
                        lea              rdx, [rip + .S0]
                        mov              ecx, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_patv_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            jns   .Lx4_240
                        add              rsp, 16;                             jmp   PAT$0_ω
.Lx4_240:               mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx4_6]
                        push             rcx
                        push             rax;                                 jmp   n1_match_alternate_α
.Lx4_6:                 add              rsp, 8
                        pop              rax
                        mov              r14d, eax
                        add              rsp, 16;                             jmp   PAT$0_ω
n0_match_defer_β:       cmp              qword ptr [rsp + 0], 0;              jne   .Lx4_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx4_12
                                                                              jmp   rax
.Lx4_12:                                                                      jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n1_match_alternate_α:   mov              dword ptr [rbp + -72], r14d
                        lea              rax, [rip + .Lx6_21]
                        mov              qword ptr [rbp + -56], rax;          jmp   n3_match_defer_α
.Lx6_21:                lea              rax, [rip + .Lx6_19]
                        mov              qword ptr [rbp + -56], rax;          jmp   n2_match_defer_α
n1_match_alternate_s0:  lea              rax, [rip + .Lx6_40]
                        mov              qword ptr [rbp + -64], rax;          jmp   n1_match_alternate_as
n1_match_alternate_s1:  lea              rax, [rip + .Lx6_41]
                        mov              qword ptr [rbp + -64], rax;          jmp   n1_match_alternate_as
.Lx6_40:                                                                      jmp   n3_match_defer_β
.Lx6_41:                                                                      jmp   n2_match_defer_β
n1_match_alternate_as:                                                        jmp   PAT$0_γ
n1_match_alternate_β:   mov              rax, qword ptr [rbp + -64];          jmp   rax
n1_match_alternate_af:  mov              r14d, dword ptr [rbp + -72]
                        mov              rax, qword ptr [rbp + -56];          jmp   rax
.Lx6_19:                                                                      jmp   n0_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n2_match_defer_α:       mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 2
                        lea              rdx, [rip + .S1]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_patv_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx7_16
                        mov              rax, qword ptr [rdx + 0]
.Lx7_16:                test             rax, rax;                            jz    .Lx7_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx7_5]
                        push             rcx
                        lea              rcx, [rip + .Lx7_4]
                        push             rcx;                                 jmp   rax
.Lx7_4:                                                                       jmp   n1_match_alternate_s1
.Lx7_5:                                                                       jmp   n1_match_alternate_af
.Lx7_0:                 push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 2
                        lea              rdx, [rip + .S1]
                        mov              ecx, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_patv_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n1_match_alternate_af
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx7_6]
                        push             rcx
                        push             rax;                                 jmp   n1_match_alternate_s1
.Lx7_6:                 add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n1_match_alternate_af
n2_match_defer_β:       cmp              qword ptr [rsp + 0], 0;              jne   .Lx7_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx7_12
                                                                              jmp   rax
.Lx7_12:                                                                      jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n3_match_defer_α:       mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 1
                        lea              rdx, [rip + .S2]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_patv_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx8_16
                        mov              rax, qword ptr [rdx + 0]
.Lx8_16:                test             rax, rax;                            jz    .Lx8_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx8_5]
                        push             rcx
                        lea              rcx, [rip + .Lx8_4]
                        push             rcx;                                 jmp   rax
.Lx8_4:                                                                       jmp   n1_match_alternate_s0
.Lx8_5:                                                                       jmp   n1_match_alternate_af
.Lx8_0:                 push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 1
                        lea              rdx, [rip + .S2]
                        mov              ecx, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_patv_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n1_match_alternate_af
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx8_6]
                        push             rcx
                        push             rax;                                 jmp   n1_match_alternate_s0
.Lx8_6:                 add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n1_match_alternate_af
n3_match_defer_β:       cmp              qword ptr [rsp + 0], 0;              jne   .Lx8_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx8_12
                                                                              jmp   rax
.Lx8_12:                                                                      jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
PAT$0_res:
                        mov              rbp, qword ptr [rsp + 24]
                        add              rsp, 32
#-----------------------------------------------------------------------------------------------------------------------
PAT$0_β:
                                                                              jmp   n1_match_alternate_β
#-----------------------------------------------------------------------------------------------------------------------
PAT$0_γ:
                        mov              rcx, qword ptr [rbp + -16]
                        push             rbp
                        push             rcx
                        mov              rcx, qword ptr [rbp + -8]
                        push             rcx
                        lea              rax, [rip + PAT$0_res]
                        push             rax
                        mov              rbp, qword ptr [rbp + 0];            jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
PAT$0_ω:
                        mov              r12, qword ptr [rbp + -32]
                        mov              rsp, rbp
                        pop              rbp
                        add              rsp, 8
                        ret
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             module_init
                        mov              edi, 10
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 10
                        call             gva_register@PLT
                        mov              r12, qword ptr [0x70000000]
                        call             rtcc_load_all@PLT
                        xor              esi, esi
                                                                              jmp   main_α
                        .section         .rodata
.Lgvan0:                .string          "lwr"
.Lgvan1:                .string          "upr"
.Lgvan2:                .string          "cap"
.Lgvan3:                .string          "icase"
.Lgvan4:                .string          "str"
.Lgvan5:                .string          "letter"
.Lgvan6:                .string          "ch"
.Lgvan7:                .string          "PAT$0$V0"
.Lgvan8:                .string          "PAT$0$V1"
.Lgvan9:                .string          "PAT$0$V2"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .quad            .Lgvan1
                        .quad            .Lgvan2
                        .quad            .Lgvan3
                        .quad            .Lgvan4
                        .quad            .Lgvan5
                        .quad            .Lgvan6
                        .quad            .Lgvan7
                        .quad            .Lgvan8
                        .quad            .Lgvan9
                        .section         .text
                        .intel_syntax    noprefix
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
                        sub              rsp, 0
#-----------------------------------------------------------------------------------------------------------------------
n9_statement_begin_α:                                                         jmp   n10_statement_end_α
n9_statement_begin_β:                                                         jmp   n11_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n10_statement_end_α:                                                          jmp   n11_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'no match ok'          :(END)
#-----------------------------------------------------------------------------------------------------------------------
n11_statement_begin_α:                                                        jmp   n12_define_α
n11_statement_begin_β:                                                        jmp   n21_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n12_define_α:           mov              rdi, qword ptr [rip + .Lx204_0]
                        mov              rsi, qword ptr [rip + .Lx204_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + n14_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdi, qword ptr [rip + .Lx204_0]
                        lea              rsi, [rip + lwr_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n13_statement_end_α
n12_define_β:                                                                 jmp   n11_statement_begin_β
.Lx204_0:               .quad            .Lx204_0_s
.Lx204_0_s:             .string          "lwr"
.Lx204_1:               .quad            .Lx204_1_s
.Lx204_1_s:             .string          "lwr"
                                                                              jmp   .Lx205_245
#-----------------------------------------------------------------------------------------------------------------------
lwr_α:                  sub              rsp, 48
                        mov              qword ptr [rsp + 16], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        cmp              rdx, 0;                              jbe   .Lx205_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 0]              # lwr
                        mov              qword ptr [r9 + 0], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        mov              qword ptr [r9 + 8], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx205_41
.Lx205_10:              mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 40], rax
                        mov              qword ptr [r9 + 0], 0
                        mov              qword ptr [r9 + 8], 0
.Lx205_41:              lea              r10, [rip + lwr_γ]
                        lea              r11, [rip + lwr_ω]
                        push             r11
                        push             r10
                        lea              rax, [rip + n14_statement_begin_α];  jmp   rax
lwr_γ:                  mov              rdi, qword ptr [r9 + 0]
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rcx, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        cmp              rdx, 0;                              jbe   .Lx205_80
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 8], rax;             jmp   .Lx205_110
.Lx205_80:              mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 8], rax
.Lx205_110:             mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 48
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
lwr_ω:                  mov              rcx, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        cmp              rdx, 0;                              jbe   .Lx205_150
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 8], rax;             jmp   .Lx205_180
.Lx205_150:             mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 8], rax
.Lx205_180:             mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 48
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx205_245:
#-----------------------------------------------------------------------------------------------------------------------
n13_statement_end_α:                                                          jmp   n21_statement_begin_α
#=======================================================================================================================
# lwr  <stmt 3, line 33: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n14_statement_begin_α:                                                        jmp   n15_var_α
n14_statement_begin_β:                                                        jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n15_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]              # lwr
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n16_keyword_snobol4_α
#-----------------------------------------------------------------------------------------------------------------------
n16_keyword_snobol4_α:  sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx211_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_kw_read_idx@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n17_keyword_snobol4_α
.Lx211_0:               .quad            21
#-----------------------------------------------------------------------------------------------------------------------
n17_keyword_snobol4_α:  sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx212_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_kw_read_idx@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n18_call_α
.Lx212_0:               .quad            22
#-----------------------------------------------------------------------------------------------------------------------
n18_call_α:             sub              rsp, 16
                        sub              rsp, 48
                        mov              r8, qword ptr [rsp + 96]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 24], r8
                        mov              r8, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 32], r8
                        mov              r8, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], r8
                        .section         .rodata
.Lrkfnzd214:            .string          "REPLACE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd214]
                        lea              rsi, [rsp + 0]
                        mov              edx, 3
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 48
                        cmp              al, 104;                             jne   .Lx213_240
                        add              rsp, 16
                        add              rsp, 48;                             jmp   n14_statement_begin_β
.Lx213_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n19_assign_α
n18_call_β:             add              rsp, 16
                        add              rsp, 48;                             jmp   n14_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n19_assign_α:           mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax              # lwr
                        mov              qword ptr [r9 + 8], rdx;             jmp   n20_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n20_statement_end_α:    add              rsp, 64;                             jmp   RETURN
#=======================================================================================================================
# lwr_end  <stmt 4, line 35: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n21_statement_begin_α:                                                        jmp   n22_statement_end_α
n21_statement_begin_β:                                                        jmp   n23_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n22_statement_end_α:                                                          jmp   n23_statement_begin_α
#=======================================================================================================================
#         <stmt 5, line 36: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n23_statement_begin_α:                                                        jmp   n24_define_α
n23_statement_begin_β:                                                        jmp   n33_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n24_define_α:           mov              rdi, qword ptr [rip + .Lx225_0]
                        mov              rsi, qword ptr [rip + .Lx225_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + n26_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdi, qword ptr [rip + .Lx225_0]
                        lea              rsi, [rip + upr_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n25_statement_end_α
n24_define_β:                                                                 jmp   n23_statement_begin_β
.Lx225_0:               .quad            .Lx225_0_s
.Lx225_0_s:             .string          "upr"
.Lx225_1:               .quad            .Lx225_1_s
.Lx225_1_s:             .string          "upr"
                                                                              jmp   .Lx226_245
#-----------------------------------------------------------------------------------------------------------------------
upr_α:                  sub              rsp, 48
                        mov              qword ptr [rsp + 16], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        cmp              rdx, 0;                              jbe   .Lx226_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 16]             # upr
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx226_41
.Lx226_10:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 40], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx226_41:              lea              r10, [rip + upr_γ]
                        lea              r11, [rip + upr_ω]
                        push             r11
                        push             r10
                        lea              rax, [rip + n26_statement_begin_α];  jmp   rax
upr_γ:                  mov              rdi, qword ptr [r9 + 16]
                        mov              rsi, qword ptr [r9 + 24]
                        mov              rcx, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        cmp              rdx, 0;                              jbe   .Lx226_80
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx226_110
.Lx226_80:              mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 24], rax
.Lx226_110:             mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 48
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
upr_ω:                  mov              rcx, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        cmp              rdx, 0;                              jbe   .Lx226_150
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx226_180
.Lx226_150:             mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 24], rax
.Lx226_180:             mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 48
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx226_245:
#-----------------------------------------------------------------------------------------------------------------------
n25_statement_end_α:                                                          jmp   n33_statement_begin_α
#=======================================================================================================================
# upr  <stmt 6, line 37: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n26_statement_begin_α:                                                        jmp   n27_var_α
n26_statement_begin_β:                                                        jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n27_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]             # upr
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n28_keyword_snobol4_α
#-----------------------------------------------------------------------------------------------------------------------
n28_keyword_snobol4_α:  sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx232_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_kw_read_idx@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n29_keyword_snobol4_α
.Lx232_0:               .quad            22
#-----------------------------------------------------------------------------------------------------------------------
n29_keyword_snobol4_α:  sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx233_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_kw_read_idx@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n30_call_α
.Lx233_0:               .quad            21
#-----------------------------------------------------------------------------------------------------------------------
n30_call_α:             sub              rsp, 16
                        sub              rsp, 48
                        mov              r8, qword ptr [rsp + 96]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 24], r8
                        mov              r8, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 32], r8
                        mov              r8, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], r8
                        .section         .rodata
.Lrkfnzd235:            .string          "REPLACE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd235]
                        lea              rsi, [rsp + 0]
                        mov              edx, 3
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 48
                        cmp              al, 104;                             jne   .Lx234_240
                        add              rsp, 16
                        add              rsp, 48;                             jmp   n26_statement_begin_β
.Lx234_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n31_assign_α
n30_call_β:             add              rsp, 16
                        add              rsp, 48;                             jmp   n26_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n31_assign_α:           mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 16], rax             # upr
                        mov              qword ptr [r9 + 24], rdx;            jmp   n32_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n32_statement_end_α:    add              rsp, 64;                             jmp   RETURN
#=======================================================================================================================
# upr_end  <stmt 7, line 39: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n33_statement_begin_α:                                                        jmp   n34_statement_end_α
n33_statement_begin_β:                                                        jmp   n35_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n34_statement_end_α:                                                          jmp   n35_statement_begin_α
#=======================================================================================================================
#         <stmt 8, line 40: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n35_statement_begin_α:                                                        jmp   n36_define_α
n35_statement_begin_β:                                                        jmp   n55_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n36_define_α:           mov              rdi, qword ptr [rip + .Lx246_0]
                        mov              rsi, qword ptr [rip + .Lx246_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + n38_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdi, qword ptr [rip + .Lx246_0]
                        lea              rsi, [rip + cap_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n37_statement_end_α
n36_define_β:                                                                 jmp   n35_statement_begin_β
.Lx246_0:               .quad            .Lx246_0_s
.Lx246_0_s:             .string          "cap"
.Lx246_1:               .quad            .Lx246_1_s
.Lx246_1_s:             .string          "cap"
                                                                              jmp   .Lx247_245
#-----------------------------------------------------------------------------------------------------------------------
cap_α:                  sub              rsp, 48
                        mov              qword ptr [rsp + 16], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        cmp              rdx, 0;                              jbe   .Lx247_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 32]             # cap
                        mov              qword ptr [r9 + 32], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 40]
                        mov              qword ptr [r9 + 40], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx247_41
.Lx247_10:              mov              rax, qword ptr [r9 + 32]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 40], rax
                        mov              qword ptr [r9 + 32], 0
                        mov              qword ptr [r9 + 40], 0
.Lx247_41:              lea              r10, [rip + cap_γ]
                        lea              r11, [rip + cap_ω]
                        push             r11
                        push             r10
                        lea              rax, [rip + n38_statement_begin_α];  jmp   rax
cap_γ:                  mov              rdi, qword ptr [r9 + 32]
                        mov              rsi, qword ptr [r9 + 40]
                        mov              rcx, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        cmp              rdx, 0;                              jbe   .Lx247_80
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 40], rax;            jmp   .Lx247_110
.Lx247_80:              mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 40], rax
.Lx247_110:             mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 48
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
cap_ω:                  mov              rcx, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        cmp              rdx, 0;                              jbe   .Lx247_150
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 40], rax;            jmp   .Lx247_180
.Lx247_150:             mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 40], rax
.Lx247_180:             mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 48
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx247_245:
#-----------------------------------------------------------------------------------------------------------------------
n37_statement_end_α:                                                          jmp   n55_statement_begin_α
#=======================================================================================================================
# cap  <stmt 9, line 41: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n38_statement_begin_α:                                                        jmp   n39_var_α
n38_statement_begin_β:                                                        jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n39_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]             # cap
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n40_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n40_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx253_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n41_lit_integer_α
n40_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n38_statement_begin_β
.Lx253_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n41_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx254_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n42_call_α
n41_lit_integer_β:      add              rsp, 16;                             jmp   n40_lit_integer_β
.Lx254_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n42_call_α:             sub              rsp, 16
                        sub              rsp, 48
                        mov              r8, qword ptr [rsp + 96]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 24], r8
                        mov              r8, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 32], r8
                        mov              r8, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], r8
                        .section         .rodata
.Lrkfnzd256:            .string          "SUBSTR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd256]
                        lea              rsi, [rsp + 0]
                        mov              edx, 3
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 48
                        cmp              al, 104;                             jne   .Lx255_240
                        add              rsp, 16;                             jmp   n41_lit_integer_β
.Lx255_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n43_keyword_snobol4_α
n42_call_β:             add              rsp, 16;                             jmp   n41_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n43_keyword_snobol4_α:  sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx257_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_kw_read_idx@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n44_keyword_snobol4_α
.Lx257_0:               .quad            22
#-----------------------------------------------------------------------------------------------------------------------
n44_keyword_snobol4_α:  sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx258_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_kw_read_idx@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n45_call_α
.Lx258_0:               .quad            21
#-----------------------------------------------------------------------------------------------------------------------
n45_call_α:             sub              rsp, 16
                        sub              rsp, 48
                        mov              r8, qword ptr [rsp + 96]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 24], r8
                        mov              r8, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 32], r8
                        mov              r8, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], r8
                        .section         .rodata
.Lrkfnzd260:            .string          "REPLACE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd260]
                        lea              rsi, [rsp + 0]
                        mov              edx, 3
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 48
                        cmp              al, 104;                             jne   .Lx259_240
                        add              rsp, 64;                             jmp   n41_lit_integer_β
.Lx259_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n46_var_α
n45_call_β:             add              rsp, 64;                             jmp   n41_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n46_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]             # cap
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n47_lit_integer_α
n46_var_β:              add              rsp, 80;                             jmp   n41_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n47_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx262_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n48_call_α
n47_lit_integer_β:      add              rsp, 16;                             jmp   n46_var_β
.Lx262_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n48_call_α:             sub              rsp, 16
                        sub              rsp, 32
                        mov              r8, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 48]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 24], r8
                        .section         .rodata
.Lrkfnzd264:            .string          "SUBSTR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd264]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx263_240
                        add              rsp, 16;                             jmp   n47_lit_integer_β
.Lx263_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n49_keyword_snobol4_α
n48_call_β:             add              rsp, 16;                             jmp   n47_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n49_keyword_snobol4_α:  sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx265_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_kw_read_idx@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n50_keyword_snobol4_α
.Lx265_0:               .quad            21
#-----------------------------------------------------------------------------------------------------------------------
n50_keyword_snobol4_α:  sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx266_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_kw_read_idx@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n51_call_α
.Lx266_0:               .quad            22
#-----------------------------------------------------------------------------------------------------------------------
n51_call_α:             sub              rsp, 16
                        sub              rsp, 48
                        mov              r8, qword ptr [rsp + 96]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 24], r8
                        mov              r8, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 32], r8
                        mov              r8, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], r8
                        .section         .rodata
.Lrkfnzd268:            .string          "REPLACE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd268]
                        lea              rsi, [rsp + 0]
                        mov              edx, 3
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 48
                        cmp              al, 104;                             jne   .Lx267_240
                        add              rsp, 64;                             jmp   n47_lit_integer_β
.Lx267_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n52_binop_α
n51_call_β:             add              rsp, 64;                             jmp   n47_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n52_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 112]           # call
                        mov              rsi, qword ptr [rsp + 120]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n53_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n53_assign_α:           mov              rax, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 32], rax             # cap
                        mov              qword ptr [r9 + 40], rdx;            jmp   n54_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n54_statement_end_α:    add              rsp, 224;                            jmp   RETURN
#=======================================================================================================================
# cap_end  <stmt 10, line 44: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n55_statement_begin_α:                                                        jmp   n56_statement_end_α
n55_statement_begin_β:                                                        jmp   n57_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n56_statement_end_α:                                                          jmp   n57_statement_begin_α
#=======================================================================================================================
#         <stmt 11, line 45: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n57_statement_begin_α:                                                        jmp   n58_define_α
n57_statement_begin_β:                                                        jmp   n106_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n58_define_α:           mov              rdi, qword ptr [rip + .Lx280_0]
                        mov              rsi, qword ptr [rip + .Lx280_1]
                        mov              edx, 3
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + n60_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdi, qword ptr [rip + .Lx280_0]
                        lea              rsi, [rip + icase_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n59_statement_end_α
n58_define_β:                                                                 jmp   n57_statement_begin_β
.Lx280_0:               .quad            .Lx280_0_s
.Lx280_0_s:             .string          "icase"
.Lx280_1:               .quad            .Lx280_1_s
.Lx280_1_s:             .string          "str,letter,ch"
                                                                              jmp   .Lx281_245
#-----------------------------------------------------------------------------------------------------------------------
icase_α:                sub              rsp, 96
                        mov              rax, qword ptr [r9 + 80]             # letter
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 80], 0
                        mov              qword ptr [r9 + 88], 0
                        mov              rax, qword ptr [r9 + 96]             # ch
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 24], rax
                        mov              qword ptr [r9 + 96], 0
                        mov              qword ptr [r9 + 104], 0
                        mov              rax, qword ptr [r9 + 48]             # icase
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 40], rax
                        mov              qword ptr [r9 + 48], 0
                        mov              qword ptr [r9 + 56], 0
                        mov              qword ptr [rsp + 64], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 96]
                        cmp              rdx, 0;                              jbe   .Lx281_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 64]             # str
                        mov              qword ptr [r9 + 64], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 72]
                        mov              qword ptr [r9 + 72], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx281_41
.Lx281_10:              mov              rax, qword ptr [r9 + 64]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 88], rax
                        mov              qword ptr [r9 + 64], 0
                        mov              qword ptr [r9 + 72], 0
.Lx281_41:              lea              r10, [rip + icase_γ]
                        lea              r11, [rip + icase_ω]
                        push             r11
                        push             r10
                        lea              rax, [rip + n60_statement_begin_α];  jmp   rax
icase_γ:                mov              rdi, qword ptr [r9 + 48]             # icase
                        mov              rsi, qword ptr [r9 + 56]
                        mov              rcx, qword ptr [rsp + 64]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 96]
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 56], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 96], rax             # ch
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 104], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 80], rax             # letter
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 88], rax
                        cmp              rdx, 0;                              jbe   .Lx281_80
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 64], rax             # str
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 72], rax;            jmp   .Lx281_110
.Lx281_80:              mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [r9 + 72], rax
.Lx281_110:             mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 96
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
icase_ω:                mov              rcx, qword ptr [rsp + 64]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 96]
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 48], rax             # icase
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 56], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 96], rax             # ch
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 104], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 80], rax             # letter
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 88], rax
                        cmp              rdx, 0;                              jbe   .Lx281_150
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 64], rax             # str
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 72], rax;            jmp   .Lx281_180
.Lx281_150:             mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [r9 + 72], rax
.Lx281_180:             mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 96
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx281_245:
#-----------------------------------------------------------------------------------------------------------------------
n59_statement_end_α:                                                          jmp   n106_statement_begin_α
#=======================================================================================================================
# icase  <stmt 12, line 46: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n60_statement_begin_α:                                                        jmp   n61_var_α
n60_statement_begin_β:                                                        jmp   n64_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n61_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 64]             # str
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n62_call_α
#-----------------------------------------------------------------------------------------------------------------------
n62_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd288:            .string          "IDENT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd288]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx287_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n60_statement_begin_β
.Lx287_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n63_statement_end_α
n62_call_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n60_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n63_statement_end_α:    add              rsp, 32;                             jmp   RETURN
#=======================================================================================================================
#         <stmt 13, line 47: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n64_statement_begin_α:                                                        jmp   n65_var_α
n64_statement_begin_β:                                                        jmp   n89_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n65_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 64]             # str
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n66_match_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n66_match_begin_α:      mov              rdi, qword ptr [rsp + 0]             # var
                        mov              rsi, qword ptr [rsp + 8]
                        push             rbp
                        mov              rbp, rsp
                        push             r12                                  # cas_mark
                        push             r13                                  # outer_Σ
                        push             r14                                  # outer_δ
                        push             r15                                  # outer_Δ
                        sub              rsp, 24
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              dword ptr [rbp + -40], 0             # start_δ
.Lx295_0:               mov              r14d, dword ptr [rbp + -40]
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # match_beta_cont
                        mov              rax, qword ptr [rcx + 248]
                        mov              qword ptr [rbp + -48], rax
                        lea              rax, [rip + .Lx295_13]
                        mov              qword ptr [rcx + 248], rax;          jmp   n67_match_pos_α
n66_match_begin_β:
.Lx295_13:              lea              rsp, [rbp + -56]                     # retry_whack
                        add              dword ptr [rbp + -40], 1             # start_δ
                        mov              eax, dword ptr [rbp + -40]
                        cmp              eax, r15d;                           jg    .Lx295_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                              jne   .Lx295_1
                                                                              jmp   .Lx295_0
.Lx295_1:
n66_match_begin_af:     mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # mbc_restore
                        mov              rax, qword ptr [rbp + -48]
                        mov              qword ptr [rcx + 248], rax
                        mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rdi, r13                             # sig
                        mov              rsi, r15                             # len
                        call             rt_match_ctx_restore@PLT
                        mov              rsp, rbp
                        pop              rbp
                        add              rsp, 16;                             jmp   n89_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n67_match_pos_α:        mov              rax, 0
                        cmp              r14d, eax;                           jne   n66_match_begin_β
                                                                              jmp   n68_match_assign_save_α
n67_match_pos_β:                                                              jmp   n66_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n68_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d;           jmp   n69_match_any_α
n68_match_assign_save_β:
                        add              rsp, 16;                             jmp   n66_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n69_match_any_α:        mov              eax, r14d
                        cmp              eax, r15d;                           jl    .Lx300_239
                        add              rsp, 16;                             jmp   n66_match_begin_β
.Lx300_239:             movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        lea              rdi, [rip + .C0]
                        cmp              byte ptr [rdi+rsi], 0;               jne   .Lx300_240
                        add              rsp, 16;                             jmp   n66_match_begin_β
.Lx300_240:             add              r14d, 1;                             jmp   n70_match_assign_cond_α
n69_match_any_β:        sub              r14d, 1
                        add              rsp, 16;                             jmp   n66_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n70_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 0]
                        lea              rcx, [rip + .S3]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                             jmp   n71_match_end_α
n70_match_assign_cond_β:
                        sub              r12, 24;                             jmp   n69_match_any_β
#-----------------------------------------------------------------------------------------------------------------------
n71_match_end_α:        mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # mbc_restore
                        mov              rax, qword ptr [rbp + -48]
                        mov              qword ptr [rcx + 248], rax
                        mov              eax, dword ptr [rbp + -40]           # repl_start
                        mov              dword ptr [rbp + -48], eax
                        mov              qword ptr [rbp + -56], r14           # repl_end
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
                        call             rt_match_end_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        test             rax, rax;                            je    .Lx304_13
                        add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14;                                 jmp   n66_match_begin_af
.Lx304_13:              add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              eax, dword ptr [rbp + -48]           # repl_start
                        mov              dword ptr [r12 + 0], eax
                        mov              rax, qword ptr [rbp + -56]           # repl_end
                        mov              qword ptr [r12 + 8], rax
                        add              r12, 16
                        mov              rsp, rbp                             # frame_whack
                        pop              rbp;                                 jmp   n72_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n72_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx305_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n73_match_replace_α
.Lx305_0:               .quad            .Lx305_0_s
.Lx305_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n73_match_replace_α:    mov              rdi, qword ptr [rip + .Lx307_0]
                        mov              rsi, qword ptr [rsp + 16]            # var
                        mov              rdx, qword ptr [rsp + 24]
                        mov              ecx, dword ptr [r12 + -16]           # repl_start
                        mov              r8, qword ptr [r12 + -8]             # repl_end
                        sub              r12, 16
                        lea              r9, [rsp + 0]                        # lit_string
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_match_replace@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16;                             jmp   .Lx307_1
.Lx307_0:               .quad            .Lx307_0_s
.Lx307_0_s:             .string          "str"
.Lx307_1:                                                                     jmp   n74_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n74_statement_end_α:    add              rsp, 16;                             jmp   n75_statement_begin_α
#=======================================================================================================================
#         <stmt 14, line 48: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n75_statement_begin_α:                                                        jmp   n76_var_α
n75_statement_begin_β:                                                        jmp   n60_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n76_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 48]             # icase
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n77_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n77_assign_α:           mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 112], rax            # PAT$0$V0
                        mov              qword ptr [r9 + 120], rdx;           jmp   n78_var_α
#-----------------------------------------------------------------------------------------------------------------------
n78_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 80]             # letter
                        mov              rdx, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n79_call_α
n78_var_β:              add              rsp, 16
                        add              rsp, 16;                             jmp   n75_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n79_call_α:             sub              rsp, 16
                        lea              rcx, [rip + .Lsig316z]
                        lea              rax, [rip + upr_α];                  jmp   rax
.Lsig316z:              .quad            1
                        .quad            .Lx316_2
                        .quad            .Lx316_2
                        .quad            16
.Lx316_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx316_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx316_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx316_240
                        add              rsp, 16;                             jmp   n78_var_β
.Lx316_240:                                                                   jmp   n80_assign_α
n79_call_β:                                                                   jmp   n78_var_β
.Lx316_0:               .quad            .Lx316_0_s
.Lx316_0_s:             .string          "upr"
#-----------------------------------------------------------------------------------------------------------------------
n80_assign_α:           mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 128], rax            # PAT$0$V1
                        mov              qword ptr [r9 + 136], rdx;           jmp   n81_var_α
#-----------------------------------------------------------------------------------------------------------------------
n81_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 80]             # letter
                        mov              rdx, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n82_call_α
n81_var_β:              add              rsp, 32;                             jmp   n78_var_β
#-----------------------------------------------------------------------------------------------------------------------
n82_call_α:             sub              rsp, 16
                        lea              rcx, [rip + .Lsig320z]
                        lea              rax, [rip + lwr_α];                  jmp   rax
.Lsig320z:              .quad            1
                        .quad            .Lx320_2
                        .quad            .Lx320_2
                        .quad            16
.Lx320_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx320_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx320_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx320_240
                        add              rsp, 16;                             jmp   n81_var_β
.Lx320_240:                                                                   jmp   n83_assign_α
n82_call_β:                                                                   jmp   n81_var_β
.Lx320_0:               .quad            .Lx320_0_s
.Lx320_0_s:             .string          "lwr"
#-----------------------------------------------------------------------------------------------------------------------
n83_assign_α:           mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 144], rax            # PAT$0$V2
                        mov              qword ptr [r9 + 152], rdx;           jmp   n84_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n84_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx322_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n85_lit_string_α
n84_lit_string_β:       add              rsp, 32;                             jmp   n81_var_β
.Lx322_0:               .quad            .Lx322_0_s
.Lx322_0_s:             .string          "PAT$0"
#-----------------------------------------------------------------------------------------------------------------------
n85_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx323_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n86_call_α
n85_lit_string_β:       add              rsp, 16;                             jmp   n84_lit_string_β
.Lx323_0:               .quad            .Lx323_0_s
.Lx323_0_s:             .string          "3"
#-----------------------------------------------------------------------------------------------------------------------
n86_call_α:             sub              rsp, 16
                        sub              rsp, 32
                        mov              r8, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 48]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 24], r8
                        .section         .rodata
.Lrkfnzd325:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd325]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx324_240
                        add              rsp, 16;                             jmp   n85_lit_string_β
.Lx324_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n87_assign_α
n86_call_β:             add              rsp, 16;                             jmp   n85_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n87_assign_α:           mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 48], rax             # icase
                        mov              qword ptr [r9 + 56], rdx;            jmp   n88_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n88_statement_end_α:    add              rsp, 128;                            jmp   n60_statement_begin_α
#=======================================================================================================================
# icase1  <stmt 15, line 49: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n89_statement_begin_α:                                                        jmp   n90_var_α
n89_statement_begin_β:                                                        jmp   n100_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n90_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 64]             # str
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n91_match_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n91_match_begin_α:      mov              rdi, qword ptr [rsp + 0]             # var
                        mov              rsi, qword ptr [rsp + 8]
                        push             rbp
                        mov              rbp, rsp
                        push             r12                                  # cas_mark
                        push             r13                                  # outer_Σ
                        push             r14                                  # outer_δ
                        push             r15                                  # outer_Δ
                        sub              rsp, 24
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              dword ptr [rbp + -40], 0             # start_δ
.Lx333_0:               mov              r14d, dword ptr [rbp + -40]
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # match_beta_cont
                        mov              rax, qword ptr [rcx + 248]
                        mov              qword ptr [rbp + -48], rax
                        lea              rax, [rip + .Lx333_13]
                        mov              qword ptr [rcx + 248], rax;          jmp   n92_match_pos_α
n91_match_begin_β:
.Lx333_13:              lea              rsp, [rbp + -56]                     # retry_whack
                        add              dword ptr [rbp + -40], 1             # start_δ
                        mov              eax, dword ptr [rbp + -40]
                        cmp              eax, r15d;                           jg    .Lx333_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                              jne   .Lx333_1
                                                                              jmp   .Lx333_0
.Lx333_1:
n91_match_begin_af:     mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # mbc_restore
                        mov              rax, qword ptr [rbp + -48]
                        mov              qword ptr [rcx + 248], rax
                        mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rdi, r13                             # sig
                        mov              rsi, r15                             # len
                        call             rt_match_ctx_restore@PLT
                        mov              rsp, rbp
                        pop              rbp;                                 jmp   n99_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n92_match_pos_α:        mov              rax, 0
                        cmp              r14d, eax;                           jne   n91_match_begin_β
                                                                              jmp   n93_match_assign_save_α
n92_match_pos_β:                                                              jmp   n91_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n93_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d;           jmp   n94_match_len_α
n93_match_assign_save_β:
                        add              rsp, 16;                             jmp   n91_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n94_match_len_α:        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d;                           jle   .Lx337_240
                        add              rsp, 16;                             jmp   n91_match_begin_β
.Lx337_240:             add              r14d, 1;                             jmp   n95_match_assign_cond_α
n94_match_len_β:        sub              r14d, 1
                        add              rsp, 16;                             jmp   n91_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n95_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 0]
                        lea              rcx, [rip + .S4]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                             jmp   n96_match_end_α
n95_match_assign_cond_β:
                        sub              r12, 24;                             jmp   n94_match_len_β
#-----------------------------------------------------------------------------------------------------------------------
n96_match_end_α:        mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # mbc_restore
                        mov              rax, qword ptr [rbp + -48]
                        mov              qword ptr [rcx + 248], rax
                        mov              eax, dword ptr [rbp + -40]           # repl_start
                        mov              dword ptr [rbp + -48], eax
                        mov              qword ptr [rbp + -56], r14           # repl_end
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
                        call             rt_match_end_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        test             rax, rax;                            je    .Lx341_13
                        add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14;                                 jmp   n91_match_begin_af
.Lx341_13:              add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              eax, dword ptr [rbp + -48]           # repl_start
                        mov              dword ptr [r12 + 0], eax
                        mov              rax, qword ptr [rbp + -56]           # repl_end
                        mov              qword ptr [r12 + 8], rax
                        add              r12, 16
                        mov              rsp, rbp                             # frame_whack
                        pop              rbp;                                 jmp   n97_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n97_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx342_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n98_match_replace_α
.Lx342_0:               .quad            .Lx342_0_s
.Lx342_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n98_match_replace_α:    mov              rdi, qword ptr [rip + .Lx344_0]
                        mov              rsi, qword ptr [rsp + 16]            # var
                        mov              rdx, qword ptr [rsp + 24]
                        mov              ecx, dword ptr [r12 + -16]           # repl_start
                        mov              r8, qword ptr [r12 + -8]             # repl_end
                        sub              r12, 16
                        lea              r9, [rsp + 0]                        # lit_string
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_match_replace@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16;                             jmp   .Lx344_1
.Lx344_0:               .quad            .Lx344_0_s
.Lx344_0_s:             .string          "str"
.Lx344_1:                                                                     jmp   n99_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n99_statement_end_α:    add              rsp, 16;                             jmp   n100_statement_begin_α
#=======================================================================================================================
#         <stmt 16, line 50: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n100_statement_begin_α:                                                       jmp   n101_var_α
n100_statement_begin_β:                                                       jmp   n60_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n101_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 48]             # icase
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n102_var_α
#-----------------------------------------------------------------------------------------------------------------------
n102_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 96]             # ch
                        mov              rdx, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n103_binop_α
n102_var_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n100_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n103_binop_α:           sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n104_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n104_assign_α:          mov              rax, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 48], rax             # icase
                        mov              qword ptr [r9 + 56], rdx;            jmp   n105_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n105_statement_end_α:   add              rsp, 48;                             jmp   n60_statement_begin_α
#=======================================================================================================================
# icase_end  <stmt 17, line 52: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n106_statement_begin_α:                                                       jmp   n107_statement_end_α
n106_statement_begin_β:                                                       jmp   n108_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n107_statement_end_α:                                                         jmp   n108_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n108_statement_begin_α:                                                       jmp   n109_statement_end_α
n108_statement_begin_β:                                                       jmp   n110_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n109_statement_end_α:                                                         jmp   n110_statement_begin_α
#=======================================================================================================================
#         <stmt 19, line 53: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n110_statement_begin_α:                                                       jmp   n111_lit_integer_α
n110_statement_begin_β:                                                       jmp   n114_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n111_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx365_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n112_keyword_assign_snobol4_α
.Lx365_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n112_keyword_assign_snobol4_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx366_0]
                        mov              rsi, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_kw_write_idx@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx366_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n110_statement_begin_β
.Lx366_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n113_statement_end_α
.Lx366_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n113_statement_end_α:                                                         jmp   n114_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n114_statement_begin_α:                                                       jmp   n115_statement_end_α
n114_statement_begin_β: add              rsp, 32;                             jmp   n116_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n115_statement_end_α:   add              rsp, 32;                             jmp   n116_statement_begin_α
#=======================================================================================================================
#         <stmt 21, line 55: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n116_statement_begin_α:                                                       jmp   n117_lit_string_α
n116_statement_begin_β:                                                       jmp   n121_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n117_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 11
                        mov              rax, qword ptr [rip + .Lx375_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n118_call_α
.Lx375_0:               .quad            .Lx375_0_s
.Lx375_0_s:             .string          "HELLO WORLD"
#-----------------------------------------------------------------------------------------------------------------------
n118_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig377z]
                        lea              rax, [rip + lwr_α];                  jmp   rax
.Lsig377z:              .quad            1
                        .quad            .Lx377_2
                        .quad            .Lx377_2
                        .quad            16
.Lx377_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx377_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx377_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx377_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n116_statement_begin_β
.Lx377_240:                                                                   jmp   n119_assign_α
n118_call_β:                                                                  jmp   n116_statement_begin_β
.Lx377_0:               .quad            .Lx377_0_s
.Lx377_0_s:             .string          "lwr"
#-----------------------------------------------------------------------------------------------------------------------
n119_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx378_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n120_statement_end_α
.Lx378_0:               .quad            .Lx378_0_s
.Lx378_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n120_statement_end_α:   add              rsp, 32;                             jmp   n121_statement_begin_α
#=======================================================================================================================
#         <stmt 22, line 56: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n121_statement_begin_α:                                                       jmp   n122_lit_string_α
n121_statement_begin_β:                                                       jmp   n126_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n122_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 11
                        mov              rax, qword ptr [rip + .Lx383_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n123_call_α
.Lx383_0:               .quad            .Lx383_0_s
.Lx383_0_s:             .string          "hello world"
#-----------------------------------------------------------------------------------------------------------------------
n123_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig385z]
                        lea              rax, [rip + upr_α];                  jmp   rax
.Lsig385z:              .quad            1
                        .quad            .Lx385_2
                        .quad            .Lx385_2
                        .quad            16
.Lx385_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx385_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx385_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx385_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n121_statement_begin_β
.Lx385_240:                                                                   jmp   n124_assign_α
n123_call_β:                                                                  jmp   n121_statement_begin_β
.Lx385_0:               .quad            .Lx385_0_s
.Lx385_0_s:             .string          "upr"
#-----------------------------------------------------------------------------------------------------------------------
n124_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx386_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n125_statement_end_α
.Lx386_0:               .quad            .Lx386_0_s
.Lx386_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n125_statement_end_α:   add              rsp, 32;                             jmp   n126_statement_begin_α
#=======================================================================================================================
#         <stmt 23, line 57: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n126_statement_begin_α:                                                       jmp   n127_lit_string_α
n126_statement_begin_β:                                                       jmp   n131_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n127_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 11
                        mov              rax, qword ptr [rip + .Lx391_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n128_call_α
.Lx391_0:               .quad            .Lx391_0_s
.Lx391_0_s:             .string          "hELLO wORLD"
#-----------------------------------------------------------------------------------------------------------------------
n128_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig393z]
                        lea              rax, [rip + cap_α];                  jmp   rax
.Lsig393z:              .quad            1
                        .quad            .Lx393_2
                        .quad            .Lx393_2
                        .quad            16
.Lx393_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx393_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx393_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx393_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n126_statement_begin_β
.Lx393_240:                                                                   jmp   n129_assign_α
n128_call_β:                                                                  jmp   n126_statement_begin_β
.Lx393_0:               .quad            .Lx393_0_s
.Lx393_0_s:             .string          "cap"
#-----------------------------------------------------------------------------------------------------------------------
n129_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx394_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n130_statement_end_α
.Lx394_0:               .quad            .Lx394_0_s
.Lx394_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n130_statement_end_α:                                                         jmp   n131_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n131_statement_begin_α:                                                       jmp   n132_statement_end_α
n131_statement_begin_β: add              rsp, 32;                             jmp   n133_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n132_statement_end_α:   add              rsp, 32;                             jmp   n133_statement_begin_α
#=======================================================================================================================
#         <stmt 25, line 60: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n133_statement_begin_α:                                                       jmp   n134_lit_string_α
n133_statement_begin_β:                                                       jmp   n146_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n134_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx403_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n135_lit_string_α
.Lx403_0:               .quad            .Lx403_0_s
.Lx403_0_s:             .string          "Hello"
#-----------------------------------------------------------------------------------------------------------------------
n135_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx404_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n136_call_α
n135_lit_string_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n146_statement_begin_α
.Lx404_0:               .quad            .Lx404_0_s
.Lx404_0_s:             .string          "hello"
#-----------------------------------------------------------------------------------------------------------------------
n136_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig406z]
                        lea              rax, [rip + icase_α];                jmp   rax
.Lsig406z:              .quad            1
                        .quad            .Lx406_2
                        .quad            .Lx406_2
                        .quad            16
.Lx406_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx406_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx406_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx406_240
                        add              rsp, 16;                             jmp   n135_lit_string_β
.Lx406_240:                                                                   jmp   n137_assign_α
n136_call_β:                                                                  jmp   n135_lit_string_β
.Lx406_0:               .quad            .Lx406_0_s
.Lx406_0_s:             .string          "icase"
#-----------------------------------------------------------------------------------------------------------------------
n137_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx407_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n138_match_begin_α
n137_assign_β:          add              rsp, 16;                             jmp   n135_lit_string_β
.Lx407_0:               .quad            .Lx407_0_s
.Lx407_0_s:             .string          "PATV$0"
#-----------------------------------------------------------------------------------------------------------------------
n138_match_begin_α:     mov              rdi, qword ptr [rsp + 32]            # lit_string
                        mov              rsi, qword ptr [rsp + 40]
                        push             rbp
                        mov              rbp, rsp
                        push             r12                                  # cas_mark
                        push             r13                                  # outer_Σ
                        push             r14                                  # outer_δ
                        push             r15                                  # outer_Δ
                        sub              rsp, 24
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              dword ptr [rbp + -40], 0             # start_δ
.Lx409_0:               mov              r14d, dword ptr [rbp + -40]
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # match_beta_cont
                        mov              rax, qword ptr [rcx + 248]
                        mov              qword ptr [rbp + -48], rax
                        lea              rax, [rip + .Lx409_13]
                        mov              qword ptr [rcx + 248], rax;          jmp   n139_match_defer_α
n138_match_begin_β:
.Lx409_13:              lea              rsp, [rbp + -56]                     # retry_whack
                        add              dword ptr [rbp + -40], 1             # start_δ
                        mov              eax, dword ptr [rbp + -40]
                        cmp              eax, r15d;                           jg    .Lx409_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                              jne   .Lx409_1
                                                                              jmp   .Lx409_0
.Lx409_1:
n138_match_begin_af:    mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # mbc_restore
                        mov              rax, qword ptr [rbp + -48]
                        mov              qword ptr [rcx + 248], rax
                        mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rdi, r13                             # sig
                        mov              rsi, r15                             # len
                        call             rt_match_ctx_restore@PLT
                        mov              rsp, rbp
                        pop              rbp;                                 jmp   n137_assign_β
#-----------------------------------------------------------------------------------------------------------------------
n139_match_defer_α:     lea              rdi, [rip + .S5]
                        xor              esi, esi
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx410_14
                        mov              rax, qword ptr [rdx + 0]
.Lx410_14:              test             rax, rax;                            jz    .Lx410_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx410_5]
                        push             rcx
                        lea              rcx, [rip + .Lx410_4]
                        push             rcx;                                 jmp   rax
.Lx410_4:                                                                     jmp   n140_match_end_α
.Lx410_5:                                                                     jmp   n138_match_begin_β
.Lx410_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S5]
                        mov              esi, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n138_match_begin_β
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx410_6]
                        push             rcx
                        push             rax;                                 jmp   n140_match_end_α
.Lx410_6:               add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n138_match_begin_β
n139_match_defer_β:     cmp              qword ptr [rsp + 0], 0;              jne   .Lx410_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx410_12
                                                                              jmp   rax
.Lx410_12:                                                                    jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n140_match_end_α:       mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # mbc_restore
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
                        call             rt_match_end_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        test             rax, rax;                            je    .Lx412_13
                        add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14;                                 jmp   n138_match_begin_af
.Lx412_13:              add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rsp, rbp                             # frame_whack
                        pop              rbp;                                 jmp   n141_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n141_statement_end_α:   add              rsp, 48;                             jmp   n142_statement_begin_α
#=======================================================================================================================
#         <stmt 26, line 61: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n142_statement_begin_α:                                                       jmp   n143_lit_string_α
n142_statement_begin_β:                                                       jmp   n146_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n143_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 15
                        mov              rax, qword ptr [rip + .Lx417_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n144_assign_α
.Lx417_0:               .quad            .Lx417_0_s
.Lx417_0_s:             .string          "ok: icase hello"
#-----------------------------------------------------------------------------------------------------------------------
n144_assign_α:          mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx418_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n145_statement_end_α
.Lx418_0:               .quad            .Lx418_0_s
.Lx418_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n145_statement_end_α:   add              rsp, 16;                             jmp   n146_statement_begin_α
#=======================================================================================================================
# bad_ic1  <stmt 27, line 63: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n146_statement_begin_α:                                                       jmp   n147_statement_end_α
n146_statement_begin_β:                                                       jmp   n148_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n147_statement_end_α:                                                         jmp   n148_statement_begin_α
#=======================================================================================================================
#         <stmt 28, line 63: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n148_statement_begin_α:                                                       jmp   n149_lit_string_α
n148_statement_begin_β:                                                       jmp   n161_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n149_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx427_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n150_lit_string_α
.Lx427_0:               .quad            .Lx427_0_s
.Lx427_0_s:             .string          "HELLO"
#-----------------------------------------------------------------------------------------------------------------------
n150_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx428_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n151_call_α
n150_lit_string_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n161_statement_begin_α
.Lx428_0:               .quad            .Lx428_0_s
.Lx428_0_s:             .string          "hello"
#-----------------------------------------------------------------------------------------------------------------------
n151_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig430z]
                        lea              rax, [rip + icase_α];                jmp   rax
.Lsig430z:              .quad            1
                        .quad            .Lx430_2
                        .quad            .Lx430_2
                        .quad            16
.Lx430_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx430_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx430_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx430_240
                        add              rsp, 16;                             jmp   n150_lit_string_β
.Lx430_240:                                                                   jmp   n152_assign_α
n151_call_β:                                                                  jmp   n150_lit_string_β
.Lx430_0:               .quad            .Lx430_0_s
.Lx430_0_s:             .string          "icase"
#-----------------------------------------------------------------------------------------------------------------------
n152_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx431_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n153_match_begin_α
n152_assign_β:          add              rsp, 16;                             jmp   n150_lit_string_β
.Lx431_0:               .quad            .Lx431_0_s
.Lx431_0_s:             .string          "PATV$1"
#-----------------------------------------------------------------------------------------------------------------------
n153_match_begin_α:     mov              rdi, qword ptr [rsp + 32]            # lit_string
                        mov              rsi, qword ptr [rsp + 40]
                        push             rbp
                        mov              rbp, rsp
                        push             r12                                  # cas_mark
                        push             r13                                  # outer_Σ
                        push             r14                                  # outer_δ
                        push             r15                                  # outer_Δ
                        sub              rsp, 24
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              dword ptr [rbp + -40], 0             # start_δ
.Lx433_0:               mov              r14d, dword ptr [rbp + -40]
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # match_beta_cont
                        mov              rax, qword ptr [rcx + 248]
                        mov              qword ptr [rbp + -48], rax
                        lea              rax, [rip + .Lx433_13]
                        mov              qword ptr [rcx + 248], rax;          jmp   n154_match_defer_α
n153_match_begin_β:
.Lx433_13:              lea              rsp, [rbp + -56]                     # retry_whack
                        add              dword ptr [rbp + -40], 1             # start_δ
                        mov              eax, dword ptr [rbp + -40]
                        cmp              eax, r15d;                           jg    .Lx433_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                              jne   .Lx433_1
                                                                              jmp   .Lx433_0
.Lx433_1:
n153_match_begin_af:    mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # mbc_restore
                        mov              rax, qword ptr [rbp + -48]
                        mov              qword ptr [rcx + 248], rax
                        mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rdi, r13                             # sig
                        mov              rsi, r15                             # len
                        call             rt_match_ctx_restore@PLT
                        mov              rsp, rbp
                        pop              rbp;                                 jmp   n152_assign_β
#-----------------------------------------------------------------------------------------------------------------------
n154_match_defer_α:     lea              rdi, [rip + .S6]
                        xor              esi, esi
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx434_14
                        mov              rax, qword ptr [rdx + 0]
.Lx434_14:              test             rax, rax;                            jz    .Lx434_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx434_5]
                        push             rcx
                        lea              rcx, [rip + .Lx434_4]
                        push             rcx;                                 jmp   rax
.Lx434_4:                                                                     jmp   n155_match_end_α
.Lx434_5:                                                                     jmp   n153_match_begin_β
.Lx434_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S6]
                        mov              esi, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n153_match_begin_β
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx434_6]
                        push             rcx
                        push             rax;                                 jmp   n155_match_end_α
.Lx434_6:               add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n153_match_begin_β
n154_match_defer_β:     cmp              qword ptr [rsp + 0], 0;              jne   .Lx434_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx434_12
                                                                              jmp   rax
.Lx434_12:                                                                    jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n155_match_end_α:       mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # mbc_restore
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
                        call             rt_match_end_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        test             rax, rax;                            je    .Lx436_13
                        add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14;                                 jmp   n153_match_begin_af
.Lx436_13:              add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rsp, rbp                             # frame_whack
                        pop              rbp;                                 jmp   n156_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n156_statement_end_α:   add              rsp, 48;                             jmp   n157_statement_begin_α
#=======================================================================================================================
#         <stmt 29, line 64: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n157_statement_begin_α:                                                       jmp   n158_lit_string_α
n157_statement_begin_β:                                                       jmp   n161_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n158_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 15
                        mov              rax, qword ptr [rip + .Lx441_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n159_assign_α
.Lx441_0:               .quad            .Lx441_0_s
.Lx441_0_s:             .string          "ok: icase HELLO"
#-----------------------------------------------------------------------------------------------------------------------
n159_assign_α:          mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx442_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n160_statement_end_α
.Lx442_0:               .quad            .Lx442_0_s
.Lx442_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n160_statement_end_α:   add              rsp, 16;                             jmp   n161_statement_begin_α
#=======================================================================================================================
# bad_ic2  <stmt 30, line 66: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n161_statement_begin_α:                                                       jmp   n162_statement_end_α
n161_statement_begin_β:                                                       jmp   n163_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n162_statement_end_α:                                                         jmp   n163_statement_begin_α
#=======================================================================================================================
#         <stmt 31, line 66: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n163_statement_begin_α:                                                       jmp   n164_lit_string_α
n163_statement_begin_β:                                                       jmp   n176_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n164_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx451_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n165_lit_string_α
.Lx451_0:               .quad            .Lx451_0_s
.Lx451_0_s:             .string          "HeLLo"
#-----------------------------------------------------------------------------------------------------------------------
n165_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx452_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n166_call_α
n165_lit_string_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n176_statement_begin_α
.Lx452_0:               .quad            .Lx452_0_s
.Lx452_0_s:             .string          "hello"
#-----------------------------------------------------------------------------------------------------------------------
n166_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig454z]
                        lea              rax, [rip + icase_α];                jmp   rax
.Lsig454z:              .quad            1
                        .quad            .Lx454_2
                        .quad            .Lx454_2
                        .quad            16
.Lx454_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx454_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx454_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx454_240
                        add              rsp, 16;                             jmp   n165_lit_string_β
.Lx454_240:                                                                   jmp   n167_assign_α
n166_call_β:                                                                  jmp   n165_lit_string_β
.Lx454_0:               .quad            .Lx454_0_s
.Lx454_0_s:             .string          "icase"
#-----------------------------------------------------------------------------------------------------------------------
n167_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx455_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n168_match_begin_α
n167_assign_β:          add              rsp, 16;                             jmp   n165_lit_string_β
.Lx455_0:               .quad            .Lx455_0_s
.Lx455_0_s:             .string          "PATV$2"
#-----------------------------------------------------------------------------------------------------------------------
n168_match_begin_α:     mov              rdi, qword ptr [rsp + 32]            # lit_string
                        mov              rsi, qword ptr [rsp + 40]
                        push             rbp
                        mov              rbp, rsp
                        push             r12                                  # cas_mark
                        push             r13                                  # outer_Σ
                        push             r14                                  # outer_δ
                        push             r15                                  # outer_Δ
                        sub              rsp, 24
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              dword ptr [rbp + -40], 0             # start_δ
.Lx457_0:               mov              r14d, dword ptr [rbp + -40]
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # match_beta_cont
                        mov              rax, qword ptr [rcx + 248]
                        mov              qword ptr [rbp + -48], rax
                        lea              rax, [rip + .Lx457_13]
                        mov              qword ptr [rcx + 248], rax;          jmp   n169_match_defer_α
n168_match_begin_β:
.Lx457_13:              lea              rsp, [rbp + -56]                     # retry_whack
                        add              dword ptr [rbp + -40], 1             # start_δ
                        mov              eax, dword ptr [rbp + -40]
                        cmp              eax, r15d;                           jg    .Lx457_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                              jne   .Lx457_1
                                                                              jmp   .Lx457_0
.Lx457_1:
n168_match_begin_af:    mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # mbc_restore
                        mov              rax, qword ptr [rbp + -48]
                        mov              qword ptr [rcx + 248], rax
                        mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rdi, r13                             # sig
                        mov              rsi, r15                             # len
                        call             rt_match_ctx_restore@PLT
                        mov              rsp, rbp
                        pop              rbp;                                 jmp   n167_assign_β
#-----------------------------------------------------------------------------------------------------------------------
n169_match_defer_α:     lea              rdi, [rip + .S7]
                        xor              esi, esi
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx458_14
                        mov              rax, qword ptr [rdx + 0]
.Lx458_14:              test             rax, rax;                            jz    .Lx458_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx458_5]
                        push             rcx
                        lea              rcx, [rip + .Lx458_4]
                        push             rcx;                                 jmp   rax
.Lx458_4:                                                                     jmp   n170_match_end_α
.Lx458_5:                                                                     jmp   n168_match_begin_β
.Lx458_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S7]
                        mov              esi, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n168_match_begin_β
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx458_6]
                        push             rcx
                        push             rax;                                 jmp   n170_match_end_α
.Lx458_6:               add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n168_match_begin_β
n169_match_defer_β:     cmp              qword ptr [rsp + 0], 0;              jne   .Lx458_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx458_12
                                                                              jmp   rax
.Lx458_12:                                                                    jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n170_match_end_α:       mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # mbc_restore
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
                        call             rt_match_end_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        test             rax, rax;                            je    .Lx460_13
                        add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14;                                 jmp   n168_match_begin_af
.Lx460_13:              add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rsp, rbp                             # frame_whack
                        pop              rbp;                                 jmp   n171_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n171_statement_end_α:   add              rsp, 48;                             jmp   n172_statement_begin_α
#=======================================================================================================================
#         <stmt 32, line 67: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n172_statement_begin_α:                                                       jmp   n173_lit_string_α
n172_statement_begin_β:                                                       jmp   n176_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n173_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 15
                        mov              rax, qword ptr [rip + .Lx465_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n174_assign_α
.Lx465_0:               .quad            .Lx465_0_s
.Lx465_0_s:             .string          "ok: icase HeLLo"
#-----------------------------------------------------------------------------------------------------------------------
n174_assign_α:          mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx466_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n175_statement_end_α
.Lx466_0:               .quad            .Lx466_0_s
.Lx466_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n175_statement_end_α:   add              rsp, 16;                             jmp   n176_statement_begin_α
#=======================================================================================================================
# bad_ic3  <stmt 33, line 69: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n176_statement_begin_α:                                                       jmp   n177_statement_end_α
n176_statement_begin_β:                                                       jmp   n178_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n177_statement_end_α:                                                         jmp   n178_statement_begin_α
#=======================================================================================================================
#         <stmt 34, line 69: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n178_statement_begin_α:                                                       jmp   n179_lit_string_α
n178_statement_begin_β:                                                       jmp   n187_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n179_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx475_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n180_lit_string_α
.Lx475_0:               .quad            .Lx475_0_s
.Lx475_0_s:             .string          "world"
#-----------------------------------------------------------------------------------------------------------------------
n180_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx476_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n181_call_α
n180_lit_string_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n187_statement_begin_α
.Lx476_0:               .quad            .Lx476_0_s
.Lx476_0_s:             .string          "hello"
#-----------------------------------------------------------------------------------------------------------------------
n181_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig478z]
                        lea              rax, [rip + icase_α];                jmp   rax
.Lsig478z:              .quad            1
                        .quad            .Lx478_2
                        .quad            .Lx478_2
                        .quad            16
.Lx478_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx478_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx478_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx478_240
                        add              rsp, 16;                             jmp   n180_lit_string_β
.Lx478_240:                                                                   jmp   n182_assign_α
n181_call_β:                                                                  jmp   n180_lit_string_β
.Lx478_0:               .quad            .Lx478_0_s
.Lx478_0_s:             .string          "icase"
#-----------------------------------------------------------------------------------------------------------------------
n182_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx479_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n183_match_begin_α
n182_assign_β:          add              rsp, 16;                             jmp   n180_lit_string_β
.Lx479_0:               .quad            .Lx479_0_s
.Lx479_0_s:             .string          "PATV$3"
#-----------------------------------------------------------------------------------------------------------------------
n183_match_begin_α:     mov              rdi, qword ptr [rsp + 32]            # lit_string
                        mov              rsi, qword ptr [rsp + 40]
                        push             rbp
                        mov              rbp, rsp
                        push             r12                                  # cas_mark
                        push             r13                                  # outer_Σ
                        push             r14                                  # outer_δ
                        push             r15                                  # outer_Δ
                        sub              rsp, 24
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              dword ptr [rbp + -40], 0             # start_δ
.Lx481_0:               mov              r14d, dword ptr [rbp + -40]
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # match_beta_cont
                        mov              rax, qword ptr [rcx + 248]
                        mov              qword ptr [rbp + -48], rax
                        lea              rax, [rip + .Lx481_13]
                        mov              qword ptr [rcx + 248], rax;          jmp   n184_match_defer_α
n183_match_begin_β:
.Lx481_13:              lea              rsp, [rbp + -56]                     # retry_whack
                        add              dword ptr [rbp + -40], 1             # start_δ
                        mov              eax, dword ptr [rbp + -40]
                        cmp              eax, r15d;                           jg    .Lx481_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                              jne   .Lx481_1
                                                                              jmp   .Lx481_0
.Lx481_1:
n183_match_begin_af:    mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # mbc_restore
                        mov              rax, qword ptr [rbp + -48]
                        mov              qword ptr [rcx + 248], rax
                        mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rdi, r13                             # sig
                        mov              rsi, r15                             # len
                        call             rt_match_ctx_restore@PLT
                        mov              rsp, rbp
                        pop              rbp;                                 jmp   n182_assign_β
#-----------------------------------------------------------------------------------------------------------------------
n184_match_defer_α:     lea              rdi, [rip + .S8]
                        xor              esi, esi
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx482_14
                        mov              rax, qword ptr [rdx + 0]
.Lx482_14:              test             rax, rax;                            jz    .Lx482_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx482_5]
                        push             rcx
                        lea              rcx, [rip + .Lx482_4]
                        push             rcx;                                 jmp   rax
.Lx482_4:                                                                     jmp   n185_match_end_α
.Lx482_5:                                                                     jmp   n183_match_begin_β
.Lx482_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S8]
                        mov              esi, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n183_match_begin_β
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx482_6]
                        push             rcx
                        push             rax;                                 jmp   n185_match_end_α
.Lx482_6:               add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n183_match_begin_β
n184_match_defer_β:     cmp              qword ptr [rsp + 0], 0;              jne   .Lx482_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx482_12
                                                                              jmp   rax
.Lx482_12:                                                                    jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n185_match_end_α:       mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # mbc_restore
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
                        call             rt_match_end_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        test             rax, rax;                            je    .Lx484_13
                        add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14;                                 jmp   n183_match_begin_af
.Lx484_13:              add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rsp, rbp                             # frame_whack
                        pop              rbp;                                 jmp   n186_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n186_statement_end_α:   add              rsp, 48;                             jmp   n191_statement_begin_α
#=======================================================================================================================
#         <stmt 35, line 70: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n187_statement_begin_α:                                                       jmp   n188_lit_string_α
n187_statement_begin_β:                                                       jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n188_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 11
                        mov              rax, qword ptr [rip + .Lx489_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n189_assign_α
.Lx489_0:               .quad            .Lx489_0_s
.Lx489_0_s:             .string          "no match ok"
#-----------------------------------------------------------------------------------------------------------------------
n189_assign_α:          mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx490_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n190_statement_end_α
.Lx490_0:               .quad            .Lx490_0_s
.Lx490_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n190_statement_end_α:   add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# bad_ic4  <stmt 36, line 71: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n191_statement_begin_α:                                                       jmp   n192_lit_string_α
n191_statement_begin_β:                                                       jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n192_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 32
                        mov              rax, qword ptr [rip + .Lx495_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n193_assign_α
.Lx495_0:               .quad            .Lx495_0_s
.Lx495_0_s:             .string          "FAIL: icase matched wrong string"
#-----------------------------------------------------------------------------------------------------------------------
n193_assign_α:          mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx496_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n194_statement_end_α
.Lx496_0:               .quad            .Lx496_0_s
.Lx496_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n194_statement_end_α:   add              rsp, 16;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
RETURN:                 pop              rcx
                        add              rsp, 8;                              jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
FRETURN:                add              rsp, 8
                        pop              rcx;                                 jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                              jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        add              rsp, 0
                        xor              edi, edi
                        call             exit@PLT
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        add              rsp, 0
                        mov              edi, 1
                        call             exit@PLT
module_init:
                        sub              rsp, 8
                        .section         .rodata
.Lseala4:               .string          "lwr"
                        .section         .text
                        .intel_syntax    noprefix
                        .weak            lwr_α
                        lea              rdi, [rip + .Lseala4]
                        mov              rsi, qword ptr [rip + lwr_α@GOTPCREL]
                        call             rt_proc_seal_alpha@PLT
                        .section         .rodata
.Lseala5:               .string          "upr"
                        .section         .text
                        .intel_syntax    noprefix
                        .weak            upr_α
                        lea              rdi, [rip + .Lseala5]
                        mov              rsi, qword ptr [rip + upr_α@GOTPCREL]
                        call             rt_proc_seal_alpha@PLT
                        .section         .rodata
.Lseala6:               .string          "cap"
                        .section         .text
                        .intel_syntax    noprefix
                        .weak            cap_α
                        lea              rdi, [rip + .Lseala6]
                        mov              rsi, qword ptr [rip + cap_α@GOTPCREL]
                        call             rt_proc_seal_alpha@PLT
                        .section         .rodata
.Lseala7:               .string          "icase"
                        .section         .text
                        .intel_syntax    noprefix
                        .weak            icase_α
                        lea              rdi, [rip + .Lseala7]
                        mov              rsi, qword ptr [rip + icase_α@GOTPCREL]
                        call             rt_proc_seal_alpha@PLT
                        .section         .rodata
.Lstartup_pname8:       .string          "PAT$0"
                        .align           8
.Lstartup_prec8:
                        .quad            .Lstartup_pname8
                        .quad            FN__PAT$0
                        .quad            0
                        .quad            0
                        .quad            0
                        .long            0
                        .long            0
                        .long            112
                        .long            16
                        .long            0
                        .long            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_prec8]
                        call             rt_proc_register_rec@PLT
                        add              rsp, 8
                        ret
                        .section         .rodata
.S0:                    .string          "PAT$0$V0"
.S1:                    .string          "PAT$0$V2"
.S2:                    .string          "PAT$0$V1"
.S3:                    .string          "letter"
.S4:                    .string          "ch"
.S5:                    .string          "PATV$0"
.S6:                    .string          "PATV$1"
.S7:                    .string          "PATV$2"
.S8:                    .string          "PATV$3"
                        .text
                        .section         .rodata
.C0:                    .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
                        .byte            1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0
                        .byte            0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
                        .byte            1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .text
                        .section         .note.GNU-stack,"",@progbits

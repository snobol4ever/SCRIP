                        .intel_syntax    noprefix
                        .text
                        .section         .rodata
.Lgvan0:                .string          "s"
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
                        mov              r12, qword ptr [0x70000000]
                        xor              esi, esi
                                                                                        jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 8
#=======================================================================================================================
#         s = 'abcdefghij'
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:
                                                                                        jmp   n1_lit_string_α
n0_statement_begin_β:
                                                                                        jmp   n4_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n1_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 10
                        mov              rax, qword ptr [rip + .Lx29_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n2_assign_α
n1_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n4_statement_begin_α
.Lx29_0:
                        .quad            .Lx29_0_s
.Lx29_0_s:
                        .string          "abcdefghij"
#-----------------------------------------------------------------------------------------------------------------------
n2_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax                    # s
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n3_statement_end_α
n2_assign_β:
                                                                                        jmp   n3_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n3_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n4_statement_begin_α
n3_statement_end_β:
                        add              rsp, 16
                                                                                        jmp   n4_statement_begin_α
#=======================================================================================================================
#         s POS(0) ARBNO(LEN(1) | LEN(1)) RPOS(0)    :S(YES)F(NO)
#-----------------------------------------------------------------------------------------------------------------------
n4_statement_begin_α:
                                                                                        jmp   n5_var_α
n4_statement_begin_β:
                                                                                        jmp   n21_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n5_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052288]                    # s
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n6_match_begin_α
n5_var_β:
                        add              rsp, 16
                                                                                        jmp   n21_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n6_match_begin_α:
                        mov              rdi, qword ptr [rsp + 0]                       # var
                        mov              rsi, qword ptr [rsp + 8]
                        mov              rax, rsp
                        sub              rsp, 80
                        mov              qword ptr [rsp + 16], rax                      # rsp_mark
                        mov              qword ptr [rsp + 48], r13                      # outer_Σ
                        mov              qword ptr [rsp + 56], r14                      # outer_δ
                        mov              qword ptr [rsp + 64], r15                      # outer_Δ
                        mov              qword ptr [r12 + 0], 0                         # cas_top
                        mov              qword ptr [r12 + 8], rax                       # cas_rsp_mark
                        mov              qword ptr [r12 + 16], 0
                        add              r12, 24                                        # cas_top
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rsp + 72], rax                      # cap_gen
                        mov              qword ptr [rsp + 40], rbp                      # old_rbp
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              dword ptr [rsp + 0], 0                         # start_δ
.Lx37_0:
                        mov              r14d, dword ptr [rsp + 0]
                                                                                        jmp   n7_lit_integer_α
n6_match_begin_β:
                        add              dword ptr [rsp + 0], 1
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d
                                                                                        jg    .Lx37_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx37_1
                                                                                        jmp   .Lx37_0
.Lx37_1:
n6_match_begin_af:
                        sub              r12, 24                                        # cas_mark
                        mov              rsp, qword ptr [r12 + 8]                       # cas_rsp_mark
                        sub              rsp, 80
                        mov              r13, qword ptr [rsp + 48]                      # outer_Σ
                        mov              r14, qword ptr [rsp + 56]                      # outer_δ
                        mov              r15, qword ptr [rsp + 64]                      # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        mov              rdx, qword ptr [rsp + 72]                      # cap_gen
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rsp + 40]                      # old_rbp
                        add              rsp, 80
                        add              rsp, 16
                                                                                        jmp   n21_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n7_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx38_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n8_match_pos_α
n7_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n6_match_begin_β
.Lx38_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n8_match_pos_α:
                        mov              rax, 0
                        cmp              r14d, eax
                                                                                        jne   n6_match_begin_β
                                                                                        jmp   n9_match_arbno_α
n8_match_pos_β:
                                                                                        jmp   n6_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n9_match_arbno_α:
                        sub              rsp, 592
                        mov              eax, 0
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        mov              qword ptr [rsp + 32], rax
                        mov              qword ptr [rsp + 40], rax
                        mov              qword ptr [rsp + 48], rax
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [rsp + 64], rax
                        mov              qword ptr [rsp + 72], rax
                        mov              qword ptr [rsp + 80], rax
                        mov              qword ptr [rsp + 88], rax
                        mov              qword ptr [rsp + 96], rax
                        mov              qword ptr [rsp + 104], rax
                        mov              qword ptr [rsp + 112], rax
                        mov              qword ptr [rsp + 120], rax
                        mov              qword ptr [rsp + 128], rax
                        mov              qword ptr [rsp + 136], rax
                        mov              qword ptr [rsp + 144], rax
                        mov              qword ptr [rsp + 152], rax
                        mov              qword ptr [rsp + 160], rax
                        mov              qword ptr [rsp + 168], rax
                        mov              qword ptr [rsp + 176], rax
                        mov              qword ptr [rsp + 184], rax
                        mov              qword ptr [rsp + 192], rax
                        mov              qword ptr [rsp + 200], rax
                        mov              qword ptr [rsp + 208], rax
                        mov              qword ptr [rsp + 216], rax
                        mov              qword ptr [rsp + 224], rax
                        mov              qword ptr [rsp + 232], rax
                        mov              qword ptr [rsp + 240], rax
                        mov              qword ptr [rsp + 248], rax
                        mov              qword ptr [rsp + 256], rax
                        mov              qword ptr [rsp + 264], rax
                        mov              qword ptr [rsp + 272], rax
                        mov              qword ptr [rsp + 280], rax
                        mov              qword ptr [rsp + 288], rax
                        mov              qword ptr [rsp + 296], rax
                        mov              qword ptr [rsp + 304], rax
                        mov              qword ptr [rsp + 312], rax
                        mov              qword ptr [rsp + 320], rax
                        mov              qword ptr [rsp + 328], rax
                        mov              qword ptr [rsp + 336], rax
                        mov              qword ptr [rsp + 344], rax
                        mov              qword ptr [rsp + 352], rax
                        mov              qword ptr [rsp + 360], rax
                        mov              qword ptr [rsp + 368], rax
                        mov              qword ptr [rsp + 376], rax
                        mov              qword ptr [rsp + 384], rax
                        mov              qword ptr [rsp + 392], rax
                        mov              qword ptr [rsp + 400], rax
                        mov              qword ptr [rsp + 408], rax
                        mov              qword ptr [rsp + 416], rax
                        mov              qword ptr [rsp + 424], rax
                        mov              qword ptr [rsp + 432], rax
                        mov              qword ptr [rsp + 440], rax
                        mov              qword ptr [rsp + 448], rax
                        mov              qword ptr [rsp + 456], rax
                        mov              qword ptr [rsp + 464], rax
                        mov              qword ptr [rsp + 472], rax
                        mov              qword ptr [rsp + 480], rax
                        mov              qword ptr [rsp + 488], rax
                        mov              qword ptr [rsp + 496], rax
                        mov              qword ptr [rsp + 504], rax
                        mov              qword ptr [rsp + 512], rax
                        mov              qword ptr [rsp + 520], rax
                        mov              qword ptr [rsp + 528], rax
                        mov              qword ptr [rsp + 536], rax
                        mov              qword ptr [rsp + 544], rax
                        mov              qword ptr [rsp + 552], rax
                        mov              dword ptr [rsp + 560], r14d
                        mov              dword ptr [rsp + 564], r14d
                        mov              dword ptr [rsp + 568], 1
                        mov              rax, qword ptr [rsp + 616]
                        mov              qword ptr [rsp + 576], rax
                        mov              rax, qword ptr [rsp + 624]
                        mov              qword ptr [rsp + 584], rax
                                                                                        jmp   n10_lit_integer_α
n9_match_arbno_β:
                        mov              r14d, dword ptr [rsp + 564]
                        mov              rax, qword ptr [rsp + 576]
                        mov              rcx, qword ptr [rsp + 584]
                        sub              rsp, 592
                        mov              edx, 0
                        mov              qword ptr [rsp + 0], rdx
                        mov              qword ptr [rsp + 8], rdx
                        mov              qword ptr [rsp + 16], rdx
                        mov              qword ptr [rsp + 24], rdx
                        mov              qword ptr [rsp + 32], rdx
                        mov              qword ptr [rsp + 40], rdx
                        mov              qword ptr [rsp + 48], rdx
                        mov              qword ptr [rsp + 56], rdx
                        mov              qword ptr [rsp + 64], rdx
                        mov              qword ptr [rsp + 72], rdx
                        mov              qword ptr [rsp + 80], rdx
                        mov              qword ptr [rsp + 88], rdx
                        mov              qword ptr [rsp + 96], rdx
                        mov              qword ptr [rsp + 104], rdx
                        mov              qword ptr [rsp + 112], rdx
                        mov              qword ptr [rsp + 120], rdx
                        mov              qword ptr [rsp + 128], rdx
                        mov              qword ptr [rsp + 136], rdx
                        mov              qword ptr [rsp + 144], rdx
                        mov              qword ptr [rsp + 152], rdx
                        mov              qword ptr [rsp + 160], rdx
                        mov              qword ptr [rsp + 168], rdx
                        mov              qword ptr [rsp + 176], rdx
                        mov              qword ptr [rsp + 184], rdx
                        mov              qword ptr [rsp + 192], rdx
                        mov              qword ptr [rsp + 200], rdx
                        mov              qword ptr [rsp + 208], rdx
                        mov              qword ptr [rsp + 216], rdx
                        mov              qword ptr [rsp + 224], rdx
                        mov              qword ptr [rsp + 232], rdx
                        mov              qword ptr [rsp + 240], rdx
                        mov              qword ptr [rsp + 248], rdx
                        mov              qword ptr [rsp + 256], rdx
                        mov              qword ptr [rsp + 264], rdx
                        mov              qword ptr [rsp + 272], rdx
                        mov              qword ptr [rsp + 280], rdx
                        mov              qword ptr [rsp + 288], rdx
                        mov              qword ptr [rsp + 296], rdx
                        mov              qword ptr [rsp + 304], rdx
                        mov              qword ptr [rsp + 312], rdx
                        mov              qword ptr [rsp + 320], rdx
                        mov              qword ptr [rsp + 328], rdx
                        mov              qword ptr [rsp + 336], rdx
                        mov              qword ptr [rsp + 344], rdx
                        mov              qword ptr [rsp + 352], rdx
                        mov              qword ptr [rsp + 360], rdx
                        mov              qword ptr [rsp + 368], rdx
                        mov              qword ptr [rsp + 376], rdx
                        mov              qword ptr [rsp + 384], rdx
                        mov              qword ptr [rsp + 392], rdx
                        mov              qword ptr [rsp + 400], rdx
                        mov              qword ptr [rsp + 408], rdx
                        mov              qword ptr [rsp + 416], rdx
                        mov              qword ptr [rsp + 424], rdx
                        mov              qword ptr [rsp + 432], rdx
                        mov              qword ptr [rsp + 440], rdx
                        mov              qword ptr [rsp + 448], rdx
                        mov              qword ptr [rsp + 456], rdx
                        mov              qword ptr [rsp + 464], rdx
                        mov              qword ptr [rsp + 472], rdx
                        mov              qword ptr [rsp + 480], rdx
                        mov              qword ptr [rsp + 488], rdx
                        mov              qword ptr [rsp + 496], rdx
                        mov              qword ptr [rsp + 504], rdx
                        mov              qword ptr [rsp + 512], rdx
                        mov              qword ptr [rsp + 520], rdx
                        mov              qword ptr [rsp + 528], rdx
                        mov              qword ptr [rsp + 536], rdx
                        mov              qword ptr [rsp + 544], rdx
                        mov              qword ptr [rsp + 552], rdx
                        mov              dword ptr [rsp + 560], r14d
                        mov              dword ptr [rsp + 564], r14d
                        mov              dword ptr [rsp + 568], 0
                        mov              qword ptr [rsp + 576], rax
                        mov              qword ptr [rsp + 584], rcx
                                                                                        jmp   n18_match_alternate_α
n9_match_arbno_as:
                        mov              eax, dword ptr [rbp + -48]
                        cmp              r14d, eax
                                                                                        je    n18_match_alternate_β
                        mov              dword ptr [rbp + -44], r14d
                                                                                        jmp   n10_lit_integer_α
n9_match_arbno_af:
                        mov              eax, dword ptr [rsp + 568]
                        test             eax, eax
                                                                                        jnz   .Lx41_2
                        add              rsp, 592
                                                                                        jmp   n18_match_alternate_β
.Lx41_2:
                        mov              r14d, dword ptr [rsp + 560]
                        add              rsp, 592
                                                                                        jmp   n8_match_pos_β
#-----------------------------------------------------------------------------------------------------------------------
n10_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx42_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n11_match_rpos_α
n10_lit_integer_β:
                        add              rsp, 16
                                                                                        jmp   n9_match_arbno_β
.Lx42_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n11_match_rpos_α:
                        mov              rax, 0
                        mov              ecx, r15d
                        sub              ecx, eax
                        cmp              r14d, ecx
                                                                                        jne   n9_match_arbno_β
                                                                                        jmp   n12_match_end_α
n11_match_rpos_β:
                                                                                        jmp   n9_match_arbno_β
#-----------------------------------------------------------------------------------------------------------------------
n12_match_end_α:
                        mov              r10, r12
.Lx45_8:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx45_8
                        mov              rsp, qword ptr [r10 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r10, rsi
.Lx45_5:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx45_5
                        lea              rdi, [r10 + 24]
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx45_1:
                        test             rax, rax
                                                                                        je    .Lx45_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx45_3]
                        lea              rdx, [rip + .Lx45_4]
                                                                                        jmp   rax
.Lx45_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx45_1
.Lx45_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx45_1
.Lx45_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        sub              r12, 24                                        # cas_mark
                        sub              rsp, 80
                        mov              r13, qword ptr [rsp + 48]                      # outer_Σ
                        mov              r14, qword ptr [rsp + 56]                      # outer_δ
                        mov              r15, qword ptr [rsp + 64]                      # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rsp + 40]                      # old_rbp
                        add              rsp, 80
                                                                                        jmp   n13_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n13_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n14_statement_begin_α
n13_statement_end_β:
                        add              rsp, 64
                                                                                        jmp   n21_statement_begin_α
#=======================================================================================================================
# YES     OUTPUT = 'grow ok'                          :(END)
#-----------------------------------------------------------------------------------------------------------------------
n14_statement_begin_α:
                                                                                        jmp   n15_lit_string_α
n14_statement_begin_β:
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n15_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 7
                        mov              rax, qword ptr [rip + .Lx50_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n16_assign_α
n15_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   main_γ
.Lx50_0:
                        .quad            .Lx50_0_s
.Lx50_0_s:
                        .string          "grow ok"
#-----------------------------------------------------------------------------------------------------------------------
n16_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx51_0]                 # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n17_statement_end_α
n16_assign_β:
                        add              rsp, 16
                                                                                        jmp   main_γ
.Lx51_0:
                        .quad            .Lx51_0_s
.Lx51_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n17_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   main_γ
n17_statement_end_β:
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n18_match_alternate_α:
                        mov              dword ptr [rbp + 224], r14d
                        lea              rax, [rip + .Lx55_21]
                        mov              qword ptr [rbp + 240], rax
                                                                                        jmp   n20_match_len_α
.Lx55_21:
                        lea              rax, [rip + .Lx55_19]
                        mov              qword ptr [rbp + 240], rax
                                                                                        jmp   n19_match_len_α
n18_match_alternate_s0:
                        lea              rax, [rip + .Lx55_40]
                        mov              qword ptr [rbp + 232], rax
                                                                                        jmp   n18_match_alternate_as
n18_match_alternate_s1:
                        lea              rax, [rip + .Lx55_41]
                        mov              qword ptr [rbp + 232], rax
                                                                                        jmp   n18_match_alternate_as
.Lx55_40:
                                                                                        jmp   n20_match_len_β
.Lx55_41:
                                                                                        jmp   n19_match_len_β
n18_match_alternate_as:
                                                                                        jmp   n9_match_arbno_as
n18_match_alternate_β:
                        mov              rax, qword ptr [rbp + 232]
                                                                                        jmp   rax
n18_match_alternate_af:
                        mov              r14d, dword ptr [rbp + 224]
                        mov              rax, qword ptr [rbp + 240]
                                                                                        jmp   rax
.Lx55_19:
                                                                                        jmp   n9_match_arbno_af
#-----------------------------------------------------------------------------------------------------------------------
n19_match_len_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jg    n18_match_alternate_af
                        add              r14d, 1
                                                                                        jmp   n18_match_alternate_s1
n19_match_len_β:
                        sub              r14d, 1
                                                                                        jmp   n18_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n20_match_len_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jg    n18_match_alternate_af
                        add              r14d, 1
                                                                                        jmp   n18_match_alternate_s0
n20_match_len_β:
                        sub              r14d, 1
                                                                                        jmp   n18_match_alternate_af
#=======================================================================================================================
# NO      OUTPUT = 'fail'
#-----------------------------------------------------------------------------------------------------------------------
n21_statement_begin_α:
                                                                                        jmp   n22_lit_string_α
n21_statement_begin_β:
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n22_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx60_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n23_assign_α
n22_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   main_γ
.Lx60_0:
                        .quad            .Lx60_0_s
.Lx60_0_s:
                        .string          "fail"
#-----------------------------------------------------------------------------------------------------------------------
n23_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx61_0]                 # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n24_statement_end_α
n23_assign_β:
                        add              rsp, 16
                                                                                        jmp   main_γ
.Lx61_0:
                        .quad            .Lx61_0_s
.Lx61_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n24_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   main_γ
n24_statement_end_β:
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n25_goto_α:
                                                                                        jmp   n14_statement_begin_α
n25_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n26_goto_α:
                                                                                        jmp   n21_statement_begin_α
n26_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              rsp, rbp
                        pop              rbp
                        xor              edi, edi
                        call             exit@PLT
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              rsp, rbp
                        pop              rbp
                        mov              edi, 1
                        call             exit@PLT
                        .section         .note.GNU-stack,"",@progbits

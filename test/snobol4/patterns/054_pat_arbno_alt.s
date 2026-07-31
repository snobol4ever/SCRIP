                        .intel_syntax    noprefix
                        .text
                        .section         .rodata
.Lgvan0:                .string          "X"
.Lgvan1:                .string          "V"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .quad            .Lgvan1
                        .section         .text
                        .intel_syntax    noprefix
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
                        sub              rsp, 392
                        mov              rdi, rsp
                        mov              ecx, 392
                        xor              eax, eax
                        rep stosb
                        mov              [rsp + 384], rbp
                        mov              rbp, rsp
main_α_body:
#=======================================================================================================================
#         X = 'abba'
#-----------------------------------------------------------------------------------------------------------------------
n0_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx22_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n1_assign_α
n0_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n2_var_α
.Lx22_0:
                        .quad            .Lx22_0_s
.Lx22_0_s:
                        .string          "abba"
#-----------------------------------------------------------------------------------------------------------------------
n1_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   n2_var_α
n1_assign_β:
                        add              rsp, 16
                                                                                        jmp   n2_var_α
#=======================================================================================================================
#         X POS(0) ARBNO('a' | 'b') . V RPOS(0)                     :S(YES)F(NO)
#-----------------------------------------------------------------------------------------------------------------------
n2_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rbp + 336], rax
                        mov              qword ptr [rbp + 344], rdx
                                                                                        jmp   n3_match_head_α
n2_var_β:
                                                                                        jmp   n18_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n3_match_head_α:
                        mov              qword ptr [rbp + 96], r13
                        mov              qword ptr [rbp + 104], r14
                        mov              qword ptr [rbp + 112], r15
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rbp + 120], rax
                        mov              qword ptr [rbp + 88], rbp
                        mov              rdi, qword ptr [rbp + 336]
                        mov              rsi, qword ptr [rbp + 344]
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r10, qword ptr [1879048192]
                        mov              qword ptr [r10 + 0], 0
                        add              r10, 24
                        mov              qword ptr [1879048192], r10
                        mov              rax, rsp
                        sub              rsp, 32
                        mov              qword ptr [rsp + 16], rax
                        lea              rcx, [rip + g_patstk_sp]
                        mov              rax, qword ptr [rcx + 0]
                        mov              qword ptr [rsp + 8], rax
                        mov              dword ptr [rsp + 0], 0
.Lx26_0:
                        mov              r14d, dword ptr [rsp + 0]
                                                                                        jmp   n4_match_sequence_α
n3_match_head_β:
                        add              dword ptr [rsp + 0], 1
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d
                                                                                        jg    .Lx26_1
                        lea              rcx, [rip + g_anchor]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx26_1
                                                                                        jmp   .Lx26_0
.Lx26_1:
                        mov              rax, qword ptr [rsp + 8]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rsp + 16]
                        mov              r10, qword ptr [1879048192]
.Lx26_2:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx26_2
                        mov              qword ptr [1879048192], r10
                        mov              r13, qword ptr [rbp + 96]
                        mov              r14, qword ptr [rbp + 104]
                        mov              r15, qword ptr [rbp + 112]
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rbp + 120]
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + 88]
                                                                                        jmp   n18_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n4_match_sequence_α:
                                                                                        jmp   n16_lit_integer_α
n4_match_sequence_as:
                                                                                        jmp   n5_match_release_α
n4_match_sequence_β:
                                                                                        jmp   n9_match_rpos_β
n4_match_sequence_af:
                                                                                        jmp   n3_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n5_match_release_α:
                        mov              rax, qword ptr [rsp + 672]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rsp + 680]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, qword ptr [1879048192]
                        mov              r10, rsi
.Lx30_5:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx30_5
                        lea              rdi, [r10 + 24]
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx30_1:
                        test             rax, rax
                                                                                        je    .Lx30_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx30_3]
                        lea              rdx, [rip + .Lx30_4]
                                                                                        jmp   rax
.Lx30_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx30_1
.Lx30_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx30_1
.Lx30_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r10, qword ptr [1879048192]
.Lx30_6:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx30_6
                        mov              qword ptr [1879048192], r10
                        mov              r13, qword ptr [rbp + 96]
                        mov              r14, qword ptr [rbp + 104]
                        mov              r15, qword ptr [rbp + 112]
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rbp + 120]
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + 88]
                                                                                        jmp   n6_var_α
#=======================================================================================================================
# YES     OUTPUT = V
#-----------------------------------------------------------------------------------------------------------------------
n6_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n7_assign_α
n6_var_β:
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n7_assign_α:
                        mov              rsi, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx32_0]
                        call             NV_SET_fn@PLT
                        add              rsp, 16
                                                                                        jmp   main_γ
n7_assign_β:
                        add              rsp, 16
                                                                                        jmp   main_γ
.Lx32_0:
                        .quad            .Lx32_0_s
.Lx32_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n8_lit_integer_α:
                        mov              qword ptr [rbp + 320], 6
                        mov              rax, qword ptr [rip + .Lx33_0]
                        mov              qword ptr [rbp + 328], rax
                                                                                        jmp   n9_match_rpos_α
n8_lit_integer_β:
                                                                                        jmp   n12_match_assign_cond_β
.Lx33_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n9_match_rpos_α:
                        mov              rax, 0
                        mov              ecx, r15d
                        sub              ecx, eax
                        cmp              r14d, ecx
                                                                                        jne   n12_match_assign_cond_β
                                                                                        jmp   n5_match_release_α
n9_match_rpos_β:
                                                                                        jmp   n12_match_assign_cond_β
#-----------------------------------------------------------------------------------------------------------------------
n10_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                                                                                        jmp   n11_match_arbno_α
n10_match_assign_save_β:
                        add              rsp, 16
                                                                                        jmp   n3_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n11_match_arbno_α:
                        sub              rsp, 48
                        sub              rsp, 704
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
                        mov              qword ptr [rsp + 560], rax
                        mov              qword ptr [rsp + 568], rax
                        mov              qword ptr [rsp + 576], rax
                        mov              qword ptr [rsp + 584], rax
                        mov              qword ptr [rsp + 592], rax
                        mov              qword ptr [rsp + 600], rax
                        mov              qword ptr [rsp + 608], rax
                        mov              qword ptr [rsp + 616], rax
                        mov              qword ptr [rsp + 624], rax
                        mov              qword ptr [rsp + 632], rax
                        mov              qword ptr [rsp + 640], rax
                        mov              qword ptr [rsp + 648], rax
                        mov              qword ptr [rsp + 696], rax
                        mov              dword ptr [rsp + 656], r14d
                        mov              dword ptr [rsp + 660], r14d
                        mov              dword ptr [rsp + 664], 1
                        mov              rax, qword ptr [rsp + 728]
                        mov              qword ptr [rsp + 672], rax
                        mov              rax, qword ptr [rsp + 736]
                        mov              qword ptr [rsp + 680], rax
                        mov              rax, qword ptr [rsp + 704]
                        mov              qword ptr [rsp + 688], rax
                        add              rsp, 48
                                                                                        jmp   n12_match_assign_cond_α
n11_match_arbno_β:
                        sub              rsp, 48
                        mov              r14d, dword ptr [rsp + 660]
                        mov              rax, qword ptr [rsp + 672]
                        mov              rcx, qword ptr [rsp + 680]
                        sub              rsp, 704
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
                        mov              qword ptr [rsp + 560], rdx
                        mov              qword ptr [rsp + 568], rdx
                        mov              qword ptr [rsp + 576], rdx
                        mov              qword ptr [rsp + 584], rdx
                        mov              qword ptr [rsp + 592], rdx
                        mov              qword ptr [rsp + 600], rdx
                        mov              qword ptr [rsp + 608], rdx
                        mov              qword ptr [rsp + 616], rdx
                        mov              qword ptr [rsp + 624], rdx
                        mov              qword ptr [rsp + 632], rdx
                        mov              qword ptr [rsp + 640], rdx
                        mov              qword ptr [rsp + 648], rdx
                        mov              dword ptr [rsp + 656], r14d
                        mov              dword ptr [rsp + 660], r14d
                        mov              dword ptr [rsp + 664], 0
                        mov              qword ptr [rsp + 672], rax
                        mov              qword ptr [rsp + 680], rcx
                        mov              qword ptr [rsp + 696], rdx
                        mov              rax, qword ptr [rsp + 1392]
                        mov              qword ptr [rsp + 688], rax
                                                                                        jmp   n13_match_alternate_α
n11_match_arbno_as:
                        sub              rsp, 48
                        mov              eax, dword ptr [rsp + 656]
                        cmp              r14d, eax
                                                                                        je    n13_match_alternate_β
                        mov              dword ptr [rsp + 660], r14d
                        add              rsp, 48
                                                                                        jmp   n12_match_assign_cond_α
n11_match_arbno_af:
                        sub              rsp, 48
                        mov              eax, dword ptr [rsp + 664]
                        test             eax, eax
                                                                                        jnz   .Lx38_2
                        add              rsp, 704
                                                                                        jmp   n13_match_alternate_β
.Lx38_2:
                        mov              r14d, dword ptr [rsp + 656]
                        add              rsp, 704
                        add              rsp, 48
                                                                                        jmp   n10_match_assign_save_β
#-----------------------------------------------------------------------------------------------------------------------
n12_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 688]
                        lea              rcx, [rip + .S0]
                        mov              r10, qword ptr [1879048192]
                        mov              qword ptr [r10 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r10 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r10 + 16], rdx
                        add              r10, 24
                        mov              qword ptr [1879048192], r10
                                                                                        jmp   n8_lit_integer_α
n12_match_assign_cond_β:
                        mov              rax, qword ptr [1879048192]
                        sub              rax, 24
                        mov              qword ptr [1879048192], rax
                                                                                        jmp   n11_match_arbno_β
#-----------------------------------------------------------------------------------------------------------------------
n13_match_alternate_α:
                        sub              rsp, 48
                        mov              dword ptr [rbp + 272], r14d
                        lea              rax, [rip + .Lx42_21]
                        mov              qword ptr [rbp + 288], rax
                                                                                        jmp   n15_match_lit_α
.Lx42_21:
                        lea              rax, [rip + .Lx42_19]
                        mov              qword ptr [rbp + 288], rax
                                                                                        jmp   n14_match_lit_α
n13_match_alternate_s0:
                        sub              rsp, 48
                        lea              rax, [rip + .Lx42_40]
                        mov              qword ptr [rbp + 280], rax
                                                                                        jmp   n13_match_alternate_as
n13_match_alternate_s1:
                        sub              rsp, 48
                        lea              rax, [rip + .Lx42_41]
                        mov              qword ptr [rbp + 280], rax
                                                                                        jmp   n13_match_alternate_as
.Lx42_40:
                                                                                        jmp   n15_match_lit_β
.Lx42_41:
                                                                                        jmp   n14_match_lit_β
n13_match_alternate_as:
                        sub              rsp, 48
                        add              rsp, 48
                                                                                        jmp   n11_match_arbno_as
n13_match_alternate_β:
                        sub              rsp, 48
                        mov              rax, qword ptr [rbp + 280]
                                                                                        jmp   rax
n13_match_alternate_af:
                        sub              rsp, 48
                        mov              r14d, dword ptr [rbp + 272]
                        mov              rax, qword ptr [rbp + 288]
                                                                                        jmp   rax
.Lx42_19:
                        add              rsp, 48
                                                                                        jmp   n11_match_arbno_af
#-----------------------------------------------------------------------------------------------------------------------
n14_match_lit_α:
                        sub              rsp, 16
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jle   .Lx44_239
                        add              rsp, 16
                                                                                        jmp   n13_match_alternate_af
.Lx44_239:
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 98
                                                                                        je    .Lx44_240
                        add              rsp, 16
                                                                                        jmp   n13_match_alternate_af
.Lx44_240:
                        add              r14d, 1
                        add              rsp, 16
                                                                                        jmp   n13_match_alternate_s1
n14_match_lit_β:
                        sub              rsp, 16
                        sub              r14d, 1
                        add              rsp, 16
                                                                                        jmp   n13_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n15_match_lit_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jg    n13_match_alternate_af
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 97
                                                                                        jne   n13_match_alternate_af
                        add              r14d, 1
                                                                                        jmp   n13_match_alternate_s0
n15_match_lit_β:
                        sub              r14d, 1
                                                                                        jmp   n13_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n16_lit_integer_α:
                        mov              qword ptr [rbp + 144], 6
                        mov              rax, qword ptr [rip + .Lx47_0]
                        mov              qword ptr [rbp + 152], rax
                                                                                        jmp   n17_match_pos_α
n16_lit_integer_β:
                                                                                        jmp   n3_match_head_β
.Lx47_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n17_match_pos_α:
                        mov              rax, 0
                        cmp              r14d, eax
                                                                                        jne   n3_match_head_β
                                                                                        jmp   n10_match_assign_save_α
n17_match_pos_β:
                                                                                        jmp   n3_match_head_β
#=======================================================================================================================
# NO      OUTPUT = 'no match'
#-----------------------------------------------------------------------------------------------------------------------
n18_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 8
                        mov              rax, qword ptr [rip + .Lx49_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n19_assign_α
n18_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   main_γ
.Lx49_0:
                        .quad            .Lx49_0_s
.Lx49_0_s:
                        .string          "no match"
#-----------------------------------------------------------------------------------------------------------------------
n19_assign_α:
                        mov              rsi, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx50_0]
                        call             NV_SET_fn@PLT
                        add              rsp, 16
                                                                                        jmp   main_γ
n19_assign_β:
                        add              rsp, 16
                                                                                        jmp   main_γ
.Lx50_0:
                        .quad            .Lx50_0_s
.Lx50_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n20_goto_α:
                                                                                        jmp   n6_var_α
n20_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n21_goto_α:
                                                                                        jmp   n18_lit_string_α
n21_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        mov              rsp, rbp
                        mov              rbp, [rsp + 384]
                        add              rsp, 392
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              rsp, rbp
                        mov              eax, 99
                        xor              edx, edx
                        mov              rbp, [rsp + 384]
                        add              rsp, 392
                        ret
                        .section         .rodata
.S0:                    .string          "V"
                        .text
                        .section         .note.GNU-stack,"",@progbits

                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$0_α
proc_PAT$0_α:
                        sub              rsp, 160
                        mov              qword ptr [rsp + 136], rcx
                        mov              qword ptr [rsp + 144], rdx
                        mov              qword ptr [rsp + 152], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp + 128], r8
                        mov              dword ptr [rsp + 120], r14d
proc_PAT$0_attempt:
proc_PAT$0_α_body:
                        lea              rax, [rip + n4_match_sequence_β]
                        mov              qword ptr [rbp + 96], rax
#-----------------------------------------------------------------------------------------------------------------------
n0_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052384]                    # PAT$0$A1
                        mov              rdx, qword ptr [1879052392]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n1_coerce_string_α
n0_var_β:
                        add              rsp, 16
                                                                                        jmp   proc_PAT$0_scanfail
#-----------------------------------------------------------------------------------------------------------------------
n1_coerce_string_α:
                        sub              rsp, 16
                        lea              rdi, [rsp + 16]                                # var
                        lea              rsi, [rsp + 0]                                 # result
                        mov              rdx, 12320956                                  # codes
                        call             rt_coerce_str_d@PLT
                                                                                        jmp   n2_var_α
n1_coerce_string_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   proc_PAT$0_scanfail
#-----------------------------------------------------------------------------------------------------------------------
n2_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052368]                    # PAT$0$A0
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n3_coerce_string_α
n2_var_β:
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   proc_PAT$0_scanfail
#-----------------------------------------------------------------------------------------------------------------------
n3_coerce_string_α:
                        sub              rsp, 16
                        lea              rdi, [rsp + 16]                                # var
                        lea              rsi, [rsp + 0]                                 # result
                        mov              rdx, 4522053                                   # codes
                        call             rt_coerce_str_d@PLT
                                                                                        jmp   n4_match_sequence_α
n3_coerce_string_β:
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   proc_PAT$0_scanfail
#-----------------------------------------------------------------------------------------------------------------------
n4_match_sequence_α:
                                                                                        jmp   n6_match_break_α
n4_match_sequence_as:
                                                                                        jmp   proc_PAT$0_γ
n4_match_sequence_β:
                                                                                        jmp   n5_match_span_β
n4_match_sequence_af:
                                                                                        jmp   proc_PAT$0_ω
#-----------------------------------------------------------------------------------------------------------------------
n5_match_span_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], 0
                        mov              r8, qword ptr [rbp + 72]
                        mov              r9d, dword ptr [rbp + 68]
.Lx16_0:
                        mov              eax, r14d
                        add              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d
                                                                                        jge   .Lx16_1
                        movsxd           rcx, eax
                        movzx            esi, byte ptr [r13+rcx]
                        mov              edx, 0
.Lx16_2:
                        cmp              edx, r9d
                                                                                        jge   .Lx16_1
                        movzx            edi, byte ptr [r8 + rdx]
                        cmp              esi, edi
                                                                                        je    .Lx16_3
                        add              edx, 1
                                                                                        jmp   .Lx16_2
.Lx16_3:
                        add              dword ptr [rsp + 0], 1
                                                                                        jmp   .Lx16_0
.Lx16_1:
                        mov              eax, dword ptr [rsp + 0]
                        test             eax, eax
                                                                                        jg    .Lx16_240
                        add              rsp, 16
                                                                                        jmp   n6_match_break_β
.Lx16_240:
                        mov              edx, r14d
                        mov              dword ptr [rsp + 4], edx
                        add              edx, eax
                        mov              r14d, edx
                                                                                        jmp   proc_PAT$0_γ
n5_match_span_β:
                        mov              r14d, dword ptr [rsp + 4]
                        add              rsp, 16
                                                                                        jmp   n6_match_break_β
#-----------------------------------------------------------------------------------------------------------------------
n6_match_break_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], 0
                        mov              r8, qword ptr [rbp + 40]
                        mov              r9d, dword ptr [rbp + 36]
.Lx18_0:
                        mov              eax, r14d
                        add              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d
                                                                                        jl    .Lx18_240
                        add              rsp, 16
                                                                                        jmp   proc_PAT$0_ω
.Lx18_240:
                        movsxd           rcx, eax
                        movzx            esi, byte ptr [r13+rcx]
                        mov              edx, 0
.Lx18_2:
                        cmp              edx, r9d
                                                                                        jge   .Lx18_3
                        movzx            edi, byte ptr [r8 + rdx]
                        cmp              esi, edi
                                                                                        je    .Lx18_1
                        add              edx, 1
                                                                                        jmp   .Lx18_2
.Lx18_3:
                        add              dword ptr [rsp + 0], 1
                                                                                        jmp   .Lx18_0
.Lx18_1:
                        mov              eax, r14d
                        add              eax, dword ptr [rsp + 0]
                        mov              dword ptr [rsp + 0], r14d
                        mov              r14d, eax
                                                                                        jmp   n5_match_span_α
n6_match_break_β:
                        mov              r14d, dword ptr [rsp + 0]
                        add              rsp, 16
                                                                                        jmp   proc_PAT$0_ω
proc_PAT$0_scanhit:
                        cmp              qword ptr [rbp + 128], 1
                                                                                        jne   .Lx19_0
                        mov              ecx, dword ptr [rbp + 120]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx + 0], ecx
.Lx19_0:
                                                                                        jmp   proc_PAT$0_γ
proc_PAT$0_scanfail:
                        cmp              qword ptr [rbp + 128], 1
                                                                                        jne   .Lx20_0
                        mov              eax, dword ptr [rbp + 120]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    .Lx20_0
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        cmp              qword ptr [rcx + 0], 0
                                                                                        jne   .Lx20_0
                        mov              dword ptr [rbp + 120], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$0_attempt
.Lx20_0:
                                                                                        jmp   proc_PAT$0_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_β:
                                                                                        jmp   qword ptr [rbp + 96]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$0_res]
                        push             rax
                        mov              rax, qword ptr [rbp + 136]
                        mov              rbp, qword ptr [rbp + 152]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_ω:
                        mov              rax, qword ptr [rbp + 144]
                        lea              rsp, [rbp + 160]
                        mov              rbp, qword ptr [rbp + 152]
                                                                                        jmp   rax
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lstartup_pname0:       .string          "PAT$0"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_PAT$0_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 112
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_zstatic@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        add              rsp, 8
                        ret
                        .section         .rodata
.Lgvan0:                .string          "NUMERALS"
.Lgvan1:                .string          "WORD"
.Lgvan2:                .string          "WPAT"
.Lgvan3:                .string          "LINE"
.Lgvan4:                .string          "N"
.Lgvan5:                .string          "PAT$0$A0"
.Lgvan6:                .string          "PAT$0$A1"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .quad            .Lgvan1
                        .quad            .Lgvan2
                        .quad            .Lgvan3
                        .quad            .Lgvan4
                        .quad            .Lgvan5
                        .quad            .Lgvan6
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             proc_startup
                        mov              edi, 7
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 7
                        call             gva_register@PLT
                        xor              esi, esi
                                                                                        jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 8
#=======================================================================================================================
#       &TRIM    =  1
#-----------------------------------------------------------------------------------------------------------------------
n21_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx61_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n22_lit_integer_α
.Lx61_0:
                        .quad            .Lx61_0_s
.Lx61_0_s:
                        .string          "TRIM"
#-----------------------------------------------------------------------------------------------------------------------
n22_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx62_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n23_call_α
.Lx62_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n23_call_α:
                        sub              rsp, 16
                        sub              rsp, 32
                        mov              r10, qword ptr [rsp + 64]
                        mov              r11, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        mov              r10, qword ptr [rsp + 48]
                        mov              r11, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        .section         .rodata
.Lrkfnzd64:             .string          "SNO$KWSET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd64]                        # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx63_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n24_lit_string_α
.Lx63_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        add              rsp, 48
                                                                                        jmp   n24_lit_string_α
n23_call_β:
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n24_lit_string_α
#=======================================================================================================================
#       NUMERALS =  '0123456789'
#-----------------------------------------------------------------------------------------------------------------------
n24_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 10
                        mov              rax, qword ptr [rip + .Lx65_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n25_assign_α
.Lx65_0:
                        .quad            .Lx65_0_s
.Lx65_0_s:
                        .string          "0123456789"
#-----------------------------------------------------------------------------------------------------------------------
n25_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax                    # NUMERALS
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   n26_lit_string_α
#=======================================================================================================================
#       WORD     =  "'-" NUMERALS &UCASE &LCASE
#-----------------------------------------------------------------------------------------------------------------------
n26_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx67_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n27_var_α
.Lx67_0:
                        .quad            .Lx67_0_s
.Lx67_0_s:
                        .string          "'-"
#-----------------------------------------------------------------------------------------------------------------------
n27_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052288]                    # NUMERALS
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n28_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n28_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # lit_string
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # var
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n29_keyword_snobol4_α
#-----------------------------------------------------------------------------------------------------------------------
n29_keyword_snobol4_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx70_0]                 # sval
                        call             rt_keyword_read_snobol4@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n30_binop_α
.Lx70_0:
                        .quad            .Lx70_0_s
.Lx70_0_s:
                        .string          "UCASE"
#-----------------------------------------------------------------------------------------------------------------------
n30_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # binop
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # keyword_snobol4
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n31_keyword_snobol4_α
#-----------------------------------------------------------------------------------------------------------------------
n31_keyword_snobol4_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx72_0]                 # sval
                        call             rt_keyword_read_snobol4@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n32_binop_α
.Lx72_0:
                        .quad            .Lx72_0_s
.Lx72_0_s:
                        .string          "LCASE"
#-----------------------------------------------------------------------------------------------------------------------
n32_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # binop
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # keyword_snobol4
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n33_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n33_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax                    # WORD
                        mov              qword ptr [1879052312], rdx
                        add              rsp, 112
                                                                                        jmp   n34_var_α
#=======================================================================================================================
#       WPAT     =  BREAK(WORD) SPAN(WORD)
#-----------------------------------------------------------------------------------------------------------------------
n34_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # WORD
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n35_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n35_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052384], rax                    # PAT$0$A1
                        mov              qword ptr [1879052392], rdx
                                                                                        jmp   n36_var_α
#-----------------------------------------------------------------------------------------------------------------------
n36_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # WORD
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n37_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n37_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052368], rax                    # PAT$0$A0
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n38_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n38_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx79_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n39_call_α
.Lx79_0:
                        .quad            .Lx79_0_s
.Lx79_0_s:
                        .string          "PAT$0"
#-----------------------------------------------------------------------------------------------------------------------
n39_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd81:             .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd81]                        # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx80_240
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n41_var_α
.Lx80_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n40_assign_α
n39_call_β:
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n41_var_α
#-----------------------------------------------------------------------------------------------------------------------
n40_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052320], rax                    # WPAT
                        mov              qword ptr [1879052328], rdx
                        add              rsp, 64
                                                                                        jmp   n41_var_α
#=======================================================================================================================
# NEXTL LINE     =  INPUT                            :F(DONE)
#-----------------------------------------------------------------------------------------------------------------------
n41_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx83_0]                 # name
                        call             NV_GET_fn@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx83_240
                        add              rsp, 16
                                                                                        jmp   n53_var_α
.Lx83_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n42_assign_α
.Lx83_0:
                        .quad            .Lx83_0_s
.Lx83_0_s:
                        .string          "INPUT"
#-----------------------------------------------------------------------------------------------------------------------
n42_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052336], rax                    # LINE
                        mov              qword ptr [1879052344], rdx
                        add              rsp, 16
                                                                                        jmp   n43_var_α
#=======================================================================================================================
# NEXTW LINE     ?  WPAT =                           :F(NEXTL)
#-----------------------------------------------------------------------------------------------------------------------
n43_var_α:
                        sub              rsp, 160
                        mov              qword ptr [rsp + 0], 0                         # stmt_claim
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 16], 0
                        mov              qword ptr [rsp + 24], 0
                        mov              qword ptr [rsp + 32], 0
                        mov              qword ptr [rsp + 40], 0
                        mov              qword ptr [rsp + 48], 0
                        mov              qword ptr [rsp + 56], 0
                        mov              qword ptr [rsp + 64], 0
                        mov              qword ptr [rsp + 72], 0
                        mov              qword ptr [rsp + 80], 0
                        mov              qword ptr [rsp + 88], 0
                        mov              qword ptr [rsp + 96], 0
                        mov              qword ptr [rsp + 104], 0
                        mov              qword ptr [rsp + 112], 0
                        mov              qword ptr [rsp + 120], 0
                        mov              qword ptr [rsp + 128], 0
                        mov              qword ptr [rsp + 136], 0
                        mov              qword ptr [rsp + 144], 0
                        mov              qword ptr [rsp + 152], 0
                        mov              rax, qword ptr [1879052336]                    # LINE
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 144], rax                     # result
                        mov              qword ptr [rsp + 152], rdx
                                                                                        jmp   n44_match_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n44_match_begin_α:
                        mov              qword ptr [rsp + 56], rbp                      # old_rbp
                        mov              rbp, rsp                                       # stmt_base
                        mov              qword ptr [rbp + 64], r13                      # outer_Σ
                        mov              qword ptr [rbp + 72], r14                      # outer_δ
                        mov              qword ptr [rbp + 80], r15                      # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rbp + 88], rax                      # cap_gen
                        mov              rdi, qword ptr [rsp + 144]                     # lo
                        mov              rsi, qword ptr [rsp + 152]                     # hi
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r10, qword ptr [1879048192]                    # cas_top
                        mov              qword ptr [r10 + 0], 0
                        mov              qword ptr [r10 + 8], rsp                       # cas_rsp_mark
                        lea              rcx, [rip + g_patstk_sp]
                        mov              rax, qword ptr [rcx + 0]
                        mov              qword ptr [r10 + 16], rax                      # cas_patstk
                        add              r10, 24
                        mov              qword ptr [1879048192], r10                    # cas_top
                        mov              qword ptr [rbp + 32], rsp                      # zls2_mark
                        lea              rcx, [rip + g_patstk_sp]
                        mov              rax, qword ptr [rcx + 0]
                        mov              qword ptr [rbp + 24], rax                      # patstk_mark
                        mov              dword ptr [rbp + 16], 0                        # start_δ
.Lx87_0:
                        mov              r14d, dword ptr [rbp + 16]
                                                                                        jmp   n45_match_patref_α
n44_match_begin_β:
                        add              dword ptr [rbp + 16], 1
                        mov              eax, dword ptr [rbp + 16]
                        cmp              eax, r15d
                                                                                        jg    .Lx87_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx87_1
                                                                                        jmp   .Lx87_0
.Lx87_1:
                        mov              rax, qword ptr [rbp + 24]                      # patstk_mark
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rbp + 32]
                        mov              r10, qword ptr [1879048192]
.Lx87_2:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx87_2
                        mov              qword ptr [1879048192], r10
                        mov              r13, qword ptr [rbp + 64]                      # outer_Σ
                        mov              r14, qword ptr [rbp + 72]                      # outer_δ
                        mov              r15, qword ptr [rbp + 80]                      # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        mov              rdx, qword ptr [rbp + 88]                      # cap_gen
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + 56]                      # old_rbp
                        add              rsp, 160
                                                                                        jmp   n41_var_α
#-----------------------------------------------------------------------------------------------------------------------
n45_match_patref_α:
                        lea              rsi, [rip + g_sno_defer_cells+0]
                        mov              rax, qword ptr [rsi + 0]
                        test             rax, rax
                                                                                        jne   .Lx88_11
                        mov              rax, qword ptr [1879052320]                    # WPAT
                        mov              rdx, qword ptr [1879052328]
                        cmp              eax, 8
                                                                                        jne   .Lx88_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx88_10
                        mov              rdi, rdx                                       # headv
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx88_10
.Lx88_9:
                        xor              eax, eax
.Lx88_10:
                        lea              rsi, [rip + g_sno_defer_cells+0]
                        mov              qword ptr [rsi + 0], rax
.Lx88_11:
                        test             rax, rax
                                                                                        jz    .Lx88_0
                        mov              r8d, 1
                        lea              rcx, [rip + .Lx88_4]
                        lea              rdx, [rip + .Lx88_5]
                                                                                        jmp   rax
.Lx88_4:
                        lea              rcx, [rip + g_scan_hit_start]
                        mov              rax, qword ptr [rcx]
                        mov              dword ptr [rbp + 16], eax
                                                                                        jmp   n46_match_end_α
.Lx88_5:
                                                                                        jmp   n44_match_begin_β
.Lx88_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S0]                               # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx88_2:
                        test             rax, rax
                                                                                        je    .Lx88_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx88_7]
                        lea              rdx, [rip + .Lx88_8]
                                                                                        jmp   rax
.Lx88_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx88_2
.Lx88_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx88_2
.Lx88_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n44_match_begin_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx88_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n46_match_end_α
.Lx88_6:
                        add              rsp, 16
                                                                                        jmp   n44_match_begin_β
n45_match_patref_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n46_match_end_α:
                        mov              rax, qword ptr [rbp + 24]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              qword ptr [rbp + 40], r14
                        mov              rsp, qword ptr [rbp + 32]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, qword ptr [1879048192]
                        mov              r10, rsi
.Lx90_5:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx90_5
                        lea              rdi, [r10 + 24]
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx90_1:
                        test             rax, rax
                                                                                        je    .Lx90_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx90_3]
                        lea              rdx, [rip + .Lx90_4]
                                                                                        jmp   rax
.Lx90_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx90_1
.Lx90_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx90_1
.Lx90_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r10, qword ptr [1879048192]
.Lx90_6:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx90_6
                        mov              qword ptr [1879048192], r10
                        mov              r13, qword ptr [rbp + 64]                      # outer_Σ
                        mov              r14, qword ptr [rbp + 72]                      # outer_δ
                        mov              r15, qword ptr [rbp + 80]                      # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        mov              rdx, qword ptr [rbp + 88]                      # cap_gen
                        call             rt_match_ctx_restore@PLT
                                                                                        jmp   n47_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n47_lit_string_α:
                        mov              qword ptr [rsp + 112], 2                       # result
                        mov              dword ptr [rsp + 116], 0
                        mov              rax, qword ptr [rip + .Lx91_0]
                        mov              qword ptr [rsp + 120], rax
                                                                                        jmp   n48_match_replace_α
.Lx91_0:
                        .quad            .Lx91_0_s
.Lx91_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n48_match_replace_α:
                        mov              rdi, qword ptr [rip + .Lx93_0]                 # name
                        mov              rsi, qword ptr [rbp + 144]                     # sub_lo
                        mov              rdx, qword ptr [rbp + 152]                     # sub_hi
                        mov              ecx, dword ptr [rbp + 16]                      # start
                        mov              r8, qword ptr [rbp + 40]                       # end
                        lea              r9, [rbp + 112]                                # replp
                        call             rt_match_replace@PLT
                                                                                        jmp   .Lx93_1
.Lx93_0:
                        .quad            .Lx93_0_s
.Lx93_0_s:
                        .string          "LINE"
.Lx93_1:
                        mov              rbp, qword ptr [rbp + 56]                      # old_rbp
                        add              rsp, 160
                                                                                        jmp   n49_var_α
#=======================================================================================================================
#       N        =  N + 1                            :(NEXTW)
#-----------------------------------------------------------------------------------------------------------------------
n49_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # N
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n50_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n50_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx95_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n51_binop_α
.Lx95_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n51_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_add@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx96_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n43_var_α
.Lx96_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n52_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n52_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052352], rax                    # N
                        mov              qword ptr [1879052360], rdx
                        add              rsp, 48
                                                                                        jmp   n43_var_α
#=======================================================================================================================
# DONE  OUTPUT   =  +N ' words'
#-----------------------------------------------------------------------------------------------------------------------
n53_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # N
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n54_unop_α
#-----------------------------------------------------------------------------------------------------------------------
n54_unop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # var
                        mov              rsi, qword ptr [rsp + 24]                      # a
                        call             rt_num_pos@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n55_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n55_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx100_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n56_binop_α
.Lx100_0:
                        .quad            .Lx100_0_s
.Lx100_0_s:
                        .string          " words"
#-----------------------------------------------------------------------------------------------------------------------
n56_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # unop
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n57_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n57_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx102_0]                # name
                        call             NV_SET_fn@PLT
                        add              rsp, 64
                                                                                        jmp   main_γ
.Lx102_0:
                        .quad            .Lx102_0_s
.Lx102_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n58_goto_α:
                                                                                        jmp   n41_var_α
n58_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n59_goto_α:
                                                                                        jmp   n43_var_α
n59_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n60_goto_α:
                                                                                        jmp   n53_var_α
n60_goto_β:
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
                        .section         .rodata
.S0:                    .string          "WPAT"
                        .text
                        .section         .note.GNU-stack,"",@progbits

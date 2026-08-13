                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_init_α:
proc_LBL__stack_init_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:                                               jmp   n1_lit_string_α
n0_statement_begin_β:                                               jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n1_lit_string_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx6_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n2_assign_α
.Lx6_0:                 .quad            .Lx6_0_s
.Lx6_0_s:               .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n2_assign_α:            mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 144], rax                      # stk
                        mov              qword ptr [r9 + 152], rdx; jmp   n3_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n3_statement_end_α:     add              rsp, 16;                   jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_init_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_init_β:
                                                                    jmp   proc_LBL__stack_init_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_init_γ:
                                                                    jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_init_ω:
                                                                    jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_push_α:
proc_LBL__stack_push_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n10_statement_begin_α:                                              jmp   n11_var_α
n10_statement_begin_β:                                              jmp   n16_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n11_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]                      # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n12_var_α
#-----------------------------------------------------------------------------------------------------------------------
n12_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]                       # x
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n13_call_α
n12_var_β:              add              rsp, 16
                        add              rsp, 16;                   jmp   n10_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n13_call_α:             sub              rsp, 16
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
.Lrkfnzd26:             .string          "slink"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd26]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 32
                        cmp              eax, 104;                  jne   .Lx25_240
                        add              rsp, 16;                   jmp   n12_var_β
.Lx25_240:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n14_assign_α
n13_call_β:             add              rsp, 16;                   jmp   n12_var_β
#-----------------------------------------------------------------------------------------------------------------------
n14_assign_α:           mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 144], rax                      # stk
                        mov              qword ptr [r9 + 152], rdx; jmp   n15_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n15_statement_end_α:    add              rsp, 48;                   jmp   n16_statement_begin_α
#=======================================================================================================================
#         stack_push(w1)
#-----------------------------------------------------------------------------------------------------------------------
n16_statement_begin_α:                                              jmp   n17_var_α
n16_statement_begin_β:                                              jmp   NRETURN
#-----------------------------------------------------------------------------------------------------------------------
n17_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]                      # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n18_field_var_α
#-----------------------------------------------------------------------------------------------------------------------
n18_field_var_α:        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx33_0]
                        mov              rsi, qword ptr [rsp + 16]                      # var
                        mov              rdx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_field_var@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        cmp              eax, 104;                  jne   .Lx33_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n16_statement_begin_β
.Lx33_240:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n19_assign_α
.Lx33_0:                .quad            .Lx33_0_s
.Lx33_0_s:              .string          "sval"
#-----------------------------------------------------------------------------------------------------------------------
n19_assign_α:           mov              rax, qword ptr [rsp + 0]                       # field_var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 16], rax                       # stack_push
                        mov              qword ptr [r9 + 24], rdx;  jmp   n20_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n20_statement_end_α:    add              rsp, 32;                   jmp   NRETURN
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_push_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_push_β:
                                                                    jmp   proc_LBL__stack_push_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_push_γ:
                                                                    jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_push_ω:
                                                                    jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_pop_α:
proc_LBL__stack_pop_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n37_statement_begin_α:                                              jmp   n38_var_α
n37_statement_begin_β:                                              jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n38_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]                      # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n39_call_α
#-----------------------------------------------------------------------------------------------------------------------
n39_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd76:             .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd76]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 16
                        cmp              eax, 104;                  jne   .Lx75_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n37_statement_begin_β
.Lx75_240:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n40_statement_end_α
n39_call_β:             add              rsp, 16
                        add              rsp, 16;                   jmp   n37_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n40_statement_end_α:                                                jmp   n41_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n41_statement_begin_α:                                              jmp   n42_var_α
n41_statement_begin_β:  add              rsp, 32;                   jmp   n55_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n42_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 64]                       # var
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n43_call_α
n42_var_β:              add              rsp, 16
                        add              rsp, 32;                   jmp   n41_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n43_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd83:             .string          "IDENT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd83]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 16
                        cmp              eax, 104;                  jne   .Lx82_240
                        add              rsp, 16;                   jmp   n42_var_β
.Lx82_240:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n44_statement_end_α
n43_call_β:             add              rsp, 16;                   jmp   n42_var_β
#-----------------------------------------------------------------------------------------------------------------------
n44_statement_end_α:                                                jmp   n45_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n45_statement_begin_α:                                              jmp   n46_var_α
n45_statement_begin_β:                                              jmp   n50_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n46_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]                      # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n47_call_α
n46_var_β:              add              rsp, 16
                        add              rsp, 64;                   jmp   n45_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n47_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd90:             .string          "sval"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd90]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 16
                        cmp              eax, 104;                  jne   .Lx89_240
                        add              rsp, 16;                   jmp   n46_var_β
.Lx89_240:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n48_assign_α
n47_call_β:             add              rsp, 16;                   jmp   n46_var_β
#-----------------------------------------------------------------------------------------------------------------------
n48_assign_α:           mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 48], rax                       # stack_pop
                        mov              qword ptr [r9 + 56], rdx;  jmp   n49_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n49_statement_end_α:                                                jmp   n50_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n50_statement_begin_α:                                              jmp   n51_var_α
n50_statement_begin_β:  add              rsp, 96;                   jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n51_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]                      # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n52_call_α
n51_var_β:              add              rsp, 16
                        add              rsp, 96;                   jmp   n50_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n52_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd98:             .string          "snext"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd98]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 16
                        cmp              eax, 104;                  jne   .Lx97_240
                        add              rsp, 16;                   jmp   n51_var_β
.Lx97_240:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n53_assign_α
n52_call_β:             add              rsp, 16;                   jmp   n51_var_β
#-----------------------------------------------------------------------------------------------------------------------
n53_assign_α:           mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 144], rax                      # stk
                        mov              qword ptr [r9 + 152], rdx; jmp   n54_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n54_statement_end_α:    add              rsp, 128;                  jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n55_statement_begin_α:                                              jmp   n56_var_α
n55_statement_begin_β:                                              jmp   n62_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n56_var_α:              mov              rax, qword ptr [r9 + 64]                       # var
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 672], rax                     # result
                        mov              qword ptr [rsp + 680], rdx; jmp   n57_call_α
#-----------------------------------------------------------------------------------------------------------------------
n57_call_α:             mov              rax, qword ptr [rsp + 672]
                        mov              qword ptr [rsp + 640], rax
                        mov              rax, qword ptr [rsp + 680]
                        mov              qword ptr [rsp + 648], rax
                        .section         .rodata
.Lrkfn106:              .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn106]
                        lea              rsi, [rsp + 640]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 624], rax
                        mov              qword ptr [rsp + 632], rdx
                        cmp              eax, 104;                  je    n55_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n58_var_α
n57_call_β:                                                         jmp   n55_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n58_var_α:              mov              rax, qword ptr [r9 + 144]                      # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 736], rax                     # result
                        mov              qword ptr [rsp + 744], rdx; jmp   n59_call_α
#-----------------------------------------------------------------------------------------------------------------------
n59_call_α:             mov              rax, qword ptr [rsp + 736]
                        mov              qword ptr [rsp + 704], rax
                        mov              rax, qword ptr [rsp + 744]
                        mov              qword ptr [rsp + 712], rax
                        .section         .rodata
.Lrkfn109:              .string          "sval"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn109]
                        lea              rsi, [rsp + 704]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 688], rax
                        mov              qword ptr [rsp + 696], rdx
                        cmp              eax, 104;                  je    n55_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n60_assign_var_α
n59_call_β:                                                         jmp   n55_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n60_assign_var_α:       mov              rdi, qword ptr [rsp + 624]
                        mov              rsi, qword ptr [rsp + 632]
                        mov              rdx, qword ptr [rsp + 688]
                        mov              rcx, qword ptr [rsp + 696]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_assign_var@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        cmp              eax, 104;                  je    n55_statement_begin_β
                        mov              qword ptr [rsp + 752], rax
                        mov              qword ptr [rsp + 760], rdx; jmp   n61_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n61_statement_end_α:                                                jmp   n62_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n62_statement_begin_α:                                              jmp   n63_var_α
n62_statement_begin_β:                                              jmp   n67_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n63_var_α:              mov              rax, qword ptr [r9 + 144]                      # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 816], rax                     # result
                        mov              qword ptr [rsp + 824], rdx; jmp   n64_call_α
#-----------------------------------------------------------------------------------------------------------------------
n64_call_α:             mov              rax, qword ptr [rsp + 816]
                        mov              qword ptr [rsp + 784], rax
                        mov              rax, qword ptr [rsp + 824]
                        mov              qword ptr [rsp + 792], rax
                        .section         .rodata
.Lrkfn117:              .string          "snext"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn117]
                        lea              rsi, [rsp + 784]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 768], rax
                        mov              qword ptr [rsp + 776], rdx
                        cmp              eax, 104;                  je    n62_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n65_assign_α
n64_call_β:                                                         jmp   n62_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n65_assign_α:           mov              rax, qword ptr [rsp + 768]
                        mov              rdx, qword ptr [rsp + 776]
                        mov              qword ptr [r9 + 144], rax                      # stk
                        mov              qword ptr [r9 + 152], rdx; jmp   n66_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n66_statement_end_α:                                                jmp   n67_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n67_statement_begin_α:                                              jmp   n68_lit_string_α
n67_statement_begin_β:                                              jmp   NRETURN
#-----------------------------------------------------------------------------------------------------------------------
n68_lit_string_α:       mov              qword ptr [rsp + 880], 2                       # result
                        mov              dword ptr [rsp + 884], 5
                        mov              rax, qword ptr [rip + .Lx123_0]
                        mov              qword ptr [rsp + 888], rax; jmp   n69_call_α
.Lx123_0:               .quad            .Lx123_0_s
.Lx123_0_s:             .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n69_call_α:             mov              rax, qword ptr [rsp + 880]
                        mov              qword ptr [rsp + 848], rax
                        mov              rax, qword ptr [rsp + 888]
                        mov              qword ptr [rsp + 856], rax
                        .section         .rodata
.Lrkfn125:              .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn125]
                        lea              rsi, [rsp + 848]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 832], rax
                        mov              qword ptr [rsp + 840], rdx
                        cmp              eax, 104;                  je    n67_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n70_assign_α
n69_call_β:                                                         jmp   n67_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n70_assign_α:           mov              rax, qword ptr [rsp + 832]
                        mov              rdx, qword ptr [rsp + 840]
                        mov              qword ptr [r9 + 48], rax                       # stack_pop
                        mov              qword ptr [r9 + 56], rdx;  jmp   n71_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n71_statement_end_α:                                                jmp   NRETURN
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_pop_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_pop_β:
                                                                    jmp   proc_LBL__stack_pop_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_pop_γ:
                                                                    jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_pop_ω:
                                                                    jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_peek_α:
proc_LBL__stack_peek_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n129_statement_begin_α:                                             jmp   n130_var_α
n129_statement_begin_β:                                             jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n130_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]                      # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n131_call_α
#-----------------------------------------------------------------------------------------------------------------------
n131_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd142:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd142]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 16
                        cmp              eax, 104;                  jne   .Lx141_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n129_statement_begin_β
.Lx141_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n132_statement_end_α
n131_call_β:            add              rsp, 16
                        add              rsp, 16;                   jmp   n129_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n132_statement_end_α:                                               jmp   n133_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n133_statement_begin_α:                                             jmp   n134_var_α
n133_statement_begin_β: add              rsp, 32;                   jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n134_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]                      # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n135_call_α
n134_var_β:             add              rsp, 16
                        add              rsp, 32;                   jmp   n133_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n135_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd149:            .string          "sval"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd149]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 16
                        cmp              eax, 104;                  jne   .Lx148_240
                        add              rsp, 16;                   jmp   n134_var_β
.Lx148_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n136_assign_α
n135_call_β:            add              rsp, 16;                   jmp   n134_var_β
#-----------------------------------------------------------------------------------------------------------------------
n136_assign_α:          mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 80], rax                       # stack_peek
                        mov              qword ptr [r9 + 88], rdx;  jmp   n137_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n137_statement_end_α:   add              rsp, 64;                   jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_peek_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_peek_β:
                                                                    jmp   proc_LBL__stack_peek_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_peek_γ:
                                                                    jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_peek_ω:
                                                                    jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_top_α:
proc_LBL__stack_top_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n153_statement_begin_α:                                             jmp   n154_var_α
n153_statement_begin_β:                                             jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n154_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]                      # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n155_call_α
#-----------------------------------------------------------------------------------------------------------------------
n155_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd166:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd166]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 16
                        cmp              eax, 104;                  jne   .Lx165_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n153_statement_begin_β
.Lx165_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n156_statement_end_α
n155_call_β:            add              rsp, 16
                        add              rsp, 16;                   jmp   n153_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n156_statement_end_α:                                               jmp   n157_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n157_statement_begin_α:                                             jmp   n158_var_α
n157_statement_begin_β: add              rsp, 32;                   jmp   NRETURN
#-----------------------------------------------------------------------------------------------------------------------
n158_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]                      # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n159_field_var_α
n158_var_β:             add              rsp, 16
                        add              rsp, 32;                   jmp   n157_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n159_field_var_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx172_0]
                        mov              rsi, qword ptr [rsp + 16]                      # var
                        mov              rdx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_field_var@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        cmp              eax, 104;                  jne   .Lx172_240
                        add              rsp, 16;                   jmp   n158_var_β
.Lx172_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n160_assign_α
.Lx172_0:               .quad            .Lx172_0_s
.Lx172_0_s:             .string          "sval"
#-----------------------------------------------------------------------------------------------------------------------
n160_assign_α:          mov              rax, qword ptr [rsp + 0]                       # field_var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 96], rax                       # stack_top
                        mov              qword ptr [r9 + 104], rdx; jmp   n161_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n161_statement_end_α:   add              rsp, 64;                   jmp   NRETURN
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_top_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_top_β:
                                                                    jmp   proc_LBL__stack_top_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_top_γ:
                                                                    jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_top_ω:
                                                                    jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_depth_α:
proc_LBL__stack_depth_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n176_statement_begin_α:                                             jmp   n177_lit_integer_α
n176_statement_begin_β:                                             jmp   n180_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n177_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx201_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n178_assign_α
.Lx201_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n178_assign_α:          mov              rax, qword ptr [rsp + 0]                       # lit_integer
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 112], rax                      # stack_depth
                        mov              qword ptr [r9 + 120], rdx; jmp   n179_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n179_statement_end_α:                                               jmp   n180_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n180_statement_begin_α:                                             jmp   n181_var_α
n180_statement_begin_β:                                             jmp   n184_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n181_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]                      # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n182_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n182_assign_α:          mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 128], rax                      # sd
                        mov              qword ptr [r9 + 136], rdx; jmp   n183_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n183_statement_end_α:                                               jmp   n184_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n184_statement_begin_α:                                             jmp   n185_var_α
n184_statement_begin_β: add              rsp, 32;                   jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n185_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 128]                      # sd
                        mov              rdx, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n186_call_α
n185_var_β:             add              rsp, 16
                        add              rsp, 32;                   jmp   n184_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n186_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd215:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd215]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 16
                        cmp              eax, 104;                  jne   .Lx214_240
                        add              rsp, 16;                   jmp   n185_var_β
.Lx214_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n187_statement_end_α
n186_call_β:            add              rsp, 16;                   jmp   n185_var_β
#-----------------------------------------------------------------------------------------------------------------------
n187_statement_end_α:                                               jmp   n188_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n188_statement_begin_α:                                             jmp   n189_var_α
n188_statement_begin_β:                                             jmp   n194_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n189_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 112]                      # stack_depth
                        mov              rdx, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n190_lit_integer_α
n189_var_β:             add              rsp, 16
                        add              rsp, 64;                   jmp   n188_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n190_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx221_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n191_binop_α
n190_lit_integer_β:     add              rsp, 16;                   jmp   n189_var_β
.Lx221_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n191_binop_α:           sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_add@PLT
                        cmp              eax, 104;                  jne   .Lx222_240
                        add              rsp, 16;                   jmp   n190_lit_integer_β
.Lx222_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n192_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n192_assign_α:          mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 112], rax                      # stack_depth
                        mov              qword ptr [r9 + 120], rdx; jmp   n193_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n193_statement_end_α:                                               jmp   n194_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n194_statement_begin_α:                                             jmp   n195_var_α
n194_statement_begin_β:                                             jmp   n184_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n195_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 128]                      # sd
                        mov              rdx, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n196_call_α
n195_var_β:             add              rsp, 16
                        add              rsp, 112;                  jmp   n194_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n196_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd230:            .string          "snext"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd230]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 16
                        cmp              eax, 104;                  jne   .Lx229_240
                        add              rsp, 16;                   jmp   n195_var_β
.Lx229_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n197_assign_α
n196_call_β:            add              rsp, 16;                   jmp   n195_var_β
#-----------------------------------------------------------------------------------------------------------------------
n197_assign_α:          mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 128], rax                      # sd
                        mov              qword ptr [r9 + 136], rdx; jmp   n198_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n198_statement_end_α:   add              rsp, 144;                  jmp   n184_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_depth_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_depth_β:
                                                                    jmp   proc_LBL__stack_depth_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_depth_γ:
                                                                    jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__stack_depth_ω:
                                                                    jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_stack_init_α
proc_stack_init_α:
proc_stack_init_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n234_save_restore_α:                                                jmp   n235_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n235_goto_deferred_α:   lea              rax, [rip + proc_LBL__stack_init_α]; jmp   rax
                                                                    jmp   n236_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n236_save_restore_α:
stack_init_alpha:       mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 0
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 0;                    jbe   .Lx242_2
                        mov              eax, 0
.Lx242_2:               sub              rsp, 48
                        add              rsp, rax
                        mov              rax, qword ptr [r9 + 0]                        # stack_init
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 0], 0
                        mov              qword ptr [r9 + 8], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        lea              r10, [rip + stack_init_gamma]
                        lea              r11, [rip + stack_init_omega]
                        lea              rax, [rip + proc_LBL__stack_init_α]; jmp   rax
stack_init_gamma:       mov              rdi, qword ptr [r9 + 0]
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 0;                    jbe   .Lx242_75
                        mov              eax, 0
.Lx242_75:              lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 80
                        cmp              rcx, 0;                    jbe   .Lx242_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx242_3:               mov              rcx, qword ptr [r8 + 16]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
stack_init_omega:       mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 0;                    jbe   .Lx242_145
                        mov              eax, 0
.Lx242_145:             lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 80
                        cmp              rcx, 0;                    jbe   .Lx242_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx242_4:               mov              rcx, qword ptr [r8 + 24]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
                                                                    jmp   proc_stack_init_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_init_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_init_β:
                                                                    jmp   proc_stack_init_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_init_γ:
                                                                    jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_init_ω:
                                                                    jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_stack_push_α
proc_stack_push_α:
proc_stack_push_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n243_save_restore_α:                                                jmp   n244_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n244_goto_deferred_α:   lea              rax, [rip + proc_LBL__stack_push_α]; jmp   rax
                                                                    jmp   n245_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n245_save_restore_α:
stack_push_alpha:       mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 16
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx251_2
                        mov              eax, 16
.Lx251_2:               sub              rsp, 64
                        add              rsp, rax
                        mov              rax, qword ptr [r9 + 16]                       # stack_push
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        cmp              rcx, 0;                    jbe   .Lx251_10
                        mov              rax, qword ptr [r8 + 48]
                        mov              rdx, qword ptr [r9 + 32]                       # x
                        mov              qword ptr [r9 + 32], rax
                        mov              qword ptr [r8 + 48], rdx
                        mov              rax, qword ptr [r8 + 56]
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [r9 + 40], rax
                        mov              qword ptr [r8 + 56], rdx;  jmp   .Lx251_41
.Lx251_10:              mov              rax, qword ptr [r9 + 32]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 32], 0
                        mov              qword ptr [r9 + 40], 0
.Lx251_41:              lea              r10, [rip + stack_push_gamma]
                        lea              r11, [rip + stack_push_omega]
                        lea              rax, [rip + proc_LBL__stack_push_α]; jmp   rax
stack_push_gamma:       mov              rdi, qword ptr [r9 + 16]                       # stack_push
                        mov              rsi, qword ptr [r9 + 24]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx251_75
                        mov              eax, 16
.Lx251_75:              lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 24], rax
                        cmp              rcx, 0;                    jbe   .Lx251_80
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 32], rax                       # x
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 40], rax;  jmp   .Lx251_110
.Lx251_80:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 40], rax
.Lx251_110:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 96
                        cmp              rcx, 1;                    jbe   .Lx251_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx251_3:               mov              rcx, qword ptr [r8 + 32]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
stack_push_omega:       mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx251_145
                        mov              eax, 16
.Lx251_145:             lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 16], rax                       # stack_push
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 24], rax
                        cmp              rcx, 0;                    jbe   .Lx251_150
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 32], rax                       # x
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 40], rax;  jmp   .Lx251_180
.Lx251_150:             mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 40], rax
.Lx251_180:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 96
                        cmp              rcx, 1;                    jbe   .Lx251_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx251_4:               mov              rcx, qword ptr [r8 + 40]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
                                                                    jmp   proc_stack_push_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_push_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_push_β:
                                                                    jmp   proc_stack_push_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_push_γ:
                                                                    jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_push_ω:
                                                                    jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_stack_pop_α
proc_stack_pop_α:
proc_stack_pop_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n252_save_restore_α:                                                jmp   n253_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n253_goto_deferred_α:   lea              rax, [rip + proc_LBL__stack_pop_α]; jmp   rax
                                                                    jmp   n254_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n254_save_restore_α:
stack_pop_alpha:        mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 16
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx260_2
                        mov              eax, 16
.Lx260_2:               sub              rsp, 64
                        add              rsp, rax
                        mov              rax, qword ptr [r9 + 48]                       # stack_pop
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 48], 0
                        mov              qword ptr [r9 + 56], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        cmp              rcx, 0;                    jbe   .Lx260_10
                        mov              rax, qword ptr [r8 + 48]
                        mov              rdx, qword ptr [r9 + 64]                       # var
                        mov              qword ptr [r9 + 64], rax
                        mov              qword ptr [r8 + 48], rdx
                        mov              rax, qword ptr [r8 + 56]
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [r9 + 72], rax
                        mov              qword ptr [r8 + 56], rdx;  jmp   .Lx260_41
.Lx260_10:              mov              rax, qword ptr [r9 + 64]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 72]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 64], 0
                        mov              qword ptr [r9 + 72], 0
.Lx260_41:              lea              r10, [rip + stack_pop_gamma]
                        lea              r11, [rip + stack_pop_omega]
                        lea              rax, [rip + proc_LBL__stack_pop_α]; jmp   rax
stack_pop_gamma:        mov              rdi, qword ptr [r9 + 48]                       # stack_pop
                        mov              rsi, qword ptr [r9 + 56]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx260_75
                        mov              eax, 16
.Lx260_75:              lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 56], rax
                        cmp              rcx, 0;                    jbe   .Lx260_80
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 64], rax                       # var
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 72], rax;  jmp   .Lx260_110
.Lx260_80:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 72], rax
.Lx260_110:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 96
                        cmp              rcx, 1;                    jbe   .Lx260_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx260_3:               mov              rcx, qword ptr [r8 + 32]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
stack_pop_omega:        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx260_145
                        mov              eax, 16
.Lx260_145:             lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 48], rax                       # stack_pop
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 56], rax
                        cmp              rcx, 0;                    jbe   .Lx260_150
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 64], rax                       # var
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 72], rax;  jmp   .Lx260_180
.Lx260_150:             mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 72], rax
.Lx260_180:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 96
                        cmp              rcx, 1;                    jbe   .Lx260_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx260_4:               mov              rcx, qword ptr [r8 + 40]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
                                                                    jmp   proc_stack_pop_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_pop_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_pop_β:
                                                                    jmp   proc_stack_pop_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_pop_γ:
                                                                    jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_pop_ω:
                                                                    jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_stack_peek_α
proc_stack_peek_α:
proc_stack_peek_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n261_save_restore_α:                                                jmp   n262_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n262_goto_deferred_α:   lea              rax, [rip + proc_LBL__stack_peek_α]; jmp   rax
                                                                    jmp   n263_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n263_save_restore_α:
stack_peek_alpha:       mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 0
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 0;                    jbe   .Lx269_2
                        mov              eax, 0
.Lx269_2:               sub              rsp, 48
                        add              rsp, rax
                        mov              rax, qword ptr [r9 + 80]                       # stack_peek
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 80], 0
                        mov              qword ptr [r9 + 88], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        lea              r10, [rip + stack_peek_gamma]
                        lea              r11, [rip + stack_peek_omega]
                        lea              rax, [rip + proc_LBL__stack_peek_α]; jmp   rax
stack_peek_gamma:       mov              rdi, qword ptr [r9 + 80]
                        mov              rsi, qword ptr [r9 + 88]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 0;                    jbe   .Lx269_75
                        mov              eax, 0
.Lx269_75:              lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 80], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 88], rax
                        mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 80
                        cmp              rcx, 0;                    jbe   .Lx269_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx269_3:               mov              rcx, qword ptr [r8 + 16]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
stack_peek_omega:       mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 0;                    jbe   .Lx269_145
                        mov              eax, 0
.Lx269_145:             lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 80], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 88], rax
                        mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 80
                        cmp              rcx, 0;                    jbe   .Lx269_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx269_4:               mov              rcx, qword ptr [r8 + 24]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
                                                                    jmp   proc_stack_peek_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_peek_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_peek_β:
                                                                    jmp   proc_stack_peek_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_peek_γ:
                                                                    jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_peek_ω:
                                                                    jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_stack_top_α
proc_stack_top_α:
proc_stack_top_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n270_save_restore_α:                                                jmp   n271_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n271_goto_deferred_α:   lea              rax, [rip + proc_LBL__stack_top_α]; jmp   rax
                                                                    jmp   n272_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n272_save_restore_α:
stack_top_alpha:        mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 0
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 0;                    jbe   .Lx278_2
                        mov              eax, 0
.Lx278_2:               sub              rsp, 48
                        add              rsp, rax
                        mov              rax, qword ptr [r9 + 96]                       # stack_top
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 96], 0
                        mov              qword ptr [r9 + 104], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        lea              r10, [rip + stack_top_gamma]
                        lea              r11, [rip + stack_top_omega]
                        lea              rax, [rip + proc_LBL__stack_top_α]; jmp   rax
stack_top_gamma:        mov              rdi, qword ptr [r9 + 96]
                        mov              rsi, qword ptr [r9 + 104]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 0;                    jbe   .Lx278_75
                        mov              eax, 0
.Lx278_75:              lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 96], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 104], rax
                        mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 80
                        cmp              rcx, 0;                    jbe   .Lx278_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx278_3:               mov              rcx, qword ptr [r8 + 16]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
stack_top_omega:        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 0;                    jbe   .Lx278_145
                        mov              eax, 0
.Lx278_145:             lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 96], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 104], rax
                        mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 80
                        cmp              rcx, 0;                    jbe   .Lx278_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx278_4:               mov              rcx, qword ptr [r8 + 24]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
                                                                    jmp   proc_stack_top_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_top_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_top_β:
                                                                    jmp   proc_stack_top_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_top_γ:
                                                                    jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_top_ω:
                                                                    jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_stack_depth_α
proc_stack_depth_α:
proc_stack_depth_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n279_save_restore_α:                                                jmp   n280_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n280_goto_deferred_α:   lea              rax, [rip + proc_LBL__stack_depth_α]; jmp   rax
                                                                    jmp   n281_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n281_save_restore_α:
stack_depth_alpha:      mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 16
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx287_2
                        mov              eax, 16
.Lx287_2:               sub              rsp, 64
                        add              rsp, rax
                        mov              rax, qword ptr [r9 + 112]                      # stack_depth
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 112], 0
                        mov              qword ptr [r9 + 120], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        cmp              rcx, 0;                    jbe   .Lx287_10
                        mov              rax, qword ptr [r8 + 48]
                        mov              rdx, qword ptr [r9 + 128]                      # sd
                        mov              qword ptr [r9 + 128], rax
                        mov              qword ptr [r8 + 48], rdx
                        mov              rax, qword ptr [r8 + 56]
                        mov              rdx, qword ptr [r9 + 136]
                        mov              qword ptr [r9 + 136], rax
                        mov              qword ptr [r8 + 56], rdx;  jmp   .Lx287_41
.Lx287_10:              mov              rax, qword ptr [r9 + 128]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 136]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 128], 0
                        mov              qword ptr [r9 + 136], 0
.Lx287_41:              lea              r10, [rip + stack_depth_gamma]
                        lea              r11, [rip + stack_depth_omega]
                        lea              rax, [rip + proc_LBL__stack_depth_α]; jmp   rax
stack_depth_gamma:      mov              rdi, qword ptr [r9 + 112]                      # stack_depth
                        mov              rsi, qword ptr [r9 + 120]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx287_75
                        mov              eax, 16
.Lx287_75:              lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 112], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 120], rax
                        cmp              rcx, 0;                    jbe   .Lx287_80
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 128], rax                      # sd
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 136], rax; jmp   .Lx287_110
.Lx287_80:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 128], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 136], rax
.Lx287_110:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 96
                        cmp              rcx, 1;                    jbe   .Lx287_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx287_3:               mov              rcx, qword ptr [r8 + 32]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
stack_depth_omega:      mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx287_145
                        mov              eax, 16
.Lx287_145:             lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 112], rax                      # stack_depth
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 120], rax
                        cmp              rcx, 0;                    jbe   .Lx287_150
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 128], rax                      # sd
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 136], rax; jmp   .Lx287_180
.Lx287_150:             mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 128], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 136], rax
.Lx287_180:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 96
                        cmp              rcx, 1;                    jbe   .Lx287_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx287_4:               mov              rcx, qword ptr [r8 + 40]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
                                                                    jmp   proc_stack_depth_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_depth_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_depth_β:
                                                                    jmp   proc_stack_depth_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_depth_γ:
                                                                    jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_stack_depth_ω:
                                                                    jmp   r11
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lclassspec0:           .string          "slink(snext,sval)"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lclassspec0]
                        call             record_register@PLT
                        .section         .rodata
.Lstartup_pname0:       .string          "LBL__stack_init"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_LBL__stack_init_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 2880
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname1:       .string          "LBL__stack_push"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname1]
                        lea              rsi, [rip + proc_LBL__stack_push_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 2880
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname2:       .string          "LBL__stack_pop"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname2]
                        lea              rsi, [rip + proc_LBL__stack_pop_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 2880
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname3:       .string          "LBL__stack_peek"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname3]
                        lea              rsi, [rip + proc_LBL__stack_peek_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 2880
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname4:       .string          "LBL__stack_top"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname4]
                        lea              rsi, [rip + proc_LBL__stack_top_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname4]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname4]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname4]
                        mov              esi, 2880
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname4]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname5:       .string          "LBL__stack_depth"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname5]
                        lea              rsi, [rip + proc_LBL__stack_depth_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 2880
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        add              rsp, 8
                        ret
                        .section         .rodata
.Lgvan0:                .string          "stack_init"
.Lgvan1:                .string          "stack_push"
.Lgvan2:                .string          "x"
.Lgvan3:                .string          "stack_pop"
.Lgvan4:                .string          "var"
.Lgvan5:                .string          "stack_peek"
.Lgvan6:                .string          "stack_top"
.Lgvan7:                .string          "stack_depth"
.Lgvan8:                .string          "sd"
.Lgvan9:                .string          "stk"
.Lgvan10:               .string          "dummy"
.Lgvan11:               .string          "myvar"
.Lgvan12:               .string          "subject"
.Lgvan13:               .string          "w1"
.Lgvan14:               .string          "w2"
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
                        .quad            .Lgvan10
                        .quad            .Lgvan11
                        .quad            .Lgvan12
                        .quad            .Lgvan13
                        .quad            .Lgvan14
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             proc_startup
                        mov              edi, 15
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 15
                        call             gva_register@PLT
                        mov              r12, qword ptr [0x70000000]
                        call             rtcc_load_all@PLT
                        xor              esi, esi
                                                                    jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n288_statement_begin_α:                                             jmp   n289_statement_end_α
n288_statement_begin_β:                                             jmp   n290_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n289_statement_end_α:                                               jmp   n290_statement_begin_α
#=======================================================================================================================
#         stack_push(99)
#-----------------------------------------------------------------------------------------------------------------------
n290_statement_begin_α:                                             jmp   n291_lit_string_α
n290_statement_begin_β:                                             jmp   n294_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n291_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 17
                        mov              rax, qword ptr [rip + .Lx527_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n292_call_α
.Lx527_0:               .quad            .Lx527_0_s
.Lx527_0_s:             .string          "slink(snext,sval)"
#-----------------------------------------------------------------------------------------------------------------------
n292_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd529:            .string          "DATA"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd529]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 16
                        cmp              eax, 104;                  jne   .Lx528_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n290_statement_begin_β
.Lx528_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n293_statement_end_α
n292_call_β:            add              rsp, 16
                        add              rsp, 16;                   jmp   n290_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n293_statement_end_α:   add              rsp, 32;                   jmp   n294_statement_begin_α
#=======================================================================================================================
#         stack_push(99)
#-----------------------------------------------------------------------------------------------------------------------
n294_statement_begin_α:                                             jmp   n295_statement_end_α
n294_statement_begin_β:                                             jmp   n296_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n295_statement_end_α:                                               jmp   n296_statement_begin_α
#=======================================================================================================================
#         OUTPUT = myvar                  ;* 99
#-----------------------------------------------------------------------------------------------------------------------
n296_statement_begin_α:                                             jmp   n297_func_activate_α
n296_statement_begin_β:                                             jmp   n299_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n297_func_activate_α:   mov              rdi, qword ptr [rip + .Lx539_0]
                        mov              rsi, qword ptr [rip + .Lx539_1]
                        mov              edx, 0
                        mov              ecx, 0
                        mov              r8d, 0
                        lea              r9, [rip + proc_stack_init_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n298_statement_end_α
n297_func_activate_β:                                               jmp   n296_statement_begin_β
.Lx539_0:               .quad            .Lx539_0_s
.Lx539_0_s:             .string          "stack_init"
.Lx539_1:               .quad            .Lx539_1_s
.Lx539_1_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n298_statement_end_α:                                               jmp   n299_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n299_statement_begin_α:                                             jmp   n300_statement_end_α
n299_statement_begin_β:                                             jmp   n301_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n300_statement_end_α:                                               jmp   n301_statement_begin_α
#=======================================================================================================================
#         subject = 'hello world'
#-----------------------------------------------------------------------------------------------------------------------
n301_statement_begin_α:                                             jmp   n302_func_activate_α
n301_statement_begin_β:                                             jmp   n304_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n302_func_activate_α:   mov              rdi, qword ptr [rip + .Lx549_0]
                        mov              rsi, qword ptr [rip + .Lx549_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + proc_stack_push_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n303_statement_end_α
n302_func_activate_β:                                               jmp   n301_statement_begin_β
.Lx549_0:               .quad            .Lx549_0_s
.Lx549_0_s:             .string          "stack_push"
.Lx549_1:               .quad            .Lx549_1_s
.Lx549_1_s:             .string          "x"
#-----------------------------------------------------------------------------------------------------------------------
n303_statement_end_α:                                               jmp   n304_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n304_statement_begin_α:                                             jmp   n305_statement_end_α
n304_statement_begin_β:                                             jmp   n306_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n305_statement_end_α:                                               jmp   n306_statement_begin_α
#=======================================================================================================================
#         OUTPUT = stack_pop()            ;* hello
#-----------------------------------------------------------------------------------------------------------------------
n306_statement_begin_α:                                             jmp   n307_func_activate_α
n306_statement_begin_β:                                             jmp   n309_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n307_func_activate_α:   mov              rdi, qword ptr [rip + .Lx559_0]
                        mov              rsi, qword ptr [rip + .Lx559_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + proc_stack_pop_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n308_statement_end_α
n307_func_activate_β:                                               jmp   n306_statement_begin_β
.Lx559_0:               .quad            .Lx559_0_s
.Lx559_0_s:             .string          "stack_pop"
.Lx559_1:               .quad            .Lx559_1_s
.Lx559_1_s:             .string          "var"
#-----------------------------------------------------------------------------------------------------------------------
n308_statement_end_α:                                               jmp   n309_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n309_statement_begin_α:                                             jmp   n310_statement_end_α
n309_statement_begin_β:                                             jmp   n311_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n310_statement_end_α:                                               jmp   n311_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n311_statement_begin_α:                                             jmp   n312_func_activate_α
n311_statement_begin_β:                                             jmp   n314_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n312_func_activate_α:   mov              rdi, qword ptr [rip + .Lx569_0]
                        mov              rsi, qword ptr [rip + .Lx569_1]
                        mov              edx, 0
                        mov              ecx, 0
                        mov              r8d, 0
                        lea              r9, [rip + proc_stack_peek_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n313_statement_end_α
n312_func_activate_β:                                               jmp   n311_statement_begin_β
.Lx569_0:               .quad            .Lx569_0_s
.Lx569_0_s:             .string          "stack_peek"
.Lx569_1:               .quad            .Lx569_1_s
.Lx569_1_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n313_statement_end_α:                                               jmp   n314_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n314_statement_begin_α:                                             jmp   n315_statement_end_α
n314_statement_begin_β:                                             jmp   n316_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n315_statement_end_α:                                               jmp   n316_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n316_statement_begin_α:                                             jmp   n317_func_activate_α
n316_statement_begin_β:                                             jmp   n319_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n317_func_activate_α:   mov              rdi, qword ptr [rip + .Lx579_0]
                        mov              rsi, qword ptr [rip + .Lx579_1]
                        mov              edx, 0
                        mov              ecx, 0
                        mov              r8d, 0
                        lea              r9, [rip + proc_stack_top_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n318_statement_end_α
n317_func_activate_β:                                               jmp   n316_statement_begin_β
.Lx579_0:               .quad            .Lx579_0_s
.Lx579_0_s:             .string          "stack_top"
.Lx579_1:               .quad            .Lx579_1_s
.Lx579_1_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n318_statement_end_α:                                               jmp   n319_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n319_statement_begin_α:                                             jmp   n320_statement_end_α
n319_statement_begin_β:                                             jmp   n321_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n320_statement_end_α:                                               jmp   n321_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n321_statement_begin_α:                                             jmp   n322_func_activate_α
n321_statement_begin_β:                                             jmp   n324_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n322_func_activate_α:   mov              rdi, qword ptr [rip + .Lx589_0]
                        mov              rsi, qword ptr [rip + .Lx589_1]
                        mov              edx, 1
                        mov              ecx, 0
                        mov              r8d, 0
                        lea              r9, [rip + proc_stack_depth_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n323_statement_end_α
n322_func_activate_β:                                               jmp   n321_statement_begin_β
.Lx589_0:               .quad            .Lx589_0_s
.Lx589_0_s:             .string          "stack_depth"
.Lx589_1:               .quad            .Lx589_1_s
.Lx589_1_s:             .string          "sd"
#-----------------------------------------------------------------------------------------------------------------------
n323_statement_end_α:                                               jmp   n324_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n324_statement_begin_α:                                             jmp   n325_statement_end_α
n324_statement_begin_β:                                             jmp   n326_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n325_statement_end_α:                                               jmp   n326_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n326_statement_begin_α:                                             jmp   n327_statement_end_α
n326_statement_begin_β:                                             jmp   n328_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n327_statement_end_α:                                               jmp   n328_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n328_statement_begin_α:                                             jmp   n329_lit_string_α
n328_statement_begin_β:                                             jmp   n333_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n329_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx602_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n330_lit_integer_α
.Lx602_0:               .quad            .Lx602_0_s
.Lx602_0_s:             .string          "TRIM"
#-----------------------------------------------------------------------------------------------------------------------
n330_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx603_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n331_call_α
n330_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                   jmp   n328_statement_begin_β
.Lx603_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n331_call_α:            sub              rsp, 16
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
.Lrkfnzd605:            .string          "SNO$KWSET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd605]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 32
                        cmp              eax, 104;                  jne   .Lx604_240
                        add              rsp, 16;                   jmp   n330_lit_integer_β
.Lx604_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n332_statement_end_α
n331_call_β:            add              rsp, 16;                   jmp   n330_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n332_statement_end_α:                                               jmp   n333_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n333_statement_begin_α:                                             jmp   n334_statement_end_α
n333_statement_begin_β:                                             jmp   n335_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n334_statement_end_α:                                               jmp   n335_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n335_statement_begin_α:                                             jmp   n336_call_α
n335_statement_begin_β:                                             jmp   n338_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n336_call_α:            sub              rsp, 16
                        sub              rsp, 32
                        mov              eax, 0
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx615_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + stack_init_alpha]; jmp   rax
.Lx615_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx615_240
                        add              rsp, 16
                        add              rsp, 48;                   jmp   n335_statement_begin_β
.Lx615_240:                                                         jmp   n337_statement_end_α
n336_call_β:                                                        jmp   n335_statement_begin_β
.Lx615_0:               .quad            .Lx615_0_s
.Lx615_0_s:             .string          "stack_init"
#-----------------------------------------------------------------------------------------------------------------------
n337_statement_end_α:                                               jmp   n338_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n338_statement_begin_α:                                             jmp   n339_lit_string_α
n338_statement_begin_β:                                             jmp   n342_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n339_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx620_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n340_call_α
n339_lit_string_β:      add              rsp, 16
                        add              rsp, 64;                   jmp   n338_statement_begin_β
.Lx620_0:               .quad            .Lx620_0_s
.Lx620_0_s:             .string          "a"
#-----------------------------------------------------------------------------------------------------------------------
n340_call_α:            sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # lit_string
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx622_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + stack_push_alpha]; jmp   rax
.Lx622_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx622_240
                        add              rsp, 16;                   jmp   n339_lit_string_β
.Lx622_240:                                                         jmp   n341_statement_end_α
n340_call_β:                                                        jmp   n339_lit_string_β
.Lx622_0:               .quad            .Lx622_0_s
.Lx622_0_s:             .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n341_statement_end_α:                                               jmp   n342_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n342_statement_begin_α:                                             jmp   n343_lit_string_α
n342_statement_begin_β:                                             jmp   n346_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n343_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx627_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n344_call_α
n343_lit_string_β:      add              rsp, 16
                        add              rsp, 96;                   jmp   n342_statement_begin_β
.Lx627_0:               .quad            .Lx627_0_s
.Lx627_0_s:             .string          "b"
#-----------------------------------------------------------------------------------------------------------------------
n344_call_α:            sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # lit_string
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx629_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + stack_push_alpha]; jmp   rax
.Lx629_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx629_240
                        add              rsp, 16;                   jmp   n343_lit_string_β
.Lx629_240:                                                         jmp   n345_statement_end_α
n344_call_β:                                                        jmp   n343_lit_string_β
.Lx629_0:               .quad            .Lx629_0_s
.Lx629_0_s:             .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n345_statement_end_α:                                               jmp   n346_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n346_statement_begin_α:                                             jmp   n347_lit_string_α
n346_statement_begin_β:                                             jmp   n350_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n347_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx634_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n348_call_α
n347_lit_string_β:      add              rsp, 16
                        add              rsp, 128;                  jmp   n346_statement_begin_β
.Lx634_0:               .quad            .Lx634_0_s
.Lx634_0_s:             .string          "c"
#-----------------------------------------------------------------------------------------------------------------------
n348_call_α:            sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # lit_string
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx636_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + stack_push_alpha]; jmp   rax
.Lx636_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx636_240
                        add              rsp, 16;                   jmp   n347_lit_string_β
.Lx636_240:                                                         jmp   n349_statement_end_α
n348_call_β:                                                        jmp   n347_lit_string_β
.Lx636_0:               .quad            .Lx636_0_s
.Lx636_0_s:             .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n349_statement_end_α:                                               jmp   n350_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n350_statement_begin_α:                                             jmp   n351_call_α
n350_statement_begin_β:                                             jmp   n354_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n351_call_α:            sub              rsp, 16
                        sub              rsp, 32
                        mov              eax, 0
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx642_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + stack_depth_alpha]; jmp   rax
.Lx642_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx642_240
                        add              rsp, 16
                        add              rsp, 160;                  jmp   n350_statement_begin_β
.Lx642_240:                                                         jmp   n352_assign_α
n351_call_β:                                                        jmp   n350_statement_begin_β
.Lx642_0:               .quad            .Lx642_0_s
.Lx642_0_s:             .string          "stack_depth"
#-----------------------------------------------------------------------------------------------------------------------
n352_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx643_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n353_statement_end_α
.Lx643_0:               .quad            .Lx643_0_s
.Lx643_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n353_statement_end_α:                                               jmp   n354_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n354_statement_begin_α:                                             jmp   n355_call_α
n354_statement_begin_β:                                             jmp   n358_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n355_call_α:            sub              rsp, 16
                        sub              rsp, 32
                        mov              eax, 0
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx649_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + stack_pop_alpha]; jmp   rax
.Lx649_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx649_240
                        add              rsp, 16
                        add              rsp, 176;                  jmp   n354_statement_begin_β
.Lx649_240:                                                         jmp   n356_assign_α
n355_call_β:                                                        jmp   n354_statement_begin_β
.Lx649_0:               .quad            .Lx649_0_s
.Lx649_0_s:             .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n356_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx650_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n357_statement_end_α
.Lx650_0:               .quad            .Lx650_0_s
.Lx650_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n357_statement_end_α:                                               jmp   n358_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n358_statement_begin_α:                                             jmp   n359_call_α
n358_statement_begin_β:                                             jmp   n362_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n359_call_α:            sub              rsp, 16
                        sub              rsp, 32
                        mov              eax, 0
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx656_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + stack_pop_alpha]; jmp   rax
.Lx656_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx656_240
                        add              rsp, 16
                        add              rsp, 192;                  jmp   n358_statement_begin_β
.Lx656_240:                                                         jmp   n360_assign_α
n359_call_β:                                                        jmp   n358_statement_begin_β
.Lx656_0:               .quad            .Lx656_0_s
.Lx656_0_s:             .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n360_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx657_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n361_statement_end_α
.Lx657_0:               .quad            .Lx657_0_s
.Lx657_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n361_statement_end_α:                                               jmp   n362_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n362_statement_begin_α:                                             jmp   n363_call_α
n362_statement_begin_β:                                             jmp   n366_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n363_call_α:            sub              rsp, 16
                        sub              rsp, 32
                        mov              eax, 0
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx663_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + stack_depth_alpha]; jmp   rax
.Lx663_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx663_240
                        add              rsp, 16
                        add              rsp, 208;                  jmp   n362_statement_begin_β
.Lx663_240:                                                         jmp   n364_assign_α
n363_call_β:                                                        jmp   n362_statement_begin_β
.Lx663_0:               .quad            .Lx663_0_s
.Lx663_0_s:             .string          "stack_depth"
#-----------------------------------------------------------------------------------------------------------------------
n364_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx664_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n365_statement_end_α
.Lx664_0:               .quad            .Lx664_0_s
.Lx664_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n365_statement_end_α:                                               jmp   n366_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n366_statement_begin_α:                                             jmp   n367_call_α
n366_statement_begin_β:                                             jmp   n370_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n367_call_α:            sub              rsp, 16
                        sub              rsp, 32
                        mov              eax, 0
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx670_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + stack_pop_alpha]; jmp   rax
.Lx670_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx670_240
                        add              rsp, 16
                        add              rsp, 224;                  jmp   n366_statement_begin_β
.Lx670_240:                                                         jmp   n368_assign_α
n367_call_β:                                                        jmp   n366_statement_begin_β
.Lx670_0:               .quad            .Lx670_0_s
.Lx670_0_s:             .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n368_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx671_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n369_statement_end_α
.Lx671_0:               .quad            .Lx671_0_s
.Lx671_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n369_statement_end_α:                                               jmp   n370_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n370_statement_begin_α:                                             jmp   n371_call_α
n370_statement_begin_β:                                             jmp   n374_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n371_call_α:            sub              rsp, 16
                        sub              rsp, 32
                        mov              eax, 0
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx677_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + stack_depth_alpha]; jmp   rax
.Lx677_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx677_240
                        add              rsp, 16
                        add              rsp, 240;                  jmp   n370_statement_begin_β
.Lx677_240:                                                         jmp   n372_assign_α
n371_call_β:                                                        jmp   n370_statement_begin_β
.Lx677_0:               .quad            .Lx677_0_s
.Lx677_0_s:             .string          "stack_depth"
#-----------------------------------------------------------------------------------------------------------------------
n372_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx678_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n373_statement_end_α
.Lx678_0:               .quad            .Lx678_0_s
.Lx678_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n373_statement_end_α:                                               jmp   n374_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n374_statement_begin_α:                                             jmp   n375_statement_end_α
n374_statement_begin_β:                                             jmp   n376_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n375_statement_end_α:                                               jmp   n376_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n376_statement_begin_α:                                             jmp   n377_call_α
n376_statement_begin_β: add              rsp, 256;                  jmp   n383_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n377_call_α:            sub              rsp, 16
                        sub              rsp, 32
                        mov              eax, 0
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx688_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + stack_pop_alpha]; jmp   rax
.Lx688_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx688_240
                        add              rsp, 16
                        add              rsp, 256;                  jmp   n376_statement_begin_β
.Lx688_240:                                                         jmp   n378_statement_end_α
n377_call_β:                                                        jmp   n376_statement_begin_β
.Lx688_0:               .quad            .Lx688_0_s
.Lx688_0_s:             .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n378_statement_end_α:                                               jmp   n379_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n379_statement_begin_α:                                             jmp   n380_lit_string_α
n379_statement_begin_β:                                             jmp   n387_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n380_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 30
                        mov              rax, qword ptr [rip + .Lx693_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n381_assign_α
.Lx693_0:               .quad            .Lx693_0_s
.Lx693_0_s:             .string          "FAIL: empty pop should FRETURN"
#-----------------------------------------------------------------------------------------------------------------------
n381_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx694_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n382_statement_end_α
.Lx694_0:               .quad            .Lx694_0_s
.Lx694_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n382_statement_end_α:                                               jmp   n387_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n383_statement_begin_α:                                             jmp   n384_lit_string_α
n383_statement_begin_β:                                             jmp   n387_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n384_lit_string_α:      mov              qword ptr [rsp + 1920], 2                      # result
                        mov              dword ptr [rsp + 1924], 8
                        mov              rax, qword ptr [rip + .Lx699_0]
                        mov              qword ptr [rsp + 1928], rax; jmp   n385_assign_α
.Lx699_0:               .quad            .Lx699_0_s
.Lx699_0_s:             .string          "empty ok"
#-----------------------------------------------------------------------------------------------------------------------
n385_assign_α:          mov              rsi, qword ptr [rsp + 1920]
                        mov              rdx, qword ptr [rsp + 1928]
                        mov              rdi, qword ptr [rip + .Lx700_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n386_statement_end_α
.Lx700_0:               .quad            .Lx700_0_s
.Lx700_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n386_statement_end_α:                                               jmp   n387_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n387_statement_begin_α:                                             jmp   n388_statement_end_α
n387_statement_begin_β:                                             jmp   n389_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n388_statement_end_α:                                               jmp   n389_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n389_statement_begin_α:                                             jmp   n390_call_α
n389_statement_begin_β:                                             jmp   n392_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n390_call_α:            sub              rsp, 16
                        sub              rsp, 32
                        mov              eax, 0
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx710_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + stack_init_alpha]; jmp   rax
.Lx710_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx710_240
                        add              rsp, 16
                        add              rsp, 288;                  jmp   n389_statement_begin_β
.Lx710_240:                                                         jmp   n391_statement_end_α
n390_call_β:                                                        jmp   n389_statement_begin_β
.Lx710_0:               .quad            .Lx710_0_s
.Lx710_0_s:             .string          "stack_init"
#-----------------------------------------------------------------------------------------------------------------------
n391_statement_end_α:                                               jmp   n392_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n392_statement_begin_α:                                             jmp   n393_lit_string_α
n392_statement_begin_β:                                             jmp   n396_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n393_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx715_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n394_call_α
n393_lit_string_β:      add              rsp, 16
                        add              rsp, 304;                  jmp   n392_statement_begin_β
.Lx715_0:               .quad            .Lx715_0_s
.Lx715_0_s:             .string          "x"
#-----------------------------------------------------------------------------------------------------------------------
n394_call_α:            sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # lit_string
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx717_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + stack_push_alpha]; jmp   rax
.Lx717_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx717_240
                        add              rsp, 16;                   jmp   n393_lit_string_β
.Lx717_240:                                                         jmp   n395_statement_end_α
n394_call_β:                                                        jmp   n393_lit_string_β
.Lx717_0:               .quad            .Lx717_0_s
.Lx717_0_s:             .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n395_statement_end_α:                                               jmp   n396_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n396_statement_begin_α:                                             jmp   n397_call_α
n396_statement_begin_β:                                             jmp   n400_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n397_call_α:            sub              rsp, 16
                        sub              rsp, 32
                        mov              eax, 0
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx723_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + stack_peek_alpha]; jmp   rax
.Lx723_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx723_240
                        add              rsp, 16
                        add              rsp, 336;                  jmp   n396_statement_begin_β
.Lx723_240:                                                         jmp   n398_assign_α
n397_call_β:                                                        jmp   n396_statement_begin_β
.Lx723_0:               .quad            .Lx723_0_s
.Lx723_0_s:             .string          "stack_peek"
#-----------------------------------------------------------------------------------------------------------------------
n398_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx724_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n399_statement_end_α
.Lx724_0:               .quad            .Lx724_0_s
.Lx724_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n399_statement_end_α:                                               jmp   n400_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n400_statement_begin_α:                                             jmp   n401_call_α
n400_statement_begin_β:                                             jmp   n404_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n401_call_α:            sub              rsp, 16
                        sub              rsp, 32
                        mov              eax, 0
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx730_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + stack_depth_alpha]; jmp   rax
.Lx730_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx730_240
                        add              rsp, 16
                        add              rsp, 352;                  jmp   n400_statement_begin_β
.Lx730_240:                                                         jmp   n402_assign_α
n401_call_β:                                                        jmp   n400_statement_begin_β
.Lx730_0:               .quad            .Lx730_0_s
.Lx730_0_s:             .string          "stack_depth"
#-----------------------------------------------------------------------------------------------------------------------
n402_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx731_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n403_statement_end_α
.Lx731_0:               .quad            .Lx731_0_s
.Lx731_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n403_statement_end_α:                                               jmp   n404_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n404_statement_begin_α:                                             jmp   n405_call_α
n404_statement_begin_β:                                             jmp   n408_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n405_call_α:            sub              rsp, 16
                        sub              rsp, 32
                        mov              eax, 0
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx737_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + stack_pop_alpha]; jmp   rax
.Lx737_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx737_240
                        add              rsp, 16
                        add              rsp, 368;                  jmp   n404_statement_begin_β
.Lx737_240:                                                         jmp   n406_assign_α
n405_call_β:                                                        jmp   n404_statement_begin_β
.Lx737_0:               .quad            .Lx737_0_s
.Lx737_0_s:             .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n406_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx738_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n407_statement_end_α
.Lx738_0:               .quad            .Lx738_0_s
.Lx738_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n407_statement_end_α:                                               jmp   n408_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n408_statement_begin_α:                                             jmp   n409_statement_end_α
n408_statement_begin_β:                                             jmp   n410_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n409_statement_end_α:                                               jmp   n410_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n410_statement_begin_α:                                             jmp   n411_call_α
n410_statement_begin_β:                                             jmp   n413_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n411_call_α:            sub              rsp, 16
                        sub              rsp, 32
                        mov              eax, 0
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx748_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + stack_init_alpha]; jmp   rax
.Lx748_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx748_240
                        add              rsp, 16
                        add              rsp, 384;                  jmp   n410_statement_begin_β
.Lx748_240:                                                         jmp   n412_statement_end_α
n411_call_β:                                                        jmp   n410_statement_begin_β
.Lx748_0:               .quad            .Lx748_0_s
.Lx748_0_s:             .string          "stack_init"
#-----------------------------------------------------------------------------------------------------------------------
n412_statement_end_α:                                               jmp   n413_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n413_statement_begin_α:                                             jmp   n414_lit_integer_α
n413_statement_begin_β:                                             jmp   n417_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n414_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx753_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n415_call_α
n414_lit_integer_β:     add              rsp, 16
                        add              rsp, 400;                  jmp   n413_statement_begin_β
.Lx753_0:               .quad            42
#-----------------------------------------------------------------------------------------------------------------------
n415_call_α:            sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # lit_integer
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx755_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + stack_push_alpha]; jmp   rax
.Lx755_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx755_240
                        add              rsp, 16;                   jmp   n414_lit_integer_β
.Lx755_240:                                                         jmp   n416_statement_end_α
n415_call_β:                                                        jmp   n414_lit_integer_β
.Lx755_0:               .quad            .Lx755_0_s
.Lx755_0_s:             .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n416_statement_end_α:                                               jmp   n417_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n417_statement_begin_α:                                             jmp   n418_lit_integer_α
n417_statement_begin_β:                                             jmp   n421_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n418_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx760_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n419_call_α
n418_lit_integer_β:     add              rsp, 16
                        add              rsp, 432;                  jmp   n417_statement_begin_β
.Lx760_0:               .quad            99
#-----------------------------------------------------------------------------------------------------------------------
n419_call_α:            sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # lit_integer
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx762_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + stack_push_alpha]; jmp   rax
.Lx762_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx762_240
                        add              rsp, 16;                   jmp   n418_lit_integer_β
.Lx762_240:                                                         jmp   n420_statement_end_α
n419_call_β:                                                        jmp   n418_lit_integer_β
.Lx762_0:               .quad            .Lx762_0_s
.Lx762_0_s:             .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n420_statement_end_α:                                               jmp   n421_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n421_statement_begin_α:                                             jmp   n422_lit_string_α
n421_statement_begin_β:                                             jmp   n425_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n422_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx767_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n423_call_α
n422_lit_string_β:      add              rsp, 16
                        add              rsp, 464;                  jmp   n421_statement_begin_β
.Lx767_0:               .quad            .Lx767_0_s
.Lx767_0_s:             .string          "myvar"
#-----------------------------------------------------------------------------------------------------------------------
n423_call_α:            sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # lit_string
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx769_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + stack_pop_alpha]; jmp   rax
.Lx769_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx769_240
                        add              rsp, 16;                   jmp   n422_lit_string_β
.Lx769_240:                                                         jmp   n424_statement_end_α
n423_call_β:                                                        jmp   n422_lit_string_β
.Lx769_0:               .quad            .Lx769_0_s
.Lx769_0_s:             .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n424_statement_end_α:                                               jmp   n425_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n425_statement_begin_α:                                             jmp   n426_var_α
n425_statement_begin_β:                                             jmp   n429_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n426_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 176]                      # myvar
                        mov              rdx, qword ptr [r9 + 184]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n427_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n427_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx775_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n428_statement_end_α
.Lx775_0:               .quad            .Lx775_0_s
.Lx775_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n428_statement_end_α:                                               jmp   n429_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n429_statement_begin_α:                                             jmp   n430_statement_end_α
n429_statement_begin_β:                                             jmp   n431_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n430_statement_end_α:                                               jmp   n431_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n431_statement_begin_α:                                             jmp   n432_call_α
n431_statement_begin_β:                                             jmp   n434_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n432_call_α:            sub              rsp, 16
                        sub              rsp, 32
                        mov              eax, 0
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx785_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + stack_init_alpha]; jmp   rax
.Lx785_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx785_240
                        add              rsp, 16
                        add              rsp, 512;                  jmp   n431_statement_begin_β
.Lx785_240:                                                         jmp   n433_statement_end_α
n432_call_β:                                                        jmp   n431_statement_begin_β
.Lx785_0:               .quad            .Lx785_0_s
.Lx785_0_s:             .string          "stack_init"
#-----------------------------------------------------------------------------------------------------------------------
n433_statement_end_α:                                               jmp   n434_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n434_statement_begin_α:                                             jmp   n435_lit_string_α
n434_statement_begin_β:                                             jmp   n438_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n435_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 11
                        mov              rax, qword ptr [rip + .Lx790_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n436_assign_α
.Lx790_0:               .quad            .Lx790_0_s
.Lx790_0_s:             .string          "hello world"
#-----------------------------------------------------------------------------------------------------------------------
n436_assign_α:          mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 192], rax                      # subject
                        mov              qword ptr [r9 + 200], rdx; jmp   n437_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n437_statement_end_α:                                               jmp   n438_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n438_statement_begin_α:                                             jmp   n439_var_α
n438_statement_begin_β: add              rsp, 544;                  jmp   n470_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n439_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 192]                      # subject
                        mov              rdx, qword ptr [r9 + 200]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n440_match_begin_α
n439_var_β:             add              rsp, 16
                        add              rsp, 544;                  jmp   n470_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n440_match_begin_α:     mov              rdi, qword ptr [rsp + 0]                       # var
                        mov              rsi, qword ptr [rsp + 8]
                        mov              qword ptr [rsp + 2656], rdi
                        mov              qword ptr [rsp + 2664], rsi
                        mov              qword ptr [rsp + 48], r13                      # outer_Σ
                        mov              qword ptr [rsp + 56], r14                      # outer_δ
                        mov              qword ptr [rsp + 64], r15                      # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rsp + 72], rax                      # cap_gen
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              qword ptr [r12 + 0], 0                         # cas_top
                        mov              qword ptr [r12 + 8], rsp                       # cas_rsp_mark
                        mov              qword ptr [r12 + 16], 0
                        add              r12, 24                                        # cas_top
                        mov              rax, rsp
                        sub              rsp, 32
                        mov              qword ptr [rsp + 16], rax                      # rsp_mark
                        mov              dword ptr [rsp + 0], 0                         # start_δ
.Lx798_0:               mov              r14d, dword ptr [rsp + 0]; jmp   n441_match_assign_save_α
n440_match_begin_β:     mov              rsp, qword ptr [r12 + -16]                     # rsp_restore
                        sub              rsp, 32
                        add              dword ptr [rsp + 0], 1                         # start_δ
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d;                 jg    .Lx798_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                    jne   .Lx798_1
                                                                    jmp   .Lx798_0
.Lx798_1:
n440_match_begin_af:    sub              r12, 24                                        # cas_mark
                        mov              rsp, qword ptr [r12 + 8]                       # cas_rsp_mark
                        mov              r13, qword ptr [rsp + 48]                      # outer_Σ
                        mov              r14, qword ptr [rsp + 56]                      # outer_δ
                        mov              r15, qword ptr [rsp + 64]                      # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rsp + 72]                      # cap_gen
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_ctx_restore@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n439_var_β
#-----------------------------------------------------------------------------------------------------------------------
n441_match_assign_save_α: sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d; jmp   n442_match_break_α
n441_match_assign_save_β: add              rsp, 16;                 jmp   n440_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n442_match_break_α:     sub              rsp, 16
                        movsxd           rcx, r14d
.Lx802_0:               cmp              ecx, r15d;                 jl    .Lx802_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n440_match_begin_β
.Lx802_240:             movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32;                   je    .Lx802_1
                        add              ecx, 1;                    jmp   .Lx802_0
.Lx802_1:               mov              dword ptr [rsp + 0], r14d
                        mov              r14d, ecx;                 jmp   n443_match_assign_cond_α
n442_match_break_β:     mov              r14d, dword ptr [rsp + 0]
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n440_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n443_match_assign_cond_α: mov              eax, dword ptr [rsp + 16]
                        lea              rcx, [rip + .S0]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                   jmp   n444_match_lit_α
n443_match_assign_cond_β: sub              r12, 24;                 jmp   n442_match_break_β
#-----------------------------------------------------------------------------------------------------------------------
n444_match_lit_α:       mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d;                 jg    n443_match_assign_cond_β
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 32;                   jne   n443_match_assign_cond_β
                        add              r14d, 1;                   jmp   n445_match_assign_save_α
n444_match_lit_β:       sub              r14d, 1;                   jmp   n443_match_assign_cond_β
#-----------------------------------------------------------------------------------------------------------------------
n445_match_assign_save_α: sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d; jmp   n446_match_rem_α
n445_match_assign_save_β: add              rsp, 16;                 jmp   n444_match_lit_β
#-----------------------------------------------------------------------------------------------------------------------
n446_match_rem_α:       sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                        mov              r14d, r15d;                jmp   n447_match_assign_cond_α
n446_match_rem_β:       mov              r14d, dword ptr [rsp + 0]
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n444_match_lit_β
#-----------------------------------------------------------------------------------------------------------------------
n447_match_assign_cond_α: mov              eax, dword ptr [rsp + 16]
                        lea              rcx, [rip + .S1]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                   jmp   n448_match_end_α
n447_match_assign_cond_β: sub              r12, 24;                 jmp   n446_match_rem_β
#-----------------------------------------------------------------------------------------------------------------------
n448_match_end_α:       mov              r8, r12
.Lx813_9:               sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                  jne   .Lx813_9
                        mov              eax, dword ptr [rsp + 64]
                        mov              dword ptr [rsp + 2544], eax
                        mov              qword ptr [rsp + 2568], r14
                        mov              rsp, qword ptr [r8 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r8, rsi
.Lx813_5:               sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                  jne   .Lx813_5
                        lea              rdi, [r8 + 24]
                        mov              rdx, r13
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_end_ok_open@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
.Lx813_1:               test             rax, rax;                  je    .Lx813_2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        lea              rcx, [rip + .Lx813_3]
                        lea              rdx, [rip + .Lx813_4];     jmp   rax
.Lx813_3:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_step@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx813_1
.Lx813_4:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_step@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx813_1
.Lx813_2:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_end_ok_close@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
.Lx813_10:              sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax;                  jne   .Lx813_10
                        mov              r13, qword ptr [rsp + 48]                      # outer_Σ
                        mov              r14, qword ptr [rsp + 56]                      # outer_δ
                        mov              r15, qword ptr [rsp + 64]                      # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_ctx_restore@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n449_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n449_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx814_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n450_match_replace_α
.Lx814_0:               .quad            .Lx814_0_s
.Lx814_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n450_match_replace_α:   mov              rdi, qword ptr [rip + .Lx816_0]
                        mov              rsi, qword ptr [rsp + 2672]
                        mov              rdx, qword ptr [rsp + 2680]
                        mov              ecx, dword ptr [rsp + 2464]
                        mov              r8, qword ptr [rsp + 2488]
                        lea              r9, [rsp + 0]                                  # lit_string
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_replace@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx816_1
.Lx816_0:               .quad            .Lx816_0_s
.Lx816_0_s:             .string          "subject"
.Lx816_1:                                                           jmp   n451_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n451_statement_end_α:                                               jmp   n452_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n452_statement_begin_α:                                             jmp   n453_var_α
n452_statement_begin_β:                                             jmp   n456_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n453_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 208]                      # w1
                        mov              rdx, qword ptr [r9 + 216]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n454_call_α
n453_var_β:             add              rsp, 16
                        add              rsp, 640;                  jmp   n452_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n454_call_α:            sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # var
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx823_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + stack_push_alpha]; jmp   rax
.Lx823_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx823_240
                        add              rsp, 16;                   jmp   n453_var_β
.Lx823_240:                                                         jmp   n455_statement_end_α
n454_call_β:                                                        jmp   n453_var_β
.Lx823_0:               .quad            .Lx823_0_s
.Lx823_0_s:             .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n455_statement_end_α:                                               jmp   n456_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n456_statement_begin_α:                                             jmp   n457_var_α
n456_statement_begin_β:                                             jmp   n460_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n457_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 224]                      # w2
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n458_call_α
n457_var_β:             add              rsp, 16
                        add              rsp, 672;                  jmp   n456_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n458_call_α:            sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # var
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx830_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + stack_push_alpha]; jmp   rax
.Lx830_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx830_240
                        add              rsp, 16;                   jmp   n457_var_β
.Lx830_240:                                                         jmp   n459_statement_end_α
n458_call_β:                                                        jmp   n457_var_β
.Lx830_0:               .quad            .Lx830_0_s
.Lx830_0_s:             .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n459_statement_end_α:                                               jmp   n460_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n460_statement_begin_α:                                             jmp   n461_call_α
n460_statement_begin_β:                                             jmp   n464_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n461_call_α:            sub              rsp, 16
                        sub              rsp, 32
                        mov              eax, 0
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx836_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + stack_pop_alpha]; jmp   rax
.Lx836_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx836_240
                        add              rsp, 16
                        add              rsp, 704;                  jmp   n460_statement_begin_β
.Lx836_240:                                                         jmp   n462_assign_α
n461_call_β:                                                        jmp   n460_statement_begin_β
.Lx836_0:               .quad            .Lx836_0_s
.Lx836_0_s:             .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n462_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx837_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n463_statement_end_α
.Lx837_0:               .quad            .Lx837_0_s
.Lx837_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n463_statement_end_α:                                               jmp   n464_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n464_statement_begin_α:                                             jmp   n465_call_α
n464_statement_begin_β:                                             jmp   n468_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n465_call_α:            sub              rsp, 16
                        sub              rsp, 32
                        mov              eax, 0
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx843_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + stack_pop_alpha]; jmp   rax
.Lx843_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx843_240
                        add              rsp, 16
                        add              rsp, 720;                  jmp   n464_statement_begin_β
.Lx843_240:                                                         jmp   n466_assign_α
n465_call_β:                                                        jmp   n464_statement_begin_β
.Lx843_0:               .quad            .Lx843_0_s
.Lx843_0_s:             .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n466_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx844_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n467_statement_end_α
.Lx844_0:               .quad            .Lx844_0_s
.Lx844_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n467_statement_end_α:                                               jmp   n468_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n468_statement_begin_α:                                             jmp   n469_statement_end_α
n468_statement_begin_β: add              rsp, 736;                  jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n469_statement_end_α:   add              rsp, 560;                  jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n470_statement_begin_α:                                             jmp   n471_lit_string_α
n470_statement_begin_β:                                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n471_lit_string_α:      mov              qword ptr [rsp + 2864], 2                      # result
                        mov              dword ptr [rsp + 2868], 26
                        mov              rax, qword ptr [rip + .Lx853_0]
                        mov              qword ptr [rsp + 2872], rax; jmp   n472_assign_α
.Lx853_0:               .quad            .Lx853_0_s
.Lx853_0_s:             .string          "FAIL: pattern match failed"
#-----------------------------------------------------------------------------------------------------------------------
n472_assign_α:          mov              rsi, qword ptr [rsp + 2864]
                        mov              rdx, qword ptr [rsp + 2872]
                        mov              rdi, qword ptr [rip + .Lx854_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n473_statement_end_α
.Lx854_0:               .quad            .Lx854_0_s
.Lx854_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n473_statement_end_α:                                               jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n474_goto_α:                                                        jmp   n299_statement_begin_α
n474_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n475_goto_α:                                                        jmp   n304_statement_begin_α
n475_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n476_goto_α:                                                        jmp   n477_statement_begin_α
n476_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n477_statement_begin_α:                                             jmp   n478_var_α
n477_statement_begin_β:                                             jmp   n484_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n478_var_α:             mov              rax, qword ptr [r9 + 64]                       # var
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 672], rax                     # result
                        mov              qword ptr [rsp + 680], rdx; jmp   n479_call_α
#-----------------------------------------------------------------------------------------------------------------------
n479_call_α:            mov              rax, qword ptr [rsp + 672]
                        mov              qword ptr [rsp + 640], rax
                        mov              rax, qword ptr [rsp + 680]
                        mov              qword ptr [rsp + 648], rax
                        .section         .rodata
.Lrkfn864:              .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn864]
                        lea              rsi, [rsp + 640]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 624], rax
                        mov              qword ptr [rsp + 632], rdx
                        cmp              eax, 104;                  je    n477_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n480_var_α
n479_call_β:                                                        jmp   n477_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n480_var_α:             mov              rax, qword ptr [r9 + 144]                      # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 736], rax                     # result
                        mov              qword ptr [rsp + 744], rdx; jmp   n481_call_α
#-----------------------------------------------------------------------------------------------------------------------
n481_call_α:            mov              rax, qword ptr [rsp + 736]
                        mov              qword ptr [rsp + 704], rax
                        mov              rax, qword ptr [rsp + 744]
                        mov              qword ptr [rsp + 712], rax
                        .section         .rodata
.Lrkfn867:              .string          "sval"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn867]
                        lea              rsi, [rsp + 704]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 688], rax
                        mov              qword ptr [rsp + 696], rdx
                        cmp              eax, 104;                  je    n477_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n482_assign_var_α
n481_call_β:                                                        jmp   n477_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n482_assign_var_α:      mov              rdi, qword ptr [rsp + 624]
                        mov              rsi, qword ptr [rsp + 632]
                        mov              rdx, qword ptr [rsp + 688]
                        mov              rcx, qword ptr [rsp + 696]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_assign_var@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        cmp              eax, 104;                  je    n477_statement_begin_β
                        mov              qword ptr [rsp + 752], rax
                        mov              qword ptr [rsp + 760], rdx; jmp   n483_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n483_statement_end_α:                                               jmp   n484_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n484_statement_begin_α:                                             jmp   n485_var_α
n484_statement_begin_β:                                             jmp   n489_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n485_var_α:             mov              rax, qword ptr [r9 + 144]                      # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 816], rax                     # result
                        mov              qword ptr [rsp + 824], rdx; jmp   n486_call_α
#-----------------------------------------------------------------------------------------------------------------------
n486_call_α:            mov              rax, qword ptr [rsp + 816]
                        mov              qword ptr [rsp + 784], rax
                        mov              rax, qword ptr [rsp + 824]
                        mov              qword ptr [rsp + 792], rax
                        .section         .rodata
.Lrkfn875:              .string          "snext"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn875]
                        lea              rsi, [rsp + 784]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 768], rax
                        mov              qword ptr [rsp + 776], rdx
                        cmp              eax, 104;                  je    n484_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n487_assign_α
n486_call_β:                                                        jmp   n484_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n487_assign_α:          mov              rax, qword ptr [rsp + 768]
                        mov              rdx, qword ptr [rsp + 776]
                        mov              qword ptr [r9 + 144], rax                      # stk
                        mov              qword ptr [r9 + 152], rdx; jmp   n488_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n488_statement_end_α:                                               jmp   n489_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n489_statement_begin_α:                                             jmp   n490_lit_string_α
n489_statement_begin_β:                                             jmp   NRETURN
#-----------------------------------------------------------------------------------------------------------------------
n490_lit_string_α:      mov              qword ptr [rsp + 880], 2                       # result
                        mov              dword ptr [rsp + 884], 5
                        mov              rax, qword ptr [rip + .Lx881_0]
                        mov              qword ptr [rsp + 888], rax; jmp   n491_call_α
.Lx881_0:               .quad            .Lx881_0_s
.Lx881_0_s:             .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n491_call_α:            mov              rax, qword ptr [rsp + 880]
                        mov              qword ptr [rsp + 848], rax
                        mov              rax, qword ptr [rsp + 888]
                        mov              qword ptr [rsp + 856], rax
                        .section         .rodata
.Lrkfn883:              .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn883]
                        lea              rsi, [rsp + 848]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 832], rax
                        mov              qword ptr [rsp + 840], rdx
                        cmp              eax, 104;                  je    n489_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n492_assign_α
n491_call_β:                                                        jmp   n489_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n492_assign_α:          mov              rax, qword ptr [rsp + 832]
                        mov              rdx, qword ptr [rsp + 840]
                        mov              qword ptr [r9 + 48], rax                       # stack_pop
                        mov              qword ptr [r9 + 56], rdx;  jmp   n493_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n493_statement_end_α:                                               jmp   NRETURN
#-----------------------------------------------------------------------------------------------------------------------
n494_goto_α:                                                        jmp   n309_statement_begin_α
n494_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n495_goto_α:                                                        jmp   n314_statement_begin_α
n495_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n496_goto_α:                                                        jmp   n319_statement_begin_α
n496_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n497_goto_α:                                                        jmp   n498_statement_begin_α
n497_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n498_statement_begin_α:                                             jmp   n499_var_α
n498_statement_begin_β:                                             jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n499_var_α:             mov              rax, qword ptr [r9 + 128]                      # sd
                        mov              rdx, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 1248], rax                    # result
                        mov              qword ptr [rsp + 1256], rdx; jmp   n500_call_α
#-----------------------------------------------------------------------------------------------------------------------
n500_call_α:            mov              rax, qword ptr [rsp + 1248]
                        mov              qword ptr [rsp + 1216], rax
                        mov              rax, qword ptr [rsp + 1256]
                        mov              qword ptr [rsp + 1224], rax
                        .section         .rodata
.Lrkfn895:              .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn895]
                        lea              rsi, [rsp + 1216]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1200], rax
                        mov              qword ptr [rsp + 1208], rdx
                        cmp              eax, 104;                  je    n498_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n501_statement_end_α
n500_call_β:                                                        jmp   n498_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n501_statement_end_α:                                               jmp   n502_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n502_statement_begin_α:                                             jmp   n503_var_α
n502_statement_begin_β:                                             jmp   n508_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n503_var_α:             mov              rax, qword ptr [r9 + 112]                      # stack_depth
                        mov              rdx, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 1280], rax                    # result
                        mov              qword ptr [rsp + 1288], rdx; jmp   n504_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n504_lit_integer_α:     mov              qword ptr [rsp + 1296], 3                      # result
                        mov              rax, qword ptr [rip + .Lx901_0]
                        mov              qword ptr [rsp + 1304], rax; jmp   n505_binop_α
.Lx901_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n505_binop_α:           mov              eax, dword ptr [rsp + 1280]
                        cmp              eax, 3;                    jne   .Lx902_0
                        mov              rax, qword ptr [rsp + 1288]
                        mov              rcx, 1
                        add              rax, rcx
                        mov              qword ptr [rsp + 1264], 3
                        mov              qword ptr [rsp + 1272], rax; jmp   n506_assign_α
.Lx902_0:               mov              rdi, qword ptr [rsp + 1280]
                        mov              rsi, qword ptr [rsp + 1288]
                        mov              rdx, qword ptr [rsp + 1296]
                        mov              rcx, qword ptr [rsp + 1304]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_add@PLT
                        cmp              eax, 104;                  jne   .Lx902_240
                        add              rsp, 32;                   jmp   n502_statement_begin_β
.Lx902_240:             mov              qword ptr [rsp + 1264], rax
                        mov              qword ptr [rsp + 1272], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n506_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n506_assign_α:          mov              rax, qword ptr [rsp + 1264]
                        mov              rdx, qword ptr [rsp + 1272]
                        mov              qword ptr [r9 + 112], rax                      # stack_depth
                        mov              qword ptr [r9 + 120], rdx; jmp   n507_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n507_statement_end_α:                                               jmp   n508_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n508_statement_begin_α:                                             jmp   n509_var_α
n508_statement_begin_β:                                             jmp   n498_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n509_var_α:             mov              rax, qword ptr [r9 + 128]                      # sd
                        mov              rdx, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 1360], rax                    # result
                        mov              qword ptr [rsp + 1368], rdx; jmp   n510_call_α
#-----------------------------------------------------------------------------------------------------------------------
n510_call_α:            mov              rax, qword ptr [rsp + 1360]
                        mov              qword ptr [rsp + 1328], rax
                        mov              rax, qword ptr [rsp + 1368]
                        mov              qword ptr [rsp + 1336], rax
                        .section         .rodata
.Lrkfn910:              .string          "snext"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn910]
                        lea              rsi, [rsp + 1328]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1312], rax
                        mov              qword ptr [rsp + 1320], rdx
                        cmp              eax, 104;                  je    n508_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n511_assign_α
n510_call_β:                                                        jmp   n508_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n511_assign_α:          mov              rax, qword ptr [rsp + 1312]
                        mov              rdx, qword ptr [rsp + 1320]
                        mov              qword ptr [r9 + 128], rax                      # sd
                        mov              qword ptr [r9 + 136], rdx; jmp   n512_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n512_statement_end_α:                                               jmp   n498_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n513_goto_α:                                                        jmp   n324_statement_begin_α
n513_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n514_goto_α:                                                        jmp   n379_statement_begin_α
n514_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n515_goto_α:                                                        jmp   n387_statement_begin_α
n515_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n516_goto_α:                                                        jmp   n470_statement_begin_α
n516_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
RETURN:                 lea              rdi, [rip + .S2]
                        call             rt_bomb@PLT
                        ud2
#-----------------------------------------------------------------------------------------------------------------------
FRETURN:                lea              rdi, [rip + .S3]
                        call             rt_bomb@PLT
                        ud2
#-----------------------------------------------------------------------------------------------------------------------
NRETURN:                mov              qword ptr [rsp + 32], 2                        # result
                        mov              dword ptr [rsp + 36], 0
                        mov              rax, qword ptr [rip + .Lx922_0]
                        mov              qword ptr [rsp + 40], rax; jmp   n520_call_α
.Lx922_0:               .quad            .Lx922_0_s
.Lx922_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n520_call_α:            mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 72], rax
                        .section         .rodata
.Lrkfn924:              .string          "SNO$NRET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn924]
                        lea              rsi, [rsp + 64]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 48], rax
                        mov              qword ptr [rsp + 56], rdx
                        cmp              eax, 104;                  je    FRETURN
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   RETURN
n520_call_β:                                                        jmp   FRETURN
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
.S0:                    .string          "w1"
.S1:                    .string          "w2"
.S2:                    .string          "BOMB-RETURN: descent complete, coming-out frozen (s58 RSP-only) \342\200\224 UNKNOWN STACK DEPTH: the rsp-resident record cannot be found from here without a frame anchor"
.S3:                    .string          "BOMB-FRETURN: descent complete, coming-out frozen (s58 RSP-only) \342\200\224 UNKNOWN STACK DEPTH: the rsp-resident record cannot be found from here without a frame anchor"
                        .text
                        .section         .note.GNU-stack,"",@progbits

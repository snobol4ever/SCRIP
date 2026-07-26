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
                        mov              qword ptr [rsp + 536], rsp
                        mov              r12, qword ptr [1879048192]
                        mov              [rsp + 65536], rbp
                        mov              rbp, rsp
main_α_body:
#=======================================================================================================================
#         s = 'abcdefghij'
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n0_α:
                        mov              qword ptr [rbp + 112], 1
                        mov              rax, qword ptr [rip + .Lx1_0]
                        mov              qword ptr [rbp + 120], rax
                                                                                        jmp   xchain0_n1_α
xchain0_n0_β:
                                                                                        jmp   xchain0_n2_α
.Lx1_0:
                        .quad            .Lx1_0_s
.Lx1_0_s:
                        .string          "abcdefghij"
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN gva
xchain0_n1_α:
                        mov              rax, qword ptr [rbp + 112]
                        mov              rdx, qword ptr [rbp + 120]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        mov              qword ptr [rbp + 96], rax
                        mov              qword ptr [rbp + 104], rdx
                                                                                        jmp   xchain0_n2_α
xchain0_n1_β:
                                                                                        jmp   xchain0_n2_α
#=======================================================================================================================
#         s POS(0) ARBNO(LEN(1) | LEN(1)) RPOS(0)    :S(YES)F(NO)
#-----------------------------------------------------------------------------------------------------------------------
# IR_VAR
xchain0_n2_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rbp + 400], rax
                        mov              qword ptr [rbp + 408], rdx
                                                                                        jmp   xchain0_n3_α
xchain0_n2_β:
                                                                                        jmp   xchain0_n4_α
#-----------------------------------------------------------------------------------------------------------------------
# IR_MATCH_HEAD
xchain0_n3_α:
                        mov              qword ptr [rbp + 200], rbp
                        mov              rdi, qword ptr [rbp + 400]
                        mov              rsi, qword ptr [rbp + 408]
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              qword ptr [rbp + 192], r12
                        mov              rax, rsp
                        sub              rsp, 32
                        mov              qword ptr [rsp + 16], rax
                        lea              rcx, [rip + g_patstk_sp]
                        mov              rax, qword ptr [rcx + 0]
                        mov              qword ptr [rsp + 8], rax
                        mov              dword ptr [rsp + 0], 0
.Lx5_0:
                        mov              r14d, dword ptr [rsp + 0]
                                                                                        jmp   xchain0_n5_α
xchain0_n3_β:
                        add              dword ptr [rsp + 0], 1
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d
                                                                                        jg    .Lx5_1
                        lea              rcx, [rip + g_anchor]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx5_1
                                                                                        jmp   .Lx5_0
.Lx5_1:
                        mov              rax, qword ptr [rsp + 8]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rsp + 16]
                        mov              r12, qword ptr [rbp + 192]
                        mov              rbp, qword ptr [rbp + 200]
                                                                                        jmp   xchain0_n4_α
#=======================================================================================================================
# NO      OUTPUT = 'fail'
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n4_α:
                        mov              qword ptr [rbp + 496], 1
                        mov              rax, qword ptr [rip + .Lx6_0]
                        mov              qword ptr [rbp + 504], rax
                                                                                        jmp   xchain0_n6_α
xchain0_n4_β:
                                                                                        jmp   main_γ
.Lx6_0:
                        .quad            .Lx6_0_s
.Lx6_0_s:
                        .string          "fail"
#-----------------------------------------------------------------------------------------------------------------------
# IR_MATCH_SEQ_NARY (ZB-FC-3b: zero cell, LIFO-structural)
xchain0_n5_α:
                                                                                        jmp   xchain0_n8_α
xchain0_n5_as:
                                                                                        jmp   xchain0_n7_α
xchain0_n5_β:
                                                                                        jmp   xchain0_n12_β
xchain0_n5_af:
                                                                                        jmp   xchain0_n3_β
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN global
xchain0_n6_α:
                        mov              rsi, qword ptr [rbp + 496]
                        mov              rdx, qword ptr [rbp + 504]
                        mov              rdi, qword ptr [rip + .Lx9_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 480], rax
                        mov              qword ptr [rbp + 488], rdx
                                                                                        jmp   main_γ
xchain0_n6_β:
                                                                                        jmp   main_γ
.Lx9_0:
                        .quad            .Lx9_0_s
.Lx9_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
# IR_MATCH_RELEASE (R12-EXIT-1 tail: bracket read off the TOP ELEMENT, then the one-mov unwind collapses every element, every suspended cell, and HEAD's cell together)
xchain0_n7_α:
                        mov              rax, qword ptr [rsp + 336]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rsp + 344]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rsp + 224]
                        mov              rsi, r12
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx11_1:
                        test             rax, rax
                                                                                        je    .Lx11_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx11_3]
                        lea              rdx, [rip + .Lx11_4]
                                                                                        jmp   rax
.Lx11_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx11_1
.Lx11_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx11_1
.Lx11_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r12, qword ptr [rbp + 192]
                        mov              rbp, qword ptr [rbp + 200]
                                                                                        jmp   xchain0_n13_α
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_INTEGER
xchain0_n8_α:
                        mov              qword ptr [rbp + 240], 6
                        mov              rax, qword ptr [rip + .Lx12_0]
                        mov              qword ptr [rbp + 248], rax
                                                                                        jmp   xchain0_n9_α
xchain0_n8_β:
                                                                                        jmp   xchain0_n3_β
.Lx12_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
# IR_MATCH_POS
xchain0_n9_α:
                        mov              rax, 0
                        cmp              r14d, eax
                                                                                        jne   xchain0_n3_β
                                                                                        jmp   xchain0_n10_α
xchain0_n9_β:
                                                                                        jmp   xchain0_n3_β
#-----------------------------------------------------------------------------------------------------------------------
# IR_MATCH_ARBNO_TAIL (R12-EXIT-1 carry-the-tail rsp elements)
xchain0_n10_α:
                        sub              rsp, 352
                        mov              eax, 0
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
                        mov              dword ptr [rsp + 320], r14d
                        mov              dword ptr [rsp + 324], r14d
                        mov              dword ptr [rsp + 328], 1
                        mov              rax, qword ptr [rsp + 360]
                        mov              qword ptr [rsp + 336], rax
                        mov              rax, qword ptr [rsp + 368]
                        mov              qword ptr [rsp + 344], rax
                                                                                        jmp   xchain0_n11_α
xchain0_n10_β:
                        mov              r14d, dword ptr [rsp + 324]
                        mov              rax, qword ptr [rsp + 336]
                        mov              rcx, qword ptr [rsp + 344]
                        sub              rsp, 336
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
                        mov              dword ptr [rsp + 304], r14d
                        mov              dword ptr [rsp + 308], r14d
                        mov              dword ptr [rsp + 312], 0
                        mov              qword ptr [rsp + 320], rax
                        mov              qword ptr [rsp + 328], rcx
                                                                                        jmp   xchain0_n14_α
xchain0_n10_as:
                        mov              eax, dword ptr [rsp + 320]
                        cmp              r14d, eax
                                                                                        je    xchain0_n14_β
                        mov              dword ptr [rsp + 324], r14d
                                                                                        jmp   xchain0_n11_α
xchain0_n10_af:
                        mov              eax, dword ptr [rsp + 312]
                        test             eax, eax
                                                                                        jnz   .Lx15_2
                        add              rsp, 336
                                                                                        jmp   xchain0_n14_β
.Lx15_2:
                        mov              r14d, dword ptr [rsp + 304]
                        add              rsp, 336
                                                                                        jmp   xchain0_n9_β
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_INTEGER
xchain0_n11_α:
                        mov              qword ptr [rbp + 368], 6
                        mov              rax, qword ptr [rip + .Lx16_0]
                        mov              qword ptr [rbp + 376], rax
                                                                                        jmp   xchain0_n12_α
xchain0_n11_β:
                                                                                        jmp   xchain0_n10_β
.Lx16_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
# IR_MATCH_RPOS
xchain0_n12_α:
                        mov              rax, 0
                        mov              ecx, r15d
                        sub              ecx, eax
                        cmp              r14d, ecx
                                                                                        jne   xchain0_n10_β
                                                                                        jmp   xchain0_n7_α
xchain0_n12_β:
                                                                                        jmp   xchain0_n10_β
#=======================================================================================================================
# YES     OUTPUT = 'grow ok'                          :(END)
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n13_α:
                        mov              qword ptr [rbp + 448], 1
                        mov              rax, qword ptr [rip + .Lx18_0]
                        mov              qword ptr [rbp + 456], rax
                                                                                        jmp   xchain0_n16_α
xchain0_n13_β:
                                                                                        jmp   main_γ
.Lx18_0:
                        .quad            .Lx18_0_s
.Lx18_0_s:
                        .string          "grow ok"
#-----------------------------------------------------------------------------------------------------------------------
# IR_MATCH_ALT_NARY
xchain0_n14_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                        mov              dword ptr [rsp + 4], 0
                                                                                        jmp   xchain0_n17_α
xchain0_n14_s0:
                                                                                        jmp   xchain0_n14_as
xchain0_n14_s1:
                                                                                        jmp   xchain0_n14_as
xchain0_n14_as:
                                                                                        jmp   xchain0_n10_as
xchain0_n14_β:
                        mov              eax, dword ptr [rsp + 4]
                        cmp              eax, 0
                                                                                        je    xchain0_n17_β
                                                                                        jmp   xchain0_n15_β
xchain0_n14_af:
                        add              dword ptr [rsp + 4], 1
                        mov              r14d, dword ptr [rsp + 0]
                        mov              eax, dword ptr [rsp + 4]
                        cmp              eax, 1
                                                                                        je    xchain0_n15_α
                        add              rsp, 16
                                                                                        jmp   xchain0_n10_af
#-----------------------------------------------------------------------------------------------------------------------
# IR_MATCH_LEN
xchain0_n15_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jg    xchain0_n14_af
                        add              r14d, 1
                                                                                        jmp   xchain0_n14_s1
xchain0_n15_β:
                        sub              r14d, 1
                                                                                        jmp   xchain0_n14_af
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN global
xchain0_n16_α:
                        mov              rsi, qword ptr [rbp + 448]
                        mov              rdx, qword ptr [rbp + 456]
                        mov              rdi, qword ptr [rip + .Lx22_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 432], rax
                        mov              qword ptr [rbp + 440], rdx
                                                                                        jmp   main_γ
xchain0_n16_β:
                                                                                        jmp   main_γ
.Lx22_0:
                        .quad            .Lx22_0_s
.Lx22_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
# IR_MATCH_LEN
xchain0_n17_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jg    xchain0_n14_af
                        add              r14d, 1
                                                                                        jmp   xchain0_n14_s0
xchain0_n17_β:
                        sub              r14d, 1
                                                                                        jmp   xchain0_n14_af
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        mov              rsp, qword ptr [rbp + 536]
                        mov              rbp, [rsp + 65536]
                        add              rsp, 65544
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              rsp, qword ptr [rbp + 536]
                        mov              dword ptr [rsp+0], 99
                        mov              dword ptr [rsp+4], 0
                        mov              qword ptr [rsp+8], 0
                        mov              eax, 99
                        xor              edx, edx
                        mov              rbp, [rsp + 65536]
                        add              rsp, 65544
                        ret
                        .section         .note.GNU-stack,"",@progbits

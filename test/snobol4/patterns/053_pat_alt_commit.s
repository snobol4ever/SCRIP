                        .intel_syntax    noprefix
                        .text
  .globl proc_PAT$0_α
proc_PAT$0_α:
#=======================================================================================================================
    .global proc_PAT$0_α
    .global proc_PAT$0_β
    .global proc_PAT$0_γ
    .global proc_PAT$0_ω
  sub rsp, 112
  mov [rsp + 88], rcx
  mov [rsp + 96], rdx
  mov [rsp + 104], rbp
  mov rbp, rsp
  mov qword ptr [rsp], 0
  mov qword ptr [rsp + 8], 0
  mov qword ptr [rsp + 48], 0
  mov qword ptr [rsp + 56], 0
  mov qword ptr [rsp + 64], 0
  mov qword ptr [rsp + 72], rsp
mov qword ptr [rbp + 80], r8
mov dword ptr [rbp + 72], r14d
proc_PAT$0_attempt:
proc_PAT$0_α_body:
lea rax, [rip + xchain0_n0_β]
mov qword ptr [rbp + 48], rax
# IR_MATCH_ALT_NARY
xchain0_n0_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                        mov              dword ptr [rsp + 4], 0
                        jmp              xchain0_n1_α
xchain0_n0_s0:
                        jmp              xchain0_n0_as
xchain0_n0_s1:
                        jmp              xchain0_n0_as
xchain0_n0_s2:
                        jmp              xchain0_n0_as
xchain0_n0_as:
                        jmp              proc_PAT$0_γ
xchain0_n0_β:
                        mov              eax, dword ptr [rsp + 4]
                        cmp              eax, 0
                        je               xchain0_n1_β
                        cmp              eax, 1
                        je               xchain0_n2_β
                        jmp              xchain0_n3_β
xchain0_n0_af:
                        add              dword ptr [rsp + 4], 1
                        mov              r14d, dword ptr [rsp + 0]
                        mov              eax, dword ptr [rsp + 4]
                        cmp              eax, 1
                        je               xchain0_n2_α
                        cmp              eax, 2
                        je               xchain0_n3_α
                        add              rsp, 16
                        jmp              proc_PAT$0_ω
# IR_MATCH_LIT
xchain0_n1_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                        jg               xchain0_n0_af
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 97
                        jne              xchain0_n0_af
                        add              r14d, 1
                        jmp              xchain0_n0_s0
xchain0_n1_β:
                        sub              r14d, 1
                        jmp              xchain0_n0_af
# IR_MATCH_LIT
xchain0_n2_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                        jg               xchain0_n0_af
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 98
                        jne              xchain0_n0_af
                        add              r14d, 1
                        jmp              xchain0_n0_s1
xchain0_n2_β:
                        sub              r14d, 1
                        jmp              xchain0_n0_af
# IR_MATCH_LIT
xchain0_n3_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                        jg               xchain0_n0_af
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 99
                        jne              xchain0_n0_af
                        add              r14d, 1
                        jmp              xchain0_n0_s2
xchain0_n3_β:
                        sub              r14d, 1
                        jmp              xchain0_n0_af
proc_PAT$0_scanhit:
cmp qword ptr [rbp + 80], 1
jne 7f
mov ecx, dword ptr [rbp + 72]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$0_γ
proc_PAT$0_scanfail:
cmp qword ptr [rbp + 80], 1
jne 8f
mov eax, dword ptr [rbp + 72]
inc eax
cmp eax, r15d
jg 8f
lea rcx, [rip + g_anchor]
cmp qword ptr [rcx], 0
jne 8f
mov dword ptr [rbp + 72], eax
mov r14d, eax
mov rsp, rbp
jmp proc_PAT$0_attempt
8:
jmp proc_PAT$0_ω
proc_PAT$0_res:
add rsp, 8
pop rbp
proc_PAT$0_β:
jmp qword ptr [rbp + 48]
proc_PAT$0_γ:
push rbp
lea rax, [rip + proc_PAT$0_res]
push rax
mov rax, [rbp + 88]
mov rbp, [rbp + 104]
jmp rax
proc_PAT$0_ω:
mov rax, [rbp + 96]
lea rsp, [rbp + 112]
mov rbp, [rbp + 104]
jmp rax
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
                        mov              esi, 80
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
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             proc_startup
                        mov              edi, 3
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 3
                        call             gva_register@PLT
                        xor              esi, esi
                        call             main_α
                        xor              eax, eax
                        add              rsp, 24
                        ret
main_α:
#=======================================================================================================================
    .global main_α
    .global main_β
    .global main_γ
    .global main_ω
  sub rsp, 65544
  mov rdi, rsp
  mov ecx, 65544
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 552], rsp
  mov r12, qword ptr [1879048192]
  mov [rsp + 65536], rbp
  mov rbp, rsp
main_α_body:
#         P = ('a' | 'b' | 'c')
# IR_LIT_STRING
xchain9_n0_α:
                        mov              qword ptr [rbp + 160], 1
                        mov              rax, qword ptr [rip + .Lx10_0]
                        mov              qword ptr [rbp + 168], rax
                        jmp              xchain9_n1_α
.Lx10_0:
                        .quad            .Lx10_0_s
.Lx10_0_s:
                        .string          "PAT$0"
xchain9_n1_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+160] -> [zr+128]
                        mov              rax, qword ptr [rbp + 160]
                        mov              qword ptr [rbp + 128], rax
                        mov              rax, qword ptr [rbp + 168]
                        mov              qword ptr [rbp + 136], rax
                        .section         .rodata
.Lrkfn12:               .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn12]
                        lea              rsi, [rbp + 128]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 112], rax
                        mov              qword ptr [rbp + 120], rdx
                        cmp              eax, 99
                        je               xchain9_n3_α
                        jmp              xchain9_n2_α
xchain9_n1_β:
                        jmp              xchain9_n3_α
# IR_ASSIGN gva
xchain9_n2_α:
                        mov              rax, qword ptr [rbp + 112]
                        mov              rdx, qword ptr [rbp + 120]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        mov              qword ptr [rbp + 96], rax
                        mov              qword ptr [rbp + 104], rdx
                        jmp              xchain9_n3_α
#         X = 'b'
# IR_LIT_STRING
xchain9_n3_α:
                        mov              qword ptr [rbp + 208], 1
                        mov              rax, qword ptr [rip + .Lx14_0]
                        mov              qword ptr [rbp + 216], rax
                        jmp              xchain9_n4_α
.Lx14_0:
                        .quad            .Lx14_0_s
.Lx14_0_s:
                        .string          "b"
# IR_ASSIGN gva
xchain9_n4_α:
                        mov              rax, qword ptr [rbp + 208]
                        mov              rdx, qword ptr [rbp + 216]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        mov              qword ptr [rbp + 192], rax
                        mov              qword ptr [rbp + 200], rdx
                        jmp              xchain9_n5_α
#         X P . V                                                     :S(YES)F(NO)
# IR_VAR
xchain9_n5_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rbp + 416], rax
                        mov              qword ptr [rbp + 424], rdx
                        jmp              xchain9_n6_α
# IR_MATCH_HEAD
xchain9_n6_α:
                        mov              qword ptr [rbp + 296], rbp
                        mov              rdi, qword ptr [rbp + 416]
                        mov              rsi, qword ptr [rbp + 424]
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              qword ptr [rbp + 288], r12
                        mov              qword ptr [rbp + 272], rsp
                        lea              rcx, [rip + g_patstk_sp]
                        mov              rax, qword ptr [rcx + 0]
                        mov              qword ptr [rbp + 264], rax
                        mov              dword ptr [rbp + 256], 0
.Lx18_0:
                        mov              r14d, dword ptr [rbp + 256]
                        jmp              xchain9_n8_α
xchain9_n6_β:
                        add              dword ptr [rbp + 256], 1
                        mov              eax, dword ptr [rbp + 256]
                        cmp              eax, r15d
                        jg               .Lx18_1
                        lea              rcx, [rip + g_anchor]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                        jne              .Lx18_1
                        jmp              .Lx18_0
.Lx18_1:
                        mov              rax, qword ptr [rbp + 264]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rbp + 272]
                        mov              r12, qword ptr [rbp + 288]
                        mov              rbp, qword ptr [rbp + 296]
                        jmp              xchain9_n7_α
# NO      OUTPUT = 'no match'
# IR_LIT_STRING
xchain9_n7_α:
                        mov              qword ptr [rbp + 512], 1
                        mov              rax, qword ptr [rip + .Lx19_0]
                        mov              qword ptr [rbp + 520], rax
                        jmp              xchain9_n9_α
.Lx19_0:
                        .quad            .Lx19_0_s
.Lx19_0_s:
                        .string          "no match"
# IR_MATCH_CAPTURE_SAVE push
xchain9_n8_α:
                        lea              rdi, [rbp + 336]
                        mov              esi, r14d
                        call             rt_cap_push@PLT
                        jmp              xchain9_n10_α
xchain9_n8_β:
                        lea              rdi, [rbp + 336]
                        call             rt_cap_pop@PLT
                        jmp              xchain9_n6_β
# IR_ASSIGN global
xchain9_n9_α:
                        mov              rsi, qword ptr [rbp + 512]
                        mov              rdx, qword ptr [rbp + 520]
                        mov              rdi, qword ptr [rip + .Lx22_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 496], rax
                        mov              qword ptr [rbp + 504], rdx
                        jmp              main_γ
.Lx22_0:
                        .quad            .Lx22_0_s
.Lx22_0_s:
                        .string          "OUTPUT"
# IR_MATCH_DEFER (ZS-2 jmp-entry)
xchain9_n10_α:
                        lea              rsi, [rip + g_sno_defer_cells+0]
                        mov              rax, qword ptr [rsi + 0]
                        test             rax, rax
                        jne              .Lx23_11
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        cmp              eax, 3
                        jne              .Lx23_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                        jne              .Lx23_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                        jmp              .Lx23_10
.Lx23_9:
                        xor              eax, eax
.Lx23_10:
                        lea              rsi, [rip + g_sno_defer_cells+0]
                        mov              qword ptr [rsi + 0], rax
.Lx23_11:
                        test             rax, rax
                        jz               .Lx23_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx23_4]
                        lea              rdx, [rip + .Lx23_5]
                        jmp              rax
.Lx23_4:
                        jmp              xchain9_n11_α
.Lx23_5:
                        jmp              xchain9_n8_β
.Lx23_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S0]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx23_2:
                        test             rax, rax
                        je               .Lx23_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx23_7]
                        lea              rdx, [rip + .Lx23_8]
                        jmp              rax
.Lx23_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                        jmp              .Lx23_2
.Lx23_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                        jmp              .Lx23_2
.Lx23_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                        js               xchain9_n8_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx23_6]
                        sub              rsp, 8
                        push             rax
                        jmp              xchain9_n11_α
.Lx23_6:
                        add              rsp, 16
                        jmp              xchain9_n8_β
xchain9_n10_β:
                        jmp              qword ptr [rsp]
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
xchain9_n11_α:
                        lea              rdi, [rbp + 336]
                        call             rt_cap_top@PLT
                        lea              rcx, [rip + .S1]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24
                        jmp              xchain9_n12_α
xchain9_n11_β:
                        sub              r12, 24
                        jmp              xchain9_n10_β
# IR_MATCH_RELEASE
xchain9_n12_α:
                        mov              rax, qword ptr [rbp + 264]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rbp + 272]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rsp + 320]
                        mov              rsi, r12
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx27_1:
                        test             rax, rax
                        je               .Lx27_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx27_3]
                        lea              rdx, [rip + .Lx27_4]
                        jmp              rax
.Lx27_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                        jmp              .Lx27_1
.Lx27_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                        jmp              .Lx27_1
.Lx27_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r12, qword ptr [rbp + 288]
                        mov              rbp, qword ptr [rbp + 296]
                        jmp              xchain9_n13_α
# YES     OUTPUT = V
# IR_VAR
xchain9_n13_α:
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rbp + 464], rax
                        mov              qword ptr [rbp + 472], rdx
                        jmp              xchain9_n14_α
# IR_ASSIGN global
xchain9_n14_α:
                        mov              rsi, qword ptr [rbp + 464]
                        mov              rdx, qword ptr [rbp + 472]
                        mov              rdi, qword ptr [rip + .Lx29_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 448], rax
                        mov              qword ptr [rbp + 456], rdx
                        jmp              main_γ
.Lx29_0:
                        .quad            .Lx29_0_s
.Lx29_0_s:
                        .string          "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rbp + 552]
mov rbp, [rsp + 65536]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rbp + 552]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
mov rbp, [rsp + 65536]
add rsp, 65544
ret
.section .rodata
.S0: .string "P"
.S1: .string "V"
.text

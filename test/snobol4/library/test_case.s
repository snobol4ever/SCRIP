  .intel_syntax noprefix
  .text
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  xor esi, esi
  call main_α
  xor eax, eax
  add rsp, 24
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
  mov qword ptr [rsp + 1528], rsp
  mov r12, qword ptr [1879048192]
  mov [rsp + 65536], rbp
  mov rbp, rsp
main_α_body:
#         &TRIM = 1
# IR_LIT_STRING
xchain0_n0_α:
                        mov              qword ptr [rbp + 160], 1
                        mov              rax, qword ptr [rip + .Lx1_0]
                        mov              qword ptr [rbp + 168], rax
                        jmp              xchain0_n1_α
.Lx1_0:
                        .quad            .Lx1_0_s
.Lx1_0_s:
                        .string          "TRIM"
# IR_LIT_INTEGER
xchain0_n1_α:
                        mov              qword ptr [rbp + 192], 6
                        mov              rax, qword ptr [rip + .Lx2_0]
                        mov              qword ptr [rbp + 200], rax
                        jmp              xchain0_n2_α
.Lx2_0:
                        .quad            1
xchain0_n2_α:
# BOX IR_CALL SNO$KWSET(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+160] -> [zr+112]
                        mov              rax, qword ptr [rbp + 160]
                        mov              qword ptr [rbp + 112], rax
                        mov              rax, qword ptr [rbp + 168]
                        mov              qword ptr [rbp + 120], rax
# marshal arg1 = producer-box slot [zr+192] -> [zr+128]
                        mov              rax, qword ptr [rbp + 192]
                        mov              qword ptr [rbp + 128], rax
                        mov              rax, qword ptr [rbp + 200]
                        mov              qword ptr [rbp + 136], rax
                        .section         .rodata
.Lrkfn4:                .string          "SNO$KWSET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn4]
                        lea              rsi, [rbp + 112]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 96], rax
                        mov              qword ptr [rbp + 104], rdx
                        cmp              eax, 99
                        je               xchain0_n3_α
                        jmp              xchain0_n3_α
xchain0_n2_β:
                        jmp              xchain0_n3_α
#         &TRIM = 1
#         OUTPUT = lwr('HELLO WORLD')     ;* hello world
# IR_LIT_STRING
xchain0_n3_α:
                        mov              qword ptr [rbp + 288], 1
                        mov              rax, qword ptr [rip + .Lx5_0]
                        mov              qword ptr [rbp + 296], rax
                        jmp              xchain0_n4_α
.Lx5_0:
                        .quad            .Lx5_0_s
.Lx5_0_s:
                        .string          "HELLO WORLD"
xchain0_n4_α:
# BOX CALL lwr(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+288] -> [zr+256]
                        mov              rax, qword ptr [rbp + 288]
                        mov              qword ptr [rbp + 256], rax
                        mov              rax, qword ptr [rbp + 296]
                        mov              qword ptr [rbp + 264], rax
                        .section         .rodata
.Lbynamefn5:            .string          "lwr"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn5]
                        lea              rsi, [rbp + 256]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 240], rax
                        mov              qword ptr [rbp + 248], rdx
                        cmp              eax, 99
                        je               xchain0_n6_α
                        jmp              xchain0_n5_α
xchain0_n4_β:
                        jmp              xchain0_n6_α
# IR_ASSIGN global
xchain0_n5_α:
                        mov              rsi, qword ptr [rbp + 240]
                        mov              rdx, qword ptr [rbp + 248]
                        mov              rdi, qword ptr [rip + .Lx7_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 224], rax
                        mov              qword ptr [rbp + 232], rdx
                        jmp              xchain0_n6_α
.Lx7_0:
                        .quad            .Lx7_0_s
.Lx7_0_s:
                        .string          "OUTPUT"
#         OUTPUT = upr('hello world')     ;* HELLO WORLD
# IR_LIT_STRING
xchain0_n6_α:
                        mov              qword ptr [rbp + 384], 1
                        mov              rax, qword ptr [rip + .Lx8_0]
                        mov              qword ptr [rbp + 392], rax
                        jmp              xchain0_n7_α
.Lx8_0:
                        .quad            .Lx8_0_s
.Lx8_0_s:
                        .string          "hello world"
xchain0_n7_α:
# BOX CALL upr(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+384] -> [zr+352]
                        mov              rax, qword ptr [rbp + 384]
                        mov              qword ptr [rbp + 352], rax
                        mov              rax, qword ptr [rbp + 392]
                        mov              qword ptr [rbp + 360], rax
                        .section         .rodata
.Lbynamefn8:            .string          "upr"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn8]
                        lea              rsi, [rbp + 352]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 336], rax
                        mov              qword ptr [rbp + 344], rdx
                        cmp              eax, 99
                        je               xchain0_n9_α
                        jmp              xchain0_n8_α
xchain0_n7_β:
                        jmp              xchain0_n9_α
# IR_ASSIGN global
xchain0_n8_α:
                        mov              rsi, qword ptr [rbp + 336]
                        mov              rdx, qword ptr [rbp + 344]
                        mov              rdi, qword ptr [rip + .Lx10_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 320], rax
                        mov              qword ptr [rbp + 328], rdx
                        jmp              xchain0_n9_α
.Lx10_0:
                        .quad            .Lx10_0_s
.Lx10_0_s:
                        .string          "OUTPUT"
#         OUTPUT = cap('hELLO wORLD')     ;* Hello world
# IR_LIT_STRING
xchain0_n9_α:
                        mov              qword ptr [rbp + 480], 1
                        mov              rax, qword ptr [rip + .Lx11_0]
                        mov              qword ptr [rbp + 488], rax
                        jmp              xchain0_n10_α
.Lx11_0:
                        .quad            .Lx11_0_s
.Lx11_0_s:
                        .string          "hELLO wORLD"
xchain0_n10_α:
# BOX CALL cap(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+480] -> [zr+448]
                        mov              rax, qword ptr [rbp + 480]
                        mov              qword ptr [rbp + 448], rax
                        mov              rax, qword ptr [rbp + 488]
                        mov              qword ptr [rbp + 456], rax
                        .section         .rodata
.Lbynamefn11:           .string          "cap"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn11]
                        lea              rsi, [rbp + 448]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 432], rax
                        mov              qword ptr [rbp + 440], rdx
                        cmp              eax, 99
                        je               xchain0_n12_α
                        jmp              xchain0_n11_α
xchain0_n10_β:
                        jmp              xchain0_n12_α
# IR_ASSIGN global
xchain0_n11_α:
                        mov              rsi, qword ptr [rbp + 432]
                        mov              rdx, qword ptr [rbp + 440]
                        mov              rdi, qword ptr [rip + .Lx13_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 416], rax
                        mov              qword ptr [rbp + 424], rdx
                        jmp              xchain0_n12_α
.Lx13_0:
                        .quad            .Lx13_0_s
.Lx13_0_s:
                        .string          "OUTPUT"
#         OUTPUT = cap('hELLO wORLD')     ;* Hello world
#         'Hello' icase('hello')          :F(bad_ic1)
# IR_LIT_STRING
xchain0_n12_α:
                        mov              qword ptr [rbp + 672], 1
                        mov              rax, qword ptr [rip + .Lx14_0]
                        mov              qword ptr [rbp + 680], rax
                        jmp              xchain0_n13_α
.Lx14_0:
                        .quad            .Lx14_0_s
.Lx14_0_s:
                        .string          "Hello"
# IR_MATCH_HEAD
xchain0_n13_α:
                        mov              qword ptr [rbp + 568], rbp
                        mov              rdi, qword ptr [rbp + 672]
                        mov              rsi, qword ptr [rbp + 680]
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              qword ptr [rbp + 560], r12
                        mov              qword ptr [rbp + 544], rsp
                        lea              rcx, [rip + g_patstk_sp]
                        mov              rax, qword ptr [rcx + 0]
                        mov              qword ptr [rbp + 536], rax
                        mov              dword ptr [rbp + 528], 0
.Lx16_0:
                        mov              r14d, dword ptr [rbp + 528]
                        jmp              xchain0_n14_α
xchain0_n13_β:
                        add              dword ptr [rbp + 528], 1
                        mov              eax, dword ptr [rbp + 528]
                        cmp              eax, r15d
                        jg               .Lx16_1
                        lea              rcx, [rip + g_anchor]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                        jne              .Lx16_1
                        jmp              .Lx16_0
.Lx16_1:
                        mov              rax, qword ptr [rbp + 536]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rbp + 544]
                        mov              r12, qword ptr [rbp + 560]
                        mov              rbp, qword ptr [rbp + 568]
                        jmp              xchain0_n20_α
# IR_LIT_STRING
xchain0_n14_α:
                        mov              qword ptr [rbp + 640], 1
                        mov              rax, qword ptr [rip + .Lx17_0]
                        mov              qword ptr [rbp + 648], rax
                        jmp              xchain0_n15_α
.Lx17_0:
                        .quad            .Lx17_0_s
.Lx17_0_s:
                        .string          "hello"
xchain0_n15_α:
# BOX CALL icase(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+640] -> [zr+608]
                        mov              rax, qword ptr [rbp + 640]
                        mov              qword ptr [rbp + 608], rax
                        mov              rax, qword ptr [rbp + 648]
                        mov              qword ptr [rbp + 616], rax
                        .section         .rodata
.Lbynamefn16:           .string          "icase"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn16]
                        lea              rsi, [rbp + 608]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 592], rax
                        mov              qword ptr [rbp + 600], rdx
                        cmp              eax, 99
                        je               xchain0_n13_α
                        jmp              xchain0_n16_α
xchain0_n15_β:
                        jmp              xchain0_n13_α
# IR_MATCH_VALUE (operand[0] pattern value, no manufactured name)
xchain0_n16_α:
                        lea              rdi, [rbp + 592]
                        call             rt_match_value_get_pat_fn@PLT
                        test             rax, rax
                        jz               .Lx19_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx19_4]
                        lea              rdx, [rip + .Lx19_5]
                        jmp              rax
.Lx19_4:
                        jmp              xchain0_n17_α
.Lx19_5:
                        jmp              xchain0_n13_β
.Lx19_0:
                        lea              rdi, [rbp + 592]
                        call             rt_match_value_open@PLT
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                        js               xchain0_n13_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx19_6]
                        sub              rsp, 8
                        push             rax
                        jmp              xchain0_n17_α
.Lx19_6:
                        add              rsp, 16
                        jmp              xchain0_n13_β
xchain0_n16_β:
                        jmp              qword ptr [rsp]
# IR_MATCH_RELEASE
xchain0_n17_α:
                        mov              rax, qword ptr [rbp + 536]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rbp + 544]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rsp + 592]
                        mov              rsi, r12
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx21_1:
                        test             rax, rax
                        je               .Lx21_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx21_3]
                        lea              rdx, [rip + .Lx21_4]
                        jmp              rax
.Lx21_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                        jmp              .Lx21_1
.Lx21_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                        jmp              .Lx21_1
.Lx21_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r12, qword ptr [rbp + 560]
                        mov              rbp, qword ptr [rbp + 568]
                        jmp              xchain0_n18_α
#         OUTPUT = 'ok: icase hello'
# IR_LIT_STRING
xchain0_n18_α:
                        mov              qword ptr [rbp + 720], 1
                        mov              rax, qword ptr [rip + .Lx22_0]
                        mov              qword ptr [rbp + 728], rax
                        jmp              xchain0_n19_α
.Lx22_0:
                        .quad            .Lx22_0_s
.Lx22_0_s:
                        .string          "ok: icase hello"
# IR_ASSIGN global
xchain0_n19_α:
                        mov              rsi, qword ptr [rbp + 720]
                        mov              rdx, qword ptr [rbp + 728]
                        mov              rdi, qword ptr [rip + .Lx23_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 704], rax
                        mov              qword ptr [rbp + 712], rdx
                        jmp              xchain0_n20_α
.Lx23_0:
                        .quad            .Lx23_0_s
.Lx23_0_s:
                        .string          "OUTPUT"
#         'HELLO' icase('hello')          :F(bad_ic2)
# IR_LIT_STRING
xchain0_n20_α:
                        mov              qword ptr [rbp + 912], 1
                        mov              rax, qword ptr [rip + .Lx24_0]
                        mov              qword ptr [rbp + 920], rax
                        jmp              xchain0_n21_α
.Lx24_0:
                        .quad            .Lx24_0_s
.Lx24_0_s:
                        .string          "HELLO"
# IR_MATCH_HEAD
xchain0_n21_α:
                        mov              qword ptr [rbp + 808], rbp
                        mov              rdi, qword ptr [rbp + 912]
                        mov              rsi, qword ptr [rbp + 920]
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              qword ptr [rbp + 800], r12
                        mov              qword ptr [rbp + 784], rsp
                        lea              rcx, [rip + g_patstk_sp]
                        mov              rax, qword ptr [rcx + 0]
                        mov              qword ptr [rbp + 776], rax
                        mov              dword ptr [rbp + 768], 0
.Lx26_0:
                        mov              r14d, dword ptr [rbp + 768]
                        jmp              xchain0_n22_α
xchain0_n21_β:
                        add              dword ptr [rbp + 768], 1
                        mov              eax, dword ptr [rbp + 768]
                        cmp              eax, r15d
                        jg               .Lx26_1
                        lea              rcx, [rip + g_anchor]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                        jne              .Lx26_1
                        jmp              .Lx26_0
.Lx26_1:
                        mov              rax, qword ptr [rbp + 776]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rbp + 784]
                        mov              r12, qword ptr [rbp + 800]
                        mov              rbp, qword ptr [rbp + 808]
                        jmp              xchain0_n28_α
# IR_LIT_STRING
xchain0_n22_α:
                        mov              qword ptr [rbp + 880], 1
                        mov              rax, qword ptr [rip + .Lx27_0]
                        mov              qword ptr [rbp + 888], rax
                        jmp              xchain0_n23_α
.Lx27_0:
                        .quad            .Lx27_0_s
.Lx27_0_s:
                        .string          "hello"
xchain0_n23_α:
# BOX CALL icase(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+880] -> [zr+848]
                        mov              rax, qword ptr [rbp + 880]
                        mov              qword ptr [rbp + 848], rax
                        mov              rax, qword ptr [rbp + 888]
                        mov              qword ptr [rbp + 856], rax
                        .section         .rodata
.Lbynamefn24:           .string          "icase"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn24]
                        lea              rsi, [rbp + 848]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 832], rax
                        mov              qword ptr [rbp + 840], rdx
                        cmp              eax, 99
                        je               xchain0_n21_α
                        jmp              xchain0_n24_α
xchain0_n23_β:
                        jmp              xchain0_n21_α
# IR_MATCH_VALUE (operand[0] pattern value, no manufactured name)
xchain0_n24_α:
                        lea              rdi, [rbp + 832]
                        call             rt_match_value_get_pat_fn@PLT
                        test             rax, rax
                        jz               .Lx29_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx29_4]
                        lea              rdx, [rip + .Lx29_5]
                        jmp              rax
.Lx29_4:
                        jmp              xchain0_n25_α
.Lx29_5:
                        jmp              xchain0_n21_β
.Lx29_0:
                        lea              rdi, [rbp + 832]
                        call             rt_match_value_open@PLT
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                        js               xchain0_n21_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx29_6]
                        sub              rsp, 8
                        push             rax
                        jmp              xchain0_n25_α
.Lx29_6:
                        add              rsp, 16
                        jmp              xchain0_n21_β
xchain0_n24_β:
                        jmp              qword ptr [rsp]
# IR_MATCH_RELEASE
xchain0_n25_α:
                        mov              rax, qword ptr [rbp + 776]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rbp + 784]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rsp + 832]
                        mov              rsi, r12
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx31_1:
                        test             rax, rax
                        je               .Lx31_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx31_3]
                        lea              rdx, [rip + .Lx31_4]
                        jmp              rax
.Lx31_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                        jmp              .Lx31_1
.Lx31_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                        jmp              .Lx31_1
.Lx31_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r12, qword ptr [rbp + 800]
                        mov              rbp, qword ptr [rbp + 808]
                        jmp              xchain0_n26_α
#         OUTPUT = 'ok: icase HELLO'
# IR_LIT_STRING
xchain0_n26_α:
                        mov              qword ptr [rbp + 960], 1
                        mov              rax, qword ptr [rip + .Lx32_0]
                        mov              qword ptr [rbp + 968], rax
                        jmp              xchain0_n27_α
.Lx32_0:
                        .quad            .Lx32_0_s
.Lx32_0_s:
                        .string          "ok: icase HELLO"
# IR_ASSIGN global
xchain0_n27_α:
                        mov              rsi, qword ptr [rbp + 960]
                        mov              rdx, qword ptr [rbp + 968]
                        mov              rdi, qword ptr [rip + .Lx33_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 944], rax
                        mov              qword ptr [rbp + 952], rdx
                        jmp              xchain0_n28_α
.Lx33_0:
                        .quad            .Lx33_0_s
.Lx33_0_s:
                        .string          "OUTPUT"
#         'HeLLo' icase('hello')          :F(bad_ic3)
# IR_LIT_STRING
xchain0_n28_α:
                        mov              qword ptr [rbp + 1152], 1
                        mov              rax, qword ptr [rip + .Lx34_0]
                        mov              qword ptr [rbp + 1160], rax
                        jmp              xchain0_n29_α
.Lx34_0:
                        .quad            .Lx34_0_s
.Lx34_0_s:
                        .string          "HeLLo"
# IR_MATCH_HEAD
xchain0_n29_α:
                        mov              qword ptr [rbp + 1048], rbp
                        mov              rdi, qword ptr [rbp + 1152]
                        mov              rsi, qword ptr [rbp + 1160]
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              qword ptr [rbp + 1040], r12
                        mov              qword ptr [rbp + 1024], rsp
                        lea              rcx, [rip + g_patstk_sp]
                        mov              rax, qword ptr [rcx + 0]
                        mov              qword ptr [rbp + 1016], rax
                        mov              dword ptr [rbp + 1008], 0
.Lx36_0:
                        mov              r14d, dword ptr [rbp + 1008]
                        jmp              xchain0_n30_α
xchain0_n29_β:
                        add              dword ptr [rbp + 1008], 1
                        mov              eax, dword ptr [rbp + 1008]
                        cmp              eax, r15d
                        jg               .Lx36_1
                        lea              rcx, [rip + g_anchor]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                        jne              .Lx36_1
                        jmp              .Lx36_0
.Lx36_1:
                        mov              rax, qword ptr [rbp + 1016]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rbp + 1024]
                        mov              r12, qword ptr [rbp + 1040]
                        mov              rbp, qword ptr [rbp + 1048]
                        jmp              xchain0_n36_α
# IR_LIT_STRING
xchain0_n30_α:
                        mov              qword ptr [rbp + 1120], 1
                        mov              rax, qword ptr [rip + .Lx37_0]
                        mov              qword ptr [rbp + 1128], rax
                        jmp              xchain0_n31_α
.Lx37_0:
                        .quad            .Lx37_0_s
.Lx37_0_s:
                        .string          "hello"
xchain0_n31_α:
# BOX CALL icase(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1120] -> [zr+1088]
                        mov              rax, qword ptr [rbp + 1120]
                        mov              qword ptr [rbp + 1088], rax
                        mov              rax, qword ptr [rbp + 1128]
                        mov              qword ptr [rbp + 1096], rax
                        .section         .rodata
.Lbynamefn32:           .string          "icase"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn32]
                        lea              rsi, [rbp + 1088]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1072], rax
                        mov              qword ptr [rbp + 1080], rdx
                        cmp              eax, 99
                        je               xchain0_n29_α
                        jmp              xchain0_n32_α
xchain0_n31_β:
                        jmp              xchain0_n29_α
# IR_MATCH_VALUE (operand[0] pattern value, no manufactured name)
xchain0_n32_α:
                        lea              rdi, [rbp + 1072]
                        call             rt_match_value_get_pat_fn@PLT
                        test             rax, rax
                        jz               .Lx39_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx39_4]
                        lea              rdx, [rip + .Lx39_5]
                        jmp              rax
.Lx39_4:
                        jmp              xchain0_n33_α
.Lx39_5:
                        jmp              xchain0_n29_β
.Lx39_0:
                        lea              rdi, [rbp + 1072]
                        call             rt_match_value_open@PLT
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                        js               xchain0_n29_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx39_6]
                        sub              rsp, 8
                        push             rax
                        jmp              xchain0_n33_α
.Lx39_6:
                        add              rsp, 16
                        jmp              xchain0_n29_β
xchain0_n32_β:
                        jmp              qword ptr [rsp]
# IR_MATCH_RELEASE
xchain0_n33_α:
                        mov              rax, qword ptr [rbp + 1016]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rbp + 1024]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rsp + 1072]
                        mov              rsi, r12
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx41_1:
                        test             rax, rax
                        je               .Lx41_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx41_3]
                        lea              rdx, [rip + .Lx41_4]
                        jmp              rax
.Lx41_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                        jmp              .Lx41_1
.Lx41_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                        jmp              .Lx41_1
.Lx41_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r12, qword ptr [rbp + 1040]
                        mov              rbp, qword ptr [rbp + 1048]
                        jmp              xchain0_n34_α
#         OUTPUT = 'ok: icase HeLLo'
# IR_LIT_STRING
xchain0_n34_α:
                        mov              qword ptr [rbp + 1200], 1
                        mov              rax, qword ptr [rip + .Lx42_0]
                        mov              qword ptr [rbp + 1208], rax
                        jmp              xchain0_n35_α
.Lx42_0:
                        .quad            .Lx42_0_s
.Lx42_0_s:
                        .string          "ok: icase HeLLo"
# IR_ASSIGN global
xchain0_n35_α:
                        mov              rsi, qword ptr [rbp + 1200]
                        mov              rdx, qword ptr [rbp + 1208]
                        mov              rdi, qword ptr [rip + .Lx43_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 1184], rax
                        mov              qword ptr [rbp + 1192], rdx
                        jmp              xchain0_n36_α
.Lx43_0:
                        .quad            .Lx43_0_s
.Lx43_0_s:
                        .string          "OUTPUT"
#         'world' icase('hello')          :S(bad_ic4)
# IR_LIT_STRING
xchain0_n36_α:
                        mov              qword ptr [rbp + 1392], 1
                        mov              rax, qword ptr [rip + .Lx44_0]
                        mov              qword ptr [rbp + 1400], rax
                        jmp              xchain0_n37_α
.Lx44_0:
                        .quad            .Lx44_0_s
.Lx44_0_s:
                        .string          "world"
# IR_MATCH_HEAD
xchain0_n37_α:
                        mov              qword ptr [rbp + 1288], rbp
                        mov              rdi, qword ptr [rbp + 1392]
                        mov              rsi, qword ptr [rbp + 1400]
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              qword ptr [rbp + 1280], r12
                        mov              qword ptr [rbp + 1264], rsp
                        lea              rcx, [rip + g_patstk_sp]
                        mov              rax, qword ptr [rcx + 0]
                        mov              qword ptr [rbp + 1256], rax
                        mov              dword ptr [rbp + 1248], 0
.Lx46_0:
                        mov              r14d, dword ptr [rbp + 1248]
                        jmp              xchain0_n38_α
xchain0_n37_β:
                        add              dword ptr [rbp + 1248], 1
                        mov              eax, dword ptr [rbp + 1248]
                        cmp              eax, r15d
                        jg               .Lx46_1
                        lea              rcx, [rip + g_anchor]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                        jne              .Lx46_1
                        jmp              .Lx46_0
.Lx46_1:
                        mov              rax, qword ptr [rbp + 1256]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rbp + 1264]
                        mov              r12, qword ptr [rbp + 1280]
                        mov              rbp, qword ptr [rbp + 1288]
                        jmp              xchain0_n44_α
# IR_LIT_STRING
xchain0_n38_α:
                        mov              qword ptr [rbp + 1360], 1
                        mov              rax, qword ptr [rip + .Lx47_0]
                        mov              qword ptr [rbp + 1368], rax
                        jmp              xchain0_n39_α
.Lx47_0:
                        .quad            .Lx47_0_s
.Lx47_0_s:
                        .string          "hello"
xchain0_n39_α:
# BOX CALL icase(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1360] -> [zr+1328]
                        mov              rax, qword ptr [rbp + 1360]
                        mov              qword ptr [rbp + 1328], rax
                        mov              rax, qword ptr [rbp + 1368]
                        mov              qword ptr [rbp + 1336], rax
                        .section         .rodata
.Lbynamefn40:           .string          "icase"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn40]
                        lea              rsi, [rbp + 1328]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1312], rax
                        mov              qword ptr [rbp + 1320], rdx
                        cmp              eax, 99
                        je               xchain0_n37_α
                        jmp              xchain0_n40_α
xchain0_n39_β:
                        jmp              xchain0_n37_α
# IR_MATCH_VALUE (operand[0] pattern value, no manufactured name)
xchain0_n40_α:
                        lea              rdi, [rbp + 1312]
                        call             rt_match_value_get_pat_fn@PLT
                        test             rax, rax
                        jz               .Lx49_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx49_4]
                        lea              rdx, [rip + .Lx49_5]
                        jmp              rax
.Lx49_4:
                        jmp              xchain0_n41_α
.Lx49_5:
                        jmp              xchain0_n37_β
.Lx49_0:
                        lea              rdi, [rbp + 1312]
                        call             rt_match_value_open@PLT
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                        js               xchain0_n37_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx49_6]
                        sub              rsp, 8
                        push             rax
                        jmp              xchain0_n41_α
.Lx49_6:
                        add              rsp, 16
                        jmp              xchain0_n37_β
xchain0_n40_β:
                        jmp              qword ptr [rsp]
# IR_MATCH_RELEASE
xchain0_n41_α:
                        mov              rax, qword ptr [rbp + 1256]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rbp + 1264]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rsp + 1312]
                        mov              rsi, r12
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx51_1:
                        test             rax, rax
                        je               .Lx51_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx51_3]
                        lea              rdx, [rip + .Lx51_4]
                        jmp              rax
.Lx51_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                        jmp              .Lx51_1
.Lx51_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                        jmp              .Lx51_1
.Lx51_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r12, qword ptr [rbp + 1280]
                        mov              rbp, qword ptr [rbp + 1288]
                        jmp              xchain0_n42_α
# bad_ic4 OUTPUT = 'FAIL: icase matched wrong string'
# IR_LIT_STRING
xchain0_n42_α:
                        mov              qword ptr [rbp + 1488], 1
                        mov              rax, qword ptr [rip + .Lx52_0]
                        mov              qword ptr [rbp + 1496], rax
                        jmp              xchain0_n43_α
.Lx52_0:
                        .quad            .Lx52_0_s
.Lx52_0_s:
                        .string          "FAIL: icase matched wrong string"
# IR_ASSIGN global
xchain0_n43_α:
                        mov              rsi, qword ptr [rbp + 1488]
                        mov              rdx, qword ptr [rbp + 1496]
                        mov              rdi, qword ptr [rip + .Lx53_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 1472], rax
                        mov              qword ptr [rbp + 1480], rdx
                        jmp              main_γ
.Lx53_0:
                        .quad            .Lx53_0_s
.Lx53_0_s:
                        .string          "OUTPUT"
#         OUTPUT = 'no match ok'          :(END)
# IR_LIT_STRING
xchain0_n44_α:
                        mov              qword ptr [rbp + 1440], 1
                        mov              rax, qword ptr [rip + .Lx54_0]
                        mov              qword ptr [rbp + 1448], rax
                        jmp              xchain0_n45_α
.Lx54_0:
                        .quad            .Lx54_0_s
.Lx54_0_s:
                        .string          "no match ok"
# IR_ASSIGN global
xchain0_n45_α:
                        mov              rsi, qword ptr [rbp + 1440]
                        mov              rdx, qword ptr [rbp + 1448]
                        mov              rdi, qword ptr [rip + .Lx55_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 1424], rax
                        mov              qword ptr [rbp + 1432], rdx
                        jmp              main_γ
.Lx55_0:
                        .quad            .Lx55_0_s
.Lx55_0_s:
                        .string          "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rbp + 1528]
mov rbp, [rsp + 65536]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rbp + 1528]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
mov rbp, [rsp + 65536]
add rsp, 65544
ret

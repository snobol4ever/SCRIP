                        .intel_syntax    noprefix
                        .text
                        .section         .rodata
.Lgvan0:                .string          "eq"
.Lgvan1:                .string          "output"
.Lgvan2:                .string          "trim"
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
                        .global          main_α
                        .global          main_β
                        .global          main_γ
                        .global          main_ω
                        sub              rsp, 65544
                        mov              rdi, rsp
                        mov              ecx, 65544
                        xor              eax, eax
                        rep stosb
                        mov              qword ptr [rsp + 1016], rsp
                        mov              r12, qword ptr [1879048192]
                        mov              [rsp + 65536], rbp
                        mov              rbp, rsp
main_α_body:
#         apply(.eq, 1, 2)           :f(e001)
# IR_LIT_STRING
xchain0_n0_α:
                        mov              qword ptr [rbp + 224], 1
                        mov              rax, qword ptr [rip + .Lx1_0]
                        mov              qword ptr [rbp + 232], rax
                                                                                        jmp   xchain0_n1_α
.Lx1_0:
                        .quad            .Lx1_0_s
.Lx1_0_s:
                        .string          "eq"
xchain0_n1_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+224] -> [zr+192]
                        mov              rax, qword ptr [rbp + 224]
                        mov              qword ptr [rbp + 192], rax
                        mov              rax, qword ptr [rbp + 232]
                        mov              qword ptr [rbp + 200], rax
                        .section         .rodata
.Lrkfn3:                .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn3]
                        lea              rsi, [rbp + 192]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 176], rax
                        mov              qword ptr [rbp + 184], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n3_α
                                                                                        jmp   xchain0_n2_α
xchain0_n1_β:
                                                                                        jmp   xchain0_n3_α
# IR_LIT_INTEGER
xchain0_n2_α:
                        mov              qword ptr [rbp + 256], 6
                        mov              rax, qword ptr [rip + .Lx4_0]
                        mov              qword ptr [rbp + 264], rax
                                                                                        jmp   xchain0_n4_α
.Lx4_0:
                        .quad            1
#         apply(.eq, 1, 1)           :s(e002)
# IR_LIT_STRING
xchain0_n3_α:
                        mov              qword ptr [rbp + 496], 1
                        mov              rax, qword ptr [rip + .Lx5_0]
                        mov              qword ptr [rbp + 504], rax
                                                                                        jmp   xchain0_n5_α
.Lx5_0:
                        .quad            .Lx5_0_s
.Lx5_0_s:
                        .string          "eq"
# IR_LIT_INTEGER
xchain0_n4_α:
                        mov              qword ptr [rbp + 288], 6
                        mov              rax, qword ptr [rip + .Lx6_0]
                        mov              qword ptr [rbp + 296], rax
                                                                                        jmp   xchain0_n6_α
.Lx6_0:
                        .quad            2
xchain0_n5_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+496] -> [zr+464]
                        mov              rax, qword ptr [rbp + 496]
                        mov              qword ptr [rbp + 464], rax
                        mov              rax, qword ptr [rbp + 504]
                        mov              qword ptr [rbp + 472], rax
                        .section         .rodata
.Lrkfn8:                .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn8]
                        lea              rsi, [rbp + 464]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 448], rax
                        mov              qword ptr [rbp + 456], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n8_α
                                                                                        jmp   xchain0_n7_α
xchain0_n5_β:
                                                                                        jmp   xchain0_n8_α
xchain0_n6_α:
# BOX CALL apply(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+176] -> [zr+112]
                        mov              rax, qword ptr [rbp + 176]
                        mov              qword ptr [rbp + 112], rax
                        mov              rax, qword ptr [rbp + 184]
                        mov              qword ptr [rbp + 120], rax
# marshal arg1 = producer-box slot [zr+256] -> [zr+128]
                        mov              rax, qword ptr [rbp + 256]
                        mov              qword ptr [rbp + 128], rax
                        mov              rax, qword ptr [rbp + 264]
                        mov              qword ptr [rbp + 136], rax
# marshal arg2 = producer-box slot [zr+288] -> [zr+144]
                        mov              rax, qword ptr [rbp + 288]
                        mov              qword ptr [rbp + 144], rax
                        mov              rax, qword ptr [rbp + 296]
                        mov              qword ptr [rbp + 152], rax
                        .section         .rodata
.Lbynamefn7:            .string          "apply"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn7]
                        lea              rsi, [rbp + 112]
                        mov              edx, 3
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 96], rax
                        mov              qword ptr [rbp + 104], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n3_α
                                                                                        jmp   xchain0_n9_α
xchain0_n6_β:
                                                                                        jmp   xchain0_n3_α
# IR_LIT_INTEGER
xchain0_n7_α:
                        mov              qword ptr [rbp + 528], 6
                        mov              rax, qword ptr [rip + .Lx10_0]
                        mov              qword ptr [rbp + 536], rax
                                                                                        jmp   xchain0_n10_α
.Lx10_0:
                        .quad            1
#         output = 'FAIL 1018/002: apply(.eq,1,1) should succeed' :(end)
# IR_LIT_STRING
xchain0_n8_α:
                        mov              qword ptr [rbp + 608], 1
                        mov              rax, qword ptr [rip + .Lx11_0]
                        mov              qword ptr [rbp + 616], rax
                                                                                        jmp   xchain0_n11_α
.Lx11_0:
                        .quad            .Lx11_0_s
.Lx11_0_s:
                        .string          "FAIL 1018/002: apply(.eq,1,1) should succeed"
#         output = 'FAIL 1018/001: apply(.eq,1,2) should fail' :(end)
# IR_LIT_STRING
xchain0_n9_α:
                        mov              qword ptr [rbp + 336], 1
                        mov              rax, qword ptr [rip + .Lx12_0]
                        mov              qword ptr [rbp + 344], rax
                                                                                        jmp   xchain0_n12_α
.Lx12_0:
                        .quad            .Lx12_0_s
.Lx12_0_s:
                        .string          "FAIL 1018/001: apply(.eq,1,2) should fail"
# IR_LIT_INTEGER
xchain0_n10_α:
                        mov              qword ptr [rbp + 560], 6
                        mov              rax, qword ptr [rip + .Lx13_0]
                        mov              qword ptr [rbp + 568], rax
                                                                                        jmp   xchain0_n13_α
.Lx13_0:
                        .quad            1
# IR_ASSIGN gva
xchain0_n11_α:
                        mov              rax, qword ptr [rbp + 608]
                        mov              rdx, qword ptr [rbp + 616]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        mov              qword ptr [rbp + 592], rax
                        mov              qword ptr [rbp + 600], rdx
                                                                                        jmp   main_γ
# IR_ASSIGN gva
xchain0_n12_α:
                        mov              rax, qword ptr [rbp + 336]
                        mov              rdx, qword ptr [rbp + 344]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        mov              qword ptr [rbp + 320], rax
                        mov              qword ptr [rbp + 328], rdx
                                                                                        jmp   main_γ
xchain0_n13_α:
# BOX CALL apply(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+448] -> [zr+384]
                        mov              rax, qword ptr [rbp + 448]
                        mov              qword ptr [rbp + 384], rax
                        mov              rax, qword ptr [rbp + 456]
                        mov              qword ptr [rbp + 392], rax
# marshal arg1 = producer-box slot [zr+528] -> [zr+400]
                        mov              rax, qword ptr [rbp + 528]
                        mov              qword ptr [rbp + 400], rax
                        mov              rax, qword ptr [rbp + 536]
                        mov              qword ptr [rbp + 408], rax
# marshal arg2 = producer-box slot [zr+560] -> [zr+416]
                        mov              rax, qword ptr [rbp + 560]
                        mov              qword ptr [rbp + 416], rax
                        mov              rax, qword ptr [rbp + 568]
                        mov              qword ptr [rbp + 424], rax
                        .section         .rodata
.Lbynamefn14:           .string          "apply"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn14]
                        lea              rsi, [rbp + 384]
                        mov              edx, 3
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 368], rax
                        mov              qword ptr [rbp + 376], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n8_α
                                                                                        jmp   xchain0_n14_α
xchain0_n13_β:
                                                                                        jmp   xchain0_n8_α
#         differ(apply(.trim, 'abc   '), 'abc')                   :f(e003)
# IR_LIT_STRING
xchain0_n14_α:
                        mov              qword ptr [rbp + 816], 1
                        mov              rax, qword ptr [rip + .Lx17_0]
                        mov              qword ptr [rbp + 824], rax
                                                                                        jmp   xchain0_n15_α
.Lx17_0:
                        .quad            .Lx17_0_s
.Lx17_0_s:
                        .string          "trim"
xchain0_n15_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+816] -> [zr+784]
                        mov              rax, qword ptr [rbp + 816]
                        mov              qword ptr [rbp + 784], rax
                        mov              rax, qword ptr [rbp + 824]
                        mov              qword ptr [rbp + 792], rax
                        .section         .rodata
.Lrkfn19:               .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn19]
                        lea              rsi, [rbp + 784]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 768], rax
                        mov              qword ptr [rbp + 776], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n17_α
                                                                                        jmp   xchain0_n16_α
xchain0_n15_β:
                                                                                        jmp   xchain0_n17_α
# IR_LIT_STRING
xchain0_n16_α:
                        mov              qword ptr [rbp + 848], 1
                        mov              rax, qword ptr [rip + .Lx20_0]
                        mov              qword ptr [rbp + 856], rax
                                                                                        jmp   xchain0_n18_α
.Lx20_0:
                        .quad            .Lx20_0_s
.Lx20_0_s:
                        .string          "abc   "
#         output = 'PASS 1018_apply (3/3)'
# IR_LIT_STRING
xchain0_n17_α:
                        mov              qword ptr [rbp + 976], 1
                        mov              rax, qword ptr [rip + .Lx21_0]
                        mov              qword ptr [rbp + 984], rax
                                                                                        jmp   xchain0_n19_α
.Lx21_0:
                        .quad            .Lx21_0_s
.Lx21_0_s:
                        .string          "PASS 1018_apply (3/3)"
xchain0_n18_α:
# BOX CALL apply(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+768] -> [zr+720]
                        mov              rax, qword ptr [rbp + 768]
                        mov              qword ptr [rbp + 720], rax
                        mov              rax, qword ptr [rbp + 776]
                        mov              qword ptr [rbp + 728], rax
# marshal arg1 = producer-box slot [zr+848] -> [zr+736]
                        mov              rax, qword ptr [rbp + 848]
                        mov              qword ptr [rbp + 736], rax
                        mov              rax, qword ptr [rbp + 856]
                        mov              qword ptr [rbp + 744], rax
                        .section         .rodata
.Lbynamefn19:           .string          "apply"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn19]
                        lea              rsi, [rbp + 720]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 704], rax
                        mov              qword ptr [rbp + 712], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n17_α
                                                                                        jmp   xchain0_n20_α
xchain0_n18_β:
                                                                                        jmp   xchain0_n17_α
# IR_ASSIGN gva
xchain0_n19_α:
                        mov              rax, qword ptr [rbp + 976]
                        mov              rdx, qword ptr [rbp + 984]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        mov              qword ptr [rbp + 960], rax
                        mov              qword ptr [rbp + 968], rdx
                                                                                        jmp   main_γ
# IR_LIT_STRING
xchain0_n20_α:
                        mov              qword ptr [rbp + 880], 1
                        mov              rax, qword ptr [rip + .Lx24_0]
                        mov              qword ptr [rbp + 888], rax
                                                                                        jmp   xchain0_n21_α
.Lx24_0:
                        .quad            .Lx24_0_s
.Lx24_0_s:
                        .string          "abc"
xchain0_n21_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+704] -> [zr+656]
                        mov              rax, qword ptr [rbp + 704]
                        mov              qword ptr [rbp + 656], rax
                        mov              rax, qword ptr [rbp + 712]
                        mov              qword ptr [rbp + 664], rax
# marshal arg1 = producer-box slot [zr+880] -> [zr+672]
                        mov              rax, qword ptr [rbp + 880]
                        mov              qword ptr [rbp + 672], rax
                        mov              rax, qword ptr [rbp + 888]
                        mov              qword ptr [rbp + 680], rax
                        .section         .rodata
.Lbynamefn22:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn22]
                        lea              rsi, [rbp + 656]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 640], rax
                        mov              qword ptr [rbp + 648], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n17_α
                                                                                        jmp   xchain0_n22_α
xchain0_n21_β:
                                                                                        jmp   xchain0_n17_α
#         output = 'FAIL 1018/003: apply(.trim,...)'     :(end)
# IR_LIT_STRING
xchain0_n22_α:
                        mov              qword ptr [rbp + 928], 1
                        mov              rax, qword ptr [rip + .Lx26_0]
                        mov              qword ptr [rbp + 936], rax
                                                                                        jmp   xchain0_n23_α
.Lx26_0:
                        .quad            .Lx26_0_s
.Lx26_0_s:
                        .string          "FAIL 1018/003: apply(.trim,...)"
# IR_ASSIGN gva
xchain0_n23_α:
                        mov              rax, qword ptr [rbp + 928]
                        mov              rdx, qword ptr [rbp + 936]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        mov              qword ptr [rbp + 912], rax
                        mov              qword ptr [rbp + 920], rdx
                                                                                        jmp   main_γ
main_β:
                                                                                        jmp   main_ω
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        mov              rsp, qword ptr [rbp + 1016]
                        mov              rbp, [rsp + 65536]
                        add              rsp, 65544
                        ret
main_ω:
                        mov              rsp, qword ptr [rbp + 1016]
                        mov              dword ptr [rsp+0], 99
                        mov              dword ptr [rsp+4], 0
                        mov              qword ptr [rsp+8], 0
                        mov              eax, 99
                        xor              edx, edx
                        mov              rbp, [rsp + 65536]
                        add              rsp, 65544
                        ret

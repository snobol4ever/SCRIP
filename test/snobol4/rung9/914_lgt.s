                        .intel_syntax    noprefix
                        .text
                        .section         .rodata
.Lgvan0:                .string          "output"
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
                        mov              qword ptr [rsp + 1032], rsp
                        mov              r12, qword ptr [1879048192]
                        mov              [rsp + 65536], rbp
                        mov              rbp, rsp
main_α_body:
#=======================================================================================================================
#         lgt('abc', 'xyz')          :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n0_α:
                        mov              qword ptr [rbp + 160], 1
                        mov              rax, qword ptr [rip + .Lx1_0]
                        mov              qword ptr [rbp + 168], rax
                                                                                        jmp   xchain0_n1_α
.Lx1_0:
                        .quad            .Lx1_0_s
.Lx1_0_s:
                        .string          "abc"
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n1_α:
                        mov              qword ptr [rbp + 192], 1
                        mov              rax, qword ptr [rip + .Lx2_0]
                        mov              qword ptr [rbp + 200], rax
                                                                                        jmp   xchain0_n2_α
.Lx2_0:
                        .quad            .Lx2_0_s
.Lx2_0_s:
                        .string          "xyz"
#-----------------------------------------------------------------------------------------------------------------------
xchain0_n2_α:
# BOX CALL lgt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
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
.Lbynamefn3:            .string          "lgt"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn3]
                        lea              rsi, [rbp + 112]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 96], rax
                        mov              qword ptr [rbp + 104], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n4_α
                                                                                        jmp   xchain0_n3_α
xchain0_n2_β:
                                                                                        jmp   xchain0_n4_α
#=======================================================================================================================
#         output = 'FAIL 914/001: lgt(abc,xyz) should fail'   :(end)
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n3_α:
                        mov              qword ptr [rbp + 240], 1
                        mov              rax, qword ptr [rip + .Lx4_0]
                        mov              qword ptr [rbp + 248], rax
                                                                                        jmp   xchain0_n5_α
.Lx4_0:
                        .quad            .Lx4_0_s
.Lx4_0_s:
                        .string          "FAIL 914/001: lgt(abc,xyz) should fail"
#=======================================================================================================================
#         lgt('abc', 'abc')          :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n4_α:
                        mov              qword ptr [rbp + 336], 1
                        mov              rax, qword ptr [rip + .Lx5_0]
                        mov              qword ptr [rbp + 344], rax
                                                                                        jmp   xchain0_n6_α
.Lx5_0:
                        .quad            .Lx5_0_s
.Lx5_0_s:
                        .string          "abc"
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN gva
xchain0_n5_α:
                        mov              rax, qword ptr [rbp + 240]
                        mov              rdx, qword ptr [rbp + 248]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        mov              qword ptr [rbp + 224], rax
                        mov              qword ptr [rbp + 232], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n6_α:
                        mov              qword ptr [rbp + 368], 1
                        mov              rax, qword ptr [rip + .Lx7_0]
                        mov              qword ptr [rbp + 376], rax
                                                                                        jmp   xchain0_n7_α
.Lx7_0:
                        .quad            .Lx7_0_s
.Lx7_0_s:
                        .string          "abc"
#-----------------------------------------------------------------------------------------------------------------------
xchain0_n7_α:
# BOX CALL lgt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+336] -> [zr+288]
                        mov              rax, qword ptr [rbp + 336]
                        mov              qword ptr [rbp + 288], rax
                        mov              rax, qword ptr [rbp + 344]
                        mov              qword ptr [rbp + 296], rax
# marshal arg1 = producer-box slot [zr+368] -> [zr+304]
                        mov              rax, qword ptr [rbp + 368]
                        mov              qword ptr [rbp + 304], rax
                        mov              rax, qword ptr [rbp + 376]
                        mov              qword ptr [rbp + 312], rax
                        .section         .rodata
.Lbynamefn8:            .string          "lgt"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn8]
                        lea              rsi, [rbp + 288]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 272], rax
                        mov              qword ptr [rbp + 280], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n9_α
                                                                                        jmp   xchain0_n8_α
xchain0_n7_β:
                                                                                        jmp   xchain0_n9_α
#=======================================================================================================================
#         output = 'FAIL 914/002: lgt(abc,abc) should fail'   :(end)
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n8_α:
                        mov              qword ptr [rbp + 416], 1
                        mov              rax, qword ptr [rip + .Lx9_0]
                        mov              qword ptr [rbp + 424], rax
                                                                                        jmp   xchain0_n10_α
.Lx9_0:
                        .quad            .Lx9_0_s
.Lx9_0_s:
                        .string          "FAIL 914/002: lgt(abc,abc) should fail"
#=======================================================================================================================
#         lgt('xyz', 'abc')          :s(e003)
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n9_α:
                        mov              qword ptr [rbp + 512], 1
                        mov              rax, qword ptr [rip + .Lx10_0]
                        mov              qword ptr [rbp + 520], rax
                                                                                        jmp   xchain0_n11_α
.Lx10_0:
                        .quad            .Lx10_0_s
.Lx10_0_s:
                        .string          "xyz"
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN gva
xchain0_n10_α:
                        mov              rax, qword ptr [rbp + 416]
                        mov              rdx, qword ptr [rbp + 424]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        mov              qword ptr [rbp + 400], rax
                        mov              qword ptr [rbp + 408], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n11_α:
                        mov              qword ptr [rbp + 544], 1
                        mov              rax, qword ptr [rip + .Lx12_0]
                        mov              qword ptr [rbp + 552], rax
                                                                                        jmp   xchain0_n12_α
.Lx12_0:
                        .quad            .Lx12_0_s
.Lx12_0_s:
                        .string          "abc"
#-----------------------------------------------------------------------------------------------------------------------
xchain0_n12_α:
# BOX CALL lgt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+512] -> [zr+464]
                        mov              rax, qword ptr [rbp + 512]
                        mov              qword ptr [rbp + 464], rax
                        mov              rax, qword ptr [rbp + 520]
                        mov              qword ptr [rbp + 472], rax
# marshal arg1 = producer-box slot [zr+544] -> [zr+480]
                        mov              rax, qword ptr [rbp + 544]
                        mov              qword ptr [rbp + 480], rax
                        mov              rax, qword ptr [rbp + 552]
                        mov              qword ptr [rbp + 488], rax
                        .section         .rodata
.Lbynamefn13:           .string          "lgt"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn13]
                        lea              rsi, [rbp + 464]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 448], rax
                        mov              qword ptr [rbp + 456], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n14_α
                                                                                        jmp   xchain0_n13_α
xchain0_n12_β:
                                                                                        jmp   xchain0_n14_α
#=======================================================================================================================
#         lgt('', 'abc')             :f(e004)
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n13_α:
                        mov              qword ptr [rbp + 688], 1
                        mov              rax, qword ptr [rip + .Lx14_0]
                        mov              qword ptr [rbp + 696], rax
                                                                                        jmp   xchain0_n15_α
.Lx14_0:
                        .quad            .Lx14_0_s
.Lx14_0_s:
                        .string          ""
#=======================================================================================================================
#         output = 'FAIL 914/003: lgt(xyz,abc) should succeed' :(end)
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n14_α:
                        mov              qword ptr [rbp + 592], 1
                        mov              rax, qword ptr [rip + .Lx15_0]
                        mov              qword ptr [rbp + 600], rax
                                                                                        jmp   xchain0_n16_α
.Lx15_0:
                        .quad            .Lx15_0_s
.Lx15_0_s:
                        .string          "FAIL 914/003: lgt(xyz,abc) should succeed"
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n15_α:
                        mov              qword ptr [rbp + 720], 1
                        mov              rax, qword ptr [rip + .Lx16_0]
                        mov              qword ptr [rbp + 728], rax
                                                                                        jmp   xchain0_n17_α
.Lx16_0:
                        .quad            .Lx16_0_s
.Lx16_0_s:
                        .string          "abc"
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN gva
xchain0_n16_α:
                        mov              rax, qword ptr [rbp + 592]
                        mov              rdx, qword ptr [rbp + 600]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        mov              qword ptr [rbp + 576], rax
                        mov              qword ptr [rbp + 584], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
xchain0_n17_α:
# BOX CALL lgt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+688] -> [zr+640]
                        mov              rax, qword ptr [rbp + 688]
                        mov              qword ptr [rbp + 640], rax
                        mov              rax, qword ptr [rbp + 696]
                        mov              qword ptr [rbp + 648], rax
# marshal arg1 = producer-box slot [zr+720] -> [zr+656]
                        mov              rax, qword ptr [rbp + 720]
                        mov              qword ptr [rbp + 656], rax
                        mov              rax, qword ptr [rbp + 728]
                        mov              qword ptr [rbp + 664], rax
                        .section         .rodata
.Lbynamefn18:           .string          "lgt"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn18]
                        lea              rsi, [rbp + 640]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 624], rax
                        mov              qword ptr [rbp + 632], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n19_α
                                                                                        jmp   xchain0_n18_α
xchain0_n17_β:
                                                                                        jmp   xchain0_n19_α
#=======================================================================================================================
#         output = 'FAIL 914/004: lgt(null,abc) should fail'  :(end)
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n18_α:
                        mov              qword ptr [rbp + 768], 1
                        mov              rax, qword ptr [rip + .Lx19_0]
                        mov              qword ptr [rbp + 776], rax
                                                                                        jmp   xchain0_n20_α
.Lx19_0:
                        .quad            .Lx19_0_s
.Lx19_0_s:
                        .string          "FAIL 914/004: lgt(null,abc) should fail"
#=======================================================================================================================
#         lgt('abc', '')             :s(e005)
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n19_α:
                        mov              qword ptr [rbp + 864], 1
                        mov              rax, qword ptr [rip + .Lx20_0]
                        mov              qword ptr [rbp + 872], rax
                                                                                        jmp   xchain0_n21_α
.Lx20_0:
                        .quad            .Lx20_0_s
.Lx20_0_s:
                        .string          "abc"
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN gva
xchain0_n20_α:
                        mov              rax, qword ptr [rbp + 768]
                        mov              rdx, qword ptr [rbp + 776]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        mov              qword ptr [rbp + 752], rax
                        mov              qword ptr [rbp + 760], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n21_α:
                        mov              qword ptr [rbp + 896], 1
                        mov              rax, qword ptr [rip + .Lx22_0]
                        mov              qword ptr [rbp + 904], rax
                                                                                        jmp   xchain0_n22_α
.Lx22_0:
                        .quad            .Lx22_0_s
.Lx22_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
xchain0_n22_α:
# BOX CALL lgt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+864] -> [zr+816]
                        mov              rax, qword ptr [rbp + 864]
                        mov              qword ptr [rbp + 816], rax
                        mov              rax, qword ptr [rbp + 872]
                        mov              qword ptr [rbp + 824], rax
# marshal arg1 = producer-box slot [zr+896] -> [zr+832]
                        mov              rax, qword ptr [rbp + 896]
                        mov              qword ptr [rbp + 832], rax
                        mov              rax, qword ptr [rbp + 904]
                        mov              qword ptr [rbp + 840], rax
                        .section         .rodata
.Lbynamefn23:           .string          "lgt"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn23]
                        lea              rsi, [rbp + 816]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 800], rax
                        mov              qword ptr [rbp + 808], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n24_α
                                                                                        jmp   xchain0_n23_α
xchain0_n22_β:
                                                                                        jmp   xchain0_n24_α
#=======================================================================================================================
#         output = 'PASS 914_lgt (5/5)'
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n23_α:
                        mov              qword ptr [rbp + 992], 1
                        mov              rax, qword ptr [rip + .Lx24_0]
                        mov              qword ptr [rbp + 1000], rax
                                                                                        jmp   xchain0_n25_α
.Lx24_0:
                        .quad            .Lx24_0_s
.Lx24_0_s:
                        .string          "PASS 914_lgt (5/5)"
#=======================================================================================================================
#         output = 'FAIL 914/005: lgt(abc,null) should succeed' :(end)
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n24_α:
                        mov              qword ptr [rbp + 944], 1
                        mov              rax, qword ptr [rip + .Lx25_0]
                        mov              qword ptr [rbp + 952], rax
                                                                                        jmp   xchain0_n26_α
.Lx25_0:
                        .quad            .Lx25_0_s
.Lx25_0_s:
                        .string          "FAIL 914/005: lgt(abc,null) should succeed"
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN gva
xchain0_n25_α:
                        mov              rax, qword ptr [rbp + 992]
                        mov              rdx, qword ptr [rbp + 1000]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        mov              qword ptr [rbp + 976], rax
                        mov              qword ptr [rbp + 984], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN gva
xchain0_n26_α:
                        mov              rax, qword ptr [rbp + 944]
                        mov              rdx, qword ptr [rbp + 952]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        mov              qword ptr [rbp + 928], rax
                        mov              qword ptr [rbp + 936], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        mov              rsp, qword ptr [rbp + 1032]
                        mov              rbp, [rsp + 65536]
                        add              rsp, 65544
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              rsp, qword ptr [rbp + 1032]
                        mov              dword ptr [rsp+0], 99
                        mov              dword ptr [rsp+4], 0
                        mov              qword ptr [rsp+8], 0
                        mov              eax, 99
                        xor              edx, edx
                        mov              rbp, [rsp + 65536]
                        add              rsp, 65544
                        ret
                        .section         .note.GNU-stack,"",@progbits

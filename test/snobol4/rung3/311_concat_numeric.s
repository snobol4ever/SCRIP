  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "output"
  .align 8
__gva_names:
  .quad .Lgvan0
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  mov edi, 1
  call rt_gva_island@PLT
  mov rsi, rax
  lea rdi, [rip + __gva_names]
  mov edx, 1
  call gva_register@PLT
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
  mov qword ptr [rsp + 936], rsp
  mov r12, qword ptr [1879048192]
  mov [rsp + 65536], rbp
  mov rbp, rsp
main_α_body:
#         differ(1 2, '12')                   :f(e001)
# IR_LIT_INTEGER
xchain0_n0_α:
                        mov              qword ptr [rbp + 192], 6
                        mov              rax, qword ptr [rip + .Lx1_0]
                        mov              qword ptr [rbp + 200], rax
                        jmp              xchain0_n1_α
.Lx1_0:
                        .quad            1
# IR_LIT_INTEGER
xchain0_n1_α:
                        mov              qword ptr [rbp + 224], 6
                        mov              rax, qword ptr [rip + .Lx2_0]
                        mov              qword ptr [rbp + 232], rax
                        jmp              xchain0_n2_α
.Lx2_0:
                        .quad            2
xchain0_n2_α:
# IR_BINOP_CONCAT
                        mov              rdi, qword ptr [rbp + 192]
                        mov              rsi, qword ptr [rbp + 200]
                        mov              rdx, qword ptr [rbp + 224]
                        mov              rcx, qword ptr [rbp + 232]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 160], rax
                        mov              qword ptr [rbp + 168], rdx
                        jmp              xchain0_n3_α
# IR_LIT_STRING
xchain0_n3_α:
                        mov              qword ptr [rbp + 256], 1
                        mov              rax, qword ptr [rip + .Lx4_0]
                        mov              qword ptr [rbp + 264], rax
                        jmp              xchain0_n5_α
.Lx4_0:
                        .quad            .Lx4_0_s
.Lx4_0_s:
                        .string          "12"
#         differ(2 2 2, '222')                   :f(e002)
# IR_LIT_INTEGER
xchain0_n4_α:
                        mov              qword ptr [rbp + 464], 6
                        mov              rax, qword ptr [rip + .Lx5_0]
                        mov              qword ptr [rbp + 472], rax
                        jmp              xchain0_n6_α
.Lx5_0:
                        .quad            2
xchain0_n5_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+160] -> [zr+112]
                        mov              rax, qword ptr [rbp + 160]
                        mov              qword ptr [rbp + 112], rax
                        mov              rax, qword ptr [rbp + 168]
                        mov              qword ptr [rbp + 120], rax
# marshal arg1 = producer-box slot [zr+256] -> [zr+128]
                        mov              rax, qword ptr [rbp + 256]
                        mov              qword ptr [rbp + 128], rax
                        mov              rax, qword ptr [rbp + 264]
                        mov              qword ptr [rbp + 136], rax
                        .section         .rodata
.Lbynamefn6:            .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn6]
                        lea              rsi, [rbp + 112]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 96], rax
                        mov              qword ptr [rbp + 104], rdx
                        cmp              eax, 99
                        je               xchain0_n4_α
                        jmp              xchain0_n7_α
xchain0_n5_β:
                        jmp              xchain0_n4_α
# IR_LIT_INTEGER
xchain0_n6_α:
                        mov              qword ptr [rbp + 496], 6
                        mov              rax, qword ptr [rip + .Lx7_0]
                        mov              qword ptr [rbp + 504], rax
                        jmp              xchain0_n8_α
.Lx7_0:
                        .quad            2
#         output = 'FAIL 311/001: int int concat'         :(end)
# IR_LIT_STRING
xchain0_n7_α:
                        mov              qword ptr [rbp + 304], 1
                        mov              rax, qword ptr [rip + .Lx8_0]
                        mov              qword ptr [rbp + 312], rax
                        jmp              xchain0_n9_α
.Lx8_0:
                        .quad            .Lx8_0_s
.Lx8_0_s:
                        .string          "FAIL 311/001: int int concat"
xchain0_n8_α:
# IR_BINOP_CONCAT
                        mov              rdi, qword ptr [rbp + 464]
                        mov              rsi, qword ptr [rbp + 472]
                        mov              rdx, qword ptr [rbp + 496]
                        mov              rcx, qword ptr [rbp + 504]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 432], rax
                        mov              qword ptr [rbp + 440], rdx
                        jmp              xchain0_n10_α
# IR_ASSIGN gva
xchain0_n9_α:
                        mov              rax, qword ptr [rbp + 304]
                        mov              rdx, qword ptr [rbp + 312]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        mov              qword ptr [rbp + 288], rax
                        mov              qword ptr [rbp + 296], rdx
                        jmp              main_γ
# IR_LIT_INTEGER
xchain0_n10_α:
                        mov              qword ptr [rbp + 528], 6
                        mov              rax, qword ptr [rip + .Lx11_0]
                        mov              qword ptr [rbp + 536], rax
                        jmp              xchain0_n12_α
.Lx11_0:
                        .quad            2
#         differ(1 3.4, '13.4')                   :f(e003)
# IR_LIT_INTEGER
xchain0_n11_α:
                        mov              qword ptr [rbp + 736], 6
                        mov              rax, qword ptr [rip + .Lx12_0]
                        mov              qword ptr [rbp + 744], rax
                        jmp              xchain0_n13_α
.Lx12_0:
                        .quad            1
xchain0_n12_α:
# IR_BINOP_CONCAT
                        mov              rdi, qword ptr [rbp + 432]
                        mov              rsi, qword ptr [rbp + 440]
                        mov              rdx, qword ptr [rbp + 528]
                        mov              rcx, qword ptr [rbp + 536]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 400], rax
                        mov              qword ptr [rbp + 408], rdx
                        jmp              xchain0_n14_α
# IR_LIT_REAL
xchain0_n13_α:
                        mov              qword ptr [rbp + 768], 7
                        mov              rax, qword ptr [rip + .Lx14_0]
                        mov              qword ptr [rbp + 776], rax
                        jmp              xchain0_n15_α
.Lx14_0:
                        .quad            4614838538166547251
# IR_LIT_STRING
xchain0_n14_α:
                        mov              qword ptr [rbp + 560], 1
                        mov              rax, qword ptr [rip + .Lx15_0]
                        mov              qword ptr [rbp + 568], rax
                        jmp              xchain0_n16_α
.Lx15_0:
                        .quad            .Lx15_0_s
.Lx15_0_s:
                        .string          "222"
xchain0_n15_α:
# IR_BINOP_CONCAT
                        mov              rdi, qword ptr [rbp + 736]
                        mov              rsi, qword ptr [rbp + 744]
                        mov              rdx, qword ptr [rbp + 768]
                        mov              rcx, qword ptr [rbp + 776]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 704], rax
                        mov              qword ptr [rbp + 712], rdx
                        jmp              xchain0_n17_α
xchain0_n16_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+400] -> [zr+352]
                        mov              rax, qword ptr [rbp + 400]
                        mov              qword ptr [rbp + 352], rax
                        mov              rax, qword ptr [rbp + 408]
                        mov              qword ptr [rbp + 360], rax
# marshal arg1 = producer-box slot [zr+560] -> [zr+368]
                        mov              rax, qword ptr [rbp + 560]
                        mov              qword ptr [rbp + 368], rax
                        mov              rax, qword ptr [rbp + 568]
                        mov              qword ptr [rbp + 376], rax
                        .section         .rodata
.Lbynamefn17:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn17]
                        lea              rsi, [rbp + 352]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 336], rax
                        mov              qword ptr [rbp + 344], rdx
                        cmp              eax, 99
                        je               xchain0_n11_α
                        jmp              xchain0_n19_α
xchain0_n16_β:
                        jmp              xchain0_n11_α
# IR_LIT_STRING
xchain0_n17_α:
                        mov              qword ptr [rbp + 800], 1
                        mov              rax, qword ptr [rip + .Lx18_0]
                        mov              qword ptr [rbp + 808], rax
                        jmp              xchain0_n20_α
.Lx18_0:
                        .quad            .Lx18_0_s
.Lx18_0_s:
                        .string          "13.4"
#         output = 'PASS 311_concat_numeric (3/3)'
# IR_LIT_STRING
xchain0_n18_α:
                        mov              qword ptr [rbp + 896], 1
                        mov              rax, qword ptr [rip + .Lx19_0]
                        mov              qword ptr [rbp + 904], rax
                        jmp              xchain0_n21_α
.Lx19_0:
                        .quad            .Lx19_0_s
.Lx19_0_s:
                        .string          "PASS 311_concat_numeric (3/3)"
#         output = 'FAIL 311/002: three int concat'       :(end)
# IR_LIT_STRING
xchain0_n19_α:
                        mov              qword ptr [rbp + 608], 1
                        mov              rax, qword ptr [rip + .Lx20_0]
                        mov              qword ptr [rbp + 616], rax
                        jmp              xchain0_n22_α
.Lx20_0:
                        .quad            .Lx20_0_s
.Lx20_0_s:
                        .string          "FAIL 311/002: three int concat"
xchain0_n20_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+704] -> [zr+656]
                        mov              rax, qword ptr [rbp + 704]
                        mov              qword ptr [rbp + 656], rax
                        mov              rax, qword ptr [rbp + 712]
                        mov              qword ptr [rbp + 664], rax
# marshal arg1 = producer-box slot [zr+800] -> [zr+672]
                        mov              rax, qword ptr [rbp + 800]
                        mov              qword ptr [rbp + 672], rax
                        mov              rax, qword ptr [rbp + 808]
                        mov              qword ptr [rbp + 680], rax
                        .section         .rodata
.Lbynamefn21:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn21]
                        lea              rsi, [rbp + 656]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 640], rax
                        mov              qword ptr [rbp + 648], rdx
                        cmp              eax, 99
                        je               xchain0_n18_α
                        jmp              xchain0_n23_α
xchain0_n20_β:
                        jmp              xchain0_n18_α
# IR_ASSIGN gva
xchain0_n21_α:
                        mov              rax, qword ptr [rbp + 896]
                        mov              rdx, qword ptr [rbp + 904]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        mov              qword ptr [rbp + 880], rax
                        mov              qword ptr [rbp + 888], rdx
                        jmp              main_γ
# IR_ASSIGN gva
xchain0_n22_α:
                        mov              rax, qword ptr [rbp + 608]
                        mov              rdx, qword ptr [rbp + 616]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        mov              qword ptr [rbp + 592], rax
                        mov              qword ptr [rbp + 600], rdx
                        jmp              main_γ
#         output = 'FAIL 311/003: int real concat'        :(end)
# IR_LIT_STRING
xchain0_n23_α:
                        mov              qword ptr [rbp + 848], 1
                        mov              rax, qword ptr [rip + .Lx24_0]
                        mov              qword ptr [rbp + 856], rax
                        jmp              xchain0_n24_α
.Lx24_0:
                        .quad            .Lx24_0_s
.Lx24_0_s:
                        .string          "FAIL 311/003: int real concat"
# IR_ASSIGN gva
xchain0_n24_α:
                        mov              rax, qword ptr [rbp + 848]
                        mov              rdx, qword ptr [rbp + 856]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        mov              qword ptr [rbp + 832], rax
                        mov              qword ptr [rbp + 840], rdx
                        jmp              main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rbp + 936]
mov rbp, [rsp + 65536]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rbp + 936]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
mov rbp, [rsp + 65536]
add rsp, 65544
ret

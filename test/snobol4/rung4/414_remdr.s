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
  mov qword ptr [rsp + 696], rsp
  mov r12, qword ptr [1879048192]
  mov [rsp + 65536], rbp
  mov rbp, rsp
main_α_body:
#         differ(remdr(10, 3), 1)                   :f(e001)
# IR_LIT_INTEGER
xchain0_n0_α:
                        mov              qword ptr [rbp + 224], 6
                        mov              rax, qword ptr [rip + .Lx1_0]
                        mov              qword ptr [rbp + 232], rax
                        jmp              xchain0_n1_α
.Lx1_0:
                        .quad            10
# IR_LIT_INTEGER
xchain0_n1_α:
                        mov              qword ptr [rbp + 256], 6
                        mov              rax, qword ptr [rip + .Lx2_0]
                        mov              qword ptr [rbp + 264], rax
                        jmp              xchain0_n2_α
.Lx2_0:
                        .quad            3
xchain0_n2_α:
# BOX CALL remdr(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+224] -> [zr+176]
                        mov              rax, qword ptr [rbp + 224]
                        mov              qword ptr [rbp + 176], rax
                        mov              rax, qword ptr [rbp + 232]
                        mov              qword ptr [rbp + 184], rax
# marshal arg1 = producer-box slot [zr+256] -> [zr+192]
                        mov              rax, qword ptr [rbp + 256]
                        mov              qword ptr [rbp + 192], rax
                        mov              rax, qword ptr [rbp + 264]
                        mov              qword ptr [rbp + 200], rax
                        .section         .rodata
.Lbynamefn3:            .string          "remdr"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn3]
                        lea              rsi, [rbp + 176]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 160], rax
                        mov              qword ptr [rbp + 168], rdx
                        cmp              eax, 99
                        je               xchain0_n4_α
                        jmp              xchain0_n3_α
xchain0_n2_β:
                        jmp              xchain0_n4_α
# IR_LIT_INTEGER
xchain0_n3_α:
                        mov              qword ptr [rbp + 288], 6
                        mov              rax, qword ptr [rip + .Lx4_0]
                        mov              qword ptr [rbp + 296], rax
                        jmp              xchain0_n5_α
.Lx4_0:
                        .quad            1
#         differ(remdr(11, 10), 1)                   :f(e002)
# IR_LIT_INTEGER
xchain0_n4_α:
                        mov              qword ptr [rbp + 496], 6
                        mov              rax, qword ptr [rip + .Lx5_0]
                        mov              qword ptr [rbp + 504], rax
                        jmp              xchain0_n6_α
.Lx5_0:
                        .quad            11
xchain0_n5_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+160] -> [zr+112]
                        mov              rax, qword ptr [rbp + 160]
                        mov              qword ptr [rbp + 112], rax
                        mov              rax, qword ptr [rbp + 168]
                        mov              qword ptr [rbp + 120], rax
# marshal arg1 = producer-box slot [zr+288] -> [zr+128]
                        mov              rax, qword ptr [rbp + 288]
                        mov              qword ptr [rbp + 128], rax
                        mov              rax, qword ptr [rbp + 296]
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
                        mov              qword ptr [rbp + 528], 6
                        mov              rax, qword ptr [rip + .Lx7_0]
                        mov              qword ptr [rbp + 536], rax
                        jmp              xchain0_n8_α
.Lx7_0:
                        .quad            10
#         output = 'FAIL 414/001: remdr(10,3)'           :(end)
# IR_LIT_STRING
xchain0_n7_α:
                        mov              qword ptr [rbp + 336], 1
                        mov              rax, qword ptr [rip + .Lx8_0]
                        mov              qword ptr [rbp + 344], rax
                        jmp              xchain0_n9_α
.Lx8_0:
                        .quad            .Lx8_0_s
.Lx8_0_s:
                        .string          "FAIL 414/001: remdr(10,3)"
xchain0_n8_α:
# BOX CALL remdr(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+496] -> [zr+448]
                        mov              rax, qword ptr [rbp + 496]
                        mov              qword ptr [rbp + 448], rax
                        mov              rax, qword ptr [rbp + 504]
                        mov              qword ptr [rbp + 456], rax
# marshal arg1 = producer-box slot [zr+528] -> [zr+464]
                        mov              rax, qword ptr [rbp + 528]
                        mov              qword ptr [rbp + 464], rax
                        mov              rax, qword ptr [rbp + 536]
                        mov              qword ptr [rbp + 472], rax
                        .section         .rodata
.Lbynamefn9:            .string          "remdr"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn9]
                        lea              rsi, [rbp + 448]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 432], rax
                        mov              qword ptr [rbp + 440], rdx
                        cmp              eax, 99
                        je               xchain0_n11_α
                        jmp              xchain0_n10_α
xchain0_n8_β:
                        jmp              xchain0_n11_α
# IR_ASSIGN gva
xchain0_n9_α:
                        mov              rax, qword ptr [rbp + 336]
                        mov              rdx, qword ptr [rbp + 344]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        mov              qword ptr [rbp + 320], rax
                        mov              qword ptr [rbp + 328], rdx
                        jmp              main_γ
# IR_LIT_INTEGER
xchain0_n10_α:
                        mov              qword ptr [rbp + 560], 6
                        mov              rax, qword ptr [rip + .Lx11_0]
                        mov              qword ptr [rbp + 568], rax
                        jmp              xchain0_n12_α
.Lx11_0:
                        .quad            1
#         output = 'PASS 414_remdr (2/2)'
# IR_LIT_STRING
xchain0_n11_α:
                        mov              qword ptr [rbp + 656], 1
                        mov              rax, qword ptr [rip + .Lx12_0]
                        mov              qword ptr [rbp + 664], rax
                        jmp              xchain0_n13_α
.Lx12_0:
                        .quad            .Lx12_0_s
.Lx12_0_s:
                        .string          "PASS 414_remdr (2/2)"
xchain0_n12_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+432] -> [zr+384]
                        mov              rax, qword ptr [rbp + 432]
                        mov              qword ptr [rbp + 384], rax
                        mov              rax, qword ptr [rbp + 440]
                        mov              qword ptr [rbp + 392], rax
# marshal arg1 = producer-box slot [zr+560] -> [zr+400]
                        mov              rax, qword ptr [rbp + 560]
                        mov              qword ptr [rbp + 400], rax
                        mov              rax, qword ptr [rbp + 568]
                        mov              qword ptr [rbp + 408], rax
                        .section         .rodata
.Lbynamefn13:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn13]
                        lea              rsi, [rbp + 384]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 368], rax
                        mov              qword ptr [rbp + 376], rdx
                        cmp              eax, 99
                        je               xchain0_n11_α
                        jmp              xchain0_n14_α
xchain0_n12_β:
                        jmp              xchain0_n11_α
# IR_ASSIGN gva
xchain0_n13_α:
                        mov              rax, qword ptr [rbp + 656]
                        mov              rdx, qword ptr [rbp + 664]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        mov              qword ptr [rbp + 640], rax
                        mov              qword ptr [rbp + 648], rdx
                        jmp              main_γ
#         output = 'FAIL 414/002: remdr(11,10)'          :(end)
# IR_LIT_STRING
xchain0_n14_α:
                        mov              qword ptr [rbp + 608], 1
                        mov              rax, qword ptr [rip + .Lx15_0]
                        mov              qword ptr [rbp + 616], rax
                        jmp              xchain0_n15_α
.Lx15_0:
                        .quad            .Lx15_0_s
.Lx15_0_s:
                        .string          "FAIL 414/002: remdr(11,10)"
# IR_ASSIGN gva
xchain0_n15_α:
                        mov              rax, qword ptr [rbp + 608]
                        mov              rdx, qword ptr [rbp + 616]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        mov              qword ptr [rbp + 592], rax
                        mov              qword ptr [rbp + 600], rdx
                        jmp              main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rbp + 696]
mov rbp, [rsp + 65536]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rbp + 696]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
mov rbp, [rsp + 65536]
add rsp, 65544
ret

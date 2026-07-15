  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "output"
  .align 8
__gva_names:
  .quad .Lgvan0
  .section .bss
  .align 16
__gva: .space 16, 0
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 1
  call gva_register@PLT
  mov rbx, rax
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
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  mov qword ptr [rsp + 1000], rsp
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [rsp + 192], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rsp + 200], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n4_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "a"
# IR_LIT_STRING
 xchain0_n1_α:
 mov qword ptr [rsp + 224], 1
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [rsp + 232], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n4_α
.Lx2_0:
 .quad .Lx2_0_s
.Lx2_0_s:
 .string "b"
 xchain0_n2_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rsp + 192]
 mov rsi, qword ptr [rsp + 200]
 mov rdx, qword ptr [rsp + 224]
 mov rcx, qword ptr [rsp + 232]
 call str_concat_d@PLT
 mov qword ptr [rsp + 160], rax
 mov qword ptr [rsp + 168], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
# IR_LIT_STRING
 xchain0_n3_α:
 mov qword ptr [rsp + 256], 1
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [rsp + 264], rax
 jmp xchain0_n5_α
 xchain0_n3_β:
 jmp xchain0_n4_α
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "ab"
# IR_LIT_STRING
 xchain0_n4_α:
 mov qword ptr [rsp + 464], 1
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [rsp + 472], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n11_α
.Lx5_0:
 .quad .Lx5_0_s
.Lx5_0_s:
 .string "a"
 xchain0_n5_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+160] -> [zr+112]
 mov rax, qword ptr [rsp + 160]
 mov qword ptr [rsp + 112], rax
 mov rax, qword ptr [rsp + 168]
 mov qword ptr [rsp + 120], rax
# marshal arg1 = producer-box slot [zr+256] -> [zr+128]
 mov rax, qword ptr [rsp + 256]
 mov qword ptr [rsp + 128], rax
 mov rax, qword ptr [rsp + 264]
 mov qword ptr [rsp + 136], rax
  .section .rodata
  .Lbynamefn6: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn6]
 lea rsi, [rsp + 112]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 96], rax
 mov qword ptr [rsp + 104], rdx
 cmp eax, 99
 je xchain0_n4_α
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n4_α
# IR_LIT_STRING
 xchain0_n6_α:
 mov qword ptr [rsp + 496], 1
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [rsp + 504], rax
 jmp xchain0_n8_α
 xchain0_n6_β:
 jmp xchain0_n11_α
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string "b"
# IR_LIT_STRING
 xchain0_n7_α:
 mov qword ptr [rsp + 304], 1
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [rsp + 312], rax
 jmp xchain0_n9_α
 xchain0_n7_β:
 jmp main_γ
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "FAIL 310/001: two-string concat"
 xchain0_n8_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rsp + 464]
 mov rsi, qword ptr [rsp + 472]
 mov rdx, qword ptr [rsp + 496]
 mov rcx, qword ptr [rsp + 504]
 call str_concat_d@PLT
 mov qword ptr [rsp + 432], rax
 mov qword ptr [rsp + 440], rdx
 jmp xchain0_n10_α
 xchain0_n8_β:
 jmp xchain0_n11_α
# IR_ASSIGN gva
 xchain0_n9_α:
 mov rax, qword ptr [rsp + 304]
 mov rdx, qword ptr [rsp + 312]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [rsp + 288], rax
 mov qword ptr [rsp + 296], rdx
 jmp main_γ
 xchain0_n9_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n10_α:
 mov qword ptr [rsp + 528], 1
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [rsp + 536], rax
 jmp xchain0_n12_α
 xchain0_n10_β:
 jmp xchain0_n11_α
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string "c"
# IR_LIT_STRING
 xchain0_n11_α:
 mov qword ptr [rsp + 768], 1
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [rsp + 776], rax
 jmp xchain0_n13_α
 xchain0_n11_β:
 jmp xchain0_n18_α
.Lx12_0:
 .quad .Lx12_0_s
.Lx12_0_s:
 .string "hello"
 xchain0_n12_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rsp + 432]
 mov rsi, qword ptr [rsp + 440]
 mov rdx, qword ptr [rsp + 528]
 mov rcx, qword ptr [rsp + 536]
 call str_concat_d@PLT
 mov qword ptr [rsp + 400], rax
 mov qword ptr [rsp + 408], rdx
 jmp xchain0_n14_α
 xchain0_n12_β:
 jmp xchain0_n11_α
# IR_LIT_STRING
 xchain0_n13_α:
 mov qword ptr [rsp + 800], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [rsp + 808], rax
 jmp xchain0_n15_α
 xchain0_n13_β:
 jmp xchain0_n18_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string " "
# IR_LIT_STRING
 xchain0_n14_α:
 mov qword ptr [rsp + 560], 1
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [rsp + 568], rax
 jmp xchain0_n16_α
 xchain0_n14_β:
 jmp xchain0_n11_α
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "abc"
 xchain0_n15_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rsp + 768]
 mov rsi, qword ptr [rsp + 776]
 mov rdx, qword ptr [rsp + 800]
 mov rcx, qword ptr [rsp + 808]
 call str_concat_d@PLT
 mov qword ptr [rsp + 736], rax
 mov qword ptr [rsp + 744], rdx
 jmp xchain0_n17_α
 xchain0_n15_β:
 jmp xchain0_n18_α
 xchain0_n16_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+400] -> [zr+352]
 mov rax, qword ptr [rsp + 400]
 mov qword ptr [rsp + 352], rax
 mov rax, qword ptr [rsp + 408]
 mov qword ptr [rsp + 360], rax
# marshal arg1 = producer-box slot [zr+560] -> [zr+368]
 mov rax, qword ptr [rsp + 560]
 mov qword ptr [rsp + 368], rax
 mov rax, qword ptr [rsp + 568]
 mov qword ptr [rsp + 376], rax
  .section .rodata
  .Lbynamefn17: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn17]
 lea rsi, [rsp + 352]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 336], rax
 mov qword ptr [rsp + 344], rdx
 cmp eax, 99
 je xchain0_n11_α
 jmp xchain0_n19_α
 xchain0_n16_β:
 jmp xchain0_n11_α
# IR_LIT_STRING
 xchain0_n17_α:
 mov qword ptr [rsp + 832], 1
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [rsp + 840], rax
 jmp xchain0_n20_α
 xchain0_n17_β:
 jmp xchain0_n18_α
.Lx18_0:
 .quad .Lx18_0_s
.Lx18_0_s:
 .string "world"
# IR_LIT_STRING
 xchain0_n18_α:
 mov qword ptr [rsp + 960], 1
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [rsp + 968], rax
 jmp xchain0_n21_α
 xchain0_n18_β:
 jmp main_γ
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "PASS 310_concat_strings (3/3)"
# IR_LIT_STRING
 xchain0_n19_α:
 mov qword ptr [rsp + 608], 1
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [rsp + 616], rax
 jmp xchain0_n22_α
 xchain0_n19_β:
 jmp main_γ
.Lx20_0:
 .quad .Lx20_0_s
.Lx20_0_s:
 .string "FAIL 310/002: three-string concat"
 xchain0_n20_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rsp + 736]
 mov rsi, qword ptr [rsp + 744]
 mov rdx, qword ptr [rsp + 832]
 mov rcx, qword ptr [rsp + 840]
 call str_concat_d@PLT
 mov qword ptr [rsp + 704], rax
 mov qword ptr [rsp + 712], rdx
 jmp xchain0_n23_α
 xchain0_n20_β:
 jmp xchain0_n18_α
# IR_ASSIGN gva
 xchain0_n21_α:
 mov rax, qword ptr [rsp + 960]
 mov rdx, qword ptr [rsp + 968]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [rsp + 944], rax
 mov qword ptr [rsp + 952], rdx
 jmp main_γ
 xchain0_n21_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain0_n22_α:
 mov rax, qword ptr [rsp + 608]
 mov rdx, qword ptr [rsp + 616]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [rsp + 592], rax
 mov qword ptr [rsp + 600], rdx
 jmp main_γ
 xchain0_n22_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n23_α:
 mov qword ptr [rsp + 864], 1
 mov rax, qword ptr [rip + .Lx24_0]
 mov qword ptr [rsp + 872], rax
 jmp xchain0_n24_α
 xchain0_n23_β:
 jmp xchain0_n18_α
.Lx24_0:
 .quad .Lx24_0_s
.Lx24_0_s:
 .string "hello world"
 xchain0_n24_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+704] -> [zr+656]
 mov rax, qword ptr [rsp + 704]
 mov qword ptr [rsp + 656], rax
 mov rax, qword ptr [rsp + 712]
 mov qword ptr [rsp + 664], rax
# marshal arg1 = producer-box slot [zr+864] -> [zr+672]
 mov rax, qword ptr [rsp + 864]
 mov qword ptr [rsp + 672], rax
 mov rax, qword ptr [rsp + 872]
 mov qword ptr [rsp + 680], rax
  .section .rodata
  .Lbynamefn25: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn25]
 lea rsi, [rsp + 656]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 640], rax
 mov qword ptr [rsp + 648], rdx
 cmp eax, 99
 je xchain0_n18_α
 jmp xchain0_n25_α
 xchain0_n24_β:
 jmp xchain0_n18_α
# IR_LIT_STRING
 xchain0_n25_α:
 mov qword ptr [rsp + 912], 1
 mov rax, qword ptr [rip + .Lx26_0]
 mov qword ptr [rsp + 920], rax
 jmp xchain0_n26_α
 xchain0_n25_β:
 jmp main_γ
.Lx26_0:
 .quad .Lx26_0_s
.Lx26_0_s:
 .string "FAIL 310/003: left-associative concat"
# IR_ASSIGN gva
 xchain0_n26_α:
 mov rax, qword ptr [rsp + 912]
 mov rdx, qword ptr [rsp + 920]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [rsp + 896], rax
 mov qword ptr [rsp + 904], rdx
 jmp main_γ
 xchain0_n26_β:
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rsp + 1000]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 1000]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret

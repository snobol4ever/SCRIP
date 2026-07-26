  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "dupl"
  .Lgvan1: .string "output"
  .Lgvan2: .string "size"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .quad .Lgvan2
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  mov edi, 3
  call rt_gva_island@PLT
  mov rsi, rax
  lea rdi, [rip + __gva_names]
  mov edx, 3
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
  mov qword ptr [rsp + 1096], rsp
  mov r12, qword ptr [1879048192]
  mov [rsp + 65536], rbp
  mov rbp, rsp
main_α_body:
#         opsyn('@', .dupl, 2)
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [rbp + 176], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rbp + 184], rax
 jmp xchain0_n1_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "@"
# IR_LIT_STRING
 xchain0_n1_α:
 mov qword ptr [rbp + 256], 1
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [rbp + 264], rax
 jmp xchain0_n2_α
.Lx2_0:
 .quad .Lx2_0_s
.Lx2_0_s:
 .string "dupl"
 xchain0_n2_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+256] -> [zr+224]
 mov rax, qword ptr [rbp + 256]
 mov qword ptr [rbp + 224], rax
 mov rax, qword ptr [rbp + 264]
 mov qword ptr [rbp + 232], rax
  .section .rodata
  .Lrkfn4: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn4]
 lea rsi, [rbp + 224]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 208], rax
 mov qword ptr [rbp + 216], rdx
 cmp eax, 99
 je xchain0_n4_α
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
# IR_LIT_INTEGER
 xchain0_n3_α:
 mov qword ptr [rbp + 288], 6
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [rbp + 296], rax
 jmp xchain0_n5_α
.Lx5_0:
 .quad 2
#         opsyn('@', .dupl, 2)
#         differ('a' @ 4, 'aaaa')                   :f(e001)
# IR_LIT_STRING
 xchain0_n4_α:
 mov qword ptr [rbp + 448], 1
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [rbp + 456], rax
 jmp xchain0_n6_α
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "a"
 xchain0_n5_α:
# BOX CALL opsyn(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+176] -> [zr+112]
 mov rax, qword ptr [rbp + 176]
 mov qword ptr [rbp + 112], rax
 mov rax, qword ptr [rbp + 184]
 mov qword ptr [rbp + 120], rax
# marshal arg1 = producer-box slot [zr+208] -> [zr+128]
 mov rax, qword ptr [rbp + 208]
 mov qword ptr [rbp + 128], rax
 mov rax, qword ptr [rbp + 216]
 mov qword ptr [rbp + 136], rax
# marshal arg2 = producer-box slot [zr+288] -> [zr+144]
 mov rax, qword ptr [rbp + 288]
 mov qword ptr [rbp + 144], rax
 mov rax, qword ptr [rbp + 296]
 mov qword ptr [rbp + 152], rax
  .section .rodata
  .Lbynamefn6: .string "opsyn"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn6]
 lea rsi, [rbp + 112]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [rbp + 96], rax
 mov qword ptr [rbp + 104], rdx
 cmp eax, 99
 je xchain0_n4_α
 jmp xchain0_n4_α
 xchain0_n5_β:
 jmp xchain0_n4_α
# IR_LIT_INTEGER
 xchain0_n6_α:
 mov qword ptr [rbp + 480], 6
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [rbp + 488], rax
 jmp xchain0_n7_α
.Lx8_0:
 .quad 4
 xchain0_n7_α:
# BOX CALL @(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+448] -> [zr+400]
 mov rax, qword ptr [rbp + 448]
 mov qword ptr [rbp + 400], rax
 mov rax, qword ptr [rbp + 456]
 mov qword ptr [rbp + 408], rax
# marshal arg1 = producer-box slot [zr+480] -> [zr+416]
 mov rax, qword ptr [rbp + 480]
 mov qword ptr [rbp + 416], rax
 mov rax, qword ptr [rbp + 488]
 mov qword ptr [rbp + 424], rax
  .section .rodata
  .Lbynamefn8: .string "@"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn8]
 lea rsi, [rbp + 400]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 384], rax
 mov qword ptr [rbp + 392], rdx
 cmp eax, 99
 je xchain0_n9_α
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp xchain0_n9_α
# IR_LIT_STRING
 xchain0_n8_α:
 mov qword ptr [rbp + 512], 1
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [rbp + 520], rax
 jmp xchain0_n10_α
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "aaaa"
#         opsyn('|', .size, 1)
# IR_LIT_STRING
 xchain0_n9_α:
 mov qword ptr [rbp + 672], 1
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [rbp + 680], rax
 jmp xchain0_n11_α
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string "|"
 xchain0_n10_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+384] -> [zr+336]
 mov rax, qword ptr [rbp + 384]
 mov qword ptr [rbp + 336], rax
 mov rax, qword ptr [rbp + 392]
 mov qword ptr [rbp + 344], rax
# marshal arg1 = producer-box slot [zr+512] -> [zr+352]
 mov rax, qword ptr [rbp + 512]
 mov qword ptr [rbp + 352], rax
 mov rax, qword ptr [rbp + 520]
 mov qword ptr [rbp + 360], rax
  .section .rodata
  .Lbynamefn11: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn11]
 lea rsi, [rbp + 336]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 320], rax
 mov qword ptr [rbp + 328], rdx
 cmp eax, 99
 je xchain0_n9_α
 jmp xchain0_n12_α
 xchain0_n10_β:
 jmp xchain0_n9_α
# IR_LIT_STRING
 xchain0_n11_α:
 mov qword ptr [rbp + 752], 1
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [rbp + 760], rax
 jmp xchain0_n13_α
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "size"
#         output = 'FAIL 1015/001: @ as binary dupl'     :(end)
# IR_LIT_STRING
 xchain0_n12_α:
 mov qword ptr [rbp + 560], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [rbp + 568], rax
 jmp xchain0_n14_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "FAIL 1015/001: @ as binary dupl"
 xchain0_n13_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+752] -> [zr+720]
 mov rax, qword ptr [rbp + 752]
 mov qword ptr [rbp + 720], rax
 mov rax, qword ptr [rbp + 760]
 mov qword ptr [rbp + 728], rax
  .section .rodata
  .Lrkfn16: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn16]
 lea rsi, [rbp + 720]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 704], rax
 mov qword ptr [rbp + 712], rdx
 cmp eax, 99
 je xchain0_n16_α
 jmp xchain0_n15_α
 xchain0_n13_β:
 jmp xchain0_n16_α
# IR_ASSIGN gva
 xchain0_n14_α:
 mov rax, qword ptr [rbp + 560]
 mov rdx, qword ptr [rbp + 568]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rbp + 544], rax
 mov qword ptr [rbp + 552], rdx
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n15_α:
 mov qword ptr [rbp + 784], 6
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [rbp + 792], rax
 jmp xchain0_n17_α
.Lx18_0:
 .quad 1
#         opsyn('|', .size, 1)
#         differ(|'string', 6)                   :f(e002)
# IR_LIT_STRING
 xchain0_n16_α:
 mov qword ptr [rbp + 928], 1
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [rbp + 936], rax
 jmp xchain0_n18_α
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "string"
 xchain0_n17_α:
# BOX CALL opsyn(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+672] -> [zr+608]
 mov rax, qword ptr [rbp + 672]
 mov qword ptr [rbp + 608], rax
 mov rax, qword ptr [rbp + 680]
 mov qword ptr [rbp + 616], rax
# marshal arg1 = producer-box slot [zr+704] -> [zr+624]
 mov rax, qword ptr [rbp + 704]
 mov qword ptr [rbp + 624], rax
 mov rax, qword ptr [rbp + 712]
 mov qword ptr [rbp + 632], rax
# marshal arg2 = producer-box slot [zr+784] -> [zr+640]
 mov rax, qword ptr [rbp + 784]
 mov qword ptr [rbp + 640], rax
 mov rax, qword ptr [rbp + 792]
 mov qword ptr [rbp + 648], rax
  .section .rodata
  .Lbynamefn18: .string "opsyn"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn18]
 lea rsi, [rbp + 608]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [rbp + 592], rax
 mov qword ptr [rbp + 600], rdx
 cmp eax, 99
 je xchain0_n16_α
 jmp xchain0_n16_α
 xchain0_n17_β:
 jmp xchain0_n16_α
 xchain0_n18_α:
# BOX CALL |(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+928] -> [zr+896]
 mov rax, qword ptr [rbp + 928]
 mov qword ptr [rbp + 896], rax
 mov rax, qword ptr [rbp + 936]
 mov qword ptr [rbp + 904], rax
  .section .rodata
  .Lbynamefn19: .string "|"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn19]
 lea rsi, [rbp + 896]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 880], rax
 mov qword ptr [rbp + 888], rdx
 cmp eax, 99
 je xchain0_n20_α
 jmp xchain0_n19_α
 xchain0_n18_β:
 jmp xchain0_n20_α
# IR_LIT_INTEGER
 xchain0_n19_α:
 mov qword ptr [rbp + 960], 6
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [rbp + 968], rax
 jmp xchain0_n21_α
.Lx22_0:
 .quad 6
#         output = 'PASS 1015_opsyn (2/2)'
# IR_LIT_STRING
 xchain0_n20_α:
 mov qword ptr [rbp + 1056], 1
 mov rax, qword ptr [rip + .Lx23_0]
 mov qword ptr [rbp + 1064], rax
 jmp xchain0_n22_α
.Lx23_0:
 .quad .Lx23_0_s
.Lx23_0_s:
 .string "PASS 1015_opsyn (2/2)"
 xchain0_n21_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+880] -> [zr+832]
 mov rax, qword ptr [rbp + 880]
 mov qword ptr [rbp + 832], rax
 mov rax, qword ptr [rbp + 888]
 mov qword ptr [rbp + 840], rax
# marshal arg1 = producer-box slot [zr+960] -> [zr+848]
 mov rax, qword ptr [rbp + 960]
 mov qword ptr [rbp + 848], rax
 mov rax, qword ptr [rbp + 968]
 mov qword ptr [rbp + 856], rax
  .section .rodata
  .Lbynamefn22: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn22]
 lea rsi, [rbp + 832]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 816], rax
 mov qword ptr [rbp + 824], rdx
 cmp eax, 99
 je xchain0_n20_α
 jmp xchain0_n23_α
 xchain0_n21_β:
 jmp xchain0_n20_α
# IR_ASSIGN gva
 xchain0_n22_α:
 mov rax, qword ptr [rbp + 1056]
 mov rdx, qword ptr [rbp + 1064]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rbp + 1040], rax
 mov qword ptr [rbp + 1048], rdx
 jmp main_γ
#         output = 'FAIL 1015/002: | as unary size'      :(end)
# IR_LIT_STRING
 xchain0_n23_α:
 mov qword ptr [rbp + 1008], 1
 mov rax, qword ptr [rip + .Lx26_0]
 mov qword ptr [rbp + 1016], rax
 jmp xchain0_n24_α
.Lx26_0:
 .quad .Lx26_0_s
.Lx26_0_s:
 .string "FAIL 1015/002: | as unary size"
# IR_ASSIGN gva
 xchain0_n24_α:
 mov rax, qword ptr [rbp + 1008]
 mov rdx, qword ptr [rbp + 1016]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rbp + 992], rax
 mov qword ptr [rbp + 1000], rdx
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rbp + 1096]
mov rbp, [rsp + 65536]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rbp + 1096]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
mov rbp, [rsp + 65536]
add rsp, 65544
ret

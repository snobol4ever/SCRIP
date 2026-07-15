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
  .section .bss
  .align 16
__gva: .space 48, 0
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
  mov edx, 3
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
  mov qword ptr [rsp + 1096], rsp
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [rsp + 176], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rsp + 184], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n4_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "@"
# IR_LIT_STRING
 xchain0_n1_α:
 mov qword ptr [rsp + 256], 1
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [rsp + 264], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n4_α
.Lx2_0:
 .quad .Lx2_0_s
.Lx2_0_s:
 .string "dupl"
 xchain0_n2_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+256] -> [zr+224]
 mov rax, qword ptr [rsp + 256]
 mov qword ptr [rsp + 224], rax
 mov rax, qword ptr [rsp + 264]
 mov qword ptr [rsp + 232], rax
  .section .rodata
  .Lrkfn4: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn4]
 lea rsi, [rsp + 224]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 208], rax
 mov qword ptr [rsp + 216], rdx
 cmp eax, 99
 je xchain0_n4_α
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
# IR_LIT_INTEGER
 xchain0_n3_α:
 mov qword ptr [rsp + 288], 6
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [rsp + 296], rax
 jmp xchain0_n5_α
 xchain0_n3_β:
 jmp xchain0_n4_α
.Lx5_0:
 .quad 2
# IR_LIT_STRING
 xchain0_n4_α:
 mov qword ptr [rsp + 448], 1
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [rsp + 456], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n9_α
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "a"
 xchain0_n5_α:
# BOX CALL opsyn(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+176] -> [zr+112]
 mov rax, qword ptr [rsp + 176]
 mov qword ptr [rsp + 112], rax
 mov rax, qword ptr [rsp + 184]
 mov qword ptr [rsp + 120], rax
# marshal arg1 = producer-box slot [zr+208] -> [zr+128]
 mov rax, qword ptr [rsp + 208]
 mov qword ptr [rsp + 128], rax
 mov rax, qword ptr [rsp + 216]
 mov qword ptr [rsp + 136], rax
# marshal arg2 = producer-box slot [zr+288] -> [zr+144]
 mov rax, qword ptr [rsp + 288]
 mov qword ptr [rsp + 144], rax
 mov rax, qword ptr [rsp + 296]
 mov qword ptr [rsp + 152], rax
  .section .rodata
  .Lbynamefn6: .string "opsyn"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn6]
 lea rsi, [rsp + 112]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [rsp + 96], rax
 mov qword ptr [rsp + 104], rdx
 cmp eax, 99
 je xchain0_n4_α
 jmp xchain0_n4_α
 xchain0_n5_β:
 jmp xchain0_n4_α
# IR_LIT_INTEGER
 xchain0_n6_α:
 mov qword ptr [rsp + 480], 6
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [rsp + 488], rax
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp xchain0_n9_α
.Lx8_0:
 .quad 4
 xchain0_n7_α:
# BOX CALL @(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+448] -> [zr+400]
 mov rax, qword ptr [rsp + 448]
 mov qword ptr [rsp + 400], rax
 mov rax, qword ptr [rsp + 456]
 mov qword ptr [rsp + 408], rax
# marshal arg1 = producer-box slot [zr+480] -> [zr+416]
 mov rax, qword ptr [rsp + 480]
 mov qword ptr [rsp + 416], rax
 mov rax, qword ptr [rsp + 488]
 mov qword ptr [rsp + 424], rax
  .section .rodata
  .Lbynamefn8: .string "@"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn8]
 lea rsi, [rsp + 400]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 384], rax
 mov qword ptr [rsp + 392], rdx
 cmp eax, 99
 je xchain0_n9_α
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp xchain0_n9_α
# IR_LIT_STRING
 xchain0_n8_α:
 mov qword ptr [rsp + 512], 1
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [rsp + 520], rax
 jmp xchain0_n10_α
 xchain0_n8_β:
 jmp xchain0_n9_α
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "aaaa"
# IR_LIT_STRING
 xchain0_n9_α:
 mov qword ptr [rsp + 672], 1
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [rsp + 680], rax
 jmp xchain0_n11_α
 xchain0_n9_β:
 jmp xchain0_n16_α
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string "|"
 xchain0_n10_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+384] -> [zr+336]
 mov rax, qword ptr [rsp + 384]
 mov qword ptr [rsp + 336], rax
 mov rax, qword ptr [rsp + 392]
 mov qword ptr [rsp + 344], rax
# marshal arg1 = producer-box slot [zr+512] -> [zr+352]
 mov rax, qword ptr [rsp + 512]
 mov qword ptr [rsp + 352], rax
 mov rax, qword ptr [rsp + 520]
 mov qword ptr [rsp + 360], rax
  .section .rodata
  .Lbynamefn11: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn11]
 lea rsi, [rsp + 336]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 320], rax
 mov qword ptr [rsp + 328], rdx
 cmp eax, 99
 je xchain0_n9_α
 jmp xchain0_n12_α
 xchain0_n10_β:
 jmp xchain0_n9_α
# IR_LIT_STRING
 xchain0_n11_α:
 mov qword ptr [rsp + 752], 1
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [rsp + 760], rax
 jmp xchain0_n13_α
 xchain0_n11_β:
 jmp xchain0_n16_α
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "size"
# IR_LIT_STRING
 xchain0_n12_α:
 mov qword ptr [rsp + 560], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [rsp + 568], rax
 jmp xchain0_n14_α
 xchain0_n12_β:
 jmp main_γ
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "FAIL 1015/001: @ as binary dupl"
 xchain0_n13_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+752] -> [zr+720]
 mov rax, qword ptr [rsp + 752]
 mov qword ptr [rsp + 720], rax
 mov rax, qword ptr [rsp + 760]
 mov qword ptr [rsp + 728], rax
  .section .rodata
  .Lrkfn16: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn16]
 lea rsi, [rsp + 720]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 704], rax
 mov qword ptr [rsp + 712], rdx
 cmp eax, 99
 je xchain0_n16_α
 jmp xchain0_n15_α
 xchain0_n13_β:
 jmp xchain0_n16_α
# IR_ASSIGN gva
 xchain0_n14_α:
 mov rax, qword ptr [rsp + 560]
 mov rdx, qword ptr [rsp + 568]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [rsp + 544], rax
 mov qword ptr [rsp + 552], rdx
 jmp main_γ
 xchain0_n14_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n15_α:
 mov qword ptr [rsp + 784], 6
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [rsp + 792], rax
 jmp xchain0_n17_α
 xchain0_n15_β:
 jmp xchain0_n16_α
.Lx18_0:
 .quad 1
# IR_LIT_STRING
 xchain0_n16_α:
 mov qword ptr [rsp + 928], 1
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [rsp + 936], rax
 jmp xchain0_n18_α
 xchain0_n16_β:
 jmp xchain0_n20_α
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "string"
 xchain0_n17_α:
# BOX CALL opsyn(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+672] -> [zr+608]
 mov rax, qword ptr [rsp + 672]
 mov qword ptr [rsp + 608], rax
 mov rax, qword ptr [rsp + 680]
 mov qword ptr [rsp + 616], rax
# marshal arg1 = producer-box slot [zr+704] -> [zr+624]
 mov rax, qword ptr [rsp + 704]
 mov qword ptr [rsp + 624], rax
 mov rax, qword ptr [rsp + 712]
 mov qword ptr [rsp + 632], rax
# marshal arg2 = producer-box slot [zr+784] -> [zr+640]
 mov rax, qword ptr [rsp + 784]
 mov qword ptr [rsp + 640], rax
 mov rax, qword ptr [rsp + 792]
 mov qword ptr [rsp + 648], rax
  .section .rodata
  .Lbynamefn18: .string "opsyn"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn18]
 lea rsi, [rsp + 608]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [rsp + 592], rax
 mov qword ptr [rsp + 600], rdx
 cmp eax, 99
 je xchain0_n16_α
 jmp xchain0_n16_α
 xchain0_n17_β:
 jmp xchain0_n16_α
 xchain0_n18_α:
# BOX CALL |(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+928] -> [zr+896]
 mov rax, qword ptr [rsp + 928]
 mov qword ptr [rsp + 896], rax
 mov rax, qword ptr [rsp + 936]
 mov qword ptr [rsp + 904], rax
  .section .rodata
  .Lbynamefn19: .string "|"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn19]
 lea rsi, [rsp + 896]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 880], rax
 mov qword ptr [rsp + 888], rdx
 cmp eax, 99
 je xchain0_n20_α
 jmp xchain0_n19_α
 xchain0_n18_β:
 jmp xchain0_n20_α
# IR_LIT_INTEGER
 xchain0_n19_α:
 mov qword ptr [rsp + 960], 6
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [rsp + 968], rax
 jmp xchain0_n21_α
 xchain0_n19_β:
 jmp xchain0_n20_α
.Lx22_0:
 .quad 6
# IR_LIT_STRING
 xchain0_n20_α:
 mov qword ptr [rsp + 1056], 1
 mov rax, qword ptr [rip + .Lx23_0]
 mov qword ptr [rsp + 1064], rax
 jmp xchain0_n22_α
 xchain0_n20_β:
 jmp main_γ
.Lx23_0:
 .quad .Lx23_0_s
.Lx23_0_s:
 .string "PASS 1015_opsyn (2/2)"
 xchain0_n21_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+880] -> [zr+832]
 mov rax, qword ptr [rsp + 880]
 mov qword ptr [rsp + 832], rax
 mov rax, qword ptr [rsp + 888]
 mov qword ptr [rsp + 840], rax
# marshal arg1 = producer-box slot [zr+960] -> [zr+848]
 mov rax, qword ptr [rsp + 960]
 mov qword ptr [rsp + 848], rax
 mov rax, qword ptr [rsp + 968]
 mov qword ptr [rsp + 856], rax
  .section .rodata
  .Lbynamefn22: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn22]
 lea rsi, [rsp + 832]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 816], rax
 mov qword ptr [rsp + 824], rdx
 cmp eax, 99
 je xchain0_n20_α
 jmp xchain0_n23_α
 xchain0_n21_β:
 jmp xchain0_n20_α
# IR_ASSIGN gva
 xchain0_n22_α:
 mov rax, qword ptr [rsp + 1056]
 mov rdx, qword ptr [rsp + 1064]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [rsp + 1040], rax
 mov qword ptr [rsp + 1048], rdx
 jmp main_γ
 xchain0_n22_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n23_α:
 mov qword ptr [rsp + 1008], 1
 mov rax, qword ptr [rip + .Lx26_0]
 mov qword ptr [rsp + 1016], rax
 jmp xchain0_n24_α
 xchain0_n23_β:
 jmp main_γ
.Lx26_0:
 .quad .Lx26_0_s
.Lx26_0_s:
 .string "FAIL 1015/002: | as unary size"
# IR_ASSIGN gva
 xchain0_n24_α:
 mov rax, qword ptr [rsp + 1008]
 mov rdx, qword ptr [rsp + 1016]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [rsp + 992], rax
 mov qword ptr [rsp + 1000], rdx
 jmp main_γ
 xchain0_n24_β:
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rsp + 1096]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 1096]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret

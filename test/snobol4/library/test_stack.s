  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "myvar"
  .Lgvan1: .string "subject"
  .Lgvan2: .string "w1"
  .Lgvan3: .string "w2"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .quad .Lgvan2
  .quad .Lgvan3
  .section .bss
  .align 16
__gva: .space 64, 0
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
  mov edx, 4
  call gva_register@PLT
  mov rbx, rax
  sub rsp, 65536
  mov rdi, rsp
  mov ecx, 8192
  xor eax, eax
  rep stosq
  mov rdi, rsp
  xor esi, esi
  call main_α
  xor eax, eax
  add rsp, 65536
  add rsp, 24
  ret
main_α:
#=======================================================================================================================
    .global main_α
    .global main_β
    .global main_γ
    .global main_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  mov qword ptr [r12 + 2232], rsp
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 2224], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [r12 + 160], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 168], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n3_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "TRIM"
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [r12 + 192], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 200], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n3_α
.Lx2_0:
 .quad 1
 xchain0_n2_α:
# BOX IR_CALL SNO$KWSET(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+160] -> [zr+112]
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 112], rax
 mov rax, qword ptr [r12 + 168]
 mov qword ptr [r12 + 120], rax
# marshal arg1 = producer-box slot [zr+192] -> [zr+128]
 mov rax, qword ptr [r12 + 192]
 mov qword ptr [r12 + 128], rax
 mov rax, qword ptr [r12 + 200]
 mov qword ptr [r12 + 136], rax
  .section .rodata
  .Lrkfn4: .string "SNO$KWSET"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn4]
 lea rsi, [r12 + 112]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n3_α
 xchain0_n3_α:
# BOX CALL stack_init(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn4: .string "stack_init"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn4]
 lea rsi, [r12 + 240]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 cmp eax, 99
 je xchain0_n4_α
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n4_α
# IR_LIT_STRING
 xchain0_n4_α:
 mov qword ptr [r12 + 304], 1
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 312], rax
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n6_α
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "a"
 xchain0_n5_α:
# BOX CALL stack_push(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+304] -> [zr+272]
 mov rax, qword ptr [r12 + 304]
 mov qword ptr [r12 + 272], rax
 mov rax, qword ptr [r12 + 312]
 mov qword ptr [r12 + 280], rax
  .section .rodata
  .Lbynamefn6: .string "stack_push"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn6]
 lea rsi, [r12 + 272]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
 cmp eax, 99
 je xchain0_n6_α
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n6_α
# IR_LIT_STRING
 xchain0_n6_α:
 mov qword ptr [r12 + 384], 1
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [r12 + 392], rax
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp xchain0_n8_α
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "b"
 xchain0_n7_α:
# BOX CALL stack_push(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+384] -> [zr+352]
 mov rax, qword ptr [r12 + 384]
 mov qword ptr [r12 + 352], rax
 mov rax, qword ptr [r12 + 392]
 mov qword ptr [r12 + 360], rax
  .section .rodata
  .Lbynamefn8: .string "stack_push"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn8]
 lea rsi, [r12 + 352]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 cmp eax, 99
 je xchain0_n8_α
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp xchain0_n8_α
# IR_LIT_STRING
 xchain0_n8_α:
 mov qword ptr [r12 + 464], 1
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [r12 + 472], rax
 jmp xchain0_n9_α
 xchain0_n8_β:
 jmp xchain0_n10_α
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "c"
 xchain0_n9_α:
# BOX CALL stack_push(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+464] -> [zr+432]
 mov rax, qword ptr [r12 + 464]
 mov qword ptr [r12 + 432], rax
 mov rax, qword ptr [r12 + 472]
 mov qword ptr [r12 + 440], rax
  .section .rodata
  .Lbynamefn10: .string "stack_push"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn10]
 lea rsi, [r12 + 432]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 cmp eax, 99
 je xchain0_n10_α
 jmp xchain0_n10_α
 xchain0_n9_β:
 jmp xchain0_n10_α
 xchain0_n10_α:
# BOX CALL stack_depth(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn11: .string "stack_depth"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn11]
 lea rsi, [r12 + 528]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 512], rax
 mov qword ptr [r12 + 520], rdx
 cmp eax, 99
 je xchain0_n12_α
 jmp xchain0_n11_α
 xchain0_n10_β:
 jmp xchain0_n12_α
# IR_ASSIGN global
 xchain0_n11_α:
 mov rsi, qword ptr [r12 + 512]
 mov rdx, qword ptr [r12 + 520]
 mov rdi, qword ptr [rip + .Lx13_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 496], rax
 mov qword ptr [r12 + 504], rdx
 jmp xchain0_n12_α
 xchain0_n11_β:
 jmp xchain0_n12_α
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "OUTPUT"
 xchain0_n12_α:
# BOX CALL stack_pop(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn13: .string "stack_pop"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn13]
 lea rsi, [r12 + 576]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 560], rax
 mov qword ptr [r12 + 568], rdx
 cmp eax, 99
 je xchain0_n14_α
 jmp xchain0_n13_α
 xchain0_n12_β:
 jmp xchain0_n14_α
# IR_ASSIGN global
 xchain0_n13_α:
 mov rsi, qword ptr [r12 + 560]
 mov rdx, qword ptr [r12 + 568]
 mov rdi, qword ptr [rip + .Lx15_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 544], rax
 mov qword ptr [r12 + 552], rdx
 jmp xchain0_n14_α
 xchain0_n13_β:
 jmp xchain0_n14_α
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "OUTPUT"
 xchain0_n14_α:
# BOX CALL stack_pop(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn15: .string "stack_pop"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn15]
 lea rsi, [r12 + 624]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 608], rax
 mov qword ptr [r12 + 616], rdx
 cmp eax, 99
 je xchain0_n16_α
 jmp xchain0_n15_α
 xchain0_n14_β:
 jmp xchain0_n16_α
# IR_ASSIGN global
 xchain0_n15_α:
 mov rsi, qword ptr [r12 + 608]
 mov rdx, qword ptr [r12 + 616]
 mov rdi, qword ptr [rip + .Lx17_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 592], rax
 mov qword ptr [r12 + 600], rdx
 jmp xchain0_n16_α
 xchain0_n15_β:
 jmp xchain0_n16_α
.Lx17_0:
 .quad .Lx17_0_s
.Lx17_0_s:
 .string "OUTPUT"
 xchain0_n16_α:
# BOX CALL stack_depth(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn17: .string "stack_depth"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn17]
 lea rsi, [r12 + 672]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 656], rax
 mov qword ptr [r12 + 664], rdx
 cmp eax, 99
 je xchain0_n18_α
 jmp xchain0_n17_α
 xchain0_n16_β:
 jmp xchain0_n18_α
# IR_ASSIGN global
 xchain0_n17_α:
 mov rsi, qword ptr [r12 + 656]
 mov rdx, qword ptr [r12 + 664]
 mov rdi, qword ptr [rip + .Lx19_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 640], rax
 mov qword ptr [r12 + 648], rdx
 jmp xchain0_n18_α
 xchain0_n17_β:
 jmp xchain0_n18_α
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "OUTPUT"
 xchain0_n18_α:
# BOX CALL stack_pop(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn19: .string "stack_pop"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn19]
 lea rsi, [r12 + 720]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 704], rax
 mov qword ptr [r12 + 712], rdx
 cmp eax, 99
 je xchain0_n20_α
 jmp xchain0_n19_α
 xchain0_n18_β:
 jmp xchain0_n20_α
# IR_ASSIGN global
 xchain0_n19_α:
 mov rsi, qword ptr [r12 + 704]
 mov rdx, qword ptr [r12 + 712]
 mov rdi, qword ptr [rip + .Lx21_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 688], rax
 mov qword ptr [r12 + 696], rdx
 jmp xchain0_n20_α
 xchain0_n19_β:
 jmp xchain0_n20_α
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string "OUTPUT"
 xchain0_n20_α:
# BOX CALL stack_depth(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn21: .string "stack_depth"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn21]
 lea rsi, [r12 + 768]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 752], rax
 mov qword ptr [r12 + 760], rdx
 cmp eax, 99
 je xchain0_n22_α
 jmp xchain0_n21_α
 xchain0_n20_β:
 jmp xchain0_n22_α
# IR_ASSIGN global
 xchain0_n21_α:
 mov rsi, qword ptr [r12 + 752]
 mov rdx, qword ptr [r12 + 760]
 mov rdi, qword ptr [rip + .Lx23_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 736], rax
 mov qword ptr [r12 + 744], rdx
 jmp xchain0_n22_α
 xchain0_n21_β:
 jmp xchain0_n22_α
.Lx23_0:
 .quad .Lx23_0_s
.Lx23_0_s:
 .string "OUTPUT"
 xchain0_n22_α:
# BOX CALL stack_pop(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn23: .string "stack_pop"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn23]
 lea rsi, [r12 + 800]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 784], rax
 mov qword ptr [r12 + 792], rdx
 cmp eax, 99
 je xchain0_n24_α
 jmp xchain0_n23_α
 xchain0_n22_β:
 jmp xchain0_n24_α
# IR_LIT_STRING
 xchain0_n23_α:
 mov qword ptr [r12 + 880], 1
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [r12 + 888], rax
 jmp xchain0_n25_α
 xchain0_n23_β:
 jmp xchain0_n27_α
.Lx25_0:
 .quad .Lx25_0_s
.Lx25_0_s:
 .string "FAIL: empty pop should FRETURN"
# IR_LIT_STRING
 xchain0_n24_α:
 mov qword ptr [r12 + 832], 1
 mov rax, qword ptr [rip + .Lx26_0]
 mov qword ptr [r12 + 840], rax
 jmp xchain0_n26_α
 xchain0_n24_β:
 jmp xchain0_n27_α
.Lx26_0:
 .quad .Lx26_0_s
.Lx26_0_s:
 .string "empty ok"
# IR_ASSIGN global
 xchain0_n25_α:
 mov rsi, qword ptr [r12 + 880]
 mov rdx, qword ptr [r12 + 888]
 mov rdi, qword ptr [rip + .Lx27_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 864], rax
 mov qword ptr [r12 + 872], rdx
 jmp xchain0_n27_α
 xchain0_n25_β:
 jmp xchain0_n27_α
.Lx27_0:
 .quad .Lx27_0_s
.Lx27_0_s:
 .string "OUTPUT"
# IR_ASSIGN global
 xchain0_n26_α:
 mov rsi, qword ptr [r12 + 832]
 mov rdx, qword ptr [r12 + 840]
 mov rdi, qword ptr [rip + .Lx28_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 816], rax
 mov qword ptr [r12 + 824], rdx
 jmp xchain0_n27_α
 xchain0_n26_β:
 jmp xchain0_n27_α
.Lx28_0:
 .quad .Lx28_0_s
.Lx28_0_s:
 .string "OUTPUT"
 xchain0_n27_α:
# BOX CALL stack_init(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn28: .string "stack_init"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn28]
 lea rsi, [r12 + 928]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 912], rax
 mov qword ptr [r12 + 920], rdx
 cmp eax, 99
 je xchain0_n28_α
 jmp xchain0_n28_α
 xchain0_n27_β:
 jmp xchain0_n28_α
# IR_LIT_STRING
 xchain0_n28_α:
 mov qword ptr [r12 + 992], 1
 mov rax, qword ptr [rip + .Lx30_0]
 mov qword ptr [r12 + 1000], rax
 jmp xchain0_n29_α
 xchain0_n28_β:
 jmp xchain0_n30_α
.Lx30_0:
 .quad .Lx30_0_s
.Lx30_0_s:
 .string "x"
 xchain0_n29_α:
# BOX CALL stack_push(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+992] -> [zr+960]
 mov rax, qword ptr [r12 + 992]
 mov qword ptr [r12 + 960], rax
 mov rax, qword ptr [r12 + 1000]
 mov qword ptr [r12 + 968], rax
  .section .rodata
  .Lbynamefn30: .string "stack_push"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn30]
 lea rsi, [r12 + 960]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 944], rax
 mov qword ptr [r12 + 952], rdx
 cmp eax, 99
 je xchain0_n30_α
 jmp xchain0_n30_α
 xchain0_n29_β:
 jmp xchain0_n30_α
 xchain0_n30_α:
# BOX CALL stack_peek(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn31: .string "stack_peek"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn31]
 lea rsi, [r12 + 1056]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1040], rax
 mov qword ptr [r12 + 1048], rdx
 cmp eax, 99
 je xchain0_n32_α
 jmp xchain0_n31_α
 xchain0_n30_β:
 jmp xchain0_n32_α
# IR_ASSIGN global
 xchain0_n31_α:
 mov rsi, qword ptr [r12 + 1040]
 mov rdx, qword ptr [r12 + 1048]
 mov rdi, qword ptr [rip + .Lx33_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 1024], rax
 mov qword ptr [r12 + 1032], rdx
 jmp xchain0_n32_α
 xchain0_n31_β:
 jmp xchain0_n32_α
.Lx33_0:
 .quad .Lx33_0_s
.Lx33_0_s:
 .string "OUTPUT"
 xchain0_n32_α:
# BOX CALL stack_depth(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn33: .string "stack_depth"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn33]
 lea rsi, [r12 + 1104]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1088], rax
 mov qword ptr [r12 + 1096], rdx
 cmp eax, 99
 je xchain0_n34_α
 jmp xchain0_n33_α
 xchain0_n32_β:
 jmp xchain0_n34_α
# IR_ASSIGN global
 xchain0_n33_α:
 mov rsi, qword ptr [r12 + 1088]
 mov rdx, qword ptr [r12 + 1096]
 mov rdi, qword ptr [rip + .Lx35_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 1072], rax
 mov qword ptr [r12 + 1080], rdx
 jmp xchain0_n34_α
 xchain0_n33_β:
 jmp xchain0_n34_α
.Lx35_0:
 .quad .Lx35_0_s
.Lx35_0_s:
 .string "OUTPUT"
 xchain0_n34_α:
# BOX CALL stack_pop(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn35: .string "stack_pop"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn35]
 lea rsi, [r12 + 1152]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1136], rax
 mov qword ptr [r12 + 1144], rdx
 cmp eax, 99
 je xchain0_n36_α
 jmp xchain0_n35_α
 xchain0_n34_β:
 jmp xchain0_n36_α
# IR_ASSIGN global
 xchain0_n35_α:
 mov rsi, qword ptr [r12 + 1136]
 mov rdx, qword ptr [r12 + 1144]
 mov rdi, qword ptr [rip + .Lx37_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 1120], rax
 mov qword ptr [r12 + 1128], rdx
 jmp xchain0_n36_α
 xchain0_n35_β:
 jmp xchain0_n36_α
.Lx37_0:
 .quad .Lx37_0_s
.Lx37_0_s:
 .string "OUTPUT"
 xchain0_n36_α:
# BOX CALL stack_init(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn37: .string "stack_init"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn37]
 lea rsi, [r12 + 1184]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1168], rax
 mov qword ptr [r12 + 1176], rdx
 cmp eax, 99
 je xchain0_n37_α
 jmp xchain0_n37_α
 xchain0_n36_β:
 jmp xchain0_n37_α
# IR_LIT_INTEGER
 xchain0_n37_α:
 mov qword ptr [r12 + 1248], 6
 mov rax, qword ptr [rip + .Lx39_0]
 mov qword ptr [r12 + 1256], rax
 jmp xchain0_n38_α
 xchain0_n37_β:
 jmp xchain0_n39_α
.Lx39_0:
 .quad 42
 xchain0_n38_α:
# BOX CALL stack_push(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1248] -> [zr+1216]
 mov rax, qword ptr [r12 + 1248]
 mov qword ptr [r12 + 1216], rax
 mov rax, qword ptr [r12 + 1256]
 mov qword ptr [r12 + 1224], rax
  .section .rodata
  .Lbynamefn39: .string "stack_push"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn39]
 lea rsi, [r12 + 1216]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1200], rax
 mov qword ptr [r12 + 1208], rdx
 cmp eax, 99
 je xchain0_n39_α
 jmp xchain0_n39_α
 xchain0_n38_β:
 jmp xchain0_n39_α
# IR_LIT_INTEGER
 xchain0_n39_α:
 mov qword ptr [r12 + 1328], 6
 mov rax, qword ptr [rip + .Lx41_0]
 mov qword ptr [r12 + 1336], rax
 jmp xchain0_n40_α
 xchain0_n39_β:
 jmp xchain0_n41_α
.Lx41_0:
 .quad 99
 xchain0_n40_α:
# BOX CALL stack_push(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1328] -> [zr+1296]
 mov rax, qword ptr [r12 + 1328]
 mov qword ptr [r12 + 1296], rax
 mov rax, qword ptr [r12 + 1336]
 mov qword ptr [r12 + 1304], rax
  .section .rodata
  .Lbynamefn41: .string "stack_push"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn41]
 lea rsi, [r12 + 1296]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1280], rax
 mov qword ptr [r12 + 1288], rdx
 cmp eax, 99
 je xchain0_n41_α
 jmp xchain0_n41_α
 xchain0_n40_β:
 jmp xchain0_n41_α
# IR_LIT_STRING
 xchain0_n41_α:
 mov qword ptr [r12 + 1408], 1
 mov rax, qword ptr [rip + .Lx43_0]
 mov qword ptr [r12 + 1416], rax
 jmp xchain0_n42_α
 xchain0_n41_β:
 jmp xchain0_n43_α
.Lx43_0:
 .quad .Lx43_0_s
.Lx43_0_s:
 .string "myvar"
 xchain0_n42_α:
# BOX CALL stack_pop(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1408] -> [zr+1376]
 mov rax, qword ptr [r12 + 1408]
 mov qword ptr [r12 + 1376], rax
 mov rax, qword ptr [r12 + 1416]
 mov qword ptr [r12 + 1384], rax
  .section .rodata
  .Lbynamefn43: .string "stack_pop"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn43]
 lea rsi, [r12 + 1376]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1360], rax
 mov qword ptr [r12 + 1368], rdx
 cmp eax, 99
 je xchain0_n43_α
 jmp xchain0_n43_α
 xchain0_n42_β:
 jmp xchain0_n43_α
# IR_VAR
 xchain0_n43_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 1456], rax
 mov qword ptr [r12 + 1464], rdx
 jmp xchain0_n44_α
 xchain0_n43_β:
 jmp xchain0_n45_α
# IR_ASSIGN global
 xchain0_n44_α:
 mov rsi, qword ptr [r12 + 1456]
 mov rdx, qword ptr [r12 + 1464]
 mov rdi, qword ptr [rip + .Lx46_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 1440], rax
 mov qword ptr [r12 + 1448], rdx
 jmp xchain0_n45_α
 xchain0_n44_β:
 jmp xchain0_n45_α
.Lx46_0:
 .quad .Lx46_0_s
.Lx46_0_s:
 .string "OUTPUT"
 xchain0_n45_α:
# BOX CALL stack_init(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn46: .string "stack_init"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn46]
 lea rsi, [r12 + 1504]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1488], rax
 mov qword ptr [r12 + 1496], rdx
 cmp eax, 99
 je xchain0_n46_α
 jmp xchain0_n46_α
 xchain0_n45_β:
 jmp xchain0_n46_α
# IR_LIT_STRING
 xchain0_n46_α:
 mov qword ptr [r12 + 1536], 1
 mov rax, qword ptr [rip + .Lx48_0]
 mov qword ptr [r12 + 1544], rax
 jmp xchain0_n47_α
 xchain0_n46_β:
 jmp xchain0_n48_α
.Lx48_0:
 .quad .Lx48_0_s
.Lx48_0_s:
 .string "hello world"
# IR_ASSIGN gva
 xchain0_n47_α:
 mov rax, qword ptr [r12 + 1536]
 mov rdx, qword ptr [r12 + 1544]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1520], rax
 mov qword ptr [r12 + 1528], rdx
 jmp xchain0_n48_α
 xchain0_n47_β:
 jmp xchain0_n48_α
# IR_VAR
 xchain0_n48_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 1888], rax
 mov qword ptr [r12 + 1896], rdx
 jmp xchain0_n49_α
 xchain0_n48_β:
 jmp xchain0_n50_α
# IR_MATCH_HEAD
 xchain0_n49_α:
 sub rsp, 32
 call rt_zls_mark@PLT
 mov qword ptr [rsp + 8], rax
 mov rax, rsp
 add rax, 32
 mov qword ptr [rsp + 16], rax
 mov rdi, qword ptr [r12 + 1888]
 mov rsi, qword ptr [r12 + 1896]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov qword ptr [r12 + 1624], rbp
 lea rcx, [rip + g_dcap_top]
 mov rbp, qword ptr [rcx + 0]
 mov qword ptr [r12 + 1616], rbp
 mov dword ptr [rsp + 0], 0
.Lx52_0:
 mov r14d, dword ptr [rsp + 0]
 jmp xchain0_n51_α
 xchain0_n49_β:
 add dword ptr [rsp + 0], 1
 mov eax, dword ptr [rsp + 0]
 cmp eax, r15d
 jg .Lx52_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx52_1
 jmp .Lx52_0
.Lx52_1:
 mov rdi, qword ptr [rsp + 8]
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
 mov rsp, qword ptr [rsp + 16]
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rsp, [rsp + 8]
 lea rcx, [rip + g_dcap_top]
 mov rax, qword ptr [r12 + 1616]
 mov qword ptr [rcx + 0], rax
 mov rbp, qword ptr [r12 + 1624]
 jmp xchain0_n50_α
# IR_LIT_STRING
 xchain0_n50_α:
 mov qword ptr [r12 + 2192], 1
 mov rax, qword ptr [rip + .Lx53_0]
 mov qword ptr [r12 + 2200], rax
 jmp xchain0_n52_α
 xchain0_n50_β:
 jmp main_γ
.Lx53_0:
 .quad .Lx53_0_s
.Lx53_0_s:
 .string "FAIL: pattern match failed"
# IR_MATCH_SEQ_NARY (ZB-FC-3b: zero cell, LIFO-structural)
 xchain0_n51_α:
 jmp xchain0_n54_α
xchain0_n51_as:
 jmp xchain0_n53_α
 xchain0_n51_β:
 jmp xchain0_n58_β
xchain0_n51_af:
 jmp xchain0_n49_β
# IR_ASSIGN global
 xchain0_n52_α:
 mov rsi, qword ptr [r12 + 2192]
 mov rdx, qword ptr [r12 + 2200]
 mov rdi, qword ptr [rip + .Lx56_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 2176], rax
 mov qword ptr [r12 + 2184], rdx
 jmp main_γ
 xchain0_n52_β:
 jmp main_γ
.Lx56_0:
 .quad .Lx56_0_s
.Lx56_0_s:
 .string "OUTPUT"
# IR_MATCH_RELEASE
 xchain0_n53_α:
 mov eax, dword ptr [rsp + 64]
 mov dword ptr [r12 + 1584], eax
 mov qword ptr [r12 + 1608], r14
 mov rdi, qword ptr [rsp + 72]
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
 mov rsp, qword ptr [rsp + 80]
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rsp, [rsp + 8]
 push r14
 push r15
 push r13
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 lea rcx, [rip + g_dcap_top]
 mov qword ptr [rcx + 0], rbp
 mov rdi, qword ptr [r12 + 1616]
 mov rsi, rbp
 mov rdx, r13
 call rt_dcap_end_ok_open@PLT
.Lx58_1:
 test rax, rax
 je .Lx58_2
 call rt_proc_open_fn@PLT
 push r12
 sub rsp, 8
 lea rcx, [rip + .Lx58_3]
 lea rdx, [rip + .Lx58_4]
 mov r12, rsp
 jmp rax
.Lx58_3:
 mov rax, rsp
 mov rax, qword ptr [rax + 8]
 mov rdi, qword ptr [rax + 0]
 mov rsi, qword ptr [rax + 8]
 mov rsp, r12
 add rsp, 8
 pop r12
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx58_1
.Lx58_4:
 mov rsp, r12
 add rsp, 8
 pop r12
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx58_1
.Lx58_2:
 call rt_dcap_end_ok_close@PLT
 mov rsp, [rsp + 8]
 pop r13
 pop r15
 pop r14
 lea rcx, [rip + g_dcap_top]
 mov rax, qword ptr [r12 + 1616]
 mov qword ptr [rcx + 0], rax
 mov rbp, qword ptr [r12 + 1624]
 jmp xchain0_n59_α
# IR_MATCH_CAPTURE_SAVE fc cell
 xchain0_n54_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 jmp xchain0_n60_α
 xchain0_n54_β:
 add rsp, 16
 jmp xchain0_n51_af
# IR_MATCH_CAPTURE_COND (rbp-dcap inline pend)
 xchain0_n55_α:
 mov eax, dword ptr [rsp + 16]
 lea rcx, [rip + .S0]
 mov qword ptr [rbp + 0], rcx
 mov esi, eax
 mov qword ptr [rbp + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [rbp + 16], rdx
 add rbp, 24
 jmp xchain0_n56_α
 xchain0_n55_β:
 sub rbp, 24
 jmp xchain0_n60_β
# IR_MATCH_LIT
 xchain0_n56_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain0_n55_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S1]
 mov edx, 1
 call memcmp@PLT
 test eax, eax
 jne xchain0_n55_β
 add r14d, 1
 jmp xchain0_n57_α
 xchain0_n56_β:
 sub r14d, 1
 jmp xchain0_n55_β
# IR_MATCH_CAPTURE_SAVE fc cell
 xchain0_n57_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 jmp xchain0_n61_α
 xchain0_n57_β:
 add rsp, 16
 jmp xchain0_n56_β
# IR_MATCH_CAPTURE_COND (rbp-dcap inline pend)
 xchain0_n58_α:
 mov eax, dword ptr [rsp + 16]
 lea rcx, [rip + .S2]
 mov qword ptr [rbp + 0], rcx
 mov esi, eax
 mov qword ptr [rbp + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [rbp + 16], rdx
 add rbp, 24
 jmp xchain0_n51_as
 xchain0_n58_β:
 sub rbp, 24
 jmp xchain0_n61_β
# IR_LIT_STRING
 xchain0_n59_α:
 mov qword ptr [r12 + 1648], 1
 mov rax, qword ptr [rip + .Lx69_0]
 mov qword ptr [r12 + 1656], rax
 jmp xchain0_n62_α
 xchain0_n59_β:
 jmp xchain0_n50_α
.Lx69_0:
 .quad .Lx69_0_s
.Lx69_0_s:
 .string ""
# IR_MATCH_BREAK
 xchain0_n60_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], 0
.Lx71_0:
 mov eax, r14d
 add eax, dword ptr [rsp + 0]
 cmp eax, r15d
 jl .Lx71_240
 add rsp, 16
 jmp xchain0_n54_β
.Lx71_240:
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S1]
 call strchr@PLT
 test rax, rax
 jnz .Lx71_1
 add dword ptr [rsp + 0], 1
 jmp .Lx71_0
.Lx71_1:
 mov eax, r14d
 add eax, dword ptr [rsp + 0]
 mov r14d, eax
 jmp xchain0_n55_α
 xchain0_n60_β:
 mov eax, r14d
 sub eax, dword ptr [rsp + 0]
 mov r14d, eax
 add rsp, 16
 jmp xchain0_n54_β
# IR_MATCH_REM
 xchain0_n61_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 mov r14d, r15d
 jmp xchain0_n58_α
 xchain0_n61_β:
 mov r14d, dword ptr [rsp + 0]
 add rsp, 16
 jmp xchain0_n57_β
# IR_MATCH_REPLACE
 xchain0_n62_α:
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx74_0]
 mov rsi, qword ptr [r12 + 1888]
 mov rdx, qword ptr [r12 + 1896]
 mov ecx, dword ptr [r12 + 1584]
 mov r8, qword ptr [r12 + 1608]
 lea r9, [r12 + 1648]
 call rt_match_replace@PLT
 mov rsp, [rsp + 8]
 jmp .Lx74_1
.Lx74_0:
 .quad .Lx74_0_s
.Lx74_0_s:
 .string "subject"
.Lx74_1:
 jmp xchain0_n63_α
# IR_VAR
 xchain0_n63_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 1968], rax
 mov qword ptr [r12 + 1976], rdx
 jmp xchain0_n64_α
 xchain0_n63_β:
 jmp xchain0_n65_α
 xchain0_n64_α:
# BOX CALL stack_push(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1968] -> [zr+1936]
 mov rax, qword ptr [r12 + 1968]
 mov qword ptr [r12 + 1936], rax
 mov rax, qword ptr [r12 + 1976]
 mov qword ptr [r12 + 1944], rax
  .section .rodata
  .Lbynamefn65: .string "stack_push"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn65]
 lea rsi, [r12 + 1936]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1920], rax
 mov qword ptr [r12 + 1928], rdx
 cmp eax, 99
 je xchain0_n65_α
 jmp xchain0_n65_α
 xchain0_n64_β:
 jmp xchain0_n65_α
# IR_VAR
 xchain0_n65_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 2048], rax
 mov qword ptr [r12 + 2056], rdx
 jmp xchain0_n66_α
 xchain0_n65_β:
 jmp xchain0_n67_α
 xchain0_n66_α:
# BOX CALL stack_push(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2048] -> [zr+2016]
 mov rax, qword ptr [r12 + 2048]
 mov qword ptr [r12 + 2016], rax
 mov rax, qword ptr [r12 + 2056]
 mov qword ptr [r12 + 2024], rax
  .section .rodata
  .Lbynamefn67: .string "stack_push"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn67]
 lea rsi, [r12 + 2016]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2000], rax
 mov qword ptr [r12 + 2008], rdx
 cmp eax, 99
 je xchain0_n67_α
 jmp xchain0_n67_α
 xchain0_n66_β:
 jmp xchain0_n67_α
 xchain0_n67_α:
# BOX CALL stack_pop(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn68: .string "stack_pop"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn68]
 lea rsi, [r12 + 2112]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2096], rax
 mov qword ptr [r12 + 2104], rdx
 cmp eax, 99
 je xchain0_n69_α
 jmp xchain0_n68_α
 xchain0_n67_β:
 jmp xchain0_n69_α
# IR_ASSIGN global
 xchain0_n68_α:
 mov rsi, qword ptr [r12 + 2096]
 mov rdx, qword ptr [r12 + 2104]
 mov rdi, qword ptr [rip + .Lx80_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 2080], rax
 mov qword ptr [r12 + 2088], rdx
 jmp xchain0_n69_α
 xchain0_n68_β:
 jmp xchain0_n69_α
.Lx80_0:
 .quad .Lx80_0_s
.Lx80_0_s:
 .string "OUTPUT"
 xchain0_n69_α:
# BOX CALL stack_pop(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn70: .string "stack_pop"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn70]
 lea rsi, [r12 + 2160]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2144], rax
 mov qword ptr [r12 + 2152], rdx
 cmp eax, 99
 je main_γ
 jmp xchain0_n70_α
 xchain0_n69_β:
 jmp main_γ
# IR_ASSIGN global
 xchain0_n70_α:
 mov rsi, qword ptr [r12 + 2144]
 mov rdx, qword ptr [r12 + 2152]
 mov rdi, qword ptr [rip + .Lx82_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 2128], rax
 mov qword ptr [r12 + 2136], rdx
 jmp main_γ
 xchain0_n70_β:
 jmp main_γ
.Lx82_0:
 .quad .Lx82_0_s
.Lx82_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [r12 + 2232]
pop r12
ret
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 2224]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, qword ptr [r12 + 2232]
pop r12
ret
.section .rodata
.S0: .string "w1"
.S1: .string " "
.S2: .string "w2"
.text

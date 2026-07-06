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
  push rbp
  mov rbp, rsp
  push rdi
  push rsi
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 1
  call gva_register@PLT
  mov rbx, rax
  call rt_frame@PLT
  mov rdi, rax
  xor esi, esi
  call main_α
  xor eax, eax
  mov rsp, rbp
  pop rbp
  ret
main_α:
#=======================================================================================================================
    .global main_α
    .global main_β
    .global main_γ
    .global main_ω
push r12
  mov r12, rdi
main_α_body:
xchain0_n0_α:
# IR_LIT_INTEGER
bb1_α:
 mov qword ptr [r12 + 64], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n4_α
.Lx1_0:
 .quad 5
xchain0_n1_α:
# IR_LIT_INTEGER
bb2_α:
 mov qword ptr [r12 + 80], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 88], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n4_α
.Lx2_0:
 .quad 4
xchain0_n2_α:
bb3_α:
# BOX IR_CALL lt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+64] -> [r12+32]
 mov rax, qword ptr [r12 + 64]
 mov qword ptr [r12 + 32], rax
 mov rax, qword ptr [r12 + 72]
 mov qword ptr [r12 + 40], rax
# marshal arg1 = producer-box slot [r12+80] -> [r12+48]
 mov rax, qword ptr [r12 + 80]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 88]
 mov qword ptr [r12 + 56], rax
  .section .rodata
  .Lbynamefn4: .string "lt"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn4]
 lea rsi, [r12 + 32]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 cmp eax, 99
 je xchain0_n4_α
 jmp xchain0_n3_α
xchain0_n2_β:
 jmp xchain0_n4_α
xchain0_n3_α:
# IR_LIT_STRING
bb4_α:
 mov qword ptr [r12 + 112], 1
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain0_n5_α
 xchain0_n3_β:
 jmp main_γ
.Lx5_0:
 .quad .Lx5_0_s
.Lx5_0_s:
 .string "FAIL 912/001: lt(5,4) should fail"
xchain0_n4_α:
# IR_LIT_INTEGER
bb5_α:
 mov qword ptr [r12 + 176], 6
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 184], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n9_α
.Lx6_0:
 .quad 4
xchain0_n5_α:
# IR_ASSIGN gva
bb6_α:
 mov rax, qword ptr [r12 + 112]
 mov rdx, qword ptr [r12 + 120]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 jmp main_γ
 xchain0_n5_β:
 jmp main_γ
xchain0_n6_α:
# IR_LIT_INTEGER
bb7_α:
 mov qword ptr [r12 + 192], 6
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [r12 + 200], rax
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp xchain0_n9_α
.Lx8_0:
 .quad 4
xchain0_n7_α:
bb8_α:
# BOX IR_CALL lt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+176] -> [r12+144]
 mov rax, qword ptr [r12 + 176]
 mov qword ptr [r12 + 144], rax
 mov rax, qword ptr [r12 + 184]
 mov qword ptr [r12 + 152], rax
# marshal arg1 = producer-box slot [r12+192] -> [r12+160]
 mov rax, qword ptr [r12 + 192]
 mov qword ptr [r12 + 160], rax
 mov rax, qword ptr [r12 + 200]
 mov qword ptr [r12 + 168], rax
  .section .rodata
  .Lbynamefn10: .string "lt"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn10]
 lea rsi, [r12 + 144]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
 cmp eax, 99
 je xchain0_n9_α
 jmp xchain0_n8_α
xchain0_n7_β:
 jmp xchain0_n9_α
xchain0_n8_α:
# IR_LIT_STRING
bb9_α:
 mov qword ptr [r12 + 224], 1
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 232], rax
 jmp xchain0_n10_α
 xchain0_n8_β:
 jmp main_γ
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string "FAIL 912/002: lt(4,4) should fail"
xchain0_n9_α:
# IR_LIT_INTEGER
bb10_α:
 mov qword ptr [r12 + 288], 6
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [r12 + 296], rax
 jmp xchain0_n11_α
 xchain0_n9_β:
 jmp xchain0_n14_α
.Lx12_0:
 .quad 4
xchain0_n10_α:
# IR_ASSIGN gva
bb11_α:
 mov rax, qword ptr [r12 + 224]
 mov rdx, qword ptr [r12 + 232]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 jmp main_γ
 xchain0_n10_β:
 jmp main_γ
xchain0_n11_α:
# IR_LIT_INTEGER
bb12_α:
 mov qword ptr [r12 + 304], 6
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [r12 + 312], rax
 jmp xchain0_n12_α
 xchain0_n11_β:
 jmp xchain0_n14_α
.Lx14_0:
 .quad 5
xchain0_n12_α:
bb13_α:
# BOX IR_CALL lt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+288] -> [r12+256]
 mov rax, qword ptr [r12 + 288]
 mov qword ptr [r12 + 256], rax
 mov rax, qword ptr [r12 + 296]
 mov qword ptr [r12 + 264], rax
# marshal arg1 = producer-box slot [r12+304] -> [r12+272]
 mov rax, qword ptr [r12 + 304]
 mov qword ptr [r12 + 272], rax
 mov rax, qword ptr [r12 + 312]
 mov qword ptr [r12 + 280], rax
  .section .rodata
  .Lbynamefn16: .string "lt"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn16]
 lea rsi, [r12 + 256]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 cmp eax, 99
 je xchain0_n14_α
 jmp xchain0_n13_α
xchain0_n12_β:
 jmp xchain0_n14_α
xchain0_n13_α:
# IR_LIT_INTEGER
bb14_α:
 mov qword ptr [r12 + 400], 6
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [r12 + 408], rax
 jmp xchain0_n15_α
 xchain0_n13_β:
 jmp xchain0_n19_α
.Lx17_0:
 .quad 5
xchain0_n14_α:
# IR_LIT_STRING
bb15_α:
 mov qword ptr [r12 + 336], 1
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [r12 + 344], rax
 jmp xchain0_n16_α
 xchain0_n14_β:
 jmp main_γ
.Lx18_0:
 .quad .Lx18_0_s
.Lx18_0_s:
 .string "FAIL 912/003: lt(4,5) should succeed"
xchain0_n15_α:
# IR_LIT_INTEGER
bb16_α:
 mov qword ptr [r12 + 416], 6
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [r12 + 424], rax
 jmp xchain0_n17_α
 xchain0_n15_β:
 jmp xchain0_n19_α
.Lx19_0:
 .quad 2
xchain0_n16_α:
# IR_ASSIGN gva
bb17_α:
 mov rax, qword ptr [r12 + 336]
 mov rdx, qword ptr [r12 + 344]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 jmp main_γ
 xchain0_n16_β:
 jmp main_γ
xchain0_n17_α:
bb18_α:
# BOX IR_CALL le(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+400] -> [r12+368]
 mov rax, qword ptr [r12 + 400]
 mov qword ptr [r12 + 368], rax
 mov rax, qword ptr [r12 + 408]
 mov qword ptr [r12 + 376], rax
# marshal arg1 = producer-box slot [r12+416] -> [r12+384]
 mov rax, qword ptr [r12 + 416]
 mov qword ptr [r12 + 384], rax
 mov rax, qword ptr [r12 + 424]
 mov qword ptr [r12 + 392], rax
  .section .rodata
  .Lbynamefn22: .string "le"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn22]
 lea rsi, [r12 + 368]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 352], rax
 mov qword ptr [r12 + 360], rdx
 cmp eax, 99
 je xchain0_n19_α
 jmp xchain0_n18_α
xchain0_n17_β:
 jmp xchain0_n19_α
xchain0_n18_α:
# IR_LIT_STRING
bb19_α:
 mov qword ptr [r12 + 448], 1
 mov rax, qword ptr [rip + .Lx23_0]
 mov qword ptr [r12 + 456], rax
 jmp xchain0_n20_α
 xchain0_n18_β:
 jmp main_γ
.Lx23_0:
 .quad .Lx23_0_s
.Lx23_0_s:
 .string "FAIL 912/004: le(5,2) should fail"
xchain0_n19_α:
# IR_LIT_INTEGER
bb20_α:
 mov qword ptr [r12 + 512], 6
 mov rax, qword ptr [rip + .Lx24_0]
 mov qword ptr [r12 + 520], rax
 jmp xchain0_n21_α
 xchain0_n19_β:
 jmp xchain0_n24_α
.Lx24_0:
 .quad 4
xchain0_n20_α:
# IR_ASSIGN gva
bb21_α:
 mov rax, qword ptr [r12 + 448]
 mov rdx, qword ptr [r12 + 456]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 432], rax
 mov qword ptr [r12 + 440], rdx
 jmp main_γ
 xchain0_n20_β:
 jmp main_γ
xchain0_n21_α:
# IR_LIT_INTEGER
bb22_α:
 mov qword ptr [r12 + 528], 6
 mov rax, qword ptr [rip + .Lx26_0]
 mov qword ptr [r12 + 536], rax
 jmp xchain0_n22_α
 xchain0_n21_β:
 jmp xchain0_n24_α
.Lx26_0:
 .quad 4
xchain0_n22_α:
bb23_α:
# BOX IR_CALL le(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+512] -> [r12+480]
 mov rax, qword ptr [r12 + 512]
 mov qword ptr [r12 + 480], rax
 mov rax, qword ptr [r12 + 520]
 mov qword ptr [r12 + 488], rax
# marshal arg1 = producer-box slot [r12+528] -> [r12+496]
 mov rax, qword ptr [r12 + 528]
 mov qword ptr [r12 + 496], rax
 mov rax, qword ptr [r12 + 536]
 mov qword ptr [r12 + 504], rax
  .section .rodata
  .Lbynamefn28: .string "le"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn28]
 lea rsi, [r12 + 480]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 464], rax
 mov qword ptr [r12 + 472], rdx
 cmp eax, 99
 je xchain0_n24_α
 jmp xchain0_n23_α
xchain0_n22_β:
 jmp xchain0_n24_α
xchain0_n23_α:
# IR_LIT_INTEGER
bb24_α:
 mov qword ptr [r12 + 624], 6
 mov rax, qword ptr [rip + .Lx29_0]
 mov qword ptr [r12 + 632], rax
 jmp xchain0_n25_α
 xchain0_n23_β:
 jmp xchain0_n29_α
.Lx29_0:
 .quad 4
xchain0_n24_α:
# IR_LIT_STRING
bb25_α:
 mov qword ptr [r12 + 560], 1
 mov rax, qword ptr [rip + .Lx30_0]
 mov qword ptr [r12 + 568], rax
 jmp xchain0_n26_α
 xchain0_n24_β:
 jmp main_γ
.Lx30_0:
 .quad .Lx30_0_s
.Lx30_0_s:
 .string "FAIL 912/005: le(4,4) should succeed"
xchain0_n25_α:
# IR_LIT_INTEGER
bb26_α:
 mov qword ptr [r12 + 640], 6
 mov rax, qword ptr [rip + .Lx31_0]
 mov qword ptr [r12 + 648], rax
 jmp xchain0_n27_α
 xchain0_n25_β:
 jmp xchain0_n29_α
.Lx31_0:
 .quad 10
xchain0_n26_α:
# IR_ASSIGN gva
bb27_α:
 mov rax, qword ptr [r12 + 560]
 mov rdx, qword ptr [r12 + 568]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 544], rax
 mov qword ptr [r12 + 552], rdx
 jmp main_γ
 xchain0_n26_β:
 jmp main_γ
xchain0_n27_α:
bb28_α:
# BOX IR_CALL le(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+624] -> [r12+592]
 mov rax, qword ptr [r12 + 624]
 mov qword ptr [r12 + 592], rax
 mov rax, qword ptr [r12 + 632]
 mov qword ptr [r12 + 600], rax
# marshal arg1 = producer-box slot [r12+640] -> [r12+608]
 mov rax, qword ptr [r12 + 640]
 mov qword ptr [r12 + 608], rax
 mov rax, qword ptr [r12 + 648]
 mov qword ptr [r12 + 616], rax
  .section .rodata
  .Lbynamefn34: .string "le"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn34]
 lea rsi, [r12 + 592]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 576], rax
 mov qword ptr [r12 + 584], rdx
 cmp eax, 99
 je xchain0_n29_α
 jmp xchain0_n28_α
xchain0_n27_β:
 jmp xchain0_n29_α
xchain0_n28_α:
# IR_LIT_INTEGER
bb29_α:
 mov qword ptr [r12 + 736], 6
 mov rax, qword ptr [rip + .Lx35_0]
 mov qword ptr [r12 + 744], rax
 jmp xchain0_n30_α
 xchain0_n28_β:
 jmp xchain0_n34_α
.Lx35_0:
 .quad 4
xchain0_n29_α:
# IR_LIT_STRING
bb30_α:
 mov qword ptr [r12 + 672], 1
 mov rax, qword ptr [rip + .Lx36_0]
 mov qword ptr [r12 + 680], rax
 jmp xchain0_n31_α
 xchain0_n29_β:
 jmp main_γ
.Lx36_0:
 .quad .Lx36_0_s
.Lx36_0_s:
 .string "FAIL 912/006: le(4,10) should succeed"
xchain0_n30_α:
# IR_LIT_INTEGER
bb31_α:
 mov qword ptr [r12 + 752], 6
 mov rax, qword ptr [rip + .Lx37_0]
 mov qword ptr [r12 + 760], rax
 jmp xchain0_n32_α
 xchain0_n30_β:
 jmp xchain0_n34_α
.Lx37_0:
 .quad 5
xchain0_n31_α:
# IR_ASSIGN gva
bb32_α:
 mov rax, qword ptr [r12 + 672]
 mov rdx, qword ptr [r12 + 680]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 656], rax
 mov qword ptr [r12 + 664], rdx
 jmp main_γ
 xchain0_n31_β:
 jmp main_γ
xchain0_n32_α:
bb33_α:
# BOX IR_CALL eq(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+736] -> [r12+704]
 mov rax, qword ptr [r12 + 736]
 mov qword ptr [r12 + 704], rax
 mov rax, qword ptr [r12 + 744]
 mov qword ptr [r12 + 712], rax
# marshal arg1 = producer-box slot [r12+752] -> [r12+720]
 mov rax, qword ptr [r12 + 752]
 mov qword ptr [r12 + 720], rax
 mov rax, qword ptr [r12 + 760]
 mov qword ptr [r12 + 728], rax
  .section .rodata
  .Lbynamefn40: .string "eq"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn40]
 lea rsi, [r12 + 704]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 688], rax
 mov qword ptr [r12 + 696], rdx
 cmp eax, 99
 je xchain0_n34_α
 jmp xchain0_n33_α
xchain0_n32_β:
 jmp xchain0_n34_α
xchain0_n33_α:
# IR_LIT_STRING
bb34_α:
 mov qword ptr [r12 + 784], 1
 mov rax, qword ptr [rip + .Lx41_0]
 mov qword ptr [r12 + 792], rax
 jmp xchain0_n35_α
 xchain0_n33_β:
 jmp main_γ
.Lx41_0:
 .quad .Lx41_0_s
.Lx41_0_s:
 .string "FAIL 912/007: eq(4,5) should fail"
xchain0_n34_α:
# IR_LIT_INTEGER
bb35_α:
 mov qword ptr [r12 + 848], 6
 mov rax, qword ptr [rip + .Lx42_0]
 mov qword ptr [r12 + 856], rax
 jmp xchain0_n36_α
 xchain0_n34_β:
 jmp xchain0_n39_α
.Lx42_0:
 .quad 5
xchain0_n35_α:
# IR_ASSIGN gva
bb36_α:
 mov rax, qword ptr [r12 + 784]
 mov rdx, qword ptr [r12 + 792]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 768], rax
 mov qword ptr [r12 + 776], rdx
 jmp main_γ
 xchain0_n35_β:
 jmp main_γ
xchain0_n36_α:
# IR_LIT_INTEGER
bb37_α:
 mov qword ptr [r12 + 864], 6
 mov rax, qword ptr [rip + .Lx44_0]
 mov qword ptr [r12 + 872], rax
 jmp xchain0_n37_α
 xchain0_n36_β:
 jmp xchain0_n39_α
.Lx44_0:
 .quad 5
xchain0_n37_α:
bb38_α:
# BOX IR_CALL eq(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+848] -> [r12+816]
 mov rax, qword ptr [r12 + 848]
 mov qword ptr [r12 + 816], rax
 mov rax, qword ptr [r12 + 856]
 mov qword ptr [r12 + 824], rax
# marshal arg1 = producer-box slot [r12+864] -> [r12+832]
 mov rax, qword ptr [r12 + 864]
 mov qword ptr [r12 + 832], rax
 mov rax, qword ptr [r12 + 872]
 mov qword ptr [r12 + 840], rax
  .section .rodata
  .Lbynamefn46: .string "eq"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn46]
 lea rsi, [r12 + 816]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 800], rax
 mov qword ptr [r12 + 808], rdx
 cmp eax, 99
 je xchain0_n39_α
 jmp xchain0_n38_α
xchain0_n37_β:
 jmp xchain0_n39_α
xchain0_n38_α:
# IR_LIT_INTEGER
bb39_α:
 mov qword ptr [r12 + 960], 6
 mov rax, qword ptr [rip + .Lx47_0]
 mov qword ptr [r12 + 968], rax
 jmp xchain0_n40_α
 xchain0_n38_β:
 jmp xchain0_n44_α
.Lx47_0:
 .quad 4
xchain0_n39_α:
# IR_LIT_STRING
bb40_α:
 mov qword ptr [r12 + 896], 1
 mov rax, qword ptr [rip + .Lx48_0]
 mov qword ptr [r12 + 904], rax
 jmp xchain0_n41_α
 xchain0_n39_β:
 jmp main_γ
.Lx48_0:
 .quad .Lx48_0_s
.Lx48_0_s:
 .string "FAIL 912/008: eq(5,5) should succeed"
xchain0_n40_α:
# IR_LIT_INTEGER
bb41_α:
 mov qword ptr [r12 + 976], 6
 mov rax, qword ptr [rip + .Lx49_0]
 mov qword ptr [r12 + 984], rax
 jmp xchain0_n42_α
 xchain0_n40_β:
 jmp xchain0_n44_α
.Lx49_0:
 .quad 4
xchain0_n41_α:
# IR_ASSIGN gva
bb42_α:
 mov rax, qword ptr [r12 + 896]
 mov rdx, qword ptr [r12 + 904]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 880], rax
 mov qword ptr [r12 + 888], rdx
 jmp main_γ
 xchain0_n41_β:
 jmp main_γ
xchain0_n42_α:
bb43_α:
# BOX IR_CALL ne(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+960] -> [r12+928]
 mov rax, qword ptr [r12 + 960]
 mov qword ptr [r12 + 928], rax
 mov rax, qword ptr [r12 + 968]
 mov qword ptr [r12 + 936], rax
# marshal arg1 = producer-box slot [r12+976] -> [r12+944]
 mov rax, qword ptr [r12 + 976]
 mov qword ptr [r12 + 944], rax
 mov rax, qword ptr [r12 + 984]
 mov qword ptr [r12 + 952], rax
  .section .rodata
  .Lbynamefn52: .string "ne"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn52]
 lea rsi, [r12 + 928]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 912], rax
 mov qword ptr [r12 + 920], rdx
 cmp eax, 99
 je xchain0_n44_α
 jmp xchain0_n43_α
xchain0_n42_β:
 jmp xchain0_n44_α
xchain0_n43_α:
# IR_LIT_STRING
bb44_α:
 mov qword ptr [r12 + 1008], 1
 mov rax, qword ptr [rip + .Lx53_0]
 mov qword ptr [r12 + 1016], rax
 jmp xchain0_n45_α
 xchain0_n43_β:
 jmp main_γ
.Lx53_0:
 .quad .Lx53_0_s
.Lx53_0_s:
 .string "FAIL 912/009: ne(4,4) should fail"
xchain0_n44_α:
# IR_LIT_INTEGER
bb45_α:
 mov qword ptr [r12 + 1072], 6
 mov rax, qword ptr [rip + .Lx54_0]
 mov qword ptr [r12 + 1080], rax
 jmp xchain0_n46_α
 xchain0_n44_β:
 jmp xchain0_n49_α
.Lx54_0:
 .quad 4
xchain0_n45_α:
# IR_ASSIGN gva
bb46_α:
 mov rax, qword ptr [r12 + 1008]
 mov rdx, qword ptr [r12 + 1016]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 992], rax
 mov qword ptr [r12 + 1000], rdx
 jmp main_γ
 xchain0_n45_β:
 jmp main_γ
xchain0_n46_α:
# IR_LIT_INTEGER
bb47_α:
 mov qword ptr [r12 + 1088], 6
 mov rax, qword ptr [rip + .Lx56_0]
 mov qword ptr [r12 + 1096], rax
 jmp xchain0_n47_α
 xchain0_n46_β:
 jmp xchain0_n49_α
.Lx56_0:
 .quad 6
xchain0_n47_α:
bb48_α:
# BOX IR_CALL ne(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+1072] -> [r12+1040]
 mov rax, qword ptr [r12 + 1072]
 mov qword ptr [r12 + 1040], rax
 mov rax, qword ptr [r12 + 1080]
 mov qword ptr [r12 + 1048], rax
# marshal arg1 = producer-box slot [r12+1088] -> [r12+1056]
 mov rax, qword ptr [r12 + 1088]
 mov qword ptr [r12 + 1056], rax
 mov rax, qword ptr [r12 + 1096]
 mov qword ptr [r12 + 1064], rax
  .section .rodata
  .Lbynamefn58: .string "ne"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn58]
 lea rsi, [r12 + 1040]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1024], rax
 mov qword ptr [r12 + 1032], rdx
 cmp eax, 99
 je xchain0_n49_α
 jmp xchain0_n48_α
xchain0_n47_β:
 jmp xchain0_n49_α
xchain0_n48_α:
# IR_LIT_INTEGER
bb49_α:
 mov qword ptr [r12 + 1184], 6
 mov rax, qword ptr [rip + .Lx59_0]
 mov qword ptr [r12 + 1192], rax
 jmp xchain0_n50_α
 xchain0_n48_β:
 jmp xchain0_n54_α
.Lx59_0:
 .quad 4
xchain0_n49_α:
# IR_LIT_STRING
bb50_α:
 mov qword ptr [r12 + 1120], 1
 mov rax, qword ptr [rip + .Lx60_0]
 mov qword ptr [r12 + 1128], rax
 jmp xchain0_n51_α
 xchain0_n49_β:
 jmp main_γ
.Lx60_0:
 .quad .Lx60_0_s
.Lx60_0_s:
 .string "FAIL 912/010: ne(4,6) should succeed"
xchain0_n50_α:
# IR_LIT_INTEGER
bb51_α:
 mov qword ptr [r12 + 1200], 6
 mov rax, qword ptr [rip + .Lx61_0]
 mov qword ptr [r12 + 1208], rax
 jmp xchain0_n52_α
 xchain0_n50_β:
 jmp xchain0_n54_α
.Lx61_0:
 .quad 6
xchain0_n51_α:
# IR_ASSIGN gva
bb52_α:
 mov rax, qword ptr [r12 + 1120]
 mov rdx, qword ptr [r12 + 1128]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 1104], rax
 mov qword ptr [r12 + 1112], rdx
 jmp main_γ
 xchain0_n51_β:
 jmp main_γ
xchain0_n52_α:
bb53_α:
# BOX IR_CALL gt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+1184] -> [r12+1152]
 mov rax, qword ptr [r12 + 1184]
 mov qword ptr [r12 + 1152], rax
 mov rax, qword ptr [r12 + 1192]
 mov qword ptr [r12 + 1160], rax
# marshal arg1 = producer-box slot [r12+1200] -> [r12+1168]
 mov rax, qword ptr [r12 + 1200]
 mov qword ptr [r12 + 1168], rax
 mov rax, qword ptr [r12 + 1208]
 mov qword ptr [r12 + 1176], rax
  .section .rodata
  .Lbynamefn64: .string "gt"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn64]
 lea rsi, [r12 + 1152]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1136], rax
 mov qword ptr [r12 + 1144], rdx
 cmp eax, 99
 je xchain0_n54_α
 jmp xchain0_n53_α
xchain0_n52_β:
 jmp xchain0_n54_α
xchain0_n53_α:
# IR_LIT_STRING
bb54_α:
 mov qword ptr [r12 + 1232], 1
 mov rax, qword ptr [rip + .Lx65_0]
 mov qword ptr [r12 + 1240], rax
 jmp xchain0_n55_α
 xchain0_n53_β:
 jmp main_γ
.Lx65_0:
 .quad .Lx65_0_s
.Lx65_0_s:
 .string "FAIL 912/011: gt(4,6) should fail"
xchain0_n54_α:
# IR_LIT_INTEGER
bb55_α:
 mov qword ptr [r12 + 1296], 6
 mov rax, qword ptr [rip + .Lx66_0]
 mov qword ptr [r12 + 1304], rax
 jmp xchain0_n56_α
 xchain0_n54_β:
 jmp xchain0_n59_α
.Lx66_0:
 .quad 4
xchain0_n55_α:
# IR_ASSIGN gva
bb56_α:
 mov rax, qword ptr [r12 + 1232]
 mov rdx, qword ptr [r12 + 1240]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 1216], rax
 mov qword ptr [r12 + 1224], rdx
 jmp main_γ
 xchain0_n55_β:
 jmp main_γ
xchain0_n56_α:
# IR_LIT_INTEGER
bb57_α:
 mov qword ptr [r12 + 1312], 6
 mov rax, qword ptr [rip + .Lx68_0]
 mov qword ptr [r12 + 1320], rax
 jmp xchain0_n57_α
 xchain0_n56_β:
 jmp xchain0_n59_α
.Lx68_0:
 .quad 4
xchain0_n57_α:
bb58_α:
# BOX IR_CALL gt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+1296] -> [r12+1264]
 mov rax, qword ptr [r12 + 1296]
 mov qword ptr [r12 + 1264], rax
 mov rax, qword ptr [r12 + 1304]
 mov qword ptr [r12 + 1272], rax
# marshal arg1 = producer-box slot [r12+1312] -> [r12+1280]
 mov rax, qword ptr [r12 + 1312]
 mov qword ptr [r12 + 1280], rax
 mov rax, qword ptr [r12 + 1320]
 mov qword ptr [r12 + 1288], rax
  .section .rodata
  .Lbynamefn70: .string "gt"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn70]
 lea rsi, [r12 + 1264]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1248], rax
 mov qword ptr [r12 + 1256], rdx
 cmp eax, 99
 je xchain0_n59_α
 jmp xchain0_n58_α
xchain0_n57_β:
 jmp xchain0_n59_α
xchain0_n58_α:
# IR_LIT_STRING
bb59_α:
 mov qword ptr [r12 + 1344], 1
 mov rax, qword ptr [rip + .Lx71_0]
 mov qword ptr [r12 + 1352], rax
 jmp xchain0_n60_α
 xchain0_n58_β:
 jmp main_γ
.Lx71_0:
 .quad .Lx71_0_s
.Lx71_0_s:
 .string "FAIL 912/012: gt(4,4) should fail"
xchain0_n59_α:
# IR_LIT_INTEGER
bb60_α:
 mov qword ptr [r12 + 1408], 6
 mov rax, qword ptr [rip + .Lx72_0]
 mov qword ptr [r12 + 1416], rax
 jmp xchain0_n61_α
 xchain0_n59_β:
 jmp xchain0_n64_α
.Lx72_0:
 .quad 5
xchain0_n60_α:
# IR_ASSIGN gva
bb61_α:
 mov rax, qword ptr [r12 + 1344]
 mov rdx, qword ptr [r12 + 1352]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 1328], rax
 mov qword ptr [r12 + 1336], rdx
 jmp main_γ
 xchain0_n60_β:
 jmp main_γ
xchain0_n61_α:
# IR_LIT_INTEGER
bb62_α:
 mov qword ptr [r12 + 1424], 6
 mov rax, qword ptr [rip + .Lx74_0]
 mov qword ptr [r12 + 1432], rax
 jmp xchain0_n62_α
 xchain0_n61_β:
 jmp xchain0_n64_α
.Lx74_0:
 .quad 2
xchain0_n62_α:
bb63_α:
# BOX IR_CALL gt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+1408] -> [r12+1376]
 mov rax, qword ptr [r12 + 1408]
 mov qword ptr [r12 + 1376], rax
 mov rax, qword ptr [r12 + 1416]
 mov qword ptr [r12 + 1384], rax
# marshal arg1 = producer-box slot [r12+1424] -> [r12+1392]
 mov rax, qword ptr [r12 + 1424]
 mov qword ptr [r12 + 1392], rax
 mov rax, qword ptr [r12 + 1432]
 mov qword ptr [r12 + 1400], rax
  .section .rodata
  .Lbynamefn76: .string "gt"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn76]
 lea rsi, [r12 + 1376]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1360], rax
 mov qword ptr [r12 + 1368], rdx
 cmp eax, 99
 je xchain0_n64_α
 jmp xchain0_n63_α
xchain0_n62_β:
 jmp xchain0_n64_α
xchain0_n63_α:
# IR_LIT_INTEGER
bb64_α:
 mov qword ptr [r12 + 1520], 6
 mov rax, qword ptr [rip + .Lx77_0]
 mov qword ptr [r12 + 1528], rax
 jmp xchain0_n65_α
 xchain0_n63_β:
 jmp xchain0_n69_α
.Lx77_0:
 .quad 5
xchain0_n64_α:
# IR_LIT_STRING
bb65_α:
 mov qword ptr [r12 + 1456], 1
 mov rax, qword ptr [rip + .Lx78_0]
 mov qword ptr [r12 + 1464], rax
 jmp xchain0_n66_α
 xchain0_n64_β:
 jmp main_γ
.Lx78_0:
 .quad .Lx78_0_s
.Lx78_0_s:
 .string "FAIL 912/013: gt(5,2) should succeed"
xchain0_n65_α:
# IR_LIT_INTEGER
bb66_α:
 mov qword ptr [r12 + 1536], 6
 mov rax, qword ptr [rip + .Lx79_0]
 mov qword ptr [r12 + 1544], rax
 jmp xchain0_n67_α
 xchain0_n65_β:
 jmp xchain0_n69_α
.Lx79_0:
 .quad 7
xchain0_n66_α:
# IR_ASSIGN gva
bb67_α:
 mov rax, qword ptr [r12 + 1456]
 mov rdx, qword ptr [r12 + 1464]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 1440], rax
 mov qword ptr [r12 + 1448], rdx
 jmp main_γ
 xchain0_n66_β:
 jmp main_γ
xchain0_n67_α:
bb68_α:
# BOX IR_CALL ge(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+1520] -> [r12+1488]
 mov rax, qword ptr [r12 + 1520]
 mov qword ptr [r12 + 1488], rax
 mov rax, qword ptr [r12 + 1528]
 mov qword ptr [r12 + 1496], rax
# marshal arg1 = producer-box slot [r12+1536] -> [r12+1504]
 mov rax, qword ptr [r12 + 1536]
 mov qword ptr [r12 + 1504], rax
 mov rax, qword ptr [r12 + 1544]
 mov qword ptr [r12 + 1512], rax
  .section .rodata
  .Lbynamefn82: .string "ge"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn82]
 lea rsi, [r12 + 1488]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1472], rax
 mov qword ptr [r12 + 1480], rdx
 cmp eax, 99
 je xchain0_n69_α
 jmp xchain0_n68_α
xchain0_n67_β:
 jmp xchain0_n69_α
xchain0_n68_α:
# IR_LIT_STRING
bb69_α:
 mov qword ptr [r12 + 1568], 1
 mov rax, qword ptr [rip + .Lx83_0]
 mov qword ptr [r12 + 1576], rax
 jmp xchain0_n70_α
 xchain0_n68_β:
 jmp main_γ
.Lx83_0:
 .quad .Lx83_0_s
.Lx83_0_s:
 .string "FAIL 912/014: ge(5,7) should fail"
xchain0_n69_α:
# IR_LIT_INTEGER
bb70_α:
 mov qword ptr [r12 + 1632], 6
 mov rax, qword ptr [rip + .Lx84_0]
 mov qword ptr [r12 + 1640], rax
 jmp xchain0_n71_α
 xchain0_n69_β:
 jmp xchain0_n74_α
.Lx84_0:
 .quad 4
xchain0_n70_α:
# IR_ASSIGN gva
bb71_α:
 mov rax, qword ptr [r12 + 1568]
 mov rdx, qword ptr [r12 + 1576]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 1552], rax
 mov qword ptr [r12 + 1560], rdx
 jmp main_γ
 xchain0_n70_β:
 jmp main_γ
xchain0_n71_α:
# IR_LIT_INTEGER
bb72_α:
 mov qword ptr [r12 + 1648], 6
 mov rax, qword ptr [rip + .Lx86_0]
 mov qword ptr [r12 + 1656], rax
 jmp xchain0_n72_α
 xchain0_n71_β:
 jmp xchain0_n74_α
.Lx86_0:
 .quad 4
xchain0_n72_α:
bb73_α:
# BOX IR_CALL ge(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+1632] -> [r12+1600]
 mov rax, qword ptr [r12 + 1632]
 mov qword ptr [r12 + 1600], rax
 mov rax, qword ptr [r12 + 1640]
 mov qword ptr [r12 + 1608], rax
# marshal arg1 = producer-box slot [r12+1648] -> [r12+1616]
 mov rax, qword ptr [r12 + 1648]
 mov qword ptr [r12 + 1616], rax
 mov rax, qword ptr [r12 + 1656]
 mov qword ptr [r12 + 1624], rax
  .section .rodata
  .Lbynamefn88: .string "ge"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn88]
 lea rsi, [r12 + 1600]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1584], rax
 mov qword ptr [r12 + 1592], rdx
 cmp eax, 99
 je xchain0_n74_α
 jmp xchain0_n73_α
xchain0_n72_β:
 jmp xchain0_n74_α
xchain0_n73_α:
# IR_LIT_INTEGER
bb74_α:
 mov qword ptr [r12 + 1744], 6
 mov rax, qword ptr [rip + .Lx89_0]
 mov qword ptr [r12 + 1752], rax
 jmp xchain0_n75_α
 xchain0_n73_β:
 jmp xchain0_n79_α
.Lx89_0:
 .quad 7
xchain0_n74_α:
# IR_LIT_STRING
bb75_α:
 mov qword ptr [r12 + 1680], 1
 mov rax, qword ptr [rip + .Lx90_0]
 mov qword ptr [r12 + 1688], rax
 jmp xchain0_n76_α
 xchain0_n74_β:
 jmp main_γ
.Lx90_0:
 .quad .Lx90_0_s
.Lx90_0_s:
 .string "FAIL 912/015: ge(4,4) should succeed"
xchain0_n75_α:
# IR_LIT_INTEGER
bb76_α:
 mov qword ptr [r12 + 1760], 6
 mov rax, qword ptr [rip + .Lx91_0]
 mov qword ptr [r12 + 1768], rax
 jmp xchain0_n77_α
 xchain0_n75_β:
 jmp xchain0_n79_α
.Lx91_0:
 .quad 5
xchain0_n76_α:
# IR_ASSIGN gva
bb77_α:
 mov rax, qword ptr [r12 + 1680]
 mov rdx, qword ptr [r12 + 1688]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 1664], rax
 mov qword ptr [r12 + 1672], rdx
 jmp main_γ
 xchain0_n76_β:
 jmp main_γ
xchain0_n77_α:
bb78_α:
# BOX IR_CALL ge(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+1744] -> [r12+1712]
 mov rax, qword ptr [r12 + 1744]
 mov qword ptr [r12 + 1712], rax
 mov rax, qword ptr [r12 + 1752]
 mov qword ptr [r12 + 1720], rax
# marshal arg1 = producer-box slot [r12+1760] -> [r12+1728]
 mov rax, qword ptr [r12 + 1760]
 mov qword ptr [r12 + 1728], rax
 mov rax, qword ptr [r12 + 1768]
 mov qword ptr [r12 + 1736], rax
  .section .rodata
  .Lbynamefn94: .string "ge"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn94]
 lea rsi, [r12 + 1712]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1696], rax
 mov qword ptr [r12 + 1704], rdx
 cmp eax, 99
 je xchain0_n79_α
 jmp xchain0_n78_α
xchain0_n77_β:
 jmp xchain0_n79_α
xchain0_n78_α:
# IR_LIT_INTEGER
bb79_α:
 mov qword ptr [r12 + 1856], 6
 mov rax, qword ptr [rip + .Lx95_0]
 mov qword ptr [r12 + 1864], rax
 jmp xchain0_n80_α
 xchain0_n78_β:
 jmp xchain0_n85_α
.Lx95_0:
 .quad 4
xchain0_n79_α:
# IR_LIT_STRING
bb80_α:
 mov qword ptr [r12 + 1792], 1
 mov rax, qword ptr [rip + .Lx96_0]
 mov qword ptr [r12 + 1800], rax
 jmp xchain0_n81_α
 xchain0_n79_β:
 jmp main_γ
.Lx96_0:
 .quad .Lx96_0_s
.Lx96_0_s:
 .string "FAIL 912/016: ge(7,5) should succeed"
xchain0_n80_α:
# IR_LIT_INTEGER
bb81_α:
 mov qword ptr [r12 + 1888], 6
 mov rax, qword ptr [rip + .Lx97_0]
 mov qword ptr [r12 + 1896], rax
 jmp xchain0_n82_α
 xchain0_n80_β:
 jmp xchain0_n85_α
.Lx97_0:
 .quad 5
xchain0_n81_α:
# IR_ASSIGN gva
bb82_α:
 mov rax, qword ptr [r12 + 1792]
 mov rdx, qword ptr [r12 + 1800]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 1776], rax
 mov qword ptr [r12 + 1784], rdx
 jmp main_γ
 xchain0_n81_β:
 jmp main_γ
xchain0_n82_α:
# IR_LIT_INTEGER
bb83_α:
 mov qword ptr [r12 + 1904], 6
 mov rax, qword ptr [rip + .Lx99_0]
 mov qword ptr [r12 + 1912], rax
 jmp xchain0_n83_α
 xchain0_n82_β:
 jmp xchain0_n85_α
.Lx99_0:
 .quad 1
xchain0_n83_α:
bb84_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 1888]
 cmp eax, 100
 je .Lx100_0
 mov eax, dword ptr [r12 + 1904]
 cmp eax, 100
 je .Lx100_0
 mov eax, dword ptr [r12 + 1888]
 cmp eax, 6
 jne .Lx100_2
 mov eax, dword ptr [r12 + 1904]
 cmp eax, 6
 jne .Lx100_2
.Lx100_1:
 mov rax, qword ptr [r12 + 1896]
 mov rcx, qword ptr [r12 + 1912]
 sub rax, rcx
 mov qword ptr [r12 + 1872], 6
 mov qword ptr [r12 + 1880], rax
 jmp xchain0_n84_α
.Lx100_0:
 mov rdi, qword ptr [r12 + 1888]
 mov rsi, qword ptr [r12 + 1896]
 mov rdx, qword ptr [r12 + 1904]
 mov rcx, qword ptr [r12 + 1912]
 mov r8d, 1
 lea r9, [r12 + 1872]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx100_3
.Lx100_2:
 mov rdi, qword ptr [r12 + 1888]
 mov rsi, qword ptr [r12 + 1896]
 mov rdx, qword ptr [r12 + 1904]
 mov rcx, qword ptr [r12 + 1912]
 mov r8d, 1
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n85_α
 mov qword ptr [r12 + 1872], rax
 mov qword ptr [r12 + 1880], rdx
.Lx100_3:
 jmp xchain0_n84_α
 xchain0_n83_β:
 jmp xchain0_n85_α
xchain0_n84_α:
bb85_α:
# BOX IR_CALL ne(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+1856] -> [r12+1824]
 mov rax, qword ptr [r12 + 1856]
 mov qword ptr [r12 + 1824], rax
 mov rax, qword ptr [r12 + 1864]
 mov qword ptr [r12 + 1832], rax
# marshal arg1 = producer-box slot [r12+1872] -> [r12+1840]
 mov rax, qword ptr [r12 + 1872]
 mov qword ptr [r12 + 1840], rax
 mov rax, qword ptr [r12 + 1880]
 mov qword ptr [r12 + 1848], rax
  .section .rodata
  .Lbynamefn102: .string "ne"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn102]
 lea rsi, [r12 + 1824]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1808], rax
 mov qword ptr [r12 + 1816], rdx
 cmp eax, 99
 je xchain0_n85_α
 jmp xchain0_n86_α
xchain0_n84_β:
 jmp xchain0_n85_α
xchain0_n85_α:
# IR_LIT_STRING
bb86_α:
 mov qword ptr [r12 + 2000], 1
 mov rax, qword ptr [rip + .Lx103_0]
 mov qword ptr [r12 + 2008], rax
 jmp xchain0_n87_α
 xchain0_n85_β:
 jmp xchain0_n91_α
.Lx103_0:
 .quad .Lx103_0_s
.Lx103_0_s:
 .string "12"
xchain0_n86_α:
# IR_LIT_STRING
bb87_α:
 mov qword ptr [r12 + 1936], 1
 mov rax, qword ptr [rip + .Lx104_0]
 mov qword ptr [r12 + 1944], rax
 jmp xchain0_n88_α
 xchain0_n86_β:
 jmp main_γ
.Lx104_0:
 .quad .Lx104_0_s
.Lx104_0_s:
 .string "FAIL 912/017: ne(4,5-1) should fail (both=4)"
xchain0_n87_α:
# IR_LIT_INTEGER
bb88_α:
 mov qword ptr [r12 + 2016], 6
 mov rax, qword ptr [rip + .Lx105_0]
 mov qword ptr [r12 + 2024], rax
 jmp xchain0_n89_α
 xchain0_n87_β:
 jmp xchain0_n91_α
.Lx105_0:
 .quad 12
xchain0_n88_α:
# IR_ASSIGN gva
bb89_α:
 mov rax, qword ptr [r12 + 1936]
 mov rdx, qword ptr [r12 + 1944]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 1920], rax
 mov qword ptr [r12 + 1928], rdx
 jmp main_γ
 xchain0_n88_β:
 jmp main_γ
xchain0_n89_α:
bb90_α:
# BOX IR_CALL ne(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+2000] -> [r12+1968]
 mov rax, qword ptr [r12 + 2000]
 mov qword ptr [r12 + 1968], rax
 mov rax, qword ptr [r12 + 2008]
 mov qword ptr [r12 + 1976], rax
# marshal arg1 = producer-box slot [r12+2016] -> [r12+1984]
 mov rax, qword ptr [r12 + 2016]
 mov qword ptr [r12 + 1984], rax
 mov rax, qword ptr [r12 + 2024]
 mov qword ptr [r12 + 1992], rax
  .section .rodata
  .Lbynamefn108: .string "ne"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn108]
 lea rsi, [r12 + 1968]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1952], rax
 mov qword ptr [r12 + 1960], rdx
 cmp eax, 99
 je xchain0_n91_α
 jmp xchain0_n90_α
xchain0_n89_β:
 jmp xchain0_n91_α
xchain0_n90_α:
# IR_LIT_STRING
bb91_α:
 mov qword ptr [r12 + 2048], 1
 mov rax, qword ptr [rip + .Lx109_0]
 mov qword ptr [r12 + 2056], rax
 jmp xchain0_n92_α
 xchain0_n90_β:
 jmp main_γ
.Lx109_0:
 .quad .Lx109_0_s
.Lx109_0_s:
 .string "FAIL 912/018: ne(string,int) type mismatch"
xchain0_n91_α:
# IR_LIT_STRING
bb92_α:
 mov qword ptr [r12 + 2080], 1
 mov rax, qword ptr [rip + .Lx110_0]
 mov qword ptr [r12 + 2088], rax
 jmp xchain0_n93_α
 xchain0_n91_β:
 jmp main_γ
.Lx110_0:
 .quad .Lx110_0_s
.Lx110_0_s:
 .string "PASS 912_num_pred (18/18)"
xchain0_n92_α:
# IR_ASSIGN gva
bb93_α:
 mov rax, qword ptr [r12 + 2048]
 mov rdx, qword ptr [r12 + 2056]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 2032], rax
 mov qword ptr [r12 + 2040], rdx
 jmp main_γ
 xchain0_n92_β:
 jmp main_γ
xchain0_n93_α:
# IR_ASSIGN gva
bb94_α:
 mov rax, qword ptr [r12 + 2080]
 mov rdx, qword ptr [r12 + 2088]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 2064], rax
 mov qword ptr [r12 + 2072], rdx
 jmp main_γ
 xchain0_n93_β:
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
pop r12
ret
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
pop r12
ret

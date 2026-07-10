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
  call core_lib_init@PLT
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
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 2144], rax
 pop rsi
main_α_body:
# IR_LIT_INTEGER
 xchain0_n0_α:
 mov qword ptr [r12 + 112], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n4_α
.Lx1_0:
 .quad 5
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [r12 + 128], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 136], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n4_α
.Lx2_0:
 .quad 4
 xchain0_n2_α:
# BOX IR_CALL lt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+112] -> [zr+80]
 mov rax, qword ptr [r12 + 112]
 mov qword ptr [r12 + 80], rax
 mov rax, qword ptr [r12 + 120]
 mov qword ptr [r12 + 88], rax
# marshal arg1 = producer-box slot [zr+128] -> [zr+96]
 mov rax, qword ptr [r12 + 128]
 mov qword ptr [r12 + 96], rax
 mov rax, qword ptr [r12 + 136]
 mov qword ptr [r12 + 104], rax
  .section .rodata
  .Lbynamefn3: .string "lt"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn3]
 lea rsi, [r12 + 80]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
 cmp eax, 99
 je xchain0_n4_α
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
# IR_LIT_STRING
 xchain0_n3_α:
 mov qword ptr [r12 + 160], 1
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [r12 + 168], rax
 jmp xchain0_n5_α
 xchain0_n3_β:
 jmp main_γ
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "FAIL 912/001: lt(5,4) should fail"
# IR_LIT_INTEGER
 xchain0_n4_α:
 mov qword ptr [r12 + 224], 6
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [r12 + 232], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n9_α
.Lx5_0:
 .quad 4
# IR_ASSIGN gva
 xchain0_n5_α:
 mov rax, qword ptr [r12 + 160]
 mov rdx, qword ptr [r12 + 168]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 jmp main_γ
 xchain0_n5_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n6_α:
 mov qword ptr [r12 + 240], 6
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [r12 + 248], rax
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp xchain0_n9_α
.Lx7_0:
 .quad 4
 xchain0_n7_α:
# BOX IR_CALL lt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+224] -> [zr+192]
 mov rax, qword ptr [r12 + 224]
 mov qword ptr [r12 + 192], rax
 mov rax, qword ptr [r12 + 232]
 mov qword ptr [r12 + 200], rax
# marshal arg1 = producer-box slot [zr+240] -> [zr+208]
 mov rax, qword ptr [r12 + 240]
 mov qword ptr [r12 + 208], rax
 mov rax, qword ptr [r12 + 248]
 mov qword ptr [r12 + 216], rax
  .section .rodata
  .Lbynamefn8: .string "lt"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn8]
 lea rsi, [r12 + 192]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 cmp eax, 99
 je xchain0_n9_α
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp xchain0_n9_α
# IR_LIT_STRING
 xchain0_n8_α:
 mov qword ptr [r12 + 272], 1
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 280], rax
 jmp xchain0_n10_α
 xchain0_n8_β:
 jmp main_γ
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "FAIL 912/002: lt(4,4) should fail"
# IR_LIT_INTEGER
 xchain0_n9_α:
 mov qword ptr [r12 + 336], 6
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [r12 + 344], rax
 jmp xchain0_n11_α
 xchain0_n9_β:
 jmp xchain0_n14_α
.Lx10_0:
 .quad 4
# IR_ASSIGN gva
 xchain0_n10_α:
 mov rax, qword ptr [r12 + 272]
 mov rdx, qword ptr [r12 + 280]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
 jmp main_γ
 xchain0_n10_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n11_α:
 mov qword ptr [r12 + 352], 6
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [r12 + 360], rax
 jmp xchain0_n12_α
 xchain0_n11_β:
 jmp xchain0_n14_α
.Lx12_0:
 .quad 5
 xchain0_n12_α:
# BOX IR_CALL lt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+336] -> [zr+304]
 mov rax, qword ptr [r12 + 336]
 mov qword ptr [r12 + 304], rax
 mov rax, qword ptr [r12 + 344]
 mov qword ptr [r12 + 312], rax
# marshal arg1 = producer-box slot [zr+352] -> [zr+320]
 mov rax, qword ptr [r12 + 352]
 mov qword ptr [r12 + 320], rax
 mov rax, qword ptr [r12 + 360]
 mov qword ptr [r12 + 328], rax
  .section .rodata
  .Lbynamefn13: .string "lt"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn13]
 lea rsi, [r12 + 304]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 288], rax
 mov qword ptr [r12 + 296], rdx
 cmp eax, 99
 je xchain0_n14_α
 jmp xchain0_n13_α
 xchain0_n12_β:
 jmp xchain0_n14_α
# IR_LIT_INTEGER
 xchain0_n13_α:
 mov qword ptr [r12 + 448], 6
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [r12 + 456], rax
 jmp xchain0_n15_α
 xchain0_n13_β:
 jmp xchain0_n19_α
.Lx14_0:
 .quad 5
# IR_LIT_STRING
 xchain0_n14_α:
 mov qword ptr [r12 + 384], 1
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [r12 + 392], rax
 jmp xchain0_n16_α
 xchain0_n14_β:
 jmp main_γ
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "FAIL 912/003: lt(4,5) should succeed"
# IR_LIT_INTEGER
 xchain0_n15_α:
 mov qword ptr [r12 + 464], 6
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [r12 + 472], rax
 jmp xchain0_n17_α
 xchain0_n15_β:
 jmp xchain0_n19_α
.Lx16_0:
 .quad 2
# IR_ASSIGN gva
 xchain0_n16_α:
 mov rax, qword ptr [r12 + 384]
 mov rdx, qword ptr [r12 + 392]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 368], rax
 mov qword ptr [r12 + 376], rdx
 jmp main_γ
 xchain0_n16_β:
 jmp main_γ
 xchain0_n17_α:
# BOX IR_CALL le(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+448] -> [zr+416]
 mov rax, qword ptr [r12 + 448]
 mov qword ptr [r12 + 416], rax
 mov rax, qword ptr [r12 + 456]
 mov qword ptr [r12 + 424], rax
# marshal arg1 = producer-box slot [zr+464] -> [zr+432]
 mov rax, qword ptr [r12 + 464]
 mov qword ptr [r12 + 432], rax
 mov rax, qword ptr [r12 + 472]
 mov qword ptr [r12 + 440], rax
  .section .rodata
  .Lbynamefn18: .string "le"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn18]
 lea rsi, [r12 + 416]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 400], rax
 mov qword ptr [r12 + 408], rdx
 cmp eax, 99
 je xchain0_n19_α
 jmp xchain0_n18_α
 xchain0_n17_β:
 jmp xchain0_n19_α
# IR_LIT_STRING
 xchain0_n18_α:
 mov qword ptr [r12 + 496], 1
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [r12 + 504], rax
 jmp xchain0_n20_α
 xchain0_n18_β:
 jmp main_γ
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "FAIL 912/004: le(5,2) should fail"
# IR_LIT_INTEGER
 xchain0_n19_α:
 mov qword ptr [r12 + 560], 6
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [r12 + 568], rax
 jmp xchain0_n21_α
 xchain0_n19_β:
 jmp xchain0_n24_α
.Lx20_0:
 .quad 4
# IR_ASSIGN gva
 xchain0_n20_α:
 mov rax, qword ptr [r12 + 496]
 mov rdx, qword ptr [r12 + 504]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 480], rax
 mov qword ptr [r12 + 488], rdx
 jmp main_γ
 xchain0_n20_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n21_α:
 mov qword ptr [r12 + 576], 6
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [r12 + 584], rax
 jmp xchain0_n22_α
 xchain0_n21_β:
 jmp xchain0_n24_α
.Lx22_0:
 .quad 4
 xchain0_n22_α:
# BOX IR_CALL le(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+560] -> [zr+528]
 mov rax, qword ptr [r12 + 560]
 mov qword ptr [r12 + 528], rax
 mov rax, qword ptr [r12 + 568]
 mov qword ptr [r12 + 536], rax
# marshal arg1 = producer-box slot [zr+576] -> [zr+544]
 mov rax, qword ptr [r12 + 576]
 mov qword ptr [r12 + 544], rax
 mov rax, qword ptr [r12 + 584]
 mov qword ptr [r12 + 552], rax
  .section .rodata
  .Lbynamefn23: .string "le"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn23]
 lea rsi, [r12 + 528]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 512], rax
 mov qword ptr [r12 + 520], rdx
 cmp eax, 99
 je xchain0_n24_α
 jmp xchain0_n23_α
 xchain0_n22_β:
 jmp xchain0_n24_α
# IR_LIT_INTEGER
 xchain0_n23_α:
 mov qword ptr [r12 + 672], 6
 mov rax, qword ptr [rip + .Lx24_0]
 mov qword ptr [r12 + 680], rax
 jmp xchain0_n25_α
 xchain0_n23_β:
 jmp xchain0_n29_α
.Lx24_0:
 .quad 4
# IR_LIT_STRING
 xchain0_n24_α:
 mov qword ptr [r12 + 608], 1
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [r12 + 616], rax
 jmp xchain0_n26_α
 xchain0_n24_β:
 jmp main_γ
.Lx25_0:
 .quad .Lx25_0_s
.Lx25_0_s:
 .string "FAIL 912/005: le(4,4) should succeed"
# IR_LIT_INTEGER
 xchain0_n25_α:
 mov qword ptr [r12 + 688], 6
 mov rax, qword ptr [rip + .Lx26_0]
 mov qword ptr [r12 + 696], rax
 jmp xchain0_n27_α
 xchain0_n25_β:
 jmp xchain0_n29_α
.Lx26_0:
 .quad 10
# IR_ASSIGN gva
 xchain0_n26_α:
 mov rax, qword ptr [r12 + 608]
 mov rdx, qword ptr [r12 + 616]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 592], rax
 mov qword ptr [r12 + 600], rdx
 jmp main_γ
 xchain0_n26_β:
 jmp main_γ
 xchain0_n27_α:
# BOX IR_CALL le(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+672] -> [zr+640]
 mov rax, qword ptr [r12 + 672]
 mov qword ptr [r12 + 640], rax
 mov rax, qword ptr [r12 + 680]
 mov qword ptr [r12 + 648], rax
# marshal arg1 = producer-box slot [zr+688] -> [zr+656]
 mov rax, qword ptr [r12 + 688]
 mov qword ptr [r12 + 656], rax
 mov rax, qword ptr [r12 + 696]
 mov qword ptr [r12 + 664], rax
  .section .rodata
  .Lbynamefn28: .string "le"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn28]
 lea rsi, [r12 + 640]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 624], rax
 mov qword ptr [r12 + 632], rdx
 cmp eax, 99
 je xchain0_n29_α
 jmp xchain0_n28_α
 xchain0_n27_β:
 jmp xchain0_n29_α
# IR_LIT_INTEGER
 xchain0_n28_α:
 mov qword ptr [r12 + 784], 6
 mov rax, qword ptr [rip + .Lx29_0]
 mov qword ptr [r12 + 792], rax
 jmp xchain0_n30_α
 xchain0_n28_β:
 jmp xchain0_n34_α
.Lx29_0:
 .quad 4
# IR_LIT_STRING
 xchain0_n29_α:
 mov qword ptr [r12 + 720], 1
 mov rax, qword ptr [rip + .Lx30_0]
 mov qword ptr [r12 + 728], rax
 jmp xchain0_n31_α
 xchain0_n29_β:
 jmp main_γ
.Lx30_0:
 .quad .Lx30_0_s
.Lx30_0_s:
 .string "FAIL 912/006: le(4,10) should succeed"
# IR_LIT_INTEGER
 xchain0_n30_α:
 mov qword ptr [r12 + 800], 6
 mov rax, qword ptr [rip + .Lx31_0]
 mov qword ptr [r12 + 808], rax
 jmp xchain0_n32_α
 xchain0_n30_β:
 jmp xchain0_n34_α
.Lx31_0:
 .quad 5
# IR_ASSIGN gva
 xchain0_n31_α:
 mov rax, qword ptr [r12 + 720]
 mov rdx, qword ptr [r12 + 728]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 704], rax
 mov qword ptr [r12 + 712], rdx
 jmp main_γ
 xchain0_n31_β:
 jmp main_γ
 xchain0_n32_α:
# BOX IR_CALL eq(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+784] -> [zr+752]
 mov rax, qword ptr [r12 + 784]
 mov qword ptr [r12 + 752], rax
 mov rax, qword ptr [r12 + 792]
 mov qword ptr [r12 + 760], rax
# marshal arg1 = producer-box slot [zr+800] -> [zr+768]
 mov rax, qword ptr [r12 + 800]
 mov qword ptr [r12 + 768], rax
 mov rax, qword ptr [r12 + 808]
 mov qword ptr [r12 + 776], rax
  .section .rodata
  .Lbynamefn33: .string "eq"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn33]
 lea rsi, [r12 + 752]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 736], rax
 mov qword ptr [r12 + 744], rdx
 cmp eax, 99
 je xchain0_n34_α
 jmp xchain0_n33_α
 xchain0_n32_β:
 jmp xchain0_n34_α
# IR_LIT_STRING
 xchain0_n33_α:
 mov qword ptr [r12 + 832], 1
 mov rax, qword ptr [rip + .Lx34_0]
 mov qword ptr [r12 + 840], rax
 jmp xchain0_n35_α
 xchain0_n33_β:
 jmp main_γ
.Lx34_0:
 .quad .Lx34_0_s
.Lx34_0_s:
 .string "FAIL 912/007: eq(4,5) should fail"
# IR_LIT_INTEGER
 xchain0_n34_α:
 mov qword ptr [r12 + 896], 6
 mov rax, qword ptr [rip + .Lx35_0]
 mov qword ptr [r12 + 904], rax
 jmp xchain0_n36_α
 xchain0_n34_β:
 jmp xchain0_n39_α
.Lx35_0:
 .quad 5
# IR_ASSIGN gva
 xchain0_n35_α:
 mov rax, qword ptr [r12 + 832]
 mov rdx, qword ptr [r12 + 840]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 816], rax
 mov qword ptr [r12 + 824], rdx
 jmp main_γ
 xchain0_n35_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n36_α:
 mov qword ptr [r12 + 912], 6
 mov rax, qword ptr [rip + .Lx37_0]
 mov qword ptr [r12 + 920], rax
 jmp xchain0_n37_α
 xchain0_n36_β:
 jmp xchain0_n39_α
.Lx37_0:
 .quad 5
 xchain0_n37_α:
# BOX IR_CALL eq(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+896] -> [zr+864]
 mov rax, qword ptr [r12 + 896]
 mov qword ptr [r12 + 864], rax
 mov rax, qword ptr [r12 + 904]
 mov qword ptr [r12 + 872], rax
# marshal arg1 = producer-box slot [zr+912] -> [zr+880]
 mov rax, qword ptr [r12 + 912]
 mov qword ptr [r12 + 880], rax
 mov rax, qword ptr [r12 + 920]
 mov qword ptr [r12 + 888], rax
  .section .rodata
  .Lbynamefn38: .string "eq"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn38]
 lea rsi, [r12 + 864]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 848], rax
 mov qword ptr [r12 + 856], rdx
 cmp eax, 99
 je xchain0_n39_α
 jmp xchain0_n38_α
 xchain0_n37_β:
 jmp xchain0_n39_α
# IR_LIT_INTEGER
 xchain0_n38_α:
 mov qword ptr [r12 + 1008], 6
 mov rax, qword ptr [rip + .Lx39_0]
 mov qword ptr [r12 + 1016], rax
 jmp xchain0_n40_α
 xchain0_n38_β:
 jmp xchain0_n44_α
.Lx39_0:
 .quad 4
# IR_LIT_STRING
 xchain0_n39_α:
 mov qword ptr [r12 + 944], 1
 mov rax, qword ptr [rip + .Lx40_0]
 mov qword ptr [r12 + 952], rax
 jmp xchain0_n41_α
 xchain0_n39_β:
 jmp main_γ
.Lx40_0:
 .quad .Lx40_0_s
.Lx40_0_s:
 .string "FAIL 912/008: eq(5,5) should succeed"
# IR_LIT_INTEGER
 xchain0_n40_α:
 mov qword ptr [r12 + 1024], 6
 mov rax, qword ptr [rip + .Lx41_0]
 mov qword ptr [r12 + 1032], rax
 jmp xchain0_n42_α
 xchain0_n40_β:
 jmp xchain0_n44_α
.Lx41_0:
 .quad 4
# IR_ASSIGN gva
 xchain0_n41_α:
 mov rax, qword ptr [r12 + 944]
 mov rdx, qword ptr [r12 + 952]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 928], rax
 mov qword ptr [r12 + 936], rdx
 jmp main_γ
 xchain0_n41_β:
 jmp main_γ
 xchain0_n42_α:
# BOX IR_CALL ne(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1008] -> [zr+976]
 mov rax, qword ptr [r12 + 1008]
 mov qword ptr [r12 + 976], rax
 mov rax, qword ptr [r12 + 1016]
 mov qword ptr [r12 + 984], rax
# marshal arg1 = producer-box slot [zr+1024] -> [zr+992]
 mov rax, qword ptr [r12 + 1024]
 mov qword ptr [r12 + 992], rax
 mov rax, qword ptr [r12 + 1032]
 mov qword ptr [r12 + 1000], rax
  .section .rodata
  .Lbynamefn43: .string "ne"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn43]
 lea rsi, [r12 + 976]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 960], rax
 mov qword ptr [r12 + 968], rdx
 cmp eax, 99
 je xchain0_n44_α
 jmp xchain0_n43_α
 xchain0_n42_β:
 jmp xchain0_n44_α
# IR_LIT_STRING
 xchain0_n43_α:
 mov qword ptr [r12 + 1056], 1
 mov rax, qword ptr [rip + .Lx44_0]
 mov qword ptr [r12 + 1064], rax
 jmp xchain0_n45_α
 xchain0_n43_β:
 jmp main_γ
.Lx44_0:
 .quad .Lx44_0_s
.Lx44_0_s:
 .string "FAIL 912/009: ne(4,4) should fail"
# IR_LIT_INTEGER
 xchain0_n44_α:
 mov qword ptr [r12 + 1120], 6
 mov rax, qword ptr [rip + .Lx45_0]
 mov qword ptr [r12 + 1128], rax
 jmp xchain0_n46_α
 xchain0_n44_β:
 jmp xchain0_n49_α
.Lx45_0:
 .quad 4
# IR_ASSIGN gva
 xchain0_n45_α:
 mov rax, qword ptr [r12 + 1056]
 mov rdx, qword ptr [r12 + 1064]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 1040], rax
 mov qword ptr [r12 + 1048], rdx
 jmp main_γ
 xchain0_n45_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n46_α:
 mov qword ptr [r12 + 1136], 6
 mov rax, qword ptr [rip + .Lx47_0]
 mov qword ptr [r12 + 1144], rax
 jmp xchain0_n47_α
 xchain0_n46_β:
 jmp xchain0_n49_α
.Lx47_0:
 .quad 6
 xchain0_n47_α:
# BOX IR_CALL ne(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1120] -> [zr+1088]
 mov rax, qword ptr [r12 + 1120]
 mov qword ptr [r12 + 1088], rax
 mov rax, qword ptr [r12 + 1128]
 mov qword ptr [r12 + 1096], rax
# marshal arg1 = producer-box slot [zr+1136] -> [zr+1104]
 mov rax, qword ptr [r12 + 1136]
 mov qword ptr [r12 + 1104], rax
 mov rax, qword ptr [r12 + 1144]
 mov qword ptr [r12 + 1112], rax
  .section .rodata
  .Lbynamefn48: .string "ne"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn48]
 lea rsi, [r12 + 1088]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1072], rax
 mov qword ptr [r12 + 1080], rdx
 cmp eax, 99
 je xchain0_n49_α
 jmp xchain0_n48_α
 xchain0_n47_β:
 jmp xchain0_n49_α
# IR_LIT_INTEGER
 xchain0_n48_α:
 mov qword ptr [r12 + 1232], 6
 mov rax, qword ptr [rip + .Lx49_0]
 mov qword ptr [r12 + 1240], rax
 jmp xchain0_n50_α
 xchain0_n48_β:
 jmp xchain0_n54_α
.Lx49_0:
 .quad 4
# IR_LIT_STRING
 xchain0_n49_α:
 mov qword ptr [r12 + 1168], 1
 mov rax, qword ptr [rip + .Lx50_0]
 mov qword ptr [r12 + 1176], rax
 jmp xchain0_n51_α
 xchain0_n49_β:
 jmp main_γ
.Lx50_0:
 .quad .Lx50_0_s
.Lx50_0_s:
 .string "FAIL 912/010: ne(4,6) should succeed"
# IR_LIT_INTEGER
 xchain0_n50_α:
 mov qword ptr [r12 + 1248], 6
 mov rax, qword ptr [rip + .Lx51_0]
 mov qword ptr [r12 + 1256], rax
 jmp xchain0_n52_α
 xchain0_n50_β:
 jmp xchain0_n54_α
.Lx51_0:
 .quad 6
# IR_ASSIGN gva
 xchain0_n51_α:
 mov rax, qword ptr [r12 + 1168]
 mov rdx, qword ptr [r12 + 1176]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 1152], rax
 mov qword ptr [r12 + 1160], rdx
 jmp main_γ
 xchain0_n51_β:
 jmp main_γ
 xchain0_n52_α:
# BOX IR_CALL gt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1232] -> [zr+1200]
 mov rax, qword ptr [r12 + 1232]
 mov qword ptr [r12 + 1200], rax
 mov rax, qword ptr [r12 + 1240]
 mov qword ptr [r12 + 1208], rax
# marshal arg1 = producer-box slot [zr+1248] -> [zr+1216]
 mov rax, qword ptr [r12 + 1248]
 mov qword ptr [r12 + 1216], rax
 mov rax, qword ptr [r12 + 1256]
 mov qword ptr [r12 + 1224], rax
  .section .rodata
  .Lbynamefn53: .string "gt"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn53]
 lea rsi, [r12 + 1200]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1184], rax
 mov qword ptr [r12 + 1192], rdx
 cmp eax, 99
 je xchain0_n54_α
 jmp xchain0_n53_α
 xchain0_n52_β:
 jmp xchain0_n54_α
# IR_LIT_STRING
 xchain0_n53_α:
 mov qword ptr [r12 + 1280], 1
 mov rax, qword ptr [rip + .Lx54_0]
 mov qword ptr [r12 + 1288], rax
 jmp xchain0_n55_α
 xchain0_n53_β:
 jmp main_γ
.Lx54_0:
 .quad .Lx54_0_s
.Lx54_0_s:
 .string "FAIL 912/011: gt(4,6) should fail"
# IR_LIT_INTEGER
 xchain0_n54_α:
 mov qword ptr [r12 + 1344], 6
 mov rax, qword ptr [rip + .Lx55_0]
 mov qword ptr [r12 + 1352], rax
 jmp xchain0_n56_α
 xchain0_n54_β:
 jmp xchain0_n59_α
.Lx55_0:
 .quad 4
# IR_ASSIGN gva
 xchain0_n55_α:
 mov rax, qword ptr [r12 + 1280]
 mov rdx, qword ptr [r12 + 1288]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 1264], rax
 mov qword ptr [r12 + 1272], rdx
 jmp main_γ
 xchain0_n55_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n56_α:
 mov qword ptr [r12 + 1360], 6
 mov rax, qword ptr [rip + .Lx57_0]
 mov qword ptr [r12 + 1368], rax
 jmp xchain0_n57_α
 xchain0_n56_β:
 jmp xchain0_n59_α
.Lx57_0:
 .quad 4
 xchain0_n57_α:
# BOX IR_CALL gt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1344] -> [zr+1312]
 mov rax, qword ptr [r12 + 1344]
 mov qword ptr [r12 + 1312], rax
 mov rax, qword ptr [r12 + 1352]
 mov qword ptr [r12 + 1320], rax
# marshal arg1 = producer-box slot [zr+1360] -> [zr+1328]
 mov rax, qword ptr [r12 + 1360]
 mov qword ptr [r12 + 1328], rax
 mov rax, qword ptr [r12 + 1368]
 mov qword ptr [r12 + 1336], rax
  .section .rodata
  .Lbynamefn58: .string "gt"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn58]
 lea rsi, [r12 + 1312]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1296], rax
 mov qword ptr [r12 + 1304], rdx
 cmp eax, 99
 je xchain0_n59_α
 jmp xchain0_n58_α
 xchain0_n57_β:
 jmp xchain0_n59_α
# IR_LIT_STRING
 xchain0_n58_α:
 mov qword ptr [r12 + 1392], 1
 mov rax, qword ptr [rip + .Lx59_0]
 mov qword ptr [r12 + 1400], rax
 jmp xchain0_n60_α
 xchain0_n58_β:
 jmp main_γ
.Lx59_0:
 .quad .Lx59_0_s
.Lx59_0_s:
 .string "FAIL 912/012: gt(4,4) should fail"
# IR_LIT_INTEGER
 xchain0_n59_α:
 mov qword ptr [r12 + 1456], 6
 mov rax, qword ptr [rip + .Lx60_0]
 mov qword ptr [r12 + 1464], rax
 jmp xchain0_n61_α
 xchain0_n59_β:
 jmp xchain0_n64_α
.Lx60_0:
 .quad 5
# IR_ASSIGN gva
 xchain0_n60_α:
 mov rax, qword ptr [r12 + 1392]
 mov rdx, qword ptr [r12 + 1400]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 1376], rax
 mov qword ptr [r12 + 1384], rdx
 jmp main_γ
 xchain0_n60_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n61_α:
 mov qword ptr [r12 + 1472], 6
 mov rax, qword ptr [rip + .Lx62_0]
 mov qword ptr [r12 + 1480], rax
 jmp xchain0_n62_α
 xchain0_n61_β:
 jmp xchain0_n64_α
.Lx62_0:
 .quad 2
 xchain0_n62_α:
# BOX IR_CALL gt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1456] -> [zr+1424]
 mov rax, qword ptr [r12 + 1456]
 mov qword ptr [r12 + 1424], rax
 mov rax, qword ptr [r12 + 1464]
 mov qword ptr [r12 + 1432], rax
# marshal arg1 = producer-box slot [zr+1472] -> [zr+1440]
 mov rax, qword ptr [r12 + 1472]
 mov qword ptr [r12 + 1440], rax
 mov rax, qword ptr [r12 + 1480]
 mov qword ptr [r12 + 1448], rax
  .section .rodata
  .Lbynamefn63: .string "gt"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn63]
 lea rsi, [r12 + 1424]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1408], rax
 mov qword ptr [r12 + 1416], rdx
 cmp eax, 99
 je xchain0_n64_α
 jmp xchain0_n63_α
 xchain0_n62_β:
 jmp xchain0_n64_α
# IR_LIT_INTEGER
 xchain0_n63_α:
 mov qword ptr [r12 + 1568], 6
 mov rax, qword ptr [rip + .Lx64_0]
 mov qword ptr [r12 + 1576], rax
 jmp xchain0_n65_α
 xchain0_n63_β:
 jmp xchain0_n69_α
.Lx64_0:
 .quad 5
# IR_LIT_STRING
 xchain0_n64_α:
 mov qword ptr [r12 + 1504], 1
 mov rax, qword ptr [rip + .Lx65_0]
 mov qword ptr [r12 + 1512], rax
 jmp xchain0_n66_α
 xchain0_n64_β:
 jmp main_γ
.Lx65_0:
 .quad .Lx65_0_s
.Lx65_0_s:
 .string "FAIL 912/013: gt(5,2) should succeed"
# IR_LIT_INTEGER
 xchain0_n65_α:
 mov qword ptr [r12 + 1584], 6
 mov rax, qword ptr [rip + .Lx66_0]
 mov qword ptr [r12 + 1592], rax
 jmp xchain0_n67_α
 xchain0_n65_β:
 jmp xchain0_n69_α
.Lx66_0:
 .quad 7
# IR_ASSIGN gva
 xchain0_n66_α:
 mov rax, qword ptr [r12 + 1504]
 mov rdx, qword ptr [r12 + 1512]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 1488], rax
 mov qword ptr [r12 + 1496], rdx
 jmp main_γ
 xchain0_n66_β:
 jmp main_γ
 xchain0_n67_α:
# BOX IR_CALL ge(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1568] -> [zr+1536]
 mov rax, qword ptr [r12 + 1568]
 mov qword ptr [r12 + 1536], rax
 mov rax, qword ptr [r12 + 1576]
 mov qword ptr [r12 + 1544], rax
# marshal arg1 = producer-box slot [zr+1584] -> [zr+1552]
 mov rax, qword ptr [r12 + 1584]
 mov qword ptr [r12 + 1552], rax
 mov rax, qword ptr [r12 + 1592]
 mov qword ptr [r12 + 1560], rax
  .section .rodata
  .Lbynamefn68: .string "ge"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn68]
 lea rsi, [r12 + 1536]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1520], rax
 mov qword ptr [r12 + 1528], rdx
 cmp eax, 99
 je xchain0_n69_α
 jmp xchain0_n68_α
 xchain0_n67_β:
 jmp xchain0_n69_α
# IR_LIT_STRING
 xchain0_n68_α:
 mov qword ptr [r12 + 1616], 1
 mov rax, qword ptr [rip + .Lx69_0]
 mov qword ptr [r12 + 1624], rax
 jmp xchain0_n70_α
 xchain0_n68_β:
 jmp main_γ
.Lx69_0:
 .quad .Lx69_0_s
.Lx69_0_s:
 .string "FAIL 912/014: ge(5,7) should fail"
# IR_LIT_INTEGER
 xchain0_n69_α:
 mov qword ptr [r12 + 1680], 6
 mov rax, qword ptr [rip + .Lx70_0]
 mov qword ptr [r12 + 1688], rax
 jmp xchain0_n71_α
 xchain0_n69_β:
 jmp xchain0_n74_α
.Lx70_0:
 .quad 4
# IR_ASSIGN gva
 xchain0_n70_α:
 mov rax, qword ptr [r12 + 1616]
 mov rdx, qword ptr [r12 + 1624]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 1600], rax
 mov qword ptr [r12 + 1608], rdx
 jmp main_γ
 xchain0_n70_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n71_α:
 mov qword ptr [r12 + 1696], 6
 mov rax, qword ptr [rip + .Lx72_0]
 mov qword ptr [r12 + 1704], rax
 jmp xchain0_n72_α
 xchain0_n71_β:
 jmp xchain0_n74_α
.Lx72_0:
 .quad 4
 xchain0_n72_α:
# BOX IR_CALL ge(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1680] -> [zr+1648]
 mov rax, qword ptr [r12 + 1680]
 mov qword ptr [r12 + 1648], rax
 mov rax, qword ptr [r12 + 1688]
 mov qword ptr [r12 + 1656], rax
# marshal arg1 = producer-box slot [zr+1696] -> [zr+1664]
 mov rax, qword ptr [r12 + 1696]
 mov qword ptr [r12 + 1664], rax
 mov rax, qword ptr [r12 + 1704]
 mov qword ptr [r12 + 1672], rax
  .section .rodata
  .Lbynamefn73: .string "ge"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn73]
 lea rsi, [r12 + 1648]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1632], rax
 mov qword ptr [r12 + 1640], rdx
 cmp eax, 99
 je xchain0_n74_α
 jmp xchain0_n73_α
 xchain0_n72_β:
 jmp xchain0_n74_α
# IR_LIT_INTEGER
 xchain0_n73_α:
 mov qword ptr [r12 + 1792], 6
 mov rax, qword ptr [rip + .Lx74_0]
 mov qword ptr [r12 + 1800], rax
 jmp xchain0_n75_α
 xchain0_n73_β:
 jmp xchain0_n79_α
.Lx74_0:
 .quad 7
# IR_LIT_STRING
 xchain0_n74_α:
 mov qword ptr [r12 + 1728], 1
 mov rax, qword ptr [rip + .Lx75_0]
 mov qword ptr [r12 + 1736], rax
 jmp xchain0_n76_α
 xchain0_n74_β:
 jmp main_γ
.Lx75_0:
 .quad .Lx75_0_s
.Lx75_0_s:
 .string "FAIL 912/015: ge(4,4) should succeed"
# IR_LIT_INTEGER
 xchain0_n75_α:
 mov qword ptr [r12 + 1808], 6
 mov rax, qword ptr [rip + .Lx76_0]
 mov qword ptr [r12 + 1816], rax
 jmp xchain0_n77_α
 xchain0_n75_β:
 jmp xchain0_n79_α
.Lx76_0:
 .quad 5
# IR_ASSIGN gva
 xchain0_n76_α:
 mov rax, qword ptr [r12 + 1728]
 mov rdx, qword ptr [r12 + 1736]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 1712], rax
 mov qword ptr [r12 + 1720], rdx
 jmp main_γ
 xchain0_n76_β:
 jmp main_γ
 xchain0_n77_α:
# BOX IR_CALL ge(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1792] -> [zr+1760]
 mov rax, qword ptr [r12 + 1792]
 mov qword ptr [r12 + 1760], rax
 mov rax, qword ptr [r12 + 1800]
 mov qword ptr [r12 + 1768], rax
# marshal arg1 = producer-box slot [zr+1808] -> [zr+1776]
 mov rax, qword ptr [r12 + 1808]
 mov qword ptr [r12 + 1776], rax
 mov rax, qword ptr [r12 + 1816]
 mov qword ptr [r12 + 1784], rax
  .section .rodata
  .Lbynamefn78: .string "ge"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn78]
 lea rsi, [r12 + 1760]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1744], rax
 mov qword ptr [r12 + 1752], rdx
 cmp eax, 99
 je xchain0_n79_α
 jmp xchain0_n78_α
 xchain0_n77_β:
 jmp xchain0_n79_α
# IR_LIT_INTEGER
 xchain0_n78_α:
 mov qword ptr [r12 + 1904], 6
 mov rax, qword ptr [rip + .Lx79_0]
 mov qword ptr [r12 + 1912], rax
 jmp xchain0_n80_α
 xchain0_n78_β:
 jmp xchain0_n85_α
.Lx79_0:
 .quad 4
# IR_LIT_STRING
 xchain0_n79_α:
 mov qword ptr [r12 + 1840], 1
 mov rax, qword ptr [rip + .Lx80_0]
 mov qword ptr [r12 + 1848], rax
 jmp xchain0_n81_α
 xchain0_n79_β:
 jmp main_γ
.Lx80_0:
 .quad .Lx80_0_s
.Lx80_0_s:
 .string "FAIL 912/016: ge(7,5) should succeed"
# IR_LIT_INTEGER
 xchain0_n80_α:
 mov qword ptr [r12 + 1936], 6
 mov rax, qword ptr [rip + .Lx81_0]
 mov qword ptr [r12 + 1944], rax
 jmp xchain0_n82_α
 xchain0_n80_β:
 jmp xchain0_n85_α
.Lx81_0:
 .quad 5
# IR_ASSIGN gva
 xchain0_n81_α:
 mov rax, qword ptr [r12 + 1840]
 mov rdx, qword ptr [r12 + 1848]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 1824], rax
 mov qword ptr [r12 + 1832], rdx
 jmp main_γ
 xchain0_n81_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n82_α:
 mov qword ptr [r12 + 1952], 6
 mov rax, qword ptr [rip + .Lx83_0]
 mov qword ptr [r12 + 1960], rax
 jmp xchain0_n83_α
 xchain0_n82_β:
 jmp xchain0_n85_α
.Lx83_0:
 .quad 1
 xchain0_n83_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 1936]
 cmp eax, 100
 je .Lx84_0
 mov eax, dword ptr [r12 + 1952]
 cmp eax, 100
 je .Lx84_0
 mov eax, dword ptr [r12 + 1936]
 cmp eax, 6
 jne .Lx84_2
 mov eax, dword ptr [r12 + 1952]
 cmp eax, 6
 jne .Lx84_2
.Lx84_1:
 mov rax, qword ptr [r12 + 1944]
 mov rcx, qword ptr [r12 + 1960]
 sub rax, rcx
 mov qword ptr [r12 + 1920], 6
 mov qword ptr [r12 + 1928], rax
 jmp xchain0_n84_α
.Lx84_0:
 mov rdi, qword ptr [r12 + 1936]
 mov rsi, qword ptr [r12 + 1944]
 mov rdx, qword ptr [r12 + 1952]
 mov rcx, qword ptr [r12 + 1960]
 mov r8d, 1
 lea r9, [r12 + 1920]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx84_3
.Lx84_2:
 mov rdi, qword ptr [r12 + 1936]
 mov rsi, qword ptr [r12 + 1944]
 mov rdx, qword ptr [r12 + 1952]
 mov rcx, qword ptr [r12 + 1960]
 mov r8d, 1
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n85_α
 mov qword ptr [r12 + 1920], rax
 mov qword ptr [r12 + 1928], rdx
.Lx84_3:
 jmp xchain0_n84_α
 xchain0_n83_β:
 jmp xchain0_n85_α
 xchain0_n84_α:
# BOX IR_CALL ne(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1904] -> [zr+1872]
 mov rax, qword ptr [r12 + 1904]
 mov qword ptr [r12 + 1872], rax
 mov rax, qword ptr [r12 + 1912]
 mov qword ptr [r12 + 1880], rax
# marshal arg1 = producer-box slot [zr+1920] -> [zr+1888]
 mov rax, qword ptr [r12 + 1920]
 mov qword ptr [r12 + 1888], rax
 mov rax, qword ptr [r12 + 1928]
 mov qword ptr [r12 + 1896], rax
  .section .rodata
  .Lbynamefn85: .string "ne"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn85]
 lea rsi, [r12 + 1872]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1856], rax
 mov qword ptr [r12 + 1864], rdx
 cmp eax, 99
 je xchain0_n85_α
 jmp xchain0_n86_α
 xchain0_n84_β:
 jmp xchain0_n85_α
# IR_LIT_STRING
 xchain0_n85_α:
 mov qword ptr [r12 + 2048], 1
 mov rax, qword ptr [rip + .Lx86_0]
 mov qword ptr [r12 + 2056], rax
 jmp xchain0_n87_α
 xchain0_n85_β:
 jmp xchain0_n91_α
.Lx86_0:
 .quad .Lx86_0_s
.Lx86_0_s:
 .string "12"
# IR_LIT_STRING
 xchain0_n86_α:
 mov qword ptr [r12 + 1984], 1
 mov rax, qword ptr [rip + .Lx87_0]
 mov qword ptr [r12 + 1992], rax
 jmp xchain0_n88_α
 xchain0_n86_β:
 jmp main_γ
.Lx87_0:
 .quad .Lx87_0_s
.Lx87_0_s:
 .string "FAIL 912/017: ne(4,5-1) should fail (both=4)"
# IR_LIT_INTEGER
 xchain0_n87_α:
 mov qword ptr [r12 + 2064], 6
 mov rax, qword ptr [rip + .Lx88_0]
 mov qword ptr [r12 + 2072], rax
 jmp xchain0_n89_α
 xchain0_n87_β:
 jmp xchain0_n91_α
.Lx88_0:
 .quad 12
# IR_ASSIGN gva
 xchain0_n88_α:
 mov rax, qword ptr [r12 + 1984]
 mov rdx, qword ptr [r12 + 1992]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 1968], rax
 mov qword ptr [r12 + 1976], rdx
 jmp main_γ
 xchain0_n88_β:
 jmp main_γ
 xchain0_n89_α:
# BOX IR_CALL ne(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2048] -> [zr+2016]
 mov rax, qword ptr [r12 + 2048]
 mov qword ptr [r12 + 2016], rax
 mov rax, qword ptr [r12 + 2056]
 mov qword ptr [r12 + 2024], rax
# marshal arg1 = producer-box slot [zr+2064] -> [zr+2032]
 mov rax, qword ptr [r12 + 2064]
 mov qword ptr [r12 + 2032], rax
 mov rax, qword ptr [r12 + 2072]
 mov qword ptr [r12 + 2040], rax
  .section .rodata
  .Lbynamefn90: .string "ne"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn90]
 lea rsi, [r12 + 2016]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2000], rax
 mov qword ptr [r12 + 2008], rdx
 cmp eax, 99
 je xchain0_n91_α
 jmp xchain0_n90_α
 xchain0_n89_β:
 jmp xchain0_n91_α
# IR_LIT_STRING
 xchain0_n90_α:
 mov qword ptr [r12 + 2096], 1
 mov rax, qword ptr [rip + .Lx91_0]
 mov qword ptr [r12 + 2104], rax
 jmp xchain0_n92_α
 xchain0_n90_β:
 jmp main_γ
.Lx91_0:
 .quad .Lx91_0_s
.Lx91_0_s:
 .string "FAIL 912/018: ne(string,int) type mismatch"
# IR_LIT_STRING
 xchain0_n91_α:
 mov qword ptr [r12 + 2128], 1
 mov rax, qword ptr [rip + .Lx92_0]
 mov qword ptr [r12 + 2136], rax
 jmp xchain0_n93_α
 xchain0_n91_β:
 jmp main_γ
.Lx92_0:
 .quad .Lx92_0_s
.Lx92_0_s:
 .string "PASS 912_num_pred (18/18)"
# IR_ASSIGN gva
 xchain0_n92_α:
 mov rax, qword ptr [r12 + 2096]
 mov rdx, qword ptr [r12 + 2104]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 2080], rax
 mov qword ptr [r12 + 2088], rdx
 jmp main_γ
 xchain0_n92_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain0_n93_α:
 mov rax, qword ptr [r12 + 2128]
 mov rdx, qword ptr [r12 + 2136]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 2112], rax
 mov qword ptr [r12 + 2120], rdx
 jmp main_γ
 xchain0_n93_β:
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 2144]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret

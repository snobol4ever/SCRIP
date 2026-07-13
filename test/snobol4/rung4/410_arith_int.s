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
  call rt_frame@PLT
  mov rdi, rax
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
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  mov qword ptr [r12 + 2312], rsp
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 2304], rax
 pop rsi
main_α_body:
# IR_LIT_INTEGER
 xchain0_n0_α:
 mov qword ptr [r12 + 192], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 200], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n4_α
.Lx1_0:
 .quad 3
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [r12 + 224], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 232], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n4_α
.Lx2_0:
 .quad 2
 xchain0_n2_α:
# IR_BINOP_ARITH
.Lx3_1:
 mov rax, 3
 mov rcx, 2
 add rax, rcx
 mov qword ptr [r12 + 160], 6
 mov qword ptr [r12 + 168], rax
 jmp xchain0_n3_α
.Lx3_0:
 mov rdi, qword ptr [r12 + 192]
 mov rsi, qword ptr [r12 + 200]
 mov rdx, qword ptr [r12 + 224]
 mov rcx, qword ptr [r12 + 232]
 mov r8d, 0
 lea r9, [r12 + 160]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx3_3
.Lx3_2:
 mov rdi, qword ptr [r12 + 192]
 mov rsi, qword ptr [r12 + 200]
 mov rdx, qword ptr [r12 + 224]
 mov rcx, qword ptr [r12 + 232]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n4_α
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
.Lx3_3:
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
# IR_LIT_INTEGER
 xchain0_n3_α:
 mov qword ptr [r12 + 256], 6
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [r12 + 264], rax
 jmp xchain0_n5_α
 xchain0_n3_β:
 jmp xchain0_n4_α
.Lx4_0:
 .quad 5
# IR_LIT_INTEGER
 xchain0_n4_α:
 mov qword ptr [r12 + 432], 6
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [r12 + 440], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n11_α
.Lx5_0:
 .quad 3
 xchain0_n5_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+160] -> [zr+112]
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 112], rax
 mov rax, qword ptr [r12 + 168]
 mov qword ptr [r12 + 120], rax
# marshal arg1 = producer-box slot [zr+256] -> [zr+128]
 mov rax, qword ptr [r12 + 256]
 mov qword ptr [r12 + 128], rax
 mov rax, qword ptr [r12 + 264]
 mov qword ptr [r12 + 136], rax
  .section .rodata
  .Lbynamefn6: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn6]
 lea rsi, [r12 + 112]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 cmp eax, 99
 je xchain0_n4_α
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n4_α
# IR_LIT_INTEGER
 xchain0_n6_α:
 mov qword ptr [r12 + 464], 6
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [r12 + 472], rax
 jmp xchain0_n8_α
 xchain0_n6_β:
 jmp xchain0_n11_α
.Lx7_0:
 .quad 2
# IR_LIT_STRING
 xchain0_n7_α:
 mov qword ptr [r12 + 304], 1
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [r12 + 312], rax
 jmp xchain0_n9_α
 xchain0_n7_β:
 jmp main_γ
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "FAIL 410/001: 3+2"
 xchain0_n8_α:
# IR_BINOP_ARITH
.Lx9_1:
 mov rax, 3
 mov rcx, 2
 sub rax, rcx
 mov qword ptr [r12 + 400], 6
 mov qword ptr [r12 + 408], rax
 jmp xchain0_n10_α
.Lx9_0:
 mov rdi, qword ptr [r12 + 432]
 mov rsi, qword ptr [r12 + 440]
 mov rdx, qword ptr [r12 + 464]
 mov rcx, qword ptr [r12 + 472]
 mov r8d, 1
 lea r9, [r12 + 400]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx9_3
.Lx9_2:
 mov rdi, qword ptr [r12 + 432]
 mov rsi, qword ptr [r12 + 440]
 mov rdx, qword ptr [r12 + 464]
 mov rcx, qword ptr [r12 + 472]
 mov r8d, 1
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n11_α
 mov qword ptr [r12 + 400], rax
 mov qword ptr [r12 + 408], rdx
.Lx9_3:
 jmp xchain0_n10_α
 xchain0_n8_β:
 jmp xchain0_n11_α
# IR_ASSIGN gva
 xchain0_n9_α:
 mov rax, qword ptr [r12 + 304]
 mov rdx, qword ptr [r12 + 312]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 288], rax
 mov qword ptr [r12 + 296], rdx
 jmp main_γ
 xchain0_n9_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n10_α:
 mov qword ptr [r12 + 496], 6
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 504], rax
 jmp xchain0_n12_α
 xchain0_n10_β:
 jmp xchain0_n11_α
.Lx11_0:
 .quad 1
# IR_LIT_INTEGER
 xchain0_n11_α:
 mov qword ptr [r12 + 672], 6
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [r12 + 680], rax
 jmp xchain0_n13_α
 xchain0_n11_β:
 jmp xchain0_n18_α
.Lx12_0:
 .quad 3
 xchain0_n12_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+400] -> [zr+352]
 mov rax, qword ptr [r12 + 400]
 mov qword ptr [r12 + 352], rax
 mov rax, qword ptr [r12 + 408]
 mov qword ptr [r12 + 360], rax
# marshal arg1 = producer-box slot [zr+496] -> [zr+368]
 mov rax, qword ptr [r12 + 496]
 mov qword ptr [r12 + 368], rax
 mov rax, qword ptr [r12 + 504]
 mov qword ptr [r12 + 376], rax
  .section .rodata
  .Lbynamefn13: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn13]
 lea rsi, [r12 + 352]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 cmp eax, 99
 je xchain0_n11_α
 jmp xchain0_n14_α
 xchain0_n12_β:
 jmp xchain0_n11_α
# IR_LIT_INTEGER
 xchain0_n13_α:
 mov qword ptr [r12 + 704], 6
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [r12 + 712], rax
 jmp xchain0_n15_α
 xchain0_n13_β:
 jmp xchain0_n18_α
.Lx14_0:
 .quad 2
# IR_LIT_STRING
 xchain0_n14_α:
 mov qword ptr [r12 + 544], 1
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [r12 + 552], rax
 jmp xchain0_n16_α
 xchain0_n14_β:
 jmp main_γ
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "FAIL 410/002: 3-2"
 xchain0_n15_α:
# IR_BINOP_ARITH
.Lx16_1:
 mov rax, 3
 mov rcx, 2
 imul rax, rcx
 mov qword ptr [r12 + 640], 6
 mov qword ptr [r12 + 648], rax
 jmp xchain0_n17_α
.Lx16_0:
 mov rdi, qword ptr [r12 + 672]
 mov rsi, qword ptr [r12 + 680]
 mov rdx, qword ptr [r12 + 704]
 mov rcx, qword ptr [r12 + 712]
 mov r8d, 2
 lea r9, [r12 + 640]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx16_3
.Lx16_2:
 mov rdi, qword ptr [r12 + 672]
 mov rsi, qword ptr [r12 + 680]
 mov rdx, qword ptr [r12 + 704]
 mov rcx, qword ptr [r12 + 712]
 mov r8d, 2
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n18_α
 mov qword ptr [r12 + 640], rax
 mov qword ptr [r12 + 648], rdx
.Lx16_3:
 jmp xchain0_n17_α
 xchain0_n15_β:
 jmp xchain0_n18_α
# IR_ASSIGN gva
 xchain0_n16_α:
 mov rax, qword ptr [r12 + 544]
 mov rdx, qword ptr [r12 + 552]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 528], rax
 mov qword ptr [r12 + 536], rdx
 jmp main_γ
 xchain0_n16_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n17_α:
 mov qword ptr [r12 + 736], 6
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [r12 + 744], rax
 jmp xchain0_n19_α
 xchain0_n17_β:
 jmp xchain0_n18_α
.Lx18_0:
 .quad 6
# IR_LIT_INTEGER
 xchain0_n18_α:
 mov qword ptr [r12 + 912], 6
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [r12 + 920], rax
 jmp xchain0_n20_α
 xchain0_n18_β:
 jmp xchain0_n25_α
.Lx19_0:
 .quad 5
 xchain0_n19_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+640] -> [zr+592]
 mov rax, qword ptr [r12 + 640]
 mov qword ptr [r12 + 592], rax
 mov rax, qword ptr [r12 + 648]
 mov qword ptr [r12 + 600], rax
# marshal arg1 = producer-box slot [zr+736] -> [zr+608]
 mov rax, qword ptr [r12 + 736]
 mov qword ptr [r12 + 608], rax
 mov rax, qword ptr [r12 + 744]
 mov qword ptr [r12 + 616], rax
  .section .rodata
  .Lbynamefn20: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn20]
 lea rsi, [r12 + 592]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 576], rax
 mov qword ptr [r12 + 584], rdx
 cmp eax, 99
 je xchain0_n18_α
 jmp xchain0_n21_α
 xchain0_n19_β:
 jmp xchain0_n18_α
# IR_LIT_INTEGER
 xchain0_n20_α:
 mov qword ptr [r12 + 944], 6
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [r12 + 952], rax
 jmp xchain0_n22_α
 xchain0_n20_β:
 jmp xchain0_n25_α
.Lx21_0:
 .quad 2
# IR_LIT_STRING
 xchain0_n21_α:
 mov qword ptr [r12 + 784], 1
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [r12 + 792], rax
 jmp xchain0_n23_α
 xchain0_n21_β:
 jmp main_γ
.Lx22_0:
 .quad .Lx22_0_s
.Lx22_0_s:
 .string "FAIL 410/003: 3*2"
 xchain0_n22_α:
# IR_BINOP_ARITH
.Lx23_1:
 mov rax, 5
 mov rcx, 2
 cqo
 idiv rcx
 mov qword ptr [r12 + 880], 6
 mov qword ptr [r12 + 888], rax
 jmp xchain0_n24_α
.Lx23_0:
 mov rdi, qword ptr [r12 + 912]
 mov rsi, qword ptr [r12 + 920]
 mov rdx, qword ptr [r12 + 944]
 mov rcx, qword ptr [r12 + 952]
 mov r8d, 3
 lea r9, [r12 + 880]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx23_3
.Lx23_2:
 mov rdi, qword ptr [r12 + 912]
 mov rsi, qword ptr [r12 + 920]
 mov rdx, qword ptr [r12 + 944]
 mov rcx, qword ptr [r12 + 952]
 mov r8d, 3
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n25_α
 mov qword ptr [r12 + 880], rax
 mov qword ptr [r12 + 888], rdx
.Lx23_3:
 jmp xchain0_n24_α
 xchain0_n22_β:
 jmp xchain0_n25_α
# IR_ASSIGN gva
 xchain0_n23_α:
 mov rax, qword ptr [r12 + 784]
 mov rdx, qword ptr [r12 + 792]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 768], rax
 mov qword ptr [r12 + 776], rdx
 jmp main_γ
 xchain0_n23_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n24_α:
 mov qword ptr [r12 + 976], 6
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [r12 + 984], rax
 jmp xchain0_n26_α
 xchain0_n24_β:
 jmp xchain0_n25_α
.Lx25_0:
 .quad 2
# IR_LIT_INTEGER
 xchain0_n25_α:
 mov qword ptr [r12 + 1152], 6
 mov rax, qword ptr [rip + .Lx26_0]
 mov qword ptr [r12 + 1160], rax
 jmp xchain0_n27_α
 xchain0_n25_β:
 jmp xchain0_n32_α
.Lx26_0:
 .quad 2
 xchain0_n26_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+880] -> [zr+832]
 mov rax, qword ptr [r12 + 880]
 mov qword ptr [r12 + 832], rax
 mov rax, qword ptr [r12 + 888]
 mov qword ptr [r12 + 840], rax
# marshal arg1 = producer-box slot [zr+976] -> [zr+848]
 mov rax, qword ptr [r12 + 976]
 mov qword ptr [r12 + 848], rax
 mov rax, qword ptr [r12 + 984]
 mov qword ptr [r12 + 856], rax
  .section .rodata
  .Lbynamefn27: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn27]
 lea rsi, [r12 + 832]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 816], rax
 mov qword ptr [r12 + 824], rdx
 cmp eax, 99
 je xchain0_n25_α
 jmp xchain0_n28_α
 xchain0_n26_β:
 jmp xchain0_n25_α
# IR_LIT_INTEGER
 xchain0_n27_α:
 mov qword ptr [r12 + 1184], 6
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [r12 + 1192], rax
 jmp xchain0_n29_α
 xchain0_n27_β:
 jmp xchain0_n32_α
.Lx28_0:
 .quad 3
# IR_LIT_STRING
 xchain0_n28_α:
 mov qword ptr [r12 + 1024], 1
 mov rax, qword ptr [rip + .Lx29_0]
 mov qword ptr [r12 + 1032], rax
 jmp xchain0_n30_α
 xchain0_n28_β:
 jmp main_γ
.Lx29_0:
 .quad .Lx29_0_s
.Lx29_0_s:
 .string "FAIL 410/004: 5/2 integer division"
 xchain0_n29_α:
# IR_BINOP_ARITH_REAL
 mov rdi, qword ptr [r12 + 1152]
 mov rsi, qword ptr [r12 + 1160]
 mov rdx, qword ptr [r12 + 1184]
 mov rcx, qword ptr [r12 + 1192]
 mov r8d, 18
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n32_α
 mov qword ptr [r12 + 1120], rax
 mov qword ptr [r12 + 1128], rdx
 jmp xchain0_n31_α
 xchain0_n29_β:
 jmp xchain0_n32_α
# IR_ASSIGN gva
 xchain0_n30_α:
 mov rax, qword ptr [r12 + 1024]
 mov rdx, qword ptr [r12 + 1032]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 1008], rax
 mov qword ptr [r12 + 1016], rdx
 jmp main_γ
 xchain0_n30_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n31_α:
 mov qword ptr [r12 + 1216], 6
 mov rax, qword ptr [rip + .Lx32_0]
 mov qword ptr [r12 + 1224], rax
 jmp xchain0_n33_α
 xchain0_n31_β:
 jmp xchain0_n32_α
.Lx32_0:
 .quad 8
# IR_LIT_STRING
 xchain0_n32_α:
 mov qword ptr [r12 + 1392], 1
 mov rax, qword ptr [rip + .Lx33_0]
 mov qword ptr [r12 + 1400], rax
 jmp xchain0_n34_α
 xchain0_n32_β:
 jmp xchain0_n39_α
.Lx33_0:
 .quad .Lx33_0_s
.Lx33_0_s:
 .string "3"
 xchain0_n33_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1120] -> [zr+1072]
 mov rax, qword ptr [r12 + 1120]
 mov qword ptr [r12 + 1072], rax
 mov rax, qword ptr [r12 + 1128]
 mov qword ptr [r12 + 1080], rax
# marshal arg1 = producer-box slot [zr+1216] -> [zr+1088]
 mov rax, qword ptr [r12 + 1216]
 mov qword ptr [r12 + 1088], rax
 mov rax, qword ptr [r12 + 1224]
 mov qword ptr [r12 + 1096], rax
  .section .rodata
  .Lbynamefn34: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn34]
 lea rsi, [r12 + 1072]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1056], rax
 mov qword ptr [r12 + 1064], rdx
 cmp eax, 99
 je xchain0_n32_α
 jmp xchain0_n35_α
 xchain0_n33_β:
 jmp xchain0_n32_α
# IR_LIT_INTEGER
 xchain0_n34_α:
 mov qword ptr [r12 + 1424], 6
 mov rax, qword ptr [rip + .Lx35_0]
 mov qword ptr [r12 + 1432], rax
 jmp xchain0_n36_α
 xchain0_n34_β:
 jmp xchain0_n39_α
.Lx35_0:
 .quad 2
# IR_LIT_STRING
 xchain0_n35_α:
 mov qword ptr [r12 + 1264], 1
 mov rax, qword ptr [rip + .Lx36_0]
 mov qword ptr [r12 + 1272], rax
 jmp xchain0_n37_α
 xchain0_n35_β:
 jmp main_γ
.Lx36_0:
 .quad .Lx36_0_s
.Lx36_0_s:
 .string "FAIL 410/005: 2**3"
 xchain0_n36_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 1392]
 cmp eax, 100
 je .Lx37_0
 mov eax, dword ptr [r12 + 1392]
 cmp eax, 6
 jne .Lx37_2
.Lx37_1:
 mov rax, qword ptr [r12 + 1400]
 mov rcx, 2
 add rax, rcx
 mov qword ptr [r12 + 1360], 6
 mov qword ptr [r12 + 1368], rax
 jmp xchain0_n38_α
.Lx37_0:
 mov rdi, qword ptr [r12 + 1392]
 mov rsi, qword ptr [r12 + 1400]
 mov rdx, qword ptr [r12 + 1424]
 mov rcx, qword ptr [r12 + 1432]
 mov r8d, 0
 lea r9, [r12 + 1360]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx37_3
.Lx37_2:
 mov rdi, qword ptr [r12 + 1392]
 mov rsi, qword ptr [r12 + 1400]
 mov rdx, qword ptr [r12 + 1424]
 mov rcx, qword ptr [r12 + 1432]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n39_α
 mov qword ptr [r12 + 1360], rax
 mov qword ptr [r12 + 1368], rdx
.Lx37_3:
 jmp xchain0_n38_α
 xchain0_n36_β:
 jmp xchain0_n39_α
# IR_ASSIGN gva
 xchain0_n37_α:
 mov rax, qword ptr [r12 + 1264]
 mov rdx, qword ptr [r12 + 1272]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 1248], rax
 mov qword ptr [r12 + 1256], rdx
 jmp main_γ
 xchain0_n37_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n38_α:
 mov qword ptr [r12 + 1456], 6
 mov rax, qword ptr [rip + .Lx39_0]
 mov qword ptr [r12 + 1464], rax
 jmp xchain0_n40_α
 xchain0_n38_β:
 jmp xchain0_n39_α
.Lx39_0:
 .quad 5
# IR_LIT_INTEGER
 xchain0_n39_α:
 mov qword ptr [r12 + 1632], 6
 mov rax, qword ptr [rip + .Lx40_0]
 mov qword ptr [r12 + 1640], rax
 jmp xchain0_n41_α
 xchain0_n39_β:
 jmp xchain0_n46_α
.Lx40_0:
 .quad 3
 xchain0_n40_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1360] -> [zr+1312]
 mov rax, qword ptr [r12 + 1360]
 mov qword ptr [r12 + 1312], rax
 mov rax, qword ptr [r12 + 1368]
 mov qword ptr [r12 + 1320], rax
# marshal arg1 = producer-box slot [zr+1456] -> [zr+1328]
 mov rax, qword ptr [r12 + 1456]
 mov qword ptr [r12 + 1328], rax
 mov rax, qword ptr [r12 + 1464]
 mov qword ptr [r12 + 1336], rax
  .section .rodata
  .Lbynamefn41: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn41]
 lea rsi, [r12 + 1312]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1296], rax
 mov qword ptr [r12 + 1304], rdx
 cmp eax, 99
 je xchain0_n39_α
 jmp xchain0_n42_α
 xchain0_n40_β:
 jmp xchain0_n39_α
# IR_LIT_STRING
 xchain0_n41_α:
 mov qword ptr [r12 + 1664], 1
 mov rax, qword ptr [rip + .Lx42_0]
 mov qword ptr [r12 + 1672], rax
 jmp xchain0_n43_α
 xchain0_n41_β:
 jmp xchain0_n46_α
.Lx42_0:
 .quad .Lx42_0_s
.Lx42_0_s:
 .string "-2"
# IR_LIT_STRING
 xchain0_n42_α:
 mov qword ptr [r12 + 1504], 1
 mov rax, qword ptr [rip + .Lx43_0]
 mov qword ptr [r12 + 1512], rax
 jmp xchain0_n44_α
 xchain0_n42_β:
 jmp main_γ
.Lx43_0:
 .quad .Lx43_0_s
.Lx43_0_s:
 .string "FAIL 410/006: string+int coerce"
 xchain0_n43_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 1664]
 cmp eax, 100
 je .Lx44_0
 mov eax, dword ptr [r12 + 1664]
 cmp eax, 6
 jne .Lx44_2
.Lx44_1:
 mov rax, 3
 mov rcx, qword ptr [r12 + 1672]
 add rax, rcx
 mov qword ptr [r12 + 1600], 6
 mov qword ptr [r12 + 1608], rax
 jmp xchain0_n45_α
.Lx44_0:
 mov rdi, qword ptr [r12 + 1632]
 mov rsi, qword ptr [r12 + 1640]
 mov rdx, qword ptr [r12 + 1664]
 mov rcx, qword ptr [r12 + 1672]
 mov r8d, 0
 lea r9, [r12 + 1600]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx44_3
.Lx44_2:
 mov rdi, qword ptr [r12 + 1632]
 mov rsi, qword ptr [r12 + 1640]
 mov rdx, qword ptr [r12 + 1664]
 mov rcx, qword ptr [r12 + 1672]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n46_α
 mov qword ptr [r12 + 1600], rax
 mov qword ptr [r12 + 1608], rdx
.Lx44_3:
 jmp xchain0_n45_α
 xchain0_n43_β:
 jmp xchain0_n46_α
# IR_ASSIGN gva
 xchain0_n44_α:
 mov rax, qword ptr [r12 + 1504]
 mov rdx, qword ptr [r12 + 1512]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 1488], rax
 mov qword ptr [r12 + 1496], rdx
 jmp main_γ
 xchain0_n44_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n45_α:
 mov qword ptr [r12 + 1696], 6
 mov rax, qword ptr [rip + .Lx46_0]
 mov qword ptr [r12 + 1704], rax
 jmp xchain0_n47_α
 xchain0_n45_β:
 jmp xchain0_n46_α
.Lx46_0:
 .quad 1
# IR_LIT_STRING
 xchain0_n46_α:
 mov qword ptr [r12 + 1872], 1
 mov rax, qword ptr [rip + .Lx47_0]
 mov qword ptr [r12 + 1880], rax
 jmp xchain0_n48_α
 xchain0_n46_β:
 jmp xchain0_n53_α
.Lx47_0:
 .quad .Lx47_0_s
.Lx47_0_s:
 .string "1"
 xchain0_n47_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1600] -> [zr+1552]
 mov rax, qword ptr [r12 + 1600]
 mov qword ptr [r12 + 1552], rax
 mov rax, qword ptr [r12 + 1608]
 mov qword ptr [r12 + 1560], rax
# marshal arg1 = producer-box slot [zr+1696] -> [zr+1568]
 mov rax, qword ptr [r12 + 1696]
 mov qword ptr [r12 + 1568], rax
 mov rax, qword ptr [r12 + 1704]
 mov qword ptr [r12 + 1576], rax
  .section .rodata
  .Lbynamefn48: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn48]
 lea rsi, [r12 + 1552]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1536], rax
 mov qword ptr [r12 + 1544], rdx
 cmp eax, 99
 je xchain0_n46_α
 jmp xchain0_n49_α
 xchain0_n47_β:
 jmp xchain0_n46_α
# IR_LIT_STRING
 xchain0_n48_α:
 mov qword ptr [r12 + 1904], 1
 mov rax, qword ptr [rip + .Lx49_0]
 mov qword ptr [r12 + 1912], rax
 jmp xchain0_n50_α
 xchain0_n48_β:
 jmp xchain0_n53_α
.Lx49_0:
 .quad .Lx49_0_s
.Lx49_0_s:
 .string "0"
# IR_LIT_STRING
 xchain0_n49_α:
 mov qword ptr [r12 + 1744], 1
 mov rax, qword ptr [rip + .Lx50_0]
 mov qword ptr [r12 + 1752], rax
 jmp xchain0_n51_α
 xchain0_n49_β:
 jmp main_γ
.Lx50_0:
 .quad .Lx50_0_s
.Lx50_0_s:
 .string "FAIL 410/007: int+neg-string"
 xchain0_n50_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 1872]
 cmp eax, 100
 je .Lx51_0
 mov eax, dword ptr [r12 + 1904]
 cmp eax, 100
 je .Lx51_0
 mov eax, dword ptr [r12 + 1872]
 cmp eax, 6
 jne .Lx51_2
 mov eax, dword ptr [r12 + 1904]
 cmp eax, 6
 jne .Lx51_2
.Lx51_1:
 mov rax, qword ptr [r12 + 1880]
 mov rcx, qword ptr [r12 + 1912]
 add rax, rcx
 mov qword ptr [r12 + 1840], 6
 mov qword ptr [r12 + 1848], rax
 jmp xchain0_n52_α
.Lx51_0:
 mov rdi, qword ptr [r12 + 1872]
 mov rsi, qword ptr [r12 + 1880]
 mov rdx, qword ptr [r12 + 1904]
 mov rcx, qword ptr [r12 + 1912]
 mov r8d, 0
 lea r9, [r12 + 1840]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx51_3
.Lx51_2:
 mov rdi, qword ptr [r12 + 1872]
 mov rsi, qword ptr [r12 + 1880]
 mov rdx, qword ptr [r12 + 1904]
 mov rcx, qword ptr [r12 + 1912]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n53_α
 mov qword ptr [r12 + 1840], rax
 mov qword ptr [r12 + 1848], rdx
.Lx51_3:
 jmp xchain0_n52_α
 xchain0_n50_β:
 jmp xchain0_n53_α
# IR_ASSIGN gva
 xchain0_n51_α:
 mov rax, qword ptr [r12 + 1744]
 mov rdx, qword ptr [r12 + 1752]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 1728], rax
 mov qword ptr [r12 + 1736], rdx
 jmp main_γ
 xchain0_n51_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n52_α:
 mov qword ptr [r12 + 1936], 6
 mov rax, qword ptr [rip + .Lx53_0]
 mov qword ptr [r12 + 1944], rax
 jmp xchain0_n54_α
 xchain0_n52_β:
 jmp xchain0_n53_α
.Lx53_0:
 .quad 1
# IR_LIT_INTEGER
 xchain0_n53_α:
 mov qword ptr [r12 + 2112], 6
 mov rax, qword ptr [rip + .Lx54_0]
 mov qword ptr [r12 + 2120], rax
 jmp xchain0_n55_α
 xchain0_n53_β:
 jmp xchain0_n60_α
.Lx54_0:
 .quad 5
 xchain0_n54_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1840] -> [zr+1792]
 mov rax, qword ptr [r12 + 1840]
 mov qword ptr [r12 + 1792], rax
 mov rax, qword ptr [r12 + 1848]
 mov qword ptr [r12 + 1800], rax
# marshal arg1 = producer-box slot [zr+1936] -> [zr+1808]
 mov rax, qword ptr [r12 + 1936]
 mov qword ptr [r12 + 1808], rax
 mov rax, qword ptr [r12 + 1944]
 mov qword ptr [r12 + 1816], rax
  .section .rodata
  .Lbynamefn55: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn55]
 lea rsi, [r12 + 1792]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1776], rax
 mov qword ptr [r12 + 1784], rdx
 cmp eax, 99
 je xchain0_n53_α
 jmp xchain0_n56_α
 xchain0_n54_β:
 jmp xchain0_n53_α
# IR_LIT_STRING
 xchain0_n55_α:
 mov qword ptr [r12 + 2144], 1
 mov rax, qword ptr [rip + .Lx56_0]
 mov qword ptr [r12 + 2152], rax
 jmp xchain0_n57_α
 xchain0_n55_β:
 jmp xchain0_n60_α
.Lx56_0:
 .quad .Lx56_0_s
.Lx56_0_s:
 .string ""
# IR_LIT_STRING
 xchain0_n56_α:
 mov qword ptr [r12 + 1984], 1
 mov rax, qword ptr [rip + .Lx57_0]
 mov qword ptr [r12 + 1992], rax
 jmp xchain0_n58_α
 xchain0_n56_β:
 jmp main_γ
.Lx57_0:
 .quad .Lx57_0_s
.Lx57_0_s:
 .string "FAIL 410/008: string+string coerce"
 xchain0_n57_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 2144]
 cmp eax, 100
 je .Lx58_0
 mov eax, dword ptr [r12 + 2144]
 cmp eax, 6
 jne .Lx58_2
.Lx58_1:
 mov rax, 5
 mov rcx, qword ptr [r12 + 2152]
 add rax, rcx
 mov qword ptr [r12 + 2080], 6
 mov qword ptr [r12 + 2088], rax
 jmp xchain0_n59_α
.Lx58_0:
 mov rdi, qword ptr [r12 + 2112]
 mov rsi, qword ptr [r12 + 2120]
 mov rdx, qword ptr [r12 + 2144]
 mov rcx, qword ptr [r12 + 2152]
 mov r8d, 0
 lea r9, [r12 + 2080]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx58_3
.Lx58_2:
 mov rdi, qword ptr [r12 + 2112]
 mov rsi, qword ptr [r12 + 2120]
 mov rdx, qword ptr [r12 + 2144]
 mov rcx, qword ptr [r12 + 2152]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n60_α
 mov qword ptr [r12 + 2080], rax
 mov qword ptr [r12 + 2088], rdx
.Lx58_3:
 jmp xchain0_n59_α
 xchain0_n57_β:
 jmp xchain0_n60_α
# IR_ASSIGN gva
 xchain0_n58_α:
 mov rax, qword ptr [r12 + 1984]
 mov rdx, qword ptr [r12 + 1992]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 1968], rax
 mov qword ptr [r12 + 1976], rdx
 jmp main_γ
 xchain0_n58_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n59_α:
 mov qword ptr [r12 + 2176], 6
 mov rax, qword ptr [rip + .Lx60_0]
 mov qword ptr [r12 + 2184], rax
 jmp xchain0_n61_α
 xchain0_n59_β:
 jmp xchain0_n60_α
.Lx60_0:
 .quad 5
# IR_LIT_STRING
 xchain0_n60_α:
 mov qword ptr [r12 + 2272], 1
 mov rax, qword ptr [rip + .Lx61_0]
 mov qword ptr [r12 + 2280], rax
 jmp xchain0_n62_α
 xchain0_n60_β:
 jmp main_γ
.Lx61_0:
 .quad .Lx61_0_s
.Lx61_0_s:
 .string "PASS 410_arith_int (9/9)"
 xchain0_n61_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2080] -> [zr+2032]
 mov rax, qword ptr [r12 + 2080]
 mov qword ptr [r12 + 2032], rax
 mov rax, qword ptr [r12 + 2088]
 mov qword ptr [r12 + 2040], rax
# marshal arg1 = producer-box slot [zr+2176] -> [zr+2048]
 mov rax, qword ptr [r12 + 2176]
 mov qword ptr [r12 + 2048], rax
 mov rax, qword ptr [r12 + 2184]
 mov qword ptr [r12 + 2056], rax
  .section .rodata
  .Lbynamefn62: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn62]
 lea rsi, [r12 + 2032]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2016], rax
 mov qword ptr [r12 + 2024], rdx
 cmp eax, 99
 je xchain0_n60_α
 jmp xchain0_n63_α
 xchain0_n61_β:
 jmp xchain0_n60_α
# IR_ASSIGN gva
 xchain0_n62_α:
 mov rax, qword ptr [r12 + 2272]
 mov rdx, qword ptr [r12 + 2280]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 2256], rax
 mov qword ptr [r12 + 2264], rdx
 jmp main_γ
 xchain0_n62_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n63_α:
 mov qword ptr [r12 + 2224], 1
 mov rax, qword ptr [rip + .Lx64_0]
 mov qword ptr [r12 + 2232], rax
 jmp xchain0_n64_α
 xchain0_n63_β:
 jmp main_γ
.Lx64_0:
 .quad .Lx64_0_s
.Lx64_0_s:
 .string "FAIL 410/009: null addend is zero"
# IR_ASSIGN gva
 xchain0_n64_α:
 mov rax, qword ptr [r12 + 2224]
 mov rdx, qword ptr [r12 + 2232]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 2208], rax
 mov qword ptr [r12 + 2216], rdx
 jmp main_γ
 xchain0_n64_β:
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [r12 + 2312]
pop r12
ret
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 2304]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, qword ptr [r12 + 2312]
pop r12
ret

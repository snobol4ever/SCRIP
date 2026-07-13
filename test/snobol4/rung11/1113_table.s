  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "t"
  .Lgvan1: .string "output"
  .Lgvan2: .string "ta"
  .Lgvan3: .string "ata"
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
  push rbp
  mov rbp, rsp
  push rdi
  push rsi
  call core_lib_init@PLT
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 4
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
  mov qword ptr [r12 + 2936], rsp
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 2928], rax
 pop rsi
main_α_body:
# IR_LIT_INTEGER
 xchain0_n0_α:
 mov qword ptr [r12 + 160], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 168], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n3_α
.Lx1_0:
 .quad 10
 xchain0_n1_α:
# BOX IR_CALL table(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+160] -> [zr+128]
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 128], rax
 mov rax, qword ptr [r12 + 168]
 mov qword ptr [r12 + 136], rax
  .section .rodata
  .Lrkfn3: .string "table"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn3]
 lea rsi, [r12 + 128]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n3_α
# IR_ASSIGN gva
 xchain0_n2_α:
 mov rax, qword ptr [r12 + 112]
 mov rdx, qword ptr [r12 + 120]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n3_α
# IR_VAR
 xchain0_n3_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n5_α
# IR_LIT_STRING
 xchain0_n4_α:
 mov qword ptr [r12 + 272], 1
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 280], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n5_α
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "cat"
# IR_VAR
 xchain0_n5_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 400], rax
 mov qword ptr [r12 + 408], rdx
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n8_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n6_α:
 mov rdi, qword ptr [r12 + 240]
 mov rsi, qword ptr [r12 + 248]
 mov rdx, qword ptr [r12 + 272]
 mov rcx, qword ptr [r12 + 280]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n5_α
 mov qword ptr [r12 + 304], rax
 mov qword ptr [r12 + 312], rdx
 jmp xchain0_n9_α
 xchain0_n6_β:
 jmp xchain0_n5_α
# IR_LIT_STRING
 xchain0_n7_α:
 mov qword ptr [r12 + 432], 1
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 440], rax
 jmp xchain0_n10_α
 xchain0_n7_β:
 jmp xchain0_n8_α
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "cat"
# IR_VAR
 xchain0_n8_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 608], rax
 mov qword ptr [r12 + 616], rdx
 jmp xchain0_n11_α
 xchain0_n8_β:
 jmp xchain0_n12_α
# IR_DEREF variable -> value
 xchain0_n9_α:
 mov rdi, qword ptr [r12 + 304]
 mov rsi, qword ptr [r12 + 312]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n5_α
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 jmp xchain0_n13_α
 xchain0_n9_β:
 jmp xchain0_n5_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n10_α:
 mov rdi, qword ptr [r12 + 400]
 mov rsi, qword ptr [r12 + 408]
 mov rdx, qword ptr [r12 + 432]
 mov rcx, qword ptr [r12 + 440]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n8_α
 mov qword ptr [r12 + 464], rax
 mov qword ptr [r12 + 472], rdx
 jmp xchain0_n14_α
 xchain0_n10_β:
 jmp xchain0_n8_α
# IR_LIT_STRING
 xchain0_n11_α:
 mov qword ptr [r12 + 640], 1
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [r12 + 648], rax
 jmp xchain0_n15_α
 xchain0_n11_β:
 jmp xchain0_n12_α
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "cat"
# IR_VAR
 xchain0_n12_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 800], rax
 mov qword ptr [r12 + 808], rdx
 jmp xchain0_n16_α
 xchain0_n12_β:
 jmp xchain0_n17_α
 xchain0_n13_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+336] -> [zr+208]
 mov rax, qword ptr [r12 + 336]
 mov qword ptr [r12 + 208], rax
 mov rax, qword ptr [r12 + 344]
 mov qword ptr [r12 + 216], rax
  .section .rodata
  .Lbynamefn14: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn14]
 lea rsi, [r12 + 208]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 cmp eax, 99
 je xchain0_n5_α
 jmp xchain0_n18_α
 xchain0_n13_β:
 jmp xchain0_n5_α
# IR_LIT_STRING
 xchain0_n14_α:
 mov qword ptr [r12 + 496], 1
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [r12 + 504], rax
 jmp xchain0_n19_α
 xchain0_n14_β:
 jmp xchain0_n8_α
.Lx16_0:
 .quad .Lx16_0_s
.Lx16_0_s:
 .string "dog"
# IR_SUBSCRIPT x[i] variable
 xchain0_n15_α:
 mov rdi, qword ptr [r12 + 608]
 mov rsi, qword ptr [r12 + 616]
 mov rdx, qword ptr [r12 + 640]
 mov rcx, qword ptr [r12 + 648]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n12_α
 mov qword ptr [r12 + 672], rax
 mov qword ptr [r12 + 680], rdx
 jmp xchain0_n20_α
 xchain0_n15_β:
 jmp xchain0_n12_α
# IR_LIT_INTEGER
 xchain0_n16_α:
 mov qword ptr [r12 + 832], 6
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [r12 + 840], rax
 jmp xchain0_n21_α
 xchain0_n16_β:
 jmp xchain0_n17_α
.Lx18_0:
 .quad 7
# IR_VAR
 xchain0_n17_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 1008], rax
 mov qword ptr [r12 + 1016], rdx
 jmp xchain0_n22_α
 xchain0_n17_β:
 jmp xchain0_n23_α
# IR_LIT_STRING
 xchain0_n18_α:
 mov qword ptr [r12 + 368], 1
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [r12 + 376], rax
 jmp xchain0_n24_α
 xchain0_n18_β:
 jmp main_γ
.Lx20_0:
 .quad .Lx20_0_s
.Lx20_0_s:
 .string "FAIL 1113/001: absent key is null"
# IR_ASSIGN_VAR
 xchain0_n19_α:
 mov rdi, qword ptr [r12 + 464]
 mov rsi, qword ptr [r12 + 472]
 mov rdx, qword ptr [r12 + 496]
 mov rcx, qword ptr [r12 + 504]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n8_α
 mov qword ptr [r12 + 528], rax
 mov qword ptr [r12 + 536], rdx
 jmp xchain0_n8_α
 xchain0_n19_β:
 jmp xchain0_n8_α
# IR_DEREF variable -> value
 xchain0_n20_α:
 mov rdi, qword ptr [r12 + 672]
 mov rsi, qword ptr [r12 + 680]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n12_α
 mov qword ptr [r12 + 704], rax
 mov qword ptr [r12 + 712], rdx
 jmp xchain0_n25_α
 xchain0_n20_β:
 jmp xchain0_n12_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n21_α:
 mov rdi, qword ptr [r12 + 800]
 mov rsi, qword ptr [r12 + 808]
 mov rdx, qword ptr [r12 + 832]
 mov rcx, qword ptr [r12 + 840]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n17_α
 mov qword ptr [r12 + 864], rax
 mov qword ptr [r12 + 872], rdx
 jmp xchain0_n26_α
 xchain0_n21_β:
 jmp xchain0_n17_α
# IR_LIT_INTEGER
 xchain0_n22_α:
 mov qword ptr [r12 + 1040], 6
 mov rax, qword ptr [rip + .Lx24_0]
 mov qword ptr [r12 + 1048], rax
 jmp xchain0_n27_α
 xchain0_n22_β:
 jmp xchain0_n23_α
.Lx24_0:
 .quad 7
# IR_VAR
 xchain0_n23_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 1264], rax
 mov qword ptr [r12 + 1272], rdx
 jmp xchain0_n28_α
 xchain0_n23_β:
 jmp xchain0_n29_α
# IR_ASSIGN gva
 xchain0_n24_α:
 mov rax, qword ptr [r12 + 368]
 mov rdx, qword ptr [r12 + 376]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 352], rax
 mov qword ptr [r12 + 360], rdx
 jmp main_γ
 xchain0_n24_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n25_α:
 mov qword ptr [r12 + 720], 1
 mov rax, qword ptr [rip + .Lx27_0]
 mov qword ptr [r12 + 728], rax
 jmp xchain0_n30_α
 xchain0_n25_β:
 jmp xchain0_n12_α
.Lx27_0:
 .quad .Lx27_0_s
.Lx27_0_s:
 .string "dog"
# IR_LIT_INTEGER
 xchain0_n26_α:
 mov qword ptr [r12 + 896], 6
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [r12 + 904], rax
 jmp xchain0_n31_α
 xchain0_n26_β:
 jmp xchain0_n17_α
.Lx28_0:
 .quad 45
# IR_SUBSCRIPT x[i] variable
 xchain0_n27_α:
 mov rdi, qword ptr [r12 + 1008]
 mov rsi, qword ptr [r12 + 1016]
 mov rdx, qword ptr [r12 + 1040]
 mov rcx, qword ptr [r12 + 1048]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n23_α
 mov qword ptr [r12 + 1072], rax
 mov qword ptr [r12 + 1080], rdx
 jmp xchain0_n32_α
 xchain0_n27_β:
 jmp xchain0_n23_α
# IR_LIT_STRING
 xchain0_n28_α:
 mov qword ptr [r12 + 1296], 1
 mov rax, qword ptr [rip + .Lx30_0]
 mov qword ptr [r12 + 1304], rax
 jmp xchain0_n33_α
 xchain0_n28_β:
 jmp xchain0_n29_α
.Lx30_0:
 .quad .Lx30_0_s
.Lx30_0_s:
 .string "cat"
# IR_VAR
 xchain0_n29_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 1536], rax
 mov qword ptr [r12 + 1544], rdx
 jmp xchain0_n34_α
 xchain0_n29_β:
 jmp xchain0_n35_α
 xchain0_n30_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+704] -> [zr+560]
 mov rax, qword ptr [r12 + 704]
 mov qword ptr [r12 + 560], rax
 mov rax, qword ptr [r12 + 712]
 mov qword ptr [r12 + 568], rax
# marshal arg1 = producer-box slot [zr+720] -> [zr+576]
 mov rax, qword ptr [r12 + 720]
 mov qword ptr [r12 + 576], rax
 mov rax, qword ptr [r12 + 728]
 mov qword ptr [r12 + 584], rax
  .section .rodata
  .Lbynamefn31: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn31]
 lea rsi, [r12 + 560]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 544], rax
 mov qword ptr [r12 + 552], rdx
 cmp eax, 99
 je xchain0_n12_α
 jmp xchain0_n36_α
 xchain0_n30_β:
 jmp xchain0_n12_α
# IR_ASSIGN_VAR
 xchain0_n31_α:
 mov rdi, qword ptr [r12 + 864]
 mov rsi, qword ptr [r12 + 872]
 mov rdx, qword ptr [r12 + 896]
 mov rcx, qword ptr [r12 + 904]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n17_α
 mov qword ptr [r12 + 928], rax
 mov qword ptr [r12 + 936], rdx
 jmp xchain0_n17_α
 xchain0_n31_β:
 jmp xchain0_n17_α
# IR_DEREF variable -> value
 xchain0_n32_α:
 mov rdi, qword ptr [r12 + 1072]
 mov rsi, qword ptr [r12 + 1080]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n23_α
 mov qword ptr [r12 + 1104], rax
 mov qword ptr [r12 + 1112], rdx
 jmp xchain0_n37_α
 xchain0_n32_β:
 jmp xchain0_n23_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n33_α:
 mov rdi, qword ptr [r12 + 1264]
 mov rsi, qword ptr [r12 + 1272]
 mov rdx, qword ptr [r12 + 1296]
 mov rcx, qword ptr [r12 + 1304]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n29_α
 mov qword ptr [r12 + 1328], rax
 mov qword ptr [r12 + 1336], rdx
 jmp xchain0_n38_α
 xchain0_n33_β:
 jmp xchain0_n29_α
# IR_LIT_STRING
 xchain0_n34_α:
 mov qword ptr [r12 + 1568], 1
 mov rax, qword ptr [rip + .Lx36_0]
 mov qword ptr [r12 + 1576], rax
 jmp xchain0_n39_α
 xchain0_n34_β:
 jmp xchain0_n35_α
.Lx36_0:
 .quad .Lx36_0_s
.Lx36_0_s:
 .string "array"
# IR_VAR
 xchain0_n35_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 1712], rax
 mov qword ptr [r12 + 1720], rdx
 jmp xchain0_n40_α
 xchain0_n35_β:
 jmp xchain0_n41_α
# IR_LIT_STRING
 xchain0_n36_α:
 mov qword ptr [r12 + 768], 1
 mov rax, qword ptr [rip + .Lx38_0]
 mov qword ptr [r12 + 776], rax
 jmp xchain0_n42_α
 xchain0_n36_β:
 jmp main_γ
.Lx38_0:
 .quad .Lx38_0_s
.Lx38_0_s:
 .string "FAIL 1113/002: string key assign/read"
# IR_LIT_INTEGER
 xchain0_n37_α:
 mov qword ptr [r12 + 1120], 6
 mov rax, qword ptr [rip + .Lx39_0]
 mov qword ptr [r12 + 1128], rax
 jmp xchain0_n43_α
 xchain0_n37_β:
 jmp xchain0_n23_α
.Lx39_0:
 .quad 45
# IR_DEREF variable -> value
 xchain0_n38_α:
 mov rdi, qword ptr [r12 + 1328]
 mov rsi, qword ptr [r12 + 1336]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n29_α
 mov qword ptr [r12 + 1360], rax
 mov qword ptr [r12 + 1368], rdx
 jmp xchain0_n44_α
 xchain0_n38_β:
 jmp xchain0_n29_α
 xchain0_n39_α:
# BOX CALL convert(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1536] -> [zr+1488]
 mov rax, qword ptr [r12 + 1536]
 mov qword ptr [r12 + 1488], rax
 mov rax, qword ptr [r12 + 1544]
 mov qword ptr [r12 + 1496], rax
# marshal arg1 = producer-box slot [zr+1568] -> [zr+1504]
 mov rax, qword ptr [r12 + 1568]
 mov qword ptr [r12 + 1504], rax
 mov rax, qword ptr [r12 + 1576]
 mov qword ptr [r12 + 1512], rax
  .section .rodata
  .Lbynamefn40: .string "convert"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn40]
 lea rsi, [r12 + 1488]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1472], rax
 mov qword ptr [r12 + 1480], rdx
 cmp eax, 99
 je xchain0_n35_α
 jmp xchain0_n45_α
 xchain0_n39_β:
 jmp xchain0_n35_α
 xchain0_n40_α:
# BOX CALL prototype(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1712] -> [zr+1680]
 mov rax, qword ptr [r12 + 1712]
 mov qword ptr [r12 + 1680], rax
 mov rax, qword ptr [r12 + 1720]
 mov qword ptr [r12 + 1688], rax
  .section .rodata
  .Lbynamefn41: .string "prototype"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn41]
 lea rsi, [r12 + 1680]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1664], rax
 mov qword ptr [r12 + 1672], rdx
 cmp eax, 99
 je xchain0_n41_α
 jmp xchain0_n46_α
 xchain0_n40_β:
 jmp xchain0_n41_α
# IR_VAR
 xchain0_n41_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 1904], rax
 mov qword ptr [r12 + 1912], rdx
 jmp xchain0_n47_α
 xchain0_n41_β:
 jmp xchain0_n48_α
# IR_ASSIGN gva
 xchain0_n42_α:
 mov rax, qword ptr [r12 + 768]
 mov rdx, qword ptr [r12 + 776]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 752], rax
 mov qword ptr [r12 + 760], rdx
 jmp main_γ
 xchain0_n42_β:
 jmp main_γ
 xchain0_n43_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1104] -> [zr+960]
 mov rax, qword ptr [r12 + 1104]
 mov qword ptr [r12 + 960], rax
 mov rax, qword ptr [r12 + 1112]
 mov qword ptr [r12 + 968], rax
# marshal arg1 = producer-box slot [zr+1120] -> [zr+976]
 mov rax, qword ptr [r12 + 1120]
 mov qword ptr [r12 + 976], rax
 mov rax, qword ptr [r12 + 1128]
 mov qword ptr [r12 + 984], rax
  .section .rodata
  .Lbynamefn44: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn44]
 lea rsi, [r12 + 960]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 944], rax
 mov qword ptr [r12 + 952], rdx
 cmp eax, 99
 je xchain0_n23_α
 jmp xchain0_n49_α
 xchain0_n43_β:
 jmp xchain0_n23_α
# IR_LIT_STRING
 xchain0_n44_α:
 mov qword ptr [r12 + 1376], 1
 mov rax, qword ptr [rip + .Lx46_0]
 mov qword ptr [r12 + 1384], rax
 jmp xchain0_n50_α
 xchain0_n44_β:
 jmp xchain0_n29_α
.Lx46_0:
 .quad .Lx46_0_s
.Lx46_0_s:
 .string "dog"
# IR_ASSIGN gva
 xchain0_n45_α:
 mov rax, qword ptr [r12 + 1472]
 mov rdx, qword ptr [r12 + 1480]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [r12 + 1456], rax
 mov qword ptr [r12 + 1464], rdx
 jmp xchain0_n35_α
 xchain0_n45_β:
 jmp xchain0_n35_α
# IR_LIT_STRING
 xchain0_n46_α:
 mov qword ptr [r12 + 1744], 1
 mov rax, qword ptr [rip + .Lx48_0]
 mov qword ptr [r12 + 1752], rax
 jmp xchain0_n51_α
 xchain0_n46_β:
 jmp xchain0_n41_α
.Lx48_0:
 .quad .Lx48_0_s
.Lx48_0_s:
 .string "2,2"
# IR_LIT_STRING
 xchain0_n47_α:
 mov qword ptr [r12 + 1936], 1
 mov rax, qword ptr [rip + .Lx49_0]
 mov qword ptr [r12 + 1944], rax
 jmp xchain0_n52_α
 xchain0_n47_β:
 jmp xchain0_n48_α
.Lx49_0:
 .quad .Lx49_0_s
.Lx49_0_s:
 .string "table"
# IR_VAR
 xchain0_n48_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 2032], rax
 mov qword ptr [r12 + 2040], rdx
 jmp xchain0_n53_α
 xchain0_n48_β:
 jmp xchain0_n54_α
# IR_LIT_STRING
 xchain0_n49_α:
 mov qword ptr [r12 + 1168], 1
 mov rax, qword ptr [rip + .Lx51_0]
 mov qword ptr [r12 + 1176], rax
 jmp xchain0_n55_α
 xchain0_n49_β:
 jmp main_γ
.Lx51_0:
 .quad .Lx51_0_s
.Lx51_0_s:
 .string "FAIL 1113/003: integer key assign/read"
 xchain0_n50_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1360] -> [zr+1216]
 mov rax, qword ptr [r12 + 1360]
 mov qword ptr [r12 + 1216], rax
 mov rax, qword ptr [r12 + 1368]
 mov qword ptr [r12 + 1224], rax
# marshal arg1 = producer-box slot [zr+1376] -> [zr+1232]
 mov rax, qword ptr [r12 + 1376]
 mov qword ptr [r12 + 1232], rax
 mov rax, qword ptr [r12 + 1384]
 mov qword ptr [r12 + 1240], rax
  .section .rodata
  .Lbynamefn51: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn51]
 lea rsi, [r12 + 1216]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1200], rax
 mov qword ptr [r12 + 1208], rdx
 cmp eax, 99
 je xchain0_n29_α
 jmp xchain0_n56_α
 xchain0_n50_β:
 jmp xchain0_n29_α
 xchain0_n51_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1664] -> [zr+1616]
 mov rax, qword ptr [r12 + 1664]
 mov qword ptr [r12 + 1616], rax
 mov rax, qword ptr [r12 + 1672]
 mov qword ptr [r12 + 1624], rax
# marshal arg1 = producer-box slot [zr+1744] -> [zr+1632]
 mov rax, qword ptr [r12 + 1744]
 mov qword ptr [r12 + 1632], rax
 mov rax, qword ptr [r12 + 1752]
 mov qword ptr [r12 + 1640], rax
  .section .rodata
  .Lbynamefn52: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn52]
 lea rsi, [r12 + 1616]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1600], rax
 mov qword ptr [r12 + 1608], rdx
 cmp eax, 99
 je xchain0_n41_α
 jmp xchain0_n57_α
 xchain0_n51_β:
 jmp xchain0_n41_α
 xchain0_n52_α:
# BOX CALL convert(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1904] -> [zr+1856]
 mov rax, qword ptr [r12 + 1904]
 mov qword ptr [r12 + 1856], rax
 mov rax, qword ptr [r12 + 1912]
 mov qword ptr [r12 + 1864], rax
# marshal arg1 = producer-box slot [zr+1936] -> [zr+1872]
 mov rax, qword ptr [r12 + 1936]
 mov qword ptr [r12 + 1872], rax
 mov rax, qword ptr [r12 + 1944]
 mov qword ptr [r12 + 1880], rax
  .section .rodata
  .Lbynamefn53: .string "convert"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn53]
 lea rsi, [r12 + 1856]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1840], rax
 mov qword ptr [r12 + 1848], rdx
 cmp eax, 99
 je xchain0_n48_α
 jmp xchain0_n58_α
 xchain0_n52_β:
 jmp xchain0_n48_α
# IR_LIT_INTEGER
 xchain0_n53_α:
 mov qword ptr [r12 + 2064], 6
 mov rax, qword ptr [rip + .Lx55_0]
 mov qword ptr [r12 + 2072], rax
 jmp xchain0_n59_α
 xchain0_n53_β:
 jmp xchain0_n54_α
.Lx55_0:
 .quad 7
# IR_VAR
 xchain0_n54_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 2288], rax
 mov qword ptr [r12 + 2296], rdx
 jmp xchain0_n60_α
 xchain0_n54_β:
 jmp xchain0_n61_α
# IR_ASSIGN gva
 xchain0_n55_α:
 mov rax, qword ptr [r12 + 1168]
 mov rdx, qword ptr [r12 + 1176]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1152], rax
 mov qword ptr [r12 + 1160], rdx
 jmp main_γ
 xchain0_n55_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n56_α:
 mov qword ptr [r12 + 1424], 1
 mov rax, qword ptr [rip + .Lx58_0]
 mov qword ptr [r12 + 1432], rax
 jmp xchain0_n62_α
 xchain0_n56_β:
 jmp main_γ
.Lx58_0:
 .quad .Lx58_0_s
.Lx58_0_s:
 .string "FAIL 1113/004: string key survives int key add"
# IR_LIT_STRING
 xchain0_n57_α:
 mov qword ptr [r12 + 1792], 1
 mov rax, qword ptr [rip + .Lx59_0]
 mov qword ptr [r12 + 1800], rax
 jmp xchain0_n63_α
 xchain0_n57_β:
 jmp main_γ
.Lx59_0:
 .quad .Lx59_0_s
.Lx59_0_s:
 .string "FAIL 1113/005: table->array prototype 2,2"
# IR_ASSIGN gva
 xchain0_n58_α:
 mov rax, qword ptr [r12 + 1840]
 mov rdx, qword ptr [r12 + 1848]
 mov qword ptr [rbx + 48], rax
 mov qword ptr [rbx + 56], rdx
 mov qword ptr [r12 + 1824], rax
 mov qword ptr [r12 + 1832], rdx
 jmp xchain0_n48_α
 xchain0_n58_β:
 jmp xchain0_n48_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n59_α:
 mov rdi, qword ptr [r12 + 2032]
 mov rsi, qword ptr [r12 + 2040]
 mov rdx, qword ptr [r12 + 2064]
 mov rcx, qword ptr [r12 + 2072]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n54_α
 mov qword ptr [r12 + 2096], rax
 mov qword ptr [r12 + 2104], rdx
 jmp xchain0_n64_α
 xchain0_n59_β:
 jmp xchain0_n54_α
# IR_LIT_STRING
 xchain0_n60_α:
 mov qword ptr [r12 + 2320], 1
 mov rax, qword ptr [rip + .Lx62_0]
 mov qword ptr [r12 + 2328], rax
 jmp xchain0_n65_α
 xchain0_n60_β:
 jmp xchain0_n61_α
.Lx62_0:
 .quad .Lx62_0_s
.Lx62_0_s:
 .string "cat"
# IR_VAR
 xchain0_n61_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 2480], rax
 mov qword ptr [r12 + 2488], rdx
 jmp xchain0_n66_α
 xchain0_n61_β:
 jmp xchain0_n67_α
# IR_ASSIGN gva
 xchain0_n62_α:
 mov rax, qword ptr [r12 + 1424]
 mov rdx, qword ptr [r12 + 1432]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1408], rax
 mov qword ptr [r12 + 1416], rdx
 jmp main_γ
 xchain0_n62_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain0_n63_α:
 mov rax, qword ptr [r12 + 1792]
 mov rdx, qword ptr [r12 + 1800]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1776], rax
 mov qword ptr [r12 + 1784], rdx
 jmp main_γ
 xchain0_n63_β:
 jmp main_γ
# IR_DEREF variable -> value
 xchain0_n64_α:
 mov rdi, qword ptr [r12 + 2096]
 mov rsi, qword ptr [r12 + 2104]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n54_α
 mov qword ptr [r12 + 2128], rax
 mov qword ptr [r12 + 2136], rdx
 jmp xchain0_n68_α
 xchain0_n64_β:
 jmp xchain0_n54_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n65_α:
 mov rdi, qword ptr [r12 + 2288]
 mov rsi, qword ptr [r12 + 2296]
 mov rdx, qword ptr [r12 + 2320]
 mov rcx, qword ptr [r12 + 2328]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n61_α
 mov qword ptr [r12 + 2352], rax
 mov qword ptr [r12 + 2360], rdx
 jmp xchain0_n69_α
 xchain0_n65_β:
 jmp xchain0_n61_α
# IR_LIT_STRING
 xchain0_n66_α:
 mov qword ptr [r12 + 2512], 1
 mov rax, qword ptr [rip + .Lx68_0]
 mov qword ptr [r12 + 2520], rax
 jmp xchain0_n70_α
 xchain0_n66_β:
 jmp xchain0_n67_α
.Lx68_0:
 .quad .Lx68_0_s
.Lx68_0_s:
 .string "cat"
# IR_VAR
 xchain0_n67_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 2688], rax
 mov qword ptr [r12 + 2696], rdx
 jmp xchain0_n71_α
 xchain0_n67_β:
 jmp xchain0_n72_α
# IR_LIT_INTEGER
 xchain0_n68_α:
 mov qword ptr [r12 + 2144], 6
 mov rax, qword ptr [rip + .Lx70_0]
 mov qword ptr [r12 + 2152], rax
 jmp xchain0_n73_α
 xchain0_n68_β:
 jmp xchain0_n54_α
.Lx70_0:
 .quad 45
# IR_DEREF variable -> value
 xchain0_n69_α:
 mov rdi, qword ptr [r12 + 2352]
 mov rsi, qword ptr [r12 + 2360]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n61_α
 mov qword ptr [r12 + 2384], rax
 mov qword ptr [r12 + 2392], rdx
 jmp xchain0_n74_α
 xchain0_n69_β:
 jmp xchain0_n61_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n70_α:
 mov rdi, qword ptr [r12 + 2480]
 mov rsi, qword ptr [r12 + 2488]
 mov rdx, qword ptr [r12 + 2512]
 mov rcx, qword ptr [r12 + 2520]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n67_α
 mov qword ptr [r12 + 2544], rax
 mov qword ptr [r12 + 2552], rdx
 jmp xchain0_n75_α
 xchain0_n70_β:
 jmp xchain0_n67_α
# IR_LIT_STRING
 xchain0_n71_α:
 mov qword ptr [r12 + 2720], 1
 mov rax, qword ptr [rip + .Lx73_0]
 mov qword ptr [r12 + 2728], rax
 jmp xchain0_n76_α
 xchain0_n71_β:
 jmp xchain0_n72_α
.Lx73_0:
 .quad .Lx73_0_s
.Lx73_0_s:
 .string "cat"
# IR_LIT_STRING
 xchain0_n72_α:
 mov qword ptr [r12 + 2896], 1
 mov rax, qword ptr [rip + .Lx74_0]
 mov qword ptr [r12 + 2904], rax
 jmp xchain0_n77_α
 xchain0_n72_β:
 jmp main_γ
.Lx74_0:
 .quad .Lx74_0_s
.Lx74_0_s:
 .string "PASS 1113_table (8/8)"
 xchain0_n73_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2128] -> [zr+1984]
 mov rax, qword ptr [r12 + 2128]
 mov qword ptr [r12 + 1984], rax
 mov rax, qword ptr [r12 + 2136]
 mov qword ptr [r12 + 1992], rax
# marshal arg1 = producer-box slot [zr+2144] -> [zr+2000]
 mov rax, qword ptr [r12 + 2144]
 mov qword ptr [r12 + 2000], rax
 mov rax, qword ptr [r12 + 2152]
 mov qword ptr [r12 + 2008], rax
  .section .rodata
  .Lbynamefn74: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn74]
 lea rsi, [r12 + 1984]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1968], rax
 mov qword ptr [r12 + 1976], rdx
 cmp eax, 99
 je xchain0_n54_α
 jmp xchain0_n78_α
 xchain0_n73_β:
 jmp xchain0_n54_α
# IR_LIT_STRING
 xchain0_n74_α:
 mov qword ptr [r12 + 2400], 1
 mov rax, qword ptr [rip + .Lx76_0]
 mov qword ptr [r12 + 2408], rax
 jmp xchain0_n79_α
 xchain0_n74_β:
 jmp xchain0_n61_α
.Lx76_0:
 .quad .Lx76_0_s
.Lx76_0_s:
 .string "dog"
# IR_LIT_STRING
 xchain0_n75_α:
 mov qword ptr [r12 + 2576], 1
 mov rax, qword ptr [rip + .Lx77_0]
 mov qword ptr [r12 + 2584], rax
 jmp xchain0_n80_α
 xchain0_n75_β:
 jmp xchain0_n67_α
.Lx77_0:
 .quad .Lx77_0_s
.Lx77_0_s:
 .string "fish"
# IR_SUBSCRIPT x[i] variable
 xchain0_n76_α:
 mov rdi, qword ptr [r12 + 2688]
 mov rsi, qword ptr [r12 + 2696]
 mov rdx, qword ptr [r12 + 2720]
 mov rcx, qword ptr [r12 + 2728]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n72_α
 mov qword ptr [r12 + 2752], rax
 mov qword ptr [r12 + 2760], rdx
 jmp xchain0_n81_α
 xchain0_n76_β:
 jmp xchain0_n72_α
# IR_ASSIGN gva
 xchain0_n77_α:
 mov rax, qword ptr [r12 + 2896]
 mov rdx, qword ptr [r12 + 2904]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 2880], rax
 mov qword ptr [r12 + 2888], rdx
 jmp main_γ
 xchain0_n77_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n78_α:
 mov qword ptr [r12 + 2192], 1
 mov rax, qword ptr [rip + .Lx80_0]
 mov qword ptr [r12 + 2200], rax
 jmp xchain0_n82_α
 xchain0_n78_β:
 jmp main_γ
.Lx80_0:
 .quad .Lx80_0_s
.Lx80_0_s:
 .string "FAIL 1113/006: array->table int key roundtrip"
 xchain0_n79_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2384] -> [zr+2240]
 mov rax, qword ptr [r12 + 2384]
 mov qword ptr [r12 + 2240], rax
 mov rax, qword ptr [r12 + 2392]
 mov qword ptr [r12 + 2248], rax
# marshal arg1 = producer-box slot [zr+2400] -> [zr+2256]
 mov rax, qword ptr [r12 + 2400]
 mov qword ptr [r12 + 2256], rax
 mov rax, qword ptr [r12 + 2408]
 mov qword ptr [r12 + 2264], rax
  .section .rodata
  .Lbynamefn80: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn80]
 lea rsi, [r12 + 2240]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2224], rax
 mov qword ptr [r12 + 2232], rdx
 cmp eax, 99
 je xchain0_n61_α
 jmp xchain0_n83_α
 xchain0_n79_β:
 jmp xchain0_n61_α
# IR_ASSIGN_VAR
 xchain0_n80_α:
 mov rdi, qword ptr [r12 + 2544]
 mov rsi, qword ptr [r12 + 2552]
 mov rdx, qword ptr [r12 + 2576]
 mov rcx, qword ptr [r12 + 2584]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n67_α
 mov qword ptr [r12 + 2608], rax
 mov qword ptr [r12 + 2616], rdx
 jmp xchain0_n67_α
 xchain0_n80_β:
 jmp xchain0_n67_α
# IR_DEREF variable -> value
 xchain0_n81_α:
 mov rdi, qword ptr [r12 + 2752]
 mov rsi, qword ptr [r12 + 2760]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n72_α
 mov qword ptr [r12 + 2784], rax
 mov qword ptr [r12 + 2792], rdx
 jmp xchain0_n84_α
 xchain0_n81_β:
 jmp xchain0_n72_α
# IR_ASSIGN gva
 xchain0_n82_α:
 mov rax, qword ptr [r12 + 2192]
 mov rdx, qword ptr [r12 + 2200]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 2176], rax
 mov qword ptr [r12 + 2184], rdx
 jmp main_γ
 xchain0_n82_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n83_α:
 mov qword ptr [r12 + 2448], 1
 mov rax, qword ptr [rip + .Lx85_0]
 mov qword ptr [r12 + 2456], rax
 jmp xchain0_n85_α
 xchain0_n83_β:
 jmp main_γ
.Lx85_0:
 .quad .Lx85_0_s
.Lx85_0_s:
 .string "FAIL 1113/007: array->table string key roundtrip"
# IR_LIT_STRING
 xchain0_n84_α:
 mov qword ptr [r12 + 2800], 1
 mov rax, qword ptr [rip + .Lx86_0]
 mov qword ptr [r12 + 2808], rax
 jmp xchain0_n86_α
 xchain0_n84_β:
 jmp xchain0_n72_α
.Lx86_0:
 .quad .Lx86_0_s
.Lx86_0_s:
 .string "fish"
# IR_ASSIGN gva
 xchain0_n85_α:
 mov rax, qword ptr [r12 + 2448]
 mov rdx, qword ptr [r12 + 2456]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 2432], rax
 mov qword ptr [r12 + 2440], rdx
 jmp main_γ
 xchain0_n85_β:
 jmp main_γ
 xchain0_n86_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2784] -> [zr+2640]
 mov rax, qword ptr [r12 + 2784]
 mov qword ptr [r12 + 2640], rax
 mov rax, qword ptr [r12 + 2792]
 mov qword ptr [r12 + 2648], rax
# marshal arg1 = producer-box slot [zr+2800] -> [zr+2656]
 mov rax, qword ptr [r12 + 2800]
 mov qword ptr [r12 + 2656], rax
 mov rax, qword ptr [r12 + 2808]
 mov qword ptr [r12 + 2664], rax
  .section .rodata
  .Lbynamefn87: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn87]
 lea rsi, [r12 + 2640]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2624], rax
 mov qword ptr [r12 + 2632], rdx
 cmp eax, 99
 je xchain0_n72_α
 jmp xchain0_n87_α
 xchain0_n86_β:
 jmp xchain0_n72_α
# IR_LIT_STRING
 xchain0_n87_α:
 mov qword ptr [r12 + 2848], 1
 mov rax, qword ptr [rip + .Lx89_0]
 mov qword ptr [r12 + 2856], rax
 jmp xchain0_n88_α
 xchain0_n87_β:
 jmp main_γ
.Lx89_0:
 .quad .Lx89_0_s
.Lx89_0_s:
 .string "FAIL 1113/008: [] and <> syntax equivalent"
# IR_ASSIGN gva
 xchain0_n88_α:
 mov rax, qword ptr [r12 + 2848]
 mov rdx, qword ptr [r12 + 2856]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 2832], rax
 mov qword ptr [r12 + 2840], rdx
 jmp main_γ
 xchain0_n88_β:
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [r12 + 2936]
pop r12
ret
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 2928]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, qword ptr [r12 + 2936]
pop r12
ret

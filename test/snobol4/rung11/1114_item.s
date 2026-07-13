  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "aaa"
  .Lgvan1: .string "output"
  .Lgvan2: .string "ama"
  .Lgvan3: .string "tt"
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
  mov qword ptr [r12 + 4136], rsp
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 4128], rax
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
# BOX CALL array(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+160] -> [zr+128]
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 128], rax
 mov rax, qword ptr [r12 + 168]
 mov qword ptr [r12 + 136], rax
  .section .rodata
  .Lbynamefn2: .string "array"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn2]
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
# IR_LIT_STRING
 xchain0_n3_α:
 mov qword ptr [r12 + 192], 1
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [r12 + 200], rax
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n6_α
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string ""
 xchain0_n4_α:
# BOX IR_CALL SNO$WANTNM(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+192] -> [zr+240]
 mov rax, qword ptr [r12 + 192]
 mov qword ptr [r12 + 240], rax
 mov rax, qword ptr [r12 + 200]
 mov qword ptr [r12 + 248], rax
  .section .rodata
  .Lrkfn6: .string "SNO$WANTNM"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn6]
 lea rsi, [r12 + 240]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 cmp eax, 99
 je xchain0_n6_α
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n6_α
# IR_VAR
 xchain0_n5_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n6_α
# IR_VAR
 xchain0_n6_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 576], rax
 mov qword ptr [r12 + 584], rdx
 jmp xchain0_n8_α
 xchain0_n6_β:
 jmp xchain0_n9_α
# IR_LIT_INTEGER
 xchain0_n7_α:
 mov qword ptr [r12 + 368], 6
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 376], rax
 jmp xchain0_n10_α
 xchain0_n7_β:
 jmp xchain0_n6_α
.Lx9_0:
 .quad 1
# IR_LIT_INTEGER
 xchain0_n8_α:
 mov qword ptr [r12 + 608], 6
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [r12 + 616], rax
 jmp xchain0_n11_α
 xchain0_n8_β:
 jmp xchain0_n9_α
.Lx10_0:
 .quad 1
# IR_VAR
 xchain0_n9_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 784], rax
 mov qword ptr [r12 + 792], rdx
 jmp xchain0_n12_α
 xchain0_n9_β:
 jmp xchain0_n13_α
 xchain0_n10_α:
# BOX CALL item(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+336] -> [zr+288]
 mov rax, qword ptr [r12 + 336]
 mov qword ptr [r12 + 288], rax
 mov rax, qword ptr [r12 + 344]
 mov qword ptr [r12 + 296], rax
# marshal arg1 = producer-box slot [zr+368] -> [zr+304]
 mov rax, qword ptr [r12 + 368]
 mov qword ptr [r12 + 304], rax
 mov rax, qword ptr [r12 + 376]
 mov qword ptr [r12 + 312], rax
  .section .rodata
  .Lbynamefn11: .string "item"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn11]
 lea rsi, [r12 + 288]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 cmp eax, 99
 je xchain0_n6_α
 jmp xchain0_n14_α
 xchain0_n10_β:
 jmp xchain0_n6_α
 xchain0_n11_α:
# BOX CALL item(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+576] -> [zr+528]
 mov rax, qword ptr [r12 + 576]
 mov qword ptr [r12 + 528], rax
 mov rax, qword ptr [r12 + 584]
 mov qword ptr [r12 + 536], rax
# marshal arg1 = producer-box slot [zr+608] -> [zr+544]
 mov rax, qword ptr [r12 + 608]
 mov qword ptr [r12 + 544], rax
 mov rax, qword ptr [r12 + 616]
 mov qword ptr [r12 + 552], rax
  .section .rodata
  .Lbynamefn12: .string "item"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn12]
 lea rsi, [r12 + 528]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 512], rax
 mov qword ptr [r12 + 520], rdx
 cmp eax, 99
 je xchain0_n9_α
 jmp xchain0_n15_α
 xchain0_n11_β:
 jmp xchain0_n9_α
# IR_LIT_INTEGER
 xchain0_n12_α:
 mov qword ptr [r12 + 816], 6
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [r12 + 824], rax
 jmp xchain0_n16_α
 xchain0_n12_β:
 jmp xchain0_n13_α
.Lx14_0:
 .quad 1
# IR_VAR
 xchain0_n13_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 976], rax
 mov qword ptr [r12 + 984], rdx
 jmp xchain0_n17_α
 xchain0_n13_β:
 jmp xchain0_n18_α
# IR_LIT_INTEGER
 xchain0_n14_α:
 mov qword ptr [r12 + 400], 6
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [r12 + 408], rax
 jmp xchain0_n19_α
 xchain0_n14_β:
 jmp xchain0_n6_α
.Lx16_0:
 .quad 5
# IR_LIT_INTEGER
 xchain0_n15_α:
 mov qword ptr [r12 + 640], 6
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [r12 + 648], rax
 jmp xchain0_n20_α
 xchain0_n15_β:
 jmp xchain0_n9_α
.Lx17_0:
 .quad 5
# IR_SUBSCRIPT x[i] variable
 xchain0_n16_α:
 mov rdi, qword ptr [r12 + 784]
 mov rsi, qword ptr [r12 + 792]
 mov rdx, qword ptr [r12 + 816]
 mov rcx, qword ptr [r12 + 824]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n13_α
 mov qword ptr [r12 + 848], rax
 mov qword ptr [r12 + 856], rdx
 jmp xchain0_n21_α
 xchain0_n16_β:
 jmp xchain0_n13_α
# IR_LIT_INTEGER
 xchain0_n17_α:
 mov qword ptr [r12 + 1008], 6
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [r12 + 1016], rax
 jmp xchain0_n22_α
 xchain0_n17_β:
 jmp xchain0_n18_α
.Lx19_0:
 .quad 2
# IR_VAR
 xchain0_n18_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 1248], rax
 mov qword ptr [r12 + 1256], rdx
 jmp xchain0_n23_α
 xchain0_n18_β:
 jmp xchain0_n24_α
# IR_ASSIGN_VAR
 xchain0_n19_α:
 mov rdi, qword ptr [r12 + 272]
 mov rsi, qword ptr [r12 + 280]
 mov rdx, qword ptr [r12 + 400]
 mov rcx, qword ptr [r12 + 408]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n6_α
 mov qword ptr [r12 + 432], rax
 mov qword ptr [r12 + 440], rdx
 jmp xchain0_n6_α
 xchain0_n19_β:
 jmp xchain0_n6_α
 xchain0_n20_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+512] -> [zr+464]
 mov rax, qword ptr [r12 + 512]
 mov qword ptr [r12 + 464], rax
 mov rax, qword ptr [r12 + 520]
 mov qword ptr [r12 + 472], rax
# marshal arg1 = producer-box slot [zr+640] -> [zr+480]
 mov rax, qword ptr [r12 + 640]
 mov qword ptr [r12 + 480], rax
 mov rax, qword ptr [r12 + 648]
 mov qword ptr [r12 + 488], rax
  .section .rodata
  .Lbynamefn21: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn21]
 lea rsi, [r12 + 464]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 448], rax
 mov qword ptr [r12 + 456], rdx
 cmp eax, 99
 je xchain0_n9_α
 jmp xchain0_n25_α
 xchain0_n20_β:
 jmp xchain0_n9_α
# IR_DEREF variable -> value
 xchain0_n21_α:
 mov rdi, qword ptr [r12 + 848]
 mov rsi, qword ptr [r12 + 856]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n13_α
 mov qword ptr [r12 + 880], rax
 mov qword ptr [r12 + 888], rdx
 jmp xchain0_n26_α
 xchain0_n21_β:
 jmp xchain0_n13_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n22_α:
 mov rdi, qword ptr [r12 + 976]
 mov rsi, qword ptr [r12 + 984]
 mov rdx, qword ptr [r12 + 1008]
 mov rcx, qword ptr [r12 + 1016]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n18_α
 mov qword ptr [r12 + 1040], rax
 mov qword ptr [r12 + 1048], rdx
 jmp xchain0_n27_α
 xchain0_n22_β:
 jmp xchain0_n18_α
# IR_LIT_INTEGER
 xchain0_n23_α:
 mov qword ptr [r12 + 1280], 6
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [r12 + 1288], rax
 jmp xchain0_n28_α
 xchain0_n23_β:
 jmp xchain0_n24_α
.Lx25_0:
 .quad 2
# IR_LIT_STRING
 xchain0_n24_α:
 mov qword ptr [r12 + 1456], 1
 mov rax, qword ptr [rip + .Lx26_0]
 mov qword ptr [r12 + 1464], rax
 jmp xchain0_n29_α
 xchain0_n24_β:
 jmp xchain0_n35_α
.Lx26_0:
 .quad .Lx26_0_s
.Lx26_0_s:
 .string "2,2,2,2"
# IR_LIT_STRING
 xchain0_n25_α:
 mov qword ptr [r12 + 688], 1
 mov rax, qword ptr [rip + .Lx27_0]
 mov qword ptr [r12 + 696], rax
 jmp xchain0_n30_α
 xchain0_n25_β:
 jmp main_γ
.Lx27_0:
 .quad .Lx27_0_s
.Lx27_0_s:
 .string "FAIL 1114/001: item 1D assign/read"
# IR_LIT_INTEGER
 xchain0_n26_α:
 mov qword ptr [r12 + 896], 6
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [r12 + 904], rax
 jmp xchain0_n31_α
 xchain0_n26_β:
 jmp xchain0_n13_α
.Lx28_0:
 .quad 5
# IR_LIT_INTEGER
 xchain0_n27_α:
 mov qword ptr [r12 + 1072], 6
 mov rax, qword ptr [rip + .Lx29_0]
 mov qword ptr [r12 + 1080], rax
 jmp xchain0_n32_α
 xchain0_n27_β:
 jmp xchain0_n18_α
.Lx29_0:
 .quad 22
 xchain0_n28_α:
# BOX CALL item(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1248] -> [zr+1200]
 mov rax, qword ptr [r12 + 1248]
 mov qword ptr [r12 + 1200], rax
 mov rax, qword ptr [r12 + 1256]
 mov qword ptr [r12 + 1208], rax
# marshal arg1 = producer-box slot [zr+1280] -> [zr+1216]
 mov rax, qword ptr [r12 + 1280]
 mov qword ptr [r12 + 1216], rax
 mov rax, qword ptr [r12 + 1288]
 mov qword ptr [r12 + 1224], rax
  .section .rodata
  .Lbynamefn29: .string "item"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn29]
 lea rsi, [r12 + 1200]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1184], rax
 mov qword ptr [r12 + 1192], rdx
 cmp eax, 99
 je xchain0_n24_α
 jmp xchain0_n33_α
 xchain0_n28_β:
 jmp xchain0_n24_α
 xchain0_n29_α:
# BOX CALL array(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1456] -> [zr+1424]
 mov rax, qword ptr [r12 + 1456]
 mov qword ptr [r12 + 1424], rax
 mov rax, qword ptr [r12 + 1464]
 mov qword ptr [r12 + 1432], rax
  .section .rodata
  .Lbynamefn30: .string "array"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn30]
 lea rsi, [r12 + 1424]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1408], rax
 mov qword ptr [r12 + 1416], rdx
 cmp eax, 99
 je xchain0_n35_α
 jmp xchain0_n34_α
 xchain0_n29_β:
 jmp xchain0_n35_α
# IR_ASSIGN gva
 xchain0_n30_α:
 mov rax, qword ptr [r12 + 688]
 mov rdx, qword ptr [r12 + 696]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 672], rax
 mov qword ptr [r12 + 680], rdx
 jmp main_γ
 xchain0_n30_β:
 jmp main_γ
 xchain0_n31_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+880] -> [zr+736]
 mov rax, qword ptr [r12 + 880]
 mov qword ptr [r12 + 736], rax
 mov rax, qword ptr [r12 + 888]
 mov qword ptr [r12 + 744], rax
# marshal arg1 = producer-box slot [zr+896] -> [zr+752]
 mov rax, qword ptr [r12 + 896]
 mov qword ptr [r12 + 752], rax
 mov rax, qword ptr [r12 + 904]
 mov qword ptr [r12 + 760], rax
  .section .rodata
  .Lbynamefn32: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn32]
 lea rsi, [r12 + 736]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 720], rax
 mov qword ptr [r12 + 728], rdx
 cmp eax, 99
 je xchain0_n13_α
 jmp xchain0_n36_α
 xchain0_n31_β:
 jmp xchain0_n13_α
# IR_ASSIGN_VAR
 xchain0_n32_α:
 mov rdi, qword ptr [r12 + 1040]
 mov rsi, qword ptr [r12 + 1048]
 mov rdx, qword ptr [r12 + 1072]
 mov rcx, qword ptr [r12 + 1080]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n18_α
 mov qword ptr [r12 + 1104], rax
 mov qword ptr [r12 + 1112], rdx
 jmp xchain0_n18_α
 xchain0_n32_β:
 jmp xchain0_n18_α
# IR_LIT_INTEGER
 xchain0_n33_α:
 mov qword ptr [r12 + 1312], 6
 mov rax, qword ptr [rip + .Lx35_0]
 mov qword ptr [r12 + 1320], rax
 jmp xchain0_n37_α
 xchain0_n33_β:
 jmp xchain0_n24_α
.Lx35_0:
 .quad 22
# IR_ASSIGN gva
 xchain0_n34_α:
 mov rax, qword ptr [r12 + 1408]
 mov rdx, qword ptr [r12 + 1416]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [r12 + 1392], rax
 mov qword ptr [r12 + 1400], rdx
 jmp xchain0_n35_α
 xchain0_n34_β:
 jmp xchain0_n35_α
# IR_LIT_STRING
 xchain0_n35_α:
 mov qword ptr [r12 + 1488], 1
 mov rax, qword ptr [rip + .Lx37_0]
 mov qword ptr [r12 + 1496], rax
 jmp xchain0_n38_α
 xchain0_n35_β:
 jmp xchain0_n42_α
.Lx37_0:
 .quad .Lx37_0_s
.Lx37_0_s:
 .string ""
# IR_LIT_STRING
 xchain0_n36_α:
 mov qword ptr [r12 + 944], 1
 mov rax, qword ptr [rip + .Lx38_0]
 mov qword ptr [r12 + 952], rax
 jmp xchain0_n39_α
 xchain0_n36_β:
 jmp main_γ
.Lx38_0:
 .quad .Lx38_0_s
.Lx38_0_s:
 .string "FAIL 1114/002: item == bracket read"
 xchain0_n37_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1184] -> [zr+1136]
 mov rax, qword ptr [r12 + 1184]
 mov qword ptr [r12 + 1136], rax
 mov rax, qword ptr [r12 + 1192]
 mov qword ptr [r12 + 1144], rax
# marshal arg1 = producer-box slot [zr+1312] -> [zr+1152]
 mov rax, qword ptr [r12 + 1312]
 mov qword ptr [r12 + 1152], rax
 mov rax, qword ptr [r12 + 1320]
 mov qword ptr [r12 + 1160], rax
  .section .rodata
  .Lbynamefn38: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn38]
 lea rsi, [r12 + 1136]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1120], rax
 mov qword ptr [r12 + 1128], rdx
 cmp eax, 99
 je xchain0_n24_α
 jmp xchain0_n40_α
 xchain0_n37_β:
 jmp xchain0_n24_α
 xchain0_n38_α:
# BOX IR_CALL SNO$WANTNM(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1488] -> [zr+1536]
 mov rax, qword ptr [r12 + 1488]
 mov qword ptr [r12 + 1536], rax
 mov rax, qword ptr [r12 + 1496]
 mov qword ptr [r12 + 1544], rax
  .section .rodata
  .Lrkfn41: .string "SNO$WANTNM"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn41]
 lea rsi, [r12 + 1536]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1520], rax
 mov qword ptr [r12 + 1528], rdx
 cmp eax, 99
 je xchain0_n42_α
 jmp xchain0_n41_α
 xchain0_n38_β:
 jmp xchain0_n42_α
# IR_ASSIGN gva
 xchain0_n39_α:
 mov rax, qword ptr [r12 + 944]
 mov rdx, qword ptr [r12 + 952]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 928], rax
 mov qword ptr [r12 + 936], rdx
 jmp main_γ
 xchain0_n39_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n40_α:
 mov qword ptr [r12 + 1360], 1
 mov rax, qword ptr [rip + .Lx43_0]
 mov qword ptr [r12 + 1368], rax
 jmp xchain0_n43_α
 xchain0_n40_β:
 jmp main_γ
.Lx43_0:
 .quad .Lx43_0_s
.Lx43_0_s:
 .string "FAIL 1114/003: bracket assign, item read"
# IR_VAR
 xchain0_n41_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 1680], rax
 mov qword ptr [r12 + 1688], rdx
 jmp xchain0_n44_α
 xchain0_n41_β:
 jmp xchain0_n42_α
# IR_VAR
 xchain0_n42_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 2064], rax
 mov qword ptr [r12 + 2072], rdx
 jmp xchain0_n45_α
 xchain0_n42_β:
 jmp xchain0_n46_α
# IR_ASSIGN gva
 xchain0_n43_α:
 mov rax, qword ptr [r12 + 1360]
 mov rdx, qword ptr [r12 + 1368]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1344], rax
 mov qword ptr [r12 + 1352], rdx
 jmp main_γ
 xchain0_n43_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n44_α:
 mov qword ptr [r12 + 1712], 6
 mov rax, qword ptr [rip + .Lx47_0]
 mov qword ptr [r12 + 1720], rax
 jmp xchain0_n47_α
 xchain0_n44_β:
 jmp xchain0_n42_α
.Lx47_0:
 .quad 1
# IR_LIT_INTEGER
 xchain0_n45_α:
 mov qword ptr [r12 + 2096], 6
 mov rax, qword ptr [rip + .Lx48_0]
 mov qword ptr [r12 + 2104], rax
 jmp xchain0_n48_α
 xchain0_n45_β:
 jmp xchain0_n46_α
.Lx48_0:
 .quad 1
# IR_VAR
 xchain0_n46_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 2368], rax
 mov qword ptr [r12 + 2376], rdx
 jmp xchain0_n49_α
 xchain0_n46_β:
 jmp xchain0_n50_α
# IR_LIT_INTEGER
 xchain0_n47_α:
 mov qword ptr [r12 + 1744], 6
 mov rax, qword ptr [rip + .Lx50_0]
 mov qword ptr [r12 + 1752], rax
 jmp xchain0_n51_α
 xchain0_n47_β:
 jmp xchain0_n42_α
.Lx50_0:
 .quad 2
# IR_LIT_INTEGER
 xchain0_n48_α:
 mov qword ptr [r12 + 2128], 6
 mov rax, qword ptr [rip + .Lx51_0]
 mov qword ptr [r12 + 2136], rax
 jmp xchain0_n52_α
 xchain0_n48_β:
 jmp xchain0_n46_α
.Lx51_0:
 .quad 2
# IR_LIT_INTEGER
 xchain0_n49_α:
 mov qword ptr [r12 + 2400], 6
 mov rax, qword ptr [rip + .Lx52_0]
 mov qword ptr [r12 + 2408], rax
 jmp xchain0_n53_α
 xchain0_n49_β:
 jmp xchain0_n50_α
.Lx52_0:
 .quad 1
# IR_VAR
 xchain0_n50_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 2752], rax
 mov qword ptr [r12 + 2760], rdx
 jmp xchain0_n54_α
 xchain0_n50_β:
 jmp xchain0_n55_α
# IR_LIT_INTEGER
 xchain0_n51_α:
 mov qword ptr [r12 + 1776], 6
 mov rax, qword ptr [rip + .Lx54_0]
 mov qword ptr [r12 + 1784], rax
 jmp xchain0_n56_α
 xchain0_n51_β:
 jmp xchain0_n42_α
.Lx54_0:
 .quad 1
# IR_LIT_INTEGER
 xchain0_n52_α:
 mov qword ptr [r12 + 2160], 6
 mov rax, qword ptr [rip + .Lx55_0]
 mov qword ptr [r12 + 2168], rax
 jmp xchain0_n57_α
 xchain0_n52_β:
 jmp xchain0_n46_α
.Lx55_0:
 .quad 1
# IR_SUBSCRIPT x[i] variable
 xchain0_n53_α:
 mov rdi, qword ptr [r12 + 2368]
 mov rsi, qword ptr [r12 + 2376]
 mov rdx, qword ptr [r12 + 2400]
 mov rcx, qword ptr [r12 + 2408]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n50_α
 mov qword ptr [r12 + 2432], rax
 mov qword ptr [r12 + 2440], rdx
 jmp xchain0_n58_α
 xchain0_n53_β:
 jmp xchain0_n50_α
# IR_LIT_INTEGER
 xchain0_n54_α:
 mov qword ptr [r12 + 2784], 6
 mov rax, qword ptr [rip + .Lx57_0]
 mov qword ptr [r12 + 2792], rax
 jmp xchain0_n59_α
 xchain0_n54_β:
 jmp xchain0_n55_α
.Lx57_0:
 .quad 2
# IR_VAR
 xchain0_n55_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 3264], rax
 mov qword ptr [r12 + 3272], rdx
 jmp xchain0_n60_α
 xchain0_n55_β:
 jmp xchain0_n61_α
# IR_LIT_INTEGER
 xchain0_n56_α:
 mov qword ptr [r12 + 1808], 6
 mov rax, qword ptr [rip + .Lx59_0]
 mov qword ptr [r12 + 1816], rax
 jmp xchain0_n62_α
 xchain0_n56_β:
 jmp xchain0_n42_α
.Lx59_0:
 .quad 2
# IR_LIT_INTEGER
 xchain0_n57_α:
 mov qword ptr [r12 + 2192], 6
 mov rax, qword ptr [rip + .Lx60_0]
 mov qword ptr [r12 + 2200], rax
 jmp xchain0_n63_α
 xchain0_n57_β:
 jmp xchain0_n46_α
.Lx60_0:
 .quad 2
# IR_LIT_INTEGER
 xchain0_n58_α:
 mov qword ptr [r12 + 2464], 6
 mov rax, qword ptr [rip + .Lx61_0]
 mov qword ptr [r12 + 2472], rax
 jmp xchain0_n64_α
 xchain0_n58_β:
 jmp xchain0_n50_α
.Lx61_0:
 .quad 2
# IR_SUBSCRIPT x[i] variable
 xchain0_n59_α:
 mov rdi, qword ptr [r12 + 2752]
 mov rsi, qword ptr [r12 + 2760]
 mov rdx, qword ptr [r12 + 2784]
 mov rcx, qword ptr [r12 + 2792]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n55_α
 mov qword ptr [r12 + 2816], rax
 mov qword ptr [r12 + 2824], rdx
 jmp xchain0_n65_α
 xchain0_n59_β:
 jmp xchain0_n55_α
# IR_LIT_INTEGER
 xchain0_n60_α:
 mov qword ptr [r12 + 3296], 6
 mov rax, qword ptr [rip + .Lx63_0]
 mov qword ptr [r12 + 3304], rax
 jmp xchain0_n66_α
 xchain0_n60_β:
 jmp xchain0_n61_α
.Lx63_0:
 .quad 2
 xchain0_n61_α:
# BOX IR_CALL table(...) -> rt_call_arr [operand-marshal, FAIL->ω]
  .section .rodata
  .Lrkfn65: .string "table"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn65]
 lea rsi, [r12 + 3536]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 3520], rax
 mov qword ptr [r12 + 3528], rdx
 cmp eax, 99
 je xchain0_n68_α
 jmp xchain0_n67_α
 xchain0_n61_β:
 jmp xchain0_n68_α
 xchain0_n62_α:
# BOX CALL item(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1680] -> [zr+1584]
 mov rax, qword ptr [r12 + 1680]
 mov qword ptr [r12 + 1584], rax
 mov rax, qword ptr [r12 + 1688]
 mov qword ptr [r12 + 1592], rax
# marshal arg1 = producer-box slot [zr+1712] -> [zr+1600]
 mov rax, qword ptr [r12 + 1712]
 mov qword ptr [r12 + 1600], rax
 mov rax, qword ptr [r12 + 1720]
 mov qword ptr [r12 + 1608], rax
# marshal arg2 = producer-box slot [zr+1744] -> [zr+1616]
 mov rax, qword ptr [r12 + 1744]
 mov qword ptr [r12 + 1616], rax
 mov rax, qword ptr [r12 + 1752]
 mov qword ptr [r12 + 1624], rax
# marshal arg3 = producer-box slot [zr+1776] -> [zr+1632]
 mov rax, qword ptr [r12 + 1776]
 mov qword ptr [r12 + 1632], rax
 mov rax, qword ptr [r12 + 1784]
 mov qword ptr [r12 + 1640], rax
# marshal arg4 = producer-box slot [zr+1808] -> [zr+1648]
 mov rax, qword ptr [r12 + 1808]
 mov qword ptr [r12 + 1648], rax
 mov rax, qword ptr [r12 + 1816]
 mov qword ptr [r12 + 1656], rax
  .section .rodata
  .Lbynamefn63: .string "item"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn63]
 lea rsi, [r12 + 1584]
 mov edx, 5
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1568], rax
 mov qword ptr [r12 + 1576], rdx
 cmp eax, 99
 je xchain0_n42_α
 jmp xchain0_n69_α
 xchain0_n62_β:
 jmp xchain0_n42_α
 xchain0_n63_α:
# BOX CALL item(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2064] -> [zr+1968]
 mov rax, qword ptr [r12 + 2064]
 mov qword ptr [r12 + 1968], rax
 mov rax, qword ptr [r12 + 2072]
 mov qword ptr [r12 + 1976], rax
# marshal arg1 = producer-box slot [zr+2096] -> [zr+1984]
 mov rax, qword ptr [r12 + 2096]
 mov qword ptr [r12 + 1984], rax
 mov rax, qword ptr [r12 + 2104]
 mov qword ptr [r12 + 1992], rax
# marshal arg2 = producer-box slot [zr+2128] -> [zr+2000]
 mov rax, qword ptr [r12 + 2128]
 mov qword ptr [r12 + 2000], rax
 mov rax, qword ptr [r12 + 2136]
 mov qword ptr [r12 + 2008], rax
# marshal arg3 = producer-box slot [zr+2160] -> [zr+2016]
 mov rax, qword ptr [r12 + 2160]
 mov qword ptr [r12 + 2016], rax
 mov rax, qword ptr [r12 + 2168]
 mov qword ptr [r12 + 2024], rax
# marshal arg4 = producer-box slot [zr+2192] -> [zr+2032]
 mov rax, qword ptr [r12 + 2192]
 mov qword ptr [r12 + 2032], rax
 mov rax, qword ptr [r12 + 2200]
 mov qword ptr [r12 + 2040], rax
  .section .rodata
  .Lbynamefn64: .string "item"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn64]
 lea rsi, [r12 + 1968]
 mov edx, 5
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1952], rax
 mov qword ptr [r12 + 1960], rdx
 cmp eax, 99
 je xchain0_n46_α
 jmp xchain0_n70_α
 xchain0_n63_β:
 jmp xchain0_n46_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n64_α:
 mov rdi, qword ptr [r12 + 2432]
 mov rsi, qword ptr [r12 + 2440]
 mov rdx, qword ptr [r12 + 2464]
 mov rcx, qword ptr [r12 + 2472]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n50_α
 mov qword ptr [r12 + 2496], rax
 mov qword ptr [r12 + 2504], rdx
 jmp xchain0_n71_α
 xchain0_n64_β:
 jmp xchain0_n50_α
# IR_LIT_INTEGER
 xchain0_n65_α:
 mov qword ptr [r12 + 2848], 6
 mov rax, qword ptr [rip + .Lx69_0]
 mov qword ptr [r12 + 2856], rax
 jmp xchain0_n72_α
 xchain0_n65_β:
 jmp xchain0_n55_α
.Lx69_0:
 .quad 1
# IR_LIT_INTEGER
 xchain0_n66_α:
 mov qword ptr [r12 + 3328], 6
 mov rax, qword ptr [rip + .Lx70_0]
 mov qword ptr [r12 + 3336], rax
 jmp xchain0_n73_α
 xchain0_n66_β:
 jmp xchain0_n61_α
.Lx70_0:
 .quad 1
# IR_ASSIGN gva
 xchain0_n67_α:
 mov rax, qword ptr [r12 + 3520]
 mov rdx, qword ptr [r12 + 3528]
 mov qword ptr [rbx + 48], rax
 mov qword ptr [rbx + 56], rdx
 mov qword ptr [r12 + 3504], rax
 mov qword ptr [r12 + 3512], rdx
 jmp xchain0_n68_α
 xchain0_n67_β:
 jmp xchain0_n68_α
# IR_LIT_STRING
 xchain0_n68_α:
 mov qword ptr [r12 + 3552], 1
 mov rax, qword ptr [rip + .Lx72_0]
 mov qword ptr [r12 + 3560], rax
 jmp xchain0_n74_α
 xchain0_n68_β:
 jmp xchain0_n81_α
.Lx72_0:
 .quad .Lx72_0_s
.Lx72_0_s:
 .string ""
# IR_LIT_INTEGER
 xchain0_n69_α:
 mov qword ptr [r12 + 1840], 6
 mov rax, qword ptr [rip + .Lx73_0]
 mov qword ptr [r12 + 1848], rax
 jmp xchain0_n75_α
 xchain0_n69_β:
 jmp xchain0_n42_α
.Lx73_0:
 .quad 1212
# IR_LIT_INTEGER
 xchain0_n70_α:
 mov qword ptr [r12 + 2224], 6
 mov rax, qword ptr [rip + .Lx74_0]
 mov qword ptr [r12 + 2232], rax
 jmp xchain0_n76_α
 xchain0_n70_β:
 jmp xchain0_n46_α
.Lx74_0:
 .quad 1212
# IR_LIT_INTEGER
 xchain0_n71_α:
 mov qword ptr [r12 + 2528], 6
 mov rax, qword ptr [rip + .Lx75_0]
 mov qword ptr [r12 + 2536], rax
 jmp xchain0_n77_α
 xchain0_n71_β:
 jmp xchain0_n50_α
.Lx75_0:
 .quad 1
# IR_SUBSCRIPT x[i] variable
 xchain0_n72_α:
 mov rdi, qword ptr [r12 + 2816]
 mov rsi, qword ptr [r12 + 2824]
 mov rdx, qword ptr [r12 + 2848]
 mov rcx, qword ptr [r12 + 2856]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n55_α
 mov qword ptr [r12 + 2880], rax
 mov qword ptr [r12 + 2888], rdx
 jmp xchain0_n78_α
 xchain0_n72_β:
 jmp xchain0_n55_α
# IR_LIT_INTEGER
 xchain0_n73_α:
 mov qword ptr [r12 + 3360], 6
 mov rax, qword ptr [rip + .Lx77_0]
 mov qword ptr [r12 + 3368], rax
 jmp xchain0_n79_α
 xchain0_n73_β:
 jmp xchain0_n61_α
.Lx77_0:
 .quad 2
 xchain0_n74_α:
# BOX IR_CALL SNO$WANTNM(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+3552] -> [zr+3600]
 mov rax, qword ptr [r12 + 3552]
 mov qword ptr [r12 + 3600], rax
 mov rax, qword ptr [r12 + 3560]
 mov qword ptr [r12 + 3608], rax
  .section .rodata
  .Lrkfn79: .string "SNO$WANTNM"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn79]
 lea rsi, [r12 + 3600]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 3584], rax
 mov qword ptr [r12 + 3592], rdx
 cmp eax, 99
 je xchain0_n81_α
 jmp xchain0_n80_α
 xchain0_n74_β:
 jmp xchain0_n81_α
# IR_ASSIGN_VAR
 xchain0_n75_α:
 mov rdi, qword ptr [r12 + 1568]
 mov rsi, qword ptr [r12 + 1576]
 mov rdx, qword ptr [r12 + 1840]
 mov rcx, qword ptr [r12 + 1848]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n42_α
 mov qword ptr [r12 + 1872], rax
 mov qword ptr [r12 + 1880], rdx
 jmp xchain0_n42_α
 xchain0_n75_β:
 jmp xchain0_n42_α
 xchain0_n76_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1952] -> [zr+1904]
 mov rax, qword ptr [r12 + 1952]
 mov qword ptr [r12 + 1904], rax
 mov rax, qword ptr [r12 + 1960]
 mov qword ptr [r12 + 1912], rax
# marshal arg1 = producer-box slot [zr+2224] -> [zr+1920]
 mov rax, qword ptr [r12 + 2224]
 mov qword ptr [r12 + 1920], rax
 mov rax, qword ptr [r12 + 2232]
 mov qword ptr [r12 + 1928], rax
  .section .rodata
  .Lbynamefn77: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn77]
 lea rsi, [r12 + 1904]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1888], rax
 mov qword ptr [r12 + 1896], rdx
 cmp eax, 99
 je xchain0_n46_α
 jmp xchain0_n82_α
 xchain0_n76_β:
 jmp xchain0_n46_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n77_α:
 mov rdi, qword ptr [r12 + 2496]
 mov rsi, qword ptr [r12 + 2504]
 mov rdx, qword ptr [r12 + 2528]
 mov rcx, qword ptr [r12 + 2536]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n50_α
 mov qword ptr [r12 + 2560], rax
 mov qword ptr [r12 + 2568], rdx
 jmp xchain0_n83_α
 xchain0_n77_β:
 jmp xchain0_n50_α
# IR_LIT_INTEGER
 xchain0_n78_α:
 mov qword ptr [r12 + 2912], 6
 mov rax, qword ptr [rip + .Lx83_0]
 mov qword ptr [r12 + 2920], rax
 jmp xchain0_n84_α
 xchain0_n78_β:
 jmp xchain0_n55_α
.Lx83_0:
 .quad 2
# IR_LIT_INTEGER
 xchain0_n79_α:
 mov qword ptr [r12 + 3392], 6
 mov rax, qword ptr [rip + .Lx84_0]
 mov qword ptr [r12 + 3400], rax
 jmp xchain0_n85_α
 xchain0_n79_β:
 jmp xchain0_n61_α
.Lx84_0:
 .quad 1
# IR_VAR
 xchain0_n80_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 3696], rax
 mov qword ptr [r12 + 3704], rdx
 jmp xchain0_n86_α
 xchain0_n80_β:
 jmp xchain0_n81_α
# IR_VAR
 xchain0_n81_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 3936], rax
 mov qword ptr [r12 + 3944], rdx
 jmp xchain0_n87_α
 xchain0_n81_β:
 jmp xchain0_n88_α
# IR_LIT_STRING
 xchain0_n82_α:
 mov qword ptr [r12 + 2272], 1
 mov rax, qword ptr [rip + .Lx87_0]
 mov qword ptr [r12 + 2280], rax
 jmp xchain0_n89_α
 xchain0_n82_β:
 jmp main_γ
.Lx87_0:
 .quad .Lx87_0_s
.Lx87_0_s:
 .string "FAIL 1114/004: item 4D assign/read"
# IR_LIT_INTEGER
 xchain0_n83_α:
 mov qword ptr [r12 + 2592], 6
 mov rax, qword ptr [rip + .Lx88_0]
 mov qword ptr [r12 + 2600], rax
 jmp xchain0_n90_α
 xchain0_n83_β:
 jmp xchain0_n50_α
.Lx88_0:
 .quad 2
# IR_SUBSCRIPT x[i] variable
 xchain0_n84_α:
 mov rdi, qword ptr [r12 + 2880]
 mov rsi, qword ptr [r12 + 2888]
 mov rdx, qword ptr [r12 + 2912]
 mov rcx, qword ptr [r12 + 2920]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n55_α
 mov qword ptr [r12 + 2944], rax
 mov qword ptr [r12 + 2952], rdx
 jmp xchain0_n91_α
 xchain0_n84_β:
 jmp xchain0_n55_α
 xchain0_n85_α:
# BOX CALL item(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+3264] -> [zr+3168]
 mov rax, qword ptr [r12 + 3264]
 mov qword ptr [r12 + 3168], rax
 mov rax, qword ptr [r12 + 3272]
 mov qword ptr [r12 + 3176], rax
# marshal arg1 = producer-box slot [zr+3296] -> [zr+3184]
 mov rax, qword ptr [r12 + 3296]
 mov qword ptr [r12 + 3184], rax
 mov rax, qword ptr [r12 + 3304]
 mov qword ptr [r12 + 3192], rax
# marshal arg2 = producer-box slot [zr+3328] -> [zr+3200]
 mov rax, qword ptr [r12 + 3328]
 mov qword ptr [r12 + 3200], rax
 mov rax, qword ptr [r12 + 3336]
 mov qword ptr [r12 + 3208], rax
# marshal arg3 = producer-box slot [zr+3360] -> [zr+3216]
 mov rax, qword ptr [r12 + 3360]
 mov qword ptr [r12 + 3216], rax
 mov rax, qword ptr [r12 + 3368]
 mov qword ptr [r12 + 3224], rax
# marshal arg4 = producer-box slot [zr+3392] -> [zr+3232]
 mov rax, qword ptr [r12 + 3392]
 mov qword ptr [r12 + 3232], rax
 mov rax, qword ptr [r12 + 3400]
 mov qword ptr [r12 + 3240], rax
  .section .rodata
  .Lbynamefn86: .string "item"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn86]
 lea rsi, [r12 + 3168]
 mov edx, 5
 call rt_call_arr@PLT
 mov qword ptr [r12 + 3152], rax
 mov qword ptr [r12 + 3160], rdx
 cmp eax, 99
 je xchain0_n61_α
 jmp xchain0_n92_α
 xchain0_n85_β:
 jmp xchain0_n61_α
# IR_LIT_STRING
 xchain0_n86_α:
 mov qword ptr [r12 + 3728], 1
 mov rax, qword ptr [rip + .Lx91_0]
 mov qword ptr [r12 + 3736], rax
 jmp xchain0_n93_α
 xchain0_n86_β:
 jmp xchain0_n81_α
.Lx91_0:
 .quad .Lx91_0_s
.Lx91_0_s:
 .string "key"
# IR_LIT_STRING
 xchain0_n87_α:
 mov qword ptr [r12 + 3968], 1
 mov rax, qword ptr [rip + .Lx92_0]
 mov qword ptr [r12 + 3976], rax
 jmp xchain0_n94_α
 xchain0_n87_β:
 jmp xchain0_n88_α
.Lx92_0:
 .quad .Lx92_0_s
.Lx92_0_s:
 .string "key"
# IR_LIT_STRING
 xchain0_n88_α:
 mov qword ptr [r12 + 4096], 1
 mov rax, qword ptr [rip + .Lx93_0]
 mov qword ptr [r12 + 4104], rax
 jmp xchain0_n95_α
 xchain0_n88_β:
 jmp main_γ
.Lx93_0:
 .quad .Lx93_0_s
.Lx93_0_s:
 .string "PASS 1114_item (7/7)"
# IR_ASSIGN gva
 xchain0_n89_α:
 mov rax, qword ptr [r12 + 2272]
 mov rdx, qword ptr [r12 + 2280]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 2256], rax
 mov qword ptr [r12 + 2264], rdx
 jmp main_γ
 xchain0_n89_β:
 jmp main_γ
# IR_SUBSCRIPT x[i] variable
 xchain0_n90_α:
 mov rdi, qword ptr [r12 + 2560]
 mov rsi, qword ptr [r12 + 2568]
 mov rdx, qword ptr [r12 + 2592]
 mov rcx, qword ptr [r12 + 2600]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n50_α
 mov qword ptr [r12 + 2624], rax
 mov qword ptr [r12 + 2632], rdx
 jmp xchain0_n96_α
 xchain0_n90_β:
 jmp xchain0_n50_α
# IR_LIT_INTEGER
 xchain0_n91_α:
 mov qword ptr [r12 + 2976], 6
 mov rax, qword ptr [rip + .Lx96_0]
 mov qword ptr [r12 + 2984], rax
 jmp xchain0_n97_α
 xchain0_n91_β:
 jmp xchain0_n55_α
.Lx96_0:
 .quad 1
# IR_LIT_INTEGER
 xchain0_n92_α:
 mov qword ptr [r12 + 3424], 6
 mov rax, qword ptr [rip + .Lx97_0]
 mov qword ptr [r12 + 3432], rax
 jmp xchain0_n98_α
 xchain0_n92_β:
 jmp xchain0_n61_α
.Lx97_0:
 .quad 2121
 xchain0_n93_α:
# BOX CALL item(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+3696] -> [zr+3648]
 mov rax, qword ptr [r12 + 3696]
 mov qword ptr [r12 + 3648], rax
 mov rax, qword ptr [r12 + 3704]
 mov qword ptr [r12 + 3656], rax
# marshal arg1 = producer-box slot [zr+3728] -> [zr+3664]
 mov rax, qword ptr [r12 + 3728]
 mov qword ptr [r12 + 3664], rax
 mov rax, qword ptr [r12 + 3736]
 mov qword ptr [r12 + 3672], rax
  .section .rodata
  .Lbynamefn94: .string "item"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn94]
 lea rsi, [r12 + 3648]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 3632], rax
 mov qword ptr [r12 + 3640], rdx
 cmp eax, 99
 je xchain0_n81_α
 jmp xchain0_n99_α
 xchain0_n93_β:
 jmp xchain0_n81_α
 xchain0_n94_α:
# BOX CALL item(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+3936] -> [zr+3888]
 mov rax, qword ptr [r12 + 3936]
 mov qword ptr [r12 + 3888], rax
 mov rax, qword ptr [r12 + 3944]
 mov qword ptr [r12 + 3896], rax
# marshal arg1 = producer-box slot [zr+3968] -> [zr+3904]
 mov rax, qword ptr [r12 + 3968]
 mov qword ptr [r12 + 3904], rax
 mov rax, qword ptr [r12 + 3976]
 mov qword ptr [r12 + 3912], rax
  .section .rodata
  .Lbynamefn95: .string "item"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn95]
 lea rsi, [r12 + 3888]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 3872], rax
 mov qword ptr [r12 + 3880], rdx
 cmp eax, 99
 je xchain0_n88_α
 jmp xchain0_n100_α
 xchain0_n94_β:
 jmp xchain0_n88_α
# IR_ASSIGN gva
 xchain0_n95_α:
 mov rax, qword ptr [r12 + 4096]
 mov rdx, qword ptr [r12 + 4104]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 4080], rax
 mov qword ptr [r12 + 4088], rdx
 jmp main_γ
 xchain0_n95_β:
 jmp main_γ
# IR_DEREF variable -> value
 xchain0_n96_α:
 mov rdi, qword ptr [r12 + 2624]
 mov rsi, qword ptr [r12 + 2632]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n50_α
 mov qword ptr [r12 + 2656], rax
 mov qword ptr [r12 + 2664], rdx
 jmp xchain0_n101_α
 xchain0_n96_β:
 jmp xchain0_n50_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n97_α:
 mov rdi, qword ptr [r12 + 2944]
 mov rsi, qword ptr [r12 + 2952]
 mov rdx, qword ptr [r12 + 2976]
 mov rcx, qword ptr [r12 + 2984]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n55_α
 mov qword ptr [r12 + 3008], rax
 mov qword ptr [r12 + 3016], rdx
 jmp xchain0_n102_α
 xchain0_n97_β:
 jmp xchain0_n55_α
 xchain0_n98_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+3152] -> [zr+3104]
 mov rax, qword ptr [r12 + 3152]
 mov qword ptr [r12 + 3104], rax
 mov rax, qword ptr [r12 + 3160]
 mov qword ptr [r12 + 3112], rax
# marshal arg1 = producer-box slot [zr+3424] -> [zr+3120]
 mov rax, qword ptr [r12 + 3424]
 mov qword ptr [r12 + 3120], rax
 mov rax, qword ptr [r12 + 3432]
 mov qword ptr [r12 + 3128], rax
  .section .rodata
  .Lbynamefn99: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn99]
 lea rsi, [r12 + 3104]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 3088], rax
 mov qword ptr [r12 + 3096], rdx
 cmp eax, 99
 je xchain0_n61_α
 jmp xchain0_n103_α
 xchain0_n98_β:
 jmp xchain0_n61_α
# IR_LIT_STRING
 xchain0_n99_α:
 mov qword ptr [r12 + 3760], 1
 mov rax, qword ptr [rip + .Lx104_0]
 mov qword ptr [r12 + 3768], rax
 jmp xchain0_n104_α
 xchain0_n99_β:
 jmp xchain0_n81_α
.Lx104_0:
 .quad .Lx104_0_s
.Lx104_0_s:
 .string "val"
# IR_LIT_STRING
 xchain0_n100_α:
 mov qword ptr [r12 + 4000], 1
 mov rax, qword ptr [rip + .Lx105_0]
 mov qword ptr [r12 + 4008], rax
 jmp xchain0_n105_α
 xchain0_n100_β:
 jmp xchain0_n88_α
.Lx105_0:
 .quad .Lx105_0_s
.Lx105_0_s:
 .string "val"
# IR_LIT_INTEGER
 xchain0_n101_α:
 mov qword ptr [r12 + 2672], 6
 mov rax, qword ptr [rip + .Lx106_0]
 mov qword ptr [r12 + 2680], rax
 jmp xchain0_n106_α
 xchain0_n101_β:
 jmp xchain0_n50_α
.Lx106_0:
 .quad 1212
# IR_LIT_INTEGER
 xchain0_n102_α:
 mov qword ptr [r12 + 3040], 6
 mov rax, qword ptr [rip + .Lx107_0]
 mov qword ptr [r12 + 3048], rax
 jmp xchain0_n107_α
 xchain0_n102_β:
 jmp xchain0_n55_α
.Lx107_0:
 .quad 2121
# IR_LIT_STRING
 xchain0_n103_α:
 mov qword ptr [r12 + 3472], 1
 mov rax, qword ptr [rip + .Lx108_0]
 mov qword ptr [r12 + 3480], rax
 jmp xchain0_n108_α
 xchain0_n103_β:
 jmp main_γ
.Lx108_0:
 .quad .Lx108_0_s
.Lx108_0_s:
 .string "FAIL 1114/006: bracket 4D assign, item read"
# IR_ASSIGN_VAR
 xchain0_n104_α:
 mov rdi, qword ptr [r12 + 3632]
 mov rsi, qword ptr [r12 + 3640]
 mov rdx, qword ptr [r12 + 3760]
 mov rcx, qword ptr [r12 + 3768]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n81_α
 mov qword ptr [r12 + 3792], rax
 mov qword ptr [r12 + 3800], rdx
 jmp xchain0_n81_α
 xchain0_n104_β:
 jmp xchain0_n81_α
 xchain0_n105_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+3872] -> [zr+3824]
 mov rax, qword ptr [r12 + 3872]
 mov qword ptr [r12 + 3824], rax
 mov rax, qword ptr [r12 + 3880]
 mov qword ptr [r12 + 3832], rax
# marshal arg1 = producer-box slot [zr+4000] -> [zr+3840]
 mov rax, qword ptr [r12 + 4000]
 mov qword ptr [r12 + 3840], rax
 mov rax, qword ptr [r12 + 4008]
 mov qword ptr [r12 + 3848], rax
  .section .rodata
  .Lbynamefn106: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn106]
 lea rsi, [r12 + 3824]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 3808], rax
 mov qword ptr [r12 + 3816], rdx
 cmp eax, 99
 je xchain0_n88_α
 jmp xchain0_n109_α
 xchain0_n105_β:
 jmp xchain0_n88_α
 xchain0_n106_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2656] -> [zr+2320]
 mov rax, qword ptr [r12 + 2656]
 mov qword ptr [r12 + 2320], rax
 mov rax, qword ptr [r12 + 2664]
 mov qword ptr [r12 + 2328], rax
# marshal arg1 = producer-box slot [zr+2672] -> [zr+2336]
 mov rax, qword ptr [r12 + 2672]
 mov qword ptr [r12 + 2336], rax
 mov rax, qword ptr [r12 + 2680]
 mov qword ptr [r12 + 2344], rax
  .section .rodata
  .Lbynamefn107: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn107]
 lea rsi, [r12 + 2320]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2304], rax
 mov qword ptr [r12 + 2312], rdx
 cmp eax, 99
 je xchain0_n50_α
 jmp xchain0_n110_α
 xchain0_n106_β:
 jmp xchain0_n50_α
# IR_ASSIGN_VAR
 xchain0_n107_α:
 mov rdi, qword ptr [r12 + 3008]
 mov rsi, qword ptr [r12 + 3016]
 mov rdx, qword ptr [r12 + 3040]
 mov rcx, qword ptr [r12 + 3048]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n55_α
 mov qword ptr [r12 + 3072], rax
 mov qword ptr [r12 + 3080], rdx
 jmp xchain0_n55_α
 xchain0_n107_β:
 jmp xchain0_n55_α
# IR_ASSIGN gva
 xchain0_n108_α:
 mov rax, qword ptr [r12 + 3472]
 mov rdx, qword ptr [r12 + 3480]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 3456], rax
 mov qword ptr [r12 + 3464], rdx
 jmp main_γ
 xchain0_n108_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n109_α:
 mov qword ptr [r12 + 4048], 1
 mov rax, qword ptr [rip + .Lx114_0]
 mov qword ptr [r12 + 4056], rax
 jmp xchain0_n111_α
 xchain0_n109_β:
 jmp main_γ
.Lx114_0:
 .quad .Lx114_0_s
.Lx114_0_s:
 .string "FAIL 1114/007: item on table"
# IR_LIT_STRING
 xchain0_n110_α:
 mov qword ptr [r12 + 2720], 1
 mov rax, qword ptr [rip + .Lx115_0]
 mov qword ptr [r12 + 2728], rax
 jmp xchain0_n112_α
 xchain0_n110_β:
 jmp main_γ
.Lx115_0:
 .quad .Lx115_0_s
.Lx115_0_s:
 .string "FAIL 1114/005: item 4D == bracket"
# IR_ASSIGN gva
 xchain0_n111_α:
 mov rax, qword ptr [r12 + 4048]
 mov rdx, qword ptr [r12 + 4056]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 4032], rax
 mov qword ptr [r12 + 4040], rdx
 jmp main_γ
 xchain0_n111_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain0_n112_α:
 mov rax, qword ptr [r12 + 2720]
 mov rdx, qword ptr [r12 + 2728]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 2704], rax
 mov qword ptr [r12 + 2712], rdx
 jmp main_γ
 xchain0_n112_β:
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [r12 + 4136]
pop r12
ret
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 4128]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, qword ptr [r12 + 4136]
pop r12
ret

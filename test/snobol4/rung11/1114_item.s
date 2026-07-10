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
 mov qword ptr [r12 + 2576], rax
 pop rsi
main_α_body:
# IR_LIT_INTEGER
 xchain0_n0_α:
 mov qword ptr [r12 + 112], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n3_α
.Lx1_0:
 .quad 10
 xchain0_n1_α:
# BOX CALL array(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+112] -> [zr+96]
 mov rax, qword ptr [r12 + 112]
 mov qword ptr [r12 + 96], rax
 mov rax, qword ptr [r12 + 120]
 mov qword ptr [r12 + 104], rax
  .section .rodata
  .Lbynamefn2: .string "array"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn2]
 lea rsi, [r12 + 96]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n3_α
# IR_ASSIGN gva
 xchain0_n2_α:
 mov rax, qword ptr [r12 + 80]
 mov rdx, qword ptr [r12 + 88]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n3_α
# IR_LIT_STRING
 xchain0_n3_α:
 mov qword ptr [r12 + 128], 1
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [r12 + 136], rax
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n6_α
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string ""
 xchain0_n4_α:
# BOX IR_CALL SNO$WANTNM(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+128] -> [zr+160]
 mov rax, qword ptr [r12 + 128]
 mov qword ptr [r12 + 160], rax
 mov rax, qword ptr [r12 + 136]
 mov qword ptr [r12 + 168], rax
  .section .rodata
  .Lrkfn6: .string "SNO$WANTNM"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn6]
 lea rsi, [r12 + 160]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 cmp eax, 99
 je xchain0_n6_α
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n6_α
# IR_VAR
 xchain0_n5_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n6_α
# IR_VAR
 xchain0_n6_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 jmp xchain0_n8_α
 xchain0_n6_β:
 jmp xchain0_n9_α
# IR_LIT_INTEGER
 xchain0_n7_α:
 mov qword ptr [r12 + 240], 6
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 248], rax
 jmp xchain0_n10_α
 xchain0_n7_β:
 jmp xchain0_n6_α
.Lx9_0:
 .quad 1
# IR_LIT_INTEGER
 xchain0_n8_α:
 mov qword ptr [r12 + 400], 6
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [r12 + 408], rax
 jmp xchain0_n11_α
 xchain0_n8_β:
 jmp xchain0_n9_α
.Lx10_0:
 .quad 1
# IR_VAR
 xchain0_n9_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 512], rax
 mov qword ptr [r12 + 520], rdx
 jmp xchain0_n12_α
 xchain0_n9_β:
 jmp xchain0_n13_α
 xchain0_n10_α:
# BOX CALL item(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
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
  .Lbynamefn11: .string "item"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn11]
 lea rsi, [r12 + 192]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 cmp eax, 99
 je xchain0_n6_α
 jmp xchain0_n14_α
 xchain0_n10_β:
 jmp xchain0_n6_α
 xchain0_n11_α:
# BOX CALL item(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+384] -> [zr+352]
 mov rax, qword ptr [r12 + 384]
 mov qword ptr [r12 + 352], rax
 mov rax, qword ptr [r12 + 392]
 mov qword ptr [r12 + 360], rax
# marshal arg1 = producer-box slot [zr+400] -> [zr+368]
 mov rax, qword ptr [r12 + 400]
 mov qword ptr [r12 + 368], rax
 mov rax, qword ptr [r12 + 408]
 mov qword ptr [r12 + 376], rax
  .section .rodata
  .Lbynamefn12: .string "item"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn12]
 lea rsi, [r12 + 352]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 cmp eax, 99
 je xchain0_n9_α
 jmp xchain0_n15_α
 xchain0_n11_β:
 jmp xchain0_n9_α
# IR_LIT_INTEGER
 xchain0_n12_α:
 mov qword ptr [r12 + 528], 6
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [r12 + 536], rax
 jmp xchain0_n16_α
 xchain0_n12_β:
 jmp xchain0_n13_α
.Lx14_0:
 .quad 1
# IR_VAR
 xchain0_n13_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 624], rax
 mov qword ptr [r12 + 632], rdx
 jmp xchain0_n17_α
 xchain0_n13_β:
 jmp xchain0_n18_α
# IR_LIT_INTEGER
 xchain0_n14_α:
 mov qword ptr [r12 + 256], 6
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [r12 + 264], rax
 jmp xchain0_n19_α
 xchain0_n14_β:
 jmp xchain0_n6_α
.Lx16_0:
 .quad 5
# IR_LIT_INTEGER
 xchain0_n15_α:
 mov qword ptr [r12 + 416], 6
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [r12 + 424], rax
 jmp xchain0_n20_α
 xchain0_n15_β:
 jmp xchain0_n9_α
.Lx17_0:
 .quad 5
# IR_SUBSCRIPT x[i] variable
 xchain0_n16_α:
 mov rdi, qword ptr [r12 + 512]
 mov rsi, qword ptr [r12 + 520]
 mov rdx, qword ptr [r12 + 528]
 mov rcx, qword ptr [r12 + 536]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n13_α
 mov qword ptr [r12 + 544], rax
 mov qword ptr [r12 + 552], rdx
 jmp xchain0_n21_α
 xchain0_n16_β:
 jmp xchain0_n13_α
# IR_LIT_INTEGER
 xchain0_n17_α:
 mov qword ptr [r12 + 640], 6
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [r12 + 648], rax
 jmp xchain0_n22_α
 xchain0_n17_β:
 jmp xchain0_n18_α
.Lx19_0:
 .quad 2
# IR_VAR
 xchain0_n18_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 800], rax
 mov qword ptr [r12 + 808], rdx
 jmp xchain0_n23_α
 xchain0_n18_β:
 jmp xchain0_n24_α
# IR_ASSIGN_VAR
 xchain0_n19_α:
 mov rdi, qword ptr [r12 + 176]
 mov rsi, qword ptr [r12 + 184]
 mov rdx, qword ptr [r12 + 256]
 mov rcx, qword ptr [r12 + 264]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n6_α
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 jmp xchain0_n6_α
 xchain0_n19_β:
 jmp xchain0_n6_α
 xchain0_n20_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+336] -> [zr+304]
 mov rax, qword ptr [r12 + 336]
 mov qword ptr [r12 + 304], rax
 mov rax, qword ptr [r12 + 344]
 mov qword ptr [r12 + 312], rax
# marshal arg1 = producer-box slot [zr+416] -> [zr+320]
 mov rax, qword ptr [r12 + 416]
 mov qword ptr [r12 + 320], rax
 mov rax, qword ptr [r12 + 424]
 mov qword ptr [r12 + 328], rax
  .section .rodata
  .Lbynamefn21: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn21]
 lea rsi, [r12 + 304]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 288], rax
 mov qword ptr [r12 + 296], rdx
 cmp eax, 99
 je xchain0_n9_α
 jmp xchain0_n25_α
 xchain0_n20_β:
 jmp xchain0_n9_α
# IR_DEREF variable -> value
 xchain0_n21_α:
 mov rdi, qword ptr [r12 + 544]
 mov rsi, qword ptr [r12 + 552]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n13_α
 mov qword ptr [r12 + 560], rax
 mov qword ptr [r12 + 568], rdx
 jmp xchain0_n26_α
 xchain0_n21_β:
 jmp xchain0_n13_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n22_α:
 mov rdi, qword ptr [r12 + 624]
 mov rsi, qword ptr [r12 + 632]
 mov rdx, qword ptr [r12 + 640]
 mov rcx, qword ptr [r12 + 648]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n18_α
 mov qword ptr [r12 + 656], rax
 mov qword ptr [r12 + 664], rdx
 jmp xchain0_n27_α
 xchain0_n22_β:
 jmp xchain0_n18_α
# IR_LIT_INTEGER
 xchain0_n23_α:
 mov qword ptr [r12 + 816], 6
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [r12 + 824], rax
 jmp xchain0_n28_α
 xchain0_n23_β:
 jmp xchain0_n24_α
.Lx25_0:
 .quad 2
# IR_LIT_STRING
 xchain0_n24_α:
 mov qword ptr [r12 + 928], 1
 mov rax, qword ptr [rip + .Lx26_0]
 mov qword ptr [r12 + 936], rax
 jmp xchain0_n29_α
 xchain0_n24_β:
 jmp xchain0_n35_α
.Lx26_0:
 .quad .Lx26_0_s
.Lx26_0_s:
 .string "2,2,2,2"
# IR_LIT_STRING
 xchain0_n25_α:
 mov qword ptr [r12 + 448], 1
 mov rax, qword ptr [rip + .Lx27_0]
 mov qword ptr [r12 + 456], rax
 jmp xchain0_n30_α
 xchain0_n25_β:
 jmp main_γ
.Lx27_0:
 .quad .Lx27_0_s
.Lx27_0_s:
 .string "FAIL 1114/001: item 1D assign/read"
# IR_LIT_INTEGER
 xchain0_n26_α:
 mov qword ptr [r12 + 576], 6
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [r12 + 584], rax
 jmp xchain0_n31_α
 xchain0_n26_β:
 jmp xchain0_n13_α
.Lx28_0:
 .quad 5
# IR_LIT_INTEGER
 xchain0_n27_α:
 mov qword ptr [r12 + 672], 6
 mov rax, qword ptr [rip + .Lx29_0]
 mov qword ptr [r12 + 680], rax
 jmp xchain0_n32_α
 xchain0_n27_β:
 jmp xchain0_n18_α
.Lx29_0:
 .quad 22
 xchain0_n28_α:
# BOX CALL item(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+800] -> [zr+768]
 mov rax, qword ptr [r12 + 800]
 mov qword ptr [r12 + 768], rax
 mov rax, qword ptr [r12 + 808]
 mov qword ptr [r12 + 776], rax
# marshal arg1 = producer-box slot [zr+816] -> [zr+784]
 mov rax, qword ptr [r12 + 816]
 mov qword ptr [r12 + 784], rax
 mov rax, qword ptr [r12 + 824]
 mov qword ptr [r12 + 792], rax
  .section .rodata
  .Lbynamefn29: .string "item"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn29]
 lea rsi, [r12 + 768]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 752], rax
 mov qword ptr [r12 + 760], rdx
 cmp eax, 99
 je xchain0_n24_α
 jmp xchain0_n33_α
 xchain0_n28_β:
 jmp xchain0_n24_α
 xchain0_n29_α:
# BOX CALL array(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+928] -> [zr+912]
 mov rax, qword ptr [r12 + 928]
 mov qword ptr [r12 + 912], rax
 mov rax, qword ptr [r12 + 936]
 mov qword ptr [r12 + 920], rax
  .section .rodata
  .Lbynamefn30: .string "array"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn30]
 lea rsi, [r12 + 912]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 896], rax
 mov qword ptr [r12 + 904], rdx
 cmp eax, 99
 je xchain0_n35_α
 jmp xchain0_n34_α
 xchain0_n29_β:
 jmp xchain0_n35_α
# IR_ASSIGN gva
 xchain0_n30_α:
 mov rax, qword ptr [r12 + 448]
 mov rdx, qword ptr [r12 + 456]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 432], rax
 mov qword ptr [r12 + 440], rdx
 jmp main_γ
 xchain0_n30_β:
 jmp main_γ
 xchain0_n31_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+560] -> [zr+480]
 mov rax, qword ptr [r12 + 560]
 mov qword ptr [r12 + 480], rax
 mov rax, qword ptr [r12 + 568]
 mov qword ptr [r12 + 488], rax
# marshal arg1 = producer-box slot [zr+576] -> [zr+496]
 mov rax, qword ptr [r12 + 576]
 mov qword ptr [r12 + 496], rax
 mov rax, qword ptr [r12 + 584]
 mov qword ptr [r12 + 504], rax
  .section .rodata
  .Lbynamefn32: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn32]
 lea rsi, [r12 + 480]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 464], rax
 mov qword ptr [r12 + 472], rdx
 cmp eax, 99
 je xchain0_n13_α
 jmp xchain0_n36_α
 xchain0_n31_β:
 jmp xchain0_n13_α
# IR_ASSIGN_VAR
 xchain0_n32_α:
 mov rdi, qword ptr [r12 + 656]
 mov rsi, qword ptr [r12 + 664]
 mov rdx, qword ptr [r12 + 672]
 mov rcx, qword ptr [r12 + 680]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n18_α
 mov qword ptr [r12 + 688], rax
 mov qword ptr [r12 + 696], rdx
 jmp xchain0_n18_α
 xchain0_n32_β:
 jmp xchain0_n18_α
# IR_LIT_INTEGER
 xchain0_n33_α:
 mov qword ptr [r12 + 832], 6
 mov rax, qword ptr [rip + .Lx35_0]
 mov qword ptr [r12 + 840], rax
 jmp xchain0_n37_α
 xchain0_n33_β:
 jmp xchain0_n24_α
.Lx35_0:
 .quad 22
# IR_ASSIGN gva
 xchain0_n34_α:
 mov rax, qword ptr [r12 + 896]
 mov rdx, qword ptr [r12 + 904]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [r12 + 880], rax
 mov qword ptr [r12 + 888], rdx
 jmp xchain0_n35_α
 xchain0_n34_β:
 jmp xchain0_n35_α
# IR_LIT_STRING
 xchain0_n35_α:
 mov qword ptr [r12 + 944], 1
 mov rax, qword ptr [rip + .Lx37_0]
 mov qword ptr [r12 + 952], rax
 jmp xchain0_n38_α
 xchain0_n35_β:
 jmp xchain0_n42_α
.Lx37_0:
 .quad .Lx37_0_s
.Lx37_0_s:
 .string ""
# IR_LIT_STRING
 xchain0_n36_α:
 mov qword ptr [r12 + 608], 1
 mov rax, qword ptr [rip + .Lx38_0]
 mov qword ptr [r12 + 616], rax
 jmp xchain0_n39_α
 xchain0_n36_β:
 jmp main_γ
.Lx38_0:
 .quad .Lx38_0_s
.Lx38_0_s:
 .string "FAIL 1114/002: item == bracket read"
 xchain0_n37_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+752] -> [zr+720]
 mov rax, qword ptr [r12 + 752]
 mov qword ptr [r12 + 720], rax
 mov rax, qword ptr [r12 + 760]
 mov qword ptr [r12 + 728], rax
# marshal arg1 = producer-box slot [zr+832] -> [zr+736]
 mov rax, qword ptr [r12 + 832]
 mov qword ptr [r12 + 736], rax
 mov rax, qword ptr [r12 + 840]
 mov qword ptr [r12 + 744], rax
  .section .rodata
  .Lbynamefn38: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn38]
 lea rsi, [r12 + 720]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 704], rax
 mov qword ptr [r12 + 712], rdx
 cmp eax, 99
 je xchain0_n24_α
 jmp xchain0_n40_α
 xchain0_n37_β:
 jmp xchain0_n24_α
 xchain0_n38_α:
# BOX IR_CALL SNO$WANTNM(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+944] -> [zr+976]
 mov rax, qword ptr [r12 + 944]
 mov qword ptr [r12 + 976], rax
 mov rax, qword ptr [r12 + 952]
 mov qword ptr [r12 + 984], rax
  .section .rodata
  .Lrkfn41: .string "SNO$WANTNM"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn41]
 lea rsi, [r12 + 976]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 960], rax
 mov qword ptr [r12 + 968], rdx
 cmp eax, 99
 je xchain0_n42_α
 jmp xchain0_n41_α
 xchain0_n38_β:
 jmp xchain0_n42_α
# IR_ASSIGN gva
 xchain0_n39_α:
 mov rax, qword ptr [r12 + 608]
 mov rdx, qword ptr [r12 + 616]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 592], rax
 mov qword ptr [r12 + 600], rdx
 jmp main_γ
 xchain0_n39_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n40_α:
 mov qword ptr [r12 + 864], 1
 mov rax, qword ptr [rip + .Lx43_0]
 mov qword ptr [r12 + 872], rax
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
 mov qword ptr [r12 + 1088], rax
 mov qword ptr [r12 + 1096], rdx
 jmp xchain0_n44_α
 xchain0_n41_β:
 jmp xchain0_n42_α
# IR_VAR
 xchain0_n42_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 1344], rax
 mov qword ptr [r12 + 1352], rdx
 jmp xchain0_n45_α
 xchain0_n42_β:
 jmp xchain0_n46_α
# IR_ASSIGN gva
 xchain0_n43_α:
 mov rax, qword ptr [r12 + 864]
 mov rdx, qword ptr [r12 + 872]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 848], rax
 mov qword ptr [r12 + 856], rdx
 jmp main_γ
 xchain0_n43_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n44_α:
 mov qword ptr [r12 + 1104], 6
 mov rax, qword ptr [rip + .Lx47_0]
 mov qword ptr [r12 + 1112], rax
 jmp xchain0_n47_α
 xchain0_n44_β:
 jmp xchain0_n42_α
.Lx47_0:
 .quad 1
# IR_LIT_INTEGER
 xchain0_n45_α:
 mov qword ptr [r12 + 1360], 6
 mov rax, qword ptr [rip + .Lx48_0]
 mov qword ptr [r12 + 1368], rax
 jmp xchain0_n48_α
 xchain0_n45_β:
 jmp xchain0_n46_α
.Lx48_0:
 .quad 1
# IR_VAR
 xchain0_n46_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 1520], rax
 mov qword ptr [r12 + 1528], rdx
 jmp xchain0_n49_α
 xchain0_n46_β:
 jmp xchain0_n50_α
# IR_LIT_INTEGER
 xchain0_n47_α:
 mov qword ptr [r12 + 1120], 6
 mov rax, qword ptr [rip + .Lx50_0]
 mov qword ptr [r12 + 1128], rax
 jmp xchain0_n51_α
 xchain0_n47_β:
 jmp xchain0_n42_α
.Lx50_0:
 .quad 2
# IR_LIT_INTEGER
 xchain0_n48_α:
 mov qword ptr [r12 + 1376], 6
 mov rax, qword ptr [rip + .Lx51_0]
 mov qword ptr [r12 + 1384], rax
 jmp xchain0_n52_α
 xchain0_n48_β:
 jmp xchain0_n46_α
.Lx51_0:
 .quad 2
# IR_LIT_INTEGER
 xchain0_n49_α:
 mov qword ptr [r12 + 1536], 6
 mov rax, qword ptr [rip + .Lx52_0]
 mov qword ptr [r12 + 1544], rax
 jmp xchain0_n53_α
 xchain0_n49_β:
 jmp xchain0_n50_α
.Lx52_0:
 .quad 1
# IR_VAR
 xchain0_n50_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 1728], rax
 mov qword ptr [r12 + 1736], rdx
 jmp xchain0_n54_α
 xchain0_n50_β:
 jmp xchain0_n55_α
# IR_LIT_INTEGER
 xchain0_n51_α:
 mov qword ptr [r12 + 1136], 6
 mov rax, qword ptr [rip + .Lx54_0]
 mov qword ptr [r12 + 1144], rax
 jmp xchain0_n56_α
 xchain0_n51_β:
 jmp xchain0_n42_α
.Lx54_0:
 .quad 1
# IR_LIT_INTEGER
 xchain0_n52_α:
 mov qword ptr [r12 + 1392], 6
 mov rax, qword ptr [rip + .Lx55_0]
 mov qword ptr [r12 + 1400], rax
 jmp xchain0_n57_α
 xchain0_n52_β:
 jmp xchain0_n46_α
.Lx55_0:
 .quad 1
# IR_SUBSCRIPT x[i] variable
 xchain0_n53_α:
 mov rdi, qword ptr [r12 + 1520]
 mov rsi, qword ptr [r12 + 1528]
 mov rdx, qword ptr [r12 + 1536]
 mov rcx, qword ptr [r12 + 1544]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n50_α
 mov qword ptr [r12 + 1552], rax
 mov qword ptr [r12 + 1560], rdx
 jmp xchain0_n58_α
 xchain0_n53_β:
 jmp xchain0_n50_α
# IR_LIT_INTEGER
 xchain0_n54_α:
 mov qword ptr [r12 + 1744], 6
 mov rax, qword ptr [rip + .Lx57_0]
 mov qword ptr [r12 + 1752], rax
 jmp xchain0_n59_α
 xchain0_n54_β:
 jmp xchain0_n55_α
.Lx57_0:
 .quad 2
# IR_VAR
 xchain0_n55_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 2048], rax
 mov qword ptr [r12 + 2056], rdx
 jmp xchain0_n60_α
 xchain0_n55_β:
 jmp xchain0_n61_α
# IR_LIT_INTEGER
 xchain0_n56_α:
 mov qword ptr [r12 + 1152], 6
 mov rax, qword ptr [rip + .Lx59_0]
 mov qword ptr [r12 + 1160], rax
 jmp xchain0_n62_α
 xchain0_n56_β:
 jmp xchain0_n42_α
.Lx59_0:
 .quad 2
# IR_LIT_INTEGER
 xchain0_n57_α:
 mov qword ptr [r12 + 1408], 6
 mov rax, qword ptr [rip + .Lx60_0]
 mov qword ptr [r12 + 1416], rax
 jmp xchain0_n63_α
 xchain0_n57_β:
 jmp xchain0_n46_α
.Lx60_0:
 .quad 2
# IR_LIT_INTEGER
 xchain0_n58_α:
 mov qword ptr [r12 + 1568], 6
 mov rax, qword ptr [rip + .Lx61_0]
 mov qword ptr [r12 + 1576], rax
 jmp xchain0_n64_α
 xchain0_n58_β:
 jmp xchain0_n50_α
.Lx61_0:
 .quad 2
# IR_SUBSCRIPT x[i] variable
 xchain0_n59_α:
 mov rdi, qword ptr [r12 + 1728]
 mov rsi, qword ptr [r12 + 1736]
 mov rdx, qword ptr [r12 + 1744]
 mov rcx, qword ptr [r12 + 1752]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n55_α
 mov qword ptr [r12 + 1760], rax
 mov qword ptr [r12 + 1768], rdx
 jmp xchain0_n65_α
 xchain0_n59_β:
 jmp xchain0_n55_α
# IR_LIT_INTEGER
 xchain0_n60_α:
 mov qword ptr [r12 + 2064], 6
 mov rax, qword ptr [rip + .Lx63_0]
 mov qword ptr [r12 + 2072], rax
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
 lea rsi, [r12 + 2208]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2192], rax
 mov qword ptr [r12 + 2200], rdx
 cmp eax, 99
 je xchain0_n68_α
 jmp xchain0_n67_α
 xchain0_n61_β:
 jmp xchain0_n68_α
 xchain0_n62_α:
# BOX CALL item(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1088] -> [zr+1008]
 mov rax, qword ptr [r12 + 1088]
 mov qword ptr [r12 + 1008], rax
 mov rax, qword ptr [r12 + 1096]
 mov qword ptr [r12 + 1016], rax
# marshal arg1 = producer-box slot [zr+1104] -> [zr+1024]
 mov rax, qword ptr [r12 + 1104]
 mov qword ptr [r12 + 1024], rax
 mov rax, qword ptr [r12 + 1112]
 mov qword ptr [r12 + 1032], rax
# marshal arg2 = producer-box slot [zr+1120] -> [zr+1040]
 mov rax, qword ptr [r12 + 1120]
 mov qword ptr [r12 + 1040], rax
 mov rax, qword ptr [r12 + 1128]
 mov qword ptr [r12 + 1048], rax
# marshal arg3 = producer-box slot [zr+1136] -> [zr+1056]
 mov rax, qword ptr [r12 + 1136]
 mov qword ptr [r12 + 1056], rax
 mov rax, qword ptr [r12 + 1144]
 mov qword ptr [r12 + 1064], rax
# marshal arg4 = producer-box slot [zr+1152] -> [zr+1072]
 mov rax, qword ptr [r12 + 1152]
 mov qword ptr [r12 + 1072], rax
 mov rax, qword ptr [r12 + 1160]
 mov qword ptr [r12 + 1080], rax
  .section .rodata
  .Lbynamefn63: .string "item"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn63]
 lea rsi, [r12 + 1008]
 mov edx, 5
 call rt_call_arr@PLT
 mov qword ptr [r12 + 992], rax
 mov qword ptr [r12 + 1000], rdx
 cmp eax, 99
 je xchain0_n42_α
 jmp xchain0_n69_α
 xchain0_n62_β:
 jmp xchain0_n42_α
 xchain0_n63_α:
# BOX CALL item(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1344] -> [zr+1264]
 mov rax, qword ptr [r12 + 1344]
 mov qword ptr [r12 + 1264], rax
 mov rax, qword ptr [r12 + 1352]
 mov qword ptr [r12 + 1272], rax
# marshal arg1 = producer-box slot [zr+1360] -> [zr+1280]
 mov rax, qword ptr [r12 + 1360]
 mov qword ptr [r12 + 1280], rax
 mov rax, qword ptr [r12 + 1368]
 mov qword ptr [r12 + 1288], rax
# marshal arg2 = producer-box slot [zr+1376] -> [zr+1296]
 mov rax, qword ptr [r12 + 1376]
 mov qword ptr [r12 + 1296], rax
 mov rax, qword ptr [r12 + 1384]
 mov qword ptr [r12 + 1304], rax
# marshal arg3 = producer-box slot [zr+1392] -> [zr+1312]
 mov rax, qword ptr [r12 + 1392]
 mov qword ptr [r12 + 1312], rax
 mov rax, qword ptr [r12 + 1400]
 mov qword ptr [r12 + 1320], rax
# marshal arg4 = producer-box slot [zr+1408] -> [zr+1328]
 mov rax, qword ptr [r12 + 1408]
 mov qword ptr [r12 + 1328], rax
 mov rax, qword ptr [r12 + 1416]
 mov qword ptr [r12 + 1336], rax
  .section .rodata
  .Lbynamefn64: .string "item"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn64]
 lea rsi, [r12 + 1264]
 mov edx, 5
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1248], rax
 mov qword ptr [r12 + 1256], rdx
 cmp eax, 99
 je xchain0_n46_α
 jmp xchain0_n70_α
 xchain0_n63_β:
 jmp xchain0_n46_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n64_α:
 mov rdi, qword ptr [r12 + 1552]
 mov rsi, qword ptr [r12 + 1560]
 mov rdx, qword ptr [r12 + 1568]
 mov rcx, qword ptr [r12 + 1576]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n50_α
 mov qword ptr [r12 + 1584], rax
 mov qword ptr [r12 + 1592], rdx
 jmp xchain0_n71_α
 xchain0_n64_β:
 jmp xchain0_n50_α
# IR_LIT_INTEGER
 xchain0_n65_α:
 mov qword ptr [r12 + 1776], 6
 mov rax, qword ptr [rip + .Lx69_0]
 mov qword ptr [r12 + 1784], rax
 jmp xchain0_n72_α
 xchain0_n65_β:
 jmp xchain0_n55_α
.Lx69_0:
 .quad 1
# IR_LIT_INTEGER
 xchain0_n66_α:
 mov qword ptr [r12 + 2080], 6
 mov rax, qword ptr [rip + .Lx70_0]
 mov qword ptr [r12 + 2088], rax
 jmp xchain0_n73_α
 xchain0_n66_β:
 jmp xchain0_n61_α
.Lx70_0:
 .quad 1
# IR_ASSIGN gva
 xchain0_n67_α:
 mov rax, qword ptr [r12 + 2192]
 mov rdx, qword ptr [r12 + 2200]
 mov qword ptr [rbx + 48], rax
 mov qword ptr [rbx + 56], rdx
 mov qword ptr [r12 + 2176], rax
 mov qword ptr [r12 + 2184], rdx
 jmp xchain0_n68_α
 xchain0_n67_β:
 jmp xchain0_n68_α
# IR_LIT_STRING
 xchain0_n68_α:
 mov qword ptr [r12 + 2208], 1
 mov rax, qword ptr [rip + .Lx72_0]
 mov qword ptr [r12 + 2216], rax
 jmp xchain0_n74_α
 xchain0_n68_β:
 jmp xchain0_n81_α
.Lx72_0:
 .quad .Lx72_0_s
.Lx72_0_s:
 .string ""
# IR_LIT_INTEGER
 xchain0_n69_α:
 mov qword ptr [r12 + 1168], 6
 mov rax, qword ptr [rip + .Lx73_0]
 mov qword ptr [r12 + 1176], rax
 jmp xchain0_n75_α
 xchain0_n69_β:
 jmp xchain0_n42_α
.Lx73_0:
 .quad 1212
# IR_LIT_INTEGER
 xchain0_n70_α:
 mov qword ptr [r12 + 1424], 6
 mov rax, qword ptr [rip + .Lx74_0]
 mov qword ptr [r12 + 1432], rax
 jmp xchain0_n76_α
 xchain0_n70_β:
 jmp xchain0_n46_α
.Lx74_0:
 .quad 1212
# IR_LIT_INTEGER
 xchain0_n71_α:
 mov qword ptr [r12 + 1600], 6
 mov rax, qword ptr [rip + .Lx75_0]
 mov qword ptr [r12 + 1608], rax
 jmp xchain0_n77_α
 xchain0_n71_β:
 jmp xchain0_n50_α
.Lx75_0:
 .quad 1
# IR_SUBSCRIPT x[i] variable
 xchain0_n72_α:
 mov rdi, qword ptr [r12 + 1760]
 mov rsi, qword ptr [r12 + 1768]
 mov rdx, qword ptr [r12 + 1776]
 mov rcx, qword ptr [r12 + 1784]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n55_α
 mov qword ptr [r12 + 1792], rax
 mov qword ptr [r12 + 1800], rdx
 jmp xchain0_n78_α
 xchain0_n72_β:
 jmp xchain0_n55_α
# IR_LIT_INTEGER
 xchain0_n73_α:
 mov qword ptr [r12 + 2096], 6
 mov rax, qword ptr [rip + .Lx77_0]
 mov qword ptr [r12 + 2104], rax
 jmp xchain0_n79_α
 xchain0_n73_β:
 jmp xchain0_n61_α
.Lx77_0:
 .quad 2
 xchain0_n74_α:
# BOX IR_CALL SNO$WANTNM(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2208] -> [zr+2240]
 mov rax, qword ptr [r12 + 2208]
 mov qword ptr [r12 + 2240], rax
 mov rax, qword ptr [r12 + 2216]
 mov qword ptr [r12 + 2248], rax
  .section .rodata
  .Lrkfn79: .string "SNO$WANTNM"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn79]
 lea rsi, [r12 + 2240]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2224], rax
 mov qword ptr [r12 + 2232], rdx
 cmp eax, 99
 je xchain0_n81_α
 jmp xchain0_n80_α
 xchain0_n74_β:
 jmp xchain0_n81_α
# IR_ASSIGN_VAR
 xchain0_n75_α:
 mov rdi, qword ptr [r12 + 992]
 mov rsi, qword ptr [r12 + 1000]
 mov rdx, qword ptr [r12 + 1168]
 mov rcx, qword ptr [r12 + 1176]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n42_α
 mov qword ptr [r12 + 1184], rax
 mov qword ptr [r12 + 1192], rdx
 jmp xchain0_n42_α
 xchain0_n75_β:
 jmp xchain0_n42_α
 xchain0_n76_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1248] -> [zr+1216]
 mov rax, qword ptr [r12 + 1248]
 mov qword ptr [r12 + 1216], rax
 mov rax, qword ptr [r12 + 1256]
 mov qword ptr [r12 + 1224], rax
# marshal arg1 = producer-box slot [zr+1424] -> [zr+1232]
 mov rax, qword ptr [r12 + 1424]
 mov qword ptr [r12 + 1232], rax
 mov rax, qword ptr [r12 + 1432]
 mov qword ptr [r12 + 1240], rax
  .section .rodata
  .Lbynamefn77: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn77]
 lea rsi, [r12 + 1216]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1200], rax
 mov qword ptr [r12 + 1208], rdx
 cmp eax, 99
 je xchain0_n46_α
 jmp xchain0_n82_α
 xchain0_n76_β:
 jmp xchain0_n46_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n77_α:
 mov rdi, qword ptr [r12 + 1584]
 mov rsi, qword ptr [r12 + 1592]
 mov rdx, qword ptr [r12 + 1600]
 mov rcx, qword ptr [r12 + 1608]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n50_α
 mov qword ptr [r12 + 1616], rax
 mov qword ptr [r12 + 1624], rdx
 jmp xchain0_n83_α
 xchain0_n77_β:
 jmp xchain0_n50_α
# IR_LIT_INTEGER
 xchain0_n78_α:
 mov qword ptr [r12 + 1808], 6
 mov rax, qword ptr [rip + .Lx83_0]
 mov qword ptr [r12 + 1816], rax
 jmp xchain0_n84_α
 xchain0_n78_β:
 jmp xchain0_n55_α
.Lx83_0:
 .quad 2
# IR_LIT_INTEGER
 xchain0_n79_α:
 mov qword ptr [r12 + 2112], 6
 mov rax, qword ptr [rip + .Lx84_0]
 mov qword ptr [r12 + 2120], rax
 jmp xchain0_n85_α
 xchain0_n79_β:
 jmp xchain0_n61_α
.Lx84_0:
 .quad 1
# IR_VAR
 xchain0_n80_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 2304], rax
 mov qword ptr [r12 + 2312], rdx
 jmp xchain0_n86_α
 xchain0_n80_β:
 jmp xchain0_n81_α
# IR_VAR
 xchain0_n81_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 2464], rax
 mov qword ptr [r12 + 2472], rdx
 jmp xchain0_n87_α
 xchain0_n81_β:
 jmp xchain0_n88_α
# IR_LIT_STRING
 xchain0_n82_α:
 mov qword ptr [r12 + 1456], 1
 mov rax, qword ptr [rip + .Lx87_0]
 mov qword ptr [r12 + 1464], rax
 jmp xchain0_n89_α
 xchain0_n82_β:
 jmp main_γ
.Lx87_0:
 .quad .Lx87_0_s
.Lx87_0_s:
 .string "FAIL 1114/004: item 4D assign/read"
# IR_LIT_INTEGER
 xchain0_n83_α:
 mov qword ptr [r12 + 1632], 6
 mov rax, qword ptr [rip + .Lx88_0]
 mov qword ptr [r12 + 1640], rax
 jmp xchain0_n90_α
 xchain0_n83_β:
 jmp xchain0_n50_α
.Lx88_0:
 .quad 2
# IR_SUBSCRIPT x[i] variable
 xchain0_n84_α:
 mov rdi, qword ptr [r12 + 1792]
 mov rsi, qword ptr [r12 + 1800]
 mov rdx, qword ptr [r12 + 1808]
 mov rcx, qword ptr [r12 + 1816]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n55_α
 mov qword ptr [r12 + 1824], rax
 mov qword ptr [r12 + 1832], rdx
 jmp xchain0_n91_α
 xchain0_n84_β:
 jmp xchain0_n55_α
 xchain0_n85_α:
# BOX CALL item(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2048] -> [zr+1968]
 mov rax, qword ptr [r12 + 2048]
 mov qword ptr [r12 + 1968], rax
 mov rax, qword ptr [r12 + 2056]
 mov qword ptr [r12 + 1976], rax
# marshal arg1 = producer-box slot [zr+2064] -> [zr+1984]
 mov rax, qword ptr [r12 + 2064]
 mov qword ptr [r12 + 1984], rax
 mov rax, qword ptr [r12 + 2072]
 mov qword ptr [r12 + 1992], rax
# marshal arg2 = producer-box slot [zr+2080] -> [zr+2000]
 mov rax, qword ptr [r12 + 2080]
 mov qword ptr [r12 + 2000], rax
 mov rax, qword ptr [r12 + 2088]
 mov qword ptr [r12 + 2008], rax
# marshal arg3 = producer-box slot [zr+2096] -> [zr+2016]
 mov rax, qword ptr [r12 + 2096]
 mov qword ptr [r12 + 2016], rax
 mov rax, qword ptr [r12 + 2104]
 mov qword ptr [r12 + 2024], rax
# marshal arg4 = producer-box slot [zr+2112] -> [zr+2032]
 mov rax, qword ptr [r12 + 2112]
 mov qword ptr [r12 + 2032], rax
 mov rax, qword ptr [r12 + 2120]
 mov qword ptr [r12 + 2040], rax
  .section .rodata
  .Lbynamefn86: .string "item"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn86]
 lea rsi, [r12 + 1968]
 mov edx, 5
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1952], rax
 mov qword ptr [r12 + 1960], rdx
 cmp eax, 99
 je xchain0_n61_α
 jmp xchain0_n92_α
 xchain0_n85_β:
 jmp xchain0_n61_α
# IR_LIT_STRING
 xchain0_n86_α:
 mov qword ptr [r12 + 2320], 1
 mov rax, qword ptr [rip + .Lx91_0]
 mov qword ptr [r12 + 2328], rax
 jmp xchain0_n93_α
 xchain0_n86_β:
 jmp xchain0_n81_α
.Lx91_0:
 .quad .Lx91_0_s
.Lx91_0_s:
 .string "key"
# IR_LIT_STRING
 xchain0_n87_α:
 mov qword ptr [r12 + 2480], 1
 mov rax, qword ptr [rip + .Lx92_0]
 mov qword ptr [r12 + 2488], rax
 jmp xchain0_n94_α
 xchain0_n87_β:
 jmp xchain0_n88_α
.Lx92_0:
 .quad .Lx92_0_s
.Lx92_0_s:
 .string "key"
# IR_LIT_STRING
 xchain0_n88_α:
 mov qword ptr [r12 + 2560], 1
 mov rax, qword ptr [rip + .Lx93_0]
 mov qword ptr [r12 + 2568], rax
 jmp xchain0_n95_α
 xchain0_n88_β:
 jmp main_γ
.Lx93_0:
 .quad .Lx93_0_s
.Lx93_0_s:
 .string "PASS 1114_item (7/7)"
# IR_ASSIGN gva
 xchain0_n89_α:
 mov rax, qword ptr [r12 + 1456]
 mov rdx, qword ptr [r12 + 1464]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1440], rax
 mov qword ptr [r12 + 1448], rdx
 jmp main_γ
 xchain0_n89_β:
 jmp main_γ
# IR_SUBSCRIPT x[i] variable
 xchain0_n90_α:
 mov rdi, qword ptr [r12 + 1616]
 mov rsi, qword ptr [r12 + 1624]
 mov rdx, qword ptr [r12 + 1632]
 mov rcx, qword ptr [r12 + 1640]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n50_α
 mov qword ptr [r12 + 1648], rax
 mov qword ptr [r12 + 1656], rdx
 jmp xchain0_n96_α
 xchain0_n90_β:
 jmp xchain0_n50_α
# IR_LIT_INTEGER
 xchain0_n91_α:
 mov qword ptr [r12 + 1840], 6
 mov rax, qword ptr [rip + .Lx96_0]
 mov qword ptr [r12 + 1848], rax
 jmp xchain0_n97_α
 xchain0_n91_β:
 jmp xchain0_n55_α
.Lx96_0:
 .quad 1
# IR_LIT_INTEGER
 xchain0_n92_α:
 mov qword ptr [r12 + 2128], 6
 mov rax, qword ptr [rip + .Lx97_0]
 mov qword ptr [r12 + 2136], rax
 jmp xchain0_n98_α
 xchain0_n92_β:
 jmp xchain0_n61_α
.Lx97_0:
 .quad 2121
 xchain0_n93_α:
# BOX CALL item(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2304] -> [zr+2272]
 mov rax, qword ptr [r12 + 2304]
 mov qword ptr [r12 + 2272], rax
 mov rax, qword ptr [r12 + 2312]
 mov qword ptr [r12 + 2280], rax
# marshal arg1 = producer-box slot [zr+2320] -> [zr+2288]
 mov rax, qword ptr [r12 + 2320]
 mov qword ptr [r12 + 2288], rax
 mov rax, qword ptr [r12 + 2328]
 mov qword ptr [r12 + 2296], rax
  .section .rodata
  .Lbynamefn94: .string "item"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn94]
 lea rsi, [r12 + 2272]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2256], rax
 mov qword ptr [r12 + 2264], rdx
 cmp eax, 99
 je xchain0_n81_α
 jmp xchain0_n99_α
 xchain0_n93_β:
 jmp xchain0_n81_α
 xchain0_n94_α:
# BOX CALL item(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2464] -> [zr+2432]
 mov rax, qword ptr [r12 + 2464]
 mov qword ptr [r12 + 2432], rax
 mov rax, qword ptr [r12 + 2472]
 mov qword ptr [r12 + 2440], rax
# marshal arg1 = producer-box slot [zr+2480] -> [zr+2448]
 mov rax, qword ptr [r12 + 2480]
 mov qword ptr [r12 + 2448], rax
 mov rax, qword ptr [r12 + 2488]
 mov qword ptr [r12 + 2456], rax
  .section .rodata
  .Lbynamefn95: .string "item"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn95]
 lea rsi, [r12 + 2432]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2416], rax
 mov qword ptr [r12 + 2424], rdx
 cmp eax, 99
 je xchain0_n88_α
 jmp xchain0_n100_α
 xchain0_n94_β:
 jmp xchain0_n88_α
# IR_ASSIGN gva
 xchain0_n95_α:
 mov rax, qword ptr [r12 + 2560]
 mov rdx, qword ptr [r12 + 2568]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 2544], rax
 mov qword ptr [r12 + 2552], rdx
 jmp main_γ
 xchain0_n95_β:
 jmp main_γ
# IR_DEREF variable -> value
 xchain0_n96_α:
 mov rdi, qword ptr [r12 + 1648]
 mov rsi, qword ptr [r12 + 1656]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n50_α
 mov qword ptr [r12 + 1664], rax
 mov qword ptr [r12 + 1672], rdx
 jmp xchain0_n101_α
 xchain0_n96_β:
 jmp xchain0_n50_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n97_α:
 mov rdi, qword ptr [r12 + 1824]
 mov rsi, qword ptr [r12 + 1832]
 mov rdx, qword ptr [r12 + 1840]
 mov rcx, qword ptr [r12 + 1848]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n55_α
 mov qword ptr [r12 + 1856], rax
 mov qword ptr [r12 + 1864], rdx
 jmp xchain0_n102_α
 xchain0_n97_β:
 jmp xchain0_n55_α
 xchain0_n98_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1952] -> [zr+1920]
 mov rax, qword ptr [r12 + 1952]
 mov qword ptr [r12 + 1920], rax
 mov rax, qword ptr [r12 + 1960]
 mov qword ptr [r12 + 1928], rax
# marshal arg1 = producer-box slot [zr+2128] -> [zr+1936]
 mov rax, qword ptr [r12 + 2128]
 mov qword ptr [r12 + 1936], rax
 mov rax, qword ptr [r12 + 2136]
 mov qword ptr [r12 + 1944], rax
  .section .rodata
  .Lbynamefn99: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn99]
 lea rsi, [r12 + 1920]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1904], rax
 mov qword ptr [r12 + 1912], rdx
 cmp eax, 99
 je xchain0_n61_α
 jmp xchain0_n103_α
 xchain0_n98_β:
 jmp xchain0_n61_α
# IR_LIT_STRING
 xchain0_n99_α:
 mov qword ptr [r12 + 2336], 1
 mov rax, qword ptr [rip + .Lx104_0]
 mov qword ptr [r12 + 2344], rax
 jmp xchain0_n104_α
 xchain0_n99_β:
 jmp xchain0_n81_α
.Lx104_0:
 .quad .Lx104_0_s
.Lx104_0_s:
 .string "val"
# IR_LIT_STRING
 xchain0_n100_α:
 mov qword ptr [r12 + 2496], 1
 mov rax, qword ptr [rip + .Lx105_0]
 mov qword ptr [r12 + 2504], rax
 jmp xchain0_n105_α
 xchain0_n100_β:
 jmp xchain0_n88_α
.Lx105_0:
 .quad .Lx105_0_s
.Lx105_0_s:
 .string "val"
# IR_LIT_INTEGER
 xchain0_n101_α:
 mov qword ptr [r12 + 1680], 6
 mov rax, qword ptr [rip + .Lx106_0]
 mov qword ptr [r12 + 1688], rax
 jmp xchain0_n106_α
 xchain0_n101_β:
 jmp xchain0_n50_α
.Lx106_0:
 .quad 1212
# IR_LIT_INTEGER
 xchain0_n102_α:
 mov qword ptr [r12 + 1872], 6
 mov rax, qword ptr [rip + .Lx107_0]
 mov qword ptr [r12 + 1880], rax
 jmp xchain0_n107_α
 xchain0_n102_β:
 jmp xchain0_n55_α
.Lx107_0:
 .quad 2121
# IR_LIT_STRING
 xchain0_n103_α:
 mov qword ptr [r12 + 2160], 1
 mov rax, qword ptr [rip + .Lx108_0]
 mov qword ptr [r12 + 2168], rax
 jmp xchain0_n108_α
 xchain0_n103_β:
 jmp main_γ
.Lx108_0:
 .quad .Lx108_0_s
.Lx108_0_s:
 .string "FAIL 1114/006: bracket 4D assign, item read"
# IR_ASSIGN_VAR
 xchain0_n104_α:
 mov rdi, qword ptr [r12 + 2256]
 mov rsi, qword ptr [r12 + 2264]
 mov rdx, qword ptr [r12 + 2336]
 mov rcx, qword ptr [r12 + 2344]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n81_α
 mov qword ptr [r12 + 2352], rax
 mov qword ptr [r12 + 2360], rdx
 jmp xchain0_n81_α
 xchain0_n104_β:
 jmp xchain0_n81_α
 xchain0_n105_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2416] -> [zr+2384]
 mov rax, qword ptr [r12 + 2416]
 mov qword ptr [r12 + 2384], rax
 mov rax, qword ptr [r12 + 2424]
 mov qword ptr [r12 + 2392], rax
# marshal arg1 = producer-box slot [zr+2496] -> [zr+2400]
 mov rax, qword ptr [r12 + 2496]
 mov qword ptr [r12 + 2400], rax
 mov rax, qword ptr [r12 + 2504]
 mov qword ptr [r12 + 2408], rax
  .section .rodata
  .Lbynamefn106: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn106]
 lea rsi, [r12 + 2384]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2368], rax
 mov qword ptr [r12 + 2376], rdx
 cmp eax, 99
 je xchain0_n88_α
 jmp xchain0_n109_α
 xchain0_n105_β:
 jmp xchain0_n88_α
 xchain0_n106_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1664] -> [zr+1488]
 mov rax, qword ptr [r12 + 1664]
 mov qword ptr [r12 + 1488], rax
 mov rax, qword ptr [r12 + 1672]
 mov qword ptr [r12 + 1496], rax
# marshal arg1 = producer-box slot [zr+1680] -> [zr+1504]
 mov rax, qword ptr [r12 + 1680]
 mov qword ptr [r12 + 1504], rax
 mov rax, qword ptr [r12 + 1688]
 mov qword ptr [r12 + 1512], rax
  .section .rodata
  .Lbynamefn107: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn107]
 lea rsi, [r12 + 1488]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1472], rax
 mov qword ptr [r12 + 1480], rdx
 cmp eax, 99
 je xchain0_n50_α
 jmp xchain0_n110_α
 xchain0_n106_β:
 jmp xchain0_n50_α
# IR_ASSIGN_VAR
 xchain0_n107_α:
 mov rdi, qword ptr [r12 + 1856]
 mov rsi, qword ptr [r12 + 1864]
 mov rdx, qword ptr [r12 + 1872]
 mov rcx, qword ptr [r12 + 1880]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n55_α
 mov qword ptr [r12 + 1888], rax
 mov qword ptr [r12 + 1896], rdx
 jmp xchain0_n55_α
 xchain0_n107_β:
 jmp xchain0_n55_α
# IR_ASSIGN gva
 xchain0_n108_α:
 mov rax, qword ptr [r12 + 2160]
 mov rdx, qword ptr [r12 + 2168]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 2144], rax
 mov qword ptr [r12 + 2152], rdx
 jmp main_γ
 xchain0_n108_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n109_α:
 mov qword ptr [r12 + 2528], 1
 mov rax, qword ptr [rip + .Lx114_0]
 mov qword ptr [r12 + 2536], rax
 jmp xchain0_n111_α
 xchain0_n109_β:
 jmp main_γ
.Lx114_0:
 .quad .Lx114_0_s
.Lx114_0_s:
 .string "FAIL 1114/007: item on table"
# IR_LIT_STRING
 xchain0_n110_α:
 mov qword ptr [r12 + 1712], 1
 mov rax, qword ptr [rip + .Lx115_0]
 mov qword ptr [r12 + 1720], rax
 jmp xchain0_n112_α
 xchain0_n110_β:
 jmp main_γ
.Lx115_0:
 .quad .Lx115_0_s
.Lx115_0_s:
 .string "FAIL 1114/005: item 4D == bracket"
# IR_ASSIGN gva
 xchain0_n111_α:
 mov rax, qword ptr [r12 + 2528]
 mov rdx, qword ptr [r12 + 2536]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 2512], rax
 mov qword ptr [r12 + 2520], rdx
 jmp main_γ
 xchain0_n111_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain0_n112_α:
 mov rax, qword ptr [r12 + 1712]
 mov rdx, qword ptr [r12 + 1720]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1696], rax
 mov qword ptr [r12 + 1704], rdx
 jmp main_γ
 xchain0_n112_β:
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
 mov rdi, qword ptr [r12 + 2576]
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

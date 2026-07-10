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
 mov qword ptr [r12 + 1824], rax
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
# BOX IR_CALL table(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+112] -> [zr+96]
 mov rax, qword ptr [r12 + 112]
 mov qword ptr [r12 + 96], rax
 mov rax, qword ptr [r12 + 120]
 mov qword ptr [r12 + 104], rax
  .section .rodata
  .Lrkfn3: .string "table"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn3]
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
# IR_VAR
 xchain0_n3_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n5_α
# IR_LIT_STRING
 xchain0_n4_α:
 mov qword ptr [r12 + 176], 1
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 184], rax
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
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n8_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n6_α:
 mov rdi, qword ptr [r12 + 160]
 mov rsi, qword ptr [r12 + 168]
 mov rdx, qword ptr [r12 + 176]
 mov rcx, qword ptr [r12 + 184]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n5_α
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 jmp xchain0_n9_α
 xchain0_n6_β:
 jmp xchain0_n5_α
# IR_LIT_STRING
 xchain0_n7_α:
 mov qword ptr [r12 + 272], 1
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 280], rax
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
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 jmp xchain0_n11_α
 xchain0_n8_β:
 jmp xchain0_n12_α
# IR_DEREF variable -> value
 xchain0_n9_α:
 mov rdi, qword ptr [r12 + 192]
 mov rsi, qword ptr [r12 + 200]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n5_α
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 jmp xchain0_n13_α
 xchain0_n9_β:
 jmp xchain0_n5_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n10_α:
 mov rdi, qword ptr [r12 + 256]
 mov rsi, qword ptr [r12 + 264]
 mov rdx, qword ptr [r12 + 272]
 mov rcx, qword ptr [r12 + 280]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n8_α
 mov qword ptr [r12 + 288], rax
 mov qword ptr [r12 + 296], rdx
 jmp xchain0_n14_α
 xchain0_n10_β:
 jmp xchain0_n8_α
# IR_LIT_STRING
 xchain0_n11_α:
 mov qword ptr [r12 + 400], 1
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [r12 + 408], rax
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
 mov qword ptr [r12 + 496], rax
 mov qword ptr [r12 + 504], rdx
 jmp xchain0_n16_α
 xchain0_n12_β:
 jmp xchain0_n17_α
 xchain0_n13_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+208] -> [zr+144]
 mov rax, qword ptr [r12 + 208]
 mov qword ptr [r12 + 144], rax
 mov rax, qword ptr [r12 + 216]
 mov qword ptr [r12 + 152], rax
  .section .rodata
  .Lbynamefn14: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn14]
 lea rsi, [r12 + 144]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
 cmp eax, 99
 je xchain0_n5_α
 jmp xchain0_n18_α
 xchain0_n13_β:
 jmp xchain0_n5_α
# IR_LIT_STRING
 xchain0_n14_α:
 mov qword ptr [r12 + 304], 1
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [r12 + 312], rax
 jmp xchain0_n19_α
 xchain0_n14_β:
 jmp xchain0_n8_α
.Lx16_0:
 .quad .Lx16_0_s
.Lx16_0_s:
 .string "dog"
# IR_SUBSCRIPT x[i] variable
 xchain0_n15_α:
 mov rdi, qword ptr [r12 + 384]
 mov rsi, qword ptr [r12 + 392]
 mov rdx, qword ptr [r12 + 400]
 mov rcx, qword ptr [r12 + 408]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n12_α
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 jmp xchain0_n20_α
 xchain0_n15_β:
 jmp xchain0_n12_α
# IR_LIT_INTEGER
 xchain0_n16_α:
 mov qword ptr [r12 + 512], 6
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [r12 + 520], rax
 jmp xchain0_n21_α
 xchain0_n16_β:
 jmp xchain0_n17_α
.Lx18_0:
 .quad 7
# IR_VAR
 xchain0_n17_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 624], rax
 mov qword ptr [r12 + 632], rdx
 jmp xchain0_n22_α
 xchain0_n17_β:
 jmp xchain0_n23_α
# IR_LIT_STRING
 xchain0_n18_α:
 mov qword ptr [r12 + 240], 1
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [r12 + 248], rax
 jmp xchain0_n24_α
 xchain0_n18_β:
 jmp main_γ
.Lx20_0:
 .quad .Lx20_0_s
.Lx20_0_s:
 .string "FAIL 1113/001: absent key is null"
# IR_ASSIGN_VAR
 xchain0_n19_α:
 mov rdi, qword ptr [r12 + 288]
 mov rsi, qword ptr [r12 + 296]
 mov rdx, qword ptr [r12 + 304]
 mov rcx, qword ptr [r12 + 312]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n8_α
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 jmp xchain0_n8_α
 xchain0_n19_β:
 jmp xchain0_n8_α
# IR_DEREF variable -> value
 xchain0_n20_α:
 mov rdi, qword ptr [r12 + 416]
 mov rsi, qword ptr [r12 + 424]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n12_α
 mov qword ptr [r12 + 432], rax
 mov qword ptr [r12 + 440], rdx
 jmp xchain0_n25_α
 xchain0_n20_β:
 jmp xchain0_n12_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n21_α:
 mov rdi, qword ptr [r12 + 496]
 mov rsi, qword ptr [r12 + 504]
 mov rdx, qword ptr [r12 + 512]
 mov rcx, qword ptr [r12 + 520]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n17_α
 mov qword ptr [r12 + 528], rax
 mov qword ptr [r12 + 536], rdx
 jmp xchain0_n26_α
 xchain0_n21_β:
 jmp xchain0_n17_α
# IR_LIT_INTEGER
 xchain0_n22_α:
 mov qword ptr [r12 + 640], 6
 mov rax, qword ptr [rip + .Lx24_0]
 mov qword ptr [r12 + 648], rax
 jmp xchain0_n27_α
 xchain0_n22_β:
 jmp xchain0_n23_α
.Lx24_0:
 .quad 7
# IR_VAR
 xchain0_n23_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 784], rax
 mov qword ptr [r12 + 792], rdx
 jmp xchain0_n28_α
 xchain0_n23_β:
 jmp xchain0_n29_α
# IR_ASSIGN gva
 xchain0_n24_α:
 mov rax, qword ptr [r12 + 240]
 mov rdx, qword ptr [r12 + 248]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 jmp main_γ
 xchain0_n24_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n25_α:
 mov qword ptr [r12 + 448], 1
 mov rax, qword ptr [rip + .Lx27_0]
 mov qword ptr [r12 + 456], rax
 jmp xchain0_n30_α
 xchain0_n25_β:
 jmp xchain0_n12_α
.Lx27_0:
 .quad .Lx27_0_s
.Lx27_0_s:
 .string "dog"
# IR_LIT_INTEGER
 xchain0_n26_α:
 mov qword ptr [r12 + 544], 6
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [r12 + 552], rax
 jmp xchain0_n31_α
 xchain0_n26_β:
 jmp xchain0_n17_α
.Lx28_0:
 .quad 45
# IR_SUBSCRIPT x[i] variable
 xchain0_n27_α:
 mov rdi, qword ptr [r12 + 624]
 mov rsi, qword ptr [r12 + 632]
 mov rdx, qword ptr [r12 + 640]
 mov rcx, qword ptr [r12 + 648]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n23_α
 mov qword ptr [r12 + 656], rax
 mov qword ptr [r12 + 664], rdx
 jmp xchain0_n32_α
 xchain0_n27_β:
 jmp xchain0_n23_α
# IR_LIT_STRING
 xchain0_n28_α:
 mov qword ptr [r12 + 800], 1
 mov rax, qword ptr [rip + .Lx30_0]
 mov qword ptr [r12 + 808], rax
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
 mov qword ptr [r12 + 960], rax
 mov qword ptr [r12 + 968], rdx
 jmp xchain0_n34_α
 xchain0_n29_β:
 jmp xchain0_n35_α
 xchain0_n30_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+432] -> [zr+352]
 mov rax, qword ptr [r12 + 432]
 mov qword ptr [r12 + 352], rax
 mov rax, qword ptr [r12 + 440]
 mov qword ptr [r12 + 360], rax
# marshal arg1 = producer-box slot [zr+448] -> [zr+368]
 mov rax, qword ptr [r12 + 448]
 mov qword ptr [r12 + 368], rax
 mov rax, qword ptr [r12 + 456]
 mov qword ptr [r12 + 376], rax
  .section .rodata
  .Lbynamefn31: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn31]
 lea rsi, [r12 + 352]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 cmp eax, 99
 je xchain0_n12_α
 jmp xchain0_n36_α
 xchain0_n30_β:
 jmp xchain0_n12_α
# IR_ASSIGN_VAR
 xchain0_n31_α:
 mov rdi, qword ptr [r12 + 528]
 mov rsi, qword ptr [r12 + 536]
 mov rdx, qword ptr [r12 + 544]
 mov rcx, qword ptr [r12 + 552]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n17_α
 mov qword ptr [r12 + 560], rax
 mov qword ptr [r12 + 568], rdx
 jmp xchain0_n17_α
 xchain0_n31_β:
 jmp xchain0_n17_α
# IR_DEREF variable -> value
 xchain0_n32_α:
 mov rdi, qword ptr [r12 + 656]
 mov rsi, qword ptr [r12 + 664]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n23_α
 mov qword ptr [r12 + 672], rax
 mov qword ptr [r12 + 680], rdx
 jmp xchain0_n37_α
 xchain0_n32_β:
 jmp xchain0_n23_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n33_α:
 mov rdi, qword ptr [r12 + 784]
 mov rsi, qword ptr [r12 + 792]
 mov rdx, qword ptr [r12 + 800]
 mov rcx, qword ptr [r12 + 808]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n29_α
 mov qword ptr [r12 + 816], rax
 mov qword ptr [r12 + 824], rdx
 jmp xchain0_n38_α
 xchain0_n33_β:
 jmp xchain0_n29_α
# IR_LIT_STRING
 xchain0_n34_α:
 mov qword ptr [r12 + 976], 1
 mov rax, qword ptr [rip + .Lx36_0]
 mov qword ptr [r12 + 984], rax
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
 mov qword ptr [r12 + 1072], rax
 mov qword ptr [r12 + 1080], rdx
 jmp xchain0_n40_α
 xchain0_n35_β:
 jmp xchain0_n41_α
# IR_LIT_STRING
 xchain0_n36_α:
 mov qword ptr [r12 + 480], 1
 mov rax, qword ptr [rip + .Lx38_0]
 mov qword ptr [r12 + 488], rax
 jmp xchain0_n42_α
 xchain0_n36_β:
 jmp main_γ
.Lx38_0:
 .quad .Lx38_0_s
.Lx38_0_s:
 .string "FAIL 1113/002: string key assign/read"
# IR_LIT_INTEGER
 xchain0_n37_α:
 mov qword ptr [r12 + 688], 6
 mov rax, qword ptr [rip + .Lx39_0]
 mov qword ptr [r12 + 696], rax
 jmp xchain0_n43_α
 xchain0_n37_β:
 jmp xchain0_n23_α
.Lx39_0:
 .quad 45
# IR_DEREF variable -> value
 xchain0_n38_α:
 mov rdi, qword ptr [r12 + 816]
 mov rsi, qword ptr [r12 + 824]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n29_α
 mov qword ptr [r12 + 832], rax
 mov qword ptr [r12 + 840], rdx
 jmp xchain0_n44_α
 xchain0_n38_β:
 jmp xchain0_n29_α
 xchain0_n39_α:
# BOX CALL convert(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+960] -> [zr+928]
 mov rax, qword ptr [r12 + 960]
 mov qword ptr [r12 + 928], rax
 mov rax, qword ptr [r12 + 968]
 mov qword ptr [r12 + 936], rax
# marshal arg1 = producer-box slot [zr+976] -> [zr+944]
 mov rax, qword ptr [r12 + 976]
 mov qword ptr [r12 + 944], rax
 mov rax, qword ptr [r12 + 984]
 mov qword ptr [r12 + 952], rax
  .section .rodata
  .Lbynamefn40: .string "convert"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn40]
 lea rsi, [r12 + 928]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 912], rax
 mov qword ptr [r12 + 920], rdx
 cmp eax, 99
 je xchain0_n35_α
 jmp xchain0_n45_α
 xchain0_n39_β:
 jmp xchain0_n35_α
 xchain0_n40_α:
# BOX CALL prototype(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1072] -> [zr+1056]
 mov rax, qword ptr [r12 + 1072]
 mov qword ptr [r12 + 1056], rax
 mov rax, qword ptr [r12 + 1080]
 mov qword ptr [r12 + 1064], rax
  .section .rodata
  .Lbynamefn41: .string "prototype"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn41]
 lea rsi, [r12 + 1056]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1040], rax
 mov qword ptr [r12 + 1048], rdx
 cmp eax, 99
 je xchain0_n41_α
 jmp xchain0_n46_α
 xchain0_n40_β:
 jmp xchain0_n41_α
# IR_VAR
 xchain0_n41_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 1200], rax
 mov qword ptr [r12 + 1208], rdx
 jmp xchain0_n47_α
 xchain0_n41_β:
 jmp xchain0_n48_α
# IR_ASSIGN gva
 xchain0_n42_α:
 mov rax, qword ptr [r12 + 480]
 mov rdx, qword ptr [r12 + 488]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 464], rax
 mov qword ptr [r12 + 472], rdx
 jmp main_γ
 xchain0_n42_β:
 jmp main_γ
 xchain0_n43_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+672] -> [zr+592]
 mov rax, qword ptr [r12 + 672]
 mov qword ptr [r12 + 592], rax
 mov rax, qword ptr [r12 + 680]
 mov qword ptr [r12 + 600], rax
# marshal arg1 = producer-box slot [zr+688] -> [zr+608]
 mov rax, qword ptr [r12 + 688]
 mov qword ptr [r12 + 608], rax
 mov rax, qword ptr [r12 + 696]
 mov qword ptr [r12 + 616], rax
  .section .rodata
  .Lbynamefn44: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn44]
 lea rsi, [r12 + 592]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 576], rax
 mov qword ptr [r12 + 584], rdx
 cmp eax, 99
 je xchain0_n23_α
 jmp xchain0_n49_α
 xchain0_n43_β:
 jmp xchain0_n23_α
# IR_LIT_STRING
 xchain0_n44_α:
 mov qword ptr [r12 + 848], 1
 mov rax, qword ptr [rip + .Lx46_0]
 mov qword ptr [r12 + 856], rax
 jmp xchain0_n50_α
 xchain0_n44_β:
 jmp xchain0_n29_α
.Lx46_0:
 .quad .Lx46_0_s
.Lx46_0_s:
 .string "dog"
# IR_ASSIGN gva
 xchain0_n45_α:
 mov rax, qword ptr [r12 + 912]
 mov rdx, qword ptr [r12 + 920]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [r12 + 896], rax
 mov qword ptr [r12 + 904], rdx
 jmp xchain0_n35_α
 xchain0_n45_β:
 jmp xchain0_n35_α
# IR_LIT_STRING
 xchain0_n46_α:
 mov qword ptr [r12 + 1088], 1
 mov rax, qword ptr [rip + .Lx48_0]
 mov qword ptr [r12 + 1096], rax
 jmp xchain0_n51_α
 xchain0_n46_β:
 jmp xchain0_n41_α
.Lx48_0:
 .quad .Lx48_0_s
.Lx48_0_s:
 .string "2,2"
# IR_LIT_STRING
 xchain0_n47_α:
 mov qword ptr [r12 + 1216], 1
 mov rax, qword ptr [rip + .Lx49_0]
 mov qword ptr [r12 + 1224], rax
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
 mov qword ptr [r12 + 1280], rax
 mov qword ptr [r12 + 1288], rdx
 jmp xchain0_n53_α
 xchain0_n48_β:
 jmp xchain0_n54_α
# IR_LIT_STRING
 xchain0_n49_α:
 mov qword ptr [r12 + 720], 1
 mov rax, qword ptr [rip + .Lx51_0]
 mov qword ptr [r12 + 728], rax
 jmp xchain0_n55_α
 xchain0_n49_β:
 jmp main_γ
.Lx51_0:
 .quad .Lx51_0_s
.Lx51_0_s:
 .string "FAIL 1113/003: integer key assign/read"
 xchain0_n50_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+832] -> [zr+752]
 mov rax, qword ptr [r12 + 832]
 mov qword ptr [r12 + 752], rax
 mov rax, qword ptr [r12 + 840]
 mov qword ptr [r12 + 760], rax
# marshal arg1 = producer-box slot [zr+848] -> [zr+768]
 mov rax, qword ptr [r12 + 848]
 mov qword ptr [r12 + 768], rax
 mov rax, qword ptr [r12 + 856]
 mov qword ptr [r12 + 776], rax
  .section .rodata
  .Lbynamefn51: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn51]
 lea rsi, [r12 + 752]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 736], rax
 mov qword ptr [r12 + 744], rdx
 cmp eax, 99
 je xchain0_n29_α
 jmp xchain0_n56_α
 xchain0_n50_β:
 jmp xchain0_n29_α
 xchain0_n51_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1040] -> [zr+1008]
 mov rax, qword ptr [r12 + 1040]
 mov qword ptr [r12 + 1008], rax
 mov rax, qword ptr [r12 + 1048]
 mov qword ptr [r12 + 1016], rax
# marshal arg1 = producer-box slot [zr+1088] -> [zr+1024]
 mov rax, qword ptr [r12 + 1088]
 mov qword ptr [r12 + 1024], rax
 mov rax, qword ptr [r12 + 1096]
 mov qword ptr [r12 + 1032], rax
  .section .rodata
  .Lbynamefn52: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn52]
 lea rsi, [r12 + 1008]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 992], rax
 mov qword ptr [r12 + 1000], rdx
 cmp eax, 99
 je xchain0_n41_α
 jmp xchain0_n57_α
 xchain0_n51_β:
 jmp xchain0_n41_α
 xchain0_n52_α:
# BOX CALL convert(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1200] -> [zr+1168]
 mov rax, qword ptr [r12 + 1200]
 mov qword ptr [r12 + 1168], rax
 mov rax, qword ptr [r12 + 1208]
 mov qword ptr [r12 + 1176], rax
# marshal arg1 = producer-box slot [zr+1216] -> [zr+1184]
 mov rax, qword ptr [r12 + 1216]
 mov qword ptr [r12 + 1184], rax
 mov rax, qword ptr [r12 + 1224]
 mov qword ptr [r12 + 1192], rax
  .section .rodata
  .Lbynamefn53: .string "convert"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn53]
 lea rsi, [r12 + 1168]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1152], rax
 mov qword ptr [r12 + 1160], rdx
 cmp eax, 99
 je xchain0_n48_α
 jmp xchain0_n58_α
 xchain0_n52_β:
 jmp xchain0_n48_α
# IR_LIT_INTEGER
 xchain0_n53_α:
 mov qword ptr [r12 + 1296], 6
 mov rax, qword ptr [rip + .Lx55_0]
 mov qword ptr [r12 + 1304], rax
 jmp xchain0_n59_α
 xchain0_n53_β:
 jmp xchain0_n54_α
.Lx55_0:
 .quad 7
# IR_VAR
 xchain0_n54_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 1440], rax
 mov qword ptr [r12 + 1448], rdx
 jmp xchain0_n60_α
 xchain0_n54_β:
 jmp xchain0_n61_α
# IR_ASSIGN gva
 xchain0_n55_α:
 mov rax, qword ptr [r12 + 720]
 mov rdx, qword ptr [r12 + 728]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 704], rax
 mov qword ptr [r12 + 712], rdx
 jmp main_γ
 xchain0_n55_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n56_α:
 mov qword ptr [r12 + 880], 1
 mov rax, qword ptr [rip + .Lx58_0]
 mov qword ptr [r12 + 888], rax
 jmp xchain0_n62_α
 xchain0_n56_β:
 jmp main_γ
.Lx58_0:
 .quad .Lx58_0_s
.Lx58_0_s:
 .string "FAIL 1113/004: string key survives int key add"
# IR_LIT_STRING
 xchain0_n57_α:
 mov qword ptr [r12 + 1120], 1
 mov rax, qword ptr [rip + .Lx59_0]
 mov qword ptr [r12 + 1128], rax
 jmp xchain0_n63_α
 xchain0_n57_β:
 jmp main_γ
.Lx59_0:
 .quad .Lx59_0_s
.Lx59_0_s:
 .string "FAIL 1113/005: table->array prototype 2,2"
# IR_ASSIGN gva
 xchain0_n58_α:
 mov rax, qword ptr [r12 + 1152]
 mov rdx, qword ptr [r12 + 1160]
 mov qword ptr [rbx + 48], rax
 mov qword ptr [rbx + 56], rdx
 mov qword ptr [r12 + 1136], rax
 mov qword ptr [r12 + 1144], rdx
 jmp xchain0_n48_α
 xchain0_n58_β:
 jmp xchain0_n48_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n59_α:
 mov rdi, qword ptr [r12 + 1280]
 mov rsi, qword ptr [r12 + 1288]
 mov rdx, qword ptr [r12 + 1296]
 mov rcx, qword ptr [r12 + 1304]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n54_α
 mov qword ptr [r12 + 1312], rax
 mov qword ptr [r12 + 1320], rdx
 jmp xchain0_n64_α
 xchain0_n59_β:
 jmp xchain0_n54_α
# IR_LIT_STRING
 xchain0_n60_α:
 mov qword ptr [r12 + 1456], 1
 mov rax, qword ptr [rip + .Lx62_0]
 mov qword ptr [r12 + 1464], rax
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
 mov qword ptr [r12 + 1552], rax
 mov qword ptr [r12 + 1560], rdx
 jmp xchain0_n66_α
 xchain0_n61_β:
 jmp xchain0_n67_α
# IR_ASSIGN gva
 xchain0_n62_α:
 mov rax, qword ptr [r12 + 880]
 mov rdx, qword ptr [r12 + 888]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 864], rax
 mov qword ptr [r12 + 872], rdx
 jmp main_γ
 xchain0_n62_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain0_n63_α:
 mov rax, qword ptr [r12 + 1120]
 mov rdx, qword ptr [r12 + 1128]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1104], rax
 mov qword ptr [r12 + 1112], rdx
 jmp main_γ
 xchain0_n63_β:
 jmp main_γ
# IR_DEREF variable -> value
 xchain0_n64_α:
 mov rdi, qword ptr [r12 + 1312]
 mov rsi, qword ptr [r12 + 1320]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n54_α
 mov qword ptr [r12 + 1328], rax
 mov qword ptr [r12 + 1336], rdx
 jmp xchain0_n68_α
 xchain0_n64_β:
 jmp xchain0_n54_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n65_α:
 mov rdi, qword ptr [r12 + 1440]
 mov rsi, qword ptr [r12 + 1448]
 mov rdx, qword ptr [r12 + 1456]
 mov rcx, qword ptr [r12 + 1464]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n61_α
 mov qword ptr [r12 + 1472], rax
 mov qword ptr [r12 + 1480], rdx
 jmp xchain0_n69_α
 xchain0_n65_β:
 jmp xchain0_n61_α
# IR_LIT_STRING
 xchain0_n66_α:
 mov qword ptr [r12 + 1568], 1
 mov rax, qword ptr [rip + .Lx68_0]
 mov qword ptr [r12 + 1576], rax
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
 mov qword ptr [r12 + 1680], rax
 mov qword ptr [r12 + 1688], rdx
 jmp xchain0_n71_α
 xchain0_n67_β:
 jmp xchain0_n72_α
# IR_LIT_INTEGER
 xchain0_n68_α:
 mov qword ptr [r12 + 1344], 6
 mov rax, qword ptr [rip + .Lx70_0]
 mov qword ptr [r12 + 1352], rax
 jmp xchain0_n73_α
 xchain0_n68_β:
 jmp xchain0_n54_α
.Lx70_0:
 .quad 45
# IR_DEREF variable -> value
 xchain0_n69_α:
 mov rdi, qword ptr [r12 + 1472]
 mov rsi, qword ptr [r12 + 1480]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n61_α
 mov qword ptr [r12 + 1488], rax
 mov qword ptr [r12 + 1496], rdx
 jmp xchain0_n74_α
 xchain0_n69_β:
 jmp xchain0_n61_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n70_α:
 mov rdi, qword ptr [r12 + 1552]
 mov rsi, qword ptr [r12 + 1560]
 mov rdx, qword ptr [r12 + 1568]
 mov rcx, qword ptr [r12 + 1576]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n67_α
 mov qword ptr [r12 + 1584], rax
 mov qword ptr [r12 + 1592], rdx
 jmp xchain0_n75_α
 xchain0_n70_β:
 jmp xchain0_n67_α
# IR_LIT_STRING
 xchain0_n71_α:
 mov qword ptr [r12 + 1696], 1
 mov rax, qword ptr [rip + .Lx73_0]
 mov qword ptr [r12 + 1704], rax
 jmp xchain0_n76_α
 xchain0_n71_β:
 jmp xchain0_n72_α
.Lx73_0:
 .quad .Lx73_0_s
.Lx73_0_s:
 .string "cat"
# IR_LIT_STRING
 xchain0_n72_α:
 mov qword ptr [r12 + 1808], 1
 mov rax, qword ptr [rip + .Lx74_0]
 mov qword ptr [r12 + 1816], rax
 jmp xchain0_n77_α
 xchain0_n72_β:
 jmp main_γ
.Lx74_0:
 .quad .Lx74_0_s
.Lx74_0_s:
 .string "PASS 1113_table (8/8)"
 xchain0_n73_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1328] -> [zr+1248]
 mov rax, qword ptr [r12 + 1328]
 mov qword ptr [r12 + 1248], rax
 mov rax, qword ptr [r12 + 1336]
 mov qword ptr [r12 + 1256], rax
# marshal arg1 = producer-box slot [zr+1344] -> [zr+1264]
 mov rax, qword ptr [r12 + 1344]
 mov qword ptr [r12 + 1264], rax
 mov rax, qword ptr [r12 + 1352]
 mov qword ptr [r12 + 1272], rax
  .section .rodata
  .Lbynamefn74: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn74]
 lea rsi, [r12 + 1248]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1232], rax
 mov qword ptr [r12 + 1240], rdx
 cmp eax, 99
 je xchain0_n54_α
 jmp xchain0_n78_α
 xchain0_n73_β:
 jmp xchain0_n54_α
# IR_LIT_STRING
 xchain0_n74_α:
 mov qword ptr [r12 + 1504], 1
 mov rax, qword ptr [rip + .Lx76_0]
 mov qword ptr [r12 + 1512], rax
 jmp xchain0_n79_α
 xchain0_n74_β:
 jmp xchain0_n61_α
.Lx76_0:
 .quad .Lx76_0_s
.Lx76_0_s:
 .string "dog"
# IR_LIT_STRING
 xchain0_n75_α:
 mov qword ptr [r12 + 1600], 1
 mov rax, qword ptr [rip + .Lx77_0]
 mov qword ptr [r12 + 1608], rax
 jmp xchain0_n80_α
 xchain0_n75_β:
 jmp xchain0_n67_α
.Lx77_0:
 .quad .Lx77_0_s
.Lx77_0_s:
 .string "fish"
# IR_SUBSCRIPT x[i] variable
 xchain0_n76_α:
 mov rdi, qword ptr [r12 + 1680]
 mov rsi, qword ptr [r12 + 1688]
 mov rdx, qword ptr [r12 + 1696]
 mov rcx, qword ptr [r12 + 1704]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n72_α
 mov qword ptr [r12 + 1712], rax
 mov qword ptr [r12 + 1720], rdx
 jmp xchain0_n81_α
 xchain0_n76_β:
 jmp xchain0_n72_α
# IR_ASSIGN gva
 xchain0_n77_α:
 mov rax, qword ptr [r12 + 1808]
 mov rdx, qword ptr [r12 + 1816]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1792], rax
 mov qword ptr [r12 + 1800], rdx
 jmp main_γ
 xchain0_n77_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n78_α:
 mov qword ptr [r12 + 1376], 1
 mov rax, qword ptr [rip + .Lx80_0]
 mov qword ptr [r12 + 1384], rax
 jmp xchain0_n82_α
 xchain0_n78_β:
 jmp main_γ
.Lx80_0:
 .quad .Lx80_0_s
.Lx80_0_s:
 .string "FAIL 1113/006: array->table int key roundtrip"
 xchain0_n79_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1488] -> [zr+1408]
 mov rax, qword ptr [r12 + 1488]
 mov qword ptr [r12 + 1408], rax
 mov rax, qword ptr [r12 + 1496]
 mov qword ptr [r12 + 1416], rax
# marshal arg1 = producer-box slot [zr+1504] -> [zr+1424]
 mov rax, qword ptr [r12 + 1504]
 mov qword ptr [r12 + 1424], rax
 mov rax, qword ptr [r12 + 1512]
 mov qword ptr [r12 + 1432], rax
  .section .rodata
  .Lbynamefn80: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn80]
 lea rsi, [r12 + 1408]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1392], rax
 mov qword ptr [r12 + 1400], rdx
 cmp eax, 99
 je xchain0_n61_α
 jmp xchain0_n83_α
 xchain0_n79_β:
 jmp xchain0_n61_α
# IR_ASSIGN_VAR
 xchain0_n80_α:
 mov rdi, qword ptr [r12 + 1584]
 mov rsi, qword ptr [r12 + 1592]
 mov rdx, qword ptr [r12 + 1600]
 mov rcx, qword ptr [r12 + 1608]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n67_α
 mov qword ptr [r12 + 1616], rax
 mov qword ptr [r12 + 1624], rdx
 jmp xchain0_n67_α
 xchain0_n80_β:
 jmp xchain0_n67_α
# IR_DEREF variable -> value
 xchain0_n81_α:
 mov rdi, qword ptr [r12 + 1712]
 mov rsi, qword ptr [r12 + 1720]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n72_α
 mov qword ptr [r12 + 1728], rax
 mov qword ptr [r12 + 1736], rdx
 jmp xchain0_n84_α
 xchain0_n81_β:
 jmp xchain0_n72_α
# IR_ASSIGN gva
 xchain0_n82_α:
 mov rax, qword ptr [r12 + 1376]
 mov rdx, qword ptr [r12 + 1384]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1360], rax
 mov qword ptr [r12 + 1368], rdx
 jmp main_γ
 xchain0_n82_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n83_α:
 mov qword ptr [r12 + 1536], 1
 mov rax, qword ptr [rip + .Lx85_0]
 mov qword ptr [r12 + 1544], rax
 jmp xchain0_n85_α
 xchain0_n83_β:
 jmp main_γ
.Lx85_0:
 .quad .Lx85_0_s
.Lx85_0_s:
 .string "FAIL 1113/007: array->table string key roundtrip"
# IR_LIT_STRING
 xchain0_n84_α:
 mov qword ptr [r12 + 1744], 1
 mov rax, qword ptr [rip + .Lx86_0]
 mov qword ptr [r12 + 1752], rax
 jmp xchain0_n86_α
 xchain0_n84_β:
 jmp xchain0_n72_α
.Lx86_0:
 .quad .Lx86_0_s
.Lx86_0_s:
 .string "fish"
# IR_ASSIGN gva
 xchain0_n85_α:
 mov rax, qword ptr [r12 + 1536]
 mov rdx, qword ptr [r12 + 1544]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1520], rax
 mov qword ptr [r12 + 1528], rdx
 jmp main_γ
 xchain0_n85_β:
 jmp main_γ
 xchain0_n86_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1728] -> [zr+1648]
 mov rax, qword ptr [r12 + 1728]
 mov qword ptr [r12 + 1648], rax
 mov rax, qword ptr [r12 + 1736]
 mov qword ptr [r12 + 1656], rax
# marshal arg1 = producer-box slot [zr+1744] -> [zr+1664]
 mov rax, qword ptr [r12 + 1744]
 mov qword ptr [r12 + 1664], rax
 mov rax, qword ptr [r12 + 1752]
 mov qword ptr [r12 + 1672], rax
  .section .rodata
  .Lbynamefn87: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn87]
 lea rsi, [r12 + 1648]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1632], rax
 mov qword ptr [r12 + 1640], rdx
 cmp eax, 99
 je xchain0_n72_α
 jmp xchain0_n87_α
 xchain0_n86_β:
 jmp xchain0_n72_α
# IR_LIT_STRING
 xchain0_n87_α:
 mov qword ptr [r12 + 1776], 1
 mov rax, qword ptr [rip + .Lx89_0]
 mov qword ptr [r12 + 1784], rax
 jmp xchain0_n88_α
 xchain0_n87_β:
 jmp main_γ
.Lx89_0:
 .quad .Lx89_0_s
.Lx89_0_s:
 .string "FAIL 1113/008: [] and <> syntax equivalent"
# IR_ASSIGN gva
 xchain0_n88_α:
 mov rax, qword ptr [r12 + 1776]
 mov rdx, qword ptr [r12 + 1784]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1760], rax
 mov qword ptr [r12 + 1768], rdx
 jmp main_γ
 xchain0_n88_β:
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
 mov rdi, qword ptr [r12 + 1824]
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

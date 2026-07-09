  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "c"
  .Lgvan1: .string "output"
  .Lgvan2: .string "d"
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
  push rbp
  mov rbp, rsp
  push rdi
  push rsi
  call core_lib_init@PLT
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 3
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
 mov qword ptr [r12 + 1232], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [r12 + 64], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n3_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "2,2"
 xchain0_n1_α:
# BOX IR_CALL array(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+64] -> [zr+48]
 mov rax, qword ptr [r12 + 64]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 72]
 mov qword ptr [r12 + 56], rax
  .section .rodata
  .Lbynamefn3: .string "array"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn3]
 lea rsi, [r12 + 48]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n2_α
xchain0_n1_β:
 jmp xchain0_n3_α
# IR_ASSIGN gva
 xchain0_n2_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n3_α
# IR_VAR gva
 xchain0_n3_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n5_α
# IR_LIT_INTEGER
 xchain0_n4_α:
 mov qword ptr [r12 + 96], 6
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 104], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n5_α
.Lx6_0:
 .quad 1
# IR_VAR gva
 xchain0_n5_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n8_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n6_α:
 mov rdi, qword ptr [r12 + 80]
 mov rsi, qword ptr [r12 + 88]
 mov rdx, qword ptr [r12 + 96]
 mov rcx, qword ptr [r12 + 104]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n5_α
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 jmp xchain0_n9_α
 xchain0_n6_β:
 jmp xchain0_n5_α
# IR_LIT_INTEGER
 xchain0_n7_α:
 mov qword ptr [r12 + 256], 6
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 264], rax
 jmp xchain0_n10_α
 xchain0_n7_β:
 jmp xchain0_n8_α
.Lx9_0:
 .quad 1
# IR_VAR gva
 xchain0_n8_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 464], rax
 mov qword ptr [r12 + 472], rdx
 jmp xchain0_n11_α
 xchain0_n8_β:
 jmp xchain0_n12_α
# IR_LIT_INTEGER
 xchain0_n9_α:
 mov qword ptr [r12 + 128], 6
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 136], rax
 jmp xchain0_n13_α
 xchain0_n9_β:
 jmp xchain0_n5_α
.Lx11_0:
 .quad 2
# IR_SUBSCRIPT x[i] variable
 xchain0_n10_α:
 mov rdi, qword ptr [r12 + 240]
 mov rsi, qword ptr [r12 + 248]
 mov rdx, qword ptr [r12 + 256]
 mov rcx, qword ptr [r12 + 264]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n8_α
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 jmp xchain0_n14_α
 xchain0_n10_β:
 jmp xchain0_n8_α
 xchain0_n11_α:
# BOX IR_CALL prototype(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+464] -> [zr+448]
 mov rax, qword ptr [r12 + 464]
 mov qword ptr [r12 + 448], rax
 mov rax, qword ptr [r12 + 472]
 mov qword ptr [r12 + 456], rax
  .section .rodata
  .Lbynamefn14: .string "prototype"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn14]
 lea rsi, [r12 + 448]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 432], rax
 mov qword ptr [r12 + 440], rdx
 cmp eax, 99
 je xchain0_n12_α
 jmp xchain0_n15_α
xchain0_n11_β:
 jmp xchain0_n12_α
# IR_LIT_STRING
 xchain0_n12_α:
 mov qword ptr [r12 + 576], 1
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [r12 + 584], rax
 jmp xchain0_n16_α
 xchain0_n12_β:
 jmp xchain0_n21_α
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "-1:1,2"
# IR_SUBSCRIPT x[i] variable
 xchain0_n13_α:
 mov rdi, qword ptr [r12 + 112]
 mov rsi, qword ptr [r12 + 120]
 mov rdx, qword ptr [r12 + 128]
 mov rcx, qword ptr [r12 + 136]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n5_α
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 jmp xchain0_n17_α
 xchain0_n13_β:
 jmp xchain0_n5_α
# IR_LIT_INTEGER
 xchain0_n14_α:
 mov qword ptr [r12 + 288], 6
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [r12 + 296], rax
 jmp xchain0_n18_α
 xchain0_n14_β:
 jmp xchain0_n8_α
.Lx17_0:
 .quad 2
# IR_LIT_STRING
 xchain0_n15_α:
 mov qword ptr [r12 + 480], 1
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [r12 + 488], rax
 jmp xchain0_n19_α
 xchain0_n15_β:
 jmp xchain0_n12_α
.Lx18_0:
 .quad .Lx18_0_s
.Lx18_0_s:
 .string "2,2"
 xchain0_n16_α:
# BOX IR_CALL array(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+576] -> [zr+560]
 mov rax, qword ptr [r12 + 576]
 mov qword ptr [r12 + 560], rax
 mov rax, qword ptr [r12 + 584]
 mov qword ptr [r12 + 568], rax
  .section .rodata
  .Lbynamefn20: .string "array"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn20]
 lea rsi, [r12 + 560]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 544], rax
 mov qword ptr [r12 + 552], rdx
 cmp eax, 99
 je xchain0_n21_α
 jmp xchain0_n20_α
xchain0_n16_β:
 jmp xchain0_n21_α
# IR_LIT_STRING
 xchain0_n17_α:
 mov qword ptr [r12 + 160], 1
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [r12 + 168], rax
 jmp xchain0_n22_α
 xchain0_n17_β:
 jmp xchain0_n5_α
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string "*"
# IR_SUBSCRIPT x[i] variable
 xchain0_n18_α:
 mov rdi, qword ptr [r12 + 272]
 mov rsi, qword ptr [r12 + 280]
 mov rdx, qword ptr [r12 + 288]
 mov rcx, qword ptr [r12 + 296]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n8_α
 mov qword ptr [r12 + 304], rax
 mov qword ptr [r12 + 312], rdx
 jmp xchain0_n23_α
 xchain0_n18_β:
 jmp xchain0_n8_α
 xchain0_n19_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+432] -> [zr+400]
 mov rax, qword ptr [r12 + 432]
 mov qword ptr [r12 + 400], rax
 mov rax, qword ptr [r12 + 440]
 mov qword ptr [r12 + 408], rax
# marshal arg1 = producer-box slot [zr+480] -> [zr+416]
 mov rax, qword ptr [r12 + 480]
 mov qword ptr [r12 + 416], rax
 mov rax, qword ptr [r12 + 488]
 mov qword ptr [r12 + 424], rax
  .section .rodata
  .Lbynamefn24: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn24]
 lea rsi, [r12 + 400]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 cmp eax, 99
 je xchain0_n12_α
 jmp xchain0_n24_α
xchain0_n19_β:
 jmp xchain0_n12_α
# IR_ASSIGN gva
 xchain0_n20_α:
 mov rax, qword ptr [r12 + 544]
 mov rdx, qword ptr [r12 + 552]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [r12 + 528], rax
 mov qword ptr [r12 + 536], rdx
 jmp xchain0_n21_α
 xchain0_n20_β:
 jmp xchain0_n21_α
# IR_VAR gva
 xchain0_n21_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 592], rax
 mov qword ptr [r12 + 600], rdx
 jmp xchain0_n25_α
 xchain0_n21_β:
 jmp xchain0_n26_α
# IR_ASSIGN_VAR
 xchain0_n22_α:
 mov rdi, qword ptr [r12 + 144]
 mov rsi, qword ptr [r12 + 152]
 mov rdx, qword ptr [r12 + 160]
 mov rcx, qword ptr [r12 + 168]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n5_α
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 jmp xchain0_n5_α
 xchain0_n22_β:
 jmp xchain0_n5_α
# IR_DEREF variable -> value
 xchain0_n23_α:
 mov rdi, qword ptr [r12 + 304]
 mov rsi, qword ptr [r12 + 312]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n8_α
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 jmp xchain0_n27_α
 xchain0_n23_β:
 jmp xchain0_n8_α
# IR_LIT_STRING
 xchain0_n24_α:
 mov qword ptr [r12 + 512], 1
 mov rax, qword ptr [rip + .Lx29_0]
 mov qword ptr [r12 + 520], rax
 jmp xchain0_n28_α
 xchain0_n24_β:
 jmp main_γ
.Lx29_0:
 .quad .Lx29_0_s
.Lx29_0_s:
 .string "FAIL 1112/002: prototype of 2D array"
# IR_LIT_INTEGER
 xchain0_n25_α:
 mov qword ptr [r12 + 624], 6
 mov rax, qword ptr [rip + .Lx30_0]
 mov qword ptr [r12 + 632], rax
 jmp xchain0_n29_α
 xchain0_n25_β:
 jmp xchain0_n26_α
.Lx30_0:
 .quad 1
# IR_VAR gva
 xchain0_n26_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 768], rax
 mov qword ptr [r12 + 776], rdx
 jmp xchain0_n30_α
 xchain0_n26_β:
 jmp xchain0_n31_α
# IR_LIT_STRING
 xchain0_n27_α:
 mov qword ptr [r12 + 336], 1
 mov rax, qword ptr [rip + .Lx32_0]
 mov qword ptr [r12 + 344], rax
 jmp xchain0_n32_α
 xchain0_n27_β:
 jmp xchain0_n8_α
.Lx32_0:
 .quad .Lx32_0_s
.Lx32_0_s:
 .string "*"
# IR_ASSIGN gva
 xchain0_n28_α:
 mov rax, qword ptr [r12 + 512]
 mov rdx, qword ptr [r12 + 520]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 496], rax
 mov qword ptr [r12 + 504], rdx
 jmp main_γ
 xchain0_n28_β:
 jmp main_γ
# IR_UNOP
 xchain0_n29_α:
 mov rdi, qword ptr [r12 + 624]
 mov rsi, qword ptr [r12 + 632]
 call rt_num_neg@PLT
 mov qword ptr [r12 + 608], rax
 mov qword ptr [r12 + 616], rdx
 jmp xchain0_n33_α
 xchain0_n29_β:
 jmp xchain0_n26_α
# IR_LIT_INTEGER
 xchain0_n30_α:
 mov qword ptr [r12 + 800], 6
 mov rax, qword ptr [rip + .Lx35_0]
 mov qword ptr [r12 + 808], rax
 jmp xchain0_n34_α
 xchain0_n30_β:
 jmp xchain0_n31_α
.Lx35_0:
 .quad 1
# IR_VAR gva
 xchain0_n31_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 928], rax
 mov qword ptr [r12 + 936], rdx
 jmp xchain0_n35_α
 xchain0_n31_β:
 jmp xchain0_n36_α
 xchain0_n32_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+320] -> [zr+208]
 mov rax, qword ptr [r12 + 320]
 mov qword ptr [r12 + 208], rax
 mov rax, qword ptr [r12 + 328]
 mov qword ptr [r12 + 216], rax
# marshal arg1 = producer-box slot [zr+336] -> [zr+224]
 mov rax, qword ptr [r12 + 336]
 mov qword ptr [r12 + 224], rax
 mov rax, qword ptr [r12 + 344]
 mov qword ptr [r12 + 232], rax
  .section .rodata
  .Lbynamefn38: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn38]
 lea rsi, [r12 + 208]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 cmp eax, 99
 je xchain0_n8_α
 jmp xchain0_n37_α
xchain0_n32_β:
 jmp xchain0_n8_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n33_α:
 mov rdi, qword ptr [r12 + 592]
 mov rsi, qword ptr [r12 + 600]
 mov rdx, qword ptr [r12 + 608]
 mov rcx, qword ptr [r12 + 616]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n26_α
 mov qword ptr [r12 + 640], rax
 mov qword ptr [r12 + 648], rdx
 jmp xchain0_n38_α
 xchain0_n33_β:
 jmp xchain0_n26_α
# IR_UNOP
 xchain0_n34_α:
 mov rdi, qword ptr [r12 + 800]
 mov rsi, qword ptr [r12 + 808]
 call rt_num_neg@PLT
 mov qword ptr [r12 + 784], rax
 mov qword ptr [r12 + 792], rdx
 jmp xchain0_n39_α
 xchain0_n34_β:
 jmp xchain0_n31_α
# IR_LIT_INTEGER
 xchain0_n35_α:
 mov qword ptr [r12 + 960], 6
 mov rax, qword ptr [rip + .Lx41_0]
 mov qword ptr [r12 + 968], rax
 jmp xchain0_n40_α
 xchain0_n35_β:
 jmp xchain0_n36_α
.Lx41_0:
 .quad 2
# IR_VAR gva
 xchain0_n36_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 1072], rax
 mov qword ptr [r12 + 1080], rdx
 jmp xchain0_n41_α
 xchain0_n36_β:
 jmp xchain0_n42_α
# IR_LIT_STRING
 xchain0_n37_α:
 mov qword ptr [r12 + 368], 1
 mov rax, qword ptr [rip + .Lx43_0]
 mov qword ptr [r12 + 376], rax
 jmp xchain0_n43_α
 xchain0_n37_β:
 jmp main_γ
.Lx43_0:
 .quad .Lx43_0_s
.Lx43_0_s:
 .string "FAIL 1112/001: 2D array assign/read"
# IR_LIT_INTEGER
 xchain0_n38_α:
 mov qword ptr [r12 + 656], 6
 mov rax, qword ptr [rip + .Lx44_0]
 mov qword ptr [r12 + 664], rax
 jmp xchain0_n44_α
 xchain0_n38_β:
 jmp xchain0_n26_α
.Lx44_0:
 .quad 1
# IR_SUBSCRIPT x[i] variable
 xchain0_n39_α:
 mov rdi, qword ptr [r12 + 768]
 mov rsi, qword ptr [r12 + 776]
 mov rdx, qword ptr [r12 + 784]
 mov rcx, qword ptr [r12 + 792]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n31_α
 mov qword ptr [r12 + 816], rax
 mov qword ptr [r12 + 824], rdx
 jmp xchain0_n45_α
 xchain0_n39_β:
 jmp xchain0_n31_α
# IR_UNOP
 xchain0_n40_α:
 mov rdi, qword ptr [r12 + 960]
 mov rsi, qword ptr [r12 + 968]
 call rt_num_neg@PLT
 mov qword ptr [r12 + 944], rax
 mov qword ptr [r12 + 952], rdx
 jmp xchain0_n46_α
 xchain0_n40_β:
 jmp xchain0_n36_α
# IR_LIT_INTEGER
 xchain0_n41_α:
 mov qword ptr [r12 + 1088], 6
 mov rax, qword ptr [rip + .Lx47_0]
 mov qword ptr [r12 + 1096], rax
 jmp xchain0_n47_α
 xchain0_n41_β:
 jmp xchain0_n42_α
.Lx47_0:
 .quad 2
# IR_LIT_STRING
 xchain0_n42_α:
 mov qword ptr [r12 + 1216], 1
 mov rax, qword ptr [rip + .Lx48_0]
 mov qword ptr [r12 + 1224], rax
 jmp xchain0_n48_α
 xchain0_n42_β:
 jmp main_γ
.Lx48_0:
 .quad .Lx48_0_s
.Lx48_0_s:
 .string "PASS 1112_array_multi (5/5)"
# IR_ASSIGN gva
 xchain0_n43_α:
 mov rax, qword ptr [r12 + 368]
 mov rdx, qword ptr [r12 + 376]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 352], rax
 mov qword ptr [r12 + 360], rdx
 jmp main_γ
 xchain0_n43_β:
 jmp main_γ
# IR_SUBSCRIPT x[i] variable
 xchain0_n44_α:
 mov rdi, qword ptr [r12 + 640]
 mov rsi, qword ptr [r12 + 648]
 mov rdx, qword ptr [r12 + 656]
 mov rcx, qword ptr [r12 + 664]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n26_α
 mov qword ptr [r12 + 672], rax
 mov qword ptr [r12 + 680], rdx
 jmp xchain0_n49_α
 xchain0_n44_β:
 jmp xchain0_n26_α
# IR_LIT_INTEGER
 xchain0_n45_α:
 mov qword ptr [r12 + 832], 6
 mov rax, qword ptr [rip + .Lx51_0]
 mov qword ptr [r12 + 840], rax
 jmp xchain0_n50_α
 xchain0_n45_β:
 jmp xchain0_n31_α
.Lx51_0:
 .quad 1
# IR_SUBSCRIPT x[i] variable
 xchain0_n46_α:
 mov rdi, qword ptr [r12 + 928]
 mov rsi, qword ptr [r12 + 936]
 mov rdx, qword ptr [r12 + 944]
 mov rcx, qword ptr [r12 + 952]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n36_α
 mov qword ptr [r12 + 976], rax
 mov qword ptr [r12 + 984], rdx
 jmp xchain0_n51_α
 xchain0_n46_β:
 jmp xchain0_n36_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n47_α:
 mov rdi, qword ptr [r12 + 1072]
 mov rsi, qword ptr [r12 + 1080]
 mov rdx, qword ptr [r12 + 1088]
 mov rcx, qword ptr [r12 + 1096]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n42_α
 mov qword ptr [r12 + 1104], rax
 mov qword ptr [r12 + 1112], rdx
 jmp xchain0_n52_α
 xchain0_n47_β:
 jmp xchain0_n42_α
# IR_ASSIGN gva
 xchain0_n48_α:
 mov rax, qword ptr [r12 + 1216]
 mov rdx, qword ptr [r12 + 1224]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1200], rax
 mov qword ptr [r12 + 1208], rdx
 jmp main_γ
 xchain0_n48_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n49_α:
 mov qword ptr [r12 + 688], 6
 mov rax, qword ptr [rip + .Lx55_0]
 mov qword ptr [r12 + 696], rax
 jmp xchain0_n53_α
 xchain0_n49_β:
 jmp xchain0_n26_α
.Lx55_0:
 .quad 0
# IR_SUBSCRIPT x[i] variable
 xchain0_n50_α:
 mov rdi, qword ptr [r12 + 816]
 mov rsi, qword ptr [r12 + 824]
 mov rdx, qword ptr [r12 + 832]
 mov rcx, qword ptr [r12 + 840]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n31_α
 mov qword ptr [r12 + 848], rax
 mov qword ptr [r12 + 856], rdx
 jmp xchain0_n54_α
 xchain0_n50_β:
 jmp xchain0_n31_α
# IR_LIT_INTEGER
 xchain0_n51_α:
 mov qword ptr [r12 + 992], 6
 mov rax, qword ptr [rip + .Lx57_0]
 mov qword ptr [r12 + 1000], rax
 jmp xchain0_n55_α
 xchain0_n51_β:
 jmp xchain0_n36_α
.Lx57_0:
 .quad 1
# IR_LIT_INTEGER
 xchain0_n52_α:
 mov qword ptr [r12 + 1120], 6
 mov rax, qword ptr [rip + .Lx58_0]
 mov qword ptr [r12 + 1128], rax
 jmp xchain0_n56_α
 xchain0_n52_β:
 jmp xchain0_n42_α
.Lx58_0:
 .quad 1
# IR_ASSIGN_VAR
 xchain0_n53_α:
 mov rdi, qword ptr [r12 + 672]
 mov rsi, qword ptr [r12 + 680]
 mov rdx, qword ptr [r12 + 688]
 mov rcx, qword ptr [r12 + 696]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n26_α
 mov qword ptr [r12 + 704], rax
 mov qword ptr [r12 + 712], rdx
 jmp xchain0_n26_α
 xchain0_n53_β:
 jmp xchain0_n26_α
# IR_DEREF variable -> value
 xchain0_n54_α:
 mov rdi, qword ptr [r12 + 848]
 mov rsi, qword ptr [r12 + 856]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n31_α
 mov qword ptr [r12 + 864], rax
 mov qword ptr [r12 + 872], rdx
 jmp xchain0_n57_α
 xchain0_n54_β:
 jmp xchain0_n31_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n55_α:
 mov rdi, qword ptr [r12 + 976]
 mov rsi, qword ptr [r12 + 984]
 mov rdx, qword ptr [r12 + 992]
 mov rcx, qword ptr [r12 + 1000]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n36_α
 mov qword ptr [r12 + 1008], rax
 mov qword ptr [r12 + 1016], rdx
 jmp xchain0_n58_α
 xchain0_n55_β:
 jmp xchain0_n36_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n56_α:
 mov rdi, qword ptr [r12 + 1104]
 mov rsi, qword ptr [r12 + 1112]
 mov rdx, qword ptr [r12 + 1120]
 mov rcx, qword ptr [r12 + 1128]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n42_α
 mov qword ptr [r12 + 1136], rax
 mov qword ptr [r12 + 1144], rdx
 jmp xchain0_n59_α
 xchain0_n56_β:
 jmp xchain0_n42_α
# IR_LIT_INTEGER
 xchain0_n57_α:
 mov qword ptr [r12 + 880], 6
 mov rax, qword ptr [rip + .Lx63_0]
 mov qword ptr [r12 + 888], rax
 jmp xchain0_n60_α
 xchain0_n57_β:
 jmp xchain0_n31_α
.Lx63_0:
 .quad 0
# IR_DEREF variable -> value
 xchain0_n58_α:
 mov rdi, qword ptr [r12 + 1008]
 mov rsi, qword ptr [r12 + 1016]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n36_α
 mov qword ptr [r12 + 1024], rax
 mov qword ptr [r12 + 1032], rdx
 jmp xchain0_n61_α
 xchain0_n58_β:
 jmp xchain0_n36_α
# IR_DEREF variable -> value
 xchain0_n59_α:
 mov rdi, qword ptr [r12 + 1136]
 mov rsi, qword ptr [r12 + 1144]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n42_α
 mov qword ptr [r12 + 1152], rax
 mov qword ptr [r12 + 1160], rdx
 jmp xchain0_n62_α
 xchain0_n59_β:
 jmp xchain0_n42_α
 xchain0_n60_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+864] -> [zr+736]
 mov rax, qword ptr [r12 + 864]
 mov qword ptr [r12 + 736], rax
 mov rax, qword ptr [r12 + 872]
 mov qword ptr [r12 + 744], rax
# marshal arg1 = producer-box slot [zr+880] -> [zr+752]
 mov rax, qword ptr [r12 + 880]
 mov qword ptr [r12 + 752], rax
 mov rax, qword ptr [r12 + 888]
 mov qword ptr [r12 + 760], rax
  .section .rodata
  .Lbynamefn67: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn67]
 lea rsi, [r12 + 736]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 720], rax
 mov qword ptr [r12 + 728], rdx
 cmp eax, 99
 je xchain0_n31_α
 jmp xchain0_n63_α
xchain0_n60_β:
 jmp xchain0_n31_α
# IR_LIT_STRING
 xchain0_n61_α:
 mov qword ptr [r12 + 1056], 1
 mov rax, qword ptr [rip + .Lx68_0]
 mov qword ptr [r12 + 1064], rax
 jmp xchain0_n64_α
 xchain0_n61_β:
 jmp main_γ
.Lx68_0:
 .quad .Lx68_0_s
.Lx68_0_s:
 .string "FAIL 1112/004: below lower bound fails"
# IR_LIT_STRING
 xchain0_n62_α:
 mov qword ptr [r12 + 1184], 1
 mov rax, qword ptr [rip + .Lx69_0]
 mov qword ptr [r12 + 1192], rax
 jmp xchain0_n65_α
 xchain0_n62_β:
 jmp main_γ
.Lx69_0:
 .quad .Lx69_0_s
.Lx69_0_s:
 .string "FAIL 1112/005: above upper bound fails"
# IR_LIT_STRING
 xchain0_n63_α:
 mov qword ptr [r12 + 912], 1
 mov rax, qword ptr [rip + .Lx70_0]
 mov qword ptr [r12 + 920], rax
 jmp xchain0_n66_α
 xchain0_n63_β:
 jmp main_γ
.Lx70_0:
 .quad .Lx70_0_s
.Lx70_0_s:
 .string "FAIL 1112/003: custom lower bound assign/read"
# IR_ASSIGN gva
 xchain0_n64_α:
 mov rax, qword ptr [r12 + 1056]
 mov rdx, qword ptr [r12 + 1064]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1040], rax
 mov qword ptr [r12 + 1048], rdx
 jmp main_γ
 xchain0_n64_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain0_n65_α:
 mov rax, qword ptr [r12 + 1184]
 mov rdx, qword ptr [r12 + 1192]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1168], rax
 mov qword ptr [r12 + 1176], rdx
 jmp main_γ
 xchain0_n65_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain0_n66_α:
 mov rax, qword ptr [r12 + 912]
 mov rdx, qword ptr [r12 + 920]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 896], rax
 mov qword ptr [r12 + 904], rdx
 jmp main_γ
 xchain0_n66_β:
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
 mov rdi, qword ptr [r12 + 1232]
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

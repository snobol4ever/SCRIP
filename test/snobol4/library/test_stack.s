  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "myvar"
  .Lgvan1: .string "subject"
  .Lgvan2: .string "w2"
  .Lgvan3: .string "w1"
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
 mov qword ptr [r12 + 1296], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [r12 + 112], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n3_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "TRIM"
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [r12 + 128], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 136], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n3_α
.Lx2_0:
 .quad 1
 xchain0_n2_α:
# BOX IR_CALL SNO$KWSET(...) -> rt_call_arr [operand-marshal, FAIL->ω]
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
  .Lrkfn4: .string "SNO$KWSET"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn4]
 lea rsi, [r12 + 80]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
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
 lea rsi, [r12 + 160]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 cmp eax, 99
 je xchain0_n4_α
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n4_α
# IR_LIT_STRING
 xchain0_n4_α:
 mov qword ptr [r12 + 192], 1
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 200], rax
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n6_α
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "a"
 xchain0_n5_α:
# BOX CALL stack_push(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+192] -> [zr+176]
 mov rax, qword ptr [r12 + 192]
 mov qword ptr [r12 + 176], rax
 mov rax, qword ptr [r12 + 200]
 mov qword ptr [r12 + 184], rax
  .section .rodata
  .Lbynamefn6: .string "stack_push"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn6]
 lea rsi, [r12 + 176]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 cmp eax, 99
 je xchain0_n6_α
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n6_α
# IR_LIT_STRING
 xchain0_n6_α:
 mov qword ptr [r12 + 240], 1
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [r12 + 248], rax
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp xchain0_n8_α
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "b"
 xchain0_n7_α:
# BOX CALL stack_push(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+240] -> [zr+224]
 mov rax, qword ptr [r12 + 240]
 mov qword ptr [r12 + 224], rax
 mov rax, qword ptr [r12 + 248]
 mov qword ptr [r12 + 232], rax
  .section .rodata
  .Lbynamefn8: .string "stack_push"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn8]
 lea rsi, [r12 + 224]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 cmp eax, 99
 je xchain0_n8_α
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp xchain0_n8_α
# IR_LIT_STRING
 xchain0_n8_α:
 mov qword ptr [r12 + 288], 1
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [r12 + 296], rax
 jmp xchain0_n9_α
 xchain0_n8_β:
 jmp xchain0_n10_α
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "c"
 xchain0_n9_α:
# BOX CALL stack_push(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+288] -> [zr+272]
 mov rax, qword ptr [r12 + 288]
 mov qword ptr [r12 + 272], rax
 mov rax, qword ptr [r12 + 296]
 mov qword ptr [r12 + 280], rax
  .section .rodata
  .Lbynamefn10: .string "stack_push"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn10]
 lea rsi, [r12 + 272]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
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
 lea rsi, [r12 + 336]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 cmp eax, 99
 je xchain0_n12_α
 jmp xchain0_n11_α
 xchain0_n10_β:
 jmp xchain0_n12_α
# IR_ASSIGN global
 xchain0_n11_α:
 mov rsi, qword ptr [r12 + 320]
 mov rdx, qword ptr [r12 + 328]
 mov rdi, qword ptr [rip + .Lx13_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 304], rax
 mov qword ptr [r12 + 312], rdx
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
 lea rsi, [r12 + 368]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 352], rax
 mov qword ptr [r12 + 360], rdx
 cmp eax, 99
 je xchain0_n14_α
 jmp xchain0_n13_α
 xchain0_n12_β:
 jmp xchain0_n14_α
# IR_ASSIGN global
 xchain0_n13_α:
 mov rsi, qword ptr [r12 + 352]
 mov rdx, qword ptr [r12 + 360]
 mov rdi, qword ptr [rip + .Lx15_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
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
 lea rsi, [r12 + 400]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 cmp eax, 99
 je xchain0_n16_α
 jmp xchain0_n15_α
 xchain0_n14_β:
 jmp xchain0_n16_α
# IR_ASSIGN global
 xchain0_n15_α:
 mov rsi, qword ptr [r12 + 384]
 mov rdx, qword ptr [r12 + 392]
 mov rdi, qword ptr [rip + .Lx17_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 368], rax
 mov qword ptr [r12 + 376], rdx
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
 lea rsi, [r12 + 432]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 cmp eax, 99
 je xchain0_n18_α
 jmp xchain0_n17_α
 xchain0_n16_β:
 jmp xchain0_n18_α
# IR_ASSIGN global
 xchain0_n17_α:
 mov rsi, qword ptr [r12 + 416]
 mov rdx, qword ptr [r12 + 424]
 mov rdi, qword ptr [rip + .Lx19_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 400], rax
 mov qword ptr [r12 + 408], rdx
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
 lea rsi, [r12 + 464]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 448], rax
 mov qword ptr [r12 + 456], rdx
 cmp eax, 99
 je xchain0_n20_α
 jmp xchain0_n19_α
 xchain0_n18_β:
 jmp xchain0_n20_α
# IR_ASSIGN global
 xchain0_n19_α:
 mov rsi, qword ptr [r12 + 448]
 mov rdx, qword ptr [r12 + 456]
 mov rdi, qword ptr [rip + .Lx21_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 432], rax
 mov qword ptr [r12 + 440], rdx
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
 lea rsi, [r12 + 496]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 480], rax
 mov qword ptr [r12 + 488], rdx
 cmp eax, 99
 je xchain0_n22_α
 jmp xchain0_n21_α
 xchain0_n20_β:
 jmp xchain0_n22_α
# IR_ASSIGN global
 xchain0_n21_α:
 mov rsi, qword ptr [r12 + 480]
 mov rdx, qword ptr [r12 + 488]
 mov rdi, qword ptr [rip + .Lx23_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 464], rax
 mov qword ptr [r12 + 472], rdx
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
 lea rsi, [r12 + 512]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 496], rax
 mov qword ptr [r12 + 504], rdx
 cmp eax, 99
 je xchain0_n24_α
 jmp xchain0_n23_α
 xchain0_n22_β:
 jmp xchain0_n24_α
# IR_LIT_STRING
 xchain0_n23_α:
 mov qword ptr [r12 + 560], 1
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [r12 + 568], rax
 jmp xchain0_n25_α
 xchain0_n23_β:
 jmp xchain0_n27_α
.Lx25_0:
 .quad .Lx25_0_s
.Lx25_0_s:
 .string "FAIL: empty pop should FRETURN"
# IR_LIT_STRING
 xchain0_n24_α:
 mov qword ptr [r12 + 528], 1
 mov rax, qword ptr [rip + .Lx26_0]
 mov qword ptr [r12 + 536], rax
 jmp xchain0_n26_α
 xchain0_n24_β:
 jmp xchain0_n27_α
.Lx26_0:
 .quad .Lx26_0_s
.Lx26_0_s:
 .string "empty ok"
# IR_ASSIGN global
 xchain0_n25_α:
 mov rsi, qword ptr [r12 + 560]
 mov rdx, qword ptr [r12 + 568]
 mov rdi, qword ptr [rip + .Lx27_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 544], rax
 mov qword ptr [r12 + 552], rdx
 jmp xchain0_n27_α
 xchain0_n25_β:
 jmp xchain0_n27_α
.Lx27_0:
 .quad .Lx27_0_s
.Lx27_0_s:
 .string "OUTPUT"
# IR_ASSIGN global
 xchain0_n26_α:
 mov rsi, qword ptr [r12 + 528]
 mov rdx, qword ptr [r12 + 536]
 mov rdi, qword ptr [rip + .Lx28_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 512], rax
 mov qword ptr [r12 + 520], rdx
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
 lea rsi, [r12 + 592]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 576], rax
 mov qword ptr [r12 + 584], rdx
 cmp eax, 99
 je xchain0_n28_α
 jmp xchain0_n28_α
 xchain0_n27_β:
 jmp xchain0_n28_α
# IR_LIT_STRING
 xchain0_n28_α:
 mov qword ptr [r12 + 624], 1
 mov rax, qword ptr [rip + .Lx30_0]
 mov qword ptr [r12 + 632], rax
 jmp xchain0_n29_α
 xchain0_n28_β:
 jmp xchain0_n30_α
.Lx30_0:
 .quad .Lx30_0_s
.Lx30_0_s:
 .string "x"
 xchain0_n29_α:
# BOX CALL stack_push(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+624] -> [zr+608]
 mov rax, qword ptr [r12 + 624]
 mov qword ptr [r12 + 608], rax
 mov rax, qword ptr [r12 + 632]
 mov qword ptr [r12 + 616], rax
  .section .rodata
  .Lbynamefn30: .string "stack_push"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn30]
 lea rsi, [r12 + 608]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 592], rax
 mov qword ptr [r12 + 600], rdx
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
 lea rsi, [r12 + 672]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 656], rax
 mov qword ptr [r12 + 664], rdx
 cmp eax, 99
 je xchain0_n32_α
 jmp xchain0_n31_α
 xchain0_n30_β:
 jmp xchain0_n32_α
# IR_ASSIGN global
 xchain0_n31_α:
 mov rsi, qword ptr [r12 + 656]
 mov rdx, qword ptr [r12 + 664]
 mov rdi, qword ptr [rip + .Lx33_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 640], rax
 mov qword ptr [r12 + 648], rdx
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
 lea rsi, [r12 + 704]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 688], rax
 mov qword ptr [r12 + 696], rdx
 cmp eax, 99
 je xchain0_n34_α
 jmp xchain0_n33_α
 xchain0_n32_β:
 jmp xchain0_n34_α
# IR_ASSIGN global
 xchain0_n33_α:
 mov rsi, qword ptr [r12 + 688]
 mov rdx, qword ptr [r12 + 696]
 mov rdi, qword ptr [rip + .Lx35_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 672], rax
 mov qword ptr [r12 + 680], rdx
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
 lea rsi, [r12 + 736]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 720], rax
 mov qword ptr [r12 + 728], rdx
 cmp eax, 99
 je xchain0_n36_α
 jmp xchain0_n35_α
 xchain0_n34_β:
 jmp xchain0_n36_α
# IR_ASSIGN global
 xchain0_n35_α:
 mov rsi, qword ptr [r12 + 720]
 mov rdx, qword ptr [r12 + 728]
 mov rdi, qword ptr [rip + .Lx37_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 704], rax
 mov qword ptr [r12 + 712], rdx
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
 lea rsi, [r12 + 752]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 736], rax
 mov qword ptr [r12 + 744], rdx
 cmp eax, 99
 je xchain0_n37_α
 jmp xchain0_n37_α
 xchain0_n36_β:
 jmp xchain0_n37_α
# IR_LIT_INTEGER
 xchain0_n37_α:
 mov qword ptr [r12 + 784], 6
 mov rax, qword ptr [rip + .Lx39_0]
 mov qword ptr [r12 + 792], rax
 jmp xchain0_n38_α
 xchain0_n37_β:
 jmp xchain0_n39_α
.Lx39_0:
 .quad 42
 xchain0_n38_α:
# BOX CALL stack_push(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+784] -> [zr+768]
 mov rax, qword ptr [r12 + 784]
 mov qword ptr [r12 + 768], rax
 mov rax, qword ptr [r12 + 792]
 mov qword ptr [r12 + 776], rax
  .section .rodata
  .Lbynamefn39: .string "stack_push"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn39]
 lea rsi, [r12 + 768]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 752], rax
 mov qword ptr [r12 + 760], rdx
 cmp eax, 99
 je xchain0_n39_α
 jmp xchain0_n39_α
 xchain0_n38_β:
 jmp xchain0_n39_α
# IR_LIT_INTEGER
 xchain0_n39_α:
 mov qword ptr [r12 + 832], 6
 mov rax, qword ptr [rip + .Lx41_0]
 mov qword ptr [r12 + 840], rax
 jmp xchain0_n40_α
 xchain0_n39_β:
 jmp xchain0_n41_α
.Lx41_0:
 .quad 99
 xchain0_n40_α:
# BOX CALL stack_push(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+832] -> [zr+816]
 mov rax, qword ptr [r12 + 832]
 mov qword ptr [r12 + 816], rax
 mov rax, qword ptr [r12 + 840]
 mov qword ptr [r12 + 824], rax
  .section .rodata
  .Lbynamefn41: .string "stack_push"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn41]
 lea rsi, [r12 + 816]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 800], rax
 mov qword ptr [r12 + 808], rdx
 cmp eax, 99
 je xchain0_n41_α
 jmp xchain0_n41_α
 xchain0_n40_β:
 jmp xchain0_n41_α
# IR_LIT_STRING
 xchain0_n41_α:
 mov qword ptr [r12 + 880], 1
 mov rax, qword ptr [rip + .Lx43_0]
 mov qword ptr [r12 + 888], rax
 jmp xchain0_n42_α
 xchain0_n41_β:
 jmp xchain0_n43_α
.Lx43_0:
 .quad .Lx43_0_s
.Lx43_0_s:
 .string "myvar"
 xchain0_n42_α:
# BOX CALL stack_pop(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+880] -> [zr+864]
 mov rax, qword ptr [r12 + 880]
 mov qword ptr [r12 + 864], rax
 mov rax, qword ptr [r12 + 888]
 mov qword ptr [r12 + 872], rax
  .section .rodata
  .Lbynamefn43: .string "stack_pop"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn43]
 lea rsi, [r12 + 864]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 848], rax
 mov qword ptr [r12 + 856], rdx
 cmp eax, 99
 je xchain0_n43_α
 jmp xchain0_n43_α
 xchain0_n42_β:
 jmp xchain0_n43_α
# IR_VAR
 xchain0_n43_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 912], rax
 mov qword ptr [r12 + 920], rdx
 jmp xchain0_n44_α
 xchain0_n43_β:
 jmp xchain0_n45_α
# IR_ASSIGN global
 xchain0_n44_α:
 mov rsi, qword ptr [r12 + 912]
 mov rdx, qword ptr [r12 + 920]
 mov rdi, qword ptr [rip + .Lx46_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 896], rax
 mov qword ptr [r12 + 904], rdx
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
 lea rsi, [r12 + 944]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 928], rax
 mov qword ptr [r12 + 936], rdx
 cmp eax, 99
 je xchain0_n46_α
 jmp xchain0_n46_α
 xchain0_n45_β:
 jmp xchain0_n46_α
# IR_LIT_STRING
 xchain0_n46_α:
 mov qword ptr [r12 + 960], 1
 mov rax, qword ptr [rip + .Lx48_0]
 mov qword ptr [r12 + 968], rax
 jmp xchain0_n47_α
 xchain0_n46_β:
 jmp xchain0_n48_α
.Lx48_0:
 .quad .Lx48_0_s
.Lx48_0_s:
 .string "hello world"
# IR_ASSIGN gva
 xchain0_n47_α:
 mov rax, qword ptr [r12 + 960]
 mov rdx, qword ptr [r12 + 968]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 944], rax
 mov qword ptr [r12 + 952], rdx
 jmp xchain0_n48_α
 xchain0_n47_β:
 jmp xchain0_n48_α
# IR_VAR
 xchain0_n48_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 1088], rax
 mov qword ptr [r12 + 1096], rdx
 jmp xchain0_n49_α
 xchain0_n48_β:
 jmp xchain0_n50_α
# IR_MATCH_HEAD
 xchain0_n49_α:
 call rt_zls_mark@PLT
 mov qword ptr [r12 + 984], rax
 mov qword ptr [r12 + 992], rsp
 mov rdi, qword ptr [r12 + 1088]
 mov rsi, qword ptr [r12 + 1096]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov dword ptr [r12 + 976], 0
.Lx52_0:
 mov r14d, dword ptr [r12 + 976]
 jmp xchain0_n51_α
 xchain0_n49_β:
 add dword ptr [r12 + 976], 1
 mov eax, dword ptr [r12 + 976]
 cmp eax, r15d
 jg .Lx52_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx52_1
 jmp .Lx52_0
.Lx52_1:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 984]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
 mov rsp, qword ptr [r12 + 992]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_end_fail@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n50_α
# IR_LIT_STRING
 xchain0_n50_α:
 mov qword ptr [r12 + 1280], 1
 mov rax, qword ptr [rip + .Lx53_0]
 mov qword ptr [r12 + 1288], rax
 jmp xchain0_n52_α
 xchain0_n50_β:
 jmp main_γ
.Lx53_0:
 .quad .Lx53_0_s
.Lx53_0_s:
 .string "FAIL: pattern match failed"
# IR_MATCH_CAPTURE_SAVE push
 xchain0_n51_α:
 lea rdi, [r12 + 1056]
 mov esi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_cap_push@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n53_α
 xchain0_n51_β:
 lea rdi, [r12 + 1056]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_cap_pop@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n49_β
# IR_ASSIGN global
 xchain0_n52_α:
 mov rsi, qword ptr [r12 + 1280]
 mov rdx, qword ptr [r12 + 1288]
 mov rdi, qword ptr [rip + .Lx56_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 1264], rax
 mov qword ptr [r12 + 1272], rdx
 jmp main_γ
 xchain0_n52_β:
 jmp main_γ
.Lx56_0:
 .quad .Lx56_0_s
.Lx56_0_s:
 .string "OUTPUT"
# IR_MATCH_BREAK
 xchain0_n53_α:
 mov dword ptr [r12 + 1072], 0
.Lx58_0:
 mov eax, r14d
 add eax, dword ptr [r12 + 1072]
 cmp eax, r15d
 jge xchain0_n51_β
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S0]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 jnz .Lx58_1
 add dword ptr [r12 + 1072], 1
 jmp .Lx58_0
.Lx58_1:
 mov eax, r14d
 add eax, dword ptr [r12 + 1072]
 mov r14d, eax
 jmp xchain0_n54_α
 xchain0_n53_β:
 mov eax, r14d
 sub eax, dword ptr [r12 + 1072]
 mov r14d, eax
 jmp xchain0_n51_β
# IR_MATCH_CAPTURE_COND
 xchain0_n54_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 lea rdi, [r12 + 1056]
 call rt_cap_top@PLT
 lea rdi, [rip + .S1]
 mov esi, eax
 mov edx, r14d
 mov ecx, 0
 call rt_cap_assign_cursor@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n55_α
 xchain0_n54_β:
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_pop@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n51_β
# IR_MATCH_LIT
 xchain0_n55_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain0_n54_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S0]
 mov edx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xchain0_n54_β
 add r14d, 1
 jmp xchain0_n56_α
 xchain0_n55_β:
 sub r14d, 1
 jmp xchain0_n54_β
# IR_MATCH_CAPTURE_SAVE push
 xchain0_n56_α:
 lea rdi, [r12 + 1024]
 mov esi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_cap_push@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n57_α
 xchain0_n56_β:
 lea rdi, [r12 + 1024]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_cap_pop@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n49_β
# IR_MATCH_REM
 xchain0_n57_α:
 mov dword ptr [r12 + 1040], r14d
 mov r14d, r15d
 jmp xchain0_n58_α
 xchain0_n57_β:
 mov r14d, dword ptr [r12 + 1040]
 jmp xchain0_n56_β
# IR_MATCH_CAPTURE_COND
 xchain0_n58_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 lea rdi, [r12 + 1024]
 call rt_cap_top@PLT
 lea rdi, [rip + .S2]
 mov esi, eax
 mov edx, r14d
 mov ecx, 0
 call rt_cap_assign_cursor@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n59_α
 xchain0_n58_β:
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_pop@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n56_β
# IR_MATCH_RELEASE
 xchain0_n59_α:
 mov qword ptr [r12 + 1000], r14
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 984]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
 mov rsp, qword ptr [r12 + 992]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_end_ok@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n60_α
# IR_LIT_STRING
 xchain0_n60_α:
 mov qword ptr [r12 + 1008], 1
 mov rax, qword ptr [rip + .Lx70_0]
 mov qword ptr [r12 + 1016], rax
 jmp xchain0_n61_α
 xchain0_n60_β:
 jmp xchain0_n50_α
.Lx70_0:
 .quad .Lx70_0_s
.Lx70_0_s:
 .string ""
# IR_MATCH_REPLACE
 xchain0_n61_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx72_0]
 mov rsi, qword ptr [r12 + 1088]
 mov rdx, qword ptr [r12 + 1096]
 mov ecx, dword ptr [r12 + 976]
 mov r8, qword ptr [r12 + 1000]
 lea r9, [r12 + 1008]
 call rt_match_replace@PLT
 mov rsp, rbp
 pop rbp
 jmp .Lx72_1
.Lx72_0:
 .quad .Lx72_0_s
.Lx72_0_s:
 .string "subject"
.Lx72_1:
 jmp xchain0_n62_α
# IR_VAR
 xchain0_n62_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 1136], rax
 mov qword ptr [r12 + 1144], rdx
 jmp xchain0_n63_α
 xchain0_n62_β:
 jmp xchain0_n64_α
 xchain0_n63_α:
# BOX CALL stack_push(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1136] -> [zr+1120]
 mov rax, qword ptr [r12 + 1136]
 mov qword ptr [r12 + 1120], rax
 mov rax, qword ptr [r12 + 1144]
 mov qword ptr [r12 + 1128], rax
  .section .rodata
  .Lbynamefn64: .string "stack_push"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn64]
 lea rsi, [r12 + 1120]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1104], rax
 mov qword ptr [r12 + 1112], rdx
 cmp eax, 99
 je xchain0_n64_α
 jmp xchain0_n64_α
 xchain0_n63_β:
 jmp xchain0_n64_α
# IR_VAR
 xchain0_n64_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 1184], rax
 mov qword ptr [r12 + 1192], rdx
 jmp xchain0_n65_α
 xchain0_n64_β:
 jmp xchain0_n66_α
 xchain0_n65_α:
# BOX CALL stack_push(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1184] -> [zr+1168]
 mov rax, qword ptr [r12 + 1184]
 mov qword ptr [r12 + 1168], rax
 mov rax, qword ptr [r12 + 1192]
 mov qword ptr [r12 + 1176], rax
  .section .rodata
  .Lbynamefn66: .string "stack_push"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn66]
 lea rsi, [r12 + 1168]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1152], rax
 mov qword ptr [r12 + 1160], rdx
 cmp eax, 99
 je xchain0_n66_α
 jmp xchain0_n66_α
 xchain0_n65_β:
 jmp xchain0_n66_α
 xchain0_n66_α:
# BOX CALL stack_pop(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn67: .string "stack_pop"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn67]
 lea rsi, [r12 + 1232]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1216], rax
 mov qword ptr [r12 + 1224], rdx
 cmp eax, 99
 je xchain0_n68_α
 jmp xchain0_n67_α
 xchain0_n66_β:
 jmp xchain0_n68_α
# IR_ASSIGN global
 xchain0_n67_α:
 mov rsi, qword ptr [r12 + 1216]
 mov rdx, qword ptr [r12 + 1224]
 mov rdi, qword ptr [rip + .Lx78_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 1200], rax
 mov qword ptr [r12 + 1208], rdx
 jmp xchain0_n68_α
 xchain0_n67_β:
 jmp xchain0_n68_α
.Lx78_0:
 .quad .Lx78_0_s
.Lx78_0_s:
 .string "OUTPUT"
 xchain0_n68_α:
# BOX CALL stack_pop(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn69: .string "stack_pop"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn69]
 lea rsi, [r12 + 1264]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1248], rax
 mov qword ptr [r12 + 1256], rdx
 cmp eax, 99
 je main_γ
 jmp xchain0_n69_α
 xchain0_n68_β:
 jmp main_γ
# IR_ASSIGN global
 xchain0_n69_α:
 mov rsi, qword ptr [r12 + 1248]
 mov rdx, qword ptr [r12 + 1256]
 mov rdi, qword ptr [rip + .Lx80_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 1232], rax
 mov qword ptr [r12 + 1240], rdx
 jmp main_γ
 xchain0_n69_β:
 jmp main_γ
.Lx80_0:
 .quad .Lx80_0_s
.Lx80_0_s:
 .string "OUTPUT"
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
 mov rdi, qword ptr [r12 + 1296]
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
.section .rodata
.S0: .string " "
.S1: .string "w1"
.S2: .string "w2"
.text

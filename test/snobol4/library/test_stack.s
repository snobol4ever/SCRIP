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
 mov qword ptr [r12 + 1248], rax
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
 .string "TRIM"
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [r12 + 80], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 88], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n3_α
.Lx2_0:
 .quad 1
 xchain0_n2_α:
# BOX IR_CALL SNO$KWSET(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+64] -> [zr+32]
 mov rax, qword ptr [r12 + 64]
 mov qword ptr [r12 + 32], rax
 mov rax, qword ptr [r12 + 72]
 mov qword ptr [r12 + 40], rax
# marshal arg1 = producer-box slot [zr+80] -> [zr+48]
 mov rax, qword ptr [r12 + 80]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 88]
 mov qword ptr [r12 + 56], rax
  .section .rodata
  .Lrkfn4: .string "SNO$KWSET"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn4]
 lea rsi, [r12 + 32]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n3_α
 xchain0_n3_α:
# BOX IR_CALL stack_init(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn6: .string "stack_init"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn6]
 lea rsi, [r12 + 112]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 cmp eax, 99
 je xchain0_n4_α
 jmp xchain0_n4_α
xchain0_n3_β:
 jmp xchain0_n4_α
# IR_LIT_STRING
 xchain0_n4_α:
 mov qword ptr [r12 + 144], 1
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [r12 + 152], rax
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n6_α
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string "a"
 xchain0_n5_α:
# BOX IR_CALL stack_push(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+144] -> [zr+128]
 mov rax, qword ptr [r12 + 144]
 mov qword ptr [r12 + 128], rax
 mov rax, qword ptr [r12 + 152]
 mov qword ptr [r12 + 136], rax
  .section .rodata
  .Lbynamefn9: .string "stack_push"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn9]
 lea rsi, [r12 + 128]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 cmp eax, 99
 je xchain0_n6_α
 jmp xchain0_n6_α
xchain0_n5_β:
 jmp xchain0_n6_α
# IR_LIT_STRING
 xchain0_n6_α:
 mov qword ptr [r12 + 192], 1
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [r12 + 200], rax
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp xchain0_n8_α
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "b"
 xchain0_n7_α:
# BOX IR_CALL stack_push(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+192] -> [zr+176]
 mov rax, qword ptr [r12 + 192]
 mov qword ptr [r12 + 176], rax
 mov rax, qword ptr [r12 + 200]
 mov qword ptr [r12 + 184], rax
  .section .rodata
  .Lbynamefn12: .string "stack_push"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn12]
 lea rsi, [r12 + 176]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 cmp eax, 99
 je xchain0_n8_α
 jmp xchain0_n8_α
xchain0_n7_β:
 jmp xchain0_n8_α
# IR_LIT_STRING
 xchain0_n8_α:
 mov qword ptr [r12 + 240], 1
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [r12 + 248], rax
 jmp xchain0_n9_α
 xchain0_n8_β:
 jmp xchain0_n10_α
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "c"
 xchain0_n9_α:
# BOX IR_CALL stack_push(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+240] -> [zr+224]
 mov rax, qword ptr [r12 + 240]
 mov qword ptr [r12 + 224], rax
 mov rax, qword ptr [r12 + 248]
 mov qword ptr [r12 + 232], rax
  .section .rodata
  .Lbynamefn15: .string "stack_push"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn15]
 lea rsi, [r12 + 224]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 cmp eax, 99
 je xchain0_n10_α
 jmp xchain0_n10_α
xchain0_n9_β:
 jmp xchain0_n10_α
 xchain0_n10_α:
# BOX IR_CALL stack_depth(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn17: .string "stack_depth"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn17]
 lea rsi, [r12 + 288]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 cmp eax, 99
 je xchain0_n12_α
 jmp xchain0_n11_α
xchain0_n10_β:
 jmp xchain0_n12_α
# IR_ASSIGN global
 xchain0_n11_α:
 mov rsi, qword ptr [r12 + 272]
 mov rdx, qword ptr [r12 + 280]
 mov rdi, qword ptr [rip + .Lx18_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
 jmp xchain0_n12_α
 xchain0_n11_β:
 jmp xchain0_n12_α
.Lx18_0:
 .quad .Lx18_0_s
.Lx18_0_s:
 .string "OUTPUT"
 xchain0_n12_α:
# BOX IR_CALL stack_pop(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn20: .string "stack_pop"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn20]
 lea rsi, [r12 + 320]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 304], rax
 mov qword ptr [r12 + 312], rdx
 cmp eax, 99
 je xchain0_n14_α
 jmp xchain0_n13_α
xchain0_n12_β:
 jmp xchain0_n14_α
# IR_ASSIGN global
 xchain0_n13_α:
 mov rsi, qword ptr [r12 + 304]
 mov rdx, qword ptr [r12 + 312]
 mov rdi, qword ptr [rip + .Lx21_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 288], rax
 mov qword ptr [r12 + 296], rdx
 jmp xchain0_n14_α
 xchain0_n13_β:
 jmp xchain0_n14_α
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string "OUTPUT"
 xchain0_n14_α:
# BOX IR_CALL stack_pop(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn23: .string "stack_pop"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn23]
 lea rsi, [r12 + 352]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 cmp eax, 99
 je xchain0_n16_α
 jmp xchain0_n15_α
xchain0_n14_β:
 jmp xchain0_n16_α
# IR_ASSIGN global
 xchain0_n15_α:
 mov rsi, qword ptr [r12 + 336]
 mov rdx, qword ptr [r12 + 344]
 mov rdi, qword ptr [rip + .Lx24_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 jmp xchain0_n16_α
 xchain0_n15_β:
 jmp xchain0_n16_α
.Lx24_0:
 .quad .Lx24_0_s
.Lx24_0_s:
 .string "OUTPUT"
 xchain0_n16_α:
# BOX IR_CALL stack_depth(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn26: .string "stack_depth"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn26]
 lea rsi, [r12 + 384]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 368], rax
 mov qword ptr [r12 + 376], rdx
 cmp eax, 99
 je xchain0_n18_α
 jmp xchain0_n17_α
xchain0_n16_β:
 jmp xchain0_n18_α
# IR_ASSIGN global
 xchain0_n17_α:
 mov rsi, qword ptr [r12 + 368]
 mov rdx, qword ptr [r12 + 376]
 mov rdi, qword ptr [rip + .Lx27_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 352], rax
 mov qword ptr [r12 + 360], rdx
 jmp xchain0_n18_α
 xchain0_n17_β:
 jmp xchain0_n18_α
.Lx27_0:
 .quad .Lx27_0_s
.Lx27_0_s:
 .string "OUTPUT"
 xchain0_n18_α:
# BOX IR_CALL stack_pop(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn29: .string "stack_pop"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn29]
 lea rsi, [r12 + 416]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 400], rax
 mov qword ptr [r12 + 408], rdx
 cmp eax, 99
 je xchain0_n20_α
 jmp xchain0_n19_α
xchain0_n18_β:
 jmp xchain0_n20_α
# IR_ASSIGN global
 xchain0_n19_α:
 mov rsi, qword ptr [r12 + 400]
 mov rdx, qword ptr [r12 + 408]
 mov rdi, qword ptr [rip + .Lx30_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 jmp xchain0_n20_α
 xchain0_n19_β:
 jmp xchain0_n20_α
.Lx30_0:
 .quad .Lx30_0_s
.Lx30_0_s:
 .string "OUTPUT"
 xchain0_n20_α:
# BOX IR_CALL stack_depth(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn32: .string "stack_depth"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn32]
 lea rsi, [r12 + 448]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 432], rax
 mov qword ptr [r12 + 440], rdx
 cmp eax, 99
 je xchain0_n22_α
 jmp xchain0_n21_α
xchain0_n20_β:
 jmp xchain0_n22_α
# IR_ASSIGN global
 xchain0_n21_α:
 mov rsi, qword ptr [r12 + 432]
 mov rdx, qword ptr [r12 + 440]
 mov rdi, qword ptr [rip + .Lx33_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 jmp xchain0_n22_α
 xchain0_n21_β:
 jmp xchain0_n22_α
.Lx33_0:
 .quad .Lx33_0_s
.Lx33_0_s:
 .string "OUTPUT"
 xchain0_n22_α:
# BOX IR_CALL stack_pop(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn35: .string "stack_pop"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn35]
 lea rsi, [r12 + 464]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 448], rax
 mov qword ptr [r12 + 456], rdx
 cmp eax, 99
 je xchain0_n24_α
 jmp xchain0_n23_α
xchain0_n22_β:
 jmp xchain0_n24_α
# IR_LIT_STRING
 xchain0_n23_α:
 mov qword ptr [r12 + 512], 1
 mov rax, qword ptr [rip + .Lx36_0]
 mov qword ptr [r12 + 520], rax
 jmp xchain0_n25_α
 xchain0_n23_β:
 jmp xchain0_n27_α
.Lx36_0:
 .quad .Lx36_0_s
.Lx36_0_s:
 .string "FAIL: empty pop should FRETURN"
# IR_LIT_STRING
 xchain0_n24_α:
 mov qword ptr [r12 + 480], 1
 mov rax, qword ptr [rip + .Lx37_0]
 mov qword ptr [r12 + 488], rax
 jmp xchain0_n26_α
 xchain0_n24_β:
 jmp xchain0_n27_α
.Lx37_0:
 .quad .Lx37_0_s
.Lx37_0_s:
 .string "empty ok"
# IR_ASSIGN global
 xchain0_n25_α:
 mov rsi, qword ptr [r12 + 512]
 mov rdx, qword ptr [r12 + 520]
 mov rdi, qword ptr [rip + .Lx38_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 496], rax
 mov qword ptr [r12 + 504], rdx
 jmp xchain0_n27_α
 xchain0_n25_β:
 jmp xchain0_n27_α
.Lx38_0:
 .quad .Lx38_0_s
.Lx38_0_s:
 .string "OUTPUT"
# IR_ASSIGN global
 xchain0_n26_α:
 mov rsi, qword ptr [r12 + 480]
 mov rdx, qword ptr [r12 + 488]
 mov rdi, qword ptr [rip + .Lx39_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 464], rax
 mov qword ptr [r12 + 472], rdx
 jmp xchain0_n27_α
 xchain0_n26_β:
 jmp xchain0_n27_α
.Lx39_0:
 .quad .Lx39_0_s
.Lx39_0_s:
 .string "OUTPUT"
 xchain0_n27_α:
# BOX IR_CALL stack_init(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn41: .string "stack_init"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn41]
 lea rsi, [r12 + 544]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 528], rax
 mov qword ptr [r12 + 536], rdx
 cmp eax, 99
 je xchain0_n28_α
 jmp xchain0_n28_α
xchain0_n27_β:
 jmp xchain0_n28_α
# IR_LIT_STRING
 xchain0_n28_α:
 mov qword ptr [r12 + 576], 1
 mov rax, qword ptr [rip + .Lx42_0]
 mov qword ptr [r12 + 584], rax
 jmp xchain0_n29_α
 xchain0_n28_β:
 jmp xchain0_n30_α
.Lx42_0:
 .quad .Lx42_0_s
.Lx42_0_s:
 .string "x"
 xchain0_n29_α:
# BOX IR_CALL stack_push(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+576] -> [zr+560]
 mov rax, qword ptr [r12 + 576]
 mov qword ptr [r12 + 560], rax
 mov rax, qword ptr [r12 + 584]
 mov qword ptr [r12 + 568], rax
  .section .rodata
  .Lbynamefn44: .string "stack_push"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn44]
 lea rsi, [r12 + 560]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 544], rax
 mov qword ptr [r12 + 552], rdx
 cmp eax, 99
 je xchain0_n30_α
 jmp xchain0_n30_α
xchain0_n29_β:
 jmp xchain0_n30_α
 xchain0_n30_α:
# BOX IR_CALL stack_peek(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn46: .string "stack_peek"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn46]
 lea rsi, [r12 + 624]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 608], rax
 mov qword ptr [r12 + 616], rdx
 cmp eax, 99
 je xchain0_n32_α
 jmp xchain0_n31_α
xchain0_n30_β:
 jmp xchain0_n32_α
# IR_ASSIGN global
 xchain0_n31_α:
 mov rsi, qword ptr [r12 + 608]
 mov rdx, qword ptr [r12 + 616]
 mov rdi, qword ptr [rip + .Lx47_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 592], rax
 mov qword ptr [r12 + 600], rdx
 jmp xchain0_n32_α
 xchain0_n31_β:
 jmp xchain0_n32_α
.Lx47_0:
 .quad .Lx47_0_s
.Lx47_0_s:
 .string "OUTPUT"
 xchain0_n32_α:
# BOX IR_CALL stack_depth(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn49: .string "stack_depth"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn49]
 lea rsi, [r12 + 656]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 640], rax
 mov qword ptr [r12 + 648], rdx
 cmp eax, 99
 je xchain0_n34_α
 jmp xchain0_n33_α
xchain0_n32_β:
 jmp xchain0_n34_α
# IR_ASSIGN global
 xchain0_n33_α:
 mov rsi, qword ptr [r12 + 640]
 mov rdx, qword ptr [r12 + 648]
 mov rdi, qword ptr [rip + .Lx50_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 624], rax
 mov qword ptr [r12 + 632], rdx
 jmp xchain0_n34_α
 xchain0_n33_β:
 jmp xchain0_n34_α
.Lx50_0:
 .quad .Lx50_0_s
.Lx50_0_s:
 .string "OUTPUT"
 xchain0_n34_α:
# BOX IR_CALL stack_pop(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn52: .string "stack_pop"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn52]
 lea rsi, [r12 + 688]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 672], rax
 mov qword ptr [r12 + 680], rdx
 cmp eax, 99
 je xchain0_n36_α
 jmp xchain0_n35_α
xchain0_n34_β:
 jmp xchain0_n36_α
# IR_ASSIGN global
 xchain0_n35_α:
 mov rsi, qword ptr [r12 + 672]
 mov rdx, qword ptr [r12 + 680]
 mov rdi, qword ptr [rip + .Lx53_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 656], rax
 mov qword ptr [r12 + 664], rdx
 jmp xchain0_n36_α
 xchain0_n35_β:
 jmp xchain0_n36_α
.Lx53_0:
 .quad .Lx53_0_s
.Lx53_0_s:
 .string "OUTPUT"
 xchain0_n36_α:
# BOX IR_CALL stack_init(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn55: .string "stack_init"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn55]
 lea rsi, [r12 + 704]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 688], rax
 mov qword ptr [r12 + 696], rdx
 cmp eax, 99
 je xchain0_n37_α
 jmp xchain0_n37_α
xchain0_n36_β:
 jmp xchain0_n37_α
# IR_LIT_INTEGER
 xchain0_n37_α:
 mov qword ptr [r12 + 736], 6
 mov rax, qword ptr [rip + .Lx56_0]
 mov qword ptr [r12 + 744], rax
 jmp xchain0_n38_α
 xchain0_n37_β:
 jmp xchain0_n39_α
.Lx56_0:
 .quad 42
 xchain0_n38_α:
# BOX IR_CALL stack_push(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+736] -> [zr+720]
 mov rax, qword ptr [r12 + 736]
 mov qword ptr [r12 + 720], rax
 mov rax, qword ptr [r12 + 744]
 mov qword ptr [r12 + 728], rax
  .section .rodata
  .Lbynamefn58: .string "stack_push"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn58]
 lea rsi, [r12 + 720]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 704], rax
 mov qword ptr [r12 + 712], rdx
 cmp eax, 99
 je xchain0_n39_α
 jmp xchain0_n39_α
xchain0_n38_β:
 jmp xchain0_n39_α
# IR_LIT_INTEGER
 xchain0_n39_α:
 mov qword ptr [r12 + 784], 6
 mov rax, qword ptr [rip + .Lx59_0]
 mov qword ptr [r12 + 792], rax
 jmp xchain0_n40_α
 xchain0_n39_β:
 jmp xchain0_n41_α
.Lx59_0:
 .quad 99
 xchain0_n40_α:
# BOX IR_CALL stack_push(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+784] -> [zr+768]
 mov rax, qword ptr [r12 + 784]
 mov qword ptr [r12 + 768], rax
 mov rax, qword ptr [r12 + 792]
 mov qword ptr [r12 + 776], rax
  .section .rodata
  .Lbynamefn61: .string "stack_push"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn61]
 lea rsi, [r12 + 768]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 752], rax
 mov qword ptr [r12 + 760], rdx
 cmp eax, 99
 je xchain0_n41_α
 jmp xchain0_n41_α
xchain0_n40_β:
 jmp xchain0_n41_α
# IR_LIT_STRING
 xchain0_n41_α:
 mov qword ptr [r12 + 832], 1
 mov rax, qword ptr [rip + .Lx62_0]
 mov qword ptr [r12 + 840], rax
 jmp xchain0_n42_α
 xchain0_n41_β:
 jmp xchain0_n43_α
.Lx62_0:
 .quad .Lx62_0_s
.Lx62_0_s:
 .string "myvar"
 xchain0_n42_α:
# BOX IR_CALL stack_pop(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+832] -> [zr+816]
 mov rax, qword ptr [r12 + 832]
 mov qword ptr [r12 + 816], rax
 mov rax, qword ptr [r12 + 840]
 mov qword ptr [r12 + 824], rax
  .section .rodata
  .Lbynamefn64: .string "stack_pop"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn64]
 lea rsi, [r12 + 816]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 800], rax
 mov qword ptr [r12 + 808], rdx
 cmp eax, 99
 je xchain0_n43_α
 jmp xchain0_n43_α
xchain0_n42_β:
 jmp xchain0_n43_α
# IR_VAR
 xchain0_n43_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 864], rax
 mov qword ptr [r12 + 872], rdx
 jmp xchain0_n44_α
 xchain0_n43_β:
 jmp xchain0_n45_α
# IR_ASSIGN global
 xchain0_n44_α:
 mov rsi, qword ptr [r12 + 864]
 mov rdx, qword ptr [r12 + 872]
 mov rdi, qword ptr [rip + .Lx66_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 848], rax
 mov qword ptr [r12 + 856], rdx
 jmp xchain0_n45_α
 xchain0_n44_β:
 jmp xchain0_n45_α
.Lx66_0:
 .quad .Lx66_0_s
.Lx66_0_s:
 .string "OUTPUT"
 xchain0_n45_α:
# BOX IR_CALL stack_init(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn68: .string "stack_init"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn68]
 lea rsi, [r12 + 896]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 880], rax
 mov qword ptr [r12 + 888], rdx
 cmp eax, 99
 je xchain0_n46_α
 jmp xchain0_n46_α
xchain0_n45_β:
 jmp xchain0_n46_α
# IR_LIT_STRING
 xchain0_n46_α:
 mov qword ptr [r12 + 912], 1
 mov rax, qword ptr [rip + .Lx69_0]
 mov qword ptr [r12 + 920], rax
 jmp xchain0_n47_α
 xchain0_n46_β:
 jmp xchain0_n48_α
.Lx69_0:
 .quad .Lx69_0_s
.Lx69_0_s:
 .string "hello world"
# IR_ASSIGN gva
 xchain0_n47_α:
 mov rax, qword ptr [r12 + 912]
 mov rdx, qword ptr [r12 + 920]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 896], rax
 mov qword ptr [r12 + 904], rdx
 jmp xchain0_n48_α
 xchain0_n47_β:
 jmp xchain0_n48_α
# IR_VAR
 xchain0_n48_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 1040], rax
 mov qword ptr [r12 + 1048], rdx
 jmp xchain0_n49_α
 xchain0_n48_β:
 jmp xchain0_n50_α
# IR_MATCH_HEAD
 xchain0_n49_α:
 call rt_zls_mark@PLT
 mov qword ptr [r12 + 936], rax
 mov qword ptr [r12 + 944], rsp
 mov rdi, qword ptr [r12 + 1040]
 mov rsi, qword ptr [r12 + 1048]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov dword ptr [r12 + 928], 0
.Lx73_0:
 mov r14d, dword ptr [r12 + 928]
 jmp xchain0_n51_α
 xchain0_n49_β:
 add dword ptr [r12 + 928], 1
 mov eax, dword ptr [r12 + 928]
 cmp eax, r15d
 jg .Lx73_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx73_1
 jmp .Lx73_0
.Lx73_1:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 936]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
 mov rsp, qword ptr [r12 + 944]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_end_fail@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n50_α
# IR_LIT_STRING
 xchain0_n50_α:
 mov qword ptr [r12 + 1232], 1
 mov rax, qword ptr [rip + .Lx74_0]
 mov qword ptr [r12 + 1240], rax
 jmp xchain0_n52_α
 xchain0_n50_β:
 jmp main_γ
.Lx74_0:
 .quad .Lx74_0_s
.Lx74_0_s:
 .string "FAIL: pattern match failed"
# IR_MATCH_CAPTURE_SAVE push
 xchain0_n51_α:
 lea rdi, [r12 + 1008]
 mov esi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_cap_push@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n53_α
 xchain0_n51_β:
 lea rdi, [r12 + 1008]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_cap_pop@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n49_β
# IR_ASSIGN global
 xchain0_n52_α:
 mov rsi, qword ptr [r12 + 1232]
 mov rdx, qword ptr [r12 + 1240]
 mov rdi, qword ptr [rip + .Lx77_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 1216], rax
 mov qword ptr [r12 + 1224], rdx
 jmp main_γ
 xchain0_n52_β:
 jmp main_γ
.Lx77_0:
 .quad .Lx77_0_s
.Lx77_0_s:
 .string "OUTPUT"
# IR_MATCH_BREAK
 xchain0_n53_α:
 mov dword ptr [r12 + 1024], 0
.Lx79_0:
 mov eax, r14d
 add eax, dword ptr [r12 + 1024]
 cmp eax, r15d
 jge xchain0_n51_β
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S0]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 jnz .Lx79_1
 add dword ptr [r12 + 1024], 1
 jmp .Lx79_0
.Lx79_1:
 mov eax, r14d
 add eax, dword ptr [r12 + 1024]
 mov r14d, eax
 jmp xchain0_n54_α
 xchain0_n53_β:
 mov eax, r14d
 sub eax, dword ptr [r12 + 1024]
 mov r14d, eax
 jmp xchain0_n51_β
# IR_MATCH_CAPTURE_COND
 xchain0_n54_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 lea rdi, [r12 + 1008]
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
 lea rdi, [r12 + 976]
 mov esi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_cap_push@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n57_α
 xchain0_n56_β:
 lea rdi, [r12 + 976]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_cap_pop@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n49_β
# IR_MATCH_REM
 xchain0_n57_α:
 mov dword ptr [r12 + 992], r14d
 mov r14d, r15d
 jmp xchain0_n58_α
 xchain0_n57_β:
 mov r14d, dword ptr [r12 + 992]
 jmp xchain0_n56_β
# IR_MATCH_CAPTURE_COND
 xchain0_n58_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 lea rdi, [r12 + 976]
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
 jmp xchain0_n56_β
# IR_MATCH_RELEASE
 xchain0_n59_α:
 mov qword ptr [r12 + 952], r14
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 936]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
 mov rsp, qword ptr [r12 + 944]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_end_ok@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n60_α
# IR_LIT_STRING
 xchain0_n60_α:
 mov qword ptr [r12 + 960], 1
 mov rax, qword ptr [rip + .Lx91_0]
 mov qword ptr [r12 + 968], rax
 jmp xchain0_n61_α
 xchain0_n60_β:
 jmp xchain0_n50_α
.Lx91_0:
 .quad .Lx91_0_s
.Lx91_0_s:
 .string ""
# IR_MATCH_REPLACE
 xchain0_n61_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx93_0]
 mov rsi, qword ptr [r12 + 1040]
 mov rdx, qword ptr [r12 + 1048]
 mov ecx, dword ptr [r12 + 928]
 mov r8, qword ptr [r12 + 952]
 lea r9, [r12 + 960]
 call rt_match_replace@PLT
 mov rsp, rbp
 pop rbp
 jmp .Lx93_1
.Lx93_0:
 .quad .Lx93_0_s
.Lx93_0_s:
 .string "subject"
.Lx93_1:
 jmp xchain0_n62_α
# IR_VAR
 xchain0_n62_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 1088], rax
 mov qword ptr [r12 + 1096], rdx
 jmp xchain0_n63_α
 xchain0_n62_β:
 jmp xchain0_n64_α
 xchain0_n63_α:
# BOX IR_CALL stack_push(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1088] -> [zr+1072]
 mov rax, qword ptr [r12 + 1088]
 mov qword ptr [r12 + 1072], rax
 mov rax, qword ptr [r12 + 1096]
 mov qword ptr [r12 + 1080], rax
  .section .rodata
  .Lbynamefn96: .string "stack_push"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn96]
 lea rsi, [r12 + 1072]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1056], rax
 mov qword ptr [r12 + 1064], rdx
 cmp eax, 99
 je xchain0_n64_α
 jmp xchain0_n64_α
xchain0_n63_β:
 jmp xchain0_n64_α
# IR_VAR
 xchain0_n64_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 1136], rax
 mov qword ptr [r12 + 1144], rdx
 jmp xchain0_n65_α
 xchain0_n64_β:
 jmp xchain0_n66_α
 xchain0_n65_α:
# BOX IR_CALL stack_push(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1136] -> [zr+1120]
 mov rax, qword ptr [r12 + 1136]
 mov qword ptr [r12 + 1120], rax
 mov rax, qword ptr [r12 + 1144]
 mov qword ptr [r12 + 1128], rax
  .section .rodata
  .Lbynamefn99: .string "stack_push"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn99]
 lea rsi, [r12 + 1120]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1104], rax
 mov qword ptr [r12 + 1112], rdx
 cmp eax, 99
 je xchain0_n66_α
 jmp xchain0_n66_α
xchain0_n65_β:
 jmp xchain0_n66_α
 xchain0_n66_α:
# BOX IR_CALL stack_pop(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn101: .string "stack_pop"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn101]
 lea rsi, [r12 + 1184]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1168], rax
 mov qword ptr [r12 + 1176], rdx
 cmp eax, 99
 je xchain0_n68_α
 jmp xchain0_n67_α
xchain0_n66_β:
 jmp xchain0_n68_α
# IR_ASSIGN global
 xchain0_n67_α:
 mov rsi, qword ptr [r12 + 1168]
 mov rdx, qword ptr [r12 + 1176]
 mov rdi, qword ptr [rip + .Lx102_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 1152], rax
 mov qword ptr [r12 + 1160], rdx
 jmp xchain0_n68_α
 xchain0_n67_β:
 jmp xchain0_n68_α
.Lx102_0:
 .quad .Lx102_0_s
.Lx102_0_s:
 .string "OUTPUT"
 xchain0_n68_α:
# BOX IR_CALL stack_pop(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn104: .string "stack_pop"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn104]
 lea rsi, [r12 + 1216]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1200], rax
 mov qword ptr [r12 + 1208], rdx
 cmp eax, 99
 je main_γ
 jmp xchain0_n69_α
xchain0_n68_β:
 jmp main_γ
# IR_ASSIGN global
 xchain0_n69_α:
 mov rsi, qword ptr [r12 + 1200]
 mov rdx, qword ptr [r12 + 1208]
 mov rdi, qword ptr [rip + .Lx105_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 1184], rax
 mov qword ptr [r12 + 1192], rdx
 jmp main_γ
 xchain0_n69_β:
 jmp main_γ
.Lx105_0:
 .quad .Lx105_0_s
.Lx105_0_s:
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
 mov rdi, qword ptr [r12 + 1248]
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

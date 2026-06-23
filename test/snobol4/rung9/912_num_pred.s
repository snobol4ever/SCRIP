  .intel_syntax noprefix
  .text
  .globl main
main:
  push rbp
  mov rbp, rsp
  call core_lib_init@PLT
  call rt_proc_reset@PLT
  call rt_frame@PLT
  mov rdi, rax
  xor esi, esi
  call flat_α
  xor eax, eax
  pop rbp
  ret
flat_α:
#=======================================================================================================================
    .global flat_α
    .global flat_β
    .global flat_γ
    .global flat_ω
push r12
  mov r12, rdi
  lea r10, [rip + Δ]
flat_α_body:
snoch0_n0_α:
# IR_LIT_I
bb1_α:
 mov qword ptr [r12 + 16], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 24], rax
 jmp xgvarg1_done
 xgvarg1_β:
 jmp snoch0_n2_α
.Lx2_0:
 .quad 5
xgvarg1_done:
# IR_LIT_I
bb2_α:
 mov qword ptr [r12 + 32], 6
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [r12 + 40], rax
 jmp xgvarg3_done
 xgvarg3_β:
 jmp snoch0_n2_α
.Lx4_0:
 .quad 4
xgvarg3_done:
bb3_α:
# BOX IR_CALL lt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+16] -> [r12+64]
 mov rax, qword ptr [r12 + 16]
 mov qword ptr [r12 + 64], rax
 mov rax, qword ptr [r12 + 24]
 mov qword ptr [r12 + 72], rax
# marshal arg1 = producer-box slot [r12+32] -> [r12+80]
 mov rax, qword ptr [r12 + 32]
 mov qword ptr [r12 + 80], rax
 mov rax, qword ptr [r12 + 40]
 mov qword ptr [r12 + 88], rax
  .section .rodata
  .Lbynamefn6: .string "lt"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn6]
 lea rsi, [r12 + 64]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 48], rax
 mov qword ptr [r12 + 56], rdx
 cmp eax, 99
 je snoch0_n2_α
 jmp snoch0_n1_α
snoch0_n0_β:
 jmp snoch0_n2_α
snoch0_n1_α:
# IR_LIT_scalar
bb4_α:
 jmp snoch0_n3_α
 snoch0_n1_β:
 jmp flat_γ
snoch0_n2_α:
# IR_LIT_I
bb5_α:
 mov qword ptr [r12 + 96], 6
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 104], rax
 jmp xgvarg8_done
 xgvarg8_β:
 jmp snoch0_n5_α
.Lx9_0:
 .quad 4
xgvarg8_done:
# IR_LIT_I
bb6_α:
 mov qword ptr [r12 + 112], 6
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 120], rax
 jmp xgvarg10_done
 xgvarg10_β:
 jmp snoch0_n5_α
.Lx11_0:
 .quad 4
xgvarg10_done:
bb7_α:
# BOX IR_CALL lt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+96] -> [r12+144]
 mov rax, qword ptr [r12 + 96]
 mov qword ptr [r12 + 144], rax
 mov rax, qword ptr [r12 + 104]
 mov qword ptr [r12 + 152], rax
# marshal arg1 = producer-box slot [r12+112] -> [r12+160]
 mov rax, qword ptr [r12 + 112]
 mov qword ptr [r12 + 160], rax
 mov rax, qword ptr [r12 + 120]
 mov qword ptr [r12 + 168], rax
  .section .rodata
  .Lbynamefn13: .string "lt"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn13]
 lea rsi, [r12 + 144]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
 cmp eax, 99
 je snoch0_n5_α
 jmp snoch0_n4_α
snoch0_n2_β:
 jmp snoch0_n5_α
snoch0_n3_α:
bb8_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S1]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n3_β:
 jmp flat_γ
snoch0_n4_α:
# IR_LIT_scalar
bb9_α:
 jmp snoch0_n6_α
 snoch0_n4_β:
 jmp flat_γ
snoch0_n5_α:
# IR_LIT_I
bb10_α:
 mov qword ptr [r12 + 176], 6
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [r12 + 184], rax
 jmp xgvarg16_done
 xgvarg16_β:
 jmp snoch0_n8_α
.Lx17_0:
 .quad 4
xgvarg16_done:
# IR_LIT_I
bb11_α:
 mov qword ptr [r12 + 192], 6
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [r12 + 200], rax
 jmp xgvarg18_done
 xgvarg18_β:
 jmp snoch0_n8_α
.Lx19_0:
 .quad 5
xgvarg18_done:
bb12_α:
# BOX IR_CALL lt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+176] -> [r12+224]
 mov rax, qword ptr [r12 + 176]
 mov qword ptr [r12 + 224], rax
 mov rax, qword ptr [r12 + 184]
 mov qword ptr [r12 + 232], rax
# marshal arg1 = producer-box slot [r12+192] -> [r12+240]
 mov rax, qword ptr [r12 + 192]
 mov qword ptr [r12 + 240], rax
 mov rax, qword ptr [r12 + 200]
 mov qword ptr [r12 + 248], rax
  .section .rodata
  .Lbynamefn21: .string "lt"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn21]
 lea rsi, [r12 + 224]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 cmp eax, 99
 je snoch0_n8_α
 jmp snoch0_n7_α
snoch0_n5_β:
 jmp snoch0_n8_α
snoch0_n6_α:
bb13_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S2]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n6_β:
 jmp flat_γ
snoch0_n7_α:
# IR_LIT_I
bb14_α:
 mov qword ptr [r12 + 256], 6
 mov rax, qword ptr [rip + .Lx24_0]
 mov qword ptr [r12 + 264], rax
 jmp xgvarg23_done
 xgvarg23_β:
 jmp snoch0_n10_α
.Lx24_0:
 .quad 5
xgvarg23_done:
# IR_LIT_I
bb15_α:
 mov qword ptr [r12 + 272], 6
 mov rax, qword ptr [rip + .Lx26_0]
 mov qword ptr [r12 + 280], rax
 jmp xgvarg25_done
 xgvarg25_β:
 jmp snoch0_n10_α
.Lx26_0:
 .quad 2
xgvarg25_done:
bb16_α:
# BOX IR_CALL le(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+256] -> [r12+304]
 mov rax, qword ptr [r12 + 256]
 mov qword ptr [r12 + 304], rax
 mov rax, qword ptr [r12 + 264]
 mov qword ptr [r12 + 312], rax
# marshal arg1 = producer-box slot [r12+272] -> [r12+320]
 mov rax, qword ptr [r12 + 272]
 mov qword ptr [r12 + 320], rax
 mov rax, qword ptr [r12 + 280]
 mov qword ptr [r12 + 328], rax
  .section .rodata
  .Lbynamefn28: .string "le"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn28]
 lea rsi, [r12 + 304]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 288], rax
 mov qword ptr [r12 + 296], rdx
 cmp eax, 99
 je snoch0_n10_α
 jmp snoch0_n9_α
snoch0_n7_β:
 jmp snoch0_n10_α
snoch0_n8_α:
# IR_LIT_scalar
bb17_α:
 jmp snoch0_n11_α
 snoch0_n8_β:
 jmp flat_γ
snoch0_n9_α:
# IR_LIT_scalar
bb18_α:
 jmp snoch0_n12_α
 snoch0_n9_β:
 jmp flat_γ
snoch0_n10_α:
# IR_LIT_I
bb19_α:
 mov qword ptr [r12 + 336], 6
 mov rax, qword ptr [rip + .Lx32_0]
 mov qword ptr [r12 + 344], rax
 jmp xgvarg31_done
 xgvarg31_β:
 jmp snoch0_n14_α
.Lx32_0:
 .quad 4
xgvarg31_done:
# IR_LIT_I
bb20_α:
 mov qword ptr [r12 + 352], 6
 mov rax, qword ptr [rip + .Lx34_0]
 mov qword ptr [r12 + 360], rax
 jmp xgvarg33_done
 xgvarg33_β:
 jmp snoch0_n14_α
.Lx34_0:
 .quad 4
xgvarg33_done:
bb21_α:
# BOX IR_CALL le(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+336] -> [r12+384]
 mov rax, qword ptr [r12 + 336]
 mov qword ptr [r12 + 384], rax
 mov rax, qword ptr [r12 + 344]
 mov qword ptr [r12 + 392], rax
# marshal arg1 = producer-box slot [r12+352] -> [r12+400]
 mov rax, qword ptr [r12 + 352]
 mov qword ptr [r12 + 400], rax
 mov rax, qword ptr [r12 + 360]
 mov qword ptr [r12 + 408], rax
  .section .rodata
  .Lbynamefn36: .string "le"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn36]
 lea rsi, [r12 + 384]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 368], rax
 mov qword ptr [r12 + 376], rdx
 cmp eax, 99
 je snoch0_n14_α
 jmp snoch0_n13_α
snoch0_n10_β:
 jmp snoch0_n14_α
snoch0_n11_α:
bb22_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S3]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n11_β:
 jmp flat_γ
snoch0_n12_α:
bb23_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S4]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n12_β:
 jmp flat_γ
snoch0_n13_α:
# IR_LIT_I
bb24_α:
 mov qword ptr [r12 + 416], 6
 mov rax, qword ptr [rip + .Lx40_0]
 mov qword ptr [r12 + 424], rax
 jmp xgvarg39_done
 xgvarg39_β:
 jmp snoch0_n16_α
.Lx40_0:
 .quad 4
xgvarg39_done:
# IR_LIT_I
bb25_α:
 mov qword ptr [r12 + 432], 6
 mov rax, qword ptr [rip + .Lx42_0]
 mov qword ptr [r12 + 440], rax
 jmp xgvarg41_done
 xgvarg41_β:
 jmp snoch0_n16_α
.Lx42_0:
 .quad 10
xgvarg41_done:
bb26_α:
# BOX IR_CALL le(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+416] -> [r12+464]
 mov rax, qword ptr [r12 + 416]
 mov qword ptr [r12 + 464], rax
 mov rax, qword ptr [r12 + 424]
 mov qword ptr [r12 + 472], rax
# marshal arg1 = producer-box slot [r12+432] -> [r12+480]
 mov rax, qword ptr [r12 + 432]
 mov qword ptr [r12 + 480], rax
 mov rax, qword ptr [r12 + 440]
 mov qword ptr [r12 + 488], rax
  .section .rodata
  .Lbynamefn44: .string "le"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn44]
 lea rsi, [r12 + 464]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 448], rax
 mov qword ptr [r12 + 456], rdx
 cmp eax, 99
 je snoch0_n16_α
 jmp snoch0_n15_α
snoch0_n13_β:
 jmp snoch0_n16_α
snoch0_n14_α:
# IR_LIT_scalar
bb27_α:
 jmp snoch0_n17_α
 snoch0_n14_β:
 jmp flat_γ
snoch0_n15_α:
# IR_LIT_I
bb28_α:
 mov qword ptr [r12 + 496], 6
 mov rax, qword ptr [rip + .Lx47_0]
 mov qword ptr [r12 + 504], rax
 jmp xgvarg46_done
 xgvarg46_β:
 jmp snoch0_n19_α
.Lx47_0:
 .quad 4
xgvarg46_done:
# IR_LIT_I
bb29_α:
 mov qword ptr [r12 + 512], 6
 mov rax, qword ptr [rip + .Lx49_0]
 mov qword ptr [r12 + 520], rax
 jmp xgvarg48_done
 xgvarg48_β:
 jmp snoch0_n19_α
.Lx49_0:
 .quad 5
xgvarg48_done:
bb30_α:
# BOX IR_CALL eq(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+496] -> [r12+544]
 mov rax, qword ptr [r12 + 496]
 mov qword ptr [r12 + 544], rax
 mov rax, qword ptr [r12 + 504]
 mov qword ptr [r12 + 552], rax
# marshal arg1 = producer-box slot [r12+512] -> [r12+560]
 mov rax, qword ptr [r12 + 512]
 mov qword ptr [r12 + 560], rax
 mov rax, qword ptr [r12 + 520]
 mov qword ptr [r12 + 568], rax
  .section .rodata
  .Lbynamefn51: .string "eq"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn51]
 lea rsi, [r12 + 544]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 528], rax
 mov qword ptr [r12 + 536], rdx
 cmp eax, 99
 je snoch0_n19_α
 jmp snoch0_n18_α
snoch0_n15_β:
 jmp snoch0_n19_α
snoch0_n16_α:
# IR_LIT_scalar
bb31_α:
 jmp snoch0_n20_α
 snoch0_n16_β:
 jmp flat_γ
snoch0_n17_α:
bb32_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S5]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n17_β:
 jmp flat_γ
snoch0_n18_α:
# IR_LIT_scalar
bb33_α:
 jmp snoch0_n21_α
 snoch0_n18_β:
 jmp flat_γ
snoch0_n19_α:
# IR_LIT_I
bb34_α:
 mov qword ptr [r12 + 576], 6
 mov rax, qword ptr [rip + .Lx56_0]
 mov qword ptr [r12 + 584], rax
 jmp xgvarg55_done
 xgvarg55_β:
 jmp snoch0_n23_α
.Lx56_0:
 .quad 5
xgvarg55_done:
# IR_LIT_I
bb35_α:
 mov qword ptr [r12 + 592], 6
 mov rax, qword ptr [rip + .Lx58_0]
 mov qword ptr [r12 + 600], rax
 jmp xgvarg57_done
 xgvarg57_β:
 jmp snoch0_n23_α
.Lx58_0:
 .quad 5
xgvarg57_done:
bb36_α:
# BOX IR_CALL eq(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+576] -> [r12+624]
 mov rax, qword ptr [r12 + 576]
 mov qword ptr [r12 + 624], rax
 mov rax, qword ptr [r12 + 584]
 mov qword ptr [r12 + 632], rax
# marshal arg1 = producer-box slot [r12+592] -> [r12+640]
 mov rax, qword ptr [r12 + 592]
 mov qword ptr [r12 + 640], rax
 mov rax, qword ptr [r12 + 600]
 mov qword ptr [r12 + 648], rax
  .section .rodata
  .Lbynamefn60: .string "eq"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn60]
 lea rsi, [r12 + 624]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 608], rax
 mov qword ptr [r12 + 616], rdx
 cmp eax, 99
 je snoch0_n23_α
 jmp snoch0_n22_α
snoch0_n19_β:
 jmp snoch0_n23_α
snoch0_n20_α:
bb37_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S6]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n20_β:
 jmp flat_γ
snoch0_n21_α:
bb38_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S7]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n21_β:
 jmp flat_γ
snoch0_n22_α:
# IR_LIT_I
bb39_α:
 mov qword ptr [r12 + 656], 6
 mov rax, qword ptr [rip + .Lx64_0]
 mov qword ptr [r12 + 664], rax
 jmp xgvarg63_done
 xgvarg63_β:
 jmp snoch0_n25_α
.Lx64_0:
 .quad 4
xgvarg63_done:
# IR_LIT_I
bb40_α:
 mov qword ptr [r12 + 672], 6
 mov rax, qword ptr [rip + .Lx66_0]
 mov qword ptr [r12 + 680], rax
 jmp xgvarg65_done
 xgvarg65_β:
 jmp snoch0_n25_α
.Lx66_0:
 .quad 4
xgvarg65_done:
bb41_α:
# BOX IR_CALL ne(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+656] -> [r12+704]
 mov rax, qword ptr [r12 + 656]
 mov qword ptr [r12 + 704], rax
 mov rax, qword ptr [r12 + 664]
 mov qword ptr [r12 + 712], rax
# marshal arg1 = producer-box slot [r12+672] -> [r12+720]
 mov rax, qword ptr [r12 + 672]
 mov qword ptr [r12 + 720], rax
 mov rax, qword ptr [r12 + 680]
 mov qword ptr [r12 + 728], rax
  .section .rodata
  .Lbynamefn68: .string "ne"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn68]
 lea rsi, [r12 + 704]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 688], rax
 mov qword ptr [r12 + 696], rdx
 cmp eax, 99
 je snoch0_n25_α
 jmp snoch0_n24_α
snoch0_n22_β:
 jmp snoch0_n25_α
snoch0_n23_α:
# IR_LIT_scalar
bb42_α:
 jmp snoch0_n26_α
 snoch0_n23_β:
 jmp flat_γ
snoch0_n24_α:
# IR_LIT_scalar
bb43_α:
 jmp snoch0_n27_α
 snoch0_n24_β:
 jmp flat_γ
snoch0_n25_α:
# IR_LIT_I
bb44_α:
 mov qword ptr [r12 + 736], 6
 mov rax, qword ptr [rip + .Lx72_0]
 mov qword ptr [r12 + 744], rax
 jmp xgvarg71_done
 xgvarg71_β:
 jmp snoch0_n29_α
.Lx72_0:
 .quad 4
xgvarg71_done:
# IR_LIT_I
bb45_α:
 mov qword ptr [r12 + 752], 6
 mov rax, qword ptr [rip + .Lx74_0]
 mov qword ptr [r12 + 760], rax
 jmp xgvarg73_done
 xgvarg73_β:
 jmp snoch0_n29_α
.Lx74_0:
 .quad 6
xgvarg73_done:
bb46_α:
# BOX IR_CALL ne(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+736] -> [r12+784]
 mov rax, qword ptr [r12 + 736]
 mov qword ptr [r12 + 784], rax
 mov rax, qword ptr [r12 + 744]
 mov qword ptr [r12 + 792], rax
# marshal arg1 = producer-box slot [r12+752] -> [r12+800]
 mov rax, qword ptr [r12 + 752]
 mov qword ptr [r12 + 800], rax
 mov rax, qword ptr [r12 + 760]
 mov qword ptr [r12 + 808], rax
  .section .rodata
  .Lbynamefn76: .string "ne"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn76]
 lea rsi, [r12 + 784]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 768], rax
 mov qword ptr [r12 + 776], rdx
 cmp eax, 99
 je snoch0_n29_α
 jmp snoch0_n28_α
snoch0_n25_β:
 jmp snoch0_n29_α
snoch0_n26_α:
bb47_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S8]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n26_β:
 jmp flat_γ
snoch0_n27_α:
bb48_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S9]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n27_β:
 jmp flat_γ
snoch0_n28_α:
# IR_LIT_I
bb49_α:
 mov qword ptr [r12 + 816], 6
 mov rax, qword ptr [rip + .Lx80_0]
 mov qword ptr [r12 + 824], rax
 jmp xgvarg79_done
 xgvarg79_β:
 jmp snoch0_n31_α
.Lx80_0:
 .quad 4
xgvarg79_done:
# IR_LIT_I
bb50_α:
 mov qword ptr [r12 + 832], 6
 mov rax, qword ptr [rip + .Lx82_0]
 mov qword ptr [r12 + 840], rax
 jmp xgvarg81_done
 xgvarg81_β:
 jmp snoch0_n31_α
.Lx82_0:
 .quad 6
xgvarg81_done:
bb51_α:
# BOX IR_CALL gt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+816] -> [r12+864]
 mov rax, qword ptr [r12 + 816]
 mov qword ptr [r12 + 864], rax
 mov rax, qword ptr [r12 + 824]
 mov qword ptr [r12 + 872], rax
# marshal arg1 = producer-box slot [r12+832] -> [r12+880]
 mov rax, qword ptr [r12 + 832]
 mov qword ptr [r12 + 880], rax
 mov rax, qword ptr [r12 + 840]
 mov qword ptr [r12 + 888], rax
  .section .rodata
  .Lbynamefn84: .string "gt"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn84]
 lea rsi, [r12 + 864]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 848], rax
 mov qword ptr [r12 + 856], rdx
 cmp eax, 99
 je snoch0_n31_α
 jmp snoch0_n30_α
snoch0_n28_β:
 jmp snoch0_n31_α
snoch0_n29_α:
# IR_LIT_scalar
bb52_α:
 jmp snoch0_n32_α
 snoch0_n29_β:
 jmp flat_γ
snoch0_n30_α:
# IR_LIT_scalar
bb53_α:
 jmp snoch0_n33_α
 snoch0_n30_β:
 jmp flat_γ
snoch0_n31_α:
# IR_LIT_I
bb54_α:
 mov qword ptr [r12 + 896], 6
 mov rax, qword ptr [rip + .Lx88_0]
 mov qword ptr [r12 + 904], rax
 jmp xgvarg87_done
 xgvarg87_β:
 jmp snoch0_n35_α
.Lx88_0:
 .quad 4
xgvarg87_done:
# IR_LIT_I
bb55_α:
 mov qword ptr [r12 + 912], 6
 mov rax, qword ptr [rip + .Lx90_0]
 mov qword ptr [r12 + 920], rax
 jmp xgvarg89_done
 xgvarg89_β:
 jmp snoch0_n35_α
.Lx90_0:
 .quad 4
xgvarg89_done:
bb56_α:
# BOX IR_CALL gt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+896] -> [r12+944]
 mov rax, qword ptr [r12 + 896]
 mov qword ptr [r12 + 944], rax
 mov rax, qword ptr [r12 + 904]
 mov qword ptr [r12 + 952], rax
# marshal arg1 = producer-box slot [r12+912] -> [r12+960]
 mov rax, qword ptr [r12 + 912]
 mov qword ptr [r12 + 960], rax
 mov rax, qword ptr [r12 + 920]
 mov qword ptr [r12 + 968], rax
  .section .rodata
  .Lbynamefn92: .string "gt"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn92]
 lea rsi, [r12 + 944]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 928], rax
 mov qword ptr [r12 + 936], rdx
 cmp eax, 99
 je snoch0_n35_α
 jmp snoch0_n34_α
snoch0_n31_β:
 jmp snoch0_n35_α
snoch0_n32_α:
bb57_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S10]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n32_β:
 jmp flat_γ
snoch0_n33_α:
bb58_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S11]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n33_β:
 jmp flat_γ
snoch0_n34_α:
# IR_LIT_scalar
bb59_α:
 jmp snoch0_n36_α
 snoch0_n34_β:
 jmp flat_γ
snoch0_n35_α:
# IR_LIT_I
bb60_α:
 mov qword ptr [r12 + 976], 6
 mov rax, qword ptr [rip + .Lx97_0]
 mov qword ptr [r12 + 984], rax
 jmp xgvarg96_done
 xgvarg96_β:
 jmp snoch0_n38_α
.Lx97_0:
 .quad 5
xgvarg96_done:
# IR_LIT_I
bb61_α:
 mov qword ptr [r12 + 992], 6
 mov rax, qword ptr [rip + .Lx99_0]
 mov qword ptr [r12 + 1000], rax
 jmp xgvarg98_done
 xgvarg98_β:
 jmp snoch0_n38_α
.Lx99_0:
 .quad 2
xgvarg98_done:
bb62_α:
# BOX IR_CALL gt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+976] -> [r12+1024]
 mov rax, qword ptr [r12 + 976]
 mov qword ptr [r12 + 1024], rax
 mov rax, qword ptr [r12 + 984]
 mov qword ptr [r12 + 1032], rax
# marshal arg1 = producer-box slot [r12+992] -> [r12+1040]
 mov rax, qword ptr [r12 + 992]
 mov qword ptr [r12 + 1040], rax
 mov rax, qword ptr [r12 + 1000]
 mov qword ptr [r12 + 1048], rax
  .section .rodata
  .Lbynamefn101: .string "gt"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn101]
 lea rsi, [r12 + 1024]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1008], rax
 mov qword ptr [r12 + 1016], rdx
 cmp eax, 99
 je snoch0_n38_α
 jmp snoch0_n37_α
snoch0_n35_β:
 jmp snoch0_n38_α
snoch0_n36_α:
bb63_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S12]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n36_β:
 jmp flat_γ
snoch0_n37_α:
# IR_LIT_I
bb64_α:
 mov qword ptr [r12 + 1056], 6
 mov rax, qword ptr [rip + .Lx104_0]
 mov qword ptr [r12 + 1064], rax
 jmp xgvarg103_done
 xgvarg103_β:
 jmp snoch0_n40_α
.Lx104_0:
 .quad 5
xgvarg103_done:
# IR_LIT_I
bb65_α:
 mov qword ptr [r12 + 1072], 6
 mov rax, qword ptr [rip + .Lx106_0]
 mov qword ptr [r12 + 1080], rax
 jmp xgvarg105_done
 xgvarg105_β:
 jmp snoch0_n40_α
.Lx106_0:
 .quad 7
xgvarg105_done:
bb66_α:
# BOX IR_CALL ge(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+1056] -> [r12+1104]
 mov rax, qword ptr [r12 + 1056]
 mov qword ptr [r12 + 1104], rax
 mov rax, qword ptr [r12 + 1064]
 mov qword ptr [r12 + 1112], rax
# marshal arg1 = producer-box slot [r12+1072] -> [r12+1120]
 mov rax, qword ptr [r12 + 1072]
 mov qword ptr [r12 + 1120], rax
 mov rax, qword ptr [r12 + 1080]
 mov qword ptr [r12 + 1128], rax
  .section .rodata
  .Lbynamefn108: .string "ge"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn108]
 lea rsi, [r12 + 1104]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1088], rax
 mov qword ptr [r12 + 1096], rdx
 cmp eax, 99
 je snoch0_n40_α
 jmp snoch0_n39_α
snoch0_n37_β:
 jmp snoch0_n40_α
snoch0_n38_α:
# IR_LIT_scalar
bb67_α:
 jmp snoch0_n41_α
 snoch0_n38_β:
 jmp flat_γ
snoch0_n39_α:
# IR_LIT_scalar
bb68_α:
 jmp snoch0_n42_α
 snoch0_n39_β:
 jmp flat_γ
snoch0_n40_α:
# IR_LIT_I
bb69_α:
 mov qword ptr [r12 + 1136], 6
 mov rax, qword ptr [rip + .Lx112_0]
 mov qword ptr [r12 + 1144], rax
 jmp xgvarg111_done
 xgvarg111_β:
 jmp snoch0_n44_α
.Lx112_0:
 .quad 4
xgvarg111_done:
# IR_LIT_I
bb70_α:
 mov qword ptr [r12 + 1152], 6
 mov rax, qword ptr [rip + .Lx114_0]
 mov qword ptr [r12 + 1160], rax
 jmp xgvarg113_done
 xgvarg113_β:
 jmp snoch0_n44_α
.Lx114_0:
 .quad 4
xgvarg113_done:
bb71_α:
# BOX IR_CALL ge(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+1136] -> [r12+1184]
 mov rax, qword ptr [r12 + 1136]
 mov qword ptr [r12 + 1184], rax
 mov rax, qword ptr [r12 + 1144]
 mov qword ptr [r12 + 1192], rax
# marshal arg1 = producer-box slot [r12+1152] -> [r12+1200]
 mov rax, qword ptr [r12 + 1152]
 mov qword ptr [r12 + 1200], rax
 mov rax, qword ptr [r12 + 1160]
 mov qword ptr [r12 + 1208], rax
  .section .rodata
  .Lbynamefn116: .string "ge"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn116]
 lea rsi, [r12 + 1184]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1168], rax
 mov qword ptr [r12 + 1176], rdx
 cmp eax, 99
 je snoch0_n44_α
 jmp snoch0_n43_α
snoch0_n40_β:
 jmp snoch0_n44_α
snoch0_n41_α:
bb72_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S13]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n41_β:
 jmp flat_γ
snoch0_n42_α:
bb73_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S14]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n42_β:
 jmp flat_γ
snoch0_n43_α:
# IR_LIT_I
bb74_α:
 mov qword ptr [r12 + 1216], 6
 mov rax, qword ptr [rip + .Lx120_0]
 mov qword ptr [r12 + 1224], rax
 jmp xgvarg119_done
 xgvarg119_β:
 jmp snoch0_n46_α
.Lx120_0:
 .quad 7
xgvarg119_done:
# IR_LIT_I
bb75_α:
 mov qword ptr [r12 + 1232], 6
 mov rax, qword ptr [rip + .Lx122_0]
 mov qword ptr [r12 + 1240], rax
 jmp xgvarg121_done
 xgvarg121_β:
 jmp snoch0_n46_α
.Lx122_0:
 .quad 5
xgvarg121_done:
bb76_α:
# BOX IR_CALL ge(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+1216] -> [r12+1264]
 mov rax, qword ptr [r12 + 1216]
 mov qword ptr [r12 + 1264], rax
 mov rax, qword ptr [r12 + 1224]
 mov qword ptr [r12 + 1272], rax
# marshal arg1 = producer-box slot [r12+1232] -> [r12+1280]
 mov rax, qword ptr [r12 + 1232]
 mov qword ptr [r12 + 1280], rax
 mov rax, qword ptr [r12 + 1240]
 mov qword ptr [r12 + 1288], rax
  .section .rodata
  .Lbynamefn124: .string "ge"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn124]
 lea rsi, [r12 + 1264]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1248], rax
 mov qword ptr [r12 + 1256], rdx
 cmp eax, 99
 je snoch0_n46_α
 jmp snoch0_n45_α
snoch0_n43_β:
 jmp snoch0_n46_α
snoch0_n44_α:
# IR_LIT_scalar
bb77_α:
 jmp snoch0_n47_α
 snoch0_n44_β:
 jmp flat_γ
snoch0_n45_α:
# IR_LIT_I
bb78_α:
 mov qword ptr [r12 + 1296], 6
 mov rax, qword ptr [rip + .Lx127_0]
 mov qword ptr [r12 + 1304], rax
 jmp xgvarg126_done
 xgvarg126_β:
 jmp snoch0_n49_α
.Lx127_0:
 .quad 4
xgvarg126_done:
xgvarg128_done:
bb79_α:
# BOX IR_CALL ne(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+1296] -> [r12+1328]
 mov rax, qword ptr [r12 + 1296]
 mov qword ptr [r12 + 1328], rax
 mov rax, qword ptr [r12 + 1304]
 mov qword ptr [r12 + 1336], rax
# marshal arg1 = inline gvar-arith subexpr -> [r12+1344]
 movabs rax, 5
 mov qword ptr [r12 + 1360], rax
 mov rcx, 1
 mov rax, qword ptr [r12 + 1360]
 sub rax, rcx
 mov qword ptr [r12 + 1344], 6
 mov qword ptr [r12 + 1352], rax
  .section .rodata
  .Lbynamefn130: .string "ne"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn130]
 lea rsi, [r12 + 1328]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1312], rax
 mov qword ptr [r12 + 1320], rdx
 cmp eax, 99
 je snoch0_n49_α
 jmp snoch0_n48_α
snoch0_n45_β:
 jmp snoch0_n49_α
snoch0_n46_α:
# IR_LIT_scalar
bb80_α:
 jmp snoch0_n50_α
 snoch0_n46_β:
 jmp flat_γ
snoch0_n47_α:
bb81_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S15]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n47_β:
 jmp flat_γ
snoch0_n48_α:
# IR_LIT_scalar
bb82_α:
 jmp snoch0_n51_α
 snoch0_n48_β:
 jmp flat_γ
snoch0_n49_α:
# IR_LIT_S
bb83_α:
 mov qword ptr [r12 + 1376], 1
 mov rax, qword ptr [rip + .Lx135_0]
 mov qword ptr [r12 + 1384], rax
 jmp xgvarg134_done
 xgvarg134_β:
 jmp snoch0_n53_α
.Lx135_0:
 .quad .Lx135_0_s
.Lx135_0_s:
 .string "12"
xgvarg134_done:
# IR_LIT_I
bb84_α:
 mov qword ptr [r12 + 1392], 6
 mov rax, qword ptr [rip + .Lx137_0]
 mov qword ptr [r12 + 1400], rax
 jmp xgvarg136_done
 xgvarg136_β:
 jmp snoch0_n53_α
.Lx137_0:
 .quad 12
xgvarg136_done:
bb85_α:
# BOX IR_CALL ne(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+1376] -> [r12+1424]
 mov rax, qword ptr [r12 + 1376]
 mov qword ptr [r12 + 1424], rax
 mov rax, qword ptr [r12 + 1384]
 mov qword ptr [r12 + 1432], rax
# marshal arg1 = producer-box slot [r12+1392] -> [r12+1440]
 mov rax, qword ptr [r12 + 1392]
 mov qword ptr [r12 + 1440], rax
 mov rax, qword ptr [r12 + 1400]
 mov qword ptr [r12 + 1448], rax
  .section .rodata
  .Lbynamefn139: .string "ne"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn139]
 lea rsi, [r12 + 1424]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1408], rax
 mov qword ptr [r12 + 1416], rdx
 cmp eax, 99
 je snoch0_n53_α
 jmp snoch0_n52_α
snoch0_n49_β:
 jmp snoch0_n53_α
snoch0_n50_α:
bb86_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S16]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n50_β:
 jmp flat_γ
snoch0_n51_α:
bb87_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S17]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n51_β:
 jmp flat_γ
snoch0_n52_α:
# IR_LIT_scalar
bb88_α:
 jmp snoch0_n54_α
 snoch0_n52_β:
 jmp flat_γ
snoch0_n53_α:
# IR_LIT_scalar
bb89_α:
 jmp snoch0_n55_α
 snoch0_n53_β:
 jmp flat_γ
snoch0_n54_α:
bb90_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S18]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n54_β:
 jmp flat_γ
snoch0_n55_α:
bb91_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S19]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n55_β:
 jmp flat_γ
flat_β:
jmp flat_ω
flat_γ:
mov eax, 1
xor edx, edx
pop r12
ret
flat_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
pop r12
ret
.section .rodata
.S0: .string "output"
.S1: .string "FAIL 912/001: lt(5,4) should fail"
.S2: .string "FAIL 912/002: lt(4,4) should fail"
.S3: .string "FAIL 912/003: lt(4,5) should succeed"
.S4: .string "FAIL 912/004: le(5,2) should fail"
.S5: .string "FAIL 912/005: le(4,4) should succeed"
.S6: .string "FAIL 912/006: le(4,10) should succeed"
.S7: .string "FAIL 912/007: eq(4,5) should fail"
.S8: .string "FAIL 912/008: eq(5,5) should succeed"
.S9: .string "FAIL 912/009: ne(4,4) should fail"
.S10: .string "FAIL 912/010: ne(4,6) should succeed"
.S11: .string "FAIL 912/011: gt(4,6) should fail"
.S12: .string "FAIL 912/012: gt(4,4) should fail"
.S13: .string "FAIL 912/013: gt(5,2) should succeed"
.S14: .string "FAIL 912/014: ge(5,7) should fail"
.S15: .string "FAIL 912/015: ge(4,4) should succeed"
.S16: .string "FAIL 912/016: ge(7,5) should succeed"
.S17: .string "FAIL 912/017: ne(4,5-1) should fail (both=4)"
.S18: .string "FAIL 912/018: ne(string,int) type mismatch"
.S19: .string "PASS 912_num_pred (18/18)"
.text

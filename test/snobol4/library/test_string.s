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
# IR_LIT_scalar
bb1_α:
 jmp snoch0_n1_α
 snoch0_n0_β:
 jmp snoch0_n2_α
snoch0_n1_α:
bb2_α:
# IR_ASSIGN
 lea rdi, [rip + .S0]
 movabs rsi, 1
 call rt_gvar_assign_int@PLT
 jmp snoch0_n2_α
 snoch0_n1_β:
 jmp snoch0_n2_α
snoch0_n2_α:
# IR_LIT_S
bb3_α:
 mov qword ptr [r12 + 16], 1
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [r12 + 24], rax
 jmp xgvarg3_done
 xgvarg3_β:
 jmp snoch0_n4_α
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "hi"
xgvarg3_done:
# IR_LIT_I
bb4_α:
 mov qword ptr [r12 + 32], 6
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 40], rax
 jmp xgvarg5_done
 xgvarg5_β:
 jmp snoch0_n4_α
.Lx6_0:
 .quad 6
xgvarg5_done:
# IR_LIT_S
bb5_α:
 mov qword ptr [r12 + 48], 1
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [r12 + 56], rax
 jmp xgvarg7_done
 xgvarg7_β:
 jmp snoch0_n4_α
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "*"
xgvarg7_done:
bb6_α:
# BOX IR_CALL pad_left(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+16] -> [r12+80]
 mov rax, qword ptr [r12 + 16]
 mov qword ptr [r12 + 80], rax
 mov rax, qword ptr [r12 + 24]
 mov qword ptr [r12 + 88], rax
# marshal arg1 = producer-box slot [r12+32] -> [r12+96]
 mov rax, qword ptr [r12 + 32]
 mov qword ptr [r12 + 96], rax
 mov rax, qword ptr [r12 + 40]
 mov qword ptr [r12 + 104], rax
# marshal arg2 = producer-box slot [r12+48] -> [r12+112]
 mov rax, qword ptr [r12 + 48]
 mov qword ptr [r12 + 112], rax
 mov rax, qword ptr [r12 + 56]
 mov qword ptr [r12 + 120], rax
  .section .rodata
  .Lbynamefn10: .string "pad_left"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn10]
 lea rsi, [r12 + 80]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
 cmp eax, 99
 je snoch0_n4_α
 jmp snoch0_n3_α
snoch0_n2_β:
 jmp snoch0_n4_α
snoch0_n3_α:
bb7_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 64]
 mov rdx, qword ptr [r12 + 72]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n4_α
 snoch0_n3_β:
 jmp snoch0_n4_α
snoch0_n4_α:
# IR_LIT_S
bb8_α:
 mov qword ptr [r12 + 128], 1
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [r12 + 136], rax
 jmp xgvarg12_done
 xgvarg12_β:
 jmp snoch0_n6_α
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "hi"
xgvarg12_done:
# IR_LIT_I
bb9_α:
 mov qword ptr [r12 + 144], 6
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [r12 + 152], rax
 jmp xgvarg14_done
 xgvarg14_β:
 jmp snoch0_n6_α
.Lx15_0:
 .quad 6
xgvarg14_done:
# IR_LIT_S
bb10_α:
 mov qword ptr [r12 + 160], 1
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [r12 + 168], rax
 jmp xgvarg16_done
 xgvarg16_β:
 jmp snoch0_n6_α
.Lx17_0:
 .quad .Lx17_0_s
.Lx17_0_s:
 .string "*"
xgvarg16_done:
bb11_α:
# BOX IR_CALL pad_right(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+128] -> [r12+192]
 mov rax, qword ptr [r12 + 128]
 mov qword ptr [r12 + 192], rax
 mov rax, qword ptr [r12 + 136]
 mov qword ptr [r12 + 200], rax
# marshal arg1 = producer-box slot [r12+144] -> [r12+208]
 mov rax, qword ptr [r12 + 144]
 mov qword ptr [r12 + 208], rax
 mov rax, qword ptr [r12 + 152]
 mov qword ptr [r12 + 216], rax
# marshal arg2 = producer-box slot [r12+160] -> [r12+224]
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 224], rax
 mov rax, qword ptr [r12 + 168]
 mov qword ptr [r12 + 232], rax
  .section .rodata
  .Lbynamefn19: .string "pad_right"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn19]
 lea rsi, [r12 + 192]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 cmp eax, 99
 je snoch0_n6_α
 jmp snoch0_n5_α
snoch0_n4_β:
 jmp snoch0_n6_α
snoch0_n5_α:
bb12_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 176]
 mov rdx, qword ptr [r12 + 184]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n6_α
 snoch0_n5_β:
 jmp snoch0_n6_α
snoch0_n6_α:
# IR_LIT_S
bb13_α:
 mov qword ptr [r12 + 240], 1
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [r12 + 248], rax
 jmp xgvarg21_done
 xgvarg21_β:
 jmp snoch0_n8_α
.Lx22_0:
 .quad .Lx22_0_s
.Lx22_0_s:
 .string "   hello"
xgvarg21_done:
bb14_α:
# BOX IR_CALL ltrim(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+240] -> [r12+272]
 mov rax, qword ptr [r12 + 240]
 mov qword ptr [r12 + 272], rax
 mov rax, qword ptr [r12 + 248]
 mov qword ptr [r12 + 280], rax
  .section .rodata
  .Lbynamefn24: .string "ltrim"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn24]
 lea rsi, [r12 + 272]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
 cmp eax, 99
 je snoch0_n8_α
 jmp snoch0_n7_α
snoch0_n6_β:
 jmp snoch0_n8_α
snoch0_n7_α:
bb15_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 256]
 mov rdx, qword ptr [r12 + 264]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n8_α
 snoch0_n7_β:
 jmp snoch0_n8_α
snoch0_n8_α:
# IR_LIT_S
bb16_α:
 mov qword ptr [r12 + 288], 1
 mov rax, qword ptr [rip + .Lx27_0]
 mov qword ptr [r12 + 296], rax
 jmp xgvarg26_done
 xgvarg26_β:
 jmp snoch0_n10_α
.Lx27_0:
 .quad .Lx27_0_s
.Lx27_0_s:
 .string "hello   "
xgvarg26_done:
bb17_α:
# BOX IR_CALL rtrim(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+288] -> [r12+320]
 mov rax, qword ptr [r12 + 288]
 mov qword ptr [r12 + 320], rax
 mov rax, qword ptr [r12 + 296]
 mov qword ptr [r12 + 328], rax
  .section .rodata
  .Lbynamefn29: .string "rtrim"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn29]
 lea rsi, [r12 + 320]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 304], rax
 mov qword ptr [r12 + 312], rdx
 cmp eax, 99
 je snoch0_n10_α
 jmp snoch0_n9_α
snoch0_n8_β:
 jmp snoch0_n10_α
snoch0_n9_α:
bb18_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 304]
 mov rdx, qword ptr [r12 + 312]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n10_α
 snoch0_n9_β:
 jmp snoch0_n10_α
snoch0_n10_α:
# IR_LIT_S
bb19_α:
 mov qword ptr [r12 + 336], 1
 mov rax, qword ptr [rip + .Lx32_0]
 mov qword ptr [r12 + 344], rax
 jmp xgvarg31_done
 xgvarg31_β:
 jmp snoch0_n12_α
.Lx32_0:
 .quad .Lx32_0_s
.Lx32_0_s:
 .string "  hello  "
xgvarg31_done:
bb20_α:
# BOX IR_CALL trimws(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+336] -> [r12+368]
 mov rax, qword ptr [r12 + 336]
 mov qword ptr [r12 + 368], rax
 mov rax, qword ptr [r12 + 344]
 mov qword ptr [r12 + 376], rax
  .section .rodata
  .Lbynamefn34: .string "trimws"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn34]
 lea rsi, [r12 + 368]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 352], rax
 mov qword ptr [r12 + 360], rdx
 cmp eax, 99
 je snoch0_n12_α
 jmp snoch0_n11_α
snoch0_n10_β:
 jmp snoch0_n12_α
snoch0_n11_α:
bb21_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 352]
 mov rdx, qword ptr [r12 + 360]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n12_α
 snoch0_n11_β:
 jmp snoch0_n12_α
snoch0_n12_α:
# IR_LIT_S
bb22_α:
 mov qword ptr [r12 + 384], 1
 mov rax, qword ptr [rip + .Lx37_0]
 mov qword ptr [r12 + 392], rax
 jmp xgvarg36_done
 xgvarg36_β:
 jmp snoch0_n14_α
.Lx37_0:
 .quad .Lx37_0_s
.Lx37_0_s:
 .string "hi"
xgvarg36_done:
# IR_LIT_I
bb23_α:
 mov qword ptr [r12 + 400], 6
 mov rax, qword ptr [rip + .Lx39_0]
 mov qword ptr [r12 + 408], rax
 jmp xgvarg38_done
 xgvarg38_β:
 jmp snoch0_n14_α
.Lx39_0:
 .quad 3
xgvarg38_done:
bb24_α:
# BOX IR_CALL repeat(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+384] -> [r12+432]
 mov rax, qword ptr [r12 + 384]
 mov qword ptr [r12 + 432], rax
 mov rax, qword ptr [r12 + 392]
 mov qword ptr [r12 + 440], rax
# marshal arg1 = producer-box slot [r12+400] -> [r12+448]
 mov rax, qword ptr [r12 + 400]
 mov qword ptr [r12 + 448], rax
 mov rax, qword ptr [r12 + 408]
 mov qword ptr [r12 + 456], rax
  .section .rodata
  .Lbynamefn41: .string "repeat"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn41]
 lea rsi, [r12 + 432]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 cmp eax, 99
 je snoch0_n14_α
 jmp snoch0_n13_α
snoch0_n12_β:
 jmp snoch0_n14_α
snoch0_n13_α:
bb25_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 416]
 mov rdx, qword ptr [r12 + 424]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n14_α
 snoch0_n13_β:
 jmp snoch0_n14_α
snoch0_n14_α:
# IR_LIT_S
bb26_α:
 mov qword ptr [r12 + 464], 1
 mov rax, qword ptr [rip + .Lx44_0]
 mov qword ptr [r12 + 472], rax
 jmp xgvarg43_done
 xgvarg43_β:
 jmp snoch0_n16_α
.Lx44_0:
 .quad .Lx44_0_s
.Lx44_0_s:
 .string "foobar"
xgvarg43_done:
# IR_LIT_S
bb27_α:
 mov qword ptr [r12 + 480], 1
 mov rax, qword ptr [rip + .Lx46_0]
 mov qword ptr [r12 + 488], rax
 jmp xgvarg45_done
 xgvarg45_β:
 jmp snoch0_n16_α
.Lx46_0:
 .quad .Lx46_0_s
.Lx46_0_s:
 .string "oba"
xgvarg45_done:
bb28_α:
# BOX IR_CALL contains(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+464] -> [r12+512]
 mov rax, qword ptr [r12 + 464]
 mov qword ptr [r12 + 512], rax
 mov rax, qword ptr [r12 + 472]
 mov qword ptr [r12 + 520], rax
# marshal arg1 = producer-box slot [r12+480] -> [r12+528]
 mov rax, qword ptr [r12 + 480]
 mov qword ptr [r12 + 528], rax
 mov rax, qword ptr [r12 + 488]
 mov qword ptr [r12 + 536], rax
  .section .rodata
  .Lbynamefn48: .string "contains"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn48]
 lea rsi, [r12 + 512]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 496], rax
 mov qword ptr [r12 + 504], rdx
 cmp eax, 99
 je snoch0_n16_α
 jmp snoch0_n15_α
snoch0_n14_β:
 jmp snoch0_n16_α
snoch0_n15_α:
# IR_LIT_scalar
bb29_α:
 jmp snoch0_n17_α
 snoch0_n15_β:
 jmp snoch0_n18_α
snoch0_n16_α:
# IR_LIT_scalar
bb30_α:
 jmp snoch0_n19_α
 snoch0_n16_β:
 jmp snoch0_n18_α
snoch0_n17_α:
bb31_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S2]
 call rt_gvar_assign_str@PLT
 jmp snoch0_n18_α
 snoch0_n17_β:
 jmp snoch0_n18_α
snoch0_n18_α:
# IR_LIT_S
bb32_α:
 mov qword ptr [r12 + 544], 1
 mov rax, qword ptr [rip + .Lx53_0]
 mov qword ptr [r12 + 552], rax
 jmp xgvarg52_done
 xgvarg52_β:
 jmp snoch0_n21_α
.Lx53_0:
 .quad .Lx53_0_s
.Lx53_0_s:
 .string "foobar"
xgvarg52_done:
# IR_LIT_S
bb33_α:
 mov qword ptr [r12 + 560], 1
 mov rax, qword ptr [rip + .Lx55_0]
 mov qword ptr [r12 + 568], rax
 jmp xgvarg54_done
 xgvarg54_β:
 jmp snoch0_n21_α
.Lx55_0:
 .quad .Lx55_0_s
.Lx55_0_s:
 .string "foo"
xgvarg54_done:
bb34_α:
# BOX IR_CALL startswith(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+544] -> [r12+592]
 mov rax, qword ptr [r12 + 544]
 mov qword ptr [r12 + 592], rax
 mov rax, qword ptr [r12 + 552]
 mov qword ptr [r12 + 600], rax
# marshal arg1 = producer-box slot [r12+560] -> [r12+608]
 mov rax, qword ptr [r12 + 560]
 mov qword ptr [r12 + 608], rax
 mov rax, qword ptr [r12 + 568]
 mov qword ptr [r12 + 616], rax
  .section .rodata
  .Lbynamefn57: .string "startswith"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn57]
 lea rsi, [r12 + 592]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 576], rax
 mov qword ptr [r12 + 584], rdx
 cmp eax, 99
 je snoch0_n21_α
 jmp snoch0_n20_α
snoch0_n18_β:
 jmp snoch0_n21_α
snoch0_n19_α:
bb35_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S3]
 call rt_gvar_assign_str@PLT
 jmp snoch0_n18_α
 snoch0_n19_β:
 jmp snoch0_n18_α
snoch0_n20_α:
# IR_LIT_scalar
bb36_α:
 jmp snoch0_n22_α
 snoch0_n20_β:
 jmp snoch0_n23_α
snoch0_n21_α:
# IR_LIT_scalar
bb37_α:
 jmp snoch0_n24_α
 snoch0_n21_β:
 jmp snoch0_n23_α
snoch0_n22_α:
bb38_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S4]
 call rt_gvar_assign_str@PLT
 jmp snoch0_n23_α
 snoch0_n22_β:
 jmp snoch0_n23_α
snoch0_n23_α:
# IR_LIT_S
bb39_α:
 mov qword ptr [r12 + 624], 1
 mov rax, qword ptr [rip + .Lx63_0]
 mov qword ptr [r12 + 632], rax
 jmp xgvarg62_done
 xgvarg62_β:
 jmp snoch0_n26_α
.Lx63_0:
 .quad .Lx63_0_s
.Lx63_0_s:
 .string "foobar"
xgvarg62_done:
# IR_LIT_S
bb40_α:
 mov qword ptr [r12 + 640], 1
 mov rax, qword ptr [rip + .Lx65_0]
 mov qword ptr [r12 + 648], rax
 jmp xgvarg64_done
 xgvarg64_β:
 jmp snoch0_n26_α
.Lx65_0:
 .quad .Lx65_0_s
.Lx65_0_s:
 .string "bar"
xgvarg64_done:
bb41_α:
# BOX IR_CALL endswith(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+624] -> [r12+672]
 mov rax, qword ptr [r12 + 624]
 mov qword ptr [r12 + 672], rax
 mov rax, qword ptr [r12 + 632]
 mov qword ptr [r12 + 680], rax
# marshal arg1 = producer-box slot [r12+640] -> [r12+688]
 mov rax, qword ptr [r12 + 640]
 mov qword ptr [r12 + 688], rax
 mov rax, qword ptr [r12 + 648]
 mov qword ptr [r12 + 696], rax
  .section .rodata
  .Lbynamefn67: .string "endswith"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn67]
 lea rsi, [r12 + 672]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 656], rax
 mov qword ptr [r12 + 664], rdx
 cmp eax, 99
 je snoch0_n26_α
 jmp snoch0_n25_α
snoch0_n23_β:
 jmp snoch0_n26_α
snoch0_n24_α:
bb42_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S5]
 call rt_gvar_assign_str@PLT
 jmp snoch0_n23_α
 snoch0_n24_β:
 jmp snoch0_n23_α
snoch0_n25_α:
# IR_LIT_scalar
bb43_α:
 jmp snoch0_n27_α
 snoch0_n25_β:
 jmp snoch0_n28_α
snoch0_n26_α:
# IR_LIT_scalar
bb44_α:
 jmp snoch0_n29_α
 snoch0_n26_β:
 jmp snoch0_n28_α
snoch0_n27_α:
bb45_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S6]
 call rt_gvar_assign_str@PLT
 jmp snoch0_n28_α
 snoch0_n27_β:
 jmp snoch0_n28_α
snoch0_n28_α:
# IR_LIT_S
bb46_α:
 mov qword ptr [r12 + 704], 1
 mov rax, qword ptr [rip + .Lx73_0]
 mov qword ptr [r12 + 712], rax
 jmp xgvarg72_done
 xgvarg72_β:
 jmp snoch0_n31_α
.Lx73_0:
 .quad .Lx73_0_s
.Lx73_0_s:
 .string "foobar"
xgvarg72_done:
# IR_LIT_S
bb47_α:
 mov qword ptr [r12 + 720], 1
 mov rax, qword ptr [rip + .Lx75_0]
 mov qword ptr [r12 + 728], rax
 jmp xgvarg74_done
 xgvarg74_β:
 jmp snoch0_n31_α
.Lx75_0:
 .quad .Lx75_0_s
.Lx75_0_s:
 .string "bar"
xgvarg74_done:
bb48_α:
# BOX IR_CALL startswith(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+704] -> [r12+752]
 mov rax, qword ptr [r12 + 704]
 mov qword ptr [r12 + 752], rax
 mov rax, qword ptr [r12 + 712]
 mov qword ptr [r12 + 760], rax
# marshal arg1 = producer-box slot [r12+720] -> [r12+768]
 mov rax, qword ptr [r12 + 720]
 mov qword ptr [r12 + 768], rax
 mov rax, qword ptr [r12 + 728]
 mov qword ptr [r12 + 776], rax
  .section .rodata
  .Lbynamefn77: .string "startswith"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn77]
 lea rsi, [r12 + 752]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 736], rax
 mov qword ptr [r12 + 744], rdx
 cmp eax, 99
 je snoch0_n31_α
 jmp snoch0_n30_α
snoch0_n28_β:
 jmp snoch0_n31_α
snoch0_n29_α:
bb49_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S7]
 call rt_gvar_assign_str@PLT
 jmp snoch0_n28_α
 snoch0_n29_β:
 jmp snoch0_n28_α
snoch0_n30_α:
# IR_LIT_scalar
bb50_α:
 jmp snoch0_n32_α
 snoch0_n30_β:
 jmp snoch0_n33_α
snoch0_n31_α:
# IR_LIT_scalar
bb51_α:
 jmp snoch0_n34_α
 snoch0_n31_β:
 jmp snoch0_n33_α
snoch0_n32_α:
bb52_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S8]
 call rt_gvar_assign_str@PLT
 jmp snoch0_n33_α
 snoch0_n32_β:
 jmp snoch0_n33_α
snoch0_n33_α:
# IR_LIT_S
bb53_α:
 mov qword ptr [r12 + 784], 1
 mov rax, qword ptr [rip + .Lx83_0]
 mov qword ptr [r12 + 792], rax
 jmp xgvarg82_done
 xgvarg82_β:
 jmp snoch0_n36_α
.Lx83_0:
 .quad .Lx83_0_s
.Lx83_0_s:
 .string "foobar"
xgvarg82_done:
# IR_LIT_S
bb54_α:
 mov qword ptr [r12 + 800], 1
 mov rax, qword ptr [rip + .Lx85_0]
 mov qword ptr [r12 + 808], rax
 jmp xgvarg84_done
 xgvarg84_β:
 jmp snoch0_n36_α
.Lx85_0:
 .quad .Lx85_0_s
.Lx85_0_s:
 .string "oba"
xgvarg84_done:
bb55_α:
# BOX IR_CALL index(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+784] -> [r12+832]
 mov rax, qword ptr [r12 + 784]
 mov qword ptr [r12 + 832], rax
 mov rax, qword ptr [r12 + 792]
 mov qword ptr [r12 + 840], rax
# marshal arg1 = producer-box slot [r12+800] -> [r12+848]
 mov rax, qword ptr [r12 + 800]
 mov qword ptr [r12 + 848], rax
 mov rax, qword ptr [r12 + 808]
 mov qword ptr [r12 + 856], rax
  .section .rodata
  .Lbynamefn87: .string "index"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn87]
 lea rsi, [r12 + 832]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 816], rax
 mov qword ptr [r12 + 824], rdx
 cmp eax, 99
 je snoch0_n36_α
 jmp snoch0_n35_α
snoch0_n33_β:
 jmp snoch0_n36_α
snoch0_n34_α:
bb56_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S9]
 call rt_gvar_assign_str@PLT
 jmp snoch0_n33_α
 snoch0_n34_β:
 jmp snoch0_n33_α
snoch0_n35_α:
bb57_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 816]
 mov rdx, qword ptr [r12 + 824]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n36_α
 snoch0_n35_β:
 jmp snoch0_n36_α
snoch0_n36_α:
# IR_LIT_S
bb58_α:
 mov qword ptr [r12 + 864], 1
 mov rax, qword ptr [rip + .Lx91_0]
 mov qword ptr [r12 + 872], rax
 jmp xgvarg90_done
 xgvarg90_β:
 jmp flat_γ
.Lx91_0:
 .quad .Lx91_0_s
.Lx91_0_s:
 .string "foobar"
xgvarg90_done:
# IR_LIT_S
bb59_α:
 mov qword ptr [r12 + 880], 1
 mov rax, qword ptr [rip + .Lx93_0]
 mov qword ptr [r12 + 888], rax
 jmp xgvarg92_done
 xgvarg92_β:
 jmp flat_γ
.Lx93_0:
 .quad .Lx93_0_s
.Lx93_0_s:
 .string "xyz"
xgvarg92_done:
bb60_α:
# BOX IR_CALL index(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+864] -> [r12+912]
 mov rax, qword ptr [r12 + 864]
 mov qword ptr [r12 + 912], rax
 mov rax, qword ptr [r12 + 872]
 mov qword ptr [r12 + 920], rax
# marshal arg1 = producer-box slot [r12+880] -> [r12+928]
 mov rax, qword ptr [r12 + 880]
 mov qword ptr [r12 + 928], rax
 mov rax, qword ptr [r12 + 888]
 mov qword ptr [r12 + 936], rax
  .section .rodata
  .Lbynamefn95: .string "index"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn95]
 lea rsi, [r12 + 912]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 896], rax
 mov qword ptr [r12 + 904], rdx
 cmp eax, 99
 je flat_γ
 jmp snoch0_n37_α
snoch0_n36_β:
 jmp flat_γ
snoch0_n37_α:
bb61_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 896]
 mov rdx, qword ptr [r12 + 904]
 call rt_gvar_assign_descr@PLT
 jmp flat_γ
 snoch0_n37_β:
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
.S0: .string "TRIM"
.S1: .string "OUTPUT"
.S2: .string "contains ok"
.S3: .string "FAIL: contains"
.S4: .string "startswith ok"
.S5: .string "FAIL: startswith"
.S6: .string "endswith ok"
.S7: .string "FAIL: endswith"
.S8: .string "FAIL: startswith matched wrong"
.S9: .string "no startswith ok"
.text

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
# IR_LIT_I
bb3_α:
 mov qword ptr [r12 + 16], 6
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [r12 + 24], rax
 jmp xgvarg3_done
 xgvarg3_β:
 jmp snoch0_n4_α
.Lx4_0:
 .quad 3
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
 .quad 7
xgvarg5_done:
bb5_α:
# BOX IR_CALL max(...) -> rt_call_arr [operand-marshal, FAIL->ω]
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
  .Lrkfn8: .string "max"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn8]
 lea rsi, [r12 + 64]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 48], rax
 mov qword ptr [r12 + 56], rdx
 cmp eax, 99
 je snoch0_n4_α
 jmp snoch0_n3_α
 snoch0_n2_β:
 jmp snoch0_n4_α
snoch0_n3_α:
bb6_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 48]
 mov rdx, qword ptr [r12 + 56]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n4_α
 snoch0_n3_β:
 jmp snoch0_n4_α
snoch0_n4_α:
# IR_LIT_I
bb7_α:
 mov qword ptr [r12 + 96], 6
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 104], rax
 jmp xgvarg10_done
 xgvarg10_β:
 jmp snoch0_n6_α
.Lx11_0:
 .quad 3
xgvarg10_done:
# IR_LIT_I
bb8_α:
 mov qword ptr [r12 + 112], 6
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [r12 + 120], rax
 jmp xgvarg12_done
 xgvarg12_β:
 jmp snoch0_n6_α
.Lx13_0:
 .quad 7
xgvarg12_done:
bb9_α:
# BOX IR_CALL min(...) -> rt_call_arr [operand-marshal, FAIL->ω]
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
  .Lrkfn15: .string "min"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn15]
 lea rsi, [r12 + 144]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
 cmp eax, 99
 je snoch0_n6_α
 jmp snoch0_n5_α
 snoch0_n4_β:
 jmp snoch0_n6_α
snoch0_n5_α:
bb10_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 128]
 mov rdx, qword ptr [r12 + 136]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n6_α
 snoch0_n5_β:
 jmp snoch0_n6_α
snoch0_n6_α:
# IR_LIT_F
bb11_α:
 mov qword ptr [r12 + 176], 7
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [r12 + 184], rax
 jmp xgvarg17_done
 xgvarg17_β:
 jmp snoch0_n8_α
.Lx18_0:
 .quad 4615063718147915776
xgvarg17_done:
# IR_LIT_F
bb12_α:
 mov qword ptr [r12 + 192], 7
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [r12 + 200], rax
 jmp xgvarg19_done
 xgvarg19_β:
 jmp snoch0_n8_α
.Lx20_0:
 .quad 4611911198408756429
xgvarg19_done:
bb13_α:
# BOX IR_CALL max(...) -> rt_call_arr [operand-marshal, FAIL->ω]
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
  .Lrkfn22: .string "max"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn22]
 lea rsi, [r12 + 224]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 cmp eax, 99
 je snoch0_n8_α
 jmp snoch0_n7_α
 snoch0_n6_β:
 jmp snoch0_n8_α
snoch0_n7_α:
bb14_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 208]
 mov rdx, qword ptr [r12 + 216]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n8_α
 snoch0_n7_β:
 jmp snoch0_n8_α
snoch0_n8_α:
# IR_LIT_F
bb15_α:
 mov qword ptr [r12 + 256], 7
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [r12 + 264], rax
 jmp xgvarg24_done
 xgvarg24_β:
 jmp snoch0_n10_α
.Lx25_0:
 .quad 4615063718147915776
xgvarg24_done:
# IR_LIT_F
bb16_α:
 mov qword ptr [r12 + 272], 7
 mov rax, qword ptr [rip + .Lx27_0]
 mov qword ptr [r12 + 280], rax
 jmp xgvarg26_done
 xgvarg26_β:
 jmp snoch0_n10_α
.Lx27_0:
 .quad 4611911198408756429
xgvarg26_done:
bb17_α:
# BOX IR_CALL min(...) -> rt_call_arr [operand-marshal, FAIL->ω]
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
  .Lrkfn29: .string "min"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn29]
 lea rsi, [r12 + 304]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 288], rax
 mov qword ptr [r12 + 296], rdx
 cmp eax, 99
 je snoch0_n10_α
 jmp snoch0_n9_α
 snoch0_n8_β:
 jmp snoch0_n10_α
snoch0_n9_α:
bb18_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 288]
 mov rdx, qword ptr [r12 + 296]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n10_α
 snoch0_n9_β:
 jmp snoch0_n10_α
snoch0_n10_α:
xargsub32_n0_α:
# IR_LIT_I
bb19_α:
 mov qword ptr [r12 + 336], 6
 mov rax, qword ptr [rip + .Lx33_0]
 mov qword ptr [r12 + 344], rax
 jmp xargsub32_n1_α
 xargsub32_n0_β:
 jmp snoch0_n12_α
.Lx33_0:
 .quad 42
xargsub32_n1_α:
bb20_α:
# IR_UNOP_GVAR_SLOT
 mov rax, 42
 neg rax
 mov qword ptr [r12 + 352], rax
 jmp xgvarg31_done
 xargsub32_n1_β:
 jmp snoch0_n12_α
xgvarg31_done:
bb21_α:
# BOX IR_CALL abs(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+352] -> [r12+376]
 mov rax, qword ptr [r12 + 352]
 mov qword ptr [r12 + 376], rax
 mov rax, qword ptr [r12 + 360]
 mov qword ptr [r12 + 384], rax
  .section .rodata
  .Lrkfn36: .string "abs"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn36]
 lea rsi, [r12 + 376]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 360], rax
 mov qword ptr [r12 + 368], rdx
 cmp eax, 99
 je snoch0_n12_α
 jmp snoch0_n11_α
 snoch0_n10_β:
 jmp snoch0_n12_α
snoch0_n11_α:
bb22_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 360]
 mov rdx, qword ptr [r12 + 368]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n12_α
 snoch0_n11_β:
 jmp snoch0_n12_α
snoch0_n12_α:
# IR_LIT_I
bb23_α:
 mov qword ptr [r12 + 392], 6
 mov rax, qword ptr [rip + .Lx39_0]
 mov qword ptr [r12 + 400], rax
 jmp xgvarg38_done
 xgvarg38_β:
 jmp snoch0_n14_α
.Lx39_0:
 .quad 0
xgvarg38_done:
bb24_α:
# BOX IR_CALL sign(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+392] -> [r12+424]
 mov rax, qword ptr [r12 + 392]
 mov qword ptr [r12 + 424], rax
 mov rax, qword ptr [r12 + 400]
 mov qword ptr [r12 + 432], rax
  .section .rodata
  .Lbynamefn41: .string "sign"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn41]
 lea rsi, [r12 + 424]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 408], rax
 mov qword ptr [r12 + 416], rdx
 cmp eax, 99
 je snoch0_n14_α
 jmp snoch0_n13_α
snoch0_n12_β:
 jmp snoch0_n14_α
snoch0_n13_α:
bb25_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 408]
 mov rdx, qword ptr [r12 + 416]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n14_α
 snoch0_n13_β:
 jmp snoch0_n14_α
snoch0_n14_α:
# IR_LIT_I
bb26_α:
 mov qword ptr [r12 + 440], 6
 mov rax, qword ptr [rip + .Lx44_0]
 mov qword ptr [r12 + 448], rax
 jmp xgvarg43_done
 xgvarg43_β:
 jmp snoch0_n16_α
.Lx44_0:
 .quad 5
xgvarg43_done:
bb27_α:
# BOX IR_CALL sign(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+440] -> [r12+472]
 mov rax, qword ptr [r12 + 440]
 mov qword ptr [r12 + 472], rax
 mov rax, qword ptr [r12 + 448]
 mov qword ptr [r12 + 480], rax
  .section .rodata
  .Lbynamefn46: .string "sign"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn46]
 lea rsi, [r12 + 472]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 456], rax
 mov qword ptr [r12 + 464], rdx
 cmp eax, 99
 je snoch0_n16_α
 jmp snoch0_n15_α
snoch0_n14_β:
 jmp snoch0_n16_α
snoch0_n15_α:
bb28_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 456]
 mov rdx, qword ptr [r12 + 464]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n16_α
 snoch0_n15_β:
 jmp snoch0_n16_α
snoch0_n16_α:
xargsub49_n0_α:
# IR_LIT_I
bb29_α:
 mov qword ptr [r12 + 488], 6
 mov rax, qword ptr [rip + .Lx50_0]
 mov qword ptr [r12 + 496], rax
 jmp xargsub49_n1_α
 xargsub49_n0_β:
 jmp snoch0_n18_α
.Lx50_0:
 .quad 3
xargsub49_n1_α:
bb30_α:
# IR_UNOP_GVAR_SLOT
 mov rax, 3
 neg rax
 mov qword ptr [r12 + 504], rax
 jmp xgvarg48_done
 xargsub49_n1_β:
 jmp snoch0_n18_α
xgvarg48_done:
bb31_α:
# BOX IR_CALL sign(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+504] -> [r12+528]
 mov rax, qword ptr [r12 + 504]
 mov qword ptr [r12 + 528], rax
 mov rax, qword ptr [r12 + 512]
 mov qword ptr [r12 + 536], rax
  .section .rodata
  .Lbynamefn53: .string "sign"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn53]
 lea rsi, [r12 + 528]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 512], rax
 mov qword ptr [r12 + 520], rdx
 cmp eax, 99
 je snoch0_n18_α
 jmp snoch0_n17_α
snoch0_n16_β:
 jmp snoch0_n18_α
snoch0_n17_α:
bb32_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 512]
 mov rdx, qword ptr [r12 + 520]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n18_α
 snoch0_n17_β:
 jmp snoch0_n18_α
snoch0_n18_α:
# IR_LIT_I
bb33_α:
 mov qword ptr [r12 + 544], 6
 mov rax, qword ptr [rip + .Lx56_0]
 mov qword ptr [r12 + 552], rax
 jmp xgvarg55_done
 xgvarg55_β:
 jmp snoch0_n20_α
.Lx56_0:
 .quad 12
xgvarg55_done:
# IR_LIT_I
bb34_α:
 mov qword ptr [r12 + 560], 6
 mov rax, qword ptr [rip + .Lx58_0]
 mov qword ptr [r12 + 568], rax
 jmp xgvarg57_done
 xgvarg57_β:
 jmp snoch0_n20_α
.Lx58_0:
 .quad 8
xgvarg57_done:
bb35_α:
# BOX IR_CALL gcd(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
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
  .Lbynamefn60: .string "gcd"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn60]
 lea rsi, [r12 + 592]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 576], rax
 mov qword ptr [r12 + 584], rdx
 cmp eax, 99
 je snoch0_n20_α
 jmp snoch0_n19_α
snoch0_n18_β:
 jmp snoch0_n20_α
snoch0_n19_α:
bb36_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 576]
 mov rdx, qword ptr [r12 + 584]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n20_α
 snoch0_n19_β:
 jmp snoch0_n20_α
snoch0_n20_α:
# IR_LIT_I
bb37_α:
 mov qword ptr [r12 + 624], 6
 mov rax, qword ptr [rip + .Lx63_0]
 mov qword ptr [r12 + 632], rax
 jmp xgvarg62_done
 xgvarg62_β:
 jmp snoch0_n22_α
.Lx63_0:
 .quad 100
xgvarg62_done:
# IR_LIT_I
bb38_α:
 mov qword ptr [r12 + 640], 6
 mov rax, qword ptr [rip + .Lx65_0]
 mov qword ptr [r12 + 648], rax
 jmp xgvarg64_done
 xgvarg64_β:
 jmp snoch0_n22_α
.Lx65_0:
 .quad 75
xgvarg64_done:
bb39_α:
# BOX IR_CALL gcd(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
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
  .Lbynamefn67: .string "gcd"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn67]
 lea rsi, [r12 + 672]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 656], rax
 mov qword ptr [r12 + 664], rdx
 cmp eax, 99
 je snoch0_n22_α
 jmp snoch0_n21_α
snoch0_n20_β:
 jmp snoch0_n22_α
snoch0_n21_α:
bb40_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 656]
 mov rdx, qword ptr [r12 + 664]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n22_α
 snoch0_n21_β:
 jmp snoch0_n22_α
snoch0_n22_α:
# IR_LIT_I
bb41_α:
 mov qword ptr [r12 + 704], 6
 mov rax, qword ptr [rip + .Lx70_0]
 mov qword ptr [r12 + 712], rax
 jmp xgvarg69_done
 xgvarg69_β:
 jmp flat_γ
.Lx70_0:
 .quad 4
xgvarg69_done:
# IR_LIT_I
bb42_α:
 mov qword ptr [r12 + 720], 6
 mov rax, qword ptr [rip + .Lx72_0]
 mov qword ptr [r12 + 728], rax
 jmp xgvarg71_done
 xgvarg71_β:
 jmp flat_γ
.Lx72_0:
 .quad 6
xgvarg71_done:
bb43_α:
# BOX IR_CALL lcm(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
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
  .Lbynamefn74: .string "lcm"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn74]
 lea rsi, [r12 + 752]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 736], rax
 mov qword ptr [r12 + 744], rdx
 cmp eax, 99
 je flat_γ
 jmp snoch0_n23_α
snoch0_n22_β:
 jmp flat_γ
snoch0_n23_α:
bb44_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 736]
 mov rdx, qword ptr [r12 + 744]
 call rt_gvar_assign_descr@PLT
 jmp flat_γ
 snoch0_n23_β:
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
.text

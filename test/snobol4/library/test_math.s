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
xgvarg31_done:
bb19_α:
# BOX IR_CALL abs(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = inline gvar-unop subexpr -> [r12+352]
 movabs rax, 42
 neg rax
 mov qword ptr [r12 + 352], 6
 mov qword ptr [r12 + 360], rax
  .section .rodata
  .Lrkfn33: .string "abs"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn33]
 lea rsi, [r12 + 352]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 cmp eax, 99
 je snoch0_n12_α
 jmp snoch0_n11_α
 snoch0_n10_β:
 jmp snoch0_n12_α
snoch0_n11_α:
bb20_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 336]
 mov rdx, qword ptr [r12 + 344]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n12_α
 snoch0_n11_β:
 jmp snoch0_n12_α
snoch0_n12_α:
# IR_LIT_I
bb21_α:
 mov qword ptr [r12 + 368], 6
 mov rax, qword ptr [rip + .Lx36_0]
 mov qword ptr [r12 + 376], rax
 jmp xgvarg35_done
 xgvarg35_β:
 jmp snoch0_n14_α
.Lx36_0:
 .quad 0
xgvarg35_done:
bb22_α:
# BOX IR_CALL sign(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+368] -> [r12+400]
 mov rax, qword ptr [r12 + 368]
 mov qword ptr [r12 + 400], rax
 mov rax, qword ptr [r12 + 376]
 mov qword ptr [r12 + 408], rax
  .section .rodata
  .Lbynamefn38: .string "sign"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn38]
 lea rsi, [r12 + 400]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 cmp eax, 99
 je snoch0_n14_α
 jmp snoch0_n13_α
snoch0_n12_β:
 jmp snoch0_n14_α
snoch0_n13_α:
bb23_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 384]
 mov rdx, qword ptr [r12 + 392]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n14_α
 snoch0_n13_β:
 jmp snoch0_n14_α
snoch0_n14_α:
# IR_LIT_I
bb24_α:
 mov qword ptr [r12 + 416], 6
 mov rax, qword ptr [rip + .Lx41_0]
 mov qword ptr [r12 + 424], rax
 jmp xgvarg40_done
 xgvarg40_β:
 jmp snoch0_n16_α
.Lx41_0:
 .quad 5
xgvarg40_done:
bb25_α:
# BOX IR_CALL sign(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+416] -> [r12+448]
 mov rax, qword ptr [r12 + 416]
 mov qword ptr [r12 + 448], rax
 mov rax, qword ptr [r12 + 424]
 mov qword ptr [r12 + 456], rax
  .section .rodata
  .Lbynamefn43: .string "sign"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn43]
 lea rsi, [r12 + 448]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 432], rax
 mov qword ptr [r12 + 440], rdx
 cmp eax, 99
 je snoch0_n16_α
 jmp snoch0_n15_α
snoch0_n14_β:
 jmp snoch0_n16_α
snoch0_n15_α:
bb26_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 432]
 mov rdx, qword ptr [r12 + 440]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n16_α
 snoch0_n15_β:
 jmp snoch0_n16_α
snoch0_n16_α:
xgvarg45_done:
bb27_α:
# BOX IR_CALL sign(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = inline gvar-unop subexpr -> [r12+480]
 movabs rax, 3
 neg rax
 mov qword ptr [r12 + 480], 6
 mov qword ptr [r12 + 488], rax
  .section .rodata
  .Lbynamefn47: .string "sign"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn47]
 lea rsi, [r12 + 480]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 464], rax
 mov qword ptr [r12 + 472], rdx
 cmp eax, 99
 je snoch0_n18_α
 jmp snoch0_n17_α
snoch0_n16_β:
 jmp snoch0_n18_α
snoch0_n17_α:
bb28_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 464]
 mov rdx, qword ptr [r12 + 472]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n18_α
 snoch0_n17_β:
 jmp snoch0_n18_α
snoch0_n18_α:
# IR_LIT_I
bb29_α:
 mov qword ptr [r12 + 496], 6
 mov rax, qword ptr [rip + .Lx50_0]
 mov qword ptr [r12 + 504], rax
 jmp xgvarg49_done
 xgvarg49_β:
 jmp snoch0_n20_α
.Lx50_0:
 .quad 12
xgvarg49_done:
# IR_LIT_I
bb30_α:
 mov qword ptr [r12 + 512], 6
 mov rax, qword ptr [rip + .Lx52_0]
 mov qword ptr [r12 + 520], rax
 jmp xgvarg51_done
 xgvarg51_β:
 jmp snoch0_n20_α
.Lx52_0:
 .quad 8
xgvarg51_done:
bb31_α:
# BOX IR_CALL gcd(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
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
  .Lbynamefn54: .string "gcd"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn54]
 lea rsi, [r12 + 544]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 528], rax
 mov qword ptr [r12 + 536], rdx
 cmp eax, 99
 je snoch0_n20_α
 jmp snoch0_n19_α
snoch0_n18_β:
 jmp snoch0_n20_α
snoch0_n19_α:
bb32_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 528]
 mov rdx, qword ptr [r12 + 536]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n20_α
 snoch0_n19_β:
 jmp snoch0_n20_α
snoch0_n20_α:
# IR_LIT_I
bb33_α:
 mov qword ptr [r12 + 576], 6
 mov rax, qword ptr [rip + .Lx57_0]
 mov qword ptr [r12 + 584], rax
 jmp xgvarg56_done
 xgvarg56_β:
 jmp snoch0_n22_α
.Lx57_0:
 .quad 100
xgvarg56_done:
# IR_LIT_I
bb34_α:
 mov qword ptr [r12 + 592], 6
 mov rax, qword ptr [rip + .Lx59_0]
 mov qword ptr [r12 + 600], rax
 jmp xgvarg58_done
 xgvarg58_β:
 jmp snoch0_n22_α
.Lx59_0:
 .quad 75
xgvarg58_done:
bb35_α:
# BOX IR_CALL gcd(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
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
  .Lbynamefn61: .string "gcd"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn61]
 lea rsi, [r12 + 624]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 608], rax
 mov qword ptr [r12 + 616], rdx
 cmp eax, 99
 je snoch0_n22_α
 jmp snoch0_n21_α
snoch0_n20_β:
 jmp snoch0_n22_α
snoch0_n21_α:
bb36_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 608]
 mov rdx, qword ptr [r12 + 616]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n22_α
 snoch0_n21_β:
 jmp snoch0_n22_α
snoch0_n22_α:
# IR_LIT_I
bb37_α:
 mov qword ptr [r12 + 656], 6
 mov rax, qword ptr [rip + .Lx64_0]
 mov qword ptr [r12 + 664], rax
 jmp xgvarg63_done
 xgvarg63_β:
 jmp flat_γ
.Lx64_0:
 .quad 4
xgvarg63_done:
# IR_LIT_I
bb38_α:
 mov qword ptr [r12 + 672], 6
 mov rax, qword ptr [rip + .Lx66_0]
 mov qword ptr [r12 + 680], rax
 jmp xgvarg65_done
 xgvarg65_β:
 jmp flat_γ
.Lx66_0:
 .quad 6
xgvarg65_done:
bb39_α:
# BOX IR_CALL lcm(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
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
  .Lbynamefn68: .string "lcm"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn68]
 lea rsi, [r12 + 704]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 688], rax
 mov qword ptr [r12 + 696], rdx
 cmp eax, 99
 je flat_γ
 jmp snoch0_n23_α
snoch0_n22_β:
 jmp flat_γ
snoch0_n23_α:
bb40_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 688]
 mov rdx, qword ptr [r12 + 696]
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

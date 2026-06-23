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
xgvarg1_done:
# IR_LIT_I
bb1_α:
 mov qword ptr [r12 + 16], 6
 mov rax, qword ptr [rip + .Lx3_0]
 mov qword ptr [r12 + 24], rax
 jmp xgvarg2_done
 xgvarg2_β:
 jmp snoch0_n2_α
.Lx3_0:
 .quad 5
xgvarg2_done:
bb2_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = inline gvar-arith subexpr -> [r12+48]
 movabs rax, 3
 mov qword ptr [r12 + 80], rax
 mov rcx, 2
 mov rax, qword ptr [r12 + 80]
 add rax, rcx
 mov qword ptr [r12 + 48], 6
 mov qword ptr [r12 + 56], rax
# marshal arg1 = producer-box slot [r12+16] -> [r12+64]
 mov rax, qword ptr [r12 + 16]
 mov qword ptr [r12 + 64], rax
 mov rax, qword ptr [r12 + 24]
 mov qword ptr [r12 + 72], rax
  .section .rodata
  .Lbynamefn5: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn5]
 lea rsi, [r12 + 48]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je snoch0_n2_α
 jmp snoch0_n1_α
snoch0_n0_β:
 jmp snoch0_n2_α
snoch0_n1_α:
# IR_LIT_scalar
bb3_α:
 jmp snoch0_n3_α
 snoch0_n1_β:
 jmp flat_γ
snoch0_n2_α:
xgvarg7_done:
# IR_LIT_I
bb4_α:
 mov qword ptr [r12 + 96], 6
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 104], rax
 jmp xgvarg8_done
 xgvarg8_β:
 jmp snoch0_n5_α
.Lx9_0:
 .quad 1
xgvarg8_done:
bb5_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = inline gvar-arith subexpr -> [r12+128]
 movabs rax, 3
 mov qword ptr [r12 + 160], rax
 mov rcx, 2
 mov rax, qword ptr [r12 + 160]
 sub rax, rcx
 mov qword ptr [r12 + 128], 6
 mov qword ptr [r12 + 136], rax
# marshal arg1 = producer-box slot [r12+96] -> [r12+144]
 mov rax, qword ptr [r12 + 96]
 mov qword ptr [r12 + 144], rax
 mov rax, qword ptr [r12 + 104]
 mov qword ptr [r12 + 152], rax
  .section .rodata
  .Lbynamefn11: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn11]
 lea rsi, [r12 + 128]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 cmp eax, 99
 je snoch0_n5_α
 jmp snoch0_n4_α
snoch0_n2_β:
 jmp snoch0_n5_α
snoch0_n3_α:
bb6_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S1]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n3_β:
 jmp flat_γ
snoch0_n4_α:
# IR_LIT_scalar
bb7_α:
 jmp snoch0_n6_α
 snoch0_n4_β:
 jmp flat_γ
snoch0_n5_α:
xgvarg14_done:
# IR_LIT_I
bb8_α:
 mov qword ptr [r12 + 176], 6
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [r12 + 184], rax
 jmp xgvarg15_done
 xgvarg15_β:
 jmp snoch0_n8_α
.Lx16_0:
 .quad 6
xgvarg15_done:
bb9_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = inline gvar-arith subexpr -> [r12+208]
 movabs rax, 3
 mov qword ptr [r12 + 240], rax
 mov rcx, 2
 mov rax, qword ptr [r12 + 240]
 imul rax, rcx
 mov qword ptr [r12 + 208], 6
 mov qword ptr [r12 + 216], rax
# marshal arg1 = producer-box slot [r12+176] -> [r12+224]
 mov rax, qword ptr [r12 + 176]
 mov qword ptr [r12 + 224], rax
 mov rax, qword ptr [r12 + 184]
 mov qword ptr [r12 + 232], rax
  .section .rodata
  .Lbynamefn18: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn18]
 lea rsi, [r12 + 208]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 cmp eax, 99
 je snoch0_n8_α
 jmp snoch0_n7_α
snoch0_n5_β:
 jmp snoch0_n8_α
snoch0_n6_α:
bb10_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S2]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n6_β:
 jmp flat_γ
snoch0_n7_α:
# IR_LIT_scalar
bb11_α:
 jmp snoch0_n9_α
 snoch0_n7_β:
 jmp flat_γ
snoch0_n8_α:
xgvarg21_done:
# IR_LIT_I
bb12_α:
 mov qword ptr [r12 + 256], 6
 mov rax, qword ptr [rip + .Lx23_0]
 mov qword ptr [r12 + 264], rax
 jmp xgvarg22_done
 xgvarg22_β:
 jmp snoch0_n11_α
.Lx23_0:
 .quad 2
xgvarg22_done:
bb13_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = inline gvar-arith subexpr -> [r12+288]
 movabs rax, 5
 mov qword ptr [r12 + 320], rax
 mov rcx, 2
 mov rax, qword ptr [r12 + 320]
 cqo
 idiv rcx
 mov qword ptr [r12 + 288], 6
 mov qword ptr [r12 + 296], rax
# marshal arg1 = producer-box slot [r12+256] -> [r12+304]
 mov rax, qword ptr [r12 + 256]
 mov qword ptr [r12 + 304], rax
 mov rax, qword ptr [r12 + 264]
 mov qword ptr [r12 + 312], rax
  .section .rodata
  .Lbynamefn25: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn25]
 lea rsi, [r12 + 288]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 cmp eax, 99
 je snoch0_n11_α
 jmp snoch0_n10_α
snoch0_n8_β:
 jmp snoch0_n11_α
snoch0_n9_α:
bb14_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S3]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n9_β:
 jmp flat_γ
snoch0_n10_α:
# IR_LIT_scalar
bb15_α:
 jmp snoch0_n12_α
 snoch0_n10_β:
 jmp flat_γ
snoch0_n11_α:
xargsub29_n0_α:
# IR_LIT_I
bb16_α:
 mov qword ptr [r12 + 336], 6
 mov rax, qword ptr [rip + .Lx30_0]
 mov qword ptr [r12 + 344], rax
 jmp xargsub29_n1_α
 xargsub29_n0_β:
 jmp snoch0_n14_α
.Lx30_0:
 .quad 2
xargsub29_n1_α:
# IR_LIT_I
bb17_α:
 mov qword ptr [r12 + 352], 6
 mov rax, qword ptr [rip + .Lx31_0]
 mov qword ptr [r12 + 360], rax
 jmp xargsub29_n2_α
 xargsub29_n1_β:
 jmp snoch0_n14_α
.Lx31_0:
 .quad 3
xargsub29_n2_α:
xgvarg28_done:
# IR_LIT_I
bb19_α:
 mov qword ptr [r12 + 368], 6
 mov rax, qword ptr [rip + .Lx34_0]
 mov qword ptr [r12 + 376], rax
 jmp xgvarg33_done
 xgvarg33_β:
 jmp snoch0_n14_α
.Lx34_0:
 .quad 8
xgvarg33_done:
bb20_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = LIT_I -> [r12+400]
 mov qword ptr [r12 + 400], 6
 movabs rax, 2
 mov qword ptr [r12 + 408], rax
# marshal arg1 = producer-box slot [r12+368] -> [r12+416]
 mov rax, qword ptr [r12 + 368]
 mov qword ptr [r12 + 416], rax
 mov rax, qword ptr [r12 + 376]
 mov qword ptr [r12 + 424], rax
  .section .rodata
  .Lbynamefn36: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn36]
 lea rsi, [r12 + 400]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 cmp eax, 99
 je snoch0_n14_α
 jmp snoch0_n13_α
snoch0_n11_β:
 jmp snoch0_n14_α
snoch0_n12_α:
bb21_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S4]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n12_β:
 jmp flat_γ
snoch0_n13_α:
# IR_LIT_scalar
bb22_α:
 jmp snoch0_n15_α
 snoch0_n13_β:
 jmp flat_γ
snoch0_n14_α:
xgvarg39_done:
# IR_LIT_I
bb23_α:
 mov qword ptr [r12 + 432], 6
 mov rax, qword ptr [rip + .Lx41_0]
 mov qword ptr [r12 + 440], rax
 jmp xgvarg40_done
 xgvarg40_β:
 jmp snoch0_n17_α
.Lx41_0:
 .quad 5
xgvarg40_done:
bb24_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = LIT_S (string REG-RO sealed in-band) -> [r12+464]
 mov qword ptr [r12 + 464], 1
 mov rax, qword ptr [rip + .Lx42_0]
 mov qword ptr [r12 + 472], rax
 jmp .Lx42_1
.Lx42_0:
 .quad .Lx42_0_s
.Lx42_0_s:
 .string "3"
.Lx42_1:
# marshal arg1 = producer-box slot [r12+432] -> [r12+480]
 mov rax, qword ptr [r12 + 432]
 mov qword ptr [r12 + 480], rax
 mov rax, qword ptr [r12 + 440]
 mov qword ptr [r12 + 488], rax
  .section .rodata
  .Lbynamefn43: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn43]
 lea rsi, [r12 + 464]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 448], rax
 mov qword ptr [r12 + 456], rdx
 cmp eax, 99
 je snoch0_n17_α
 jmp snoch0_n16_α
snoch0_n14_β:
 jmp snoch0_n17_α
snoch0_n15_α:
bb25_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S5]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n15_β:
 jmp flat_γ
snoch0_n16_α:
# IR_LIT_scalar
bb26_α:
 jmp snoch0_n18_α
 snoch0_n16_β:
 jmp flat_γ
snoch0_n17_α:
xgvarg46_done:
# IR_LIT_I
bb27_α:
 mov qword ptr [r12 + 496], 6
 mov rax, qword ptr [rip + .Lx48_0]
 mov qword ptr [r12 + 504], rax
 jmp xgvarg47_done
 xgvarg47_β:
 jmp snoch0_n20_α
.Lx48_0:
 .quad 1
xgvarg47_done:
bb28_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = LIT_I -> [r12+528]
 mov qword ptr [r12 + 528], 6
 movabs rax, 3
 mov qword ptr [r12 + 536], rax
# marshal arg1 = producer-box slot [r12+496] -> [r12+544]
 mov rax, qword ptr [r12 + 496]
 mov qword ptr [r12 + 544], rax
 mov rax, qword ptr [r12 + 504]
 mov qword ptr [r12 + 552], rax
  .section .rodata
  .Lbynamefn50: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn50]
 lea rsi, [r12 + 528]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 512], rax
 mov qword ptr [r12 + 520], rdx
 cmp eax, 99
 je snoch0_n20_α
 jmp snoch0_n19_α
snoch0_n17_β:
 jmp snoch0_n20_α
snoch0_n18_α:
bb29_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S6]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n18_β:
 jmp flat_γ
snoch0_n19_α:
# IR_LIT_scalar
bb30_α:
 jmp snoch0_n21_α
 snoch0_n19_β:
 jmp flat_γ
snoch0_n20_α:
xgvarg53_done:
# IR_LIT_I
bb31_α:
 mov qword ptr [r12 + 560], 6
 mov rax, qword ptr [rip + .Lx55_0]
 mov qword ptr [r12 + 568], rax
 jmp xgvarg54_done
 xgvarg54_β:
 jmp snoch0_n23_α
.Lx55_0:
 .quad 1
xgvarg54_done:
bb32_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = LIT_S (string REG-RO sealed in-band) -> [r12+592]
 mov qword ptr [r12 + 592], 1
 mov rax, qword ptr [rip + .Lx56_0]
 mov qword ptr [r12 + 600], rax
 jmp .Lx56_1
.Lx56_0:
 .quad .Lx56_0_s
.Lx56_0_s:
 .string "1"
.Lx56_1:
# marshal arg1 = producer-box slot [r12+560] -> [r12+608]
 mov rax, qword ptr [r12 + 560]
 mov qword ptr [r12 + 608], rax
 mov rax, qword ptr [r12 + 568]
 mov qword ptr [r12 + 616], rax
  .section .rodata
  .Lbynamefn57: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn57]
 lea rsi, [r12 + 592]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 576], rax
 mov qword ptr [r12 + 584], rdx
 cmp eax, 99
 je snoch0_n23_α
 jmp snoch0_n22_α
snoch0_n20_β:
 jmp snoch0_n23_α
snoch0_n21_α:
bb33_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S7]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n21_β:
 jmp flat_γ
snoch0_n22_α:
# IR_LIT_scalar
bb34_α:
 jmp snoch0_n24_α
 snoch0_n22_β:
 jmp flat_γ
snoch0_n23_α:
xgvarg60_done:
# IR_LIT_I
bb35_α:
 mov qword ptr [r12 + 624], 6
 mov rax, qword ptr [rip + .Lx62_0]
 mov qword ptr [r12 + 632], rax
 jmp xgvarg61_done
 xgvarg61_β:
 jmp snoch0_n26_α
.Lx62_0:
 .quad 5
xgvarg61_done:
bb36_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = LIT_I -> [r12+656]
 mov qword ptr [r12 + 656], 6
 movabs rax, 5
 mov qword ptr [r12 + 664], rax
# marshal arg1 = producer-box slot [r12+624] -> [r12+672]
 mov rax, qword ptr [r12 + 624]
 mov qword ptr [r12 + 672], rax
 mov rax, qword ptr [r12 + 632]
 mov qword ptr [r12 + 680], rax
  .section .rodata
  .Lbynamefn64: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn64]
 lea rsi, [r12 + 656]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 640], rax
 mov qword ptr [r12 + 648], rdx
 cmp eax, 99
 je snoch0_n26_α
 jmp snoch0_n25_α
snoch0_n23_β:
 jmp snoch0_n26_α
snoch0_n24_α:
bb37_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S8]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n24_β:
 jmp flat_γ
snoch0_n25_α:
# IR_LIT_scalar
bb38_α:
 jmp snoch0_n27_α
 snoch0_n25_β:
 jmp flat_γ
snoch0_n26_α:
# IR_LIT_scalar
bb39_α:
 jmp snoch0_n28_α
 snoch0_n26_β:
 jmp flat_γ
snoch0_n27_α:
bb40_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S9]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n27_β:
 jmp flat_γ
snoch0_n28_α:
bb41_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S10]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n28_β:
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
.S1: .string "FAIL 410/001: 3+2"
.S2: .string "FAIL 410/002: 3-2"
.S3: .string "FAIL 410/003: 3*2"
.S4: .string "FAIL 410/004: 5/2 integer division"
.S5: .string "FAIL 410/005: 2**3"
.S6: .string "FAIL 410/006: string+int coerce"
.S7: .string "FAIL 410/007: int+neg-string"
.S8: .string "FAIL 410/008: string+string coerce"
.S9: .string "FAIL 410/009: null addend is zero"
.S10: .string "PASS 410_arith_int (9/9)"
.text

  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "output"
  .align 8
__gva_names:
  .quad .Lgvan0
  .section .bss
  .align 16
__gva: .space 16, 0
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  push rbp
  mov rbp, rsp
  call core_lib_init@PLT
  call rt_proc_reset@PLT
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 1
  call gva_register@PLT
  mov rbx, rax
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
xgvarg28_done:
# IR_LIT_I
bb16_α:
 mov qword ptr [r12 + 336], 6
 mov rax, qword ptr [rip + .Lx30_0]
 mov qword ptr [r12 + 344], rax
 jmp xgvarg29_done
 xgvarg29_β:
 jmp snoch0_n14_α
.Lx30_0:
 .quad 8
xgvarg29_done:
bb17_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = inline gvar-pow subexpr -> [r12+368]
 mov rdi, 6
 movabs rsi, 2
 mov rdx, 6
 movabs rcx, 3
 call POWER_fn@PLT
 mov qword ptr [r12 + 368], rax
 mov qword ptr [r12 + 376], rdx
# marshal arg1 = producer-box slot [r12+336] -> [r12+384]
 mov rax, qword ptr [r12 + 336]
 mov qword ptr [r12 + 384], rax
 mov rax, qword ptr [r12 + 344]
 mov qword ptr [r12 + 392], rax
  .section .rodata
  .Lbynamefn32: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn32]
 lea rsi, [r12 + 368]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 352], rax
 mov qword ptr [r12 + 360], rdx
 cmp eax, 99
 je snoch0_n14_α
 jmp snoch0_n13_α
snoch0_n11_β:
 jmp snoch0_n14_α
snoch0_n12_α:
bb18_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S4]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n12_β:
 jmp flat_γ
snoch0_n13_α:
# IR_LIT_scalar
bb19_α:
 jmp snoch0_n15_α
 snoch0_n13_β:
 jmp flat_γ
snoch0_n14_α:
xgvarg35_done:
# IR_LIT_I
bb20_α:
 mov qword ptr [r12 + 400], 6
 mov rax, qword ptr [rip + .Lx37_0]
 mov qword ptr [r12 + 408], rax
 jmp xgvarg36_done
 xgvarg36_β:
 jmp snoch0_n17_α
.Lx37_0:
 .quad 5
xgvarg36_done:
bb21_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = inline gvar-arith subexpr -> [r12+432]
 movabs rax, 3
 mov qword ptr [r12 + 464], rax
 mov rcx, 2
 mov rax, qword ptr [r12 + 464]
 add rax, rcx
 mov qword ptr [r12 + 432], 6
 mov qword ptr [r12 + 440], rax
# marshal arg1 = producer-box slot [r12+400] -> [r12+448]
 mov rax, qword ptr [r12 + 400]
 mov qword ptr [r12 + 448], rax
 mov rax, qword ptr [r12 + 408]
 mov qword ptr [r12 + 456], rax
  .section .rodata
  .Lbynamefn39: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn39]
 lea rsi, [r12 + 432]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 cmp eax, 99
 je snoch0_n17_α
 jmp snoch0_n16_α
snoch0_n14_β:
 jmp snoch0_n17_α
snoch0_n15_α:
bb22_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S5]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n15_β:
 jmp flat_γ
snoch0_n16_α:
# IR_LIT_scalar
bb23_α:
 jmp snoch0_n18_α
 snoch0_n16_β:
 jmp flat_γ
snoch0_n17_α:
xgvarg42_done:
# IR_LIT_I
bb24_α:
 mov qword ptr [r12 + 480], 6
 mov rax, qword ptr [rip + .Lx44_0]
 mov qword ptr [r12 + 488], rax
 jmp xgvarg43_done
 xgvarg43_β:
 jmp snoch0_n20_α
.Lx44_0:
 .quad 1
xgvarg43_done:
bb25_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = inline gvar-arith subexpr -> [r12+512]
 movabs rax, 3
 mov qword ptr [r12 + 544], rax
 movabs rcx, 18446744073709551614
 mov rax, qword ptr [r12 + 544]
 add rax, rcx
 mov qword ptr [r12 + 512], 6
 mov qword ptr [r12 + 520], rax
# marshal arg1 = producer-box slot [r12+480] -> [r12+528]
 mov rax, qword ptr [r12 + 480]
 mov qword ptr [r12 + 528], rax
 mov rax, qword ptr [r12 + 488]
 mov qword ptr [r12 + 536], rax
  .section .rodata
  .Lbynamefn46: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn46]
 lea rsi, [r12 + 512]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 496], rax
 mov qword ptr [r12 + 504], rdx
 cmp eax, 99
 je snoch0_n20_α
 jmp snoch0_n19_α
snoch0_n17_β:
 jmp snoch0_n20_α
snoch0_n18_α:
bb26_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S6]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n18_β:
 jmp flat_γ
snoch0_n19_α:
# IR_LIT_scalar
bb27_α:
 jmp snoch0_n21_α
 snoch0_n19_β:
 jmp flat_γ
snoch0_n20_α:
xgvarg49_done:
# IR_LIT_I
bb28_α:
 mov qword ptr [r12 + 560], 6
 mov rax, qword ptr [rip + .Lx51_0]
 mov qword ptr [r12 + 568], rax
 jmp xgvarg50_done
 xgvarg50_β:
 jmp snoch0_n23_α
.Lx51_0:
 .quad 1
xgvarg50_done:
bb29_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = inline gvar-arith subexpr -> [r12+592]
 movabs rax, 1
 mov qword ptr [r12 + 624], rax
 movabs rcx, 0
 mov rax, qword ptr [r12 + 624]
 add rax, rcx
 mov qword ptr [r12 + 592], 6
 mov qword ptr [r12 + 600], rax
# marshal arg1 = producer-box slot [r12+560] -> [r12+608]
 mov rax, qword ptr [r12 + 560]
 mov qword ptr [r12 + 608], rax
 mov rax, qword ptr [r12 + 568]
 mov qword ptr [r12 + 616], rax
  .section .rodata
  .Lbynamefn53: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn53]
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
bb30_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S7]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n21_β:
 jmp flat_γ
snoch0_n22_α:
# IR_LIT_scalar
bb31_α:
 jmp snoch0_n24_α
 snoch0_n22_β:
 jmp flat_γ
snoch0_n23_α:
xgvarg56_done:
# IR_LIT_I
bb32_α:
 mov qword ptr [r12 + 640], 6
 mov rax, qword ptr [rip + .Lx58_0]
 mov qword ptr [r12 + 648], rax
 jmp xgvarg57_done
 xgvarg57_β:
 jmp snoch0_n26_α
.Lx58_0:
 .quad 5
xgvarg57_done:
bb33_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = inline gvar-arith subexpr -> [r12+672]
 movabs rax, 5
 mov qword ptr [r12 + 704], rax
 movabs rcx, 0
 mov rax, qword ptr [r12 + 704]
 add rax, rcx
 mov qword ptr [r12 + 672], 6
 mov qword ptr [r12 + 680], rax
# marshal arg1 = producer-box slot [r12+640] -> [r12+688]
 mov rax, qword ptr [r12 + 640]
 mov qword ptr [r12 + 688], rax
 mov rax, qword ptr [r12 + 648]
 mov qword ptr [r12 + 696], rax
  .section .rodata
  .Lbynamefn60: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn60]
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
bb34_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S8]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n24_β:
 jmp flat_γ
snoch0_n25_α:
# IR_LIT_scalar
bb35_α:
 jmp snoch0_n27_α
 snoch0_n25_β:
 jmp flat_γ
snoch0_n26_α:
# IR_LIT_scalar
bb36_α:
 jmp snoch0_n28_α
 snoch0_n26_β:
 jmp flat_γ
snoch0_n27_α:
bb37_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S9]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n27_β:
 jmp flat_γ
snoch0_n28_α:
bb38_α:
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

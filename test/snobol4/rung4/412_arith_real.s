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
# IR_LIT_F
bb1_α:
 mov qword ptr [r12 + 16], 7
 mov rax, qword ptr [rip + .Lx3_0]
 mov qword ptr [r12 + 24], rax
 jmp xgvarg2_done
 xgvarg2_β:
 jmp snoch0_n2_α
.Lx3_0:
 .quad 4617315517961601024
xgvarg2_done:
bb2_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = LIT_F -> [r12+48]
 mov qword ptr [r12 + 48], 7
 movabs rax, 4611686018427387904
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
# IR_LIT_F
bb4_α:
 mov qword ptr [r12 + 80], 7
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 88], rax
 jmp xgvarg8_done
 xgvarg8_β:
 jmp snoch0_n5_α
.Lx9_0:
 .quad 4611686018427387904
xgvarg8_done:
bb5_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = LIT_F -> [r12+112]
 mov qword ptr [r12 + 112], 7
 movabs rax, 4613937818241073152
 mov qword ptr [r12 + 120], rax
# marshal arg1 = producer-box slot [r12+80] -> [r12+128]
 mov rax, qword ptr [r12 + 80]
 mov qword ptr [r12 + 128], rax
 mov rax, qword ptr [r12 + 88]
 mov qword ptr [r12 + 136], rax
  .section .rodata
  .Lbynamefn11: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn11]
 lea rsi, [r12 + 112]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
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
# IR_LIT_F
bb8_α:
 mov qword ptr [r12 + 144], 7
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [r12 + 152], rax
 jmp xgvarg15_done
 xgvarg15_β:
 jmp snoch0_n8_α
.Lx16_0:
 .quad 4618441417868443648
xgvarg15_done:
bb9_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = LIT_F -> [r12+176]
 mov qword ptr [r12 + 176], 7
 movabs rax, 4613937818241073152
 mov qword ptr [r12 + 184], rax
# marshal arg1 = producer-box slot [r12+144] -> [r12+192]
 mov rax, qword ptr [r12 + 144]
 mov qword ptr [r12 + 192], rax
 mov rax, qword ptr [r12 + 152]
 mov qword ptr [r12 + 200], rax
  .section .rodata
  .Lbynamefn18: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn18]
 lea rsi, [r12 + 176]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
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
# IR_LIT_F
bb12_α:
 mov qword ptr [r12 + 208], 7
 mov rax, qword ptr [rip + .Lx23_0]
 mov qword ptr [r12 + 216], rax
 jmp xgvarg22_done
 xgvarg22_β:
 jmp snoch0_n11_α
.Lx23_0:
 .quad 4609434218613702656
xgvarg22_done:
bb13_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = LIT_F -> [r12+240]
 mov qword ptr [r12 + 240], 7
 movabs rax, 4613937818241073152
 mov qword ptr [r12 + 248], rax
# marshal arg1 = producer-box slot [r12+208] -> [r12+256]
 mov rax, qword ptr [r12 + 208]
 mov qword ptr [r12 + 256], rax
 mov rax, qword ptr [r12 + 216]
 mov qword ptr [r12 + 264], rax
  .section .rodata
  .Lbynamefn25: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn25]
 lea rsi, [r12 + 240]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
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
# IR_LIT_F
bb16_α:
 mov qword ptr [r12 + 272], 7
 mov rax, qword ptr [rip + .Lx30_0]
 mov qword ptr [r12 + 280], rax
 jmp xargsub29_n1_α
 xargsub29_n0_β:
 jmp snoch0_n14_α
.Lx30_0:
 .quad 4613937818241073152
xargsub29_n1_α:
# IR_LIT_I
bb17_α:
 mov qword ptr [r12 + 288], 6
 mov rax, qword ptr [rip + .Lx31_0]
 mov qword ptr [r12 + 296], rax
 jmp xargsub29_n2_α
 xargsub29_n1_β:
 jmp snoch0_n14_α
.Lx31_0:
 .quad 3
xargsub29_n2_α:
jmp xbinop32_lhs_done
xbinop32_lhs_done:
jmp xbinop32_rhs_done
xbinop32_rhs_done:
xgvarg28_done:
# IR_LIT_F
bb19_α:
 mov qword ptr [r12 + 304], 7
 mov rax, qword ptr [rip + .Lx35_0]
 mov qword ptr [r12 + 312], rax
 jmp xgvarg34_done
 xgvarg34_β:
 jmp snoch0_n14_α
.Lx35_0:
 .quad 4628293042053316608
xgvarg34_done:
bb20_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = LIT_F -> [r12+336]
 mov qword ptr [r12 + 336], 7
 movabs rax, 4613937818241073152
 mov qword ptr [r12 + 344], rax
# marshal arg1 = producer-box slot [r12+304] -> [r12+352]
 mov rax, qword ptr [r12 + 304]
 mov qword ptr [r12 + 352], rax
 mov rax, qword ptr [r12 + 312]
 mov qword ptr [r12 + 360], rax
  .section .rodata
  .Lbynamefn37: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn37]
 lea rsi, [r12 + 336]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
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
xargsub41_n0_α:
# IR_LIT_F
bb23_α:
 mov qword ptr [r12 + 368], 7
 mov rax, qword ptr [rip + .Lx42_0]
 mov qword ptr [r12 + 376], rax
 jmp xargsub41_n1_α
 xargsub41_n0_β:
 jmp snoch0_n17_α
.Lx42_0:
 .quad 4607182418800017408
xargsub41_n1_α:
bb24_α:
# IR_UNOP_GVAR_SLOT
 mov rax, qword ptr [r12 + 368]
 neg rax
 mov qword ptr [r12 + 384], rax
 jmp xgvarg40_done
 xargsub41_n1_β:
 jmp snoch0_n17_α
xgvarg40_done:
xgvarg44_done:
bb25_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+384] -> [r12+408]
 mov rax, qword ptr [r12 + 384]
 mov qword ptr [r12 + 408], rax
 mov rax, qword ptr [r12 + 392]
 mov qword ptr [r12 + 416], rax
# marshal arg1 = LIT_F -> [r12+424]
 mov qword ptr [r12 + 424], 7
 movabs rax, 0
 mov qword ptr [r12 + 432], rax
  .section .rodata
  .Lbynamefn46: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn46]
 lea rsi, [r12 + 408]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 392], rax
 mov qword ptr [r12 + 400], rdx
 cmp eax, 99
 je snoch0_n17_α
 jmp snoch0_n16_α
snoch0_n14_β:
 jmp snoch0_n17_α
snoch0_n15_α:
bb26_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S5]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n15_β:
 jmp flat_γ
snoch0_n16_α:
# IR_LIT_scalar
bb27_α:
 jmp snoch0_n18_α
 snoch0_n16_β:
 jmp flat_γ
snoch0_n17_α:
# IR_LIT_scalar
bb28_α:
 jmp snoch0_n19_α
 snoch0_n17_β:
 jmp flat_γ
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
bb30_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S7]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n19_β:
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
.S1: .string "FAIL 412/001: 2.0+3.0"
.S2: .string "FAIL 412/002: 3.0-1.0"
.S3: .string "FAIL 412/003: 3.0*2.0"
.S4: .string "FAIL 412/004: 3.0/2.0"
.S5: .string "FAIL 412/005: 3.0**3"
.S6: .string "FAIL 412/006: unary minus on real"
.S7: .string "PASS 412_arith_real (6/6)"
.text

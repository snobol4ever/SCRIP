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
# IR_LIT_S
bb1_α:
 mov qword ptr [r12 + 16], 1
 mov rax, qword ptr [rip + .Lx3_0]
 mov qword ptr [r12 + 24], rax
 jmp xgvarg2_done
 xgvarg2_β:
 jmp snoch0_n2_α
.Lx3_0:
 .quad .Lx3_0_s
.Lx3_0_s:
 .string "12"
xgvarg2_done:
# IR_LIT_S
bb2_α:
 mov qword ptr [r12 + 32], 1
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [r12 + 40], rax
 jmp xgvarg4_done
 xgvarg4_β:
 jmp snoch0_n2_α
.Lx5_0:
 .quad .Lx5_0_s
.Lx5_0_s:
 .string "integer"
xgvarg4_done:
bb3_α:
# BOX IR_CALL convert(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
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
  .Lbynamefn7: .string "convert"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn7]
 lea rsi, [r12 + 64]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 48], rax
 mov qword ptr [r12 + 56], rdx
 cmp eax, 99
 je snoch0_n2_α
 jmp xgvarg1_done
xgvarg1_β:
 jmp snoch0_n2_α
xgvarg1_done:
# IR_LIT_I
bb4_α:
 mov qword ptr [r12 + 96], 6
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 104], rax
 jmp xgvarg8_done
 xgvarg8_β:
 jmp snoch0_n2_α
.Lx9_0:
 .quad 12
xgvarg8_done:
bb5_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+48] -> [r12+128]
 mov rax, qword ptr [r12 + 48]
 mov qword ptr [r12 + 128], rax
 mov rax, qword ptr [r12 + 56]
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
 je snoch0_n2_α
 jmp snoch0_n1_α
snoch0_n0_β:
 jmp snoch0_n2_α
snoch0_n1_α:
# IR_LIT_scalar
bb6_α:
 jmp snoch0_n3_α
 snoch0_n1_β:
 jmp flat_γ
snoch0_n2_α:
# IR_LIT_F
bb7_α:
 mov qword ptr [r12 + 160], 7
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [r12 + 168], rax
 jmp xgvarg14_done
 xgvarg14_β:
 jmp snoch0_n5_α
.Lx15_0:
 .quad 4612811918334230528
xgvarg14_done:
# IR_LIT_S
bb8_α:
 mov qword ptr [r12 + 176], 1
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [r12 + 184], rax
 jmp xgvarg16_done
 xgvarg16_β:
 jmp snoch0_n5_α
.Lx17_0:
 .quad .Lx17_0_s
.Lx17_0_s:
 .string "integer"
xgvarg16_done:
bb9_α:
# BOX IR_CALL convert(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+160] -> [r12+208]
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 208], rax
 mov rax, qword ptr [r12 + 168]
 mov qword ptr [r12 + 216], rax
# marshal arg1 = producer-box slot [r12+176] -> [r12+224]
 mov rax, qword ptr [r12 + 176]
 mov qword ptr [r12 + 224], rax
 mov rax, qword ptr [r12 + 184]
 mov qword ptr [r12 + 232], rax
  .section .rodata
  .Lbynamefn19: .string "convert"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn19]
 lea rsi, [r12 + 208]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 cmp eax, 99
 je snoch0_n5_α
 jmp xgvarg13_done
xgvarg13_β:
 jmp snoch0_n5_α
xgvarg13_done:
# IR_LIT_I
bb10_α:
 mov qword ptr [r12 + 240], 6
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [r12 + 248], rax
 jmp xgvarg20_done
 xgvarg20_β:
 jmp snoch0_n5_α
.Lx21_0:
 .quad 2
xgvarg20_done:
bb11_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+192] -> [r12+272]
 mov rax, qword ptr [r12 + 192]
 mov qword ptr [r12 + 272], rax
 mov rax, qword ptr [r12 + 200]
 mov qword ptr [r12 + 280], rax
# marshal arg1 = producer-box slot [r12+240] -> [r12+288]
 mov rax, qword ptr [r12 + 240]
 mov qword ptr [r12 + 288], rax
 mov rax, qword ptr [r12 + 248]
 mov qword ptr [r12 + 296], rax
  .section .rodata
  .Lbynamefn23: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn23]
 lea rsi, [r12 + 272]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
 cmp eax, 99
 je snoch0_n5_α
 jmp snoch0_n4_α
snoch0_n2_β:
 jmp snoch0_n5_α
snoch0_n3_α:
bb12_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S1]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n3_β:
 jmp flat_γ
snoch0_n4_α:
# IR_LIT_scalar
bb13_α:
 jmp snoch0_n6_α
 snoch0_n4_β:
 jmp flat_γ
snoch0_n5_α:
# IR_LIT_I
bb14_α:
 mov qword ptr [r12 + 304], 6
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [r12 + 312], rax
 jmp xgvarg27_done
 xgvarg27_β:
 jmp snoch0_n8_α
.Lx28_0:
 .quad 2
xgvarg27_done:
# IR_LIT_S
bb15_α:
 mov qword ptr [r12 + 320], 1
 mov rax, qword ptr [rip + .Lx30_0]
 mov qword ptr [r12 + 328], rax
 jmp xgvarg29_done
 xgvarg29_β:
 jmp snoch0_n8_α
.Lx30_0:
 .quad .Lx30_0_s
.Lx30_0_s:
 .string "real"
xgvarg29_done:
bb16_α:
# BOX IR_CALL convert(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+304] -> [r12+352]
 mov rax, qword ptr [r12 + 304]
 mov qword ptr [r12 + 352], rax
 mov rax, qword ptr [r12 + 312]
 mov qword ptr [r12 + 360], rax
# marshal arg1 = producer-box slot [r12+320] -> [r12+368]
 mov rax, qword ptr [r12 + 320]
 mov qword ptr [r12 + 368], rax
 mov rax, qword ptr [r12 + 328]
 mov qword ptr [r12 + 376], rax
  .section .rodata
  .Lbynamefn32: .string "convert"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn32]
 lea rsi, [r12 + 352]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 cmp eax, 99
 je snoch0_n8_α
 jmp xgvarg26_done
xgvarg26_β:
 jmp snoch0_n8_α
xgvarg26_done:
# IR_LIT_F
bb17_α:
 mov qword ptr [r12 + 384], 7
 mov rax, qword ptr [rip + .Lx34_0]
 mov qword ptr [r12 + 392], rax
 jmp xgvarg33_done
 xgvarg33_β:
 jmp snoch0_n8_α
.Lx34_0:
 .quad 4611686018427387904
xgvarg33_done:
bb18_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+336] -> [r12+416]
 mov rax, qword ptr [r12 + 336]
 mov qword ptr [r12 + 416], rax
 mov rax, qword ptr [r12 + 344]
 mov qword ptr [r12 + 424], rax
# marshal arg1 = producer-box slot [r12+384] -> [r12+432]
 mov rax, qword ptr [r12 + 384]
 mov qword ptr [r12 + 432], rax
 mov rax, qword ptr [r12 + 392]
 mov qword ptr [r12 + 440], rax
  .section .rodata
  .Lbynamefn36: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn36]
 lea rsi, [r12 + 416]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 400], rax
 mov qword ptr [r12 + 408], rdx
 cmp eax, 99
 je snoch0_n8_α
 jmp snoch0_n7_α
snoch0_n5_β:
 jmp snoch0_n8_α
snoch0_n6_α:
bb19_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S2]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n6_β:
 jmp flat_γ
snoch0_n7_α:
# IR_LIT_scalar
bb20_α:
 jmp snoch0_n9_α
 snoch0_n7_β:
 jmp flat_γ
snoch0_n8_α:
# IR_LIT_S
bb21_α:
 mov qword ptr [r12 + 448], 1
 mov rax, qword ptr [rip + .Lx41_0]
 mov qword ptr [r12 + 456], rax
 jmp xgvarg40_done
 xgvarg40_β:
 jmp snoch0_n11_α
.Lx41_0:
 .quad .Lx41_0_s
.Lx41_0_s:
 .string ".2"
xgvarg40_done:
# IR_LIT_S
bb22_α:
 mov qword ptr [r12 + 464], 1
 mov rax, qword ptr [rip + .Lx43_0]
 mov qword ptr [r12 + 472], rax
 jmp xgvarg42_done
 xgvarg42_β:
 jmp snoch0_n11_α
.Lx43_0:
 .quad .Lx43_0_s
.Lx43_0_s:
 .string "real"
xgvarg42_done:
bb23_α:
# BOX IR_CALL convert(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+448] -> [r12+496]
 mov rax, qword ptr [r12 + 448]
 mov qword ptr [r12 + 496], rax
 mov rax, qword ptr [r12 + 456]
 mov qword ptr [r12 + 504], rax
# marshal arg1 = producer-box slot [r12+464] -> [r12+512]
 mov rax, qword ptr [r12 + 464]
 mov qword ptr [r12 + 512], rax
 mov rax, qword ptr [r12 + 472]
 mov qword ptr [r12 + 520], rax
  .section .rodata
  .Lbynamefn45: .string "convert"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn45]
 lea rsi, [r12 + 496]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 480], rax
 mov qword ptr [r12 + 488], rdx
 cmp eax, 99
 je snoch0_n11_α
 jmp xgvarg39_done
xgvarg39_β:
 jmp snoch0_n11_α
xgvarg39_done:
# IR_LIT_F
bb24_α:
 mov qword ptr [r12 + 528], 7
 mov rax, qword ptr [rip + .Lx47_0]
 mov qword ptr [r12 + 536], rax
 jmp xgvarg46_done
 xgvarg46_β:
 jmp snoch0_n11_α
.Lx47_0:
 .quad 4596373779694328218
xgvarg46_done:
bb25_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+480] -> [r12+560]
 mov rax, qword ptr [r12 + 480]
 mov qword ptr [r12 + 560], rax
 mov rax, qword ptr [r12 + 488]
 mov qword ptr [r12 + 568], rax
# marshal arg1 = producer-box slot [r12+528] -> [r12+576]
 mov rax, qword ptr [r12 + 528]
 mov qword ptr [r12 + 576], rax
 mov rax, qword ptr [r12 + 536]
 mov qword ptr [r12 + 584], rax
  .section .rodata
  .Lbynamefn49: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn49]
 lea rsi, [r12 + 560]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 544], rax
 mov qword ptr [r12 + 552], rdx
 cmp eax, 99
 je snoch0_n11_α
 jmp snoch0_n10_α
snoch0_n8_β:
 jmp snoch0_n11_α
snoch0_n9_α:
bb26_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S3]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n9_β:
 jmp flat_γ
snoch0_n10_α:
# IR_LIT_scalar
bb27_α:
 jmp snoch0_n12_α
 snoch0_n10_β:
 jmp flat_γ
snoch0_n11_α:
# IR_LIT_scalar
bb28_α:
 jmp snoch0_n13_α
 snoch0_n11_β:
 jmp flat_γ
snoch0_n12_α:
bb29_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S4]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n12_β:
 jmp flat_γ
snoch0_n13_α:
bb30_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S5]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n13_β:
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
.S1: .string "FAIL 910/001: string->integer"
.S2: .string "FAIL 910/002: real->integer truncation"
.S3: .string "FAIL 910/003: integer->real"
.S4: .string "FAIL 910/004: string->real"
.S5: .string "PASS 910_convert (4/4)"
.text

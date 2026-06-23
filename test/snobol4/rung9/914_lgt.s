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
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 24], rax
 jmp xgvarg1_done
 xgvarg1_β:
 jmp snoch0_n2_α
.Lx2_0:
 .quad .Lx2_0_s
.Lx2_0_s:
 .string "abc"
xgvarg1_done:
# IR_LIT_S
bb2_α:
 mov qword ptr [r12 + 32], 1
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [r12 + 40], rax
 jmp xgvarg3_done
 xgvarg3_β:
 jmp snoch0_n2_α
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "xyz"
xgvarg3_done:
bb3_α:
# BOX IR_CALL lgt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
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
  .Lbynamefn6: .string "lgt"
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
# IR_LIT_S
bb5_α:
 mov qword ptr [r12 + 96], 1
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 104], rax
 jmp xgvarg8_done
 xgvarg8_β:
 jmp snoch0_n5_α
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "abc"
xgvarg8_done:
# IR_LIT_S
bb6_α:
 mov qword ptr [r12 + 112], 1
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 120], rax
 jmp xgvarg10_done
 xgvarg10_β:
 jmp snoch0_n5_α
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string "abc"
xgvarg10_done:
bb7_α:
# BOX IR_CALL lgt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
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
  .Lbynamefn13: .string "lgt"
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
# IR_LIT_S
bb10_α:
 mov qword ptr [r12 + 176], 1
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [r12 + 184], rax
 jmp xgvarg16_done
 xgvarg16_β:
 jmp snoch0_n8_α
.Lx17_0:
 .quad .Lx17_0_s
.Lx17_0_s:
 .string "xyz"
xgvarg16_done:
# IR_LIT_S
bb11_α:
 mov qword ptr [r12 + 192], 1
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [r12 + 200], rax
 jmp xgvarg18_done
 xgvarg18_β:
 jmp snoch0_n8_α
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "abc"
xgvarg18_done:
bb12_α:
# BOX IR_CALL lgt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
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
  .Lbynamefn21: .string "lgt"
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
# IR_LIT_S
bb14_α:
 mov qword ptr [r12 + 256], 1
 mov rax, qword ptr [rip + .Lx24_0]
 mov qword ptr [r12 + 264], rax
 jmp xgvarg23_done
 xgvarg23_β:
 jmp snoch0_n10_α
.Lx24_0:
 .quad .Lx24_0_s
.Lx24_0_s:
 .string ""
xgvarg23_done:
# IR_LIT_S
bb15_α:
 mov qword ptr [r12 + 272], 1
 mov rax, qword ptr [rip + .Lx26_0]
 mov qword ptr [r12 + 280], rax
 jmp xgvarg25_done
 xgvarg25_β:
 jmp snoch0_n10_α
.Lx26_0:
 .quad .Lx26_0_s
.Lx26_0_s:
 .string "abc"
xgvarg25_done:
bb16_α:
# BOX IR_CALL lgt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
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
  .Lbynamefn28: .string "lgt"
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
# IR_LIT_S
bb19_α:
 mov qword ptr [r12 + 336], 1
 mov rax, qword ptr [rip + .Lx32_0]
 mov qword ptr [r12 + 344], rax
 jmp xgvarg31_done
 xgvarg31_β:
 jmp snoch0_n14_α
.Lx32_0:
 .quad .Lx32_0_s
.Lx32_0_s:
 .string "abc"
xgvarg31_done:
# IR_LIT_S
bb20_α:
 mov qword ptr [r12 + 352], 1
 mov rax, qword ptr [rip + .Lx34_0]
 mov qword ptr [r12 + 360], rax
 jmp xgvarg33_done
 xgvarg33_β:
 jmp snoch0_n14_α
.Lx34_0:
 .quad .Lx34_0_s
.Lx34_0_s:
 .string ""
xgvarg33_done:
bb21_α:
# BOX IR_CALL lgt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
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
  .Lbynamefn36: .string "lgt"
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
# IR_LIT_scalar
bb24_α:
 jmp snoch0_n15_α
 snoch0_n13_β:
 jmp flat_γ
snoch0_n14_α:
# IR_LIT_scalar
bb25_α:
 jmp snoch0_n16_α
 snoch0_n14_β:
 jmp flat_γ
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
bb27_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S6]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n16_β:
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
.S1: .string "FAIL 914/001: lgt(abc,xyz) should fail"
.S2: .string "FAIL 914/002: lgt(abc,abc) should fail"
.S3: .string "FAIL 914/003: lgt(xyz,abc) should succeed"
.S4: .string "FAIL 914/004: lgt(null,abc) should fail"
.S5: .string "PASS 914_lgt (5/5)"
.S6: .string "FAIL 914/005: lgt(abc,null) should succeed"
.text

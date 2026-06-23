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
 jmp flat_γ
.Lx2_0:
 .quad .Lx2_0_s
.Lx2_0_s:
 .string "b"
xgvarg1_done:
# IR_LIT_S
bb2_α:
 mov qword ptr [r12 + 32], 1
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [r12 + 40], rax
 jmp xgvarg3_done
 xgvarg3_β:
 jmp flat_γ
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "a"
xgvarg3_done:
bb3_α:
# BOX IR_CALL LGT(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
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
  .Lbynamefn6: .string "LGT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn6]
 lea rsi, [r12 + 64]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 48], rax
 mov qword ptr [r12 + 56], rdx
 cmp eax, 99
 je flat_γ
 jmp snoch0_n1_α
snoch0_n0_β:
 jmp flat_γ
snoch0_n1_α:
# IR_LIT_scalar
bb4_α:
 jmp snoch0_n2_α
 snoch0_n1_β:
 jmp snoch0_n3_α
snoch0_n2_α:
bb5_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S1]
 call rt_gvar_assign_str@PLT
 jmp snoch0_n3_α
 snoch0_n2_β:
 jmp snoch0_n3_α
snoch0_n3_α:
# IR_LIT_S
bb6_α:
 mov qword ptr [r12 + 96], 1
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [r12 + 104], rax
 jmp xgvarg9_done
 xgvarg9_β:
 jmp flat_γ
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "a"
xgvarg9_done:
# IR_LIT_S
bb7_α:
 mov qword ptr [r12 + 112], 1
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [r12 + 120], rax
 jmp xgvarg11_done
 xgvarg11_β:
 jmp flat_γ
.Lx12_0:
 .quad .Lx12_0_s
.Lx12_0_s:
 .string "b"
xgvarg11_done:
bb8_α:
# BOX IR_CALL LLT(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
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
  .Lbynamefn14: .string "LLT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn14]
 lea rsi, [r12 + 144]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
 cmp eax, 99
 je flat_γ
 jmp snoch0_n4_α
snoch0_n3_β:
 jmp flat_γ
snoch0_n4_α:
# IR_LIT_scalar
bb9_α:
 jmp snoch0_n5_α
 snoch0_n4_β:
 jmp snoch0_n6_α
snoch0_n5_α:
bb10_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S2]
 call rt_gvar_assign_str@PLT
 jmp snoch0_n6_α
 snoch0_n5_β:
 jmp snoch0_n6_α
snoch0_n6_α:
# IR_LIT_S
bb11_α:
 mov qword ptr [r12 + 176], 1
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [r12 + 184], rax
 jmp xgvarg17_done
 xgvarg17_β:
 jmp flat_γ
.Lx18_0:
 .quad .Lx18_0_s
.Lx18_0_s:
 .string "cat"
xgvarg17_done:
# IR_LIT_S
bb12_α:
 mov qword ptr [r12 + 192], 1
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [r12 + 200], rax
 jmp xgvarg19_done
 xgvarg19_β:
 jmp flat_γ
.Lx20_0:
 .quad .Lx20_0_s
.Lx20_0_s:
 .string "cat"
xgvarg19_done:
bb13_α:
# BOX IR_CALL LEQ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
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
  .Lbynamefn22: .string "LEQ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn22]
 lea rsi, [r12 + 224]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 cmp eax, 99
 je flat_γ
 jmp snoch0_n7_α
snoch0_n6_β:
 jmp flat_γ
snoch0_n7_α:
# IR_LIT_scalar
bb14_α:
 jmp snoch0_n8_α
 snoch0_n7_β:
 jmp snoch0_n9_α
snoch0_n8_α:
bb15_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S3]
 call rt_gvar_assign_str@PLT
 jmp snoch0_n9_α
 snoch0_n8_β:
 jmp snoch0_n9_α
snoch0_n9_α:
# IR_LIT_S
bb16_α:
 mov qword ptr [r12 + 256], 1
 mov rax, qword ptr [rip + .Lx26_0]
 mov qword ptr [r12 + 264], rax
 jmp xgvarg25_done
 xgvarg25_β:
 jmp flat_γ
.Lx26_0:
 .quad .Lx26_0_s
.Lx26_0_s:
 .string "cat"
xgvarg25_done:
# IR_LIT_S
bb17_α:
 mov qword ptr [r12 + 272], 1
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [r12 + 280], rax
 jmp xgvarg27_done
 xgvarg27_β:
 jmp flat_γ
.Lx28_0:
 .quad .Lx28_0_s
.Lx28_0_s:
 .string "dog"
xgvarg27_done:
bb18_α:
# BOX IR_CALL LNE(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
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
  .Lbynamefn30: .string "LNE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn30]
 lea rsi, [r12 + 304]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 288], rax
 mov qword ptr [r12 + 296], rdx
 cmp eax, 99
 je flat_γ
 jmp snoch0_n10_α
snoch0_n9_β:
 jmp flat_γ
snoch0_n10_α:
# IR_LIT_scalar
bb19_α:
 jmp snoch0_n11_α
 snoch0_n10_β:
 jmp flat_γ
snoch0_n11_α:
bb20_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S4]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n11_β:
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
.S0: .string "OUTPUT"
.S1: .string "b > a"
.S2: .string "a < b"
.S3: .string "cat = cat"
.S4: .string "cat != dog"
.text

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
 .string "hello"
xgvarg2_done:
bb2_α:
# BOX IR_CALL datatype(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+16] -> [r12+48]
 mov rax, qword ptr [r12 + 16]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 24]
 mov qword ptr [r12 + 56], rax
  .section .rodata
  .Lbynamefn5: .string "datatype"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn5]
 lea rsi, [r12 + 48]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je snoch0_n2_α
 jmp xgvarg1_done
xgvarg1_β:
 jmp snoch0_n2_α
xgvarg1_done:
# IR_LIT_S
bb3_α:
 mov qword ptr [r12 + 64], 1
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [r12 + 72], rax
 jmp xgvarg6_done
 xgvarg6_β:
 jmp snoch0_n2_α
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string "string"
xgvarg6_done:
bb4_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+32] -> [r12+96]
 mov rax, qword ptr [r12 + 32]
 mov qword ptr [r12 + 96], rax
 mov rax, qword ptr [r12 + 40]
 mov qword ptr [r12 + 104], rax
# marshal arg1 = producer-box slot [r12+64] -> [r12+112]
 mov rax, qword ptr [r12 + 64]
 mov qword ptr [r12 + 112], rax
 mov rax, qword ptr [r12 + 72]
 mov qword ptr [r12 + 120], rax
  .section .rodata
  .Lbynamefn9: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn9]
 lea rsi, [r12 + 96]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 cmp eax, 99
 je snoch0_n2_α
 jmp snoch0_n1_α
snoch0_n0_β:
 jmp snoch0_n2_α
snoch0_n1_α:
# IR_LIT_scalar
bb5_α:
 jmp snoch0_n3_α
 snoch0_n1_β:
 jmp flat_γ
snoch0_n2_α:
# IR_LIT_I
bb6_α:
 mov qword ptr [r12 + 128], 6
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [r12 + 136], rax
 jmp xgvarg12_done
 xgvarg12_β:
 jmp snoch0_n5_α
.Lx13_0:
 .quad 12
xgvarg12_done:
bb7_α:
# BOX IR_CALL datatype(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+128] -> [r12+160]
 mov rax, qword ptr [r12 + 128]
 mov qword ptr [r12 + 160], rax
 mov rax, qword ptr [r12 + 136]
 mov qword ptr [r12 + 168], rax
  .section .rodata
  .Lbynamefn15: .string "datatype"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn15]
 lea rsi, [r12 + 160]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 cmp eax, 99
 je snoch0_n5_α
 jmp xgvarg11_done
xgvarg11_β:
 jmp snoch0_n5_α
xgvarg11_done:
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
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+144] -> [r12+208]
 mov rax, qword ptr [r12 + 144]
 mov qword ptr [r12 + 208], rax
 mov rax, qword ptr [r12 + 152]
 mov qword ptr [r12 + 216], rax
# marshal arg1 = producer-box slot [r12+176] -> [r12+224]
 mov rax, qword ptr [r12 + 176]
 mov qword ptr [r12 + 224], rax
 mov rax, qword ptr [r12 + 184]
 mov qword ptr [r12 + 232], rax
  .section .rodata
  .Lbynamefn19: .string "differ"
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
 jmp snoch0_n4_α
snoch0_n2_β:
 jmp snoch0_n5_α
snoch0_n3_α:
bb10_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S1]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n3_β:
 jmp flat_γ
snoch0_n4_α:
# IR_LIT_scalar
bb11_α:
 jmp snoch0_n6_α
 snoch0_n4_β:
 jmp flat_γ
snoch0_n5_α:
# IR_LIT_F
bb12_α:
 mov qword ptr [r12 + 240], 7
 mov rax, qword ptr [rip + .Lx24_0]
 mov qword ptr [r12 + 248], rax
 jmp xgvarg23_done
 xgvarg23_β:
 jmp snoch0_n8_α
.Lx24_0:
 .quad 4608668606677049672
xgvarg23_done:
bb13_α:
# BOX IR_CALL datatype(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+240] -> [r12+272]
 mov rax, qword ptr [r12 + 240]
 mov qword ptr [r12 + 272], rax
 mov rax, qword ptr [r12 + 248]
 mov qword ptr [r12 + 280], rax
  .section .rodata
  .Lbynamefn26: .string "datatype"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn26]
 lea rsi, [r12 + 272]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
 cmp eax, 99
 je snoch0_n8_α
 jmp xgvarg22_done
xgvarg22_β:
 jmp snoch0_n8_α
xgvarg22_done:
# IR_LIT_S
bb14_α:
 mov qword ptr [r12 + 288], 1
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [r12 + 296], rax
 jmp xgvarg27_done
 xgvarg27_β:
 jmp snoch0_n8_α
.Lx28_0:
 .quad .Lx28_0_s
.Lx28_0_s:
 .string "real"
xgvarg27_done:
bb15_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+256] -> [r12+320]
 mov rax, qword ptr [r12 + 256]
 mov qword ptr [r12 + 320], rax
 mov rax, qword ptr [r12 + 264]
 mov qword ptr [r12 + 328], rax
# marshal arg1 = producer-box slot [r12+288] -> [r12+336]
 mov rax, qword ptr [r12 + 288]
 mov qword ptr [r12 + 336], rax
 mov rax, qword ptr [r12 + 296]
 mov qword ptr [r12 + 344], rax
  .section .rodata
  .Lbynamefn30: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn30]
 lea rsi, [r12 + 320]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 304], rax
 mov qword ptr [r12 + 312], rdx
 cmp eax, 99
 je snoch0_n8_α
 jmp snoch0_n7_α
snoch0_n5_β:
 jmp snoch0_n8_α
snoch0_n6_α:
bb16_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S2]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n6_β:
 jmp flat_γ
snoch0_n7_α:
# IR_LIT_scalar
bb17_α:
 jmp snoch0_n9_α
 snoch0_n7_β:
 jmp flat_γ
snoch0_n8_α:
# IR_LIT_S
bb18_α:
 mov qword ptr [r12 + 352], 1
 mov rax, qword ptr [rip + .Lx35_0]
 mov qword ptr [r12 + 360], rax
 jmp xgvarg34_done
 xgvarg34_β:
 jmp snoch0_n11_α
.Lx35_0:
 .quad .Lx35_0_s
.Lx35_0_s:
 .string ""
xgvarg34_done:
bb19_α:
# BOX IR_CALL datatype(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+352] -> [r12+384]
 mov rax, qword ptr [r12 + 352]
 mov qword ptr [r12 + 384], rax
 mov rax, qword ptr [r12 + 360]
 mov qword ptr [r12 + 392], rax
  .section .rodata
  .Lbynamefn37: .string "datatype"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn37]
 lea rsi, [r12 + 384]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 368], rax
 mov qword ptr [r12 + 376], rdx
 cmp eax, 99
 je snoch0_n11_α
 jmp xgvarg33_done
xgvarg33_β:
 jmp snoch0_n11_α
xgvarg33_done:
# IR_LIT_S
bb20_α:
 mov qword ptr [r12 + 400], 1
 mov rax, qword ptr [rip + .Lx39_0]
 mov qword ptr [r12 + 408], rax
 jmp xgvarg38_done
 xgvarg38_β:
 jmp snoch0_n11_α
.Lx39_0:
 .quad .Lx39_0_s
.Lx39_0_s:
 .string "string"
xgvarg38_done:
bb21_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+368] -> [r12+432]
 mov rax, qword ptr [r12 + 368]
 mov qword ptr [r12 + 432], rax
 mov rax, qword ptr [r12 + 376]
 mov qword ptr [r12 + 440], rax
# marshal arg1 = producer-box slot [r12+400] -> [r12+448]
 mov rax, qword ptr [r12 + 400]
 mov qword ptr [r12 + 448], rax
 mov rax, qword ptr [r12 + 408]
 mov qword ptr [r12 + 456], rax
  .section .rodata
  .Lbynamefn41: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn41]
 lea rsi, [r12 + 432]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 cmp eax, 99
 je snoch0_n11_α
 jmp snoch0_n10_α
snoch0_n8_β:
 jmp snoch0_n11_α
snoch0_n9_α:
bb22_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S3]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n9_β:
 jmp flat_γ
snoch0_n10_α:
# IR_LIT_scalar
bb23_α:
 jmp snoch0_n12_α
 snoch0_n10_β:
 jmp flat_γ
snoch0_n11_α:
# IR_LIT_scalar
bb24_α:
 jmp snoch0_n13_α
 snoch0_n11_β:
 jmp flat_γ
snoch0_n12_α:
bb25_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S4]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n12_β:
 jmp flat_γ
snoch0_n13_α:
bb26_α:
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
.S1: .string "FAIL 911/001: string literal datatype"
.S2: .string "FAIL 911/002: integer datatype"
.S3: .string "FAIL 911/003: real datatype"
.S4: .string "FAIL 911/004: null is string"
.S5: .string "PASS 911_datatype (4/4)"
.text

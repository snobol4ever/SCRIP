  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "fact"
  .Lgvan1: .string "n"
  .Lgvan2: .string "output"
  .Lgvan3: .string "fact2"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .quad .Lgvan2
  .quad .Lgvan3
  .section .bss
  .align 16
__gva: .space 64, 0
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
  mov edx, 4
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
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 24], rax
 jmp xgvarg1_done
 xgvarg1_β:
 jmp snoch0_n1_α
.Lx2_0:
 .quad .Lx2_0_s
.Lx2_0_s:
 .string "fact(n)"
xgvarg1_done:
bb2_α:
# BOX IR_CALL define(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+16] -> [r12+48]
 mov rax, qword ptr [r12 + 16]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 24]
 mov qword ptr [r12 + 56], rax
  .section .rodata
  .Lbynamefn4: .string "define"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn4]
 lea rsi, [r12 + 48]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je snoch0_n1_α
 jmp snoch0_n1_α
snoch0_n0_β:
 jmp snoch0_n1_α
snoch0_n1_α:
# IR_LIT_I
bb3_α:
 mov qword ptr [r12 + 64], 6
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [r12 + 72], rax
 jmp xgvarg6_done
 xgvarg6_β:
 jmp snoch0_n3_α
.Lx7_0:
 .quad 5
xgvarg6_done:
bb4_α:
# BOX IR_CALL fact(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+64] -> [r12+96]
 mov rax, qword ptr [r12 + 64]
 mov qword ptr [r12 + 96], rax
 mov rax, qword ptr [r12 + 72]
 mov qword ptr [r12 + 104], rax
  .section .rodata
  .Lbynamefn9: .string "fact"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn9]
 lea rsi, [r12 + 96]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 cmp eax, 99
 je snoch0_n3_α
 jmp xgvarg5_done
xgvarg5_β:
 jmp snoch0_n3_α
xgvarg5_done:
# IR_LIT_I
bb5_α:
 mov qword ptr [r12 + 112], 6
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 120], rax
 jmp xgvarg10_done
 xgvarg10_β:
 jmp snoch0_n3_α
.Lx11_0:
 .quad 120
xgvarg10_done:
bb6_α:
# BOX IR_CALL ne(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+80] -> [r12+144]
 mov rax, qword ptr [r12 + 80]
 mov qword ptr [r12 + 144], rax
 mov rax, qword ptr [r12 + 88]
 mov qword ptr [r12 + 152], rax
# marshal arg1 = producer-box slot [r12+112] -> [r12+160]
 mov rax, qword ptr [r12 + 112]
 mov qword ptr [r12 + 160], rax
 mov rax, qword ptr [r12 + 120]
 mov qword ptr [r12 + 168], rax
  .section .rodata
  .Lbynamefn13: .string "ne"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn13]
 lea rsi, [r12 + 144]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
 cmp eax, 99
 je snoch0_n3_α
 jmp snoch0_n2_α
snoch0_n1_β:
 jmp snoch0_n3_α
snoch0_n2_α:
# IR_LIT_scalar
bb7_α:
 jmp snoch0_n4_α
 snoch0_n2_β:
 jmp flat_γ
snoch0_n3_α:
# IR_LIT_S
bb8_α:
 mov qword ptr [r12 + 176], 1
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [r12 + 184], rax
 jmp xgvarg16_done
 xgvarg16_β:
 jmp snoch0_n6_α
.Lx17_0:
 .quad .Lx17_0_s
.Lx17_0_s:
 .string "facto"
xgvarg16_done:
# IR_LIT_S
bb9_α:
 mov qword ptr [r12 + 192], 1
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [r12 + 200], rax
 jmp xgvarg18_done
 xgvarg18_β:
 jmp snoch0_n6_α
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "fact"
xgvarg18_done:
bb10_α:
# BOX IR_CALL opsyn(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
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
  .Lbynamefn21: .string "opsyn"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn21]
 lea rsi, [r12 + 224]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 cmp eax, 99
 je snoch0_n6_α
 jmp xgvarg15_done
xgvarg15_β:
 jmp snoch0_n6_α
xgvarg15_done:
bb11_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+208] -> [r12+272]
 mov rax, qword ptr [r12 + 208]
 mov qword ptr [r12 + 272], rax
 mov rax, qword ptr [r12 + 216]
 mov qword ptr [r12 + 280], rax
  .section .rodata
  .Lbynamefn23: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn23]
 lea rsi, [r12 + 272]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
 cmp eax, 99
 je snoch0_n6_α
 jmp snoch0_n5_α
snoch0_n3_β:
 jmp snoch0_n6_α
snoch0_n4_α:
bb12_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S1]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n4_β:
 jmp flat_γ
snoch0_n5_α:
# IR_LIT_scalar
bb13_α:
 jmp snoch0_n7_α
 snoch0_n5_β:
 jmp flat_γ
snoch0_n6_α:
# IR_LIT_I
bb14_α:
 mov qword ptr [r12 + 288], 6
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [r12 + 296], rax
 jmp xgvarg27_done
 xgvarg27_β:
 jmp snoch0_n9_α
.Lx28_0:
 .quad 4
xgvarg27_done:
bb15_α:
# BOX IR_CALL facto(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+288] -> [r12+320]
 mov rax, qword ptr [r12 + 288]
 mov qword ptr [r12 + 320], rax
 mov rax, qword ptr [r12 + 296]
 mov qword ptr [r12 + 328], rax
  .section .rodata
  .Lbynamefn30: .string "facto"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn30]
 lea rsi, [r12 + 320]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 304], rax
 mov qword ptr [r12 + 312], rdx
 cmp eax, 99
 je snoch0_n9_α
 jmp xgvarg26_done
xgvarg26_β:
 jmp snoch0_n9_α
xgvarg26_done:
# IR_LIT_I
bb16_α:
 mov qword ptr [r12 + 336], 6
 mov rax, qword ptr [rip + .Lx32_0]
 mov qword ptr [r12 + 344], rax
 jmp xgvarg31_done
 xgvarg31_β:
 jmp snoch0_n9_α
.Lx32_0:
 .quad 24
xgvarg31_done:
bb17_α:
# BOX IR_CALL ne(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+304] -> [r12+368]
 mov rax, qword ptr [r12 + 304]
 mov qword ptr [r12 + 368], rax
 mov rax, qword ptr [r12 + 312]
 mov qword ptr [r12 + 376], rax
# marshal arg1 = producer-box slot [r12+336] -> [r12+384]
 mov rax, qword ptr [r12 + 336]
 mov qword ptr [r12 + 384], rax
 mov rax, qword ptr [r12 + 344]
 mov qword ptr [r12 + 392], rax
  .section .rodata
  .Lbynamefn34: .string "ne"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn34]
 lea rsi, [r12 + 368]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 352], rax
 mov qword ptr [r12 + 360], rdx
 cmp eax, 99
 je snoch0_n9_α
 jmp snoch0_n8_α
snoch0_n6_β:
 jmp snoch0_n9_α
snoch0_n7_α:
bb18_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S2]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n7_β:
 jmp flat_γ
snoch0_n8_α:
# IR_LIT_scalar
bb19_α:
 jmp snoch0_n10_α
 snoch0_n8_β:
 jmp flat_γ
snoch0_n9_α:
# IR_LIT_S
bb20_α:
 mov qword ptr [r12 + 400], 1
 mov rax, qword ptr [rip + .Lx38_0]
 mov qword ptr [r12 + 408], rax
 jmp xgvarg37_done
 xgvarg37_β:
 jmp snoch0_n11_α
.Lx38_0:
 .quad .Lx38_0_s
.Lx38_0_s:
 .string "fact2(n)"
xgvarg37_done:
# IR_LIT_S
bb21_α:
 mov qword ptr [r12 + 416], 1
 mov rax, qword ptr [rip + .Lx40_0]
 mov qword ptr [r12 + 424], rax
 jmp xgvarg39_done
 xgvarg39_β:
 jmp snoch0_n11_α
.Lx40_0:
 .quad .Lx40_0_s
.Lx40_0_s:
 .string "fact2_entry"
xgvarg39_done:
bb22_α:
# BOX IR_CALL define(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+400] -> [r12+448]
 mov rax, qword ptr [r12 + 400]
 mov qword ptr [r12 + 448], rax
 mov rax, qword ptr [r12 + 408]
 mov qword ptr [r12 + 456], rax
# marshal arg1 = producer-box slot [r12+416] -> [r12+464]
 mov rax, qword ptr [r12 + 416]
 mov qword ptr [r12 + 464], rax
 mov rax, qword ptr [r12 + 424]
 mov qword ptr [r12 + 472], rax
  .section .rodata
  .Lbynamefn42: .string "define"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn42]
 lea rsi, [r12 + 448]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 432], rax
 mov qword ptr [r12 + 440], rdx
 cmp eax, 99
 je snoch0_n11_α
 jmp snoch0_n11_α
snoch0_n9_β:
 jmp snoch0_n11_α
snoch0_n10_α:
bb23_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S3]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n10_β:
 jmp flat_γ
snoch0_n11_α:
# IR_LIT_I
bb24_α:
 mov qword ptr [r12 + 480], 6
 mov rax, qword ptr [rip + .Lx46_0]
 mov qword ptr [r12 + 488], rax
 jmp xgvarg45_done
 xgvarg45_β:
 jmp snoch0_n13_α
.Lx46_0:
 .quad 6
xgvarg45_done:
bb25_α:
# BOX IR_CALL fact2(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+480] -> [r12+512]
 mov rax, qword ptr [r12 + 480]
 mov qword ptr [r12 + 512], rax
 mov rax, qword ptr [r12 + 488]
 mov qword ptr [r12 + 520], rax
  .section .rodata
  .Lbynamefn48: .string "fact2"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn48]
 lea rsi, [r12 + 512]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 496], rax
 mov qword ptr [r12 + 504], rdx
 cmp eax, 99
 je snoch0_n13_α
 jmp xgvarg44_done
xgvarg44_β:
 jmp snoch0_n13_α
xgvarg44_done:
# IR_LIT_I
bb26_α:
 mov qword ptr [r12 + 528], 6
 mov rax, qword ptr [rip + .Lx50_0]
 mov qword ptr [r12 + 536], rax
 jmp xgvarg49_done
 xgvarg49_β:
 jmp snoch0_n13_α
.Lx50_0:
 .quad 720
xgvarg49_done:
bb27_α:
# BOX IR_CALL ne(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+496] -> [r12+560]
 mov rax, qword ptr [r12 + 496]
 mov qword ptr [r12 + 560], rax
 mov rax, qword ptr [r12 + 504]
 mov qword ptr [r12 + 568], rax
# marshal arg1 = producer-box slot [r12+528] -> [r12+576]
 mov rax, qword ptr [r12 + 528]
 mov qword ptr [r12 + 576], rax
 mov rax, qword ptr [r12 + 536]
 mov qword ptr [r12 + 584], rax
  .section .rodata
  .Lbynamefn52: .string "ne"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn52]
 lea rsi, [r12 + 560]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 544], rax
 mov qword ptr [r12 + 552], rdx
 cmp eax, 99
 je snoch0_n13_α
 jmp snoch0_n12_α
snoch0_n11_β:
 jmp snoch0_n13_α
snoch0_n12_α:
# IR_LIT_scalar
bb28_α:
 jmp snoch0_n14_α
 snoch0_n12_β:
 jmp flat_γ
snoch0_n13_α:
# IR_LIT_scalar
bb29_α:
 jmp snoch0_n15_α
 snoch0_n13_β:
 jmp flat_γ
snoch0_n14_α:
bb30_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S4]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n14_β:
 jmp flat_γ
snoch0_n15_α:
bb31_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S5]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n15_β:
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
.S1: .string "FAIL 1010/001: fact(5)=120"
.S2: .string "FAIL 1010/002: opsyn alias"
.S3: .string "FAIL 1010/003: facto(4)=24 via alias"
.S4: .string "FAIL 1010/004: fact2(6)=720 alt entry"
.S5: .string "PASS 1010_func_recursion (4/4)"
.text

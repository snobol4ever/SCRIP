  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "output"
  .Lgvan1: .string "a"
  .Lgvan2: .string "b"
  .Lgvan3: .string "d"
  .Lgvan4: .string "lfunc"
  .Lgvan5: .string "checklocal"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .quad .Lgvan2
  .quad .Lgvan3
  .quad .Lgvan4
  .quad .Lgvan5
  .section .bss
  .align 16
__gva: .space 96, 0
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
  mov edx, 6
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
 .string "lfunc(a,b,c)d,e,f"
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
# IR_LIT_scalar
bb3_α:
 jmp snoch0_n2_α
 snoch0_n1_β:
 jmp snoch0_n3_α
snoch0_n2_α:
bb4_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S1]
 call rt_gvar_assign_str@PLT
 jmp snoch0_n3_α
 snoch0_n2_β:
 jmp snoch0_n3_α
snoch0_n3_α:
# IR_LIT_scalar
bb5_α:
 jmp snoch0_n4_α
 snoch0_n3_β:
 jmp snoch0_n5_α
snoch0_n4_α:
bb6_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S2]
 lea rsi, [rip + .S3]
 call rt_gvar_assign_str@PLT
 jmp snoch0_n5_α
 snoch0_n4_β:
 jmp snoch0_n5_α
snoch0_n5_α:
# IR_LIT_S
bb7_α:
 mov qword ptr [r12 + 64], 1
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 72], rax
 jmp xgvarg10_done
 xgvarg10_β:
 jmp snoch0_n7_α
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string "p"
xgvarg10_done:
# IR_LIT_S
bb8_α:
 mov qword ptr [r12 + 80], 1
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [r12 + 88], rax
 jmp xgvarg12_done
 xgvarg12_β:
 jmp snoch0_n7_α
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "q"
xgvarg12_done:
# IR_LIT_S
bb9_α:
 mov qword ptr [r12 + 96], 1
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [r12 + 104], rax
 jmp xgvarg14_done
 xgvarg14_β:
 jmp snoch0_n7_α
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "r"
xgvarg14_done:
bb10_α:
# BOX IR_CALL lfunc(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+64] -> [r12+128]
 mov rax, qword ptr [r12 + 64]
 mov qword ptr [r12 + 128], rax
 mov rax, qword ptr [r12 + 72]
 mov qword ptr [r12 + 136], rax
# marshal arg1 = producer-box slot [r12+80] -> [r12+144]
 mov rax, qword ptr [r12 + 80]
 mov qword ptr [r12 + 144], rax
 mov rax, qword ptr [r12 + 88]
 mov qword ptr [r12 + 152], rax
# marshal arg2 = producer-box slot [r12+96] -> [r12+160]
 mov rax, qword ptr [r12 + 96]
 mov qword ptr [r12 + 160], rax
 mov rax, qword ptr [r12 + 104]
 mov qword ptr [r12 + 168], rax
  .section .rodata
  .Lbynamefn17: .string "lfunc"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn17]
 lea rsi, [r12 + 128]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 cmp eax, 99
 je snoch0_n7_α
 jmp xgvarg9_done
xgvarg9_β:
 jmp snoch0_n7_α
xgvarg9_done:
# IR_LIT_S
bb11_α:
 mov qword ptr [r12 + 176], 1
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [r12 + 184], rax
 jmp xgvarg18_done
 xgvarg18_β:
 jmp snoch0_n7_α
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "aabbdd"
xgvarg18_done:
bb12_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+112] -> [r12+208]
 mov rax, qword ptr [r12 + 112]
 mov qword ptr [r12 + 208], rax
 mov rax, qword ptr [r12 + 120]
 mov qword ptr [r12 + 216], rax
# marshal arg1 = producer-box slot [r12+176] -> [r12+224]
 mov rax, qword ptr [r12 + 176]
 mov qword ptr [r12 + 224], rax
 mov rax, qword ptr [r12 + 184]
 mov qword ptr [r12 + 232], rax
  .section .rodata
  .Lbynamefn21: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn21]
 lea rsi, [r12 + 208]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 cmp eax, 99
 je snoch0_n7_α
 jmp snoch0_n6_α
snoch0_n5_β:
 jmp snoch0_n7_α
snoch0_n6_α:
# IR_LIT_scalar
bb13_α:
 jmp snoch0_n8_α
 snoch0_n6_β:
 jmp flat_γ
snoch0_n7_α:
# IR_VAR gva
bb14_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 jmp xgvarg23_done
 xgvarg23_β:
 jmp snoch0_n10_α
xgvarg23_done:
# IR_LIT_S
bb15_α:
 mov qword ptr [r12 + 256], 1
 mov rax, qword ptr [rip + .Lx27_0]
 mov qword ptr [r12 + 264], rax
 jmp xgvarg26_done
 xgvarg26_β:
 jmp snoch0_n10_α
.Lx27_0:
 .quad .Lx27_0_s
.Lx27_0_s:
 .string "global_a"
xgvarg26_done:
bb16_α:
# BOX IR_CALL ident(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+240] -> [r12+288]
 mov rax, qword ptr [r12 + 240]
 mov qword ptr [r12 + 288], rax
 mov rax, qword ptr [r12 + 248]
 mov qword ptr [r12 + 296], rax
# marshal arg1 = producer-box slot [r12+256] -> [r12+304]
 mov rax, qword ptr [r12 + 256]
 mov qword ptr [r12 + 304], rax
 mov rax, qword ptr [r12 + 264]
 mov qword ptr [r12 + 312], rax
  .section .rodata
  .Lbynamefn29: .string "ident"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn29]
 lea rsi, [r12 + 288]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 cmp eax, 99
 je snoch0_n10_α
 jmp snoch0_n9_α
snoch0_n7_β:
 jmp snoch0_n10_α
snoch0_n8_α:
bb17_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S4]
 lea rsi, [rip + .S5]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n8_β:
 jmp flat_γ
snoch0_n9_α:
# IR_VAR gva
bb18_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 jmp xgvarg31_done
 xgvarg31_β:
 jmp snoch0_n12_α
xgvarg31_done:
# IR_LIT_S
bb19_α:
 mov qword ptr [r12 + 336], 1
 mov rax, qword ptr [rip + .Lx35_0]
 mov qword ptr [r12 + 344], rax
 jmp xgvarg34_done
 xgvarg34_β:
 jmp snoch0_n12_α
.Lx35_0:
 .quad .Lx35_0_s
.Lx35_0_s:
 .string "global_d"
xgvarg34_done:
bb20_α:
# BOX IR_CALL ident(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+320] -> [r12+368]
 mov rax, qword ptr [r12 + 320]
 mov qword ptr [r12 + 368], rax
 mov rax, qword ptr [r12 + 328]
 mov qword ptr [r12 + 376], rax
# marshal arg1 = producer-box slot [r12+336] -> [r12+384]
 mov rax, qword ptr [r12 + 336]
 mov qword ptr [r12 + 384], rax
 mov rax, qword ptr [r12 + 344]
 mov qword ptr [r12 + 392], rax
  .section .rodata
  .Lbynamefn37: .string "ident"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn37]
 lea rsi, [r12 + 368]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 352], rax
 mov qword ptr [r12 + 360], rdx
 cmp eax, 99
 je snoch0_n12_α
 jmp snoch0_n11_α
snoch0_n9_β:
 jmp snoch0_n12_α
snoch0_n10_α:
# IR_LIT_scalar
bb21_α:
 jmp snoch0_n13_α
 snoch0_n10_β:
 jmp flat_γ
snoch0_n11_α:
# IR_LIT_S
bb22_α:
 mov qword ptr [r12 + 400], 1
 mov rax, qword ptr [rip + .Lx40_0]
 mov qword ptr [r12 + 408], rax
 jmp xgvarg39_done
 xgvarg39_β:
 jmp snoch0_n14_α
.Lx40_0:
 .quad .Lx40_0_s
.Lx40_0_s:
 .string "checklocal()x"
xgvarg39_done:
bb23_α:
# BOX IR_CALL define(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+400] -> [r12+432]
 mov rax, qword ptr [r12 + 400]
 mov qword ptr [r12 + 432], rax
 mov rax, qword ptr [r12 + 408]
 mov qword ptr [r12 + 440], rax
  .section .rodata
  .Lbynamefn42: .string "define"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn42]
 lea rsi, [r12 + 432]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 cmp eax, 99
 je snoch0_n14_α
 jmp snoch0_n14_α
snoch0_n11_β:
 jmp snoch0_n14_α
snoch0_n12_α:
# IR_LIT_scalar
bb24_α:
 jmp snoch0_n15_α
 snoch0_n12_β:
 jmp flat_γ
snoch0_n13_α:
bb25_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S4]
 lea rsi, [rip + .S6]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n13_β:
 jmp flat_γ
snoch0_n14_α:
bb26_α:
# BOX IR_CALL checklocal(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn47: .string "checklocal"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn47]
 lea rsi, [r12 + 448]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 448], rax
 mov qword ptr [r12 + 456], rdx
 cmp eax, 99
 je snoch0_n17_α
 jmp xgvarg45_done
xgvarg45_β:
 jmp snoch0_n17_α
xgvarg45_done:
bb27_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+448] -> [r12+480]
 mov rax, qword ptr [r12 + 448]
 mov qword ptr [r12 + 480], rax
 mov rax, qword ptr [r12 + 456]
 mov qword ptr [r12 + 488], rax
  .section .rodata
  .Lbynamefn49: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn49]
 lea rsi, [r12 + 480]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 464], rax
 mov qword ptr [r12 + 472], rdx
 cmp eax, 99
 je snoch0_n17_α
 jmp snoch0_n16_α
snoch0_n14_β:
 jmp snoch0_n17_α
snoch0_n15_α:
bb28_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S4]
 lea rsi, [rip + .S7]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n15_β:
 jmp flat_γ
snoch0_n16_α:
# IR_LIT_scalar
bb29_α:
 jmp snoch0_n18_α
 snoch0_n16_β:
 jmp flat_γ
snoch0_n17_α:
# IR_LIT_scalar
bb30_α:
 jmp snoch0_n19_α
 snoch0_n17_β:
 jmp flat_γ
snoch0_n18_α:
bb31_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S4]
 lea rsi, [rip + .S8]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n18_β:
 jmp flat_γ
snoch0_n19_α:
bb32_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S4]
 lea rsi, [rip + .S9]
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
.S0: .string "a"
.S1: .string "global_a"
.S2: .string "d"
.S3: .string "global_d"
.S4: .string "output"
.S5: .string "FAIL 1012/004: lfunc return value"
.S6: .string "FAIL 1012/005: global a not clobbered"
.S7: .string "FAIL 1012/006: global d not clobbered"
.S8: .string "FAIL 1012/007: local null on fresh call"
.S9: .string "PASS 1012_func_locals (7/7)"
.text

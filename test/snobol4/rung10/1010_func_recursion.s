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
xargsub17_n0_α:
# IR_VAR
bb8_α:
 mov rdi, qword ptr [rip + .Lx19_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 jmp xargsub17_n1_α
 xargsub17_n0_β:
 jmp snoch0_n6_α
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "facto"
xargsub17_n1_α:
# IR_VAR
bb9_α:
 mov rdi, qword ptr [rip + .Lx22_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 jmp xunop20_arg_done
 xunop20_arg_β:
 jmp snoch0_n6_α
.Lx22_0:
 .quad .Lx22_0_s
.Lx22_0_s:
 .string "facto"
xunop20_arg_done:
jmp xgvarg16_done
xargsub17_n1_β:
jmp snoch0_n6_α
xgvarg16_done:
# IR_LIT_S
bb10_α:
 mov qword ptr [r12 + 208], 1
 mov rax, qword ptr [rip + .Lx24_0]
 mov qword ptr [r12 + 216], rax
 jmp xgvarg23_done
 xgvarg23_β:
 jmp snoch0_n6_α
.Lx24_0:
 .quad .Lx24_0_s
.Lx24_0_s:
 .string "fact"
xgvarg23_done:
bb11_α:
# BOX IR_CALL opsyn(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = gvar NV_GET -> [r12+240]
   lea rdi, [rip + .S0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
# marshal arg1 = producer-box slot [r12+208] -> [r12+256]
 mov rax, qword ptr [r12 + 208]
 mov qword ptr [r12 + 256], rax
 mov rax, qword ptr [r12 + 216]
 mov qword ptr [r12 + 264], rax
  .section .rodata
  .Lbynamefn26: .string "opsyn"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn26]
 lea rsi, [r12 + 240]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 cmp eax, 99
 je snoch0_n6_α
 jmp xgvarg15_done
xgvarg15_β:
 jmp snoch0_n6_α
xgvarg15_done:
bb12_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+224] -> [r12+288]
 mov rax, qword ptr [r12 + 224]
 mov qword ptr [r12 + 288], rax
 mov rax, qword ptr [r12 + 232]
 mov qword ptr [r12 + 296], rax
  .section .rodata
  .Lbynamefn28: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn28]
 lea rsi, [r12 + 288]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 cmp eax, 99
 je snoch0_n6_α
 jmp snoch0_n5_α
snoch0_n3_β:
 jmp snoch0_n6_α
snoch0_n4_α:
bb13_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S2]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n4_β:
 jmp flat_γ
snoch0_n5_α:
# IR_LIT_scalar
bb14_α:
 jmp snoch0_n7_α
 snoch0_n5_β:
 jmp flat_γ
snoch0_n6_α:
# IR_LIT_I
bb15_α:
 mov qword ptr [r12 + 304], 6
 mov rax, qword ptr [rip + .Lx33_0]
 mov qword ptr [r12 + 312], rax
 jmp xgvarg32_done
 xgvarg32_β:
 jmp snoch0_n9_α
.Lx33_0:
 .quad 4
xgvarg32_done:
bb16_α:
# BOX IR_CALL facto(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+304] -> [r12+336]
 mov rax, qword ptr [r12 + 304]
 mov qword ptr [r12 + 336], rax
 mov rax, qword ptr [r12 + 312]
 mov qword ptr [r12 + 344], rax
  .section .rodata
  .Lbynamefn35: .string "facto"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn35]
 lea rsi, [r12 + 336]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 cmp eax, 99
 je snoch0_n9_α
 jmp xgvarg31_done
xgvarg31_β:
 jmp snoch0_n9_α
xgvarg31_done:
# IR_LIT_I
bb17_α:
 mov qword ptr [r12 + 352], 6
 mov rax, qword ptr [rip + .Lx37_0]
 mov qword ptr [r12 + 360], rax
 jmp xgvarg36_done
 xgvarg36_β:
 jmp snoch0_n9_α
.Lx37_0:
 .quad 24
xgvarg36_done:
bb18_α:
# BOX IR_CALL ne(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+320] -> [r12+384]
 mov rax, qword ptr [r12 + 320]
 mov qword ptr [r12 + 384], rax
 mov rax, qword ptr [r12 + 328]
 mov qword ptr [r12 + 392], rax
# marshal arg1 = producer-box slot [r12+352] -> [r12+400]
 mov rax, qword ptr [r12 + 352]
 mov qword ptr [r12 + 400], rax
 mov rax, qword ptr [r12 + 360]
 mov qword ptr [r12 + 408], rax
  .section .rodata
  .Lbynamefn39: .string "ne"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn39]
 lea rsi, [r12 + 384]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 368], rax
 mov qword ptr [r12 + 376], rdx
 cmp eax, 99
 je snoch0_n9_α
 jmp snoch0_n8_α
snoch0_n6_β:
 jmp snoch0_n9_α
snoch0_n7_α:
bb19_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S3]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n7_β:
 jmp flat_γ
snoch0_n8_α:
# IR_LIT_scalar
bb20_α:
 jmp snoch0_n10_α
 snoch0_n8_β:
 jmp flat_γ
snoch0_n9_α:
# IR_LIT_S
bb21_α:
 mov qword ptr [r12 + 416], 1
 mov rax, qword ptr [rip + .Lx43_0]
 mov qword ptr [r12 + 424], rax
 jmp xgvarg42_done
 xgvarg42_β:
 jmp snoch0_n11_α
.Lx43_0:
 .quad .Lx43_0_s
.Lx43_0_s:
 .string "fact2(n)"
xgvarg42_done:
xargsub45_n0_α:
# IR_VAR
bb22_α:
 mov rdi, qword ptr [rip + .Lx47_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 432], rax
 mov qword ptr [r12 + 440], rdx
 jmp xargsub45_n1_α
 xargsub45_n0_β:
 jmp snoch0_n11_α
.Lx47_0:
 .quad .Lx47_0_s
.Lx47_0_s:
 .string "fact2_entry"
xargsub45_n1_α:
# IR_VAR
bb23_α:
 mov rdi, qword ptr [rip + .Lx50_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 448], rax
 mov qword ptr [r12 + 456], rdx
 jmp xunop48_arg_done
 xunop48_arg_β:
 jmp snoch0_n11_α
.Lx50_0:
 .quad .Lx50_0_s
.Lx50_0_s:
 .string "fact2_entry"
xunop48_arg_done:
jmp xgvarg44_done
xargsub45_n1_β:
jmp snoch0_n11_α
xgvarg44_done:
bb24_α:
# BOX IR_CALL define(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+416] -> [r12+480]
 mov rax, qword ptr [r12 + 416]
 mov qword ptr [r12 + 480], rax
 mov rax, qword ptr [r12 + 424]
 mov qword ptr [r12 + 488], rax
# marshal arg1 = gvar NV_GET -> [r12+496]
   lea rdi, [rip + .S4]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 496], rax
 mov qword ptr [r12 + 504], rdx
  .section .rodata
  .Lbynamefn52: .string "define"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn52]
 lea rsi, [r12 + 480]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 464], rax
 mov qword ptr [r12 + 472], rdx
 cmp eax, 99
 je snoch0_n11_α
 jmp snoch0_n11_α
snoch0_n9_β:
 jmp snoch0_n11_α
snoch0_n10_α:
bb25_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S5]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n10_β:
 jmp flat_γ
snoch0_n11_α:
# IR_LIT_I
bb26_α:
 mov qword ptr [r12 + 512], 6
 mov rax, qword ptr [rip + .Lx56_0]
 mov qword ptr [r12 + 520], rax
 jmp xgvarg55_done
 xgvarg55_β:
 jmp snoch0_n13_α
.Lx56_0:
 .quad 6
xgvarg55_done:
bb27_α:
# BOX IR_CALL fact2(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+512] -> [r12+544]
 mov rax, qword ptr [r12 + 512]
 mov qword ptr [r12 + 544], rax
 mov rax, qword ptr [r12 + 520]
 mov qword ptr [r12 + 552], rax
  .section .rodata
  .Lbynamefn58: .string "fact2"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn58]
 lea rsi, [r12 + 544]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 528], rax
 mov qword ptr [r12 + 536], rdx
 cmp eax, 99
 je snoch0_n13_α
 jmp xgvarg54_done
xgvarg54_β:
 jmp snoch0_n13_α
xgvarg54_done:
# IR_LIT_I
bb28_α:
 mov qword ptr [r12 + 560], 6
 mov rax, qword ptr [rip + .Lx60_0]
 mov qword ptr [r12 + 568], rax
 jmp xgvarg59_done
 xgvarg59_β:
 jmp snoch0_n13_α
.Lx60_0:
 .quad 720
xgvarg59_done:
bb29_α:
# BOX IR_CALL ne(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+528] -> [r12+592]
 mov rax, qword ptr [r12 + 528]
 mov qword ptr [r12 + 592], rax
 mov rax, qword ptr [r12 + 536]
 mov qword ptr [r12 + 600], rax
# marshal arg1 = producer-box slot [r12+560] -> [r12+608]
 mov rax, qword ptr [r12 + 560]
 mov qword ptr [r12 + 608], rax
 mov rax, qword ptr [r12 + 568]
 mov qword ptr [r12 + 616], rax
  .section .rodata
  .Lbynamefn62: .string "ne"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn62]
 lea rsi, [r12 + 592]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 576], rax
 mov qword ptr [r12 + 584], rdx
 cmp eax, 99
 je snoch0_n13_α
 jmp snoch0_n12_α
snoch0_n11_β:
 jmp snoch0_n13_α
snoch0_n12_α:
# IR_LIT_scalar
bb30_α:
 jmp snoch0_n14_α
 snoch0_n12_β:
 jmp flat_γ
snoch0_n13_α:
# IR_LIT_scalar
bb31_α:
 jmp snoch0_n15_α
 snoch0_n13_β:
 jmp flat_γ
snoch0_n14_α:
bb32_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S6]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n14_β:
 jmp flat_γ
snoch0_n15_α:
bb33_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S7]
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
.S0: .string "facto"
.S1: .string "output"
.S2: .string "FAIL 1010/001: fact(5)=120"
.S3: .string "FAIL 1010/002: opsyn alias"
.S4: .string "fact2_entry"
.S5: .string "FAIL 1010/003: facto(4)=24 via alias"
.S6: .string "FAIL 1010/004: fact2(6)=720 alt entry"
.S7: .string "PASS 1010_func_recursion (4/4)"
.text

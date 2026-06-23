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
 .string "axxbyyy"
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
 .string "xy"
xgvarg4_done:
# IR_LIT_S
bb3_α:
 mov qword ptr [r12 + 48], 1
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [r12 + 56], rax
 jmp xgvarg6_done
 xgvarg6_β:
 jmp snoch0_n2_α
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string "01"
xgvarg6_done:
bb4_α:
# BOX IR_CALL replace(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+16] -> [r12+80]
 mov rax, qword ptr [r12 + 16]
 mov qword ptr [r12 + 80], rax
 mov rax, qword ptr [r12 + 24]
 mov qword ptr [r12 + 88], rax
# marshal arg1 = producer-box slot [r12+32] -> [r12+96]
 mov rax, qword ptr [r12 + 32]
 mov qword ptr [r12 + 96], rax
 mov rax, qword ptr [r12 + 40]
 mov qword ptr [r12 + 104], rax
# marshal arg2 = producer-box slot [r12+48] -> [r12+112]
 mov rax, qword ptr [r12 + 48]
 mov qword ptr [r12 + 112], rax
 mov rax, qword ptr [r12 + 56]
 mov qword ptr [r12 + 120], rax
  .section .rodata
  .Lbynamefn9: .string "replace"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn9]
 lea rsi, [r12 + 80]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
 cmp eax, 99
 je snoch0_n2_α
 jmp xgvarg1_done
xgvarg1_β:
 jmp snoch0_n2_α
xgvarg1_done:
# IR_LIT_S
bb5_α:
 mov qword ptr [r12 + 128], 1
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 136], rax
 jmp xgvarg10_done
 xgvarg10_β:
 jmp snoch0_n2_α
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string "a00b111"
xgvarg10_done:
bb6_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+64] -> [r12+160]
 mov rax, qword ptr [r12 + 64]
 mov qword ptr [r12 + 160], rax
 mov rax, qword ptr [r12 + 72]
 mov qword ptr [r12 + 168], rax
# marshal arg1 = producer-box slot [r12+128] -> [r12+176]
 mov rax, qword ptr [r12 + 128]
 mov qword ptr [r12 + 176], rax
 mov rax, qword ptr [r12 + 136]
 mov qword ptr [r12 + 184], rax
  .section .rodata
  .Lbynamefn13: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn13]
 lea rsi, [r12 + 160]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 cmp eax, 99
 je snoch0_n2_α
 jmp snoch0_n1_α
snoch0_n0_β:
 jmp snoch0_n2_α
snoch0_n1_α:
# IR_LIT_scalar
bb7_α:
 jmp snoch0_n3_α
 snoch0_n1_β:
 jmp flat_γ
snoch0_n2_α:
# IR_KEYWORD_read
bb8_α:
 mov rdi, qword ptr [rip + .Lx16_0]
 call rt_keyword_read@PLT
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 jmp xgvarg15_done
 xgvarg15_β:
 jmp snoch0_n5_α
.Lx16_0:
 .quad .Lx16_0_s
.Lx16_0_s:
 .string "alphabet"
xgvarg15_done:
# IR_LIT_S
bb9_α:
 mov qword ptr [r12 + 208], 1
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [r12 + 216], rax
 jmp xgvarg17_done
 xgvarg17_β:
 jmp snoch0_n5_α
.Lx18_0:
 .quad .Lx18_0_s
.Lx18_0_s:
 .string "xy"
xgvarg17_done:
# IR_LIT_S
bb10_α:
 mov qword ptr [r12 + 224], 1
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [r12 + 232], rax
 jmp xgvarg19_done
 xgvarg19_β:
 jmp snoch0_n5_α
.Lx20_0:
 .quad .Lx20_0_s
.Lx20_0_s:
 .string "ab"
xgvarg19_done:
bb11_α:
# BOX IR_CALL replace(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+192] -> [r12+256]
 mov rax, qword ptr [r12 + 192]
 mov qword ptr [r12 + 256], rax
 mov rax, qword ptr [r12 + 200]
 mov qword ptr [r12 + 264], rax
# marshal arg1 = producer-box slot [r12+208] -> [r12+272]
 mov rax, qword ptr [r12 + 208]
 mov qword ptr [r12 + 272], rax
 mov rax, qword ptr [r12 + 216]
 mov qword ptr [r12 + 280], rax
# marshal arg2 = producer-box slot [r12+224] -> [r12+288]
 mov rax, qword ptr [r12 + 224]
 mov qword ptr [r12 + 288], rax
 mov rax, qword ptr [r12 + 232]
 mov qword ptr [r12 + 296], rax
  .section .rodata
  .Lbynamefn22: .string "replace"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn22]
 lea rsi, [r12 + 256]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
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
bb13_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S2]
 mov rsi, qword ptr [r12 + 240]
 mov rdx, qword ptr [r12 + 248]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n5_α
 snoch0_n4_β:
 jmp snoch0_n5_α
snoch0_n5_α:
# IR_LIT_S
bb14_α:
 mov qword ptr [r12 + 304], 1
 mov rax, qword ptr [rip + .Lx27_0]
 mov qword ptr [r12 + 312], rax
 jmp xgvarg26_done
 xgvarg26_β:
 jmp snoch0_n7_α
.Lx27_0:
 .quad .Lx27_0_s
.Lx27_0_s:
 .string "axy"
xgvarg26_done:
# IR_KEYWORD_read
bb15_α:
 mov rdi, qword ptr [rip + .Lx29_0]
 call rt_keyword_read@PLT
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 jmp xgvarg28_done
 xgvarg28_β:
 jmp snoch0_n7_α
.Lx29_0:
 .quad .Lx29_0_s
.Lx29_0_s:
 .string "alphabet"
xgvarg28_done:
# IR_VAR
bb16_α:
 mov rdi, qword ptr [rip + .Lx32_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 jmp xgvarg30_done
 xgvarg30_β:
 jmp snoch0_n7_α
.Lx32_0:
 .quad .Lx32_0_s
.Lx32_0_s:
 .string "a"
xgvarg30_done:
bb17_α:
# BOX IR_CALL replace(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+304] -> [r12+368]
 mov rax, qword ptr [r12 + 304]
 mov qword ptr [r12 + 368], rax
 mov rax, qword ptr [r12 + 312]
 mov qword ptr [r12 + 376], rax
# marshal arg1 = producer-box slot [r12+320] -> [r12+384]
 mov rax, qword ptr [r12 + 320]
 mov qword ptr [r12 + 384], rax
 mov rax, qword ptr [r12 + 328]
 mov qword ptr [r12 + 392], rax
# marshal arg2 = producer-box slot [r12+336] -> [r12+400]
 mov rax, qword ptr [r12 + 336]
 mov qword ptr [r12 + 400], rax
 mov rax, qword ptr [r12 + 344]
 mov qword ptr [r12 + 408], rax
  .section .rodata
  .Lbynamefn34: .string "replace"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn34]
 lea rsi, [r12 + 368]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 352], rax
 mov qword ptr [r12 + 360], rdx
 cmp eax, 99
 je snoch0_n7_α
 jmp xgvarg25_done
xgvarg25_β:
 jmp snoch0_n7_α
xgvarg25_done:
# IR_LIT_S
bb18_α:
 mov qword ptr [r12 + 416], 1
 mov rax, qword ptr [rip + .Lx36_0]
 mov qword ptr [r12 + 424], rax
 jmp xgvarg35_done
 xgvarg35_β:
 jmp snoch0_n7_α
.Lx36_0:
 .quad .Lx36_0_s
.Lx36_0_s:
 .string "aab"
xgvarg35_done:
bb19_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+352] -> [r12+448]
 mov rax, qword ptr [r12 + 352]
 mov qword ptr [r12 + 448], rax
 mov rax, qword ptr [r12 + 360]
 mov qword ptr [r12 + 456], rax
# marshal arg1 = producer-box slot [r12+416] -> [r12+464]
 mov rax, qword ptr [r12 + 416]
 mov qword ptr [r12 + 464], rax
 mov rax, qword ptr [r12 + 424]
 mov qword ptr [r12 + 472], rax
  .section .rodata
  .Lbynamefn38: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn38]
 lea rsi, [r12 + 448]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 432], rax
 mov qword ptr [r12 + 440], rdx
 cmp eax, 99
 je snoch0_n7_α
 jmp snoch0_n6_α
snoch0_n5_β:
 jmp snoch0_n7_α
snoch0_n6_α:
# IR_LIT_scalar
bb20_α:
 jmp snoch0_n8_α
 snoch0_n6_β:
 jmp flat_γ
snoch0_n7_α:
# IR_LIT_S
bb21_α:
 mov qword ptr [r12 + 480], 1
 mov rax, qword ptr [rip + .Lx42_0]
 mov qword ptr [r12 + 488], rax
 jmp xgvarg41_done
 xgvarg41_β:
 jmp snoch0_n10_α
.Lx42_0:
 .quad .Lx42_0_s
.Lx42_0_s:
 .string "hello"
xgvarg41_done:
# IR_LIT_S
bb22_α:
 mov qword ptr [r12 + 496], 1
 mov rax, qword ptr [rip + .Lx44_0]
 mov qword ptr [r12 + 504], rax
 jmp xgvarg43_done
 xgvarg43_β:
 jmp snoch0_n10_α
.Lx44_0:
 .quad .Lx44_0_s
.Lx44_0_s:
 .string "aeiou"
xgvarg43_done:
# IR_LIT_S
bb23_α:
 mov qword ptr [r12 + 512], 1
 mov rax, qword ptr [rip + .Lx46_0]
 mov qword ptr [r12 + 520], rax
 jmp xgvarg45_done
 xgvarg45_β:
 jmp snoch0_n10_α
.Lx46_0:
 .quad .Lx46_0_s
.Lx46_0_s:
 .string "aeiou"
xgvarg45_done:
bb24_α:
# BOX IR_CALL replace(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+480] -> [r12+544]
 mov rax, qword ptr [r12 + 480]
 mov qword ptr [r12 + 544], rax
 mov rax, qword ptr [r12 + 488]
 mov qword ptr [r12 + 552], rax
# marshal arg1 = producer-box slot [r12+496] -> [r12+560]
 mov rax, qword ptr [r12 + 496]
 mov qword ptr [r12 + 560], rax
 mov rax, qword ptr [r12 + 504]
 mov qword ptr [r12 + 568], rax
# marshal arg2 = producer-box slot [r12+512] -> [r12+576]
 mov rax, qword ptr [r12 + 512]
 mov qword ptr [r12 + 576], rax
 mov rax, qword ptr [r12 + 520]
 mov qword ptr [r12 + 584], rax
  .section .rodata
  .Lbynamefn48: .string "replace"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn48]
 lea rsi, [r12 + 544]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 528], rax
 mov qword ptr [r12 + 536], rdx
 cmp eax, 99
 je snoch0_n10_α
 jmp xgvarg40_done
xgvarg40_β:
 jmp snoch0_n10_α
xgvarg40_done:
# IR_LIT_S
bb25_α:
 mov qword ptr [r12 + 592], 1
 mov rax, qword ptr [rip + .Lx50_0]
 mov qword ptr [r12 + 600], rax
 jmp xgvarg49_done
 xgvarg49_β:
 jmp snoch0_n10_α
.Lx50_0:
 .quad .Lx50_0_s
.Lx50_0_s:
 .string "hello"
xgvarg49_done:
bb26_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+528] -> [r12+624]
 mov rax, qword ptr [r12 + 528]
 mov qword ptr [r12 + 624], rax
 mov rax, qword ptr [r12 + 536]
 mov qword ptr [r12 + 632], rax
# marshal arg1 = producer-box slot [r12+592] -> [r12+640]
 mov rax, qword ptr [r12 + 592]
 mov qword ptr [r12 + 640], rax
 mov rax, qword ptr [r12 + 600]
 mov qword ptr [r12 + 648], rax
  .section .rodata
  .Lbynamefn52: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn52]
 lea rsi, [r12 + 624]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 608], rax
 mov qword ptr [r12 + 616], rdx
 cmp eax, 99
 je snoch0_n10_α
 jmp snoch0_n9_α
snoch0_n7_β:
 jmp snoch0_n10_α
snoch0_n8_α:
bb27_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S3]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n8_β:
 jmp flat_γ
snoch0_n9_α:
# IR_LIT_scalar
bb28_α:
 jmp snoch0_n11_α
 snoch0_n9_β:
 jmp flat_γ
snoch0_n10_α:
# IR_LIT_scalar
bb29_α:
 jmp snoch0_n12_α
 snoch0_n10_β:
 jmp flat_γ
snoch0_n11_α:
bb30_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S4]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n11_β:
 jmp flat_γ
snoch0_n12_α:
bb31_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S5]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n12_β:
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
.S1: .string "FAIL 810/001: xy->01 mapping"
.S2: .string "a"
.S3: .string "FAIL 810/002: alphabet translation"
.S4: .string "FAIL 810/003: identity replace"
.S5: .string "PASS 810_replace (3/3)"
.text

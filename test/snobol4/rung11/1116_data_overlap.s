  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "a"
  .Lgvan1: .string "b"
  .Lgvan2: .string "output"
  .Lgvan3: .string "c"
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
 .string "node(val,lson,rson)"
xgvarg1_done:
bb2_α:
# BOX IR_CALL data(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+16] -> [r12+48]
 mov rax, qword ptr [r12 + 16]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 24]
 mov qword ptr [r12 + 56], rax
  .section .rodata
  .Lbynamefn4: .string "data"
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
# IR_LIT_S
bb3_α:
 mov qword ptr [r12 + 64], 1
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 72], rax
 jmp xgvarg5_done
 xgvarg5_β:
 jmp snoch0_n2_α
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "clunk(value,lson)"
xgvarg5_done:
bb4_α:
# BOX IR_CALL data(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+64] -> [r12+96]
 mov rax, qword ptr [r12 + 64]
 mov qword ptr [r12 + 96], rax
 mov rax, qword ptr [r12 + 72]
 mov qword ptr [r12 + 104], rax
  .section .rodata
  .Lbynamefn8: .string "data"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn8]
 lea rsi, [r12 + 96]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 cmp eax, 99
 je snoch0_n2_α
 jmp snoch0_n2_α
snoch0_n1_β:
 jmp snoch0_n2_α
snoch0_n2_α:
# IR_LIT_S
bb5_α:
 mov qword ptr [r12 + 112], 1
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [r12 + 120], rax
 jmp xgvarg9_done
 xgvarg9_β:
 jmp snoch0_n4_α
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "x"
xgvarg9_done:
# IR_LIT_S
bb6_α:
 mov qword ptr [r12 + 128], 1
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [r12 + 136], rax
 jmp xgvarg11_done
 xgvarg11_β:
 jmp snoch0_n4_α
.Lx12_0:
 .quad .Lx12_0_s
.Lx12_0_s:
 .string "y"
xgvarg11_done:
# IR_LIT_S
bb7_α:
 mov qword ptr [r12 + 144], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [r12 + 152], rax
 jmp xgvarg13_done
 xgvarg13_β:
 jmp snoch0_n4_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "z"
xgvarg13_done:
bb8_α:
# BOX IR_CALL node(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+112] -> [r12+176]
 mov rax, qword ptr [r12 + 112]
 mov qword ptr [r12 + 176], rax
 mov rax, qword ptr [r12 + 120]
 mov qword ptr [r12 + 184], rax
# marshal arg1 = producer-box slot [r12+128] -> [r12+192]
 mov rax, qword ptr [r12 + 128]
 mov qword ptr [r12 + 192], rax
 mov rax, qword ptr [r12 + 136]
 mov qword ptr [r12 + 200], rax
# marshal arg2 = producer-box slot [r12+144] -> [r12+208]
 mov rax, qword ptr [r12 + 144]
 mov qword ptr [r12 + 208], rax
 mov rax, qword ptr [r12 + 152]
 mov qword ptr [r12 + 216], rax
  .section .rodata
  .Lbynamefn16: .string "node"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn16]
 lea rsi, [r12 + 176]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 cmp eax, 99
 je snoch0_n4_α
 jmp snoch0_n3_α
snoch0_n2_β:
 jmp snoch0_n4_α
snoch0_n3_α:
bb9_α:
# IR_ASSIGN_CALL gva
 mov rax, qword ptr [r12 + 160]
 mov rcx, qword ptr [r12 + 168]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rcx
 jmp snoch0_n4_α
 snoch0_n3_β:
 jmp snoch0_n4_α
snoch0_n4_α:
bb10_α:
# BOX IR_CALL node(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn19: .string "node"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn19]
 lea rsi, [r12 + 224]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 cmp eax, 99
 je snoch0_n6_α
 jmp snoch0_n5_α
snoch0_n4_β:
 jmp snoch0_n6_α
snoch0_n5_α:
bb11_α:
# IR_ASSIGN_CALL gva
 mov rax, qword ptr [r12 + 224]
 mov rcx, qword ptr [r12 + 232]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rcx
 jmp snoch0_n6_α
 snoch0_n5_β:
 jmp snoch0_n6_α
snoch0_n6_α:
# IR_VAR gva
bb12_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 jmp xiset21_val_done
 xiset21_val_β:
 jmp snoch0_n7_α
xiset21_val_done:
# IR_LIT_S
bb13_α:
 mov qword ptr [r12 + 256], 1
 mov rax, qword ptr [rip + .Lx24_0]
 mov qword ptr [r12 + 264], rax
 jmp xiset21_key_done
 xiset21_key_β:
 jmp snoch0_n7_α
.Lx24_0:
 .quad .Lx24_0_s
.Lx24_0_s:
 .string "lson"
xiset21_key_done:
# IR_VAR gva
bb14_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 jmp xiset21_base_done
 xiset21_base_β:
 jmp snoch0_n7_α
xiset21_base_done:
bb15_α:
# IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set
 mov rax, qword ptr [r12 + 272]
 cmp eax, 4
 jne .Lx28_0
 mov rax, qword ptr [r12 + 256]
 cmp eax, 6
 jne .Lx28_0
 mov rsi, qword ptr [r12 + 280]
 mov rcx, qword ptr [r12 + 264]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx28_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx28_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 240]
 mov rdx, qword ptr [r12 + 248]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n7_α
.Lx28_0:
 mov rdi, qword ptr [r12 + 272]
 mov rsi, qword ptr [r12 + 280]
 mov rdx, qword ptr [r12 + 256]
 mov rcx, qword ptr [r12 + 264]
 mov r8, qword ptr [r12 + 240]
 mov r9, qword ptr [r12 + 248]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n7_α
 jmp snoch0_n7_α
 snoch0_n6_β:
 jmp snoch0_n7_α
snoch0_n7_α:
# IR_VAR gva
bb16_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 288], rax
 mov qword ptr [r12 + 296], rdx
 jmp xgvarg31_done
 xgvarg31_β:
 jmp snoch0_n9_α
xgvarg31_done:
bb17_α:
# BOX IR_CALL lson(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+288] -> [r12+320]
 mov rax, qword ptr [r12 + 288]
 mov qword ptr [r12 + 320], rax
 mov rax, qword ptr [r12 + 296]
 mov qword ptr [r12 + 328], rax
  .section .rodata
  .Lbynamefn35: .string "lson"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn35]
 lea rsi, [r12 + 320]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 304], rax
 mov qword ptr [r12 + 312], rdx
 cmp eax, 99
 je snoch0_n9_α
 jmp xgvarg30_done
xgvarg30_β:
 jmp snoch0_n9_α
xgvarg30_done:
bb18_α:
# BOX IR_CALL rson(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+304] -> [r12+352]
 mov rax, qword ptr [r12 + 304]
 mov qword ptr [r12 + 352], rax
 mov rax, qword ptr [r12 + 312]
 mov qword ptr [r12 + 360], rax
  .section .rodata
  .Lbynamefn37: .string "rson"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn37]
 lea rsi, [r12 + 352]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 cmp eax, 99
 je snoch0_n9_α
 jmp xgvarg29_done
xgvarg29_β:
 jmp snoch0_n9_α
xgvarg29_done:
# IR_LIT_S
bb19_α:
 mov qword ptr [r12 + 368], 1
 mov rax, qword ptr [rip + .Lx39_0]
 mov qword ptr [r12 + 376], rax
 jmp xgvarg38_done
 xgvarg38_β:
 jmp snoch0_n9_α
.Lx39_0:
 .quad .Lx39_0_s
.Lx39_0_s:
 .string "z"
xgvarg38_done:
bb20_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+336] -> [r12+400]
 mov rax, qword ptr [r12 + 336]
 mov qword ptr [r12 + 400], rax
 mov rax, qword ptr [r12 + 344]
 mov qword ptr [r12 + 408], rax
# marshal arg1 = producer-box slot [r12+368] -> [r12+416]
 mov rax, qword ptr [r12 + 368]
 mov qword ptr [r12 + 416], rax
 mov rax, qword ptr [r12 + 376]
 mov qword ptr [r12 + 424], rax
  .section .rodata
  .Lbynamefn41: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn41]
 lea rsi, [r12 + 400]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 cmp eax, 99
 je snoch0_n9_α
 jmp snoch0_n8_α
snoch0_n7_β:
 jmp snoch0_n9_α
snoch0_n8_α:
# IR_LIT_scalar
bb21_α:
 jmp snoch0_n10_α
 snoch0_n8_β:
 jmp flat_γ
snoch0_n9_α:
# IR_LIT_S
bb22_α:
 mov qword ptr [r12 + 432], 1
 mov rax, qword ptr [rip + .Lx45_0]
 mov qword ptr [r12 + 440], rax
 jmp xgvarg44_done
 xgvarg44_β:
 jmp snoch0_n12_α
.Lx45_0:
 .quad .Lx45_0_s
.Lx45_0_s:
 .string "b"
xgvarg44_done:
bb23_α:
# BOX IR_CALL value(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+432] -> [r12+464]
 mov rax, qword ptr [r12 + 432]
 mov qword ptr [r12 + 464], rax
 mov rax, qword ptr [r12 + 440]
 mov qword ptr [r12 + 472], rax
  .section .rodata
  .Lbynamefn47: .string "value"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn47]
 lea rsi, [r12 + 464]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 448], rax
 mov qword ptr [r12 + 456], rdx
 cmp eax, 99
 je snoch0_n12_α
 jmp xgvarg43_done
xgvarg43_β:
 jmp snoch0_n12_α
xgvarg43_done:
# IR_VAR gva
bb24_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 480], rax
 mov qword ptr [r12 + 488], rdx
 jmp xgvarg48_done
 xgvarg48_β:
 jmp snoch0_n12_α
xgvarg48_done:
bb25_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+448] -> [r12+512]
 mov rax, qword ptr [r12 + 448]
 mov qword ptr [r12 + 512], rax
 mov rax, qword ptr [r12 + 456]
 mov qword ptr [r12 + 520], rax
# marshal arg1 = producer-box slot [r12+480] -> [r12+528]
 mov rax, qword ptr [r12 + 480]
 mov qword ptr [r12 + 528], rax
 mov rax, qword ptr [r12 + 488]
 mov qword ptr [r12 + 536], rax
  .section .rodata
  .Lbynamefn52: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn52]
 lea rsi, [r12 + 512]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 496], rax
 mov qword ptr [r12 + 504], rdx
 cmp eax, 99
 je snoch0_n12_α
 jmp snoch0_n11_α
snoch0_n9_β:
 jmp snoch0_n12_α
snoch0_n10_α:
bb26_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S2]
 lea rsi, [rip + .S3]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n10_β:
 jmp flat_γ
snoch0_n11_α:
# IR_LIT_scalar
bb27_α:
 jmp snoch0_n13_α
 snoch0_n11_β:
 jmp flat_γ
snoch0_n12_α:
# IR_LIT_S
bb28_α:
 mov qword ptr [r12 + 544], 1
 mov rax, qword ptr [rip + .Lx56_0]
 mov qword ptr [r12 + 552], rax
 jmp xgvarg55_done
 xgvarg55_β:
 jmp snoch0_n15_α
.Lx56_0:
 .quad .Lx56_0_s
.Lx56_0_s:
 .string "alpha"
xgvarg55_done:
# IR_LIT_S
bb29_α:
 mov qword ptr [r12 + 560], 1
 mov rax, qword ptr [rip + .Lx58_0]
 mov qword ptr [r12 + 568], rax
 jmp xgvarg57_done
 xgvarg57_β:
 jmp snoch0_n15_α
.Lx58_0:
 .quad .Lx58_0_s
.Lx58_0_s:
 .string "beta"
xgvarg57_done:
bb30_α:
# BOX IR_CALL clunk(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+544] -> [r12+592]
 mov rax, qword ptr [r12 + 544]
 mov qword ptr [r12 + 592], rax
 mov rax, qword ptr [r12 + 552]
 mov qword ptr [r12 + 600], rax
# marshal arg1 = producer-box slot [r12+560] -> [r12+608]
 mov rax, qword ptr [r12 + 560]
 mov qword ptr [r12 + 608], rax
 mov rax, qword ptr [r12 + 568]
 mov qword ptr [r12 + 616], rax
  .section .rodata
  .Lbynamefn60: .string "clunk"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn60]
 lea rsi, [r12 + 592]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 576], rax
 mov qword ptr [r12 + 584], rdx
 cmp eax, 99
 je snoch0_n15_α
 jmp snoch0_n14_α
snoch0_n12_β:
 jmp snoch0_n15_α
snoch0_n13_α:
bb31_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S2]
 lea rsi, [rip + .S4]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n13_β:
 jmp flat_γ
snoch0_n14_α:
bb32_α:
# IR_ASSIGN_CALL gva
 mov rax, qword ptr [r12 + 576]
 mov rcx, qword ptr [r12 + 584]
 mov qword ptr [rbx + 48], rax
 mov qword ptr [rbx + 56], rcx
 jmp snoch0_n15_α
 snoch0_n14_β:
 jmp snoch0_n15_α
snoch0_n15_α:
# IR_VAR gva
bb33_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 624], rax
 mov qword ptr [r12 + 632], rdx
 jmp xgvarg64_done
 xgvarg64_β:
 jmp snoch0_n17_α
xgvarg64_done:
bb34_α:
# BOX IR_CALL lson(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+624] -> [r12+656]
 mov rax, qword ptr [r12 + 624]
 mov qword ptr [r12 + 656], rax
 mov rax, qword ptr [r12 + 632]
 mov qword ptr [r12 + 664], rax
  .section .rodata
  .Lbynamefn68: .string "lson"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn68]
 lea rsi, [r12 + 656]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 640], rax
 mov qword ptr [r12 + 648], rdx
 cmp eax, 99
 je snoch0_n17_α
 jmp xgvarg63_done
xgvarg63_β:
 jmp snoch0_n17_α
xgvarg63_done:
# IR_LIT_S
bb35_α:
 mov qword ptr [r12 + 672], 1
 mov rax, qword ptr [rip + .Lx70_0]
 mov qword ptr [r12 + 680], rax
 jmp xgvarg69_done
 xgvarg69_β:
 jmp snoch0_n17_α
.Lx70_0:
 .quad .Lx70_0_s
.Lx70_0_s:
 .string "beta"
xgvarg69_done:
bb36_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+640] -> [r12+704]
 mov rax, qword ptr [r12 + 640]
 mov qword ptr [r12 + 704], rax
 mov rax, qword ptr [r12 + 648]
 mov qword ptr [r12 + 712], rax
# marshal arg1 = producer-box slot [r12+672] -> [r12+720]
 mov rax, qword ptr [r12 + 672]
 mov qword ptr [r12 + 720], rax
 mov rax, qword ptr [r12 + 680]
 mov qword ptr [r12 + 728], rax
  .section .rodata
  .Lbynamefn72: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn72]
 lea rsi, [r12 + 704]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 688], rax
 mov qword ptr [r12 + 696], rdx
 cmp eax, 99
 je snoch0_n17_α
 jmp snoch0_n16_α
snoch0_n15_β:
 jmp snoch0_n17_α
snoch0_n16_α:
# IR_LIT_scalar
bb37_α:
 jmp snoch0_n18_α
 snoch0_n16_β:
 jmp flat_γ
snoch0_n17_α:
# IR_LIT_scalar
bb38_α:
 jmp snoch0_n19_α
 snoch0_n17_β:
 jmp flat_γ
snoch0_n18_α:
bb39_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S2]
 lea rsi, [rip + .S6]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n18_β:
 jmp flat_γ
snoch0_n19_α:
bb40_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S2]
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
.S0: .string "a"
.S1: .string "b"
.S2: .string "output"
.S3: .string "FAIL 1116/001: node.rson after clunk data def"
.S4: .string "FAIL 1116/002: value() still works after clunk"
.S5: .string "c"
.S6: .string "FAIL 1116/003: clunk.lson accessor"
.S7: .string "PASS 1116_data_overlap (3/3)"
.text

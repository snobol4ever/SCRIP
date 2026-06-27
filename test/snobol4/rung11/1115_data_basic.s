  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "a"
  .Lgvan1: .string "output"
  .Lgvan2: .string "b"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .quad .Lgvan2
  .section .bss
  .align 16
__gva: .space 48, 0
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
  mov edx, 3
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
 jmp snoch0_n3_α
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "x"
xgvarg5_done:
# IR_LIT_S
bb4_α:
 mov qword ptr [r12 + 80], 1
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [r12 + 88], rax
 jmp xgvarg7_done
 xgvarg7_β:
 jmp snoch0_n3_α
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "y"
xgvarg7_done:
# IR_LIT_S
bb5_α:
 mov qword ptr [r12 + 96], 1
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [r12 + 104], rax
 jmp xgvarg9_done
 xgvarg9_β:
 jmp snoch0_n3_α
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "z"
xgvarg9_done:
bb6_α:
# BOX IR_CALL node(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
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
  .Lbynamefn12: .string "node"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn12]
 lea rsi, [r12 + 128]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 cmp eax, 99
 je snoch0_n3_α
 jmp snoch0_n2_α
snoch0_n1_β:
 jmp snoch0_n3_α
snoch0_n2_α:
bb7_α:
# IR_ASSIGN_CALL gva
 mov rax, qword ptr [r12 + 112]
 mov rcx, qword ptr [r12 + 120]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rcx
 jmp snoch0_n3_α
 snoch0_n2_β:
 jmp snoch0_n3_α
snoch0_n3_α:
# IR_VAR gva
bb8_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 jmp xgvarg16_done
 xgvarg16_β:
 jmp snoch0_n5_α
xgvarg16_done:
bb9_α:
# BOX IR_CALL datatype(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+176] -> [r12+208]
 mov rax, qword ptr [r12 + 176]
 mov qword ptr [r12 + 208], rax
 mov rax, qword ptr [r12 + 184]
 mov qword ptr [r12 + 216], rax
  .section .rodata
  .Lbynamefn20: .string "datatype"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn20]
 lea rsi, [r12 + 208]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 cmp eax, 99
 je snoch0_n5_α
 jmp xgvarg15_done
xgvarg15_β:
 jmp snoch0_n5_α
xgvarg15_done:
# IR_KEYWORD_read
bb10_α:
 mov rdi, qword ptr [rip + .Lx22_0]
 call rt_keyword_read@PLT
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 jmp xgvarg21_done
 xgvarg21_β:
 jmp snoch0_n5_α
.Lx22_0:
 .quad .Lx22_0_s
.Lx22_0_s:
 .string "lcase"
xgvarg21_done:
# IR_KEYWORD_read
bb11_α:
 mov rdi, qword ptr [rip + .Lx24_0]
 call rt_keyword_read@PLT
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 jmp xgvarg23_done
 xgvarg23_β:
 jmp snoch0_n5_α
.Lx24_0:
 .quad .Lx24_0_s
.Lx24_0_s:
 .string "ucase"
xgvarg23_done:
bb12_α:
# BOX IR_CALL replace(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+192] -> [r12+272]
 mov rax, qword ptr [r12 + 192]
 mov qword ptr [r12 + 272], rax
 mov rax, qword ptr [r12 + 200]
 mov qword ptr [r12 + 280], rax
# marshal arg1 = producer-box slot [r12+224] -> [r12+288]
 mov rax, qword ptr [r12 + 224]
 mov qword ptr [r12 + 288], rax
 mov rax, qword ptr [r12 + 232]
 mov qword ptr [r12 + 296], rax
# marshal arg2 = producer-box slot [r12+240] -> [r12+304]
 mov rax, qword ptr [r12 + 240]
 mov qword ptr [r12 + 304], rax
 mov rax, qword ptr [r12 + 248]
 mov qword ptr [r12 + 312], rax
  .section .rodata
  .Lbynamefn26: .string "replace"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn26]
 lea rsi, [r12 + 272]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
 cmp eax, 99
 je snoch0_n5_α
 jmp xgvarg14_done
xgvarg14_β:
 jmp snoch0_n5_α
xgvarg14_done:
# IR_LIT_S
bb13_α:
 mov qword ptr [r12 + 320], 1
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [r12 + 328], rax
 jmp xgvarg27_done
 xgvarg27_β:
 jmp snoch0_n5_α
.Lx28_0:
 .quad .Lx28_0_s
.Lx28_0_s:
 .string "NODE"
xgvarg27_done:
bb14_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+256] -> [r12+352]
 mov rax, qword ptr [r12 + 256]
 mov qword ptr [r12 + 352], rax
 mov rax, qword ptr [r12 + 264]
 mov qword ptr [r12 + 360], rax
# marshal arg1 = producer-box slot [r12+320] -> [r12+368]
 mov rax, qword ptr [r12 + 320]
 mov qword ptr [r12 + 368], rax
 mov rax, qword ptr [r12 + 328]
 mov qword ptr [r12 + 376], rax
  .section .rodata
  .Lbynamefn30: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn30]
 lea rsi, [r12 + 352]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 cmp eax, 99
 je snoch0_n5_α
 jmp snoch0_n4_α
snoch0_n3_β:
 jmp snoch0_n5_α
snoch0_n4_α:
# IR_LIT_scalar
bb15_α:
 jmp snoch0_n6_α
 snoch0_n4_β:
 jmp flat_γ
snoch0_n5_α:
# IR_VAR gva
bb16_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 jmp xgvarg33_done
 xgvarg33_β:
 jmp snoch0_n8_α
xgvarg33_done:
bb17_α:
# BOX IR_CALL val(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+384] -> [r12+416]
 mov rax, qword ptr [r12 + 384]
 mov qword ptr [r12 + 416], rax
 mov rax, qword ptr [r12 + 392]
 mov qword ptr [r12 + 424], rax
  .section .rodata
  .Lbynamefn37: .string "val"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn37]
 lea rsi, [r12 + 416]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 400], rax
 mov qword ptr [r12 + 408], rdx
 cmp eax, 99
 je snoch0_n8_α
 jmp xgvarg32_done
xgvarg32_β:
 jmp snoch0_n8_α
xgvarg32_done:
# IR_LIT_S
bb18_α:
 mov qword ptr [r12 + 432], 1
 mov rax, qword ptr [rip + .Lx39_0]
 mov qword ptr [r12 + 440], rax
 jmp xgvarg38_done
 xgvarg38_β:
 jmp snoch0_n8_α
.Lx39_0:
 .quad .Lx39_0_s
.Lx39_0_s:
 .string "x"
xgvarg38_done:
bb19_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+400] -> [r12+464]
 mov rax, qword ptr [r12 + 400]
 mov qword ptr [r12 + 464], rax
 mov rax, qword ptr [r12 + 408]
 mov qword ptr [r12 + 472], rax
# marshal arg1 = producer-box slot [r12+432] -> [r12+480]
 mov rax, qword ptr [r12 + 432]
 mov qword ptr [r12 + 480], rax
 mov rax, qword ptr [r12 + 440]
 mov qword ptr [r12 + 488], rax
  .section .rodata
  .Lbynamefn41: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn41]
 lea rsi, [r12 + 464]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 448], rax
 mov qword ptr [r12 + 456], rdx
 cmp eax, 99
 je snoch0_n8_α
 jmp snoch0_n7_α
snoch0_n5_β:
 jmp snoch0_n8_α
snoch0_n6_α:
bb20_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S2]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n6_β:
 jmp flat_γ
snoch0_n7_α:
# IR_LIT_scalar
bb21_α:
 jmp snoch0_n9_α
 snoch0_n7_β:
 jmp flat_γ
snoch0_n8_α:
bb22_α:
# BOX IR_CALL node(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn45: .string "node"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn45]
 lea rsi, [r12 + 496]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 496], rax
 mov qword ptr [r12 + 504], rdx
 cmp eax, 99
 je snoch0_n11_α
 jmp snoch0_n10_α
snoch0_n8_β:
 jmp snoch0_n11_α
snoch0_n9_α:
bb23_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S3]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n9_β:
 jmp flat_γ
snoch0_n10_α:
bb24_α:
# IR_ASSIGN_CALL gva
 mov rax, qword ptr [r12 + 496]
 mov rcx, qword ptr [r12 + 504]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rcx
 jmp snoch0_n11_α
 snoch0_n10_β:
 jmp snoch0_n11_α
snoch0_n11_α:
# IR_VAR gva
bb25_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 512], rax
 mov qword ptr [r12 + 520], rdx
 jmp xgvarg49_done
 xgvarg49_β:
 jmp snoch0_n13_α
xgvarg49_done:
bb26_α:
# BOX IR_CALL rson(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+512] -> [r12+544]
 mov rax, qword ptr [r12 + 512]
 mov qword ptr [r12 + 544], rax
 mov rax, qword ptr [r12 + 520]
 mov qword ptr [r12 + 552], rax
  .section .rodata
  .Lbynamefn53: .string "rson"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn53]
 lea rsi, [r12 + 544]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 528], rax
 mov qword ptr [r12 + 536], rdx
 cmp eax, 99
 je snoch0_n13_α
 jmp xgvarg48_done
xgvarg48_β:
 jmp snoch0_n13_α
xgvarg48_done:
bb27_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+528] -> [r12+576]
 mov rax, qword ptr [r12 + 528]
 mov qword ptr [r12 + 576], rax
 mov rax, qword ptr [r12 + 536]
 mov qword ptr [r12 + 584], rax
  .section .rodata
  .Lbynamefn55: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn55]
 lea rsi, [r12 + 576]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 560], rax
 mov qword ptr [r12 + 568], rdx
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
# IR_VAR gva
bb29_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 592], rax
 mov qword ptr [r12 + 600], rdx
 jmp xiset57_val_done
 xiset57_val_β:
 jmp snoch0_n15_α
xiset57_val_done:
# IR_LIT_S
bb30_α:
 mov qword ptr [r12 + 608], 1
 mov rax, qword ptr [rip + .Lx60_0]
 mov qword ptr [r12 + 616], rax
 jmp xiset57_key_done
 xiset57_key_β:
 jmp snoch0_n15_α
.Lx60_0:
 .quad .Lx60_0_s
.Lx60_0_s:
 .string "lson"
xiset57_key_done:
# IR_VAR gva
bb31_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 624], rax
 mov qword ptr [r12 + 632], rdx
 jmp xiset57_base_done
 xiset57_base_β:
 jmp snoch0_n15_α
xiset57_base_done:
bb32_α:
# IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set
 mov rax, qword ptr [r12 + 624]
 cmp eax, 4
 jne .Lx64_0
 mov rax, qword ptr [r12 + 608]
 cmp eax, 6
 jne .Lx64_0
 mov rsi, qword ptr [r12 + 632]
 mov rcx, qword ptr [r12 + 616]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx64_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx64_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 592]
 mov rdx, qword ptr [r12 + 600]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n15_α
.Lx64_0:
 mov rdi, qword ptr [r12 + 624]
 mov rsi, qword ptr [r12 + 632]
 mov rdx, qword ptr [r12 + 608]
 mov rcx, qword ptr [r12 + 616]
 mov r8, qword ptr [r12 + 592]
 mov r9, qword ptr [r12 + 600]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n15_α
 jmp snoch0_n15_α
 snoch0_n13_β:
 jmp snoch0_n15_α
snoch0_n14_α:
bb33_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S5]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n14_β:
 jmp flat_γ
snoch0_n15_α:
# IR_VAR gva
bb34_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 640], rax
 mov qword ptr [r12 + 648], rdx
 jmp xgvarg68_done
 xgvarg68_β:
 jmp snoch0_n17_α
xgvarg68_done:
bb35_α:
# BOX IR_CALL lson(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+640] -> [r12+672]
 mov rax, qword ptr [r12 + 640]
 mov qword ptr [r12 + 672], rax
 mov rax, qword ptr [r12 + 648]
 mov qword ptr [r12 + 680], rax
  .section .rodata
  .Lbynamefn72: .string "lson"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn72]
 lea rsi, [r12 + 672]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 656], rax
 mov qword ptr [r12 + 664], rdx
 cmp eax, 99
 je snoch0_n17_α
 jmp xgvarg67_done
xgvarg67_β:
 jmp snoch0_n17_α
xgvarg67_done:
bb36_α:
# BOX IR_CALL rson(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+656] -> [r12+704]
 mov rax, qword ptr [r12 + 656]
 mov qword ptr [r12 + 704], rax
 mov rax, qword ptr [r12 + 664]
 mov qword ptr [r12 + 712], rax
  .section .rodata
  .Lbynamefn74: .string "rson"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn74]
 lea rsi, [r12 + 704]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 688], rax
 mov qword ptr [r12 + 696], rdx
 cmp eax, 99
 je snoch0_n17_α
 jmp xgvarg66_done
xgvarg66_β:
 jmp snoch0_n17_α
xgvarg66_done:
# IR_LIT_S
bb37_α:
 mov qword ptr [r12 + 720], 1
 mov rax, qword ptr [rip + .Lx76_0]
 mov qword ptr [r12 + 728], rax
 jmp xgvarg75_done
 xgvarg75_β:
 jmp snoch0_n17_α
.Lx76_0:
 .quad .Lx76_0_s
.Lx76_0_s:
 .string "z"
xgvarg75_done:
bb38_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+688] -> [r12+752]
 mov rax, qword ptr [r12 + 688]
 mov qword ptr [r12 + 752], rax
 mov rax, qword ptr [r12 + 696]
 mov qword ptr [r12 + 760], rax
# marshal arg1 = producer-box slot [r12+720] -> [r12+768]
 mov rax, qword ptr [r12 + 720]
 mov qword ptr [r12 + 768], rax
 mov rax, qword ptr [r12 + 728]
 mov qword ptr [r12 + 776], rax
  .section .rodata
  .Lbynamefn78: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn78]
 lea rsi, [r12 + 752]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 736], rax
 mov qword ptr [r12 + 744], rdx
 cmp eax, 99
 je snoch0_n17_α
 jmp snoch0_n16_α
snoch0_n15_β:
 jmp snoch0_n17_α
snoch0_n16_α:
# IR_LIT_scalar
bb39_α:
 jmp snoch0_n18_α
 snoch0_n16_β:
 jmp flat_γ
snoch0_n17_α:
# IR_LIT_S
bb40_α:
 mov qword ptr [r12 + 784], 1
 mov rax, qword ptr [rip + .Lx82_0]
 mov qword ptr [r12 + 792], rax
 jmp xgvarg81_done
 xgvarg81_β:
 jmp snoch0_n20_α
.Lx82_0:
 .quad .Lx82_0_s
.Lx82_0_s:
 .string "b"
xgvarg81_done:
bb41_α:
# BOX IR_CALL value(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+784] -> [r12+816]
 mov rax, qword ptr [r12 + 784]
 mov qword ptr [r12 + 816], rax
 mov rax, qword ptr [r12 + 792]
 mov qword ptr [r12 + 824], rax
  .section .rodata
  .Lbynamefn84: .string "value"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn84]
 lea rsi, [r12 + 816]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 800], rax
 mov qword ptr [r12 + 808], rdx
 cmp eax, 99
 je snoch0_n20_α
 jmp xgvarg80_done
xgvarg80_β:
 jmp snoch0_n20_α
xgvarg80_done:
# IR_VAR gva
bb42_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 832], rax
 mov qword ptr [r12 + 840], rdx
 jmp xgvarg85_done
 xgvarg85_β:
 jmp snoch0_n20_α
xgvarg85_done:
bb43_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+800] -> [r12+864]
 mov rax, qword ptr [r12 + 800]
 mov qword ptr [r12 + 864], rax
 mov rax, qword ptr [r12 + 808]
 mov qword ptr [r12 + 872], rax
# marshal arg1 = producer-box slot [r12+832] -> [r12+880]
 mov rax, qword ptr [r12 + 832]
 mov qword ptr [r12 + 880], rax
 mov rax, qword ptr [r12 + 840]
 mov qword ptr [r12 + 888], rax
  .section .rodata
  .Lbynamefn89: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn89]
 lea rsi, [r12 + 864]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 848], rax
 mov qword ptr [r12 + 856], rdx
 cmp eax, 99
 je snoch0_n20_α
 jmp snoch0_n19_α
snoch0_n17_β:
 jmp snoch0_n20_α
snoch0_n18_α:
bb44_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S6]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n18_β:
 jmp flat_γ
snoch0_n19_α:
# IR_LIT_scalar
bb45_α:
 jmp snoch0_n21_α
 snoch0_n19_β:
 jmp flat_γ
snoch0_n20_α:
# IR_LIT_S
bb46_α:
 mov qword ptr [r12 + 896], 1
 mov rax, qword ptr [rip + .Lx93_0]
 mov qword ptr [r12 + 904], rax
 jmp xiset92_val_done
 xiset92_val_β:
 jmp snoch0_n22_α
.Lx93_0:
 .quad .Lx93_0_s
.Lx93_0_s:
 .string "new"
xiset92_val_done:
# IR_LIT_S
bb47_α:
 mov qword ptr [r12 + 912], 1
 mov rax, qword ptr [rip + .Lx94_0]
 mov qword ptr [r12 + 920], rax
 jmp xiset92_key_done
 xiset92_key_β:
 jmp snoch0_n22_α
.Lx94_0:
 .quad .Lx94_0_s
.Lx94_0_s:
 .string "val"
xiset92_key_done:
# IR_VAR gva
bb48_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 928], rax
 mov qword ptr [r12 + 936], rdx
 jmp xiset92_base_done
 xiset92_base_β:
 jmp snoch0_n22_α
xiset92_base_done:
bb49_α:
# IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set
 mov rax, qword ptr [r12 + 928]
 cmp eax, 4
 jne .Lx98_0
 mov rax, qword ptr [r12 + 912]
 cmp eax, 6
 jne .Lx98_0
 mov rsi, qword ptr [r12 + 936]
 mov rcx, qword ptr [r12 + 920]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx98_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx98_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 896]
 mov rdx, qword ptr [r12 + 904]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n22_α
.Lx98_0:
 mov rdi, qword ptr [r12 + 928]
 mov rsi, qword ptr [r12 + 936]
 mov rdx, qword ptr [r12 + 912]
 mov rcx, qword ptr [r12 + 920]
 mov r8, qword ptr [r12 + 896]
 mov r9, qword ptr [r12 + 904]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n22_α
 jmp snoch0_n22_α
 snoch0_n20_β:
 jmp snoch0_n22_α
snoch0_n21_α:
bb50_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S7]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n21_β:
 jmp flat_γ
snoch0_n22_α:
# IR_VAR gva
bb51_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 944], rax
 mov qword ptr [r12 + 952], rdx
 jmp xgvarg101_done
 xgvarg101_β:
 jmp snoch0_n24_α
xgvarg101_done:
bb52_α:
# BOX IR_CALL val(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+944] -> [r12+976]
 mov rax, qword ptr [r12 + 944]
 mov qword ptr [r12 + 976], rax
 mov rax, qword ptr [r12 + 952]
 mov qword ptr [r12 + 984], rax
  .section .rodata
  .Lbynamefn105: .string "val"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn105]
 lea rsi, [r12 + 976]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 960], rax
 mov qword ptr [r12 + 968], rdx
 cmp eax, 99
 je snoch0_n24_α
 jmp xgvarg100_done
xgvarg100_β:
 jmp snoch0_n24_α
xgvarg100_done:
# IR_LIT_S
bb53_α:
 mov qword ptr [r12 + 992], 1
 mov rax, qword ptr [rip + .Lx107_0]
 mov qword ptr [r12 + 1000], rax
 jmp xgvarg106_done
 xgvarg106_β:
 jmp snoch0_n24_α
.Lx107_0:
 .quad .Lx107_0_s
.Lx107_0_s:
 .string "new"
xgvarg106_done:
bb54_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+960] -> [r12+1024]
 mov rax, qword ptr [r12 + 960]
 mov qword ptr [r12 + 1024], rax
 mov rax, qword ptr [r12 + 968]
 mov qword ptr [r12 + 1032], rax
# marshal arg1 = producer-box slot [r12+992] -> [r12+1040]
 mov rax, qword ptr [r12 + 992]
 mov qword ptr [r12 + 1040], rax
 mov rax, qword ptr [r12 + 1000]
 mov qword ptr [r12 + 1048], rax
  .section .rodata
  .Lbynamefn109: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn109]
 lea rsi, [r12 + 1024]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1008], rax
 mov qword ptr [r12 + 1016], rdx
 cmp eax, 99
 je snoch0_n24_α
 jmp snoch0_n23_α
snoch0_n22_β:
 jmp snoch0_n24_α
snoch0_n23_α:
# IR_LIT_scalar
bb55_α:
 jmp snoch0_n25_α
 snoch0_n23_β:
 jmp flat_γ
snoch0_n24_α:
# IR_LIT_scalar
bb56_α:
 jmp snoch0_n26_α
 snoch0_n24_β:
 jmp flat_γ
snoch0_n25_α:
bb57_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S8]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n25_β:
 jmp flat_γ
snoch0_n26_α:
bb58_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S9]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n26_β:
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
.S1: .string "output"
.S2: .string "FAIL 1115/001: datatype of node"
.S3: .string "FAIL 1115/002: field accessor val"
.S4: .string "b"
.S5: .string "FAIL 1115/003: unset field is null"
.S6: .string "FAIL 1115/004: nested accessor after mutate"
.S7: .string "FAIL 1115/005: value() by variable name"
.S8: .string "FAIL 1115/006: mutate field and read back"
.S9: .string "PASS 1115_data_basic (6/6)"
.text

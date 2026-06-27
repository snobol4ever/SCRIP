  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "aaa"
  .Lgvan1: .string "output"
  .Lgvan2: .string "ama"
  .Lgvan3: .string "tt"
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
# IR_LIT_I
bb1_α:
 mov qword ptr [r12 + 16], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 24], rax
 jmp xgvarg1_done
 xgvarg1_β:
 jmp snoch0_n2_α
.Lx2_0:
 .quad 10
xgvarg1_done:
bb2_α:
# BOX IR_CALL array(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+16] -> [r12+48]
 mov rax, qword ptr [r12 + 16]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 24]
 mov qword ptr [r12 + 56], rax
  .section .rodata
  .Lbynamefn4: .string "array"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn4]
 lea rsi, [r12 + 48]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je snoch0_n2_α
 jmp snoch0_n1_α
snoch0_n0_β:
 jmp snoch0_n2_α
snoch0_n1_α:
bb3_α:
# IR_ASSIGN_CALL gva
 mov rax, qword ptr [r12 + 32]
 mov rcx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rcx
 jmp snoch0_n2_α
 snoch0_n1_β:
 jmp snoch0_n2_α
snoch0_n2_α:
# IR_VAR gva
bb4_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
 jmp xgvarg7_done
 xgvarg7_β:
 jmp snoch0_n4_α
xgvarg7_done:
# IR_LIT_I
bb5_α:
 mov qword ptr [r12 + 80], 6
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 88], rax
 jmp xgvarg10_done
 xgvarg10_β:
 jmp snoch0_n4_α
.Lx11_0:
 .quad 1
xgvarg10_done:
bb6_α:
# BOX IR_CALL item(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+64] -> [r12+112]
 mov rax, qword ptr [r12 + 64]
 mov qword ptr [r12 + 112], rax
 mov rax, qword ptr [r12 + 72]
 mov qword ptr [r12 + 120], rax
# marshal arg1 = producer-box slot [r12+80] -> [r12+128]
 mov rax, qword ptr [r12 + 80]
 mov qword ptr [r12 + 128], rax
 mov rax, qword ptr [r12 + 88]
 mov qword ptr [r12 + 136], rax
  .section .rodata
  .Lbynamefn13: .string "item"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn13]
 lea rsi, [r12 + 112]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 cmp eax, 99
 je snoch0_n4_α
 jmp xgvarg6_done
xgvarg6_β:
 jmp snoch0_n4_α
xgvarg6_done:
# IR_LIT_I
bb7_α:
 mov qword ptr [r12 + 144], 6
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [r12 + 152], rax
 jmp xgvarg14_done
 xgvarg14_β:
 jmp snoch0_n4_α
.Lx15_0:
 .quad 5
xgvarg14_done:
bb8_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+96] -> [r12+176]
 mov rax, qword ptr [r12 + 96]
 mov qword ptr [r12 + 176], rax
 mov rax, qword ptr [r12 + 104]
 mov qword ptr [r12 + 184], rax
# marshal arg1 = producer-box slot [r12+144] -> [r12+192]
 mov rax, qword ptr [r12 + 144]
 mov qword ptr [r12 + 192], rax
 mov rax, qword ptr [r12 + 152]
 mov qword ptr [r12 + 200], rax
  .section .rodata
  .Lbynamefn17: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn17]
 lea rsi, [r12 + 176]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 cmp eax, 99
 je snoch0_n4_α
 jmp snoch0_n3_α
snoch0_n2_β:
 jmp snoch0_n4_α
snoch0_n3_α:
# IR_LIT_scalar
bb9_α:
 jmp snoch0_n5_α
 snoch0_n3_β:
 jmp flat_γ
snoch0_n4_α:
bb10_α:
# IR_IDX: AXS inline DT_A+int fast path, else subscript_get
 lea rdi, [rip + .S0]
 call NV_GET_fn@PLT
 cmp eax, 4
 jne .Lx21_0
 movabs rcx, 1
 mov rsi, rdx
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx21_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx21_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 mov rax, [r11 + rcx*8]
 add r11, 8
 mov rdx, [r11 + rcx*8]
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 jmp xgvarg19_done
.Lx21_0:
 mov rdi, rax
 mov rsi, rdx
 movabs rdx, 6
 movabs rcx, 1
 call subscript_get@PLT
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 jmp xgvarg19_done
 xgvarg19_β:
 jmp snoch0_n7_α
xgvarg19_done:
# IR_LIT_I
bb11_α:
 mov qword ptr [r12 + 240], 6
 mov rax, qword ptr [rip + .Lx23_0]
 mov qword ptr [r12 + 248], rax
 jmp xgvarg22_done
 xgvarg22_β:
 jmp snoch0_n7_α
.Lx23_0:
 .quad 5
xgvarg22_done:
bb12_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+224] -> [r12+272]
 mov rax, qword ptr [r12 + 224]
 mov qword ptr [r12 + 272], rax
 mov rax, qword ptr [r12 + 232]
 mov qword ptr [r12 + 280], rax
# marshal arg1 = producer-box slot [r12+240] -> [r12+288]
 mov rax, qword ptr [r12 + 240]
 mov qword ptr [r12 + 288], rax
 mov rax, qword ptr [r12 + 248]
 mov qword ptr [r12 + 296], rax
  .section .rodata
  .Lbynamefn25: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn25]
 lea rsi, [r12 + 272]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
 cmp eax, 99
 je snoch0_n7_α
 jmp snoch0_n6_α
snoch0_n4_β:
 jmp snoch0_n7_α
snoch0_n5_α:
bb13_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S2]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n5_β:
 jmp flat_γ
snoch0_n6_α:
# IR_LIT_scalar
bb14_α:
 jmp snoch0_n8_α
 snoch0_n6_β:
 jmp flat_γ
snoch0_n7_α:
# IR_LIT_I
bb15_α:
 mov qword ptr [r12 + 304], 6
 mov rax, qword ptr [rip + .Lx29_0]
 mov qword ptr [r12 + 312], rax
 jmp xiset28_val_done
 xiset28_val_β:
 jmp snoch0_n9_α
.Lx29_0:
 .quad 22
xiset28_val_done:
# IR_LIT_I
bb16_α:
 mov qword ptr [r12 + 320], 6
 mov rax, qword ptr [rip + .Lx30_0]
 mov qword ptr [r12 + 328], rax
 jmp xiset28_key_done
 xiset28_key_β:
 jmp snoch0_n9_α
.Lx30_0:
 .quad 2
xiset28_key_done:
# IR_VAR gva
bb17_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 jmp xiset28_base_done
 xiset28_base_β:
 jmp snoch0_n9_α
xiset28_base_done:
bb18_α:
# IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set
 mov rax, qword ptr [r12 + 336]
 cmp eax, 4
 jne .Lx34_0
 mov rax, qword ptr [r12 + 320]
 cmp eax, 6
 jne .Lx34_0
 mov rsi, qword ptr [r12 + 344]
 mov rcx, qword ptr [r12 + 328]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx34_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx34_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 304]
 mov rdx, qword ptr [r12 + 312]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n9_α
.Lx34_0:
 mov rdi, qword ptr [r12 + 336]
 mov rsi, qword ptr [r12 + 344]
 mov rdx, qword ptr [r12 + 320]
 mov rcx, qword ptr [r12 + 328]
 mov r8, qword ptr [r12 + 304]
 mov r9, qword ptr [r12 + 312]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n9_α
 jmp snoch0_n9_α
 snoch0_n7_β:
 jmp snoch0_n9_α
snoch0_n8_α:
bb19_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S3]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n8_β:
 jmp flat_γ
snoch0_n9_α:
# IR_VAR gva
bb20_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 352], rax
 mov qword ptr [r12 + 360], rdx
 jmp xgvarg37_done
 xgvarg37_β:
 jmp snoch0_n11_α
xgvarg37_done:
# IR_LIT_I
bb21_α:
 mov qword ptr [r12 + 368], 6
 mov rax, qword ptr [rip + .Lx41_0]
 mov qword ptr [r12 + 376], rax
 jmp xgvarg40_done
 xgvarg40_β:
 jmp snoch0_n11_α
.Lx41_0:
 .quad 2
xgvarg40_done:
bb22_α:
# BOX IR_CALL item(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+352] -> [r12+400]
 mov rax, qword ptr [r12 + 352]
 mov qword ptr [r12 + 400], rax
 mov rax, qword ptr [r12 + 360]
 mov qword ptr [r12 + 408], rax
# marshal arg1 = producer-box slot [r12+368] -> [r12+416]
 mov rax, qword ptr [r12 + 368]
 mov qword ptr [r12 + 416], rax
 mov rax, qword ptr [r12 + 376]
 mov qword ptr [r12 + 424], rax
  .section .rodata
  .Lbynamefn43: .string "item"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn43]
 lea rsi, [r12 + 400]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 cmp eax, 99
 je snoch0_n11_α
 jmp xgvarg36_done
xgvarg36_β:
 jmp snoch0_n11_α
xgvarg36_done:
# IR_LIT_I
bb23_α:
 mov qword ptr [r12 + 432], 6
 mov rax, qword ptr [rip + .Lx45_0]
 mov qword ptr [r12 + 440], rax
 jmp xgvarg44_done
 xgvarg44_β:
 jmp snoch0_n11_α
.Lx45_0:
 .quad 22
xgvarg44_done:
bb24_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+384] -> [r12+464]
 mov rax, qword ptr [r12 + 384]
 mov qword ptr [r12 + 464], rax
 mov rax, qword ptr [r12 + 392]
 mov qword ptr [r12 + 472], rax
# marshal arg1 = producer-box slot [r12+432] -> [r12+480]
 mov rax, qword ptr [r12 + 432]
 mov qword ptr [r12 + 480], rax
 mov rax, qword ptr [r12 + 440]
 mov qword ptr [r12 + 488], rax
  .section .rodata
  .Lbynamefn47: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn47]
 lea rsi, [r12 + 464]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 448], rax
 mov qword ptr [r12 + 456], rdx
 cmp eax, 99
 je snoch0_n11_α
 jmp snoch0_n10_α
snoch0_n9_β:
 jmp snoch0_n11_α
snoch0_n10_α:
# IR_LIT_scalar
bb25_α:
 jmp snoch0_n12_α
 snoch0_n10_β:
 jmp flat_γ
snoch0_n11_α:
# IR_LIT_S
bb26_α:
 mov qword ptr [r12 + 496], 1
 mov rax, qword ptr [rip + .Lx50_0]
 mov qword ptr [r12 + 504], rax
 jmp xgvarg49_done
 xgvarg49_β:
 jmp snoch0_n14_α
.Lx50_0:
 .quad .Lx50_0_s
.Lx50_0_s:
 .string "2,2,2,2"
xgvarg49_done:
bb27_α:
# BOX IR_CALL array(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+496] -> [r12+528]
 mov rax, qword ptr [r12 + 496]
 mov qword ptr [r12 + 528], rax
 mov rax, qword ptr [r12 + 504]
 mov qword ptr [r12 + 536], rax
  .section .rodata
  .Lbynamefn52: .string "array"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn52]
 lea rsi, [r12 + 528]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 512], rax
 mov qword ptr [r12 + 520], rdx
 cmp eax, 99
 je snoch0_n14_α
 jmp snoch0_n13_α
snoch0_n11_β:
 jmp snoch0_n14_α
snoch0_n12_α:
bb28_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S4]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n12_β:
 jmp flat_γ
snoch0_n13_α:
bb29_α:
# IR_ASSIGN_CALL gva
 mov rax, qword ptr [r12 + 512]
 mov rcx, qword ptr [r12 + 520]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rcx
 jmp snoch0_n14_α
 snoch0_n13_β:
 jmp snoch0_n14_α
snoch0_n14_α:
# IR_VAR gva
bb30_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 544], rax
 mov qword ptr [r12 + 552], rdx
 jmp xgvarg56_done
 xgvarg56_β:
 jmp snoch0_n16_α
xgvarg56_done:
# IR_LIT_I
bb31_α:
 mov qword ptr [r12 + 560], 6
 mov rax, qword ptr [rip + .Lx60_0]
 mov qword ptr [r12 + 568], rax
 jmp xgvarg59_done
 xgvarg59_β:
 jmp snoch0_n16_α
.Lx60_0:
 .quad 1
xgvarg59_done:
# IR_LIT_I
bb32_α:
 mov qword ptr [r12 + 576], 6
 mov rax, qword ptr [rip + .Lx62_0]
 mov qword ptr [r12 + 584], rax
 jmp xgvarg61_done
 xgvarg61_β:
 jmp snoch0_n16_α
.Lx62_0:
 .quad 2
xgvarg61_done:
# IR_LIT_I
bb33_α:
 mov qword ptr [r12 + 592], 6
 mov rax, qword ptr [rip + .Lx64_0]
 mov qword ptr [r12 + 600], rax
 jmp xgvarg63_done
 xgvarg63_β:
 jmp snoch0_n16_α
.Lx64_0:
 .quad 1
xgvarg63_done:
# IR_LIT_I
bb34_α:
 mov qword ptr [r12 + 608], 6
 mov rax, qword ptr [rip + .Lx66_0]
 mov qword ptr [r12 + 616], rax
 jmp xgvarg65_done
 xgvarg65_β:
 jmp snoch0_n16_α
.Lx66_0:
 .quad 2
xgvarg65_done:
bb35_α:
# BOX IR_CALL item(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+544] -> [r12+640]
 mov rax, qword ptr [r12 + 544]
 mov qword ptr [r12 + 640], rax
 mov rax, qword ptr [r12 + 552]
 mov qword ptr [r12 + 648], rax
# marshal arg1 = producer-box slot [r12+560] -> [r12+656]
 mov rax, qword ptr [r12 + 560]
 mov qword ptr [r12 + 656], rax
 mov rax, qword ptr [r12 + 568]
 mov qword ptr [r12 + 664], rax
# marshal arg2 = producer-box slot [r12+576] -> [r12+672]
 mov rax, qword ptr [r12 + 576]
 mov qword ptr [r12 + 672], rax
 mov rax, qword ptr [r12 + 584]
 mov qword ptr [r12 + 680], rax
# marshal arg3 = producer-box slot [r12+592] -> [r12+688]
 mov rax, qword ptr [r12 + 592]
 mov qword ptr [r12 + 688], rax
 mov rax, qword ptr [r12 + 600]
 mov qword ptr [r12 + 696], rax
# marshal arg4 = producer-box slot [r12+608] -> [r12+704]
 mov rax, qword ptr [r12 + 608]
 mov qword ptr [r12 + 704], rax
 mov rax, qword ptr [r12 + 616]
 mov qword ptr [r12 + 712], rax
  .section .rodata
  .Lbynamefn68: .string "item"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn68]
 lea rsi, [r12 + 640]
 mov edx, 5
 call rt_call_arr@PLT
 mov qword ptr [r12 + 624], rax
 mov qword ptr [r12 + 632], rdx
 cmp eax, 99
 je snoch0_n16_α
 jmp xgvarg55_done
xgvarg55_β:
 jmp snoch0_n16_α
xgvarg55_done:
# IR_LIT_I
bb36_α:
 mov qword ptr [r12 + 720], 6
 mov rax, qword ptr [rip + .Lx70_0]
 mov qword ptr [r12 + 728], rax
 jmp xgvarg69_done
 xgvarg69_β:
 jmp snoch0_n16_α
.Lx70_0:
 .quad 1212
xgvarg69_done:
bb37_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+624] -> [r12+752]
 mov rax, qword ptr [r12 + 624]
 mov qword ptr [r12 + 752], rax
 mov rax, qword ptr [r12 + 632]
 mov qword ptr [r12 + 760], rax
# marshal arg1 = producer-box slot [r12+720] -> [r12+768]
 mov rax, qword ptr [r12 + 720]
 mov qword ptr [r12 + 768], rax
 mov rax, qword ptr [r12 + 728]
 mov qword ptr [r12 + 776], rax
  .section .rodata
  .Lbynamefn72: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn72]
 lea rsi, [r12 + 752]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 736], rax
 mov qword ptr [r12 + 744], rdx
 cmp eax, 99
 je snoch0_n16_α
 jmp snoch0_n15_α
snoch0_n14_β:
 jmp snoch0_n16_α
snoch0_n15_α:
# IR_LIT_scalar
bb38_α:
 jmp snoch0_n17_α
 snoch0_n15_β:
 jmp flat_γ
snoch0_n16_α:
bb39_α:
# IR_IDX: AXS inline DT_A+int fast path, else subscript_get
 lea rdi, [rip + .S5]
 call NV_GET_fn@PLT
 cmp eax, 4
 jne .Lx76_0
 movabs rcx, 1
 mov rsi, rdx
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx76_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx76_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 mov rax, [r11 + rcx*8]
 add r11, 8
 mov rdx, [r11 + rcx*8]
 mov qword ptr [r12 + 800], rax
 mov qword ptr [r12 + 808], rdx
 jmp xgvarg74_done
.Lx76_0:
 mov rdi, rax
 mov rsi, rdx
 movabs rdx, 6
 movabs rcx, 1
 call subscript_get@PLT
 mov qword ptr [r12 + 800], rax
 mov qword ptr [r12 + 808], rdx
 jmp xgvarg74_done
 xgvarg74_β:
 jmp snoch0_n19_α
xgvarg74_done:
# IR_LIT_I
bb40_α:
 mov qword ptr [r12 + 816], 6
 mov rax, qword ptr [rip + .Lx78_0]
 mov qword ptr [r12 + 824], rax
 jmp xgvarg77_done
 xgvarg77_β:
 jmp snoch0_n19_α
.Lx78_0:
 .quad 1212
xgvarg77_done:
bb41_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+800] -> [r12+848]
 mov rax, qword ptr [r12 + 800]
 mov qword ptr [r12 + 848], rax
 mov rax, qword ptr [r12 + 808]
 mov qword ptr [r12 + 856], rax
# marshal arg1 = producer-box slot [r12+816] -> [r12+864]
 mov rax, qword ptr [r12 + 816]
 mov qword ptr [r12 + 864], rax
 mov rax, qword ptr [r12 + 824]
 mov qword ptr [r12 + 872], rax
  .section .rodata
  .Lbynamefn80: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn80]
 lea rsi, [r12 + 848]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 832], rax
 mov qword ptr [r12 + 840], rdx
 cmp eax, 99
 je snoch0_n19_α
 jmp snoch0_n18_α
snoch0_n16_β:
 jmp snoch0_n19_α
snoch0_n17_α:
bb42_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S6]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n17_β:
 jmp flat_γ
snoch0_n18_α:
# IR_LIT_scalar
bb43_α:
 jmp snoch0_n20_α
 snoch0_n18_β:
 jmp flat_γ
snoch0_n19_α:
# IR_LIT_I
bb44_α:
 mov qword ptr [r12 + 880], 6
 mov rax, qword ptr [rip + .Lx84_0]
 mov qword ptr [r12 + 888], rax
 jmp xiset83_val_done
 xiset83_val_β:
 jmp snoch0_n21_α
.Lx84_0:
 .quad 2121
xiset83_val_done:
# IR_LIT_I
bb45_α:
 mov qword ptr [r12 + 896], 6
 mov rax, qword ptr [rip + .Lx85_0]
 mov qword ptr [r12 + 904], rax
 jmp xiset83_key_done
 xiset83_key_β:
 jmp snoch0_n21_α
.Lx85_0:
 .quad 2
xiset83_key_done:
# IR_VAR gva
bb46_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 912], rax
 mov qword ptr [r12 + 920], rdx
 jmp xiset83_base_done
 xiset83_base_β:
 jmp snoch0_n21_α
xiset83_base_done:
bb47_α:
# IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set
 mov rax, qword ptr [r12 + 912]
 cmp eax, 4
 jne .Lx89_0
 mov rax, qword ptr [r12 + 896]
 cmp eax, 6
 jne .Lx89_0
 mov rsi, qword ptr [r12 + 920]
 mov rcx, qword ptr [r12 + 904]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx89_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx89_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 880]
 mov rdx, qword ptr [r12 + 888]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n21_α
.Lx89_0:
 mov rdi, qword ptr [r12 + 912]
 mov rsi, qword ptr [r12 + 920]
 mov rdx, qword ptr [r12 + 896]
 mov rcx, qword ptr [r12 + 904]
 mov r8, qword ptr [r12 + 880]
 mov r9, qword ptr [r12 + 888]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n21_α
 jmp snoch0_n21_α
 snoch0_n19_β:
 jmp snoch0_n21_α
snoch0_n20_α:
bb48_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S7]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n20_β:
 jmp flat_γ
snoch0_n21_α:
# IR_VAR gva
bb49_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 928], rax
 mov qword ptr [r12 + 936], rdx
 jmp xgvarg92_done
 xgvarg92_β:
 jmp snoch0_n23_α
xgvarg92_done:
# IR_LIT_I
bb50_α:
 mov qword ptr [r12 + 944], 6
 mov rax, qword ptr [rip + .Lx96_0]
 mov qword ptr [r12 + 952], rax
 jmp xgvarg95_done
 xgvarg95_β:
 jmp snoch0_n23_α
.Lx96_0:
 .quad 2
xgvarg95_done:
# IR_LIT_I
bb51_α:
 mov qword ptr [r12 + 960], 6
 mov rax, qword ptr [rip + .Lx98_0]
 mov qword ptr [r12 + 968], rax
 jmp xgvarg97_done
 xgvarg97_β:
 jmp snoch0_n23_α
.Lx98_0:
 .quad 1
xgvarg97_done:
# IR_LIT_I
bb52_α:
 mov qword ptr [r12 + 976], 6
 mov rax, qword ptr [rip + .Lx100_0]
 mov qword ptr [r12 + 984], rax
 jmp xgvarg99_done
 xgvarg99_β:
 jmp snoch0_n23_α
.Lx100_0:
 .quad 2
xgvarg99_done:
# IR_LIT_I
bb53_α:
 mov qword ptr [r12 + 992], 6
 mov rax, qword ptr [rip + .Lx102_0]
 mov qword ptr [r12 + 1000], rax
 jmp xgvarg101_done
 xgvarg101_β:
 jmp snoch0_n23_α
.Lx102_0:
 .quad 1
xgvarg101_done:
bb54_α:
# BOX IR_CALL item(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+928] -> [r12+1024]
 mov rax, qword ptr [r12 + 928]
 mov qword ptr [r12 + 1024], rax
 mov rax, qword ptr [r12 + 936]
 mov qword ptr [r12 + 1032], rax
# marshal arg1 = producer-box slot [r12+944] -> [r12+1040]
 mov rax, qword ptr [r12 + 944]
 mov qword ptr [r12 + 1040], rax
 mov rax, qword ptr [r12 + 952]
 mov qword ptr [r12 + 1048], rax
# marshal arg2 = producer-box slot [r12+960] -> [r12+1056]
 mov rax, qword ptr [r12 + 960]
 mov qword ptr [r12 + 1056], rax
 mov rax, qword ptr [r12 + 968]
 mov qword ptr [r12 + 1064], rax
# marshal arg3 = producer-box slot [r12+976] -> [r12+1072]
 mov rax, qword ptr [r12 + 976]
 mov qword ptr [r12 + 1072], rax
 mov rax, qword ptr [r12 + 984]
 mov qword ptr [r12 + 1080], rax
# marshal arg4 = producer-box slot [r12+992] -> [r12+1088]
 mov rax, qword ptr [r12 + 992]
 mov qword ptr [r12 + 1088], rax
 mov rax, qword ptr [r12 + 1000]
 mov qword ptr [r12 + 1096], rax
  .section .rodata
  .Lbynamefn104: .string "item"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn104]
 lea rsi, [r12 + 1024]
 mov edx, 5
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1008], rax
 mov qword ptr [r12 + 1016], rdx
 cmp eax, 99
 je snoch0_n23_α
 jmp xgvarg91_done
xgvarg91_β:
 jmp snoch0_n23_α
xgvarg91_done:
# IR_LIT_I
bb55_α:
 mov qword ptr [r12 + 1104], 6
 mov rax, qword ptr [rip + .Lx106_0]
 mov qword ptr [r12 + 1112], rax
 jmp xgvarg105_done
 xgvarg105_β:
 jmp snoch0_n23_α
.Lx106_0:
 .quad 2121
xgvarg105_done:
bb56_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+1008] -> [r12+1136]
 mov rax, qword ptr [r12 + 1008]
 mov qword ptr [r12 + 1136], rax
 mov rax, qword ptr [r12 + 1016]
 mov qword ptr [r12 + 1144], rax
# marshal arg1 = producer-box slot [r12+1104] -> [r12+1152]
 mov rax, qword ptr [r12 + 1104]
 mov qword ptr [r12 + 1152], rax
 mov rax, qword ptr [r12 + 1112]
 mov qword ptr [r12 + 1160], rax
  .section .rodata
  .Lbynamefn108: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn108]
 lea rsi, [r12 + 1136]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1120], rax
 mov qword ptr [r12 + 1128], rdx
 cmp eax, 99
 je snoch0_n23_α
 jmp snoch0_n22_α
snoch0_n21_β:
 jmp snoch0_n23_α
snoch0_n22_α:
# IR_LIT_scalar
bb57_α:
 jmp snoch0_n24_α
 snoch0_n22_β:
 jmp flat_γ
snoch0_n23_α:
bb58_α:
# BOX IR_CALL table(...) -> rt_call_arr [operand-marshal, FAIL->ω]
  .section .rodata
  .Lrkfn111: .string "table"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn111]
 lea rsi, [r12 + 1184]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1168], rax
 mov qword ptr [r12 + 1176], rdx
 cmp eax, 99
 je snoch0_n26_α
 jmp snoch0_n25_α
 snoch0_n23_β:
 jmp snoch0_n26_α
snoch0_n24_α:
bb59_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S8]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n24_β:
 jmp flat_γ
snoch0_n25_α:
bb60_α:
# IR_ASSIGN_CALL gva
 mov rax, qword ptr [r12 + 1168]
 mov rcx, qword ptr [r12 + 1176]
 mov qword ptr [rbx + 48], rax
 mov qword ptr [rbx + 56], rcx
 jmp snoch0_n26_α
 snoch0_n25_β:
 jmp snoch0_n26_α
snoch0_n26_α:
# IR_VAR gva
bb61_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 1184], rax
 mov qword ptr [r12 + 1192], rdx
 jmp xgvarg115_done
 xgvarg115_β:
 jmp snoch0_n28_α
xgvarg115_done:
# IR_LIT_S
bb62_α:
 mov qword ptr [r12 + 1200], 1
 mov rax, qword ptr [rip + .Lx119_0]
 mov qword ptr [r12 + 1208], rax
 jmp xgvarg118_done
 xgvarg118_β:
 jmp snoch0_n28_α
.Lx119_0:
 .quad .Lx119_0_s
.Lx119_0_s:
 .string "key"
xgvarg118_done:
bb63_α:
# BOX IR_CALL item(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+1184] -> [r12+1232]
 mov rax, qword ptr [r12 + 1184]
 mov qword ptr [r12 + 1232], rax
 mov rax, qword ptr [r12 + 1192]
 mov qword ptr [r12 + 1240], rax
# marshal arg1 = producer-box slot [r12+1200] -> [r12+1248]
 mov rax, qword ptr [r12 + 1200]
 mov qword ptr [r12 + 1248], rax
 mov rax, qword ptr [r12 + 1208]
 mov qword ptr [r12 + 1256], rax
  .section .rodata
  .Lbynamefn121: .string "item"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn121]
 lea rsi, [r12 + 1232]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1216], rax
 mov qword ptr [r12 + 1224], rdx
 cmp eax, 99
 je snoch0_n28_α
 jmp xgvarg114_done
xgvarg114_β:
 jmp snoch0_n28_α
xgvarg114_done:
# IR_LIT_S
bb64_α:
 mov qword ptr [r12 + 1264], 1
 mov rax, qword ptr [rip + .Lx123_0]
 mov qword ptr [r12 + 1272], rax
 jmp xgvarg122_done
 xgvarg122_β:
 jmp snoch0_n28_α
.Lx123_0:
 .quad .Lx123_0_s
.Lx123_0_s:
 .string "val"
xgvarg122_done:
bb65_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+1216] -> [r12+1296]
 mov rax, qword ptr [r12 + 1216]
 mov qword ptr [r12 + 1296], rax
 mov rax, qword ptr [r12 + 1224]
 mov qword ptr [r12 + 1304], rax
# marshal arg1 = producer-box slot [r12+1264] -> [r12+1312]
 mov rax, qword ptr [r12 + 1264]
 mov qword ptr [r12 + 1312], rax
 mov rax, qword ptr [r12 + 1272]
 mov qword ptr [r12 + 1320], rax
  .section .rodata
  .Lbynamefn125: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn125]
 lea rsi, [r12 + 1296]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1280], rax
 mov qword ptr [r12 + 1288], rdx
 cmp eax, 99
 je snoch0_n28_α
 jmp snoch0_n27_α
snoch0_n26_β:
 jmp snoch0_n28_α
snoch0_n27_α:
# IR_LIT_scalar
bb66_α:
 jmp snoch0_n29_α
 snoch0_n27_β:
 jmp flat_γ
snoch0_n28_α:
# IR_LIT_scalar
bb67_α:
 jmp snoch0_n30_α
 snoch0_n28_β:
 jmp flat_γ
snoch0_n29_α:
bb68_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S10]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n29_β:
 jmp flat_γ
snoch0_n30_α:
bb69_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S11]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n30_β:
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
.S0: .string "aaa"
.S1: .string "output"
.S2: .string "FAIL 1114/001: item 1D assign/read"
.S3: .string "FAIL 1114/002: item == bracket read"
.S4: .string "FAIL 1114/003: bracket assign, item read"
.S5: .string "ama"
.S6: .string "FAIL 1114/004: item 4D assign/read"
.S7: .string "FAIL 1114/005: item 4D == bracket"
.S8: .string "FAIL 1114/006: bracket 4D assign, item read"
.S9: .string "tt"
.S10: .string "FAIL 1114/007: item on table"
.S11: .string "PASS 1114_item (7/7)"
.text

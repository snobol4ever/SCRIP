  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "t"
  .Lgvan1: .string "output"
  .Lgvan2: .string "ta"
  .Lgvan3: .string "ata"
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
# BOX IR_CALL table(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+16] -> [r12+48]
 mov rax, qword ptr [r12 + 16]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 24]
 mov qword ptr [r12 + 56], rax
  .section .rodata
  .Lrkfn4: .string "table"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn4]
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
bb4_α:
# IR_IDX: string-literal key — table-only, subscript_get
 lea rdi, [rip + .S0]
 call NV_GET_fn@PLT
 mov rdi, rax
 mov rsi, rdx
 movabs rdx, 1
 lea rcx, [rip + .S1]
 call subscript_get@PLT
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 jmp xgvarg6_done
 xgvarg6_β:
 jmp snoch0_n4_α
xgvarg6_done:
bb5_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+80] -> [r12+112]
 mov rax, qword ptr [r12 + 80]
 mov qword ptr [r12 + 112], rax
 mov rax, qword ptr [r12 + 88]
 mov qword ptr [r12 + 120], rax
  .section .rodata
  .Lbynamefn10: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn10]
 lea rsi, [r12 + 112]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 cmp eax, 99
 je snoch0_n4_α
 jmp snoch0_n3_α
snoch0_n2_β:
 jmp snoch0_n4_α
snoch0_n3_α:
# IR_LIT_scalar
bb6_α:
 jmp snoch0_n5_α
 snoch0_n3_β:
 jmp flat_γ
snoch0_n4_α:
# IR_LIT_S
bb7_α:
 mov qword ptr [r12 + 128], 1
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [r12 + 136], rax
 jmp xiset12_val_done
 xiset12_val_β:
 jmp snoch0_n6_α
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "dog"
xiset12_val_done:
# IR_LIT_S
bb8_α:
 mov qword ptr [r12 + 144], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [r12 + 152], rax
 jmp xiset12_key_done
 xiset12_key_β:
 jmp snoch0_n6_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "cat"
xiset12_key_done:
# IR_VAR gva
bb9_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 jmp xiset12_base_done
 xiset12_base_β:
 jmp snoch0_n6_α
xiset12_base_done:
bb10_α:
# IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set
 mov rax, qword ptr [r12 + 160]
 cmp eax, 4
 jne .Lx18_0
 mov rax, qword ptr [r12 + 144]
 cmp eax, 6
 jne .Lx18_0
 mov rsi, qword ptr [r12 + 168]
 mov rcx, qword ptr [r12 + 152]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx18_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx18_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 128]
 mov rdx, qword ptr [r12 + 136]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n6_α
.Lx18_0:
 mov rdi, qword ptr [r12 + 160]
 mov rsi, qword ptr [r12 + 168]
 mov rdx, qword ptr [r12 + 144]
 mov rcx, qword ptr [r12 + 152]
 mov r8, qword ptr [r12 + 128]
 mov r9, qword ptr [r12 + 136]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n6_α
 jmp snoch0_n6_α
 snoch0_n4_β:
 jmp snoch0_n6_α
snoch0_n5_α:
bb11_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S2]
 lea rsi, [rip + .S3]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n5_β:
 jmp flat_γ
snoch0_n6_α:
bb12_α:
# IR_IDX: string-literal key — table-only, subscript_get
 lea rdi, [rip + .S0]
 call NV_GET_fn@PLT
 mov rdi, rax
 mov rsi, rdx
 movabs rdx, 1
 lea rcx, [rip + .S1]
 call subscript_get@PLT
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 jmp xgvarg20_done
 xgvarg20_β:
 jmp snoch0_n8_α
xgvarg20_done:
# IR_LIT_S
bb13_α:
 mov qword ptr [r12 + 208], 1
 mov rax, qword ptr [rip + .Lx24_0]
 mov qword ptr [r12 + 216], rax
 jmp xgvarg23_done
 xgvarg23_β:
 jmp snoch0_n8_α
.Lx24_0:
 .quad .Lx24_0_s
.Lx24_0_s:
 .string "dog"
xgvarg23_done:
bb14_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+192] -> [r12+240]
 mov rax, qword ptr [r12 + 192]
 mov qword ptr [r12 + 240], rax
 mov rax, qword ptr [r12 + 200]
 mov qword ptr [r12 + 248], rax
# marshal arg1 = producer-box slot [r12+208] -> [r12+256]
 mov rax, qword ptr [r12 + 208]
 mov qword ptr [r12 + 256], rax
 mov rax, qword ptr [r12 + 216]
 mov qword ptr [r12 + 264], rax
  .section .rodata
  .Lbynamefn26: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn26]
 lea rsi, [r12 + 240]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 cmp eax, 99
 je snoch0_n8_α
 jmp snoch0_n7_α
snoch0_n6_β:
 jmp snoch0_n8_α
snoch0_n7_α:
# IR_LIT_scalar
bb15_α:
 jmp snoch0_n9_α
 snoch0_n7_β:
 jmp flat_γ
snoch0_n8_α:
# IR_LIT_I
bb16_α:
 mov qword ptr [r12 + 272], 6
 mov rax, qword ptr [rip + .Lx29_0]
 mov qword ptr [r12 + 280], rax
 jmp xiset28_val_done
 xiset28_val_β:
 jmp snoch0_n10_α
.Lx29_0:
 .quad 45
xiset28_val_done:
# IR_LIT_I
bb17_α:
 mov qword ptr [r12 + 288], 6
 mov rax, qword ptr [rip + .Lx30_0]
 mov qword ptr [r12 + 296], rax
 jmp xiset28_key_done
 xiset28_key_β:
 jmp snoch0_n10_α
.Lx30_0:
 .quad 7
xiset28_key_done:
# IR_VAR gva
bb18_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 304], rax
 mov qword ptr [r12 + 312], rdx
 jmp xiset28_base_done
 xiset28_base_β:
 jmp snoch0_n10_α
xiset28_base_done:
bb19_α:
# IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set
 mov rax, qword ptr [r12 + 304]
 cmp eax, 4
 jne .Lx34_0
 mov rax, qword ptr [r12 + 288]
 cmp eax, 6
 jne .Lx34_0
 mov rsi, qword ptr [r12 + 312]
 mov rcx, qword ptr [r12 + 296]
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
 mov rax, qword ptr [r12 + 272]
 mov rdx, qword ptr [r12 + 280]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n10_α
.Lx34_0:
 mov rdi, qword ptr [r12 + 304]
 mov rsi, qword ptr [r12 + 312]
 mov rdx, qword ptr [r12 + 288]
 mov rcx, qword ptr [r12 + 296]
 mov r8, qword ptr [r12 + 272]
 mov r9, qword ptr [r12 + 280]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n10_α
 jmp snoch0_n10_α
 snoch0_n8_β:
 jmp snoch0_n10_α
snoch0_n9_α:
bb20_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S2]
 lea rsi, [rip + .S4]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n9_β:
 jmp flat_γ
snoch0_n10_α:
bb21_α:
# IR_IDX: AXS inline DT_A+int fast path, else subscript_get
 lea rdi, [rip + .S0]
 call NV_GET_fn@PLT
 cmp eax, 4
 jne .Lx38_0
 movabs rcx, 7
 mov rsi, rdx
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx38_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx38_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 mov rax, [r11 + rcx*8]
 add r11, 8
 mov rdx, [r11 + rcx*8]
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 jmp xgvarg36_done
.Lx38_0:
 mov rdi, rax
 mov rsi, rdx
 movabs rdx, 6
 movabs rcx, 7
 call subscript_get@PLT
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 jmp xgvarg36_done
 xgvarg36_β:
 jmp snoch0_n12_α
xgvarg36_done:
# IR_LIT_I
bb22_α:
 mov qword ptr [r12 + 352], 6
 mov rax, qword ptr [rip + .Lx40_0]
 mov qword ptr [r12 + 360], rax
 jmp xgvarg39_done
 xgvarg39_β:
 jmp snoch0_n12_α
.Lx40_0:
 .quad 45
xgvarg39_done:
bb23_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
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
  .Lbynamefn42: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn42]
 lea rsi, [r12 + 384]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 368], rax
 mov qword ptr [r12 + 376], rdx
 cmp eax, 99
 je snoch0_n12_α
 jmp snoch0_n11_α
snoch0_n10_β:
 jmp snoch0_n12_α
snoch0_n11_α:
# IR_LIT_scalar
bb24_α:
 jmp snoch0_n13_α
 snoch0_n11_β:
 jmp flat_γ
snoch0_n12_α:
bb25_α:
# IR_IDX: string-literal key — table-only, subscript_get
 lea rdi, [rip + .S0]
 call NV_GET_fn@PLT
 mov rdi, rax
 mov rsi, rdx
 movabs rdx, 1
 lea rcx, [rip + .S1]
 call subscript_get@PLT
 mov qword ptr [r12 + 432], rax
 mov qword ptr [r12 + 440], rdx
 jmp xgvarg44_done
 xgvarg44_β:
 jmp snoch0_n15_α
xgvarg44_done:
# IR_LIT_S
bb26_α:
 mov qword ptr [r12 + 448], 1
 mov rax, qword ptr [rip + .Lx48_0]
 mov qword ptr [r12 + 456], rax
 jmp xgvarg47_done
 xgvarg47_β:
 jmp snoch0_n15_α
.Lx48_0:
 .quad .Lx48_0_s
.Lx48_0_s:
 .string "dog"
xgvarg47_done:
bb27_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+432] -> [r12+480]
 mov rax, qword ptr [r12 + 432]
 mov qword ptr [r12 + 480], rax
 mov rax, qword ptr [r12 + 440]
 mov qword ptr [r12 + 488], rax
# marshal arg1 = producer-box slot [r12+448] -> [r12+496]
 mov rax, qword ptr [r12 + 448]
 mov qword ptr [r12 + 496], rax
 mov rax, qword ptr [r12 + 456]
 mov qword ptr [r12 + 504], rax
  .section .rodata
  .Lbynamefn50: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn50]
 lea rsi, [r12 + 480]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 464], rax
 mov qword ptr [r12 + 472], rdx
 cmp eax, 99
 je snoch0_n15_α
 jmp snoch0_n14_α
snoch0_n12_β:
 jmp snoch0_n15_α
snoch0_n13_α:
bb28_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S2]
 lea rsi, [rip + .S5]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n13_β:
 jmp flat_γ
snoch0_n14_α:
# IR_LIT_scalar
bb29_α:
 jmp snoch0_n16_α
 snoch0_n14_β:
 jmp flat_γ
snoch0_n15_α:
# IR_VAR gva
bb30_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 512], rax
 mov qword ptr [r12 + 520], rdx
 jmp xgvarg53_done
 xgvarg53_β:
 jmp snoch0_n18_α
xgvarg53_done:
# IR_LIT_S
bb31_α:
 mov qword ptr [r12 + 528], 1
 mov rax, qword ptr [rip + .Lx57_0]
 mov qword ptr [r12 + 536], rax
 jmp xgvarg56_done
 xgvarg56_β:
 jmp snoch0_n18_α
.Lx57_0:
 .quad .Lx57_0_s
.Lx57_0_s:
 .string "array"
xgvarg56_done:
bb32_α:
# BOX IR_CALL convert(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+512] -> [r12+560]
 mov rax, qword ptr [r12 + 512]
 mov qword ptr [r12 + 560], rax
 mov rax, qword ptr [r12 + 520]
 mov qword ptr [r12 + 568], rax
# marshal arg1 = producer-box slot [r12+528] -> [r12+576]
 mov rax, qword ptr [r12 + 528]
 mov qword ptr [r12 + 576], rax
 mov rax, qword ptr [r12 + 536]
 mov qword ptr [r12 + 584], rax
  .section .rodata
  .Lbynamefn59: .string "convert"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn59]
 lea rsi, [r12 + 560]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 544], rax
 mov qword ptr [r12 + 552], rdx
 cmp eax, 99
 je snoch0_n18_α
 jmp snoch0_n17_α
snoch0_n15_β:
 jmp snoch0_n18_α
snoch0_n16_α:
bb33_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S2]
 lea rsi, [rip + .S6]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n16_β:
 jmp flat_γ
snoch0_n17_α:
bb34_α:
# IR_ASSIGN_CALL gva
 mov rax, qword ptr [r12 + 544]
 mov rcx, qword ptr [r12 + 552]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rcx
 jmp snoch0_n18_α
 snoch0_n17_β:
 jmp snoch0_n18_α
snoch0_n18_α:
# IR_VAR gva
bb35_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 592], rax
 mov qword ptr [r12 + 600], rdx
 jmp xgvarg63_done
 xgvarg63_β:
 jmp snoch0_n20_α
xgvarg63_done:
bb36_α:
# BOX IR_CALL prototype(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+592] -> [r12+624]
 mov rax, qword ptr [r12 + 592]
 mov qword ptr [r12 + 624], rax
 mov rax, qword ptr [r12 + 600]
 mov qword ptr [r12 + 632], rax
  .section .rodata
  .Lbynamefn67: .string "prototype"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn67]
 lea rsi, [r12 + 624]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 608], rax
 mov qword ptr [r12 + 616], rdx
 cmp eax, 99
 je snoch0_n20_α
 jmp xgvarg62_done
xgvarg62_β:
 jmp snoch0_n20_α
xgvarg62_done:
# IR_LIT_S
bb37_α:
 mov qword ptr [r12 + 640], 1
 mov rax, qword ptr [rip + .Lx69_0]
 mov qword ptr [r12 + 648], rax
 jmp xgvarg68_done
 xgvarg68_β:
 jmp snoch0_n20_α
.Lx69_0:
 .quad .Lx69_0_s
.Lx69_0_s:
 .string "2,2"
xgvarg68_done:
bb38_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+608] -> [r12+672]
 mov rax, qword ptr [r12 + 608]
 mov qword ptr [r12 + 672], rax
 mov rax, qword ptr [r12 + 616]
 mov qword ptr [r12 + 680], rax
# marshal arg1 = producer-box slot [r12+640] -> [r12+688]
 mov rax, qword ptr [r12 + 640]
 mov qword ptr [r12 + 688], rax
 mov rax, qword ptr [r12 + 648]
 mov qword ptr [r12 + 696], rax
  .section .rodata
  .Lbynamefn71: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn71]
 lea rsi, [r12 + 672]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 656], rax
 mov qword ptr [r12 + 664], rdx
 cmp eax, 99
 je snoch0_n20_α
 jmp snoch0_n19_α
snoch0_n18_β:
 jmp snoch0_n20_α
snoch0_n19_α:
# IR_LIT_scalar
bb39_α:
 jmp snoch0_n21_α
 snoch0_n19_β:
 jmp flat_γ
snoch0_n20_α:
# IR_VAR gva
bb40_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 704], rax
 mov qword ptr [r12 + 712], rdx
 jmp xgvarg73_done
 xgvarg73_β:
 jmp snoch0_n23_α
xgvarg73_done:
# IR_LIT_S
bb41_α:
 mov qword ptr [r12 + 720], 1
 mov rax, qword ptr [rip + .Lx77_0]
 mov qword ptr [r12 + 728], rax
 jmp xgvarg76_done
 xgvarg76_β:
 jmp snoch0_n23_α
.Lx77_0:
 .quad .Lx77_0_s
.Lx77_0_s:
 .string "table"
xgvarg76_done:
bb42_α:
# BOX IR_CALL convert(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+704] -> [r12+752]
 mov rax, qword ptr [r12 + 704]
 mov qword ptr [r12 + 752], rax
 mov rax, qword ptr [r12 + 712]
 mov qword ptr [r12 + 760], rax
# marshal arg1 = producer-box slot [r12+720] -> [r12+768]
 mov rax, qword ptr [r12 + 720]
 mov qword ptr [r12 + 768], rax
 mov rax, qword ptr [r12 + 728]
 mov qword ptr [r12 + 776], rax
  .section .rodata
  .Lbynamefn79: .string "convert"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn79]
 lea rsi, [r12 + 752]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 736], rax
 mov qword ptr [r12 + 744], rdx
 cmp eax, 99
 je snoch0_n23_α
 jmp snoch0_n22_α
snoch0_n20_β:
 jmp snoch0_n23_α
snoch0_n21_α:
bb43_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S2]
 lea rsi, [rip + .S8]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n21_β:
 jmp flat_γ
snoch0_n22_α:
bb44_α:
# IR_ASSIGN_CALL gva
 mov rax, qword ptr [r12 + 736]
 mov rcx, qword ptr [r12 + 744]
 mov qword ptr [rbx + 48], rax
 mov qword ptr [rbx + 56], rcx
 jmp snoch0_n23_α
 snoch0_n22_β:
 jmp snoch0_n23_α
snoch0_n23_α:
bb45_α:
# IR_IDX: AXS inline DT_A+int fast path, else subscript_get
 lea rdi, [rip + .S9]
 call NV_GET_fn@PLT
 cmp eax, 4
 jne .Lx84_0
 movabs rcx, 7
 mov rsi, rdx
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx84_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx84_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 mov rax, [r11 + rcx*8]
 add r11, 8
 mov rdx, [r11 + rcx*8]
 mov qword ptr [r12 + 800], rax
 mov qword ptr [r12 + 808], rdx
 jmp xgvarg82_done
.Lx84_0:
 mov rdi, rax
 mov rsi, rdx
 movabs rdx, 6
 movabs rcx, 7
 call subscript_get@PLT
 mov qword ptr [r12 + 800], rax
 mov qword ptr [r12 + 808], rdx
 jmp xgvarg82_done
 xgvarg82_β:
 jmp snoch0_n25_α
xgvarg82_done:
# IR_LIT_I
bb46_α:
 mov qword ptr [r12 + 816], 6
 mov rax, qword ptr [rip + .Lx86_0]
 mov qword ptr [r12 + 824], rax
 jmp xgvarg85_done
 xgvarg85_β:
 jmp snoch0_n25_α
.Lx86_0:
 .quad 45
xgvarg85_done:
bb47_α:
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
  .Lbynamefn88: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn88]
 lea rsi, [r12 + 848]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 832], rax
 mov qword ptr [r12 + 840], rdx
 cmp eax, 99
 je snoch0_n25_α
 jmp snoch0_n24_α
snoch0_n23_β:
 jmp snoch0_n25_α
snoch0_n24_α:
# IR_LIT_scalar
bb48_α:
 jmp snoch0_n26_α
 snoch0_n24_β:
 jmp flat_γ
snoch0_n25_α:
bb49_α:
# IR_IDX: string-literal key — table-only, subscript_get
 lea rdi, [rip + .S9]
 call NV_GET_fn@PLT
 mov rdi, rax
 mov rsi, rdx
 movabs rdx, 1
 lea rcx, [rip + .S1]
 call subscript_get@PLT
 mov qword ptr [r12 + 896], rax
 mov qword ptr [r12 + 904], rdx
 jmp xgvarg90_done
 xgvarg90_β:
 jmp snoch0_n28_α
xgvarg90_done:
# IR_LIT_S
bb50_α:
 mov qword ptr [r12 + 912], 1
 mov rax, qword ptr [rip + .Lx94_0]
 mov qword ptr [r12 + 920], rax
 jmp xgvarg93_done
 xgvarg93_β:
 jmp snoch0_n28_α
.Lx94_0:
 .quad .Lx94_0_s
.Lx94_0_s:
 .string "dog"
xgvarg93_done:
bb51_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+896] -> [r12+944]
 mov rax, qword ptr [r12 + 896]
 mov qword ptr [r12 + 944], rax
 mov rax, qword ptr [r12 + 904]
 mov qword ptr [r12 + 952], rax
# marshal arg1 = producer-box slot [r12+912] -> [r12+960]
 mov rax, qword ptr [r12 + 912]
 mov qword ptr [r12 + 960], rax
 mov rax, qword ptr [r12 + 920]
 mov qword ptr [r12 + 968], rax
  .section .rodata
  .Lbynamefn96: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn96]
 lea rsi, [r12 + 944]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 928], rax
 mov qword ptr [r12 + 936], rdx
 cmp eax, 99
 je snoch0_n28_α
 jmp snoch0_n27_α
snoch0_n25_β:
 jmp snoch0_n28_α
snoch0_n26_α:
bb52_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S2]
 lea rsi, [rip + .S10]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n26_β:
 jmp flat_γ
snoch0_n27_α:
# IR_LIT_scalar
bb53_α:
 jmp snoch0_n29_α
 snoch0_n27_β:
 jmp flat_γ
snoch0_n28_α:
# IR_LIT_S
bb54_α:
 mov qword ptr [r12 + 976], 1
 mov rax, qword ptr [rip + .Lx100_0]
 mov qword ptr [r12 + 984], rax
 jmp xiset99_val_done
 xiset99_val_β:
 jmp snoch0_n30_α
.Lx100_0:
 .quad .Lx100_0_s
.Lx100_0_s:
 .string "fish"
xiset99_val_done:
# IR_LIT_S
bb55_α:
 mov qword ptr [r12 + 992], 1
 mov rax, qword ptr [rip + .Lx101_0]
 mov qword ptr [r12 + 1000], rax
 jmp xiset99_key_done
 xiset99_key_β:
 jmp snoch0_n30_α
.Lx101_0:
 .quad .Lx101_0_s
.Lx101_0_s:
 .string "cat"
xiset99_key_done:
# IR_VAR gva
bb56_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 1008], rax
 mov qword ptr [r12 + 1016], rdx
 jmp xiset99_base_done
 xiset99_base_β:
 jmp snoch0_n30_α
xiset99_base_done:
bb57_α:
# IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set
 mov rax, qword ptr [r12 + 1008]
 cmp eax, 4
 jne .Lx105_0
 mov rax, qword ptr [r12 + 992]
 cmp eax, 6
 jne .Lx105_0
 mov rsi, qword ptr [r12 + 1016]
 mov rcx, qword ptr [r12 + 1000]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx105_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx105_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 976]
 mov rdx, qword ptr [r12 + 984]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n30_α
.Lx105_0:
 mov rdi, qword ptr [r12 + 1008]
 mov rsi, qword ptr [r12 + 1016]
 mov rdx, qword ptr [r12 + 992]
 mov rcx, qword ptr [r12 + 1000]
 mov r8, qword ptr [r12 + 976]
 mov r9, qword ptr [r12 + 984]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n30_α
 jmp snoch0_n30_α
 snoch0_n28_β:
 jmp snoch0_n30_α
snoch0_n29_α:
bb58_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S2]
 lea rsi, [rip + .S11]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n29_β:
 jmp flat_γ
snoch0_n30_α:
bb59_α:
# IR_IDX: string-literal key — table-only, subscript_get
 lea rdi, [rip + .S0]
 call NV_GET_fn@PLT
 mov rdi, rax
 mov rsi, rdx
 movabs rdx, 1
 lea rcx, [rip + .S1]
 call subscript_get@PLT
 mov qword ptr [r12 + 1040], rax
 mov qword ptr [r12 + 1048], rdx
 jmp xgvarg107_done
 xgvarg107_β:
 jmp snoch0_n32_α
xgvarg107_done:
# IR_LIT_S
bb60_α:
 mov qword ptr [r12 + 1056], 1
 mov rax, qword ptr [rip + .Lx111_0]
 mov qword ptr [r12 + 1064], rax
 jmp xgvarg110_done
 xgvarg110_β:
 jmp snoch0_n32_α
.Lx111_0:
 .quad .Lx111_0_s
.Lx111_0_s:
 .string "fish"
xgvarg110_done:
bb61_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+1040] -> [r12+1088]
 mov rax, qword ptr [r12 + 1040]
 mov qword ptr [r12 + 1088], rax
 mov rax, qword ptr [r12 + 1048]
 mov qword ptr [r12 + 1096], rax
# marshal arg1 = producer-box slot [r12+1056] -> [r12+1104]
 mov rax, qword ptr [r12 + 1056]
 mov qword ptr [r12 + 1104], rax
 mov rax, qword ptr [r12 + 1064]
 mov qword ptr [r12 + 1112], rax
  .section .rodata
  .Lbynamefn113: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn113]
 lea rsi, [r12 + 1088]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1072], rax
 mov qword ptr [r12 + 1080], rdx
 cmp eax, 99
 je snoch0_n32_α
 jmp snoch0_n31_α
snoch0_n30_β:
 jmp snoch0_n32_α
snoch0_n31_α:
# IR_LIT_scalar
bb62_α:
 jmp snoch0_n33_α
 snoch0_n31_β:
 jmp flat_γ
snoch0_n32_α:
# IR_LIT_scalar
bb63_α:
 jmp snoch0_n34_α
 snoch0_n32_β:
 jmp flat_γ
snoch0_n33_α:
bb64_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S2]
 lea rsi, [rip + .S12]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n33_β:
 jmp flat_γ
snoch0_n34_α:
bb65_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S2]
 lea rsi, [rip + .S13]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n34_β:
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
.S0: .string "t"
.S1: .string "cat"
.S2: .string "output"
.S3: .string "FAIL 1113/001: absent key is null"
.S4: .string "FAIL 1113/002: string key assign/read"
.S5: .string "FAIL 1113/003: integer key assign/read"
.S6: .string "FAIL 1113/004: string key survives int key add"
.S7: .string "ta"
.S8: .string "FAIL 1113/005: table->array prototype 2,2"
.S9: .string "ata"
.S10: .string "FAIL 1113/006: array->table int key roundtrip"
.S11: .string "FAIL 1113/007: array->table string key roundtrip"
.S12: .string "FAIL 1113/008: [] and <> syntax equivalent"
.S13: .string "PASS 1113_table (8/8)"
.text

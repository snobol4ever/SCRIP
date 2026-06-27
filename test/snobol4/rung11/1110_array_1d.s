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
# IR_LIT_I
bb1_α:
 mov qword ptr [r12 + 16], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 24], rax
 jmp xgvarg1_done
 xgvarg1_β:
 jmp snoch0_n2_α
.Lx2_0:
 .quad 3
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
bb4_α:
# IR_IDX: AXS inline DT_A+int fast path, else subscript_get
 lea rdi, [rip + .S0]
 call NV_GET_fn@PLT
 cmp eax, 4
 jne .Lx8_0
 movabs rcx, 1
 mov rsi, rdx
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx8_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx8_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 mov rax, [r11 + rcx*8]
 add r11, 8
 mov rdx, [r11 + rcx*8]
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 jmp xgvarg6_done
.Lx8_0:
 mov rdi, rax
 mov rsi, rdx
 movabs rdx, 6
 movabs rcx, 1
 call subscript_get@PLT
 cmp eax, 99
 je snoch0_n4_α
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
# IR_LIT_F
bb7_α:
 mov qword ptr [r12 + 128], 7
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [r12 + 136], rax
 jmp xiset12_val_done
 xiset12_val_β:
 jmp snoch0_n6_α
.Lx13_0:
 .quad 4616752568008179712
xiset12_val_done:
# IR_LIT_I
bb8_α:
 mov qword ptr [r12 + 144], 6
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [r12 + 152], rax
 jmp xiset12_key_done
 xiset12_key_β:
 jmp snoch0_n6_α
.Lx14_0:
 .quad 2
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
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S2]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n5_β:
 jmp flat_γ
snoch0_n6_α:
bb12_α:
# IR_IDX: AXS inline DT_A+int fast path, else subscript_get
 lea rdi, [rip + .S0]
 call NV_GET_fn@PLT
 cmp eax, 4
 jne .Lx22_0
 movabs rcx, 2
 mov rsi, rdx
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx22_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx22_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 mov rax, [r11 + rcx*8]
 add r11, 8
 mov rdx, [r11 + rcx*8]
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 jmp xgvarg20_done
.Lx22_0:
 mov rdi, rax
 mov rsi, rdx
 movabs rdx, 6
 movabs rcx, 2
 call subscript_get@PLT
 cmp eax, 99
 je snoch0_n8_α
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 jmp xgvarg20_done
 xgvarg20_β:
 jmp snoch0_n8_α
xgvarg20_done:
# IR_LIT_F
bb13_α:
 mov qword ptr [r12 + 208], 7
 mov rax, qword ptr [rip + .Lx24_0]
 mov qword ptr [r12 + 216], rax
 jmp xgvarg23_done
 xgvarg23_β:
 jmp snoch0_n8_α
.Lx24_0:
 .quad 4616752568008179712
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
bb16_α:
# IR_IDX: AXS inline DT_A+int fast path, else subscript_get
 lea rdi, [rip + .S0]
 call NV_GET_fn@PLT
 cmp eax, 4
 jne .Lx29_0
 movabs rcx, 4
 mov rsi, rdx
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx29_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx29_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 mov rax, [r11 + rcx*8]
 add r11, 8
 mov rdx, [r11 + rcx*8]
 mov qword ptr [r12 + 288], rax
 mov qword ptr [r12 + 296], rdx
 jmp snoch0_n10_α
.Lx29_0:
 mov rdi, rax
 mov rsi, rdx
 movabs rdx, 6
 movabs rcx, 4
 call subscript_get@PLT
 cmp eax, 99
 je snoch0_n11_α
 mov qword ptr [r12 + 288], rax
 mov qword ptr [r12 + 296], rdx
 jmp snoch0_n10_α
 snoch0_n8_β:
 jmp snoch0_n11_α
snoch0_n9_α:
bb17_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S3]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n9_β:
 jmp flat_γ
snoch0_n10_α:
# IR_LIT_scalar
bb18_α:
 jmp snoch0_n12_α
 snoch0_n10_β:
 jmp flat_γ
snoch0_n11_α:
bb19_α:
# IR_IDX: AXS inline DT_A+int fast path, else subscript_get
 lea rdi, [rip + .S0]
 call NV_GET_fn@PLT
 cmp eax, 4
 jne .Lx33_0
 movabs rcx, 0
 mov rsi, rdx
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx33_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx33_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 mov rax, [r11 + rcx*8]
 add r11, 8
 mov rdx, [r11 + rcx*8]
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 jmp snoch0_n13_α
.Lx33_0:
 mov rdi, rax
 mov rsi, rdx
 movabs rdx, 6
 movabs rcx, 0
 call subscript_get@PLT
 cmp eax, 99
 je snoch0_n14_α
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 jmp snoch0_n13_α
 snoch0_n11_β:
 jmp snoch0_n14_α
snoch0_n12_α:
bb20_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S4]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n12_β:
 jmp flat_γ
snoch0_n13_α:
# IR_LIT_scalar
bb21_α:
 jmp snoch0_n15_α
 snoch0_n13_β:
 jmp flat_γ
snoch0_n14_α:
# IR_VAR gva
bb22_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 jmp xgvarg37_done
 xgvarg37_β:
 jmp snoch0_n17_α
xgvarg37_done:
bb23_α:
# BOX IR_CALL prototype(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+336] -> [r12+368]
 mov rax, qword ptr [r12 + 336]
 mov qword ptr [r12 + 368], rax
 mov rax, qword ptr [r12 + 344]
 mov qword ptr [r12 + 376], rax
  .section .rodata
  .Lbynamefn41: .string "prototype"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn41]
 lea rsi, [r12 + 368]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 352], rax
 mov qword ptr [r12 + 360], rdx
 cmp eax, 99
 je snoch0_n17_α
 jmp xgvarg36_done
xgvarg36_β:
 jmp snoch0_n17_α
xgvarg36_done:
# IR_LIT_S
bb24_α:
 mov qword ptr [r12 + 384], 1
 mov rax, qword ptr [rip + .Lx43_0]
 mov qword ptr [r12 + 392], rax
 jmp xgvarg42_done
 xgvarg42_β:
 jmp snoch0_n17_α
.Lx43_0:
 .quad .Lx43_0_s
.Lx43_0_s:
 .string "3"
xgvarg42_done:
bb25_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+352] -> [r12+416]
 mov rax, qword ptr [r12 + 352]
 mov qword ptr [r12 + 416], rax
 mov rax, qword ptr [r12 + 360]
 mov qword ptr [r12 + 424], rax
# marshal arg1 = producer-box slot [r12+384] -> [r12+432]
 mov rax, qword ptr [r12 + 384]
 mov qword ptr [r12 + 432], rax
 mov rax, qword ptr [r12 + 392]
 mov qword ptr [r12 + 440], rax
  .section .rodata
  .Lbynamefn45: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn45]
 lea rsi, [r12 + 416]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 400], rax
 mov qword ptr [r12 + 408], rdx
 cmp eax, 99
 je snoch0_n17_α
 jmp snoch0_n16_α
snoch0_n14_β:
 jmp snoch0_n17_α
snoch0_n15_α:
bb26_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S5]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n15_β:
 jmp flat_γ
snoch0_n16_α:
# IR_LIT_scalar
bb27_α:
 jmp snoch0_n18_α
 snoch0_n16_β:
 jmp flat_γ
snoch0_n17_α:
# IR_LIT_S
bb28_α:
 mov qword ptr [r12 + 448], 1
 mov rax, qword ptr [rip + .Lx49_0]
 mov qword ptr [r12 + 456], rax
 jmp xgvarg48_done
 xgvarg48_β:
 jmp snoch0_n20_α
.Lx49_0:
 .quad .Lx49_0_s
.Lx49_0_s:
 .string "3"
xgvarg48_done:
bb29_α:
# BOX IR_CALL array(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+448] -> [r12+480]
 mov rax, qword ptr [r12 + 448]
 mov qword ptr [r12 + 480], rax
 mov rax, qword ptr [r12 + 456]
 mov qword ptr [r12 + 488], rax
  .section .rodata
  .Lbynamefn51: .string "array"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn51]
 lea rsi, [r12 + 480]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 464], rax
 mov qword ptr [r12 + 472], rdx
 cmp eax, 99
 je snoch0_n20_α
 jmp snoch0_n19_α
snoch0_n17_β:
 jmp snoch0_n20_α
snoch0_n18_α:
bb30_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S6]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n18_β:
 jmp flat_γ
snoch0_n19_α:
bb31_α:
# IR_ASSIGN_CALL gva
 mov rax, qword ptr [r12 + 464]
 mov rcx, qword ptr [r12 + 472]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rcx
 jmp snoch0_n20_α
 snoch0_n19_β:
 jmp snoch0_n20_α
snoch0_n20_α:
# IR_LIT_S
bb32_α:
 mov qword ptr [r12 + 496], 1
 mov rax, qword ptr [rip + .Lx55_0]
 mov qword ptr [r12 + 504], rax
 jmp xiset54_val_done
 xiset54_val_β:
 jmp snoch0_n21_α
.Lx55_0:
 .quad .Lx55_0_s
.Lx55_0_s:
 .string "x"
xiset54_val_done:
# IR_LIT_I
bb33_α:
 mov qword ptr [r12 + 512], 6
 mov rax, qword ptr [rip + .Lx56_0]
 mov qword ptr [r12 + 520], rax
 jmp xiset54_key_done
 xiset54_key_β:
 jmp snoch0_n21_α
.Lx56_0:
 .quad 2
xiset54_key_done:
# IR_VAR gva
bb34_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 528], rax
 mov qword ptr [r12 + 536], rdx
 jmp xiset54_base_done
 xiset54_base_β:
 jmp snoch0_n21_α
xiset54_base_done:
bb35_α:
# IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set
 mov rax, qword ptr [r12 + 528]
 cmp eax, 4
 jne .Lx60_0
 mov rax, qword ptr [r12 + 512]
 cmp eax, 6
 jne .Lx60_0
 mov rsi, qword ptr [r12 + 536]
 mov rcx, qword ptr [r12 + 520]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx60_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx60_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 496]
 mov rdx, qword ptr [r12 + 504]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n21_α
.Lx60_0:
 mov rdi, qword ptr [r12 + 528]
 mov rsi, qword ptr [r12 + 536]
 mov rdx, qword ptr [r12 + 512]
 mov rcx, qword ptr [r12 + 520]
 mov r8, qword ptr [r12 + 496]
 mov r9, qword ptr [r12 + 504]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n21_α
 jmp snoch0_n21_α
 snoch0_n20_β:
 jmp snoch0_n21_α
snoch0_n21_α:
bb36_α:
# IR_IDX: AXS inline DT_A+int fast path, else subscript_get
 lea rdi, [rip + .S7]
 call NV_GET_fn@PLT
 cmp eax, 4
 jne .Lx63_0
 movabs rcx, 2
 mov rsi, rdx
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx63_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx63_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 mov rax, [r11 + rcx*8]
 add r11, 8
 mov rdx, [r11 + rcx*8]
 mov qword ptr [r12 + 560], rax
 mov qword ptr [r12 + 568], rdx
 jmp xgvarg61_done
.Lx63_0:
 mov rdi, rax
 mov rsi, rdx
 movabs rdx, 6
 movabs rcx, 2
 call subscript_get@PLT
 cmp eax, 99
 je snoch0_n23_α
 mov qword ptr [r12 + 560], rax
 mov qword ptr [r12 + 568], rdx
 jmp xgvarg61_done
 xgvarg61_β:
 jmp snoch0_n23_α
xgvarg61_done:
# IR_LIT_S
bb37_α:
 mov qword ptr [r12 + 576], 1
 mov rax, qword ptr [rip + .Lx65_0]
 mov qword ptr [r12 + 584], rax
 jmp xgvarg64_done
 xgvarg64_β:
 jmp snoch0_n23_α
.Lx65_0:
 .quad .Lx65_0_s
.Lx65_0_s:
 .string "x"
xgvarg64_done:
bb38_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+560] -> [r12+608]
 mov rax, qword ptr [r12 + 560]
 mov qword ptr [r12 + 608], rax
 mov rax, qword ptr [r12 + 568]
 mov qword ptr [r12 + 616], rax
# marshal arg1 = producer-box slot [r12+576] -> [r12+624]
 mov rax, qword ptr [r12 + 576]
 mov qword ptr [r12 + 624], rax
 mov rax, qword ptr [r12 + 584]
 mov qword ptr [r12 + 632], rax
  .section .rodata
  .Lbynamefn67: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn67]
 lea rsi, [r12 + 608]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 592], rax
 mov qword ptr [r12 + 600], rdx
 cmp eax, 99
 je snoch0_n23_α
 jmp snoch0_n22_α
snoch0_n21_β:
 jmp snoch0_n23_α
snoch0_n22_α:
# IR_LIT_scalar
bb39_α:
 jmp snoch0_n24_α
 snoch0_n22_β:
 jmp flat_γ
snoch0_n23_α:
# IR_VAR gva
bb40_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 640], rax
 mov qword ptr [r12 + 648], rdx
 jmp xgvarg70_done
 xgvarg70_β:
 jmp snoch0_n26_α
xgvarg70_done:
bb41_α:
# BOX IR_CALL prototype(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+640] -> [r12+672]
 mov rax, qword ptr [r12 + 640]
 mov qword ptr [r12 + 672], rax
 mov rax, qword ptr [r12 + 648]
 mov qword ptr [r12 + 680], rax
  .section .rodata
  .Lbynamefn74: .string "prototype"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn74]
 lea rsi, [r12 + 672]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 656], rax
 mov qword ptr [r12 + 664], rdx
 cmp eax, 99
 je snoch0_n26_α
 jmp xgvarg69_done
xgvarg69_β:
 jmp snoch0_n26_α
xgvarg69_done:
# IR_LIT_S
bb42_α:
 mov qword ptr [r12 + 688], 1
 mov rax, qword ptr [rip + .Lx76_0]
 mov qword ptr [r12 + 696], rax
 jmp xgvarg75_done
 xgvarg75_β:
 jmp snoch0_n26_α
.Lx76_0:
 .quad .Lx76_0_s
.Lx76_0_s:
 .string "3"
xgvarg75_done:
bb43_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+656] -> [r12+720]
 mov rax, qword ptr [r12 + 656]
 mov qword ptr [r12 + 720], rax
 mov rax, qword ptr [r12 + 664]
 mov qword ptr [r12 + 728], rax
# marshal arg1 = producer-box slot [r12+688] -> [r12+736]
 mov rax, qword ptr [r12 + 688]
 mov qword ptr [r12 + 736], rax
 mov rax, qword ptr [r12 + 696]
 mov qword ptr [r12 + 744], rax
  .section .rodata
  .Lbynamefn78: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn78]
 lea rsi, [r12 + 720]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 704], rax
 mov qword ptr [r12 + 712], rdx
 cmp eax, 99
 je snoch0_n26_α
 jmp snoch0_n25_α
snoch0_n23_β:
 jmp snoch0_n26_α
snoch0_n24_α:
bb44_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S8]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n24_β:
 jmp flat_γ
snoch0_n25_α:
# IR_LIT_scalar
bb45_α:
 jmp snoch0_n27_α
 snoch0_n25_β:
 jmp flat_γ
snoch0_n26_α:
# IR_LIT_F
bb46_α:
 mov qword ptr [r12 + 752], 7
 mov rax, qword ptr [rip + .Lx82_0]
 mov qword ptr [r12 + 760], rax
 jmp xiset81_val_done
 xiset81_val_β:
 jmp snoch0_n28_α
.Lx82_0:
 .quad 4614253070214989087
xiset81_val_done:
# IR_LIT_I
bb47_α:
 mov qword ptr [r12 + 768], 6
 mov rax, qword ptr [rip + .Lx83_0]
 mov qword ptr [r12 + 776], rax
 jmp xiset81_key_done
 xiset81_key_β:
 jmp snoch0_n28_α
.Lx83_0:
 .quad 1
xiset81_key_done:
# IR_VAR gva
bb48_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 784], rax
 mov qword ptr [r12 + 792], rdx
 jmp xiset81_base_done
 xiset81_base_β:
 jmp snoch0_n28_α
xiset81_base_done:
bb49_α:
# IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set
 mov rax, qword ptr [r12 + 784]
 cmp eax, 4
 jne .Lx87_0
 mov rax, qword ptr [r12 + 768]
 cmp eax, 6
 jne .Lx87_0
 mov rsi, qword ptr [r12 + 792]
 mov rcx, qword ptr [r12 + 776]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx87_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx87_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 752]
 mov rdx, qword ptr [r12 + 760]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n28_α
.Lx87_0:
 mov rdi, qword ptr [r12 + 784]
 mov rsi, qword ptr [r12 + 792]
 mov rdx, qword ptr [r12 + 768]
 mov rcx, qword ptr [r12 + 776]
 mov r8, qword ptr [r12 + 752]
 mov r9, qword ptr [r12 + 760]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n28_α
 jmp snoch0_n28_α
 snoch0_n26_β:
 jmp snoch0_n28_α
snoch0_n27_α:
bb50_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S9]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n27_β:
 jmp flat_γ
snoch0_n28_α:
bb51_α:
# IR_IDX: AXS inline DT_A+int fast path, else subscript_get
 lea rdi, [rip + .S0]
 call NV_GET_fn@PLT
 cmp eax, 4
 jne .Lx91_0
 movabs rcx, 1
 mov rsi, rdx
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx91_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx91_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 mov rax, [r11 + rcx*8]
 add r11, 8
 mov rdx, [r11 + rcx*8]
 mov qword ptr [r12 + 816], rax
 mov qword ptr [r12 + 824], rdx
 jmp xgvarg89_done
.Lx91_0:
 mov rdi, rax
 mov rsi, rdx
 movabs rdx, 6
 movabs rcx, 1
 call subscript_get@PLT
 cmp eax, 99
 je snoch0_n30_α
 mov qword ptr [r12 + 816], rax
 mov qword ptr [r12 + 824], rdx
 jmp xgvarg89_done
 xgvarg89_β:
 jmp snoch0_n30_α
xgvarg89_done:
# IR_LIT_F
bb52_α:
 mov qword ptr [r12 + 832], 7
 mov rax, qword ptr [rip + .Lx93_0]
 mov qword ptr [r12 + 840], rax
 jmp xgvarg92_done
 xgvarg92_β:
 jmp snoch0_n30_α
.Lx93_0:
 .quad 4614253070214989087
xgvarg92_done:
bb53_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+816] -> [r12+864]
 mov rax, qword ptr [r12 + 816]
 mov qword ptr [r12 + 864], rax
 mov rax, qword ptr [r12 + 824]
 mov qword ptr [r12 + 872], rax
# marshal arg1 = producer-box slot [r12+832] -> [r12+880]
 mov rax, qword ptr [r12 + 832]
 mov qword ptr [r12 + 880], rax
 mov rax, qword ptr [r12 + 840]
 mov qword ptr [r12 + 888], rax
  .section .rodata
  .Lbynamefn95: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn95]
 lea rsi, [r12 + 864]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 848], rax
 mov qword ptr [r12 + 856], rdx
 cmp eax, 99
 je snoch0_n30_α
 jmp snoch0_n29_α
snoch0_n28_β:
 jmp snoch0_n30_α
snoch0_n29_α:
# IR_LIT_scalar
bb54_α:
 jmp snoch0_n31_α
 snoch0_n29_β:
 jmp flat_γ
snoch0_n30_α:
# IR_LIT_S
bb55_α:
 mov qword ptr [r12 + 896], 1
 mov rax, qword ptr [rip + .Lx98_0]
 mov qword ptr [r12 + 904], rax
 jmp xiset97_val_done
 xiset97_val_β:
 jmp snoch0_n32_α
.Lx98_0:
 .quad .Lx98_0_s
.Lx98_0_s:
 .string "z"
xiset97_val_done:
# IR_LIT_I
bb56_α:
 mov qword ptr [r12 + 912], 6
 mov rax, qword ptr [rip + .Lx99_0]
 mov qword ptr [r12 + 920], rax
 jmp xiset97_key_done
 xiset97_key_β:
 jmp snoch0_n32_α
.Lx99_0:
 .quad 3
xiset97_key_done:
# IR_VAR gva
bb57_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 928], rax
 mov qword ptr [r12 + 936], rdx
 jmp xiset97_base_done
 xiset97_base_β:
 jmp snoch0_n32_α
xiset97_base_done:
bb58_α:
# IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set
 mov rax, qword ptr [r12 + 928]
 cmp eax, 4
 jne .Lx103_0
 mov rax, qword ptr [r12 + 912]
 cmp eax, 6
 jne .Lx103_0
 mov rsi, qword ptr [r12 + 936]
 mov rcx, qword ptr [r12 + 920]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx103_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx103_0
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
 jmp snoch0_n32_α
.Lx103_0:
 mov rdi, qword ptr [r12 + 928]
 mov rsi, qword ptr [r12 + 936]
 mov rdx, qword ptr [r12 + 912]
 mov rcx, qword ptr [r12 + 920]
 mov r8, qword ptr [r12 + 896]
 mov r9, qword ptr [r12 + 904]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n32_α
 jmp snoch0_n32_α
 snoch0_n30_β:
 jmp snoch0_n32_α
snoch0_n31_α:
bb59_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S10]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n31_β:
 jmp flat_γ
snoch0_n32_α:
bb60_α:
# IR_IDX: AXS inline DT_A+int fast path, else subscript_get
 lea rdi, [rip + .S0]
 call NV_GET_fn@PLT
 cmp eax, 4
 jne .Lx107_0
 movabs rcx, 3
 mov rsi, rdx
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx107_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx107_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 mov rax, [r11 + rcx*8]
 add r11, 8
 mov rdx, [r11 + rcx*8]
 mov qword ptr [r12 + 960], rax
 mov qword ptr [r12 + 968], rdx
 jmp xgvarg105_done
.Lx107_0:
 mov rdi, rax
 mov rsi, rdx
 movabs rdx, 6
 movabs rcx, 3
 call subscript_get@PLT
 cmp eax, 99
 je snoch0_n34_α
 mov qword ptr [r12 + 960], rax
 mov qword ptr [r12 + 968], rdx
 jmp xgvarg105_done
 xgvarg105_β:
 jmp snoch0_n34_α
xgvarg105_done:
# IR_LIT_S
bb61_α:
 mov qword ptr [r12 + 976], 1
 mov rax, qword ptr [rip + .Lx109_0]
 mov qword ptr [r12 + 984], rax
 jmp xgvarg108_done
 xgvarg108_β:
 jmp snoch0_n34_α
.Lx109_0:
 .quad .Lx109_0_s
.Lx109_0_s:
 .string "z"
xgvarg108_done:
bb62_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+960] -> [r12+1008]
 mov rax, qword ptr [r12 + 960]
 mov qword ptr [r12 + 1008], rax
 mov rax, qword ptr [r12 + 968]
 mov qword ptr [r12 + 1016], rax
# marshal arg1 = producer-box slot [r12+976] -> [r12+1024]
 mov rax, qword ptr [r12 + 976]
 mov qword ptr [r12 + 1024], rax
 mov rax, qword ptr [r12 + 984]
 mov qword ptr [r12 + 1032], rax
  .section .rodata
  .Lbynamefn111: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn111]
 lea rsi, [r12 + 1008]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 992], rax
 mov qword ptr [r12 + 1000], rdx
 cmp eax, 99
 je snoch0_n34_α
 jmp snoch0_n33_α
snoch0_n32_β:
 jmp snoch0_n34_α
snoch0_n33_α:
# IR_LIT_scalar
bb63_α:
 jmp snoch0_n35_α
 snoch0_n33_β:
 jmp flat_γ
snoch0_n34_α:
# IR_LIT_scalar
bb64_α:
 jmp snoch0_n36_α
 snoch0_n34_β:
 jmp flat_γ
snoch0_n35_α:
bb65_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S11]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n35_β:
 jmp flat_γ
snoch0_n36_α:
bb66_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S12]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n36_β:
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
.S2: .string "FAIL 1110/001: array element init null"
.S3: .string "FAIL 1110/002: array assign/read"
.S4: .string "FAIL 1110/003: OOB high should fail"
.S5: .string "FAIL 1110/004: OOB zero should fail"
.S6: .string "FAIL 1110/005: prototype(array(3))=3"
.S7: .string "b"
.S8: .string "FAIL 1110/006: array from string dim"
.S9: .string "FAIL 1110/007: prototype string-dim array"
.S10: .string "FAIL 1110/008: array stores real"
.S11: .string "FAIL 1110/009: array stores string"
.S12: .string "PASS 1110_array_1d (9/9)"
.text

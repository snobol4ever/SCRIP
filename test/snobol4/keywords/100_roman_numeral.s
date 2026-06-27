  .intel_syntax noprefix
  .text
roman_α:
#=======================================================================================================================
    .global roman_α
    .global roman_β
    .global roman_γ
    .global roman_ω
push r12
  mov r12, rdi
  lea r10, [rip + Δ]
roman_α_body:
snoch0_n0_α:
# IR_LIT_scalar
bb1_α:
 jmp snoch0_n1_α
 snoch0_n0_β:
 jmp snoch0_n2_α
snoch0_n1_α:
bb2_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S1]
 call rt_gvar_assign_str@PLT
 jmp snoch0_n2_α
 snoch0_n1_β:
 jmp snoch0_n2_α
snoch0_n2_α:
# IR_LIT_I
bb3_α:
 mov qword ptr [r12 + 16], 6
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [r12 + 24], rax
 jmp xgvarg3_done
 xgvarg3_β:
 jmp snoch0_n4_α
.Lx4_0:
 .quad 13
xgvarg3_done:
bb4_α:
# BOX IR_CALL ARRAY(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+16] -> [r12+48]
 mov rax, qword ptr [r12 + 16]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 24]
 mov qword ptr [r12 + 56], rax
  .section .rodata
  .Lbynamefn6: .string "ARRAY"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn6]
 lea rsi, [r12 + 48]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je snoch0_n4_α
 jmp snoch0_n3_α
snoch0_n2_β:
 jmp snoch0_n4_α
snoch0_n3_α:
bb5_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S2]
 mov rsi, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n4_α
 snoch0_n3_β:
 jmp snoch0_n4_α
snoch0_n4_α:
# IR_LIT_I
bb6_α:
 mov qword ptr [r12 + 64], 6
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 72], rax
 jmp xiset8_val_done
 xiset8_val_β:
 jmp snoch0_n5_α
.Lx9_0:
 .quad 1000
xiset8_val_done:
# IR_LIT_I
bb7_α:
 mov qword ptr [r12 + 80], 6
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [r12 + 88], rax
 jmp xiset8_key_done
 xiset8_key_β:
 jmp snoch0_n5_α
.Lx10_0:
 .quad 1
xiset8_key_done:
# IR_VAR
bb8_α:
 mov rdi, qword ptr [rip + .Lx12_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 jmp xiset8_base_done
 xiset8_base_β:
 jmp snoch0_n5_α
.Lx12_0:
 .quad .Lx12_0_s
.Lx12_0_s:
 .string "v"
xiset8_base_done:
bb9_α:
# IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set
 mov rax, qword ptr [r12 + 96]
 cmp eax, 4
 jne .Lx14_0
 mov rax, qword ptr [r12 + 80]
 cmp eax, 6
 jne .Lx14_0
 mov rsi, qword ptr [r12 + 104]
 mov rcx, qword ptr [r12 + 88]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx14_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx14_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 64]
 mov rdx, qword ptr [r12 + 72]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n5_α
.Lx14_0:
 mov rdi, qword ptr [r12 + 96]
 mov rsi, qword ptr [r12 + 104]
 mov rdx, qword ptr [r12 + 80]
 mov rcx, qword ptr [r12 + 88]
 mov r8, qword ptr [r12 + 64]
 mov r9, qword ptr [r12 + 72]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n5_α
 jmp snoch0_n5_α
 snoch0_n4_β:
 jmp snoch0_n5_α
snoch0_n5_α:
# IR_LIT_I
bb10_α:
 mov qword ptr [r12 + 112], 6
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [r12 + 120], rax
 jmp xiset15_val_done
 xiset15_val_β:
 jmp snoch0_n6_α
.Lx16_0:
 .quad 900
xiset15_val_done:
# IR_LIT_I
bb11_α:
 mov qword ptr [r12 + 128], 6
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [r12 + 136], rax
 jmp xiset15_key_done
 xiset15_key_β:
 jmp snoch0_n6_α
.Lx17_0:
 .quad 2
xiset15_key_done:
# IR_VAR
bb12_α:
 mov rdi, qword ptr [rip + .Lx19_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 jmp xiset15_base_done
 xiset15_base_β:
 jmp snoch0_n6_α
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "v"
xiset15_base_done:
bb13_α:
# IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set
 mov rax, qword ptr [r12 + 144]
 cmp eax, 4
 jne .Lx21_0
 mov rax, qword ptr [r12 + 128]
 cmp eax, 6
 jne .Lx21_0
 mov rsi, qword ptr [r12 + 152]
 mov rcx, qword ptr [r12 + 136]
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
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 112]
 mov rdx, qword ptr [r12 + 120]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n6_α
.Lx21_0:
 mov rdi, qword ptr [r12 + 144]
 mov rsi, qword ptr [r12 + 152]
 mov rdx, qword ptr [r12 + 128]
 mov rcx, qword ptr [r12 + 136]
 mov r8, qword ptr [r12 + 112]
 mov r9, qword ptr [r12 + 120]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n6_α
 jmp snoch0_n6_α
 snoch0_n5_β:
 jmp snoch0_n6_α
snoch0_n6_α:
# IR_LIT_I
bb14_α:
 mov qword ptr [r12 + 160], 6
 mov rax, qword ptr [rip + .Lx23_0]
 mov qword ptr [r12 + 168], rax
 jmp xiset22_val_done
 xiset22_val_β:
 jmp snoch0_n7_α
.Lx23_0:
 .quad 500
xiset22_val_done:
# IR_LIT_I
bb15_α:
 mov qword ptr [r12 + 176], 6
 mov rax, qword ptr [rip + .Lx24_0]
 mov qword ptr [r12 + 184], rax
 jmp xiset22_key_done
 xiset22_key_β:
 jmp snoch0_n7_α
.Lx24_0:
 .quad 3
xiset22_key_done:
# IR_VAR
bb16_α:
 mov rdi, qword ptr [rip + .Lx26_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 jmp xiset22_base_done
 xiset22_base_β:
 jmp snoch0_n7_α
.Lx26_0:
 .quad .Lx26_0_s
.Lx26_0_s:
 .string "v"
xiset22_base_done:
bb17_α:
# IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set
 mov rax, qword ptr [r12 + 192]
 cmp eax, 4
 jne .Lx28_0
 mov rax, qword ptr [r12 + 176]
 cmp eax, 6
 jne .Lx28_0
 mov rsi, qword ptr [r12 + 200]
 mov rcx, qword ptr [r12 + 184]
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
 mov rax, qword ptr [r12 + 160]
 mov rdx, qword ptr [r12 + 168]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n7_α
.Lx28_0:
 mov rdi, qword ptr [r12 + 192]
 mov rsi, qword ptr [r12 + 200]
 mov rdx, qword ptr [r12 + 176]
 mov rcx, qword ptr [r12 + 184]
 mov r8, qword ptr [r12 + 160]
 mov r9, qword ptr [r12 + 168]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n7_α
 jmp snoch0_n7_α
 snoch0_n6_β:
 jmp snoch0_n7_α
snoch0_n7_α:
# IR_LIT_I
bb18_α:
 mov qword ptr [r12 + 208], 6
 mov rax, qword ptr [rip + .Lx30_0]
 mov qword ptr [r12 + 216], rax
 jmp xiset29_val_done
 xiset29_val_β:
 jmp snoch0_n8_α
.Lx30_0:
 .quad 400
xiset29_val_done:
# IR_LIT_I
bb19_α:
 mov qword ptr [r12 + 224], 6
 mov rax, qword ptr [rip + .Lx31_0]
 mov qword ptr [r12 + 232], rax
 jmp xiset29_key_done
 xiset29_key_β:
 jmp snoch0_n8_α
.Lx31_0:
 .quad 4
xiset29_key_done:
# IR_VAR
bb20_α:
 mov rdi, qword ptr [rip + .Lx33_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 jmp xiset29_base_done
 xiset29_base_β:
 jmp snoch0_n8_α
.Lx33_0:
 .quad .Lx33_0_s
.Lx33_0_s:
 .string "v"
xiset29_base_done:
bb21_α:
# IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set
 mov rax, qword ptr [r12 + 240]
 cmp eax, 4
 jne .Lx35_0
 mov rax, qword ptr [r12 + 224]
 cmp eax, 6
 jne .Lx35_0
 mov rsi, qword ptr [r12 + 248]
 mov rcx, qword ptr [r12 + 232]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx35_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx35_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 208]
 mov rdx, qword ptr [r12 + 216]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n8_α
.Lx35_0:
 mov rdi, qword ptr [r12 + 240]
 mov rsi, qword ptr [r12 + 248]
 mov rdx, qword ptr [r12 + 224]
 mov rcx, qword ptr [r12 + 232]
 mov r8, qword ptr [r12 + 208]
 mov r9, qword ptr [r12 + 216]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n8_α
 jmp snoch0_n8_α
 snoch0_n7_β:
 jmp snoch0_n8_α
snoch0_n8_α:
# IR_LIT_I
bb22_α:
 mov qword ptr [r12 + 256], 6
 mov rax, qword ptr [rip + .Lx37_0]
 mov qword ptr [r12 + 264], rax
 jmp xiset36_val_done
 xiset36_val_β:
 jmp snoch0_n9_α
.Lx37_0:
 .quad 100
xiset36_val_done:
# IR_LIT_I
bb23_α:
 mov qword ptr [r12 + 272], 6
 mov rax, qword ptr [rip + .Lx38_0]
 mov qword ptr [r12 + 280], rax
 jmp xiset36_key_done
 xiset36_key_β:
 jmp snoch0_n9_α
.Lx38_0:
 .quad 5
xiset36_key_done:
# IR_VAR
bb24_α:
 mov rdi, qword ptr [rip + .Lx40_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 288], rax
 mov qword ptr [r12 + 296], rdx
 jmp xiset36_base_done
 xiset36_base_β:
 jmp snoch0_n9_α
.Lx40_0:
 .quad .Lx40_0_s
.Lx40_0_s:
 .string "v"
xiset36_base_done:
bb25_α:
# IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set
 mov rax, qword ptr [r12 + 288]
 cmp eax, 4
 jne .Lx42_0
 mov rax, qword ptr [r12 + 272]
 cmp eax, 6
 jne .Lx42_0
 mov rsi, qword ptr [r12 + 296]
 mov rcx, qword ptr [r12 + 280]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx42_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx42_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 256]
 mov rdx, qword ptr [r12 + 264]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n9_α
.Lx42_0:
 mov rdi, qword ptr [r12 + 288]
 mov rsi, qword ptr [r12 + 296]
 mov rdx, qword ptr [r12 + 272]
 mov rcx, qword ptr [r12 + 280]
 mov r8, qword ptr [r12 + 256]
 mov r9, qword ptr [r12 + 264]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n9_α
 jmp snoch0_n9_α
 snoch0_n8_β:
 jmp snoch0_n9_α
snoch0_n9_α:
# IR_LIT_I
bb26_α:
 mov qword ptr [r12 + 304], 6
 mov rax, qword ptr [rip + .Lx44_0]
 mov qword ptr [r12 + 312], rax
 jmp xiset43_val_done
 xiset43_val_β:
 jmp snoch0_n10_α
.Lx44_0:
 .quad 90
xiset43_val_done:
# IR_LIT_I
bb27_α:
 mov qword ptr [r12 + 320], 6
 mov rax, qword ptr [rip + .Lx45_0]
 mov qword ptr [r12 + 328], rax
 jmp xiset43_key_done
 xiset43_key_β:
 jmp snoch0_n10_α
.Lx45_0:
 .quad 6
xiset43_key_done:
# IR_VAR
bb28_α:
 mov rdi, qword ptr [rip + .Lx47_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 jmp xiset43_base_done
 xiset43_base_β:
 jmp snoch0_n10_α
.Lx47_0:
 .quad .Lx47_0_s
.Lx47_0_s:
 .string "v"
xiset43_base_done:
bb29_α:
# IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set
 mov rax, qword ptr [r12 + 336]
 cmp eax, 4
 jne .Lx49_0
 mov rax, qword ptr [r12 + 320]
 cmp eax, 6
 jne .Lx49_0
 mov rsi, qword ptr [r12 + 344]
 mov rcx, qword ptr [r12 + 328]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx49_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx49_0
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
 jmp snoch0_n10_α
.Lx49_0:
 mov rdi, qword ptr [r12 + 336]
 mov rsi, qword ptr [r12 + 344]
 mov rdx, qword ptr [r12 + 320]
 mov rcx, qword ptr [r12 + 328]
 mov r8, qword ptr [r12 + 304]
 mov r9, qword ptr [r12 + 312]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n10_α
 jmp snoch0_n10_α
 snoch0_n9_β:
 jmp snoch0_n10_α
snoch0_n10_α:
# IR_LIT_I
bb30_α:
 mov qword ptr [r12 + 352], 6
 mov rax, qword ptr [rip + .Lx51_0]
 mov qword ptr [r12 + 360], rax
 jmp xiset50_val_done
 xiset50_val_β:
 jmp snoch0_n11_α
.Lx51_0:
 .quad 50
xiset50_val_done:
# IR_LIT_I
bb31_α:
 mov qword ptr [r12 + 368], 6
 mov rax, qword ptr [rip + .Lx52_0]
 mov qword ptr [r12 + 376], rax
 jmp xiset50_key_done
 xiset50_key_β:
 jmp snoch0_n11_α
.Lx52_0:
 .quad 7
xiset50_key_done:
# IR_VAR
bb32_α:
 mov rdi, qword ptr [rip + .Lx54_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 jmp xiset50_base_done
 xiset50_base_β:
 jmp snoch0_n11_α
.Lx54_0:
 .quad .Lx54_0_s
.Lx54_0_s:
 .string "v"
xiset50_base_done:
bb33_α:
# IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set
 mov rax, qword ptr [r12 + 384]
 cmp eax, 4
 jne .Lx56_0
 mov rax, qword ptr [r12 + 368]
 cmp eax, 6
 jne .Lx56_0
 mov rsi, qword ptr [r12 + 392]
 mov rcx, qword ptr [r12 + 376]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx56_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx56_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 352]
 mov rdx, qword ptr [r12 + 360]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n11_α
.Lx56_0:
 mov rdi, qword ptr [r12 + 384]
 mov rsi, qword ptr [r12 + 392]
 mov rdx, qword ptr [r12 + 368]
 mov rcx, qword ptr [r12 + 376]
 mov r8, qword ptr [r12 + 352]
 mov r9, qword ptr [r12 + 360]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n11_α
 jmp snoch0_n11_α
 snoch0_n10_β:
 jmp snoch0_n11_α
snoch0_n11_α:
# IR_LIT_I
bb34_α:
 mov qword ptr [r12 + 400], 6
 mov rax, qword ptr [rip + .Lx58_0]
 mov qword ptr [r12 + 408], rax
 jmp xiset57_val_done
 xiset57_val_β:
 jmp snoch0_n12_α
.Lx58_0:
 .quad 40
xiset57_val_done:
# IR_LIT_I
bb35_α:
 mov qword ptr [r12 + 416], 6
 mov rax, qword ptr [rip + .Lx59_0]
 mov qword ptr [r12 + 424], rax
 jmp xiset57_key_done
 xiset57_key_β:
 jmp snoch0_n12_α
.Lx59_0:
 .quad 8
xiset57_key_done:
# IR_VAR
bb36_α:
 mov rdi, qword ptr [rip + .Lx61_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 432], rax
 mov qword ptr [r12 + 440], rdx
 jmp xiset57_base_done
 xiset57_base_β:
 jmp snoch0_n12_α
.Lx61_0:
 .quad .Lx61_0_s
.Lx61_0_s:
 .string "v"
xiset57_base_done:
bb37_α:
# IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set
 mov rax, qword ptr [r12 + 432]
 cmp eax, 4
 jne .Lx63_0
 mov rax, qword ptr [r12 + 416]
 cmp eax, 6
 jne .Lx63_0
 mov rsi, qword ptr [r12 + 440]
 mov rcx, qword ptr [r12 + 424]
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
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 400]
 mov rdx, qword ptr [r12 + 408]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n12_α
.Lx63_0:
 mov rdi, qword ptr [r12 + 432]
 mov rsi, qword ptr [r12 + 440]
 mov rdx, qword ptr [r12 + 416]
 mov rcx, qword ptr [r12 + 424]
 mov r8, qword ptr [r12 + 400]
 mov r9, qword ptr [r12 + 408]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n12_α
 jmp snoch0_n12_α
 snoch0_n11_β:
 jmp snoch0_n12_α
snoch0_n12_α:
# IR_LIT_I
bb38_α:
 mov qword ptr [r12 + 448], 6
 mov rax, qword ptr [rip + .Lx65_0]
 mov qword ptr [r12 + 456], rax
 jmp xiset64_val_done
 xiset64_val_β:
 jmp snoch0_n13_α
.Lx65_0:
 .quad 10
xiset64_val_done:
# IR_LIT_I
bb39_α:
 mov qword ptr [r12 + 464], 6
 mov rax, qword ptr [rip + .Lx66_0]
 mov qword ptr [r12 + 472], rax
 jmp xiset64_key_done
 xiset64_key_β:
 jmp snoch0_n13_α
.Lx66_0:
 .quad 9
xiset64_key_done:
# IR_VAR
bb40_α:
 mov rdi, qword ptr [rip + .Lx68_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 480], rax
 mov qword ptr [r12 + 488], rdx
 jmp xiset64_base_done
 xiset64_base_β:
 jmp snoch0_n13_α
.Lx68_0:
 .quad .Lx68_0_s
.Lx68_0_s:
 .string "v"
xiset64_base_done:
bb41_α:
# IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set
 mov rax, qword ptr [r12 + 480]
 cmp eax, 4
 jne .Lx70_0
 mov rax, qword ptr [r12 + 464]
 cmp eax, 6
 jne .Lx70_0
 mov rsi, qword ptr [r12 + 488]
 mov rcx, qword ptr [r12 + 472]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx70_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx70_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 448]
 mov rdx, qword ptr [r12 + 456]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n13_α
.Lx70_0:
 mov rdi, qword ptr [r12 + 480]
 mov rsi, qword ptr [r12 + 488]
 mov rdx, qword ptr [r12 + 464]
 mov rcx, qword ptr [r12 + 472]
 mov r8, qword ptr [r12 + 448]
 mov r9, qword ptr [r12 + 456]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n13_α
 jmp snoch0_n13_α
 snoch0_n12_β:
 jmp snoch0_n13_α
snoch0_n13_α:
# IR_LIT_I
bb42_α:
 mov qword ptr [r12 + 496], 6
 mov rax, qword ptr [rip + .Lx72_0]
 mov qword ptr [r12 + 504], rax
 jmp xiset71_val_done
 xiset71_val_β:
 jmp snoch0_n14_α
.Lx72_0:
 .quad 9
xiset71_val_done:
# IR_LIT_I
bb43_α:
 mov qword ptr [r12 + 512], 6
 mov rax, qword ptr [rip + .Lx73_0]
 mov qword ptr [r12 + 520], rax
 jmp xiset71_key_done
 xiset71_key_β:
 jmp snoch0_n14_α
.Lx73_0:
 .quad 10
xiset71_key_done:
# IR_VAR
bb44_α:
 mov rdi, qword ptr [rip + .Lx75_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 528], rax
 mov qword ptr [r12 + 536], rdx
 jmp xiset71_base_done
 xiset71_base_β:
 jmp snoch0_n14_α
.Lx75_0:
 .quad .Lx75_0_s
.Lx75_0_s:
 .string "v"
xiset71_base_done:
bb45_α:
# IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set
 mov rax, qword ptr [r12 + 528]
 cmp eax, 4
 jne .Lx77_0
 mov rax, qword ptr [r12 + 512]
 cmp eax, 6
 jne .Lx77_0
 mov rsi, qword ptr [r12 + 536]
 mov rcx, qword ptr [r12 + 520]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx77_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx77_0
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
 jmp snoch0_n14_α
.Lx77_0:
 mov rdi, qword ptr [r12 + 528]
 mov rsi, qword ptr [r12 + 536]
 mov rdx, qword ptr [r12 + 512]
 mov rcx, qword ptr [r12 + 520]
 mov r8, qword ptr [r12 + 496]
 mov r9, qword ptr [r12 + 504]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n14_α
 jmp snoch0_n14_α
 snoch0_n13_β:
 jmp snoch0_n14_α
snoch0_n14_α:
# IR_LIT_I
bb46_α:
 mov qword ptr [r12 + 544], 6
 mov rax, qword ptr [rip + .Lx79_0]
 mov qword ptr [r12 + 552], rax
 jmp xiset78_val_done
 xiset78_val_β:
 jmp snoch0_n15_α
.Lx79_0:
 .quad 5
xiset78_val_done:
# IR_LIT_I
bb47_α:
 mov qword ptr [r12 + 560], 6
 mov rax, qword ptr [rip + .Lx80_0]
 mov qword ptr [r12 + 568], rax
 jmp xiset78_key_done
 xiset78_key_β:
 jmp snoch0_n15_α
.Lx80_0:
 .quad 11
xiset78_key_done:
# IR_VAR
bb48_α:
 mov rdi, qword ptr [rip + .Lx82_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 576], rax
 mov qword ptr [r12 + 584], rdx
 jmp xiset78_base_done
 xiset78_base_β:
 jmp snoch0_n15_α
.Lx82_0:
 .quad .Lx82_0_s
.Lx82_0_s:
 .string "v"
xiset78_base_done:
bb49_α:
# IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set
 mov rax, qword ptr [r12 + 576]
 cmp eax, 4
 jne .Lx84_0
 mov rax, qword ptr [r12 + 560]
 cmp eax, 6
 jne .Lx84_0
 mov rsi, qword ptr [r12 + 584]
 mov rcx, qword ptr [r12 + 568]
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
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 544]
 mov rdx, qword ptr [r12 + 552]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n15_α
.Lx84_0:
 mov rdi, qword ptr [r12 + 576]
 mov rsi, qword ptr [r12 + 584]
 mov rdx, qword ptr [r12 + 560]
 mov rcx, qword ptr [r12 + 568]
 mov r8, qword ptr [r12 + 544]
 mov r9, qword ptr [r12 + 552]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n15_α
 jmp snoch0_n15_α
 snoch0_n14_β:
 jmp snoch0_n15_α
snoch0_n15_α:
# IR_LIT_I
bb50_α:
 mov qword ptr [r12 + 592], 6
 mov rax, qword ptr [rip + .Lx86_0]
 mov qword ptr [r12 + 600], rax
 jmp xiset85_val_done
 xiset85_val_β:
 jmp snoch0_n16_α
.Lx86_0:
 .quad 4
xiset85_val_done:
# IR_LIT_I
bb51_α:
 mov qword ptr [r12 + 608], 6
 mov rax, qword ptr [rip + .Lx87_0]
 mov qword ptr [r12 + 616], rax
 jmp xiset85_key_done
 xiset85_key_β:
 jmp snoch0_n16_α
.Lx87_0:
 .quad 12
xiset85_key_done:
# IR_VAR
bb52_α:
 mov rdi, qword ptr [rip + .Lx89_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 624], rax
 mov qword ptr [r12 + 632], rdx
 jmp xiset85_base_done
 xiset85_base_β:
 jmp snoch0_n16_α
.Lx89_0:
 .quad .Lx89_0_s
.Lx89_0_s:
 .string "v"
xiset85_base_done:
bb53_α:
# IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set
 mov rax, qword ptr [r12 + 624]
 cmp eax, 4
 jne .Lx91_0
 mov rax, qword ptr [r12 + 608]
 cmp eax, 6
 jne .Lx91_0
 mov rsi, qword ptr [r12 + 632]
 mov rcx, qword ptr [r12 + 616]
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
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 592]
 mov rdx, qword ptr [r12 + 600]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n16_α
.Lx91_0:
 mov rdi, qword ptr [r12 + 624]
 mov rsi, qword ptr [r12 + 632]
 mov rdx, qword ptr [r12 + 608]
 mov rcx, qword ptr [r12 + 616]
 mov r8, qword ptr [r12 + 592]
 mov r9, qword ptr [r12 + 600]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n16_α
 jmp snoch0_n16_α
 snoch0_n15_β:
 jmp snoch0_n16_α
snoch0_n16_α:
# IR_LIT_I
bb54_α:
 mov qword ptr [r12 + 640], 6
 mov rax, qword ptr [rip + .Lx93_0]
 mov qword ptr [r12 + 648], rax
 jmp xiset92_val_done
 xiset92_val_β:
 jmp snoch0_n17_α
.Lx93_0:
 .quad 1
xiset92_val_done:
# IR_LIT_I
bb55_α:
 mov qword ptr [r12 + 656], 6
 mov rax, qword ptr [rip + .Lx94_0]
 mov qword ptr [r12 + 664], rax
 jmp xiset92_key_done
 xiset92_key_β:
 jmp snoch0_n17_α
.Lx94_0:
 .quad 13
xiset92_key_done:
# IR_VAR
bb56_α:
 mov rdi, qword ptr [rip + .Lx96_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 672], rax
 mov qword ptr [r12 + 680], rdx
 jmp xiset92_base_done
 xiset92_base_β:
 jmp snoch0_n17_α
.Lx96_0:
 .quad .Lx96_0_s
.Lx96_0_s:
 .string "v"
xiset92_base_done:
bb57_α:
# IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set
 mov rax, qword ptr [r12 + 672]
 cmp eax, 4
 jne .Lx98_0
 mov rax, qword ptr [r12 + 656]
 cmp eax, 6
 jne .Lx98_0
 mov rsi, qword ptr [r12 + 680]
 mov rcx, qword ptr [r12 + 664]
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
 mov rax, qword ptr [r12 + 640]
 mov rdx, qword ptr [r12 + 648]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n17_α
.Lx98_0:
 mov rdi, qword ptr [r12 + 672]
 mov rsi, qword ptr [r12 + 680]
 mov rdx, qword ptr [r12 + 656]
 mov rcx, qword ptr [r12 + 664]
 mov r8, qword ptr [r12 + 640]
 mov r9, qword ptr [r12 + 648]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n17_α
 jmp snoch0_n17_α
 snoch0_n16_β:
 jmp snoch0_n17_α
snoch0_n17_α:
# IR_LIT_I
bb58_α:
 mov qword ptr [r12 + 688], 6
 mov rax, qword ptr [rip + .Lx100_0]
 mov qword ptr [r12 + 696], rax
 jmp xgvarg99_done
 xgvarg99_β:
 jmp snoch0_n19_α
.Lx100_0:
 .quad 13
xgvarg99_done:
bb59_α:
# BOX IR_CALL ARRAY(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+688] -> [r12+720]
 mov rax, qword ptr [r12 + 688]
 mov qword ptr [r12 + 720], rax
 mov rax, qword ptr [r12 + 696]
 mov qword ptr [r12 + 728], rax
  .section .rodata
  .Lbynamefn102: .string "ARRAY"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn102]
 lea rsi, [r12 + 720]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 704], rax
 mov qword ptr [r12 + 712], rdx
 cmp eax, 99
 je snoch0_n19_α
 jmp snoch0_n18_α
snoch0_n17_β:
 jmp snoch0_n19_α
snoch0_n18_α:
bb60_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S3]
 mov rsi, qword ptr [r12 + 704]
 mov rdx, qword ptr [r12 + 712]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n19_α
 snoch0_n18_β:
 jmp snoch0_n19_α
snoch0_n19_α:
# IR_LIT_S
bb61_α:
 mov qword ptr [r12 + 736], 1
 mov rax, qword ptr [rip + .Lx105_0]
 mov qword ptr [r12 + 744], rax
 jmp xiset104_val_done
 xiset104_val_β:
 jmp snoch0_n20_α
.Lx105_0:
 .quad .Lx105_0_s
.Lx105_0_s:
 .string "M"
xiset104_val_done:
# IR_LIT_I
bb62_α:
 mov qword ptr [r12 + 752], 6
 mov rax, qword ptr [rip + .Lx106_0]
 mov qword ptr [r12 + 760], rax
 jmp xiset104_key_done
 xiset104_key_β:
 jmp snoch0_n20_α
.Lx106_0:
 .quad 1
xiset104_key_done:
# IR_VAR
bb63_α:
 mov rdi, qword ptr [rip + .Lx108_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 768], rax
 mov qword ptr [r12 + 776], rdx
 jmp xiset104_base_done
 xiset104_base_β:
 jmp snoch0_n20_α
.Lx108_0:
 .quad .Lx108_0_s
.Lx108_0_s:
 .string "r"
xiset104_base_done:
bb64_α:
# IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set
 mov rax, qword ptr [r12 + 768]
 cmp eax, 4
 jne .Lx110_0
 mov rax, qword ptr [r12 + 752]
 cmp eax, 6
 jne .Lx110_0
 mov rsi, qword ptr [r12 + 776]
 mov rcx, qword ptr [r12 + 760]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx110_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx110_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 736]
 mov rdx, qword ptr [r12 + 744]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n20_α
.Lx110_0:
 mov rdi, qword ptr [r12 + 768]
 mov rsi, qword ptr [r12 + 776]
 mov rdx, qword ptr [r12 + 752]
 mov rcx, qword ptr [r12 + 760]
 mov r8, qword ptr [r12 + 736]
 mov r9, qword ptr [r12 + 744]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n20_α
 jmp snoch0_n20_α
 snoch0_n19_β:
 jmp snoch0_n20_α
snoch0_n20_α:
# IR_LIT_S
bb65_α:
 mov qword ptr [r12 + 784], 1
 mov rax, qword ptr [rip + .Lx112_0]
 mov qword ptr [r12 + 792], rax
 jmp xiset111_val_done
 xiset111_val_β:
 jmp snoch0_n21_α
.Lx112_0:
 .quad .Lx112_0_s
.Lx112_0_s:
 .string "CM"
xiset111_val_done:
# IR_LIT_I
bb66_α:
 mov qword ptr [r12 + 800], 6
 mov rax, qword ptr [rip + .Lx113_0]
 mov qword ptr [r12 + 808], rax
 jmp xiset111_key_done
 xiset111_key_β:
 jmp snoch0_n21_α
.Lx113_0:
 .quad 2
xiset111_key_done:
# IR_VAR
bb67_α:
 mov rdi, qword ptr [rip + .Lx115_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 816], rax
 mov qword ptr [r12 + 824], rdx
 jmp xiset111_base_done
 xiset111_base_β:
 jmp snoch0_n21_α
.Lx115_0:
 .quad .Lx115_0_s
.Lx115_0_s:
 .string "r"
xiset111_base_done:
bb68_α:
# IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set
 mov rax, qword ptr [r12 + 816]
 cmp eax, 4
 jne .Lx117_0
 mov rax, qword ptr [r12 + 800]
 cmp eax, 6
 jne .Lx117_0
 mov rsi, qword ptr [r12 + 824]
 mov rcx, qword ptr [r12 + 808]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx117_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx117_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 784]
 mov rdx, qword ptr [r12 + 792]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n21_α
.Lx117_0:
 mov rdi, qword ptr [r12 + 816]
 mov rsi, qword ptr [r12 + 824]
 mov rdx, qword ptr [r12 + 800]
 mov rcx, qword ptr [r12 + 808]
 mov r8, qword ptr [r12 + 784]
 mov r9, qword ptr [r12 + 792]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n21_α
 jmp snoch0_n21_α
 snoch0_n20_β:
 jmp snoch0_n21_α
snoch0_n21_α:
# IR_LIT_S
bb69_α:
 mov qword ptr [r12 + 832], 1
 mov rax, qword ptr [rip + .Lx119_0]
 mov qword ptr [r12 + 840], rax
 jmp xiset118_val_done
 xiset118_val_β:
 jmp snoch0_n22_α
.Lx119_0:
 .quad .Lx119_0_s
.Lx119_0_s:
 .string "D"
xiset118_val_done:
# IR_LIT_I
bb70_α:
 mov qword ptr [r12 + 848], 6
 mov rax, qword ptr [rip + .Lx120_0]
 mov qword ptr [r12 + 856], rax
 jmp xiset118_key_done
 xiset118_key_β:
 jmp snoch0_n22_α
.Lx120_0:
 .quad 3
xiset118_key_done:
# IR_VAR
bb71_α:
 mov rdi, qword ptr [rip + .Lx122_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 864], rax
 mov qword ptr [r12 + 872], rdx
 jmp xiset118_base_done
 xiset118_base_β:
 jmp snoch0_n22_α
.Lx122_0:
 .quad .Lx122_0_s
.Lx122_0_s:
 .string "r"
xiset118_base_done:
bb72_α:
# IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set
 mov rax, qword ptr [r12 + 864]
 cmp eax, 4
 jne .Lx124_0
 mov rax, qword ptr [r12 + 848]
 cmp eax, 6
 jne .Lx124_0
 mov rsi, qword ptr [r12 + 872]
 mov rcx, qword ptr [r12 + 856]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx124_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx124_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 832]
 mov rdx, qword ptr [r12 + 840]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n22_α
.Lx124_0:
 mov rdi, qword ptr [r12 + 864]
 mov rsi, qword ptr [r12 + 872]
 mov rdx, qword ptr [r12 + 848]
 mov rcx, qword ptr [r12 + 856]
 mov r8, qword ptr [r12 + 832]
 mov r9, qword ptr [r12 + 840]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n22_α
 jmp snoch0_n22_α
 snoch0_n21_β:
 jmp snoch0_n22_α
snoch0_n22_α:
# IR_LIT_S
bb73_α:
 mov qword ptr [r12 + 880], 1
 mov rax, qword ptr [rip + .Lx126_0]
 mov qword ptr [r12 + 888], rax
 jmp xiset125_val_done
 xiset125_val_β:
 jmp snoch0_n23_α
.Lx126_0:
 .quad .Lx126_0_s
.Lx126_0_s:
 .string "CD"
xiset125_val_done:
# IR_LIT_I
bb74_α:
 mov qword ptr [r12 + 896], 6
 mov rax, qword ptr [rip + .Lx127_0]
 mov qword ptr [r12 + 904], rax
 jmp xiset125_key_done
 xiset125_key_β:
 jmp snoch0_n23_α
.Lx127_0:
 .quad 4
xiset125_key_done:
# IR_VAR
bb75_α:
 mov rdi, qword ptr [rip + .Lx129_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 912], rax
 mov qword ptr [r12 + 920], rdx
 jmp xiset125_base_done
 xiset125_base_β:
 jmp snoch0_n23_α
.Lx129_0:
 .quad .Lx129_0_s
.Lx129_0_s:
 .string "r"
xiset125_base_done:
bb76_α:
# IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set
 mov rax, qword ptr [r12 + 912]
 cmp eax, 4
 jne .Lx131_0
 mov rax, qword ptr [r12 + 896]
 cmp eax, 6
 jne .Lx131_0
 mov rsi, qword ptr [r12 + 920]
 mov rcx, qword ptr [r12 + 904]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx131_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx131_0
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
 jmp snoch0_n23_α
.Lx131_0:
 mov rdi, qword ptr [r12 + 912]
 mov rsi, qword ptr [r12 + 920]
 mov rdx, qword ptr [r12 + 896]
 mov rcx, qword ptr [r12 + 904]
 mov r8, qword ptr [r12 + 880]
 mov r9, qword ptr [r12 + 888]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n23_α
 jmp snoch0_n23_α
 snoch0_n22_β:
 jmp snoch0_n23_α
snoch0_n23_α:
# IR_LIT_S
bb77_α:
 mov qword ptr [r12 + 928], 1
 mov rax, qword ptr [rip + .Lx133_0]
 mov qword ptr [r12 + 936], rax
 jmp xiset132_val_done
 xiset132_val_β:
 jmp snoch0_n24_α
.Lx133_0:
 .quad .Lx133_0_s
.Lx133_0_s:
 .string "C"
xiset132_val_done:
# IR_LIT_I
bb78_α:
 mov qword ptr [r12 + 944], 6
 mov rax, qword ptr [rip + .Lx134_0]
 mov qword ptr [r12 + 952], rax
 jmp xiset132_key_done
 xiset132_key_β:
 jmp snoch0_n24_α
.Lx134_0:
 .quad 5
xiset132_key_done:
# IR_VAR
bb79_α:
 mov rdi, qword ptr [rip + .Lx136_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 960], rax
 mov qword ptr [r12 + 968], rdx
 jmp xiset132_base_done
 xiset132_base_β:
 jmp snoch0_n24_α
.Lx136_0:
 .quad .Lx136_0_s
.Lx136_0_s:
 .string "r"
xiset132_base_done:
bb80_α:
# IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set
 mov rax, qword ptr [r12 + 960]
 cmp eax, 4
 jne .Lx138_0
 mov rax, qword ptr [r12 + 944]
 cmp eax, 6
 jne .Lx138_0
 mov rsi, qword ptr [r12 + 968]
 mov rcx, qword ptr [r12 + 952]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx138_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx138_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 928]
 mov rdx, qword ptr [r12 + 936]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n24_α
.Lx138_0:
 mov rdi, qword ptr [r12 + 960]
 mov rsi, qword ptr [r12 + 968]
 mov rdx, qword ptr [r12 + 944]
 mov rcx, qword ptr [r12 + 952]
 mov r8, qword ptr [r12 + 928]
 mov r9, qword ptr [r12 + 936]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n24_α
 jmp snoch0_n24_α
 snoch0_n23_β:
 jmp snoch0_n24_α
snoch0_n24_α:
# IR_LIT_S
bb81_α:
 mov qword ptr [r12 + 976], 1
 mov rax, qword ptr [rip + .Lx140_0]
 mov qword ptr [r12 + 984], rax
 jmp xiset139_val_done
 xiset139_val_β:
 jmp snoch0_n25_α
.Lx140_0:
 .quad .Lx140_0_s
.Lx140_0_s:
 .string "XC"
xiset139_val_done:
# IR_LIT_I
bb82_α:
 mov qword ptr [r12 + 992], 6
 mov rax, qword ptr [rip + .Lx141_0]
 mov qword ptr [r12 + 1000], rax
 jmp xiset139_key_done
 xiset139_key_β:
 jmp snoch0_n25_α
.Lx141_0:
 .quad 6
xiset139_key_done:
# IR_VAR
bb83_α:
 mov rdi, qword ptr [rip + .Lx143_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 1008], rax
 mov qword ptr [r12 + 1016], rdx
 jmp xiset139_base_done
 xiset139_base_β:
 jmp snoch0_n25_α
.Lx143_0:
 .quad .Lx143_0_s
.Lx143_0_s:
 .string "r"
xiset139_base_done:
bb84_α:
# IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set
 mov rax, qword ptr [r12 + 1008]
 cmp eax, 4
 jne .Lx145_0
 mov rax, qword ptr [r12 + 992]
 cmp eax, 6
 jne .Lx145_0
 mov rsi, qword ptr [r12 + 1016]
 mov rcx, qword ptr [r12 + 1000]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx145_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx145_0
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
 jmp snoch0_n25_α
.Lx145_0:
 mov rdi, qword ptr [r12 + 1008]
 mov rsi, qword ptr [r12 + 1016]
 mov rdx, qword ptr [r12 + 992]
 mov rcx, qword ptr [r12 + 1000]
 mov r8, qword ptr [r12 + 976]
 mov r9, qword ptr [r12 + 984]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n25_α
 jmp snoch0_n25_α
 snoch0_n24_β:
 jmp snoch0_n25_α
snoch0_n25_α:
# IR_LIT_S
bb85_α:
 mov qword ptr [r12 + 1024], 1
 mov rax, qword ptr [rip + .Lx147_0]
 mov qword ptr [r12 + 1032], rax
 jmp xiset146_val_done
 xiset146_val_β:
 jmp snoch0_n26_α
.Lx147_0:
 .quad .Lx147_0_s
.Lx147_0_s:
 .string "L"
xiset146_val_done:
# IR_LIT_I
bb86_α:
 mov qword ptr [r12 + 1040], 6
 mov rax, qword ptr [rip + .Lx148_0]
 mov qword ptr [r12 + 1048], rax
 jmp xiset146_key_done
 xiset146_key_β:
 jmp snoch0_n26_α
.Lx148_0:
 .quad 7
xiset146_key_done:
# IR_VAR
bb87_α:
 mov rdi, qword ptr [rip + .Lx150_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 1056], rax
 mov qword ptr [r12 + 1064], rdx
 jmp xiset146_base_done
 xiset146_base_β:
 jmp snoch0_n26_α
.Lx150_0:
 .quad .Lx150_0_s
.Lx150_0_s:
 .string "r"
xiset146_base_done:
bb88_α:
# IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set
 mov rax, qword ptr [r12 + 1056]
 cmp eax, 4
 jne .Lx152_0
 mov rax, qword ptr [r12 + 1040]
 cmp eax, 6
 jne .Lx152_0
 mov rsi, qword ptr [r12 + 1064]
 mov rcx, qword ptr [r12 + 1048]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx152_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx152_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 1024]
 mov rdx, qword ptr [r12 + 1032]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n26_α
.Lx152_0:
 mov rdi, qword ptr [r12 + 1056]
 mov rsi, qword ptr [r12 + 1064]
 mov rdx, qword ptr [r12 + 1040]
 mov rcx, qword ptr [r12 + 1048]
 mov r8, qword ptr [r12 + 1024]
 mov r9, qword ptr [r12 + 1032]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n26_α
 jmp snoch0_n26_α
 snoch0_n25_β:
 jmp snoch0_n26_α
snoch0_n26_α:
# IR_LIT_S
bb89_α:
 mov qword ptr [r12 + 1072], 1
 mov rax, qword ptr [rip + .Lx154_0]
 mov qword ptr [r12 + 1080], rax
 jmp xiset153_val_done
 xiset153_val_β:
 jmp snoch0_n27_α
.Lx154_0:
 .quad .Lx154_0_s
.Lx154_0_s:
 .string "XL"
xiset153_val_done:
# IR_LIT_I
bb90_α:
 mov qword ptr [r12 + 1088], 6
 mov rax, qword ptr [rip + .Lx155_0]
 mov qword ptr [r12 + 1096], rax
 jmp xiset153_key_done
 xiset153_key_β:
 jmp snoch0_n27_α
.Lx155_0:
 .quad 8
xiset153_key_done:
# IR_VAR
bb91_α:
 mov rdi, qword ptr [rip + .Lx157_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 1104], rax
 mov qword ptr [r12 + 1112], rdx
 jmp xiset153_base_done
 xiset153_base_β:
 jmp snoch0_n27_α
.Lx157_0:
 .quad .Lx157_0_s
.Lx157_0_s:
 .string "r"
xiset153_base_done:
bb92_α:
# IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set
 mov rax, qword ptr [r12 + 1104]
 cmp eax, 4
 jne .Lx159_0
 mov rax, qword ptr [r12 + 1088]
 cmp eax, 6
 jne .Lx159_0
 mov rsi, qword ptr [r12 + 1112]
 mov rcx, qword ptr [r12 + 1096]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx159_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx159_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 1072]
 mov rdx, qword ptr [r12 + 1080]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n27_α
.Lx159_0:
 mov rdi, qword ptr [r12 + 1104]
 mov rsi, qword ptr [r12 + 1112]
 mov rdx, qword ptr [r12 + 1088]
 mov rcx, qword ptr [r12 + 1096]
 mov r8, qword ptr [r12 + 1072]
 mov r9, qword ptr [r12 + 1080]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n27_α
 jmp snoch0_n27_α
 snoch0_n26_β:
 jmp snoch0_n27_α
snoch0_n27_α:
# IR_LIT_S
bb93_α:
 mov qword ptr [r12 + 1120], 1
 mov rax, qword ptr [rip + .Lx161_0]
 mov qword ptr [r12 + 1128], rax
 jmp xiset160_val_done
 xiset160_val_β:
 jmp snoch0_n28_α
.Lx161_0:
 .quad .Lx161_0_s
.Lx161_0_s:
 .string "X"
xiset160_val_done:
# IR_LIT_I
bb94_α:
 mov qword ptr [r12 + 1136], 6
 mov rax, qword ptr [rip + .Lx162_0]
 mov qword ptr [r12 + 1144], rax
 jmp xiset160_key_done
 xiset160_key_β:
 jmp snoch0_n28_α
.Lx162_0:
 .quad 9
xiset160_key_done:
# IR_VAR
bb95_α:
 mov rdi, qword ptr [rip + .Lx164_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 1152], rax
 mov qword ptr [r12 + 1160], rdx
 jmp xiset160_base_done
 xiset160_base_β:
 jmp snoch0_n28_α
.Lx164_0:
 .quad .Lx164_0_s
.Lx164_0_s:
 .string "r"
xiset160_base_done:
bb96_α:
# IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set
 mov rax, qword ptr [r12 + 1152]
 cmp eax, 4
 jne .Lx166_0
 mov rax, qword ptr [r12 + 1136]
 cmp eax, 6
 jne .Lx166_0
 mov rsi, qword ptr [r12 + 1160]
 mov rcx, qword ptr [r12 + 1144]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx166_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx166_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 1120]
 mov rdx, qword ptr [r12 + 1128]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n28_α
.Lx166_0:
 mov rdi, qword ptr [r12 + 1152]
 mov rsi, qword ptr [r12 + 1160]
 mov rdx, qword ptr [r12 + 1136]
 mov rcx, qword ptr [r12 + 1144]
 mov r8, qword ptr [r12 + 1120]
 mov r9, qword ptr [r12 + 1128]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n28_α
 jmp snoch0_n28_α
 snoch0_n27_β:
 jmp snoch0_n28_α
snoch0_n28_α:
# IR_LIT_S
bb97_α:
 mov qword ptr [r12 + 1168], 1
 mov rax, qword ptr [rip + .Lx168_0]
 mov qword ptr [r12 + 1176], rax
 jmp xiset167_val_done
 xiset167_val_β:
 jmp snoch0_n29_α
.Lx168_0:
 .quad .Lx168_0_s
.Lx168_0_s:
 .string "IX"
xiset167_val_done:
# IR_LIT_I
bb98_α:
 mov qword ptr [r12 + 1184], 6
 mov rax, qword ptr [rip + .Lx169_0]
 mov qword ptr [r12 + 1192], rax
 jmp xiset167_key_done
 xiset167_key_β:
 jmp snoch0_n29_α
.Lx169_0:
 .quad 10
xiset167_key_done:
# IR_VAR
bb99_α:
 mov rdi, qword ptr [rip + .Lx171_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 1200], rax
 mov qword ptr [r12 + 1208], rdx
 jmp xiset167_base_done
 xiset167_base_β:
 jmp snoch0_n29_α
.Lx171_0:
 .quad .Lx171_0_s
.Lx171_0_s:
 .string "r"
xiset167_base_done:
bb100_α:
# IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set
 mov rax, qword ptr [r12 + 1200]
 cmp eax, 4
 jne .Lx173_0
 mov rax, qword ptr [r12 + 1184]
 cmp eax, 6
 jne .Lx173_0
 mov rsi, qword ptr [r12 + 1208]
 mov rcx, qword ptr [r12 + 1192]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx173_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx173_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 1168]
 mov rdx, qword ptr [r12 + 1176]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n29_α
.Lx173_0:
 mov rdi, qword ptr [r12 + 1200]
 mov rsi, qword ptr [r12 + 1208]
 mov rdx, qword ptr [r12 + 1184]
 mov rcx, qword ptr [r12 + 1192]
 mov r8, qword ptr [r12 + 1168]
 mov r9, qword ptr [r12 + 1176]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n29_α
 jmp snoch0_n29_α
 snoch0_n28_β:
 jmp snoch0_n29_α
snoch0_n29_α:
# IR_LIT_S
bb101_α:
 mov qword ptr [r12 + 1216], 1
 mov rax, qword ptr [rip + .Lx175_0]
 mov qword ptr [r12 + 1224], rax
 jmp xiset174_val_done
 xiset174_val_β:
 jmp snoch0_n30_α
.Lx175_0:
 .quad .Lx175_0_s
.Lx175_0_s:
 .string "V"
xiset174_val_done:
# IR_LIT_I
bb102_α:
 mov qword ptr [r12 + 1232], 6
 mov rax, qword ptr [rip + .Lx176_0]
 mov qword ptr [r12 + 1240], rax
 jmp xiset174_key_done
 xiset174_key_β:
 jmp snoch0_n30_α
.Lx176_0:
 .quad 11
xiset174_key_done:
# IR_VAR
bb103_α:
 mov rdi, qword ptr [rip + .Lx178_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 1248], rax
 mov qword ptr [r12 + 1256], rdx
 jmp xiset174_base_done
 xiset174_base_β:
 jmp snoch0_n30_α
.Lx178_0:
 .quad .Lx178_0_s
.Lx178_0_s:
 .string "r"
xiset174_base_done:
bb104_α:
# IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set
 mov rax, qword ptr [r12 + 1248]
 cmp eax, 4
 jne .Lx180_0
 mov rax, qword ptr [r12 + 1232]
 cmp eax, 6
 jne .Lx180_0
 mov rsi, qword ptr [r12 + 1256]
 mov rcx, qword ptr [r12 + 1240]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx180_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx180_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 1216]
 mov rdx, qword ptr [r12 + 1224]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n30_α
.Lx180_0:
 mov rdi, qword ptr [r12 + 1248]
 mov rsi, qword ptr [r12 + 1256]
 mov rdx, qword ptr [r12 + 1232]
 mov rcx, qword ptr [r12 + 1240]
 mov r8, qword ptr [r12 + 1216]
 mov r9, qword ptr [r12 + 1224]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n30_α
 jmp snoch0_n30_α
 snoch0_n29_β:
 jmp snoch0_n30_α
snoch0_n30_α:
# IR_LIT_S
bb105_α:
 mov qword ptr [r12 + 1264], 1
 mov rax, qword ptr [rip + .Lx182_0]
 mov qword ptr [r12 + 1272], rax
 jmp xiset181_val_done
 xiset181_val_β:
 jmp snoch0_n31_α
.Lx182_0:
 .quad .Lx182_0_s
.Lx182_0_s:
 .string "IV"
xiset181_val_done:
# IR_LIT_I
bb106_α:
 mov qword ptr [r12 + 1280], 6
 mov rax, qword ptr [rip + .Lx183_0]
 mov qword ptr [r12 + 1288], rax
 jmp xiset181_key_done
 xiset181_key_β:
 jmp snoch0_n31_α
.Lx183_0:
 .quad 12
xiset181_key_done:
# IR_VAR
bb107_α:
 mov rdi, qword ptr [rip + .Lx185_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 1296], rax
 mov qword ptr [r12 + 1304], rdx
 jmp xiset181_base_done
 xiset181_base_β:
 jmp snoch0_n31_α
.Lx185_0:
 .quad .Lx185_0_s
.Lx185_0_s:
 .string "r"
xiset181_base_done:
bb108_α:
# IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set
 mov rax, qword ptr [r12 + 1296]
 cmp eax, 4
 jne .Lx187_0
 mov rax, qword ptr [r12 + 1280]
 cmp eax, 6
 jne .Lx187_0
 mov rsi, qword ptr [r12 + 1304]
 mov rcx, qword ptr [r12 + 1288]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx187_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx187_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 1264]
 mov rdx, qword ptr [r12 + 1272]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n31_α
.Lx187_0:
 mov rdi, qword ptr [r12 + 1296]
 mov rsi, qword ptr [r12 + 1304]
 mov rdx, qword ptr [r12 + 1280]
 mov rcx, qword ptr [r12 + 1288]
 mov r8, qword ptr [r12 + 1264]
 mov r9, qword ptr [r12 + 1272]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n31_α
 jmp snoch0_n31_α
 snoch0_n30_β:
 jmp snoch0_n31_α
snoch0_n31_α:
# IR_LIT_S
bb109_α:
 mov qword ptr [r12 + 1312], 1
 mov rax, qword ptr [rip + .Lx189_0]
 mov qword ptr [r12 + 1320], rax
 jmp xiset188_val_done
 xiset188_val_β:
 jmp snoch0_n32_α
.Lx189_0:
 .quad .Lx189_0_s
.Lx189_0_s:
 .string "I"
xiset188_val_done:
# IR_LIT_I
bb110_α:
 mov qword ptr [r12 + 1328], 6
 mov rax, qword ptr [rip + .Lx190_0]
 mov qword ptr [r12 + 1336], rax
 jmp xiset188_key_done
 xiset188_key_β:
 jmp snoch0_n32_α
.Lx190_0:
 .quad 13
xiset188_key_done:
# IR_VAR
bb111_α:
 mov rdi, qword ptr [rip + .Lx192_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 1344], rax
 mov qword ptr [r12 + 1352], rdx
 jmp xiset188_base_done
 xiset188_base_β:
 jmp snoch0_n32_α
.Lx192_0:
 .quad .Lx192_0_s
.Lx192_0_s:
 .string "r"
xiset188_base_done:
bb112_α:
# IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set
 mov rax, qword ptr [r12 + 1344]
 cmp eax, 4
 jne .Lx194_0
 mov rax, qword ptr [r12 + 1328]
 cmp eax, 6
 jne .Lx194_0
 mov rsi, qword ptr [r12 + 1352]
 mov rcx, qword ptr [r12 + 1336]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx194_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx194_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 1312]
 mov rdx, qword ptr [r12 + 1320]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n32_α
.Lx194_0:
 mov rdi, qword ptr [r12 + 1344]
 mov rsi, qword ptr [r12 + 1352]
 mov rdx, qword ptr [r12 + 1328]
 mov rcx, qword ptr [r12 + 1336]
 mov r8, qword ptr [r12 + 1312]
 mov r9, qword ptr [r12 + 1320]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n32_α
 jmp snoch0_n32_α
 snoch0_n31_β:
 jmp snoch0_n32_α
snoch0_n32_α:
# IR_LIT_scalar
bb113_α:
 jmp snoch0_n33_α
 snoch0_n32_β:
 jmp snoch0_n34_α
snoch0_n33_α:
bb114_α:
# IR_ASSIGN_LIT_I
 lea rdi, [rip + .S4]
 movabs rsi, 1
 call rt_gvar_assign_int@PLT
 jmp snoch0_n34_α
 snoch0_n33_β:
 jmp snoch0_n34_α
snoch0_n34_α:
# IR_VAR
bb115_α:
 mov rdi, qword ptr [rip + .Lx199_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 1360], rax
 mov qword ptr [r12 + 1368], rdx
 jmp xgvarg197_done
 xgvarg197_β:
 jmp snoch0_n36_α
.Lx199_0:
 .quad .Lx199_0_s
.Lx199_0_s:
 .string "n"
xgvarg197_done:
# IR_LIT_I
bb116_α:
 mov qword ptr [r12 + 1376], 6
 mov rax, qword ptr [rip + .Lx201_0]
 mov qword ptr [r12 + 1384], rax
 jmp xgvarg200_done
 xgvarg200_β:
 jmp snoch0_n36_α
.Lx201_0:
 .quad 0
xgvarg200_done:
bb117_α:
# BOX IR_CALL GT(...) inline integer relop [four-port, FAIL->ω]
   lea rdi, [rip + .S5]
 call rt_gvar_get_int@PLT
 mov qword ptr [r12 + 1408], rax
 mov rcx, 0
 mov qword ptr [r12 + 1392], 0
 mov qword ptr [r12 + 1400], 0
 mov rax, qword ptr [r12 + 1408]
 cmp rax, rcx
 jle snoch0_n36_α
 jmp snoch0_n35_α
 snoch0_n34_β:
 jmp snoch0_n36_α
snoch0_n35_α:
# IR_VAR
bb118_α:
 mov rdi, qword ptr [rip + .Lx206_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 1424], rax
 mov qword ptr [r12 + 1432], rdx
 jmp xgvarg204_done
 xgvarg204_β:
 jmp snoch0_n38_α
.Lx206_0:
 .quad .Lx206_0_s
.Lx206_0_s:
 .string "n"
xgvarg204_done:
bb119_α:
# IR_IDX: AXS inline DT_A+int fast path, else subscript_get
 lea rdi, [rip + .S4]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 1440], rax
 mov qword ptr [r12 + 1448], rdx
 lea rdi, [rip + .S2]
 call NV_GET_fn@PLT
 cmp eax, 4
 jne .Lx209_0
 mov r8, qword ptr [r12 + 1440]
 cmp r8d, 6
 jne .Lx209_0
 mov rcx, qword ptr [r12 + 1448]
 mov rsi, rdx
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx209_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx209_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 mov rax, [r11 + rcx*8]
 add r11, 8
 mov rdx, [r11 + rcx*8]
 mov qword ptr [r12 + 1456], rax
 mov qword ptr [r12 + 1464], rdx
 jmp xgvarg207_done
.Lx209_0:
 mov rdi, rax
 mov rsi, rdx
 mov rdx, qword ptr [r12 + 1440]
 mov rcx, qword ptr [r12 + 1448]
 call subscript_get@PLT
 mov qword ptr [r12 + 1456], rax
 mov qword ptr [r12 + 1464], rdx
 jmp xgvarg207_done
 xgvarg207_β:
 jmp snoch0_n38_α
xgvarg207_done:
bb120_α:
# BOX IR_CALL GE(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+1424] -> [r12+1488]
 mov rax, qword ptr [r12 + 1424]
 mov qword ptr [r12 + 1488], rax
 mov rax, qword ptr [r12 + 1432]
 mov qword ptr [r12 + 1496], rax
# marshal arg1 = producer-box slot [r12+1456] -> [r12+1504]
 mov rax, qword ptr [r12 + 1456]
 mov qword ptr [r12 + 1504], rax
 mov rax, qword ptr [r12 + 1464]
 mov qword ptr [r12 + 1512], rax
  .section .rodata
  .Lbynamefn211: .string "GE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn211]
 lea rsi, [r12 + 1488]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1472], rax
 mov qword ptr [r12 + 1480], rdx
 cmp eax, 99
 je snoch0_n38_α
 jmp snoch0_n37_α
snoch0_n35_β:
 jmp snoch0_n38_α
snoch0_n36_α:
# IR_VAR
bb121_α:
 mov rdi, qword ptr [rip + .Lx213_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 1520], rax
 mov qword ptr [r12 + 1528], rdx
 jmp snoch0_n39_α
 snoch0_n36_β:
 jmp snoch0_n40_α
.Lx213_0:
 .quad .Lx213_0_s
.Lx213_0_s:
 .string "s"
snoch0_n37_α:
# IR_VAR
bb122_α:
 mov rdi, qword ptr [rip + .Lx215_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 1536], rax
 mov qword ptr [r12 + 1544], rdx
 jmp snoch0_n41_α
 snoch0_n37_β:
 jmp snoch0_n34_α
.Lx215_0:
 .quad .Lx215_0_s
.Lx215_0_s:
 .string "n"
snoch0_n38_α:
# IR_VAR
bb123_α:
 mov rdi, qword ptr [rip + .Lx217_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 1552], rax
 mov qword ptr [r12 + 1560], rdx
 jmp snoch0_n42_α
 snoch0_n38_β:
 jmp snoch0_n34_α
.Lx217_0:
 .quad .Lx217_0_s
.Lx217_0_s:
 .string "i"
snoch0_n39_α:
bb124_α:
# IR_ASSIGN_VAR
 lea rdi, [rip + .S6]
 lea rsi, [rip + .S0]
 call rt_gvar_assign_var@PLT
 jmp snoch0_n40_α
 snoch0_n39_β:
 jmp snoch0_n40_α
snoch0_n40_α:
snoch0_n40_β:
jmp roman_γ
jmp roman_γ
snoch0_n41_α:
bb125_α:
# IR_IDX: AXS inline DT_A+int fast path, else subscript_get
 lea rdi, [rip + .S4]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 1568], rax
 mov qword ptr [r12 + 1576], rdx
 lea rdi, [rip + .S2]
 call NV_GET_fn@PLT
 cmp eax, 4
 jne .Lx220_0
 mov r8, qword ptr [r12 + 1568]
 cmp r8d, 6
 jne .Lx220_0
 mov rcx, qword ptr [r12 + 1576]
 mov rsi, rdx
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx220_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx220_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 mov rax, [r11 + rcx*8]
 add r11, 8
 mov rdx, [r11 + rcx*8]
 mov qword ptr [r12 + 1584], rax
 mov qword ptr [r12 + 1592], rdx
 jmp snoch0_n43_α
.Lx220_0:
 mov rdi, rax
 mov rsi, rdx
 mov rdx, qword ptr [r12 + 1568]
 mov rcx, qword ptr [r12 + 1576]
 call subscript_get@PLT
 mov qword ptr [r12 + 1584], rax
 mov qword ptr [r12 + 1592], rdx
 jmp snoch0_n43_α
 snoch0_n41_β:
 jmp snoch0_n34_α
snoch0_n42_α:
# IR_LIT_scalar
bb126_α:
 jmp snoch0_n44_α
 snoch0_n42_β:
 jmp snoch0_n34_α
snoch0_n43_α:
bb127_α:
# IR_IDX: AXS inline DT_A+int fast path, else subscript_get
 lea rdi, [rip + .S4]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 1600], rax
 mov qword ptr [r12 + 1608], rdx
 lea rdi, [rip + .S2]
 call NV_GET_fn@PLT
 cmp eax, 4
 jne .Lx224_0
 mov r8, qword ptr [r12 + 1600]
 cmp r8d, 6
 jne .Lx224_0
 mov rcx, qword ptr [r12 + 1608]
 mov rsi, rdx
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx224_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx224_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 mov rax, [r11 + rcx*8]
 add r11, 8
 mov rdx, [r11 + rcx*8]
 mov qword ptr [r12 + 1616], rax
 mov qword ptr [r12 + 1624], rdx
 jmp xgvidx222_done
.Lx224_0:
 mov rdi, rax
 mov rsi, rdx
 mov rdx, qword ptr [r12 + 1600]
 mov rcx, qword ptr [r12 + 1608]
 call subscript_get@PLT
 mov qword ptr [r12 + 1616], rax
 mov qword ptr [r12 + 1624], rdx
 jmp xgvidx222_done
 xgvidx222_β:
 jmp snoch0_n34_α
xgvidx222_done:
bb128_α:
# IR_BINOP_GVAR_ARITH (dynamic operands: DESCR-in DESCR-out via rt_num_arith)
 lea rdi, [rip + .S5]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 1536], rax
 mov qword ptr [r12 + 1544], rdx
 mov rdi, qword ptr [r12 + 1536]
 mov rsi, qword ptr [r12 + 1544]
 mov rdx, qword ptr [r12 + 1584]
 mov rcx, qword ptr [r12 + 1592]
 mov r8d, 1
 call rt_num_arith@PLT
 cmp eax, 99
 je snoch0_n34_α
 mov qword ptr [r12 + 1632], rax
 mov qword ptr [r12 + 1640], rdx
 jmp snoch0_n45_α
 snoch0_n43_β:
 jmp snoch0_n34_α
snoch0_n44_α:
bb129_α:
# IR_BINOP_GVAR_ARITH
 lea rdi, [rip + .S4]
 call rt_gvar_get_int@PLT
 mov rcx, 1
 add rax, rcx
 mov qword ptr [r12 + 1648], rax
 jmp snoch0_n46_α
 snoch0_n44_β:
 jmp snoch0_n34_α
snoch0_n45_α:
bb130_α:
# IR_ASSIGN
 lea rdi, [rip + .S5]
 mov rsi, qword ptr [r12 + 1632]
 call rt_gvar_assign_int@PLT
 jmp snoch0_n34_α
 snoch0_n45_β:
 jmp snoch0_n34_α
snoch0_n46_α:
bb131_α:
# IR_ASSIGN
 lea rdi, [rip + .S4]
 mov rsi, qword ptr [r12 + 1648]
 call rt_gvar_assign_int@PLT
 jmp snoch0_n34_α
 snoch0_n46_β:
 jmp snoch0_n34_α
roman_β:
jmp roman_ω
roman_γ:
mov eax, 1
xor edx, edx
pop r12
ret
roman_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
pop r12
ret
  .section .rodata
  .Lpn0: .string "roman"
  .Lpp0_0: .string "n"
  .Lpp0_1: .string "s"
  .Lpp0_2: .string "v"
  .Lpp0_3: .string "r"
  .Lpp0_4: .string "i"
  .Lpnames0:
  .quad .Lpp0_0
  .quad .Lpp0_1
  .quad .Lpp0_2
  .quad .Lpp0_3
  .quad .Lpp0_4
  .quad 0
  .section .text
  .intel_syntax noprefix
proc_startup:
  push rbp
  mov rbp, rsp
  call core_lib_init@PLT
  call rt_proc_reset@PLT
  lea rdi, [rip + .Lpn0]
  lea rsi, [rip + .Lpnames0]
  mov edx, 5
  call rt_proc_register@PLT
  lea rdi, [rip + .Lpn0]
  lea rsi, [rip + roman_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lpn0]
  mov esi, 1656
  call rt_proc_set_frame_bytes@PLT
  pop rbp
  ret
  .section .rodata
  .Lgvan0: .string "s"
  .Lgvan1: .string "v"
  .Lgvan2: .string "r"
  .Lgvan3: .string "i"
  .Lgvan4: .string "n"
  .Lgvan5: .string "roman"
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
  .section .rodata
  .Lprocn0: .string "roman"
  .align 8
__proc_names:
  .quad .Lprocn0
  .section .bss
  .align 8
__proc: .space 8, 0
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  push rbp
  mov rbp, rsp
  call proc_startup
  lea rdi, [rip + __proc]
  lea rsi, [rip + __proc_names]
  mov edx, 1
  call rt_proc_table_fill@PLT
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
snoch232_n0_α:
bb132_α:
# IR_CALL_DEFINE
 lea rdi, [rip + .S7]
 call rt_proc_define@PLT
 jmp snoch232_n1_α
 snoch232_n0_β:
 jmp snoch232_n1_α
snoch232_n1_α:
# IR_LIT_I
bb133_α:
 mov qword ptr [r12 + 16], 6
 mov rax, qword ptr [rip + .Lx235_0]
 mov qword ptr [r12 + 24], rax
 jmp xgvarg234_done
 xgvarg234_β:
 jmp snoch232_n3_α
.Lx235_0:
 .quad 1
xgvarg234_done:
bb134_α:
# BOX IR_CALL roman(...) -> rt_call_named_proc [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+16] -> [r12+48]
 mov rax, qword ptr [r12 + 16]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 24]
 mov qword ptr [r12 + 56], rax
   mov rdi, [rip + __proc + 0]
 lea rsi, [r12 + 48]
 mov edx, 1
 call rt_call_proc_direct@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je snoch232_n3_α
 jmp snoch232_n2_α
snoch232_n1_β:
 jmp snoch232_n3_α
snoch232_n2_α:
bb135_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S8]
 mov rsi, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 call rt_gvar_assign_descr@PLT
 jmp snoch232_n3_α
 snoch232_n2_β:
 jmp snoch232_n3_α
snoch232_n3_α:
# IR_LIT_I
bb136_α:
 mov qword ptr [r12 + 64], 6
 mov rax, qword ptr [rip + .Lx239_0]
 mov qword ptr [r12 + 72], rax
 jmp xgvarg238_done
 xgvarg238_β:
 jmp snoch232_n5_α
.Lx239_0:
 .quad 4
xgvarg238_done:
bb137_α:
# BOX IR_CALL roman(...) -> rt_call_named_proc [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+64] -> [r12+96]
 mov rax, qword ptr [r12 + 64]
 mov qword ptr [r12 + 96], rax
 mov rax, qword ptr [r12 + 72]
 mov qword ptr [r12 + 104], rax
   mov rdi, [rip + __proc + 0]
 lea rsi, [r12 + 96]
 mov edx, 1
 call rt_call_proc_direct@PLT
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 cmp eax, 99
 je snoch232_n5_α
 jmp snoch232_n4_α
snoch232_n3_β:
 jmp snoch232_n5_α
snoch232_n4_α:
bb138_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S8]
 mov rsi, qword ptr [r12 + 80]
 mov rdx, qword ptr [r12 + 88]
 call rt_gvar_assign_descr@PLT
 jmp snoch232_n5_α
 snoch232_n4_β:
 jmp snoch232_n5_α
snoch232_n5_α:
# IR_LIT_I
bb139_α:
 mov qword ptr [r12 + 112], 6
 mov rax, qword ptr [rip + .Lx243_0]
 mov qword ptr [r12 + 120], rax
 jmp xgvarg242_done
 xgvarg242_β:
 jmp snoch232_n7_α
.Lx243_0:
 .quad 9
xgvarg242_done:
bb140_α:
# BOX IR_CALL roman(...) -> rt_call_named_proc [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+112] -> [r12+144]
 mov rax, qword ptr [r12 + 112]
 mov qword ptr [r12 + 144], rax
 mov rax, qword ptr [r12 + 120]
 mov qword ptr [r12 + 152], rax
   mov rdi, [rip + __proc + 0]
 lea rsi, [r12 + 144]
 mov edx, 1
 call rt_call_proc_direct@PLT
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
 cmp eax, 99
 je snoch232_n7_α
 jmp snoch232_n6_α
snoch232_n5_β:
 jmp snoch232_n7_α
snoch232_n6_α:
bb141_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S8]
 mov rsi, qword ptr [r12 + 128]
 mov rdx, qword ptr [r12 + 136]
 call rt_gvar_assign_descr@PLT
 jmp snoch232_n7_α
 snoch232_n6_β:
 jmp snoch232_n7_α
snoch232_n7_α:
# IR_LIT_I
bb142_α:
 mov qword ptr [r12 + 160], 6
 mov rax, qword ptr [rip + .Lx247_0]
 mov qword ptr [r12 + 168], rax
 jmp xgvarg246_done
 xgvarg246_β:
 jmp snoch232_n9_α
.Lx247_0:
 .quad 42
xgvarg246_done:
bb143_α:
# BOX IR_CALL roman(...) -> rt_call_named_proc [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+160] -> [r12+192]
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 192], rax
 mov rax, qword ptr [r12 + 168]
 mov qword ptr [r12 + 200], rax
   mov rdi, [rip + __proc + 0]
 lea rsi, [r12 + 192]
 mov edx, 1
 call rt_call_proc_direct@PLT
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 cmp eax, 99
 je snoch232_n9_α
 jmp snoch232_n8_α
snoch232_n7_β:
 jmp snoch232_n9_α
snoch232_n8_α:
bb144_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S8]
 mov rsi, qword ptr [r12 + 176]
 mov rdx, qword ptr [r12 + 184]
 call rt_gvar_assign_descr@PLT
 jmp snoch232_n9_α
 snoch232_n8_β:
 jmp snoch232_n9_α
snoch232_n9_α:
# IR_LIT_I
bb145_α:
 mov qword ptr [r12 + 208], 6
 mov rax, qword ptr [rip + .Lx251_0]
 mov qword ptr [r12 + 216], rax
 jmp xgvarg250_done
 xgvarg250_β:
 jmp snoch232_n11_α
.Lx251_0:
 .quad 1999
xgvarg250_done:
bb146_α:
# BOX IR_CALL roman(...) -> rt_call_named_proc [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+208] -> [r12+240]
 mov rax, qword ptr [r12 + 208]
 mov qword ptr [r12 + 240], rax
 mov rax, qword ptr [r12 + 216]
 mov qword ptr [r12 + 248], rax
   mov rdi, [rip + __proc + 0]
 lea rsi, [r12 + 240]
 mov edx, 1
 call rt_call_proc_direct@PLT
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 cmp eax, 99
 je snoch232_n11_α
 jmp snoch232_n10_α
snoch232_n9_β:
 jmp snoch232_n11_α
snoch232_n10_α:
bb147_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S8]
 mov rsi, qword ptr [r12 + 224]
 mov rdx, qword ptr [r12 + 232]
 call rt_gvar_assign_descr@PLT
 jmp snoch232_n11_α
 snoch232_n10_β:
 jmp snoch232_n11_α
snoch232_n11_α:
# IR_LIT_I
bb148_α:
 mov qword ptr [r12 + 256], 6
 mov rax, qword ptr [rip + .Lx255_0]
 mov qword ptr [r12 + 264], rax
 jmp xgvarg254_done
 xgvarg254_β:
 jmp flat_γ
.Lx255_0:
 .quad 2024
xgvarg254_done:
bb149_α:
# BOX IR_CALL roman(...) -> rt_call_named_proc [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+256] -> [r12+288]
 mov rax, qword ptr [r12 + 256]
 mov qword ptr [r12 + 288], rax
 mov rax, qword ptr [r12 + 264]
 mov qword ptr [r12 + 296], rax
   mov rdi, [rip + __proc + 0]
 lea rsi, [r12 + 288]
 mov edx, 1
 call rt_call_proc_direct@PLT
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 cmp eax, 99
 je flat_γ
 jmp snoch232_n12_α
snoch232_n11_β:
 jmp flat_γ
snoch232_n12_α:
bb150_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S8]
 mov rsi, qword ptr [r12 + 272]
 mov rdx, qword ptr [r12 + 280]
 call rt_gvar_assign_descr@PLT
 jmp flat_γ
 snoch232_n12_β:
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
.S0: .string "s"
.S1: .string ""
.S2: .string "v"
.S3: .string "r"
.S4: .string "i"
.S5: .string "n"
.S6: .string "roman"
.S7: .string "roman(n)s,v,r,i"
.S8: .string "OUTPUT"
.text

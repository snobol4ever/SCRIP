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
bb6_α:
# IR_IDX_SET: AXS inline DT_A+int fast path, else subscript_set
 lea rdi, [rip + .S2]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
 movabs rax, 6
 mov qword ptr [r12 + 80], rax
 movabs rax, 1
 mov qword ptr [r12 + 88], rax
 movabs rax, 6
 mov qword ptr [r12 + 96], rax
 movabs rax, 1000
 mov qword ptr [r12 + 104], rax
 mov rax, qword ptr [r12 + 64]
 cmp eax, 4
 jne .Lx9_0
 mov rax, qword ptr [r12 + 80]
 cmp eax, 6
 jne .Lx9_0
 mov rsi, qword ptr [r12 + 72]
 mov rcx, qword ptr [r12 + 88]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx9_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx9_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 96]
 mov rdx, qword ptr [r12 + 104]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n5_α
.Lx9_0:
 mov rdi, qword ptr [r12 + 64]
 mov rsi, qword ptr [r12 + 72]
 mov rdx, qword ptr [r12 + 80]
 mov rcx, qword ptr [r12 + 88]
 mov r8, qword ptr [r12 + 96]
 mov r9, qword ptr [r12 + 104]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n5_α
 jmp snoch0_n5_α
 snoch0_n4_β:
 jmp snoch0_n5_α
snoch0_n5_α:
bb7_α:
# IR_IDX_SET: AXS inline DT_A+int fast path, else subscript_set
 lea rdi, [rip + .S2]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 movabs rax, 6
 mov qword ptr [r12 + 128], rax
 movabs rax, 2
 mov qword ptr [r12 + 136], rax
 movabs rax, 6
 mov qword ptr [r12 + 144], rax
 movabs rax, 900
 mov qword ptr [r12 + 152], rax
 mov rax, qword ptr [r12 + 112]
 cmp eax, 4
 jne .Lx11_0
 mov rax, qword ptr [r12 + 128]
 cmp eax, 6
 jne .Lx11_0
 mov rsi, qword ptr [r12 + 120]
 mov rcx, qword ptr [r12 + 136]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx11_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx11_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 144]
 mov rdx, qword ptr [r12 + 152]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n6_α
.Lx11_0:
 mov rdi, qword ptr [r12 + 112]
 mov rsi, qword ptr [r12 + 120]
 mov rdx, qword ptr [r12 + 128]
 mov rcx, qword ptr [r12 + 136]
 mov r8, qword ptr [r12 + 144]
 mov r9, qword ptr [r12 + 152]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n6_α
 jmp snoch0_n6_α
 snoch0_n5_β:
 jmp snoch0_n6_α
snoch0_n6_α:
bb8_α:
# IR_IDX_SET: AXS inline DT_A+int fast path, else subscript_set
 lea rdi, [rip + .S2]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 movabs rax, 6
 mov qword ptr [r12 + 176], rax
 movabs rax, 3
 mov qword ptr [r12 + 184], rax
 movabs rax, 6
 mov qword ptr [r12 + 192], rax
 movabs rax, 500
 mov qword ptr [r12 + 200], rax
 mov rax, qword ptr [r12 + 160]
 cmp eax, 4
 jne .Lx13_0
 mov rax, qword ptr [r12 + 176]
 cmp eax, 6
 jne .Lx13_0
 mov rsi, qword ptr [r12 + 168]
 mov rcx, qword ptr [r12 + 184]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx13_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx13_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 192]
 mov rdx, qword ptr [r12 + 200]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n7_α
.Lx13_0:
 mov rdi, qword ptr [r12 + 160]
 mov rsi, qword ptr [r12 + 168]
 mov rdx, qword ptr [r12 + 176]
 mov rcx, qword ptr [r12 + 184]
 mov r8, qword ptr [r12 + 192]
 mov r9, qword ptr [r12 + 200]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n7_α
 jmp snoch0_n7_α
 snoch0_n6_β:
 jmp snoch0_n7_α
snoch0_n7_α:
bb9_α:
# IR_IDX_SET: AXS inline DT_A+int fast path, else subscript_set
 lea rdi, [rip + .S2]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 movabs rax, 6
 mov qword ptr [r12 + 224], rax
 movabs rax, 4
 mov qword ptr [r12 + 232], rax
 movabs rax, 6
 mov qword ptr [r12 + 240], rax
 movabs rax, 400
 mov qword ptr [r12 + 248], rax
 mov rax, qword ptr [r12 + 208]
 cmp eax, 4
 jne .Lx15_0
 mov rax, qword ptr [r12 + 224]
 cmp eax, 6
 jne .Lx15_0
 mov rsi, qword ptr [r12 + 216]
 mov rcx, qword ptr [r12 + 232]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx15_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx15_0
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
 jmp snoch0_n8_α
.Lx15_0:
 mov rdi, qword ptr [r12 + 208]
 mov rsi, qword ptr [r12 + 216]
 mov rdx, qword ptr [r12 + 224]
 mov rcx, qword ptr [r12 + 232]
 mov r8, qword ptr [r12 + 240]
 mov r9, qword ptr [r12 + 248]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n8_α
 jmp snoch0_n8_α
 snoch0_n7_β:
 jmp snoch0_n8_α
snoch0_n8_α:
bb10_α:
# IR_IDX_SET: AXS inline DT_A+int fast path, else subscript_set
 lea rdi, [rip + .S2]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
 movabs rax, 6
 mov qword ptr [r12 + 272], rax
 movabs rax, 5
 mov qword ptr [r12 + 280], rax
 movabs rax, 6
 mov qword ptr [r12 + 288], rax
 movabs rax, 100
 mov qword ptr [r12 + 296], rax
 mov rax, qword ptr [r12 + 256]
 cmp eax, 4
 jne .Lx17_0
 mov rax, qword ptr [r12 + 272]
 cmp eax, 6
 jne .Lx17_0
 mov rsi, qword ptr [r12 + 264]
 mov rcx, qword ptr [r12 + 280]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx17_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx17_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 288]
 mov rdx, qword ptr [r12 + 296]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n9_α
.Lx17_0:
 mov rdi, qword ptr [r12 + 256]
 mov rsi, qword ptr [r12 + 264]
 mov rdx, qword ptr [r12 + 272]
 mov rcx, qword ptr [r12 + 280]
 mov r8, qword ptr [r12 + 288]
 mov r9, qword ptr [r12 + 296]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n9_α
 jmp snoch0_n9_α
 snoch0_n8_β:
 jmp snoch0_n9_α
snoch0_n9_α:
bb11_α:
# IR_IDX_SET: AXS inline DT_A+int fast path, else subscript_set
 lea rdi, [rip + .S2]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 304], rax
 mov qword ptr [r12 + 312], rdx
 movabs rax, 6
 mov qword ptr [r12 + 320], rax
 movabs rax, 6
 mov qword ptr [r12 + 328], rax
 movabs rax, 6
 mov qword ptr [r12 + 336], rax
 movabs rax, 90
 mov qword ptr [r12 + 344], rax
 mov rax, qword ptr [r12 + 304]
 cmp eax, 4
 jne .Lx19_0
 mov rax, qword ptr [r12 + 320]
 cmp eax, 6
 jne .Lx19_0
 mov rsi, qword ptr [r12 + 312]
 mov rcx, qword ptr [r12 + 328]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx19_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx19_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 336]
 mov rdx, qword ptr [r12 + 344]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n10_α
.Lx19_0:
 mov rdi, qword ptr [r12 + 304]
 mov rsi, qword ptr [r12 + 312]
 mov rdx, qword ptr [r12 + 320]
 mov rcx, qword ptr [r12 + 328]
 mov r8, qword ptr [r12 + 336]
 mov r9, qword ptr [r12 + 344]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n10_α
 jmp snoch0_n10_α
 snoch0_n9_β:
 jmp snoch0_n10_α
snoch0_n10_α:
bb12_α:
# IR_IDX_SET: AXS inline DT_A+int fast path, else subscript_set
 lea rdi, [rip + .S2]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 352], rax
 mov qword ptr [r12 + 360], rdx
 movabs rax, 6
 mov qword ptr [r12 + 368], rax
 movabs rax, 7
 mov qword ptr [r12 + 376], rax
 movabs rax, 6
 mov qword ptr [r12 + 384], rax
 movabs rax, 50
 mov qword ptr [r12 + 392], rax
 mov rax, qword ptr [r12 + 352]
 cmp eax, 4
 jne .Lx21_0
 mov rax, qword ptr [r12 + 368]
 cmp eax, 6
 jne .Lx21_0
 mov rsi, qword ptr [r12 + 360]
 mov rcx, qword ptr [r12 + 376]
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
 mov rax, qword ptr [r12 + 384]
 mov rdx, qword ptr [r12 + 392]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n11_α
.Lx21_0:
 mov rdi, qword ptr [r12 + 352]
 mov rsi, qword ptr [r12 + 360]
 mov rdx, qword ptr [r12 + 368]
 mov rcx, qword ptr [r12 + 376]
 mov r8, qword ptr [r12 + 384]
 mov r9, qword ptr [r12 + 392]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n11_α
 jmp snoch0_n11_α
 snoch0_n10_β:
 jmp snoch0_n11_α
snoch0_n11_α:
bb13_α:
# IR_IDX_SET: AXS inline DT_A+int fast path, else subscript_set
 lea rdi, [rip + .S2]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 400], rax
 mov qword ptr [r12 + 408], rdx
 movabs rax, 6
 mov qword ptr [r12 + 416], rax
 movabs rax, 8
 mov qword ptr [r12 + 424], rax
 movabs rax, 6
 mov qword ptr [r12 + 432], rax
 movabs rax, 40
 mov qword ptr [r12 + 440], rax
 mov rax, qword ptr [r12 + 400]
 cmp eax, 4
 jne .Lx23_0
 mov rax, qword ptr [r12 + 416]
 cmp eax, 6
 jne .Lx23_0
 mov rsi, qword ptr [r12 + 408]
 mov rcx, qword ptr [r12 + 424]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx23_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx23_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 432]
 mov rdx, qword ptr [r12 + 440]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n12_α
.Lx23_0:
 mov rdi, qword ptr [r12 + 400]
 mov rsi, qword ptr [r12 + 408]
 mov rdx, qword ptr [r12 + 416]
 mov rcx, qword ptr [r12 + 424]
 mov r8, qword ptr [r12 + 432]
 mov r9, qword ptr [r12 + 440]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n12_α
 jmp snoch0_n12_α
 snoch0_n11_β:
 jmp snoch0_n12_α
snoch0_n12_α:
bb14_α:
# IR_IDX_SET: AXS inline DT_A+int fast path, else subscript_set
 lea rdi, [rip + .S2]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 448], rax
 mov qword ptr [r12 + 456], rdx
 movabs rax, 6
 mov qword ptr [r12 + 464], rax
 movabs rax, 9
 mov qword ptr [r12 + 472], rax
 movabs rax, 6
 mov qword ptr [r12 + 480], rax
 movabs rax, 10
 mov qword ptr [r12 + 488], rax
 mov rax, qword ptr [r12 + 448]
 cmp eax, 4
 jne .Lx25_0
 mov rax, qword ptr [r12 + 464]
 cmp eax, 6
 jne .Lx25_0
 mov rsi, qword ptr [r12 + 456]
 mov rcx, qword ptr [r12 + 472]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx25_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx25_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 480]
 mov rdx, qword ptr [r12 + 488]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n13_α
.Lx25_0:
 mov rdi, qword ptr [r12 + 448]
 mov rsi, qword ptr [r12 + 456]
 mov rdx, qword ptr [r12 + 464]
 mov rcx, qword ptr [r12 + 472]
 mov r8, qword ptr [r12 + 480]
 mov r9, qword ptr [r12 + 488]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n13_α
 jmp snoch0_n13_α
 snoch0_n12_β:
 jmp snoch0_n13_α
snoch0_n13_α:
bb15_α:
# IR_IDX_SET: AXS inline DT_A+int fast path, else subscript_set
 lea rdi, [rip + .S2]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 496], rax
 mov qword ptr [r12 + 504], rdx
 movabs rax, 6
 mov qword ptr [r12 + 512], rax
 movabs rax, 10
 mov qword ptr [r12 + 520], rax
 movabs rax, 6
 mov qword ptr [r12 + 528], rax
 movabs rax, 9
 mov qword ptr [r12 + 536], rax
 mov rax, qword ptr [r12 + 496]
 cmp eax, 4
 jne .Lx27_0
 mov rax, qword ptr [r12 + 512]
 cmp eax, 6
 jne .Lx27_0
 mov rsi, qword ptr [r12 + 504]
 mov rcx, qword ptr [r12 + 520]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx27_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx27_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 528]
 mov rdx, qword ptr [r12 + 536]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n14_α
.Lx27_0:
 mov rdi, qword ptr [r12 + 496]
 mov rsi, qword ptr [r12 + 504]
 mov rdx, qword ptr [r12 + 512]
 mov rcx, qword ptr [r12 + 520]
 mov r8, qword ptr [r12 + 528]
 mov r9, qword ptr [r12 + 536]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n14_α
 jmp snoch0_n14_α
 snoch0_n13_β:
 jmp snoch0_n14_α
snoch0_n14_α:
bb16_α:
# IR_IDX_SET: AXS inline DT_A+int fast path, else subscript_set
 lea rdi, [rip + .S2]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 544], rax
 mov qword ptr [r12 + 552], rdx
 movabs rax, 6
 mov qword ptr [r12 + 560], rax
 movabs rax, 11
 mov qword ptr [r12 + 568], rax
 movabs rax, 6
 mov qword ptr [r12 + 576], rax
 movabs rax, 5
 mov qword ptr [r12 + 584], rax
 mov rax, qword ptr [r12 + 544]
 cmp eax, 4
 jne .Lx29_0
 mov rax, qword ptr [r12 + 560]
 cmp eax, 6
 jne .Lx29_0
 mov rsi, qword ptr [r12 + 552]
 mov rcx, qword ptr [r12 + 568]
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
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 576]
 mov rdx, qword ptr [r12 + 584]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n15_α
.Lx29_0:
 mov rdi, qword ptr [r12 + 544]
 mov rsi, qword ptr [r12 + 552]
 mov rdx, qword ptr [r12 + 560]
 mov rcx, qword ptr [r12 + 568]
 mov r8, qword ptr [r12 + 576]
 mov r9, qword ptr [r12 + 584]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n15_α
 jmp snoch0_n15_α
 snoch0_n14_β:
 jmp snoch0_n15_α
snoch0_n15_α:
bb17_α:
# IR_IDX_SET: AXS inline DT_A+int fast path, else subscript_set
 lea rdi, [rip + .S2]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 592], rax
 mov qword ptr [r12 + 600], rdx
 movabs rax, 6
 mov qword ptr [r12 + 608], rax
 movabs rax, 12
 mov qword ptr [r12 + 616], rax
 movabs rax, 6
 mov qword ptr [r12 + 624], rax
 movabs rax, 4
 mov qword ptr [r12 + 632], rax
 mov rax, qword ptr [r12 + 592]
 cmp eax, 4
 jne .Lx31_0
 mov rax, qword ptr [r12 + 608]
 cmp eax, 6
 jne .Lx31_0
 mov rsi, qword ptr [r12 + 600]
 mov rcx, qword ptr [r12 + 616]
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx31_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx31_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 624]
 mov rdx, qword ptr [r12 + 632]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n16_α
.Lx31_0:
 mov rdi, qword ptr [r12 + 592]
 mov rsi, qword ptr [r12 + 600]
 mov rdx, qword ptr [r12 + 608]
 mov rcx, qword ptr [r12 + 616]
 mov r8, qword ptr [r12 + 624]
 mov r9, qword ptr [r12 + 632]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n16_α
 jmp snoch0_n16_α
 snoch0_n15_β:
 jmp snoch0_n16_α
snoch0_n16_α:
bb18_α:
# IR_IDX_SET: AXS inline DT_A+int fast path, else subscript_set
 lea rdi, [rip + .S2]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 640], rax
 mov qword ptr [r12 + 648], rdx
 movabs rax, 6
 mov qword ptr [r12 + 656], rax
 movabs rax, 13
 mov qword ptr [r12 + 664], rax
 movabs rax, 6
 mov qword ptr [r12 + 672], rax
 movabs rax, 1
 mov qword ptr [r12 + 680], rax
 mov rax, qword ptr [r12 + 640]
 cmp eax, 4
 jne .Lx33_0
 mov rax, qword ptr [r12 + 656]
 cmp eax, 6
 jne .Lx33_0
 mov rsi, qword ptr [r12 + 648]
 mov rcx, qword ptr [r12 + 664]
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
 add rcx, rcx
 add rcx, rcx
 add rcx, rcx
 add r11, rcx
 mov rax, qword ptr [r12 + 672]
 mov rdx, qword ptr [r12 + 680]
 mov qword ptr [r11 + 0], rax
 mov qword ptr [r11 + 8], rdx
 jmp snoch0_n17_α
.Lx33_0:
 mov rdi, qword ptr [r12 + 640]
 mov rsi, qword ptr [r12 + 648]
 mov rdx, qword ptr [r12 + 656]
 mov rcx, qword ptr [r12 + 664]
 mov r8, qword ptr [r12 + 672]
 mov r9, qword ptr [r12 + 680]
 call subscript_set@PLT
 cmp eax, 0
 je snoch0_n17_α
 jmp snoch0_n17_α
 snoch0_n16_β:
 jmp snoch0_n17_α
snoch0_n17_α:
# IR_LIT_I
bb19_α:
 mov qword ptr [r12 + 688], 6
 mov rax, qword ptr [rip + .Lx35_0]
 mov qword ptr [r12 + 696], rax
 jmp xgvarg34_done
 xgvarg34_β:
 jmp snoch0_n19_α
.Lx35_0:
 .quad 13
xgvarg34_done:
bb20_α:
# BOX IR_CALL ARRAY(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+688] -> [r12+720]
 mov rax, qword ptr [r12 + 688]
 mov qword ptr [r12 + 720], rax
 mov rax, qword ptr [r12 + 696]
 mov qword ptr [r12 + 728], rax
  .section .rodata
  .Lbynamefn37: .string "ARRAY"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn37]
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
bb21_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S3]
 mov rsi, qword ptr [r12 + 704]
 mov rdx, qword ptr [r12 + 712]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n19_α
 snoch0_n18_β:
 jmp snoch0_n19_α
snoch0_n19_α:
# IR_LIT_scalar
bb22_α:
 jmp snoch0_n20_α
 snoch0_n19_β:
 jmp snoch0_n21_α
snoch0_n20_α:
bb23_α:
# IR_ASSIGN_LIT_I
 lea rdi, [rip + .S4]
 movabs rsi, 1
 call rt_gvar_assign_int@PLT
 jmp snoch0_n21_α
 snoch0_n20_β:
 jmp snoch0_n21_α
snoch0_n21_α:
# IR_VAR
bb24_α:
 mov rdi, qword ptr [rip + .Lx43_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 736], rax
 mov qword ptr [r12 + 744], rdx
 jmp xgvarg41_done
 xgvarg41_β:
 jmp snoch0_n23_α
.Lx43_0:
 .quad .Lx43_0_s
.Lx43_0_s:
 .string "n"
xgvarg41_done:
# IR_LIT_I
bb25_α:
 mov qword ptr [r12 + 752], 6
 mov rax, qword ptr [rip + .Lx45_0]
 mov qword ptr [r12 + 760], rax
 jmp xgvarg44_done
 xgvarg44_β:
 jmp snoch0_n23_α
.Lx45_0:
 .quad 0
xgvarg44_done:
bb26_α:
# BOX IR_CALL GT(...) inline integer relop [four-port, FAIL->ω]
   lea rdi, [rip + .S5]
 call rt_gvar_get_int@PLT
 mov qword ptr [r12 + 784], rax
 mov rcx, 0
 mov qword ptr [r12 + 768], 0
 mov qword ptr [r12 + 776], 0
 mov rax, qword ptr [r12 + 784]
 cmp rax, rcx
 jle snoch0_n23_α
 jmp snoch0_n22_α
 snoch0_n21_β:
 jmp snoch0_n23_α
snoch0_n22_α:
# IR_VAR
bb27_α:
 mov rdi, qword ptr [rip + .Lx48_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 800], rax
 mov qword ptr [r12 + 808], rdx
 jmp snoch0_n24_α
 snoch0_n22_β:
 jmp snoch0_n21_α
.Lx48_0:
 .quad .Lx48_0_s
.Lx48_0_s:
 .string "n"
snoch0_n23_α:
# IR_VAR
bb28_α:
 mov rdi, qword ptr [rip + .Lx50_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 816], rax
 mov qword ptr [r12 + 824], rdx
 jmp snoch0_n25_α
 snoch0_n23_β:
 jmp snoch0_n26_α
.Lx50_0:
 .quad .Lx50_0_s
.Lx50_0_s:
 .string "s"
snoch0_n24_α:
bb29_α:
# IR_IDX: AXS inline DT_A+int fast path, else subscript_get
 lea rdi, [rip + .S4]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 832], rax
 mov qword ptr [r12 + 840], rdx
 lea rdi, [rip + .S2]
 call NV_GET_fn@PLT
 cmp eax, 4
 jne .Lx52_0
 mov r8, qword ptr [r12 + 832]
 cmp r8d, 6
 jne .Lx52_0
 mov rcx, qword ptr [r12 + 840]
 mov rsi, rdx
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx52_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx52_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 mov rax, [r11 + rcx*8]
 add r11, 8
 mov rdx, [r11 + rcx*8]
 mov qword ptr [r12 + 848], rax
 mov qword ptr [r12 + 856], rdx
 jmp snoch0_n27_α
.Lx52_0:
 mov rdi, rax
 mov rsi, rdx
 mov rdx, qword ptr [r12 + 832]
 mov rcx, qword ptr [r12 + 840]
 call subscript_get@PLT
 mov qword ptr [r12 + 848], rax
 mov qword ptr [r12 + 856], rdx
 jmp snoch0_n27_α
 snoch0_n24_β:
 jmp snoch0_n21_α
snoch0_n25_α:
bb30_α:
# IR_ASSIGN_VAR
 lea rdi, [rip + .S6]
 lea rsi, [rip + .S0]
 call rt_gvar_assign_var@PLT
 jmp snoch0_n26_α
 snoch0_n25_β:
 jmp snoch0_n26_α
snoch0_n26_α:
snoch0_n26_β:
jmp roman_γ
jmp roman_γ
snoch0_n27_α:
bb31_α:
# IR_IDX: AXS inline DT_A+int fast path, else subscript_get
 lea rdi, [rip + .S4]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 864], rax
 mov qword ptr [r12 + 872], rdx
 lea rdi, [rip + .S2]
 call NV_GET_fn@PLT
 cmp eax, 4
 jne .Lx56_0
 mov r8, qword ptr [r12 + 864]
 cmp r8d, 6
 jne .Lx56_0
 mov rcx, qword ptr [r12 + 872]
 mov rsi, rdx
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
 mov rax, [r11 + rcx*8]
 add r11, 8
 mov rdx, [r11 + rcx*8]
 mov qword ptr [r12 + 880], rax
 mov qword ptr [r12 + 888], rdx
 jmp xgvidx54_done
.Lx56_0:
 mov rdi, rax
 mov rsi, rdx
 mov rdx, qword ptr [r12 + 864]
 mov rcx, qword ptr [r12 + 872]
 call subscript_get@PLT
 mov qword ptr [r12 + 880], rax
 mov qword ptr [r12 + 888], rdx
 jmp xgvidx54_done
 xgvidx54_β:
 jmp snoch0_n21_α
xgvidx54_done:
bb32_α:
# IR_BINOP_GVAR_ARITH (dynamic operands: DESCR-in DESCR-out via rt_num_arith)
 lea rdi, [rip + .S5]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 800], rax
 mov qword ptr [r12 + 808], rdx
 mov rdi, qword ptr [r12 + 800]
 mov rsi, qword ptr [r12 + 808]
 mov rdx, qword ptr [r12 + 848]
 mov rcx, qword ptr [r12 + 856]
 mov r8d, 1
 call rt_num_arith@PLT
 cmp eax, 99
 je snoch0_n21_α
 mov qword ptr [r12 + 896], rax
 mov qword ptr [r12 + 904], rdx
 jmp snoch0_n28_α
 snoch0_n27_β:
 jmp snoch0_n21_α
snoch0_n28_α:
bb33_α:
# IR_ASSIGN
 lea rdi, [rip + .S5]
 mov rsi, qword ptr [r12 + 896]
 mov rdx, qword ptr [r12 + 904]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n21_α
 snoch0_n28_β:
 jmp snoch0_n21_α
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
  mov esi, 912
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
  .globl main
main:
  push rbp
  mov rbp, rsp
  call proc_startup
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
snoch60_n0_α:
bb34_α:
# IR_CALL_DEFINE
 lea rdi, [rip + .S7]
 call rt_proc_define@PLT
 jmp snoch60_n1_α
 snoch60_n0_β:
 jmp snoch60_n1_α
snoch60_n1_α:
# IR_LIT_I
bb35_α:
 mov qword ptr [r12 + 16], 6
 mov rax, qword ptr [rip + .Lx63_0]
 mov qword ptr [r12 + 24], rax
 jmp xgvarg62_done
 xgvarg62_β:
 jmp snoch60_n3_α
.Lx63_0:
 .quad 1
xgvarg62_done:
bb36_α:
# BOX IR_CALL roman(...) -> rt_call_named_proc [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+16] -> [r12+48]
 mov rax, qword ptr [r12 + 16]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 24]
 mov qword ptr [r12 + 56], rax
  .section .rodata
  .Lprocfn65: .string "roman"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lprocfn65]
 lea rsi, [r12 + 48]
 mov edx, 1
 call rt_call_named_proc@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je snoch60_n3_α
 jmp snoch60_n2_α
snoch60_n1_β:
 jmp snoch60_n3_α
snoch60_n2_α:
bb37_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S8]
 mov rsi, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 call rt_gvar_assign_descr@PLT
 jmp snoch60_n3_α
 snoch60_n2_β:
 jmp snoch60_n3_α
snoch60_n3_α:
# IR_LIT_I
bb38_α:
 mov qword ptr [r12 + 64], 6
 mov rax, qword ptr [rip + .Lx68_0]
 mov qword ptr [r12 + 72], rax
 jmp xgvarg67_done
 xgvarg67_β:
 jmp snoch60_n5_α
.Lx68_0:
 .quad 4
xgvarg67_done:
bb39_α:
# BOX IR_CALL roman(...) -> rt_call_named_proc [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+64] -> [r12+96]
 mov rax, qword ptr [r12 + 64]
 mov qword ptr [r12 + 96], rax
 mov rax, qword ptr [r12 + 72]
 mov qword ptr [r12 + 104], rax
  .section .rodata
  .Lprocfn70: .string "roman"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lprocfn70]
 lea rsi, [r12 + 96]
 mov edx, 1
 call rt_call_named_proc@PLT
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 cmp eax, 99
 je snoch60_n5_α
 jmp snoch60_n4_α
snoch60_n3_β:
 jmp snoch60_n5_α
snoch60_n4_α:
bb40_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S8]
 mov rsi, qword ptr [r12 + 80]
 mov rdx, qword ptr [r12 + 88]
 call rt_gvar_assign_descr@PLT
 jmp snoch60_n5_α
 snoch60_n4_β:
 jmp snoch60_n5_α
snoch60_n5_α:
# IR_LIT_I
bb41_α:
 mov qword ptr [r12 + 112], 6
 mov rax, qword ptr [rip + .Lx73_0]
 mov qword ptr [r12 + 120], rax
 jmp xgvarg72_done
 xgvarg72_β:
 jmp snoch60_n7_α
.Lx73_0:
 .quad 9
xgvarg72_done:
bb42_α:
# BOX IR_CALL roman(...) -> rt_call_named_proc [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+112] -> [r12+144]
 mov rax, qword ptr [r12 + 112]
 mov qword ptr [r12 + 144], rax
 mov rax, qword ptr [r12 + 120]
 mov qword ptr [r12 + 152], rax
  .section .rodata
  .Lprocfn75: .string "roman"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lprocfn75]
 lea rsi, [r12 + 144]
 mov edx, 1
 call rt_call_named_proc@PLT
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
 cmp eax, 99
 je snoch60_n7_α
 jmp snoch60_n6_α
snoch60_n5_β:
 jmp snoch60_n7_α
snoch60_n6_α:
bb43_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S8]
 mov rsi, qword ptr [r12 + 128]
 mov rdx, qword ptr [r12 + 136]
 call rt_gvar_assign_descr@PLT
 jmp snoch60_n7_α
 snoch60_n6_β:
 jmp snoch60_n7_α
snoch60_n7_α:
# IR_LIT_I
bb44_α:
 mov qword ptr [r12 + 160], 6
 mov rax, qword ptr [rip + .Lx78_0]
 mov qword ptr [r12 + 168], rax
 jmp xgvarg77_done
 xgvarg77_β:
 jmp snoch60_n9_α
.Lx78_0:
 .quad 42
xgvarg77_done:
bb45_α:
# BOX IR_CALL roman(...) -> rt_call_named_proc [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+160] -> [r12+192]
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 192], rax
 mov rax, qword ptr [r12 + 168]
 mov qword ptr [r12 + 200], rax
  .section .rodata
  .Lprocfn80: .string "roman"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lprocfn80]
 lea rsi, [r12 + 192]
 mov edx, 1
 call rt_call_named_proc@PLT
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 cmp eax, 99
 je snoch60_n9_α
 jmp snoch60_n8_α
snoch60_n7_β:
 jmp snoch60_n9_α
snoch60_n8_α:
bb46_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S8]
 mov rsi, qword ptr [r12 + 176]
 mov rdx, qword ptr [r12 + 184]
 call rt_gvar_assign_descr@PLT
 jmp snoch60_n9_α
 snoch60_n8_β:
 jmp snoch60_n9_α
snoch60_n9_α:
# IR_LIT_I
bb47_α:
 mov qword ptr [r12 + 208], 6
 mov rax, qword ptr [rip + .Lx83_0]
 mov qword ptr [r12 + 216], rax
 jmp xgvarg82_done
 xgvarg82_β:
 jmp snoch60_n11_α
.Lx83_0:
 .quad 1999
xgvarg82_done:
bb48_α:
# BOX IR_CALL roman(...) -> rt_call_named_proc [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+208] -> [r12+240]
 mov rax, qword ptr [r12 + 208]
 mov qword ptr [r12 + 240], rax
 mov rax, qword ptr [r12 + 216]
 mov qword ptr [r12 + 248], rax
  .section .rodata
  .Lprocfn85: .string "roman"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lprocfn85]
 lea rsi, [r12 + 240]
 mov edx, 1
 call rt_call_named_proc@PLT
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 cmp eax, 99
 je snoch60_n11_α
 jmp snoch60_n10_α
snoch60_n9_β:
 jmp snoch60_n11_α
snoch60_n10_α:
bb49_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S8]
 mov rsi, qword ptr [r12 + 224]
 mov rdx, qword ptr [r12 + 232]
 call rt_gvar_assign_descr@PLT
 jmp snoch60_n11_α
 snoch60_n10_β:
 jmp snoch60_n11_α
snoch60_n11_α:
# IR_LIT_I
bb50_α:
 mov qword ptr [r12 + 256], 6
 mov rax, qword ptr [rip + .Lx88_0]
 mov qword ptr [r12 + 264], rax
 jmp xgvarg87_done
 xgvarg87_β:
 jmp flat_γ
.Lx88_0:
 .quad 2024
xgvarg87_done:
bb51_α:
# BOX IR_CALL roman(...) -> rt_call_named_proc [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+256] -> [r12+288]
 mov rax, qword ptr [r12 + 256]
 mov qword ptr [r12 + 288], rax
 mov rax, qword ptr [r12 + 264]
 mov qword ptr [r12 + 296], rax
  .section .rodata
  .Lprocfn90: .string "roman"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lprocfn90]
 lea rsi, [r12 + 288]
 mov edx, 1
 call rt_call_named_proc@PLT
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 cmp eax, 99
 je flat_γ
 jmp snoch60_n12_α
snoch60_n11_β:
 jmp flat_γ
snoch60_n12_α:
bb52_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S8]
 mov rsi, qword ptr [r12 + 272]
 mov rdx, qword ptr [r12 + 280]
 call rt_gvar_assign_descr@PLT
 jmp flat_γ
 snoch60_n12_β:
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

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
# IR_LIT_scalar
bb6_α:
 jmp xiset8_val_done
 xiset8_val_β:
 jmp snoch0_n5_α
xiset8_val_done:
# IR_LIT_scalar
bb7_α:
 jmp xiset8_key_done
 xiset8_key_β:
 jmp snoch0_n5_α
xiset8_key_done:
# IR_VAR
bb8_α:
 mov rdi, qword ptr [rip + .Lx12_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
 jmp xiset8_base_done
 xiset8_base_β:
 jmp snoch0_n5_α
.Lx12_0:
 .quad .Lx12_0_s
.Lx12_0_s:
 .string "v"
xiset8_base_done:
 lea rdi, [rip + .S3]
 call rt_bomb@PLT
 ud2
snoch0_n5_α:
# IR_LIT_scalar
bb10_α:
 jmp xiset14_val_done
 xiset14_val_β:
 jmp snoch0_n6_α
xiset14_val_done:
# IR_LIT_scalar
bb11_α:
 jmp xiset14_key_done
 xiset14_key_β:
 jmp snoch0_n6_α
xiset14_key_done:
# IR_VAR
bb12_α:
 mov rdi, qword ptr [rip + .Lx18_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 jmp xiset14_base_done
 xiset14_base_β:
 jmp snoch0_n6_α
.Lx18_0:
 .quad .Lx18_0_s
.Lx18_0_s:
 .string "v"
xiset14_base_done:
 lea rdi, [rip + .S3]
 call rt_bomb@PLT
 ud2
snoch0_n6_α:
# IR_LIT_scalar
bb14_α:
 jmp xiset20_val_done
 xiset20_val_β:
 jmp snoch0_n7_α
xiset20_val_done:
# IR_LIT_scalar
bb15_α:
 jmp xiset20_key_done
 xiset20_key_β:
 jmp snoch0_n7_α
xiset20_key_done:
# IR_VAR
bb16_α:
 mov rdi, qword ptr [rip + .Lx24_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 jmp xiset20_base_done
 xiset20_base_β:
 jmp snoch0_n7_α
.Lx24_0:
 .quad .Lx24_0_s
.Lx24_0_s:
 .string "v"
xiset20_base_done:
 lea rdi, [rip + .S3]
 call rt_bomb@PLT
 ud2
snoch0_n7_α:
# IR_LIT_scalar
bb18_α:
 jmp xiset26_val_done
 xiset26_val_β:
 jmp snoch0_n8_α
xiset26_val_done:
# IR_LIT_scalar
bb19_α:
 jmp xiset26_key_done
 xiset26_key_β:
 jmp snoch0_n8_α
xiset26_key_done:
# IR_VAR
bb20_α:
 mov rdi, qword ptr [rip + .Lx30_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 jmp xiset26_base_done
 xiset26_base_β:
 jmp snoch0_n8_α
.Lx30_0:
 .quad .Lx30_0_s
.Lx30_0_s:
 .string "v"
xiset26_base_done:
 lea rdi, [rip + .S3]
 call rt_bomb@PLT
 ud2
snoch0_n8_α:
# IR_LIT_scalar
bb22_α:
 jmp xiset32_val_done
 xiset32_val_β:
 jmp snoch0_n9_α
xiset32_val_done:
# IR_LIT_scalar
bb23_α:
 jmp xiset32_key_done
 xiset32_key_β:
 jmp snoch0_n9_α
xiset32_key_done:
# IR_VAR
bb24_α:
 mov rdi, qword ptr [rip + .Lx36_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
 jmp xiset32_base_done
 xiset32_base_β:
 jmp snoch0_n9_α
.Lx36_0:
 .quad .Lx36_0_s
.Lx36_0_s:
 .string "v"
xiset32_base_done:
 lea rdi, [rip + .S3]
 call rt_bomb@PLT
 ud2
snoch0_n9_α:
# IR_LIT_scalar
bb26_α:
 jmp xiset38_val_done
 xiset38_val_β:
 jmp snoch0_n10_α
xiset38_val_done:
# IR_LIT_scalar
bb27_α:
 jmp xiset38_key_done
 xiset38_key_β:
 jmp snoch0_n10_α
xiset38_key_done:
# IR_VAR
bb28_α:
 mov rdi, qword ptr [rip + .Lx42_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 jmp xiset38_base_done
 xiset38_base_β:
 jmp snoch0_n10_α
.Lx42_0:
 .quad .Lx42_0_s
.Lx42_0_s:
 .string "v"
xiset38_base_done:
 lea rdi, [rip + .S3]
 call rt_bomb@PLT
 ud2
snoch0_n10_α:
# IR_LIT_scalar
bb30_α:
 jmp xiset44_val_done
 xiset44_val_β:
 jmp snoch0_n11_α
xiset44_val_done:
# IR_LIT_scalar
bb31_α:
 jmp xiset44_key_done
 xiset44_key_β:
 jmp snoch0_n11_α
xiset44_key_done:
# IR_VAR
bb32_α:
 mov rdi, qword ptr [rip + .Lx48_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 jmp xiset44_base_done
 xiset44_base_β:
 jmp snoch0_n11_α
.Lx48_0:
 .quad .Lx48_0_s
.Lx48_0_s:
 .string "v"
xiset44_base_done:
 lea rdi, [rip + .S3]
 call rt_bomb@PLT
 ud2
snoch0_n11_α:
# IR_LIT_scalar
bb34_α:
 jmp xiset50_val_done
 xiset50_val_β:
 jmp snoch0_n12_α
xiset50_val_done:
# IR_LIT_scalar
bb35_α:
 jmp xiset50_key_done
 xiset50_key_β:
 jmp snoch0_n12_α
xiset50_key_done:
# IR_VAR
bb36_α:
 mov rdi, qword ptr [rip + .Lx54_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 jmp xiset50_base_done
 xiset50_base_β:
 jmp snoch0_n12_α
.Lx54_0:
 .quad .Lx54_0_s
.Lx54_0_s:
 .string "v"
xiset50_base_done:
 lea rdi, [rip + .S3]
 call rt_bomb@PLT
 ud2
snoch0_n12_α:
# IR_LIT_scalar
bb38_α:
 jmp xiset56_val_done
 xiset56_val_β:
 jmp snoch0_n13_α
xiset56_val_done:
# IR_LIT_scalar
bb39_α:
 jmp xiset56_key_done
 xiset56_key_β:
 jmp snoch0_n13_α
xiset56_key_done:
# IR_VAR
bb40_α:
 mov rdi, qword ptr [rip + .Lx60_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 jmp xiset56_base_done
 xiset56_base_β:
 jmp snoch0_n13_α
.Lx60_0:
 .quad .Lx60_0_s
.Lx60_0_s:
 .string "v"
xiset56_base_done:
 lea rdi, [rip + .S3]
 call rt_bomb@PLT
 ud2
snoch0_n13_α:
# IR_LIT_scalar
bb42_α:
 jmp xiset62_val_done
 xiset62_val_β:
 jmp snoch0_n14_α
xiset62_val_done:
# IR_LIT_scalar
bb43_α:
 jmp xiset62_key_done
 xiset62_key_β:
 jmp snoch0_n14_α
xiset62_key_done:
# IR_VAR
bb44_α:
 mov rdi, qword ptr [rip + .Lx66_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 jmp xiset62_base_done
 xiset62_base_β:
 jmp snoch0_n14_α
.Lx66_0:
 .quad .Lx66_0_s
.Lx66_0_s:
 .string "v"
xiset62_base_done:
 lea rdi, [rip + .S3]
 call rt_bomb@PLT
 ud2
snoch0_n14_α:
# IR_LIT_scalar
bb46_α:
 jmp xiset68_val_done
 xiset68_val_β:
 jmp snoch0_n15_α
xiset68_val_done:
# IR_LIT_scalar
bb47_α:
 jmp xiset68_key_done
 xiset68_key_β:
 jmp snoch0_n15_α
xiset68_key_done:
# IR_VAR
bb48_α:
 mov rdi, qword ptr [rip + .Lx72_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 jmp xiset68_base_done
 xiset68_base_β:
 jmp snoch0_n15_α
.Lx72_0:
 .quad .Lx72_0_s
.Lx72_0_s:
 .string "v"
xiset68_base_done:
 lea rdi, [rip + .S3]
 call rt_bomb@PLT
 ud2
snoch0_n15_α:
# IR_LIT_scalar
bb50_α:
 jmp xiset74_val_done
 xiset74_val_β:
 jmp snoch0_n16_α
xiset74_val_done:
# IR_LIT_scalar
bb51_α:
 jmp xiset74_key_done
 xiset74_key_β:
 jmp snoch0_n16_α
xiset74_key_done:
# IR_VAR
bb52_α:
 mov rdi, qword ptr [rip + .Lx78_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 jmp xiset74_base_done
 xiset74_base_β:
 jmp snoch0_n16_α
.Lx78_0:
 .quad .Lx78_0_s
.Lx78_0_s:
 .string "v"
xiset74_base_done:
 lea rdi, [rip + .S3]
 call rt_bomb@PLT
 ud2
snoch0_n16_α:
# IR_LIT_scalar
bb54_α:
 jmp xiset80_val_done
 xiset80_val_β:
 jmp snoch0_n17_α
xiset80_val_done:
# IR_LIT_scalar
bb55_α:
 jmp xiset80_key_done
 xiset80_key_β:
 jmp snoch0_n17_α
xiset80_key_done:
# IR_VAR
bb56_α:
 mov rdi, qword ptr [rip + .Lx84_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
 jmp xiset80_base_done
 xiset80_base_β:
 jmp snoch0_n17_α
.Lx84_0:
 .quad .Lx84_0_s
.Lx84_0_s:
 .string "v"
xiset80_base_done:
 lea rdi, [rip + .S3]
 call rt_bomb@PLT
 ud2
snoch0_n17_α:
# IR_LIT_I
bb58_α:
 mov qword ptr [r12 + 272], 6
 mov rax, qword ptr [rip + .Lx87_0]
 mov qword ptr [r12 + 280], rax
 jmp xgvarg86_done
 xgvarg86_β:
 jmp snoch0_n19_α
.Lx87_0:
 .quad 13
xgvarg86_done:
bb59_α:
# BOX IR_CALL ARRAY(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+272] -> [r12+304]
 mov rax, qword ptr [r12 + 272]
 mov qword ptr [r12 + 304], rax
 mov rax, qword ptr [r12 + 280]
 mov qword ptr [r12 + 312], rax
  .section .rodata
  .Lbynamefn89: .string "ARRAY"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn89]
 lea rsi, [r12 + 304]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 288], rax
 mov qword ptr [r12 + 296], rdx
 cmp eax, 99
 je snoch0_n19_α
 jmp snoch0_n18_α
snoch0_n17_β:
 jmp snoch0_n19_α
snoch0_n18_α:
bb60_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S4]
 mov rsi, qword ptr [r12 + 288]
 mov rdx, qword ptr [r12 + 296]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n19_α
 snoch0_n18_β:
 jmp snoch0_n19_α
snoch0_n19_α:
# IR_LIT_scalar
bb61_α:
 jmp snoch0_n20_α
 snoch0_n19_β:
 jmp snoch0_n21_α
snoch0_n20_α:
bb62_α:
# IR_ASSIGN_LIT_I
 lea rdi, [rip + .S5]
 movabs rsi, 1
 call rt_gvar_assign_int@PLT
 jmp snoch0_n21_α
 snoch0_n20_β:
 jmp snoch0_n21_α
snoch0_n21_α:
# IR_VAR
bb63_α:
 mov rdi, qword ptr [rip + .Lx95_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 jmp xgvarg93_done
 xgvarg93_β:
 jmp snoch0_n23_α
.Lx95_0:
 .quad .Lx95_0_s
.Lx95_0_s:
 .string "n"
xgvarg93_done:
# IR_LIT_I
bb64_α:
 mov qword ptr [r12 + 336], 6
 mov rax, qword ptr [rip + .Lx97_0]
 mov qword ptr [r12 + 344], rax
 jmp xgvarg96_done
 xgvarg96_β:
 jmp snoch0_n23_α
.Lx97_0:
 .quad 0
xgvarg96_done:
bb65_α:
# BOX IR_CALL GT(...) inline integer relop [four-port, FAIL->ω]
   lea rdi, [rip + .S6]
 call rt_gvar_get_int@PLT
 mov qword ptr [r12 + 368], rax
 mov rcx, 0
 mov qword ptr [r12 + 352], 0
 mov qword ptr [r12 + 360], 0
 mov rax, qword ptr [r12 + 368]
 cmp rax, rcx
 jle snoch0_n23_α
 jmp snoch0_n22_α
 snoch0_n21_β:
 jmp snoch0_n23_α
snoch0_n22_α:
# IR_VAR
bb66_α:
 mov rdi, qword ptr [rip + .Lx101_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 jmp snoch0_n24_α
 snoch0_n22_β:
 jmp snoch0_n21_α
.Lx101_0:
 .quad .Lx101_0_s
.Lx101_0_s:
 .string "n"
snoch0_n23_α:
# IR_VAR
bb67_α:
 mov rdi, qword ptr [rip + .Lx103_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 400], rax
 mov qword ptr [r12 + 408], rdx
 jmp snoch0_n25_α
 snoch0_n23_β:
 jmp snoch0_n26_α
.Lx103_0:
 .quad .Lx103_0_s
.Lx103_0_s:
 .string "s"
snoch0_n24_α:
bb68_α:
# IR_IDX: AXS inline DT_A+int fast path, else subscript_get
 lea rdi, [rip + .S5]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 lea rdi, [rip + .S2]
 call NV_GET_fn@PLT
 cmp eax, 4
 jne .Lx105_0
 mov r8, qword ptr [r12 + 416]
 cmp r8d, 6
 jne .Lx105_0
 mov rcx, qword ptr [r12 + 424]
 mov rsi, rdx
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
 mov rax, [r11 + rcx*8]
 add r11, 8
 mov rdx, [r11 + rcx*8]
 mov qword ptr [r12 + 432], rax
 mov qword ptr [r12 + 440], rdx
 jmp snoch0_n27_α
.Lx105_0:
 mov rdi, rax
 mov rsi, rdx
 mov rdx, qword ptr [r12 + 416]
 mov rcx, qword ptr [r12 + 424]
 call subscript_get@PLT
 mov qword ptr [r12 + 432], rax
 mov qword ptr [r12 + 440], rdx
 jmp snoch0_n27_α
 snoch0_n24_β:
 jmp snoch0_n21_α
snoch0_n25_α:
bb69_α:
# IR_ASSIGN_VAR
 lea rdi, [rip + .S7]
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
bb70_α:
# IR_IDX: AXS inline DT_A+int fast path, else subscript_get
 lea rdi, [rip + .S5]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 448], rax
 mov qword ptr [r12 + 456], rdx
 lea rdi, [rip + .S2]
 call NV_GET_fn@PLT
 cmp eax, 4
 jne .Lx109_0
 mov r8, qword ptr [r12 + 448]
 cmp r8d, 6
 jne .Lx109_0
 mov rcx, qword ptr [r12 + 456]
 mov rsi, rdx
 mov r8, qword ptr [rsi]
 sub ecx, r8d
 js .Lx109_0
 mov r9, qword ptr [rsi + 4]
 sub r9d, r8d
 cmp ecx, r9d
 jg .Lx109_0
 mov r11, qword ptr [rsi + 24]
 movsxd rcx, ecx
 add rcx, rcx
 mov rax, [r11 + rcx*8]
 add r11, 8
 mov rdx, [r11 + rcx*8]
 mov qword ptr [r12 + 464], rax
 mov qword ptr [r12 + 472], rdx
 jmp xgvidx107_done
.Lx109_0:
 mov rdi, rax
 mov rsi, rdx
 mov rdx, qword ptr [r12 + 448]
 mov rcx, qword ptr [r12 + 456]
 call subscript_get@PLT
 mov qword ptr [r12 + 464], rax
 mov qword ptr [r12 + 472], rdx
 jmp xgvidx107_done
 xgvidx107_β:
 jmp snoch0_n21_α
xgvidx107_done:
bb71_α:
# IR_BINOP_GVAR_ARITH (dynamic operands: DESCR-in DESCR-out via rt_num_arith)
 lea rdi, [rip + .S6]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 mov rdi, qword ptr [r12 + 384]
 mov rsi, qword ptr [r12 + 392]
 mov rdx, qword ptr [r12 + 432]
 mov rcx, qword ptr [r12 + 440]
 mov r8d, 1
 call rt_num_arith@PLT
 cmp eax, 99
 je snoch0_n21_α
 mov qword ptr [r12 + 480], rax
 mov qword ptr [r12 + 488], rdx
 jmp snoch0_n28_α
 snoch0_n27_β:
 jmp snoch0_n21_α
snoch0_n28_α:
bb72_α:
# IR_ASSIGN
 lea rdi, [rip + .S6]
 mov rsi, qword ptr [r12 + 480]
 call rt_gvar_assign_int@PLT
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
  mov esi, 496
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
snoch113_n0_α:
bb73_α:
# IR_CALL_DEFINE
 lea rdi, [rip + .S8]
 call rt_proc_define@PLT
 jmp snoch113_n1_α
 snoch113_n0_β:
 jmp snoch113_n1_α
snoch113_n1_α:
# IR_LIT_I
bb74_α:
 mov qword ptr [r12 + 16], 6
 mov rax, qword ptr [rip + .Lx116_0]
 mov qword ptr [r12 + 24], rax
 jmp xgvarg115_done
 xgvarg115_β:
 jmp snoch113_n3_α
.Lx116_0:
 .quad 1
xgvarg115_done:
bb75_α:
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
 je snoch113_n3_α
 jmp snoch113_n2_α
snoch113_n1_β:
 jmp snoch113_n3_α
snoch113_n2_α:
bb76_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S9]
 mov rsi, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 call rt_gvar_assign_descr@PLT
 jmp snoch113_n3_α
 snoch113_n2_β:
 jmp snoch113_n3_α
snoch113_n3_α:
# IR_LIT_I
bb77_α:
 mov qword ptr [r12 + 64], 6
 mov rax, qword ptr [rip + .Lx120_0]
 mov qword ptr [r12 + 72], rax
 jmp xgvarg119_done
 xgvarg119_β:
 jmp snoch113_n5_α
.Lx120_0:
 .quad 4
xgvarg119_done:
bb78_α:
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
 je snoch113_n5_α
 jmp snoch113_n4_α
snoch113_n3_β:
 jmp snoch113_n5_α
snoch113_n4_α:
bb79_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S9]
 mov rsi, qword ptr [r12 + 80]
 mov rdx, qword ptr [r12 + 88]
 call rt_gvar_assign_descr@PLT
 jmp snoch113_n5_α
 snoch113_n4_β:
 jmp snoch113_n5_α
snoch113_n5_α:
# IR_LIT_I
bb80_α:
 mov qword ptr [r12 + 112], 6
 mov rax, qword ptr [rip + .Lx124_0]
 mov qword ptr [r12 + 120], rax
 jmp xgvarg123_done
 xgvarg123_β:
 jmp snoch113_n7_α
.Lx124_0:
 .quad 9
xgvarg123_done:
bb81_α:
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
 je snoch113_n7_α
 jmp snoch113_n6_α
snoch113_n5_β:
 jmp snoch113_n7_α
snoch113_n6_α:
bb82_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S9]
 mov rsi, qword ptr [r12 + 128]
 mov rdx, qword ptr [r12 + 136]
 call rt_gvar_assign_descr@PLT
 jmp snoch113_n7_α
 snoch113_n6_β:
 jmp snoch113_n7_α
snoch113_n7_α:
# IR_LIT_I
bb83_α:
 mov qword ptr [r12 + 160], 6
 mov rax, qword ptr [rip + .Lx128_0]
 mov qword ptr [r12 + 168], rax
 jmp xgvarg127_done
 xgvarg127_β:
 jmp snoch113_n9_α
.Lx128_0:
 .quad 42
xgvarg127_done:
bb84_α:
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
 je snoch113_n9_α
 jmp snoch113_n8_α
snoch113_n7_β:
 jmp snoch113_n9_α
snoch113_n8_α:
bb85_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S9]
 mov rsi, qword ptr [r12 + 176]
 mov rdx, qword ptr [r12 + 184]
 call rt_gvar_assign_descr@PLT
 jmp snoch113_n9_α
 snoch113_n8_β:
 jmp snoch113_n9_α
snoch113_n9_α:
# IR_LIT_I
bb86_α:
 mov qword ptr [r12 + 208], 6
 mov rax, qword ptr [rip + .Lx132_0]
 mov qword ptr [r12 + 216], rax
 jmp xgvarg131_done
 xgvarg131_β:
 jmp snoch113_n11_α
.Lx132_0:
 .quad 1999
xgvarg131_done:
bb87_α:
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
 je snoch113_n11_α
 jmp snoch113_n10_α
snoch113_n9_β:
 jmp snoch113_n11_α
snoch113_n10_α:
bb88_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S9]
 mov rsi, qword ptr [r12 + 224]
 mov rdx, qword ptr [r12 + 232]
 call rt_gvar_assign_descr@PLT
 jmp snoch113_n11_α
 snoch113_n10_β:
 jmp snoch113_n11_α
snoch113_n11_α:
# IR_LIT_I
bb89_α:
 mov qword ptr [r12 + 256], 6
 mov rax, qword ptr [rip + .Lx136_0]
 mov qword ptr [r12 + 264], rax
 jmp xgvarg135_done
 xgvarg135_β:
 jmp flat_γ
.Lx136_0:
 .quad 2024
xgvarg135_done:
bb90_α:
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
 jmp snoch113_n12_α
snoch113_n11_β:
 jmp flat_γ
snoch113_n12_α:
bb91_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S9]
 mov rsi, qword ptr [r12 + 272]
 mov rdx, qword ptr [r12 + 280]
 call rt_gvar_assign_descr@PLT
 jmp flat_γ
 snoch113_n12_β:
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
.S3: .string "bb_idx_set: needs base/key/value operand slots ([\316\266+off] producers)"
.S4: .string "r"
.S5: .string "i"
.S6: .string "n"
.S7: .string "roman"
.S8: .string "roman(n)s,v,r,i"
.S9: .string "OUTPUT"
.text

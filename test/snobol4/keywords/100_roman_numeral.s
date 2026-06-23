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
# IR_IDX_SET (subscript_set, by-name)
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
# IR_IDX_SET (subscript_set, by-name)
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
# IR_IDX_SET (subscript_set, by-name)
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
# IR_IDX_SET (subscript_set, by-name)
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
# IR_IDX_SET (subscript_set, by-name)
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
# IR_IDX_SET (subscript_set, by-name)
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
# IR_IDX_SET (subscript_set, by-name)
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
# IR_IDX_SET (subscript_set, by-name)
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
# IR_IDX_SET (subscript_set, by-name)
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
# IR_IDX_SET (subscript_set, by-name)
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
# IR_IDX_SET (subscript_set, by-name)
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
# IR_IDX_SET (subscript_set, by-name)
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
# IR_IDX_SET (subscript_set, by-name)
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
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [r12 + 696], rax
 jmp xgvarg21_done
 xgvarg21_β:
 jmp snoch0_n19_α
.Lx22_0:
 .quad 13
xgvarg21_done:
bb20_α:
# BOX IR_CALL ARRAY(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+688] -> [r12+720]
 mov rax, qword ptr [r12 + 688]
 mov qword ptr [r12 + 720], rax
 mov rax, qword ptr [r12 + 696]
 mov qword ptr [r12 + 728], rax
  .section .rodata
  .Lbynamefn24: .string "ARRAY"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn24]
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
 mov rdi, qword ptr [rip + .Lx30_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 736], rax
 mov qword ptr [r12 + 744], rdx
 jmp xgvarg28_done
 xgvarg28_β:
 jmp snoch0_n23_α
.Lx30_0:
 .quad .Lx30_0_s
.Lx30_0_s:
 .string "n"
xgvarg28_done:
# IR_LIT_I
bb25_α:
 mov qword ptr [r12 + 752], 6
 mov rax, qword ptr [rip + .Lx32_0]
 mov qword ptr [r12 + 760], rax
 jmp xgvarg31_done
 xgvarg31_β:
 jmp snoch0_n23_α
.Lx32_0:
 .quad 0
xgvarg31_done:
bb26_α:
# BOX IR_CALL GT(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+736] -> [r12+784]
 mov rax, qword ptr [r12 + 736]
 mov qword ptr [r12 + 784], rax
 mov rax, qword ptr [r12 + 744]
 mov qword ptr [r12 + 792], rax
# marshal arg1 = producer-box slot [r12+752] -> [r12+800]
 mov rax, qword ptr [r12 + 752]
 mov qword ptr [r12 + 800], rax
 mov rax, qword ptr [r12 + 760]
 mov qword ptr [r12 + 808], rax
  .section .rodata
  .Lbynamefn34: .string "GT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn34]
 lea rsi, [r12 + 784]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 768], rax
 mov qword ptr [r12 + 776], rdx
 cmp eax, 99
 je snoch0_n23_α
 jmp snoch0_n22_α
snoch0_n21_β:
 jmp snoch0_n23_α
snoch0_n22_α:
# IR_VAR
bb27_α:
 mov rdi, qword ptr [rip + .Lx36_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 816], rax
 mov qword ptr [r12 + 824], rdx
 jmp snoch0_n24_α
 snoch0_n22_β:
 jmp snoch0_n21_α
.Lx36_0:
 .quad .Lx36_0_s
.Lx36_0_s:
 .string "n"
snoch0_n23_α:
# IR_VAR
bb28_α:
 mov rdi, qword ptr [rip + .Lx38_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 832], rax
 mov qword ptr [r12 + 840], rdx
 jmp snoch0_n25_α
 snoch0_n23_β:
 jmp snoch0_n26_α
.Lx38_0:
 .quad .Lx38_0_s
.Lx38_0_s:
 .string "s"
snoch0_n24_α:
bb29_α:
# IR_IDX (subscript_get, by-name)
 lea rdi, [rip + .S2]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 848], rax
 mov qword ptr [r12 + 856], rdx
 lea rdi, [rip + .S4]
 call NV_GET_fn@PLT
 mov rcx, rdx
 mov rdx, rax
 mov rdi, qword ptr [r12 + 848]
 mov rsi, qword ptr [r12 + 856]
 call subscript_get@PLT
 mov qword ptr [r12 + 864], rax
 mov qword ptr [r12 + 872], rdx
 jmp snoch0_n27_α
 snoch0_n24_β:
 jmp snoch0_n21_α
snoch0_n25_α:
bb30_α:
# IR_ASSIGN_VAR
 lea rdi, [rip + .S5]
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
# IR_IDX (subscript_get, by-name)
 lea rdi, [rip + .S2]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 880], rax
 mov qword ptr [r12 + 888], rdx
 lea rdi, [rip + .S4]
 call NV_GET_fn@PLT
 mov rcx, rdx
 mov rdx, rax
 mov rdi, qword ptr [r12 + 880]
 mov rsi, qword ptr [r12 + 888]
 call subscript_get@PLT
 mov qword ptr [r12 + 896], rax
 mov qword ptr [r12 + 904], rdx
 jmp xgvidx41_done
 xgvidx41_β:
 jmp snoch0_n21_α
xgvidx41_done:
bb32_α:
# IR_BINOP_GVAR_ARITH (dynamic operands: DESCR-in DESCR-out via rt_num_arith)
 lea rdi, [rip + .S6]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 816], rax
 mov qword ptr [r12 + 824], rdx
 mov rdi, qword ptr [r12 + 816]
 mov rsi, qword ptr [r12 + 824]
 mov rdx, qword ptr [r12 + 864]
 mov rcx, qword ptr [r12 + 872]
 mov r8d, 1
 call rt_num_arith@PLT
 cmp eax, 99
 je snoch0_n21_α
 mov qword ptr [r12 + 912], rax
 mov qword ptr [r12 + 920], rdx
 jmp snoch0_n28_α
 snoch0_n27_β:
 jmp snoch0_n21_α
snoch0_n28_α:
bb33_α:
# IR_ASSIGN
 lea rdi, [rip + .S6]
 mov rsi, qword ptr [r12 + 912]
 mov rdx, qword ptr [r12 + 920]
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
  .Lpnames0:
  .quad .Lpp0_0
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
  mov edx, 1
  call rt_proc_register@PLT
  lea rdi, [rip + .Lpn0]
  lea rsi, [rip + roman_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lpn0]
  mov esi, 928
  call rt_proc_set_frame_bytes@PLT
  pop rbp
  ret
  .globl main
main:
  push rbp
  mov rbp, rsp
  call proc_startup
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
snoch46_n0_α:
bb34_α:
# IR_CALL_DEFINE
 lea rdi, [rip + .S7]
 call rt_proc_define@PLT
 jmp snoch46_n1_α
 snoch46_n0_β:
 jmp snoch46_n1_α
snoch46_n1_α:
# IR_LIT_I
bb35_α:
 mov qword ptr [r12 + 16], 6
 mov rax, qword ptr [rip + .Lx49_0]
 mov qword ptr [r12 + 24], rax
 jmp xgvarg48_done
 xgvarg48_β:
 jmp snoch46_n3_α
.Lx49_0:
 .quad 1
xgvarg48_done:
bb36_α:
# BOX IR_CALL roman(...) -> rt_call_named_proc [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+16] -> [r12+48]
 mov rax, qword ptr [r12 + 16]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 24]
 mov qword ptr [r12 + 56], rax
  .section .rodata
  .Lprocfn51: .string "roman"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lprocfn51]
 lea rsi, [r12 + 48]
 mov edx, 1
 call rt_call_named_proc@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je snoch46_n3_α
 jmp snoch46_n2_α
snoch46_n1_β:
 jmp snoch46_n3_α
snoch46_n2_α:
bb37_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S8]
 mov rsi, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 call rt_gvar_assign_descr@PLT
 jmp snoch46_n3_α
 snoch46_n2_β:
 jmp snoch46_n3_α
snoch46_n3_α:
# IR_LIT_I
bb38_α:
 mov qword ptr [r12 + 64], 6
 mov rax, qword ptr [rip + .Lx54_0]
 mov qword ptr [r12 + 72], rax
 jmp xgvarg53_done
 xgvarg53_β:
 jmp snoch46_n5_α
.Lx54_0:
 .quad 4
xgvarg53_done:
bb39_α:
# BOX IR_CALL roman(...) -> rt_call_named_proc [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+64] -> [r12+96]
 mov rax, qword ptr [r12 + 64]
 mov qword ptr [r12 + 96], rax
 mov rax, qword ptr [r12 + 72]
 mov qword ptr [r12 + 104], rax
  .section .rodata
  .Lprocfn56: .string "roman"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lprocfn56]
 lea rsi, [r12 + 96]
 mov edx, 1
 call rt_call_named_proc@PLT
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 cmp eax, 99
 je snoch46_n5_α
 jmp snoch46_n4_α
snoch46_n3_β:
 jmp snoch46_n5_α
snoch46_n4_α:
bb40_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S8]
 mov rsi, qword ptr [r12 + 80]
 mov rdx, qword ptr [r12 + 88]
 call rt_gvar_assign_descr@PLT
 jmp snoch46_n5_α
 snoch46_n4_β:
 jmp snoch46_n5_α
snoch46_n5_α:
# IR_LIT_I
bb41_α:
 mov qword ptr [r12 + 112], 6
 mov rax, qword ptr [rip + .Lx59_0]
 mov qword ptr [r12 + 120], rax
 jmp xgvarg58_done
 xgvarg58_β:
 jmp snoch46_n7_α
.Lx59_0:
 .quad 9
xgvarg58_done:
bb42_α:
# BOX IR_CALL roman(...) -> rt_call_named_proc [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+112] -> [r12+144]
 mov rax, qword ptr [r12 + 112]
 mov qword ptr [r12 + 144], rax
 mov rax, qword ptr [r12 + 120]
 mov qword ptr [r12 + 152], rax
  .section .rodata
  .Lprocfn61: .string "roman"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lprocfn61]
 lea rsi, [r12 + 144]
 mov edx, 1
 call rt_call_named_proc@PLT
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
 cmp eax, 99
 je snoch46_n7_α
 jmp snoch46_n6_α
snoch46_n5_β:
 jmp snoch46_n7_α
snoch46_n6_α:
bb43_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S8]
 mov rsi, qword ptr [r12 + 128]
 mov rdx, qword ptr [r12 + 136]
 call rt_gvar_assign_descr@PLT
 jmp snoch46_n7_α
 snoch46_n6_β:
 jmp snoch46_n7_α
snoch46_n7_α:
# IR_LIT_I
bb44_α:
 mov qword ptr [r12 + 160], 6
 mov rax, qword ptr [rip + .Lx64_0]
 mov qword ptr [r12 + 168], rax
 jmp xgvarg63_done
 xgvarg63_β:
 jmp snoch46_n9_α
.Lx64_0:
 .quad 42
xgvarg63_done:
bb45_α:
# BOX IR_CALL roman(...) -> rt_call_named_proc [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+160] -> [r12+192]
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 192], rax
 mov rax, qword ptr [r12 + 168]
 mov qword ptr [r12 + 200], rax
  .section .rodata
  .Lprocfn66: .string "roman"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lprocfn66]
 lea rsi, [r12 + 192]
 mov edx, 1
 call rt_call_named_proc@PLT
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 cmp eax, 99
 je snoch46_n9_α
 jmp snoch46_n8_α
snoch46_n7_β:
 jmp snoch46_n9_α
snoch46_n8_α:
bb46_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S8]
 mov rsi, qword ptr [r12 + 176]
 mov rdx, qword ptr [r12 + 184]
 call rt_gvar_assign_descr@PLT
 jmp snoch46_n9_α
 snoch46_n8_β:
 jmp snoch46_n9_α
snoch46_n9_α:
# IR_LIT_I
bb47_α:
 mov qword ptr [r12 + 208], 6
 mov rax, qword ptr [rip + .Lx69_0]
 mov qword ptr [r12 + 216], rax
 jmp xgvarg68_done
 xgvarg68_β:
 jmp snoch46_n11_α
.Lx69_0:
 .quad 1999
xgvarg68_done:
bb48_α:
# BOX IR_CALL roman(...) -> rt_call_named_proc [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+208] -> [r12+240]
 mov rax, qword ptr [r12 + 208]
 mov qword ptr [r12 + 240], rax
 mov rax, qword ptr [r12 + 216]
 mov qword ptr [r12 + 248], rax
  .section .rodata
  .Lprocfn71: .string "roman"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lprocfn71]
 lea rsi, [r12 + 240]
 mov edx, 1
 call rt_call_named_proc@PLT
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 cmp eax, 99
 je snoch46_n11_α
 jmp snoch46_n10_α
snoch46_n9_β:
 jmp snoch46_n11_α
snoch46_n10_α:
bb49_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S8]
 mov rsi, qword ptr [r12 + 224]
 mov rdx, qword ptr [r12 + 232]
 call rt_gvar_assign_descr@PLT
 jmp snoch46_n11_α
 snoch46_n10_β:
 jmp snoch46_n11_α
snoch46_n11_α:
# IR_LIT_I
bb50_α:
 mov qword ptr [r12 + 256], 6
 mov rax, qword ptr [rip + .Lx74_0]
 mov qword ptr [r12 + 264], rax
 jmp xgvarg73_done
 xgvarg73_β:
 jmp flat_γ
.Lx74_0:
 .quad 2024
xgvarg73_done:
bb51_α:
# BOX IR_CALL roman(...) -> rt_call_named_proc [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+256] -> [r12+288]
 mov rax, qword ptr [r12 + 256]
 mov qword ptr [r12 + 288], rax
 mov rax, qword ptr [r12 + 264]
 mov qword ptr [r12 + 296], rax
  .section .rodata
  .Lprocfn76: .string "roman"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lprocfn76]
 lea rsi, [r12 + 288]
 mov edx, 1
 call rt_call_named_proc@PLT
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 cmp eax, 99
 je flat_γ
 jmp snoch46_n12_α
snoch46_n11_β:
 jmp flat_γ
snoch46_n12_α:
bb52_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S8]
 mov rsi, qword ptr [r12 + 272]
 mov rdx, qword ptr [r12 + 280]
 call rt_gvar_assign_descr@PLT
 jmp flat_γ
 snoch46_n12_β:
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
.S5: .string "roman"
.S6: .string "n"
.S7: .string "roman(n)s,v,r,i"
.S8: .string "OUTPUT"
.text

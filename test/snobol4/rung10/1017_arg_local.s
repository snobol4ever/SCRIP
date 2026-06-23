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
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 24], rax
 jmp xgvarg1_done
 xgvarg1_β:
 jmp snoch0_n1_α
.Lx2_0:
 .quad .Lx2_0_s
.Lx2_0_s:
 .string "jlab(a,b,c)d,e,f"
xgvarg1_done:
bb2_α:
# BOX IR_CALL define(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+16] -> [r12+48]
 mov rax, qword ptr [r12 + 16]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 24]
 mov qword ptr [r12 + 56], rax
  .section .rodata
  .Lbynamefn4: .string "define"
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
xargsub7_n0_α:
# IR_VAR
bb3_α:
 mov rdi, qword ptr [rip + .Lx9_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
 jmp xargsub7_n1_α
 xargsub7_n0_β:
 jmp snoch0_n3_α
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "jlab"
xargsub7_n1_α:
# IR_VAR
bb4_α:
 mov rdi, qword ptr [rip + .Lx12_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 jmp xunop10_arg_done
 xunop10_arg_β:
 jmp snoch0_n3_α
.Lx12_0:
 .quad .Lx12_0_s
.Lx12_0_s:
 .string "jlab"
xunop10_arg_done:
jmp xgvarg6_done
xargsub7_n1_β:
jmp snoch0_n3_α
xgvarg6_done:
# IR_LIT_I
bb5_α:
 mov qword ptr [r12 + 96], 6
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [r12 + 104], rax
 jmp xgvarg13_done
 xgvarg13_β:
 jmp snoch0_n3_α
.Lx14_0:
 .quad 1
xgvarg13_done:
bb6_α:
# BOX IR_CALL arg(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = gvar NV_GET -> [r12+128]
   lea rdi, [rip + .S0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
# marshal arg1 = producer-box slot [r12+96] -> [r12+144]
 mov rax, qword ptr [r12 + 96]
 mov qword ptr [r12 + 144], rax
 mov rax, qword ptr [r12 + 104]
 mov qword ptr [r12 + 152], rax
  .section .rodata
  .Lbynamefn16: .string "arg"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn16]
 lea rsi, [r12 + 128]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 cmp eax, 99
 je snoch0_n3_α
 jmp xgvarg5_done
xgvarg5_β:
 jmp snoch0_n3_α
xgvarg5_done:
# IR_LIT_S
bb7_α:
 mov qword ptr [r12 + 160], 1
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [r12 + 168], rax
 jmp xgvarg17_done
 xgvarg17_β:
 jmp snoch0_n3_α
.Lx18_0:
 .quad .Lx18_0_s
.Lx18_0_s:
 .string "A"
xgvarg17_done:
bb8_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+112] -> [r12+192]
 mov rax, qword ptr [r12 + 112]
 mov qword ptr [r12 + 192], rax
 mov rax, qword ptr [r12 + 120]
 mov qword ptr [r12 + 200], rax
# marshal arg1 = producer-box slot [r12+160] -> [r12+208]
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 208], rax
 mov rax, qword ptr [r12 + 168]
 mov qword ptr [r12 + 216], rax
  .section .rodata
  .Lbynamefn20: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn20]
 lea rsi, [r12 + 192]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 cmp eax, 99
 je snoch0_n3_α
 jmp snoch0_n2_α
snoch0_n1_β:
 jmp snoch0_n3_α
snoch0_n2_α:
# IR_LIT_scalar
bb9_α:
 jmp snoch0_n4_α
 snoch0_n2_β:
 jmp flat_γ
snoch0_n3_α:
xargsub24_n0_α:
# IR_VAR
bb10_α:
 mov rdi, qword ptr [rip + .Lx26_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 jmp xargsub24_n1_α
 xargsub24_n0_β:
 jmp snoch0_n6_α
.Lx26_0:
 .quad .Lx26_0_s
.Lx26_0_s:
 .string "jlab"
xargsub24_n1_α:
# IR_VAR
bb11_α:
 mov rdi, qword ptr [rip + .Lx29_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 jmp xunop27_arg_done
 xunop27_arg_β:
 jmp snoch0_n6_α
.Lx29_0:
 .quad .Lx29_0_s
.Lx29_0_s:
 .string "jlab"
xunop27_arg_done:
jmp xgvarg23_done
xargsub24_n1_β:
jmp snoch0_n6_α
xgvarg23_done:
# IR_LIT_I
bb12_α:
 mov qword ptr [r12 + 256], 6
 mov rax, qword ptr [rip + .Lx31_0]
 mov qword ptr [r12 + 264], rax
 jmp xgvarg30_done
 xgvarg30_β:
 jmp snoch0_n6_α
.Lx31_0:
 .quad 3
xgvarg30_done:
bb13_α:
# BOX IR_CALL arg(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = gvar NV_GET -> [r12+288]
   lea rdi, [rip + .S0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 288], rax
 mov qword ptr [r12 + 296], rdx
# marshal arg1 = producer-box slot [r12+256] -> [r12+304]
 mov rax, qword ptr [r12 + 256]
 mov qword ptr [r12 + 304], rax
 mov rax, qword ptr [r12 + 264]
 mov qword ptr [r12 + 312], rax
  .section .rodata
  .Lbynamefn33: .string "arg"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn33]
 lea rsi, [r12 + 288]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 cmp eax, 99
 je snoch0_n6_α
 jmp xgvarg22_done
xgvarg22_β:
 jmp snoch0_n6_α
xgvarg22_done:
# IR_LIT_S
bb14_α:
 mov qword ptr [r12 + 320], 1
 mov rax, qword ptr [rip + .Lx35_0]
 mov qword ptr [r12 + 328], rax
 jmp xgvarg34_done
 xgvarg34_β:
 jmp snoch0_n6_α
.Lx35_0:
 .quad .Lx35_0_s
.Lx35_0_s:
 .string "C"
xgvarg34_done:
bb15_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+272] -> [r12+352]
 mov rax, qword ptr [r12 + 272]
 mov qword ptr [r12 + 352], rax
 mov rax, qword ptr [r12 + 280]
 mov qword ptr [r12 + 360], rax
# marshal arg1 = producer-box slot [r12+320] -> [r12+368]
 mov rax, qword ptr [r12 + 320]
 mov qword ptr [r12 + 368], rax
 mov rax, qword ptr [r12 + 328]
 mov qword ptr [r12 + 376], rax
  .section .rodata
  .Lbynamefn37: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn37]
 lea rsi, [r12 + 352]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 cmp eax, 99
 je snoch0_n6_α
 jmp snoch0_n5_α
snoch0_n3_β:
 jmp snoch0_n6_α
snoch0_n4_α:
bb16_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S2]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n4_β:
 jmp flat_γ
snoch0_n5_α:
# IR_LIT_scalar
bb17_α:
 jmp snoch0_n7_α
 snoch0_n5_β:
 jmp flat_γ
snoch0_n6_α:
xargsub41_n0_α:
# IR_VAR
bb18_α:
 mov rdi, qword ptr [rip + .Lx43_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 jmp xargsub41_n1_α
 xargsub41_n0_β:
 jmp snoch0_n9_α
.Lx43_0:
 .quad .Lx43_0_s
.Lx43_0_s:
 .string "jlab"
xargsub41_n1_α:
# IR_VAR
bb19_α:
 mov rdi, qword ptr [rip + .Lx46_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 400], rax
 mov qword ptr [r12 + 408], rdx
 jmp xunop44_arg_done
 xunop44_arg_β:
 jmp snoch0_n9_α
.Lx46_0:
 .quad .Lx46_0_s
.Lx46_0_s:
 .string "jlab"
xunop44_arg_done:
jmp xgvarg40_done
xargsub41_n1_β:
jmp snoch0_n9_α
xgvarg40_done:
# IR_LIT_I
bb20_α:
 mov qword ptr [r12 + 416], 6
 mov rax, qword ptr [rip + .Lx48_0]
 mov qword ptr [r12 + 424], rax
 jmp xgvarg47_done
 xgvarg47_β:
 jmp snoch0_n9_α
.Lx48_0:
 .quad 0
xgvarg47_done:
bb21_α:
# BOX IR_CALL arg(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = gvar NV_GET -> [r12+448]
   lea rdi, [rip + .S0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 448], rax
 mov qword ptr [r12 + 456], rdx
# marshal arg1 = producer-box slot [r12+416] -> [r12+464]
 mov rax, qword ptr [r12 + 416]
 mov qword ptr [r12 + 464], rax
 mov rax, qword ptr [r12 + 424]
 mov qword ptr [r12 + 472], rax
  .section .rodata
  .Lbynamefn50: .string "arg"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn50]
 lea rsi, [r12 + 448]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 432], rax
 mov qword ptr [r12 + 440], rdx
 cmp eax, 99
 je snoch0_n9_α
 jmp snoch0_n8_α
snoch0_n6_β:
 jmp snoch0_n9_α
snoch0_n7_α:
bb22_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S3]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n7_β:
 jmp flat_γ
snoch0_n8_α:
# IR_LIT_scalar
bb23_α:
 jmp snoch0_n10_α
 snoch0_n8_β:
 jmp flat_γ
snoch0_n9_α:
xargsub54_n0_α:
# IR_VAR
bb24_α:
 mov rdi, qword ptr [rip + .Lx56_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 480], rax
 mov qword ptr [r12 + 488], rdx
 jmp xargsub54_n1_α
 xargsub54_n0_β:
 jmp snoch0_n12_α
.Lx56_0:
 .quad .Lx56_0_s
.Lx56_0_s:
 .string "jlab"
xargsub54_n1_α:
# IR_VAR
bb25_α:
 mov rdi, qword ptr [rip + .Lx59_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 496], rax
 mov qword ptr [r12 + 504], rdx
 jmp xunop57_arg_done
 xunop57_arg_β:
 jmp snoch0_n12_α
.Lx59_0:
 .quad .Lx59_0_s
.Lx59_0_s:
 .string "jlab"
xunop57_arg_done:
jmp xgvarg53_done
xargsub54_n1_β:
jmp snoch0_n12_α
xgvarg53_done:
# IR_LIT_I
bb26_α:
 mov qword ptr [r12 + 512], 6
 mov rax, qword ptr [rip + .Lx61_0]
 mov qword ptr [r12 + 520], rax
 jmp xgvarg60_done
 xgvarg60_β:
 jmp snoch0_n12_α
.Lx61_0:
 .quad 4
xgvarg60_done:
bb27_α:
# BOX IR_CALL arg(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = gvar NV_GET -> [r12+544]
   lea rdi, [rip + .S0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 544], rax
 mov qword ptr [r12 + 552], rdx
# marshal arg1 = producer-box slot [r12+512] -> [r12+560]
 mov rax, qword ptr [r12 + 512]
 mov qword ptr [r12 + 560], rax
 mov rax, qword ptr [r12 + 520]
 mov qword ptr [r12 + 568], rax
  .section .rodata
  .Lbynamefn63: .string "arg"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn63]
 lea rsi, [r12 + 544]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 528], rax
 mov qword ptr [r12 + 536], rdx
 cmp eax, 99
 je snoch0_n12_α
 jmp snoch0_n11_α
snoch0_n9_β:
 jmp snoch0_n12_α
snoch0_n10_α:
bb28_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S4]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n10_β:
 jmp flat_γ
snoch0_n11_α:
# IR_LIT_scalar
bb29_α:
 jmp snoch0_n13_α
 snoch0_n11_β:
 jmp flat_γ
snoch0_n12_α:
xargsub68_n0_α:
# IR_VAR
bb30_α:
 mov rdi, qword ptr [rip + .Lx70_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 576], rax
 mov qword ptr [r12 + 584], rdx
 jmp xargsub68_n1_α
 xargsub68_n0_β:
 jmp snoch0_n15_α
.Lx70_0:
 .quad .Lx70_0_s
.Lx70_0_s:
 .string "jlab"
xargsub68_n1_α:
# IR_VAR
bb31_α:
 mov rdi, qword ptr [rip + .Lx73_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 592], rax
 mov qword ptr [r12 + 600], rdx
 jmp xunop71_arg_done
 xunop71_arg_β:
 jmp snoch0_n15_α
.Lx73_0:
 .quad .Lx73_0_s
.Lx73_0_s:
 .string "jlab"
xunop71_arg_done:
jmp xgvarg67_done
xargsub68_n1_β:
jmp snoch0_n15_α
xgvarg67_done:
# IR_LIT_I
bb32_α:
 mov qword ptr [r12 + 608], 6
 mov rax, qword ptr [rip + .Lx75_0]
 mov qword ptr [r12 + 616], rax
 jmp xgvarg74_done
 xgvarg74_β:
 jmp snoch0_n15_α
.Lx75_0:
 .quad 1
xgvarg74_done:
bb33_α:
# BOX IR_CALL local(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = gvar NV_GET -> [r12+640]
   lea rdi, [rip + .S0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 640], rax
 mov qword ptr [r12 + 648], rdx
# marshal arg1 = producer-box slot [r12+608] -> [r12+656]
 mov rax, qword ptr [r12 + 608]
 mov qword ptr [r12 + 656], rax
 mov rax, qword ptr [r12 + 616]
 mov qword ptr [r12 + 664], rax
  .section .rodata
  .Lbynamefn77: .string "local"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn77]
 lea rsi, [r12 + 640]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 624], rax
 mov qword ptr [r12 + 632], rdx
 cmp eax, 99
 je snoch0_n15_α
 jmp xgvarg66_done
xgvarg66_β:
 jmp snoch0_n15_α
xgvarg66_done:
# IR_LIT_S
bb34_α:
 mov qword ptr [r12 + 672], 1
 mov rax, qword ptr [rip + .Lx79_0]
 mov qword ptr [r12 + 680], rax
 jmp xgvarg78_done
 xgvarg78_β:
 jmp snoch0_n15_α
.Lx79_0:
 .quad .Lx79_0_s
.Lx79_0_s:
 .string "D"
xgvarg78_done:
bb35_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+624] -> [r12+704]
 mov rax, qword ptr [r12 + 624]
 mov qword ptr [r12 + 704], rax
 mov rax, qword ptr [r12 + 632]
 mov qword ptr [r12 + 712], rax
# marshal arg1 = producer-box slot [r12+672] -> [r12+720]
 mov rax, qword ptr [r12 + 672]
 mov qword ptr [r12 + 720], rax
 mov rax, qword ptr [r12 + 680]
 mov qword ptr [r12 + 728], rax
  .section .rodata
  .Lbynamefn81: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn81]
 lea rsi, [r12 + 704]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 688], rax
 mov qword ptr [r12 + 696], rdx
 cmp eax, 99
 je snoch0_n15_α
 jmp snoch0_n14_α
snoch0_n12_β:
 jmp snoch0_n15_α
snoch0_n13_α:
bb36_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S5]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n13_β:
 jmp flat_γ
snoch0_n14_α:
# IR_LIT_scalar
bb37_α:
 jmp snoch0_n16_α
 snoch0_n14_β:
 jmp flat_γ
snoch0_n15_α:
xargsub86_n0_α:
# IR_VAR
bb38_α:
 mov rdi, qword ptr [rip + .Lx88_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 736], rax
 mov qword ptr [r12 + 744], rdx
 jmp xargsub86_n1_α
 xargsub86_n0_β:
 jmp snoch0_n18_α
.Lx88_0:
 .quad .Lx88_0_s
.Lx88_0_s:
 .string "jlab"
xargsub86_n1_α:
# IR_VAR
bb39_α:
 mov rdi, qword ptr [rip + .Lx91_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 752], rax
 mov qword ptr [r12 + 760], rdx
 jmp xunop89_arg_done
 xunop89_arg_β:
 jmp snoch0_n18_α
.Lx91_0:
 .quad .Lx91_0_s
.Lx91_0_s:
 .string "jlab"
xunop89_arg_done:
jmp xgvarg85_done
xargsub86_n1_β:
jmp snoch0_n18_α
xgvarg85_done:
# IR_LIT_I
bb40_α:
 mov qword ptr [r12 + 768], 6
 mov rax, qword ptr [rip + .Lx93_0]
 mov qword ptr [r12 + 776], rax
 jmp xgvarg92_done
 xgvarg92_β:
 jmp snoch0_n18_α
.Lx93_0:
 .quad 3
xgvarg92_done:
bb41_α:
# BOX IR_CALL local(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = gvar NV_GET -> [r12+800]
   lea rdi, [rip + .S0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 800], rax
 mov qword ptr [r12 + 808], rdx
# marshal arg1 = producer-box slot [r12+768] -> [r12+816]
 mov rax, qword ptr [r12 + 768]
 mov qword ptr [r12 + 816], rax
 mov rax, qword ptr [r12 + 776]
 mov qword ptr [r12 + 824], rax
  .section .rodata
  .Lbynamefn95: .string "local"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn95]
 lea rsi, [r12 + 800]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 784], rax
 mov qword ptr [r12 + 792], rdx
 cmp eax, 99
 je snoch0_n18_α
 jmp xgvarg84_done
xgvarg84_β:
 jmp snoch0_n18_α
xgvarg84_done:
# IR_LIT_S
bb42_α:
 mov qword ptr [r12 + 832], 1
 mov rax, qword ptr [rip + .Lx97_0]
 mov qword ptr [r12 + 840], rax
 jmp xgvarg96_done
 xgvarg96_β:
 jmp snoch0_n18_α
.Lx97_0:
 .quad .Lx97_0_s
.Lx97_0_s:
 .string "F"
xgvarg96_done:
bb43_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+784] -> [r12+864]
 mov rax, qword ptr [r12 + 784]
 mov qword ptr [r12 + 864], rax
 mov rax, qword ptr [r12 + 792]
 mov qword ptr [r12 + 872], rax
# marshal arg1 = producer-box slot [r12+832] -> [r12+880]
 mov rax, qword ptr [r12 + 832]
 mov qword ptr [r12 + 880], rax
 mov rax, qword ptr [r12 + 840]
 mov qword ptr [r12 + 888], rax
  .section .rodata
  .Lbynamefn99: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn99]
 lea rsi, [r12 + 864]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 848], rax
 mov qword ptr [r12 + 856], rdx
 cmp eax, 99
 je snoch0_n18_α
 jmp snoch0_n17_α
snoch0_n15_β:
 jmp snoch0_n18_α
snoch0_n16_α:
bb44_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S6]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n16_β:
 jmp flat_γ
snoch0_n17_α:
# IR_LIT_scalar
bb45_α:
 jmp snoch0_n19_α
 snoch0_n17_β:
 jmp flat_γ
snoch0_n18_α:
xargsub103_n0_α:
# IR_VAR
bb46_α:
 mov rdi, qword ptr [rip + .Lx105_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 896], rax
 mov qword ptr [r12 + 904], rdx
 jmp xargsub103_n1_α
 xargsub103_n0_β:
 jmp snoch0_n21_α
.Lx105_0:
 .quad .Lx105_0_s
.Lx105_0_s:
 .string "jlab"
xargsub103_n1_α:
# IR_VAR
bb47_α:
 mov rdi, qword ptr [rip + .Lx108_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 912], rax
 mov qword ptr [r12 + 920], rdx
 jmp xunop106_arg_done
 xunop106_arg_β:
 jmp snoch0_n21_α
.Lx108_0:
 .quad .Lx108_0_s
.Lx108_0_s:
 .string "jlab"
xunop106_arg_done:
jmp xgvarg102_done
xargsub103_n1_β:
jmp snoch0_n21_α
xgvarg102_done:
# IR_LIT_I
bb48_α:
 mov qword ptr [r12 + 928], 6
 mov rax, qword ptr [rip + .Lx110_0]
 mov qword ptr [r12 + 936], rax
 jmp xgvarg109_done
 xgvarg109_β:
 jmp snoch0_n21_α
.Lx110_0:
 .quad 0
xgvarg109_done:
bb49_α:
# BOX IR_CALL local(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = gvar NV_GET -> [r12+960]
   lea rdi, [rip + .S0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 960], rax
 mov qword ptr [r12 + 968], rdx
# marshal arg1 = producer-box slot [r12+928] -> [r12+976]
 mov rax, qword ptr [r12 + 928]
 mov qword ptr [r12 + 976], rax
 mov rax, qword ptr [r12 + 936]
 mov qword ptr [r12 + 984], rax
  .section .rodata
  .Lbynamefn112: .string "local"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn112]
 lea rsi, [r12 + 960]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 944], rax
 mov qword ptr [r12 + 952], rdx
 cmp eax, 99
 je snoch0_n21_α
 jmp snoch0_n20_α
snoch0_n18_β:
 jmp snoch0_n21_α
snoch0_n19_α:
bb50_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S7]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n19_β:
 jmp flat_γ
snoch0_n20_α:
# IR_LIT_scalar
bb51_α:
 jmp snoch0_n22_α
 snoch0_n20_β:
 jmp flat_γ
snoch0_n21_α:
xargsub116_n0_α:
# IR_VAR
bb52_α:
 mov rdi, qword ptr [rip + .Lx118_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 992], rax
 mov qword ptr [r12 + 1000], rdx
 jmp xargsub116_n1_α
 xargsub116_n0_β:
 jmp snoch0_n24_α
.Lx118_0:
 .quad .Lx118_0_s
.Lx118_0_s:
 .string "jlab"
xargsub116_n1_α:
# IR_VAR
bb53_α:
 mov rdi, qword ptr [rip + .Lx121_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 1008], rax
 mov qword ptr [r12 + 1016], rdx
 jmp xunop119_arg_done
 xunop119_arg_β:
 jmp snoch0_n24_α
.Lx121_0:
 .quad .Lx121_0_s
.Lx121_0_s:
 .string "jlab"
xunop119_arg_done:
jmp xgvarg115_done
xargsub116_n1_β:
jmp snoch0_n24_α
xgvarg115_done:
# IR_LIT_I
bb54_α:
 mov qword ptr [r12 + 1024], 6
 mov rax, qword ptr [rip + .Lx123_0]
 mov qword ptr [r12 + 1032], rax
 jmp xgvarg122_done
 xgvarg122_β:
 jmp snoch0_n24_α
.Lx123_0:
 .quad 4
xgvarg122_done:
bb55_α:
# BOX IR_CALL local(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = gvar NV_GET -> [r12+1056]
   lea rdi, [rip + .S0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 1056], rax
 mov qword ptr [r12 + 1064], rdx
# marshal arg1 = producer-box slot [r12+1024] -> [r12+1072]
 mov rax, qword ptr [r12 + 1024]
 mov qword ptr [r12 + 1072], rax
 mov rax, qword ptr [r12 + 1032]
 mov qword ptr [r12 + 1080], rax
  .section .rodata
  .Lbynamefn125: .string "local"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn125]
 lea rsi, [r12 + 1056]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1040], rax
 mov qword ptr [r12 + 1048], rdx
 cmp eax, 99
 je snoch0_n24_α
 jmp snoch0_n23_α
snoch0_n21_β:
 jmp snoch0_n24_α
snoch0_n22_α:
bb56_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S8]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n22_β:
 jmp flat_γ
snoch0_n23_α:
# IR_LIT_scalar
bb57_α:
 jmp snoch0_n25_α
 snoch0_n23_β:
 jmp flat_γ
snoch0_n24_α:
# IR_LIT_scalar
bb58_α:
 jmp snoch0_n26_α
 snoch0_n24_β:
 jmp flat_γ
snoch0_n25_α:
bb59_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S9]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n25_β:
 jmp flat_γ
snoch0_n26_α:
bb60_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S10]
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
.S0: .string "jlab"
.S1: .string "output"
.S2: .string "FAIL 1017/001: arg(.jlab,1) = A"
.S3: .string "FAIL 1017/002: arg(.jlab,3) = C"
.S4: .string "FAIL 1017/003: arg(.jlab,0) OOB should fail"
.S5: .string "FAIL 1017/004: arg(.jlab,4) OOB should fail"
.S6: .string "FAIL 1017/005: local(.jlab,1) = D"
.S7: .string "FAIL 1017/006: local(.jlab,3) = F"
.S8: .string "FAIL 1017/007: local(.jlab,0) OOB should fail"
.S9: .string "FAIL 1017/008: local(.jlab,4) OOB should fail"
.S10: .string "PASS 1017_arg_local (8/8)"
.text

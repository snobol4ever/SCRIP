  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "output"
  .align 8
__gva_names:
  .quad .Lgvan0
  .section .bss
  .align 16
__gva: .space 16, 0
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
  mov edx, 1
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
# IR_LIT_S
bb3_α:
 mov qword ptr [r12 + 64], 1
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [r12 + 72], rax
 jmp xgvarg6_done
 xgvarg6_β:
 jmp snoch0_n3_α
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string "jlab"
xgvarg6_done:
# IR_LIT_I
bb4_α:
 mov qword ptr [r12 + 80], 6
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 88], rax
 jmp xgvarg8_done
 xgvarg8_β:
 jmp snoch0_n3_α
.Lx9_0:
 .quad 1
xgvarg8_done:
bb5_α:
# BOX IR_CALL arg(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
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
  .Lbynamefn11: .string "arg"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn11]
 lea rsi, [r12 + 112]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 cmp eax, 99
 je snoch0_n3_α
 jmp xgvarg5_done
xgvarg5_β:
 jmp snoch0_n3_α
xgvarg5_done:
# IR_LIT_S
bb6_α:
 mov qword ptr [r12 + 144], 1
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [r12 + 152], rax
 jmp xgvarg12_done
 xgvarg12_β:
 jmp snoch0_n3_α
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "A"
xgvarg12_done:
bb7_α:
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
  .Lbynamefn15: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn15]
 lea rsi, [r12 + 176]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 cmp eax, 99
 je snoch0_n3_α
 jmp snoch0_n2_α
snoch0_n1_β:
 jmp snoch0_n3_α
snoch0_n2_α:
# IR_LIT_scalar
bb8_α:
 jmp snoch0_n4_α
 snoch0_n2_β:
 jmp flat_γ
snoch0_n3_α:
# IR_LIT_S
bb9_α:
 mov qword ptr [r12 + 208], 1
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [r12 + 216], rax
 jmp xgvarg18_done
 xgvarg18_β:
 jmp snoch0_n6_α
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "jlab"
xgvarg18_done:
# IR_LIT_I
bb10_α:
 mov qword ptr [r12 + 224], 6
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [r12 + 232], rax
 jmp xgvarg20_done
 xgvarg20_β:
 jmp snoch0_n6_α
.Lx21_0:
 .quad 3
xgvarg20_done:
bb11_α:
# BOX IR_CALL arg(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+208] -> [r12+256]
 mov rax, qword ptr [r12 + 208]
 mov qword ptr [r12 + 256], rax
 mov rax, qword ptr [r12 + 216]
 mov qword ptr [r12 + 264], rax
# marshal arg1 = producer-box slot [r12+224] -> [r12+272]
 mov rax, qword ptr [r12 + 224]
 mov qword ptr [r12 + 272], rax
 mov rax, qword ptr [r12 + 232]
 mov qword ptr [r12 + 280], rax
  .section .rodata
  .Lbynamefn23: .string "arg"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn23]
 lea rsi, [r12 + 256]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 cmp eax, 99
 je snoch0_n6_α
 jmp xgvarg17_done
xgvarg17_β:
 jmp snoch0_n6_α
xgvarg17_done:
# IR_LIT_S
bb12_α:
 mov qword ptr [r12 + 288], 1
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [r12 + 296], rax
 jmp xgvarg24_done
 xgvarg24_β:
 jmp snoch0_n6_α
.Lx25_0:
 .quad .Lx25_0_s
.Lx25_0_s:
 .string "C"
xgvarg24_done:
bb13_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+240] -> [r12+320]
 mov rax, qword ptr [r12 + 240]
 mov qword ptr [r12 + 320], rax
 mov rax, qword ptr [r12 + 248]
 mov qword ptr [r12 + 328], rax
# marshal arg1 = producer-box slot [r12+288] -> [r12+336]
 mov rax, qword ptr [r12 + 288]
 mov qword ptr [r12 + 336], rax
 mov rax, qword ptr [r12 + 296]
 mov qword ptr [r12 + 344], rax
  .section .rodata
  .Lbynamefn27: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn27]
 lea rsi, [r12 + 320]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 304], rax
 mov qword ptr [r12 + 312], rdx
 cmp eax, 99
 je snoch0_n6_α
 jmp snoch0_n5_α
snoch0_n3_β:
 jmp snoch0_n6_α
snoch0_n4_α:
bb14_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S1]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n4_β:
 jmp flat_γ
snoch0_n5_α:
# IR_LIT_scalar
bb15_α:
 jmp snoch0_n7_α
 snoch0_n5_β:
 jmp flat_γ
snoch0_n6_α:
# IR_LIT_S
bb16_α:
 mov qword ptr [r12 + 352], 1
 mov rax, qword ptr [rip + .Lx31_0]
 mov qword ptr [r12 + 360], rax
 jmp xgvarg30_done
 xgvarg30_β:
 jmp snoch0_n9_α
.Lx31_0:
 .quad .Lx31_0_s
.Lx31_0_s:
 .string "jlab"
xgvarg30_done:
# IR_LIT_I
bb17_α:
 mov qword ptr [r12 + 368], 6
 mov rax, qword ptr [rip + .Lx33_0]
 mov qword ptr [r12 + 376], rax
 jmp xgvarg32_done
 xgvarg32_β:
 jmp snoch0_n9_α
.Lx33_0:
 .quad 0
xgvarg32_done:
bb18_α:
# BOX IR_CALL arg(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
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
  .Lbynamefn35: .string "arg"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn35]
 lea rsi, [r12 + 400]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 cmp eax, 99
 je snoch0_n9_α
 jmp snoch0_n8_α
snoch0_n6_β:
 jmp snoch0_n9_α
snoch0_n7_α:
bb19_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S2]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n7_β:
 jmp flat_γ
snoch0_n8_α:
# IR_LIT_scalar
bb20_α:
 jmp snoch0_n10_α
 snoch0_n8_β:
 jmp flat_γ
snoch0_n9_α:
# IR_LIT_S
bb21_α:
 mov qword ptr [r12 + 432], 1
 mov rax, qword ptr [rip + .Lx39_0]
 mov qword ptr [r12 + 440], rax
 jmp xgvarg38_done
 xgvarg38_β:
 jmp snoch0_n12_α
.Lx39_0:
 .quad .Lx39_0_s
.Lx39_0_s:
 .string "jlab"
xgvarg38_done:
# IR_LIT_I
bb22_α:
 mov qword ptr [r12 + 448], 6
 mov rax, qword ptr [rip + .Lx41_0]
 mov qword ptr [r12 + 456], rax
 jmp xgvarg40_done
 xgvarg40_β:
 jmp snoch0_n12_α
.Lx41_0:
 .quad 4
xgvarg40_done:
bb23_α:
# BOX IR_CALL arg(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
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
  .Lbynamefn43: .string "arg"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn43]
 lea rsi, [r12 + 480]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 464], rax
 mov qword ptr [r12 + 472], rdx
 cmp eax, 99
 je snoch0_n12_α
 jmp snoch0_n11_α
snoch0_n9_β:
 jmp snoch0_n12_α
snoch0_n10_α:
bb24_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S3]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n10_β:
 jmp flat_γ
snoch0_n11_α:
# IR_LIT_scalar
bb25_α:
 jmp snoch0_n13_α
 snoch0_n11_β:
 jmp flat_γ
snoch0_n12_α:
# IR_LIT_S
bb26_α:
 mov qword ptr [r12 + 512], 1
 mov rax, qword ptr [rip + .Lx48_0]
 mov qword ptr [r12 + 520], rax
 jmp xgvarg47_done
 xgvarg47_β:
 jmp snoch0_n15_α
.Lx48_0:
 .quad .Lx48_0_s
.Lx48_0_s:
 .string "jlab"
xgvarg47_done:
# IR_LIT_I
bb27_α:
 mov qword ptr [r12 + 528], 6
 mov rax, qword ptr [rip + .Lx50_0]
 mov qword ptr [r12 + 536], rax
 jmp xgvarg49_done
 xgvarg49_β:
 jmp snoch0_n15_α
.Lx50_0:
 .quad 1
xgvarg49_done:
bb28_α:
# BOX IR_CALL local(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
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
  .Lbynamefn52: .string "local"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn52]
 lea rsi, [r12 + 560]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 544], rax
 mov qword ptr [r12 + 552], rdx
 cmp eax, 99
 je snoch0_n15_α
 jmp xgvarg46_done
xgvarg46_β:
 jmp snoch0_n15_α
xgvarg46_done:
# IR_LIT_S
bb29_α:
 mov qword ptr [r12 + 592], 1
 mov rax, qword ptr [rip + .Lx54_0]
 mov qword ptr [r12 + 600], rax
 jmp xgvarg53_done
 xgvarg53_β:
 jmp snoch0_n15_α
.Lx54_0:
 .quad .Lx54_0_s
.Lx54_0_s:
 .string "D"
xgvarg53_done:
bb30_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+544] -> [r12+624]
 mov rax, qword ptr [r12 + 544]
 mov qword ptr [r12 + 624], rax
 mov rax, qword ptr [r12 + 552]
 mov qword ptr [r12 + 632], rax
# marshal arg1 = producer-box slot [r12+592] -> [r12+640]
 mov rax, qword ptr [r12 + 592]
 mov qword ptr [r12 + 640], rax
 mov rax, qword ptr [r12 + 600]
 mov qword ptr [r12 + 648], rax
  .section .rodata
  .Lbynamefn56: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn56]
 lea rsi, [r12 + 624]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 608], rax
 mov qword ptr [r12 + 616], rdx
 cmp eax, 99
 je snoch0_n15_α
 jmp snoch0_n14_α
snoch0_n12_β:
 jmp snoch0_n15_α
snoch0_n13_α:
bb31_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S4]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n13_β:
 jmp flat_γ
snoch0_n14_α:
# IR_LIT_scalar
bb32_α:
 jmp snoch0_n16_α
 snoch0_n14_β:
 jmp flat_γ
snoch0_n15_α:
# IR_LIT_S
bb33_α:
 mov qword ptr [r12 + 656], 1
 mov rax, qword ptr [rip + .Lx61_0]
 mov qword ptr [r12 + 664], rax
 jmp xgvarg60_done
 xgvarg60_β:
 jmp snoch0_n18_α
.Lx61_0:
 .quad .Lx61_0_s
.Lx61_0_s:
 .string "jlab"
xgvarg60_done:
# IR_LIT_I
bb34_α:
 mov qword ptr [r12 + 672], 6
 mov rax, qword ptr [rip + .Lx63_0]
 mov qword ptr [r12 + 680], rax
 jmp xgvarg62_done
 xgvarg62_β:
 jmp snoch0_n18_α
.Lx63_0:
 .quad 3
xgvarg62_done:
bb35_α:
# BOX IR_CALL local(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+656] -> [r12+704]
 mov rax, qword ptr [r12 + 656]
 mov qword ptr [r12 + 704], rax
 mov rax, qword ptr [r12 + 664]
 mov qword ptr [r12 + 712], rax
# marshal arg1 = producer-box slot [r12+672] -> [r12+720]
 mov rax, qword ptr [r12 + 672]
 mov qword ptr [r12 + 720], rax
 mov rax, qword ptr [r12 + 680]
 mov qword ptr [r12 + 728], rax
  .section .rodata
  .Lbynamefn65: .string "local"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn65]
 lea rsi, [r12 + 704]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 688], rax
 mov qword ptr [r12 + 696], rdx
 cmp eax, 99
 je snoch0_n18_α
 jmp xgvarg59_done
xgvarg59_β:
 jmp snoch0_n18_α
xgvarg59_done:
# IR_LIT_S
bb36_α:
 mov qword ptr [r12 + 736], 1
 mov rax, qword ptr [rip + .Lx67_0]
 mov qword ptr [r12 + 744], rax
 jmp xgvarg66_done
 xgvarg66_β:
 jmp snoch0_n18_α
.Lx67_0:
 .quad .Lx67_0_s
.Lx67_0_s:
 .string "F"
xgvarg66_done:
bb37_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+688] -> [r12+768]
 mov rax, qword ptr [r12 + 688]
 mov qword ptr [r12 + 768], rax
 mov rax, qword ptr [r12 + 696]
 mov qword ptr [r12 + 776], rax
# marshal arg1 = producer-box slot [r12+736] -> [r12+784]
 mov rax, qword ptr [r12 + 736]
 mov qword ptr [r12 + 784], rax
 mov rax, qword ptr [r12 + 744]
 mov qword ptr [r12 + 792], rax
  .section .rodata
  .Lbynamefn69: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn69]
 lea rsi, [r12 + 768]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 752], rax
 mov qword ptr [r12 + 760], rdx
 cmp eax, 99
 je snoch0_n18_α
 jmp snoch0_n17_α
snoch0_n15_β:
 jmp snoch0_n18_α
snoch0_n16_α:
bb38_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S5]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n16_β:
 jmp flat_γ
snoch0_n17_α:
# IR_LIT_scalar
bb39_α:
 jmp snoch0_n19_α
 snoch0_n17_β:
 jmp flat_γ
snoch0_n18_α:
# IR_LIT_S
bb40_α:
 mov qword ptr [r12 + 800], 1
 mov rax, qword ptr [rip + .Lx73_0]
 mov qword ptr [r12 + 808], rax
 jmp xgvarg72_done
 xgvarg72_β:
 jmp snoch0_n21_α
.Lx73_0:
 .quad .Lx73_0_s
.Lx73_0_s:
 .string "jlab"
xgvarg72_done:
# IR_LIT_I
bb41_α:
 mov qword ptr [r12 + 816], 6
 mov rax, qword ptr [rip + .Lx75_0]
 mov qword ptr [r12 + 824], rax
 jmp xgvarg74_done
 xgvarg74_β:
 jmp snoch0_n21_α
.Lx75_0:
 .quad 0
xgvarg74_done:
bb42_α:
# BOX IR_CALL local(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
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
  .Lbynamefn77: .string "local"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn77]
 lea rsi, [r12 + 848]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 832], rax
 mov qword ptr [r12 + 840], rdx
 cmp eax, 99
 je snoch0_n21_α
 jmp snoch0_n20_α
snoch0_n18_β:
 jmp snoch0_n21_α
snoch0_n19_α:
bb43_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S6]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n19_β:
 jmp flat_γ
snoch0_n20_α:
# IR_LIT_scalar
bb44_α:
 jmp snoch0_n22_α
 snoch0_n20_β:
 jmp flat_γ
snoch0_n21_α:
# IR_LIT_S
bb45_α:
 mov qword ptr [r12 + 880], 1
 mov rax, qword ptr [rip + .Lx81_0]
 mov qword ptr [r12 + 888], rax
 jmp xgvarg80_done
 xgvarg80_β:
 jmp snoch0_n24_α
.Lx81_0:
 .quad .Lx81_0_s
.Lx81_0_s:
 .string "jlab"
xgvarg80_done:
# IR_LIT_I
bb46_α:
 mov qword ptr [r12 + 896], 6
 mov rax, qword ptr [rip + .Lx83_0]
 mov qword ptr [r12 + 904], rax
 jmp xgvarg82_done
 xgvarg82_β:
 jmp snoch0_n24_α
.Lx83_0:
 .quad 4
xgvarg82_done:
bb47_α:
# BOX IR_CALL local(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+880] -> [r12+928]
 mov rax, qword ptr [r12 + 880]
 mov qword ptr [r12 + 928], rax
 mov rax, qword ptr [r12 + 888]
 mov qword ptr [r12 + 936], rax
# marshal arg1 = producer-box slot [r12+896] -> [r12+944]
 mov rax, qword ptr [r12 + 896]
 mov qword ptr [r12 + 944], rax
 mov rax, qword ptr [r12 + 904]
 mov qword ptr [r12 + 952], rax
  .section .rodata
  .Lbynamefn85: .string "local"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn85]
 lea rsi, [r12 + 928]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 912], rax
 mov qword ptr [r12 + 920], rdx
 cmp eax, 99
 je snoch0_n24_α
 jmp snoch0_n23_α
snoch0_n21_β:
 jmp snoch0_n24_α
snoch0_n22_α:
bb48_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S7]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n22_β:
 jmp flat_γ
snoch0_n23_α:
# IR_LIT_scalar
bb49_α:
 jmp snoch0_n25_α
 snoch0_n23_β:
 jmp flat_γ
snoch0_n24_α:
# IR_LIT_scalar
bb50_α:
 jmp snoch0_n26_α
 snoch0_n24_β:
 jmp flat_γ
snoch0_n25_α:
bb51_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
 lea rsi, [rip + .S8]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n25_β:
 jmp flat_γ
snoch0_n26_α:
bb52_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S0]
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
.S0: .string "output"
.S1: .string "FAIL 1017/001: arg(.jlab,1) = A"
.S2: .string "FAIL 1017/002: arg(.jlab,3) = C"
.S3: .string "FAIL 1017/003: arg(.jlab,0) OOB should fail"
.S4: .string "FAIL 1017/004: arg(.jlab,4) OOB should fail"
.S5: .string "FAIL 1017/005: local(.jlab,1) = D"
.S6: .string "FAIL 1017/006: local(.jlab,3) = F"
.S7: .string "FAIL 1017/007: local(.jlab,0) OOB should fail"
.S8: .string "FAIL 1017/008: local(.jlab,4) OOB should fail"
.S9: .string "PASS 1017_arg_local (8/8)"
.text

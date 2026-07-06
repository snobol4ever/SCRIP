  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "jlab"
  .Lgvan1: .string "output"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .section .bss
  .align 16
__gva: .space 32, 0
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  push rbp
  mov rbp, rsp
  push rdi
  push rsi
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 2
  call gva_register@PLT
  mov rbx, rax
  call rt_frame@PLT
  mov rdi, rax
  xor esi, esi
  call main_α
  xor eax, eax
  mov rsp, rbp
  pop rbp
  ret
main_α:
#=======================================================================================================================
    .global main_α
    .global main_β
    .global main_γ
    .global main_ω
push r12
  mov r12, rdi
main_α_body:
xchain0_n0_α:
# IR_LIT_STRING
bb1_α:
 mov qword ptr [r12 + 48], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 56], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n2_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "jlab(a,b,c)d,e,f"
xchain0_n1_α:
bb2_α:
# BOX IR_CALL define(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+48] -> [r12+32]
 mov rax, qword ptr [r12 + 48]
 mov qword ptr [r12 + 32], rax
 mov rax, qword ptr [r12 + 56]
 mov qword ptr [r12 + 40], rax
  .section .rodata
  .Lbynamefn3: .string "define"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn3]
 lea rsi, [r12 + 32]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 cmp eax, 99
 je xchain0_n2_α
 jmp xchain0_n2_α
xchain0_n1_β:
 jmp xchain0_n2_α
xchain0_n2_α:
# IR_LIT_STRING
bb3_α:
 mov qword ptr [r12 + 192], 1
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [r12 + 200], rax
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n5_α
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "jlab"
xchain0_n3_α:
bb4_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+192] -> [r12+176]
 mov rax, qword ptr [r12 + 192]
 mov qword ptr [r12 + 176], rax
 mov rax, qword ptr [r12 + 200]
 mov qword ptr [r12 + 184], rax
  .section .rodata
  .Lrkfn6: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn6]
 lea rsi, [r12 + 176]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 cmp eax, 99
 je xchain0_n5_α
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n5_α
xchain0_n4_α:
# IR_LIT_INTEGER
bb5_α:
 mov qword ptr [r12 + 208], 6
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [r12 + 216], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n5_α
.Lx7_0:
 .quad 1
xchain0_n5_α:
# IR_LIT_STRING
bb6_α:
 mov qword ptr [r12 + 400], 1
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [r12 + 408], rax
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n10_α
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "jlab"
xchain0_n6_α:
bb7_α:
# BOX IR_CALL arg(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+160] -> [r12+128]
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 128], rax
 mov rax, qword ptr [r12 + 168]
 mov qword ptr [r12 + 136], rax
# marshal arg1 = producer-box slot [r12+208] -> [r12+144]
 mov rax, qword ptr [r12 + 208]
 mov qword ptr [r12 + 144], rax
 mov rax, qword ptr [r12 + 216]
 mov qword ptr [r12 + 152], rax
  .section .rodata
  .Lbynamefn10: .string "arg"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn10]
 lea rsi, [r12 + 128]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 cmp eax, 99
 je xchain0_n5_α
 jmp xchain0_n8_α
xchain0_n6_β:
 jmp xchain0_n5_α
xchain0_n7_α:
bb8_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+400] -> [r12+384]
 mov rax, qword ptr [r12 + 400]
 mov qword ptr [r12 + 384], rax
 mov rax, qword ptr [r12 + 408]
 mov qword ptr [r12 + 392], rax
  .section .rodata
  .Lrkfn12: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn12]
 lea rsi, [r12 + 384]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 368], rax
 mov qword ptr [r12 + 376], rdx
 cmp eax, 99
 je xchain0_n10_α
 jmp xchain0_n9_α
 xchain0_n7_β:
 jmp xchain0_n10_α
xchain0_n8_α:
# IR_LIT_STRING
bb9_α:
 mov qword ptr [r12 + 224], 1
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [r12 + 232], rax
 jmp xchain0_n11_α
 xchain0_n8_β:
 jmp xchain0_n5_α
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "A"
xchain0_n9_α:
# IR_LIT_INTEGER
bb10_α:
 mov qword ptr [r12 + 416], 6
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [r12 + 424], rax
 jmp xchain0_n12_α
 xchain0_n9_β:
 jmp xchain0_n10_α
.Lx14_0:
 .quad 3
xchain0_n10_α:
# IR_LIT_STRING
bb11_α:
 mov qword ptr [r12 + 560], 1
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [r12 + 568], rax
 jmp xchain0_n13_α
 xchain0_n10_β:
 jmp xchain0_n17_α
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "jlab"
xchain0_n11_α:
bb12_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+112] -> [r12+80]
 mov rax, qword ptr [r12 + 112]
 mov qword ptr [r12 + 80], rax
 mov rax, qword ptr [r12 + 120]
 mov qword ptr [r12 + 88], rax
# marshal arg1 = producer-box slot [r12+224] -> [r12+96]
 mov rax, qword ptr [r12 + 224]
 mov qword ptr [r12 + 96], rax
 mov rax, qword ptr [r12 + 232]
 mov qword ptr [r12 + 104], rax
  .section .rodata
  .Lbynamefn17: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn17]
 lea rsi, [r12 + 80]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
 cmp eax, 99
 je xchain0_n5_α
 jmp xchain0_n14_α
xchain0_n11_β:
 jmp xchain0_n5_α
xchain0_n12_α:
bb13_α:
# BOX IR_CALL arg(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+368] -> [r12+336]
 mov rax, qword ptr [r12 + 368]
 mov qword ptr [r12 + 336], rax
 mov rax, qword ptr [r12 + 376]
 mov qword ptr [r12 + 344], rax
# marshal arg1 = producer-box slot [r12+416] -> [r12+352]
 mov rax, qword ptr [r12 + 416]
 mov qword ptr [r12 + 352], rax
 mov rax, qword ptr [r12 + 424]
 mov qword ptr [r12 + 360], rax
  .section .rodata
  .Lbynamefn19: .string "arg"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn19]
 lea rsi, [r12 + 336]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 cmp eax, 99
 je xchain0_n10_α
 jmp xchain0_n15_α
xchain0_n12_β:
 jmp xchain0_n10_α
xchain0_n13_α:
bb14_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+560] -> [r12+544]
 mov rax, qword ptr [r12 + 560]
 mov qword ptr [r12 + 544], rax
 mov rax, qword ptr [r12 + 568]
 mov qword ptr [r12 + 552], rax
  .section .rodata
  .Lrkfn21: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn21]
 lea rsi, [r12 + 544]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 528], rax
 mov qword ptr [r12 + 536], rdx
 cmp eax, 99
 je xchain0_n17_α
 jmp xchain0_n16_α
 xchain0_n13_β:
 jmp xchain0_n17_α
xchain0_n14_α:
# IR_LIT_STRING
bb15_α:
 mov qword ptr [r12 + 256], 1
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [r12 + 264], rax
 jmp xchain0_n18_α
 xchain0_n14_β:
 jmp main_γ
.Lx22_0:
 .quad .Lx22_0_s
.Lx22_0_s:
 .string "FAIL 1017/001: arg(.jlab,1) = A"
xchain0_n15_α:
# IR_LIT_STRING
bb16_α:
 mov qword ptr [r12 + 432], 1
 mov rax, qword ptr [rip + .Lx23_0]
 mov qword ptr [r12 + 440], rax
 jmp xchain0_n19_α
 xchain0_n15_β:
 jmp xchain0_n10_α
.Lx23_0:
 .quad .Lx23_0_s
.Lx23_0_s:
 .string "C"
xchain0_n16_α:
# IR_LIT_INTEGER
bb17_α:
 mov qword ptr [r12 + 576], 6
 mov rax, qword ptr [rip + .Lx24_0]
 mov qword ptr [r12 + 584], rax
 jmp xchain0_n20_α
 xchain0_n16_β:
 jmp xchain0_n17_α
.Lx24_0:
 .quad 0
xchain0_n17_α:
# IR_LIT_STRING
bb18_α:
 mov qword ptr [r12 + 704], 1
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [r12 + 712], rax
 jmp xchain0_n21_α
 xchain0_n17_β:
 jmp xchain0_n25_α
.Lx25_0:
 .quad .Lx25_0_s
.Lx25_0_s:
 .string "jlab"
xchain0_n18_α:
# IR_ASSIGN gva
bb19_α:
 mov rax, qword ptr [r12 + 256]
 mov rdx, qword ptr [r12 + 264]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 jmp main_γ
 xchain0_n18_β:
 jmp main_γ
xchain0_n19_α:
bb20_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+320] -> [r12+288]
 mov rax, qword ptr [r12 + 320]
 mov qword ptr [r12 + 288], rax
 mov rax, qword ptr [r12 + 328]
 mov qword ptr [r12 + 296], rax
# marshal arg1 = producer-box slot [r12+432] -> [r12+304]
 mov rax, qword ptr [r12 + 432]
 mov qword ptr [r12 + 304], rax
 mov rax, qword ptr [r12 + 440]
 mov qword ptr [r12 + 312], rax
  .section .rodata
  .Lbynamefn28: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn28]
 lea rsi, [r12 + 288]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 cmp eax, 99
 je xchain0_n10_α
 jmp xchain0_n22_α
xchain0_n19_β:
 jmp xchain0_n10_α
xchain0_n20_α:
bb21_α:
# BOX IR_CALL arg(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+528] -> [r12+496]
 mov rax, qword ptr [r12 + 528]
 mov qword ptr [r12 + 496], rax
 mov rax, qword ptr [r12 + 536]
 mov qword ptr [r12 + 504], rax
# marshal arg1 = producer-box slot [r12+576] -> [r12+512]
 mov rax, qword ptr [r12 + 576]
 mov qword ptr [r12 + 512], rax
 mov rax, qword ptr [r12 + 584]
 mov qword ptr [r12 + 520], rax
  .section .rodata
  .Lbynamefn30: .string "arg"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn30]
 lea rsi, [r12 + 496]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 480], rax
 mov qword ptr [r12 + 488], rdx
 cmp eax, 99
 je xchain0_n17_α
 jmp xchain0_n23_α
xchain0_n20_β:
 jmp xchain0_n17_α
xchain0_n21_α:
bb22_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+704] -> [r12+688]
 mov rax, qword ptr [r12 + 704]
 mov qword ptr [r12 + 688], rax
 mov rax, qword ptr [r12 + 712]
 mov qword ptr [r12 + 696], rax
  .section .rodata
  .Lrkfn32: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn32]
 lea rsi, [r12 + 688]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 672], rax
 mov qword ptr [r12 + 680], rdx
 cmp eax, 99
 je xchain0_n25_α
 jmp xchain0_n24_α
 xchain0_n21_β:
 jmp xchain0_n25_α
xchain0_n22_α:
# IR_LIT_STRING
bb23_α:
 mov qword ptr [r12 + 464], 1
 mov rax, qword ptr [rip + .Lx33_0]
 mov qword ptr [r12 + 472], rax
 jmp xchain0_n26_α
 xchain0_n22_β:
 jmp main_γ
.Lx33_0:
 .quad .Lx33_0_s
.Lx33_0_s:
 .string "FAIL 1017/002: arg(.jlab,3) = C"
xchain0_n23_α:
# IR_LIT_STRING
bb24_α:
 mov qword ptr [r12 + 608], 1
 mov rax, qword ptr [rip + .Lx34_0]
 mov qword ptr [r12 + 616], rax
 jmp xchain0_n27_α
 xchain0_n23_β:
 jmp main_γ
.Lx34_0:
 .quad .Lx34_0_s
.Lx34_0_s:
 .string "FAIL 1017/003: arg(.jlab,0) OOB should fail"
xchain0_n24_α:
# IR_LIT_INTEGER
bb25_α:
 mov qword ptr [r12 + 720], 6
 mov rax, qword ptr [rip + .Lx35_0]
 mov qword ptr [r12 + 728], rax
 jmp xchain0_n28_α
 xchain0_n24_β:
 jmp xchain0_n25_α
.Lx35_0:
 .quad 4
xchain0_n25_α:
# IR_LIT_STRING
bb26_α:
 mov qword ptr [r12 + 896], 1
 mov rax, qword ptr [rip + .Lx36_0]
 mov qword ptr [r12 + 904], rax
 jmp xchain0_n29_α
 xchain0_n25_β:
 jmp xchain0_n32_α
.Lx36_0:
 .quad .Lx36_0_s
.Lx36_0_s:
 .string "jlab"
xchain0_n26_α:
# IR_ASSIGN gva
bb27_α:
 mov rax, qword ptr [r12 + 464]
 mov rdx, qword ptr [r12 + 472]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 448], rax
 mov qword ptr [r12 + 456], rdx
 jmp main_γ
 xchain0_n26_β:
 jmp main_γ
xchain0_n27_α:
# IR_ASSIGN gva
bb28_α:
 mov rax, qword ptr [r12 + 608]
 mov rdx, qword ptr [r12 + 616]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 592], rax
 mov qword ptr [r12 + 600], rdx
 jmp main_γ
 xchain0_n27_β:
 jmp main_γ
xchain0_n28_α:
bb29_α:
# BOX IR_CALL arg(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+672] -> [r12+640]
 mov rax, qword ptr [r12 + 672]
 mov qword ptr [r12 + 640], rax
 mov rax, qword ptr [r12 + 680]
 mov qword ptr [r12 + 648], rax
# marshal arg1 = producer-box slot [r12+720] -> [r12+656]
 mov rax, qword ptr [r12 + 720]
 mov qword ptr [r12 + 656], rax
 mov rax, qword ptr [r12 + 728]
 mov qword ptr [r12 + 664], rax
  .section .rodata
  .Lbynamefn40: .string "arg"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn40]
 lea rsi, [r12 + 640]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 624], rax
 mov qword ptr [r12 + 632], rdx
 cmp eax, 99
 je xchain0_n25_α
 jmp xchain0_n30_α
xchain0_n28_β:
 jmp xchain0_n25_α
xchain0_n29_α:
bb30_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+896] -> [r12+880]
 mov rax, qword ptr [r12 + 896]
 mov qword ptr [r12 + 880], rax
 mov rax, qword ptr [r12 + 904]
 mov qword ptr [r12 + 888], rax
  .section .rodata
  .Lrkfn42: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn42]
 lea rsi, [r12 + 880]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 864], rax
 mov qword ptr [r12 + 872], rdx
 cmp eax, 99
 je xchain0_n32_α
 jmp xchain0_n31_α
 xchain0_n29_β:
 jmp xchain0_n32_α
xchain0_n30_α:
# IR_LIT_STRING
bb31_α:
 mov qword ptr [r12 + 752], 1
 mov rax, qword ptr [rip + .Lx43_0]
 mov qword ptr [r12 + 760], rax
 jmp xchain0_n33_α
 xchain0_n30_β:
 jmp main_γ
.Lx43_0:
 .quad .Lx43_0_s
.Lx43_0_s:
 .string "FAIL 1017/004: arg(.jlab,4) OOB should fail"
xchain0_n31_α:
# IR_LIT_INTEGER
bb32_α:
 mov qword ptr [r12 + 912], 6
 mov rax, qword ptr [rip + .Lx44_0]
 mov qword ptr [r12 + 920], rax
 jmp xchain0_n34_α
 xchain0_n31_β:
 jmp xchain0_n32_α
.Lx44_0:
 .quad 1
xchain0_n32_α:
# IR_LIT_STRING
bb33_α:
 mov qword ptr [r12 + 1104], 1
 mov rax, qword ptr [rip + .Lx45_0]
 mov qword ptr [r12 + 1112], rax
 jmp xchain0_n35_α
 xchain0_n32_β:
 jmp xchain0_n38_α
.Lx45_0:
 .quad .Lx45_0_s
.Lx45_0_s:
 .string "jlab"
xchain0_n33_α:
# IR_ASSIGN gva
bb34_α:
 mov rax, qword ptr [r12 + 752]
 mov rdx, qword ptr [r12 + 760]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 736], rax
 mov qword ptr [r12 + 744], rdx
 jmp main_γ
 xchain0_n33_β:
 jmp main_γ
xchain0_n34_α:
bb35_α:
# BOX IR_CALL local(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+864] -> [r12+832]
 mov rax, qword ptr [r12 + 864]
 mov qword ptr [r12 + 832], rax
 mov rax, qword ptr [r12 + 872]
 mov qword ptr [r12 + 840], rax
# marshal arg1 = producer-box slot [r12+912] -> [r12+848]
 mov rax, qword ptr [r12 + 912]
 mov qword ptr [r12 + 848], rax
 mov rax, qword ptr [r12 + 920]
 mov qword ptr [r12 + 856], rax
  .section .rodata
  .Lbynamefn48: .string "local"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn48]
 lea rsi, [r12 + 832]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 816], rax
 mov qword ptr [r12 + 824], rdx
 cmp eax, 99
 je xchain0_n32_α
 jmp xchain0_n36_α
xchain0_n34_β:
 jmp xchain0_n32_α
xchain0_n35_α:
bb36_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+1104] -> [r12+1088]
 mov rax, qword ptr [r12 + 1104]
 mov qword ptr [r12 + 1088], rax
 mov rax, qword ptr [r12 + 1112]
 mov qword ptr [r12 + 1096], rax
  .section .rodata
  .Lrkfn50: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn50]
 lea rsi, [r12 + 1088]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1072], rax
 mov qword ptr [r12 + 1080], rdx
 cmp eax, 99
 je xchain0_n38_α
 jmp xchain0_n37_α
 xchain0_n35_β:
 jmp xchain0_n38_α
xchain0_n36_α:
# IR_LIT_STRING
bb37_α:
 mov qword ptr [r12 + 928], 1
 mov rax, qword ptr [rip + .Lx51_0]
 mov qword ptr [r12 + 936], rax
 jmp xchain0_n39_α
 xchain0_n36_β:
 jmp xchain0_n32_α
.Lx51_0:
 .quad .Lx51_0_s
.Lx51_0_s:
 .string "D"
xchain0_n37_α:
# IR_LIT_INTEGER
bb38_α:
 mov qword ptr [r12 + 1120], 6
 mov rax, qword ptr [rip + .Lx52_0]
 mov qword ptr [r12 + 1128], rax
 jmp xchain0_n40_α
 xchain0_n37_β:
 jmp xchain0_n38_α
.Lx52_0:
 .quad 3
xchain0_n38_α:
# IR_LIT_STRING
bb39_α:
 mov qword ptr [r12 + 1264], 1
 mov rax, qword ptr [rip + .Lx53_0]
 mov qword ptr [r12 + 1272], rax
 jmp xchain0_n41_α
 xchain0_n38_β:
 jmp xchain0_n45_α
.Lx53_0:
 .quad .Lx53_0_s
.Lx53_0_s:
 .string "jlab"
xchain0_n39_α:
bb40_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+816] -> [r12+784]
 mov rax, qword ptr [r12 + 816]
 mov qword ptr [r12 + 784], rax
 mov rax, qword ptr [r12 + 824]
 mov qword ptr [r12 + 792], rax
# marshal arg1 = producer-box slot [r12+928] -> [r12+800]
 mov rax, qword ptr [r12 + 928]
 mov qword ptr [r12 + 800], rax
 mov rax, qword ptr [r12 + 936]
 mov qword ptr [r12 + 808], rax
  .section .rodata
  .Lbynamefn55: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn55]
 lea rsi, [r12 + 784]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 768], rax
 mov qword ptr [r12 + 776], rdx
 cmp eax, 99
 je xchain0_n32_α
 jmp xchain0_n42_α
xchain0_n39_β:
 jmp xchain0_n32_α
xchain0_n40_α:
bb41_α:
# BOX IR_CALL local(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+1072] -> [r12+1040]
 mov rax, qword ptr [r12 + 1072]
 mov qword ptr [r12 + 1040], rax
 mov rax, qword ptr [r12 + 1080]
 mov qword ptr [r12 + 1048], rax
# marshal arg1 = producer-box slot [r12+1120] -> [r12+1056]
 mov rax, qword ptr [r12 + 1120]
 mov qword ptr [r12 + 1056], rax
 mov rax, qword ptr [r12 + 1128]
 mov qword ptr [r12 + 1064], rax
  .section .rodata
  .Lbynamefn57: .string "local"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn57]
 lea rsi, [r12 + 1040]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1024], rax
 mov qword ptr [r12 + 1032], rdx
 cmp eax, 99
 je xchain0_n38_α
 jmp xchain0_n43_α
xchain0_n40_β:
 jmp xchain0_n38_α
xchain0_n41_α:
bb42_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+1264] -> [r12+1248]
 mov rax, qword ptr [r12 + 1264]
 mov qword ptr [r12 + 1248], rax
 mov rax, qword ptr [r12 + 1272]
 mov qword ptr [r12 + 1256], rax
  .section .rodata
  .Lrkfn59: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn59]
 lea rsi, [r12 + 1248]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1232], rax
 mov qword ptr [r12 + 1240], rdx
 cmp eax, 99
 je xchain0_n45_α
 jmp xchain0_n44_α
 xchain0_n41_β:
 jmp xchain0_n45_α
xchain0_n42_α:
# IR_LIT_STRING
bb43_α:
 mov qword ptr [r12 + 960], 1
 mov rax, qword ptr [rip + .Lx60_0]
 mov qword ptr [r12 + 968], rax
 jmp xchain0_n46_α
 xchain0_n42_β:
 jmp main_γ
.Lx60_0:
 .quad .Lx60_0_s
.Lx60_0_s:
 .string "FAIL 1017/005: local(.jlab,1) = D"
xchain0_n43_α:
# IR_LIT_STRING
bb44_α:
 mov qword ptr [r12 + 1136], 1
 mov rax, qword ptr [rip + .Lx61_0]
 mov qword ptr [r12 + 1144], rax
 jmp xchain0_n47_α
 xchain0_n43_β:
 jmp xchain0_n38_α
.Lx61_0:
 .quad .Lx61_0_s
.Lx61_0_s:
 .string "F"
xchain0_n44_α:
# IR_LIT_INTEGER
bb45_α:
 mov qword ptr [r12 + 1280], 6
 mov rax, qword ptr [rip + .Lx62_0]
 mov qword ptr [r12 + 1288], rax
 jmp xchain0_n48_α
 xchain0_n44_β:
 jmp xchain0_n45_α
.Lx62_0:
 .quad 0
xchain0_n45_α:
# IR_LIT_STRING
bb46_α:
 mov qword ptr [r12 + 1408], 1
 mov rax, qword ptr [rip + .Lx63_0]
 mov qword ptr [r12 + 1416], rax
 jmp xchain0_n49_α
 xchain0_n45_β:
 jmp xchain0_n53_α
.Lx63_0:
 .quad .Lx63_0_s
.Lx63_0_s:
 .string "jlab"
xchain0_n46_α:
# IR_ASSIGN gva
bb47_α:
 mov rax, qword ptr [r12 + 960]
 mov rdx, qword ptr [r12 + 968]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 944], rax
 mov qword ptr [r12 + 952], rdx
 jmp main_γ
 xchain0_n46_β:
 jmp main_γ
xchain0_n47_α:
bb48_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+1024] -> [r12+992]
 mov rax, qword ptr [r12 + 1024]
 mov qword ptr [r12 + 992], rax
 mov rax, qword ptr [r12 + 1032]
 mov qword ptr [r12 + 1000], rax
# marshal arg1 = producer-box slot [r12+1136] -> [r12+1008]
 mov rax, qword ptr [r12 + 1136]
 mov qword ptr [r12 + 1008], rax
 mov rax, qword ptr [r12 + 1144]
 mov qword ptr [r12 + 1016], rax
  .section .rodata
  .Lbynamefn66: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn66]
 lea rsi, [r12 + 992]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 976], rax
 mov qword ptr [r12 + 984], rdx
 cmp eax, 99
 je xchain0_n38_α
 jmp xchain0_n50_α
xchain0_n47_β:
 jmp xchain0_n38_α
xchain0_n48_α:
bb49_α:
# BOX IR_CALL local(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+1232] -> [r12+1200]
 mov rax, qword ptr [r12 + 1232]
 mov qword ptr [r12 + 1200], rax
 mov rax, qword ptr [r12 + 1240]
 mov qword ptr [r12 + 1208], rax
# marshal arg1 = producer-box slot [r12+1280] -> [r12+1216]
 mov rax, qword ptr [r12 + 1280]
 mov qword ptr [r12 + 1216], rax
 mov rax, qword ptr [r12 + 1288]
 mov qword ptr [r12 + 1224], rax
  .section .rodata
  .Lbynamefn68: .string "local"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn68]
 lea rsi, [r12 + 1200]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1184], rax
 mov qword ptr [r12 + 1192], rdx
 cmp eax, 99
 je xchain0_n45_α
 jmp xchain0_n51_α
xchain0_n48_β:
 jmp xchain0_n45_α
xchain0_n49_α:
bb50_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+1408] -> [r12+1392]
 mov rax, qword ptr [r12 + 1408]
 mov qword ptr [r12 + 1392], rax
 mov rax, qword ptr [r12 + 1416]
 mov qword ptr [r12 + 1400], rax
  .section .rodata
  .Lrkfn70: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn70]
 lea rsi, [r12 + 1392]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1376], rax
 mov qword ptr [r12 + 1384], rdx
 cmp eax, 99
 je xchain0_n53_α
 jmp xchain0_n52_α
 xchain0_n49_β:
 jmp xchain0_n53_α
xchain0_n50_α:
# IR_LIT_STRING
bb51_α:
 mov qword ptr [r12 + 1168], 1
 mov rax, qword ptr [rip + .Lx71_0]
 mov qword ptr [r12 + 1176], rax
 jmp xchain0_n54_α
 xchain0_n50_β:
 jmp main_γ
.Lx71_0:
 .quad .Lx71_0_s
.Lx71_0_s:
 .string "FAIL 1017/006: local(.jlab,3) = F"
xchain0_n51_α:
# IR_LIT_STRING
bb52_α:
 mov qword ptr [r12 + 1312], 1
 mov rax, qword ptr [rip + .Lx72_0]
 mov qword ptr [r12 + 1320], rax
 jmp xchain0_n55_α
 xchain0_n51_β:
 jmp main_γ
.Lx72_0:
 .quad .Lx72_0_s
.Lx72_0_s:
 .string "FAIL 1017/007: local(.jlab,0) OOB should fail"
xchain0_n52_α:
# IR_LIT_INTEGER
bb53_α:
 mov qword ptr [r12 + 1424], 6
 mov rax, qword ptr [rip + .Lx73_0]
 mov qword ptr [r12 + 1432], rax
 jmp xchain0_n56_α
 xchain0_n52_β:
 jmp xchain0_n53_α
.Lx73_0:
 .quad 4
xchain0_n53_α:
# IR_LIT_STRING
bb54_α:
 mov qword ptr [r12 + 1488], 1
 mov rax, qword ptr [rip + .Lx74_0]
 mov qword ptr [r12 + 1496], rax
 jmp xchain0_n57_α
 xchain0_n53_β:
 jmp main_γ
.Lx74_0:
 .quad .Lx74_0_s
.Lx74_0_s:
 .string "PASS 1017_arg_local (8/8)"
xchain0_n54_α:
# IR_ASSIGN gva
bb55_α:
 mov rax, qword ptr [r12 + 1168]
 mov rdx, qword ptr [r12 + 1176]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1152], rax
 mov qword ptr [r12 + 1160], rdx
 jmp main_γ
 xchain0_n54_β:
 jmp main_γ
xchain0_n55_α:
# IR_ASSIGN gva
bb56_α:
 mov rax, qword ptr [r12 + 1312]
 mov rdx, qword ptr [r12 + 1320]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1296], rax
 mov qword ptr [r12 + 1304], rdx
 jmp main_γ
 xchain0_n55_β:
 jmp main_γ
xchain0_n56_α:
bb57_α:
# BOX IR_CALL local(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+1376] -> [r12+1344]
 mov rax, qword ptr [r12 + 1376]
 mov qword ptr [r12 + 1344], rax
 mov rax, qword ptr [r12 + 1384]
 mov qword ptr [r12 + 1352], rax
# marshal arg1 = producer-box slot [r12+1424] -> [r12+1360]
 mov rax, qword ptr [r12 + 1424]
 mov qword ptr [r12 + 1360], rax
 mov rax, qword ptr [r12 + 1432]
 mov qword ptr [r12 + 1368], rax
  .section .rodata
  .Lbynamefn78: .string "local"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn78]
 lea rsi, [r12 + 1344]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1328], rax
 mov qword ptr [r12 + 1336], rdx
 cmp eax, 99
 je xchain0_n53_α
 jmp xchain0_n58_α
xchain0_n56_β:
 jmp xchain0_n53_α
xchain0_n57_α:
# IR_ASSIGN gva
bb58_α:
 mov rax, qword ptr [r12 + 1488]
 mov rdx, qword ptr [r12 + 1496]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1472], rax
 mov qword ptr [r12 + 1480], rdx
 jmp main_γ
 xchain0_n57_β:
 jmp main_γ
xchain0_n58_α:
# IR_LIT_STRING
bb59_α:
 mov qword ptr [r12 + 1456], 1
 mov rax, qword ptr [rip + .Lx80_0]
 mov qword ptr [r12 + 1464], rax
 jmp xchain0_n59_α
 xchain0_n58_β:
 jmp main_γ
.Lx80_0:
 .quad .Lx80_0_s
.Lx80_0_s:
 .string "FAIL 1017/008: local(.jlab,4) OOB should fail"
xchain0_n59_α:
# IR_ASSIGN gva
bb60_α:
 mov rax, qword ptr [r12 + 1456]
 mov rdx, qword ptr [r12 + 1464]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1440], rax
 mov qword ptr [r12 + 1448], rdx
 jmp main_γ
 xchain0_n59_β:
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
pop r12
ret
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
pop r12
ret

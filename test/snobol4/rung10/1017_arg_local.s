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
  call core_lib_init@PLT
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
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  mov qword ptr [r12 + 2408], rsp
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 2400], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [r12 + 144], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 152], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n2_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "jlab(a,b,c)d,e,f"
 xchain0_n1_α:
# BOX CALL define(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+144] -> [zr+112]
 mov rax, qword ptr [r12 + 144]
 mov qword ptr [r12 + 112], rax
 mov rax, qword ptr [r12 + 152]
 mov qword ptr [r12 + 120], rax
  .section .rodata
  .Lbynamefn2: .string "define"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn2]
 lea rsi, [r12 + 112]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 cmp eax, 99
 je xchain0_n2_α
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n2_α
# IR_LIT_STRING
 xchain0_n2_α:
 mov qword ptr [r12 + 352], 1
 mov rax, qword ptr [rip + .Lx3_0]
 mov qword ptr [r12 + 360], rax
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n5_α
.Lx3_0:
 .quad .Lx3_0_s
.Lx3_0_s:
 .string "jlab"
 xchain0_n3_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+352] -> [zr+320]
 mov rax, qword ptr [r12 + 352]
 mov qword ptr [r12 + 320], rax
 mov rax, qword ptr [r12 + 360]
 mov qword ptr [r12 + 328], rax
  .section .rodata
  .Lrkfn5: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn5]
 lea rsi, [r12 + 320]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 304], rax
 mov qword ptr [r12 + 312], rdx
 cmp eax, 99
 je xchain0_n5_α
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n5_α
# IR_LIT_INTEGER
 xchain0_n4_α:
 mov qword ptr [r12 + 384], 6
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 392], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n5_α
.Lx6_0:
 .quad 1
# IR_LIT_STRING
 xchain0_n5_α:
 mov qword ptr [r12 + 672], 1
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [r12 + 680], rax
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n10_α
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string "jlab"
 xchain0_n6_α:
# BOX CALL arg(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+304] -> [zr+256]
 mov rax, qword ptr [r12 + 304]
 mov qword ptr [r12 + 256], rax
 mov rax, qword ptr [r12 + 312]
 mov qword ptr [r12 + 264], rax
# marshal arg1 = producer-box slot [zr+384] -> [zr+272]
 mov rax, qword ptr [r12 + 384]
 mov qword ptr [r12 + 272], rax
 mov rax, qword ptr [r12 + 392]
 mov qword ptr [r12 + 280], rax
  .section .rodata
  .Lbynamefn7: .string "arg"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn7]
 lea rsi, [r12 + 256]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 cmp eax, 99
 je xchain0_n5_α
 jmp xchain0_n8_α
 xchain0_n6_β:
 jmp xchain0_n5_α
 xchain0_n7_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+672] -> [zr+640]
 mov rax, qword ptr [r12 + 672]
 mov qword ptr [r12 + 640], rax
 mov rax, qword ptr [r12 + 680]
 mov qword ptr [r12 + 648], rax
  .section .rodata
  .Lrkfn10: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn10]
 lea rsi, [r12 + 640]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 624], rax
 mov qword ptr [r12 + 632], rdx
 cmp eax, 99
 je xchain0_n10_α
 jmp xchain0_n9_α
 xchain0_n7_β:
 jmp xchain0_n10_α
# IR_LIT_STRING
 xchain0_n8_α:
 mov qword ptr [r12 + 416], 1
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 424], rax
 jmp xchain0_n11_α
 xchain0_n8_β:
 jmp xchain0_n5_α
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string "A"
# IR_LIT_INTEGER
 xchain0_n9_α:
 mov qword ptr [r12 + 704], 6
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [r12 + 712], rax
 jmp xchain0_n12_α
 xchain0_n9_β:
 jmp xchain0_n10_α
.Lx12_0:
 .quad 3
# IR_LIT_STRING
 xchain0_n10_α:
 mov qword ptr [r12 + 928], 1
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [r12 + 936], rax
 jmp xchain0_n13_α
 xchain0_n10_β:
 jmp xchain0_n17_α
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "jlab"
 xchain0_n11_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+240] -> [zr+192]
 mov rax, qword ptr [r12 + 240]
 mov qword ptr [r12 + 192], rax
 mov rax, qword ptr [r12 + 248]
 mov qword ptr [r12 + 200], rax
# marshal arg1 = producer-box slot [zr+416] -> [zr+208]
 mov rax, qword ptr [r12 + 416]
 mov qword ptr [r12 + 208], rax
 mov rax, qword ptr [r12 + 424]
 mov qword ptr [r12 + 216], rax
  .section .rodata
  .Lbynamefn12: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn12]
 lea rsi, [r12 + 192]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 cmp eax, 99
 je xchain0_n5_α
 jmp xchain0_n14_α
 xchain0_n11_β:
 jmp xchain0_n5_α
 xchain0_n12_α:
# BOX CALL arg(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+624] -> [zr+576]
 mov rax, qword ptr [r12 + 624]
 mov qword ptr [r12 + 576], rax
 mov rax, qword ptr [r12 + 632]
 mov qword ptr [r12 + 584], rax
# marshal arg1 = producer-box slot [zr+704] -> [zr+592]
 mov rax, qword ptr [r12 + 704]
 mov qword ptr [r12 + 592], rax
 mov rax, qword ptr [r12 + 712]
 mov qword ptr [r12 + 600], rax
  .section .rodata
  .Lbynamefn13: .string "arg"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn13]
 lea rsi, [r12 + 576]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 560], rax
 mov qword ptr [r12 + 568], rdx
 cmp eax, 99
 je xchain0_n10_α
 jmp xchain0_n15_α
 xchain0_n12_β:
 jmp xchain0_n10_α
 xchain0_n13_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+928] -> [zr+896]
 mov rax, qword ptr [r12 + 928]
 mov qword ptr [r12 + 896], rax
 mov rax, qword ptr [r12 + 936]
 mov qword ptr [r12 + 904], rax
  .section .rodata
  .Lrkfn17: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn17]
 lea rsi, [r12 + 896]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 880], rax
 mov qword ptr [r12 + 888], rdx
 cmp eax, 99
 je xchain0_n17_α
 jmp xchain0_n16_α
 xchain0_n13_β:
 jmp xchain0_n17_α
# IR_LIT_STRING
 xchain0_n14_α:
 mov qword ptr [r12 + 464], 1
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [r12 + 472], rax
 jmp xchain0_n18_α
 xchain0_n14_β:
 jmp main_γ
.Lx18_0:
 .quad .Lx18_0_s
.Lx18_0_s:
 .string "FAIL 1017/001: arg(.jlab,1) = A"
# IR_LIT_STRING
 xchain0_n15_α:
 mov qword ptr [r12 + 736], 1
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [r12 + 744], rax
 jmp xchain0_n19_α
 xchain0_n15_β:
 jmp xchain0_n10_α
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "C"
# IR_LIT_INTEGER
 xchain0_n16_α:
 mov qword ptr [r12 + 960], 6
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [r12 + 968], rax
 jmp xchain0_n20_α
 xchain0_n16_β:
 jmp xchain0_n17_α
.Lx20_0:
 .quad 0
# IR_LIT_STRING
 xchain0_n17_α:
 mov qword ptr [r12 + 1152], 1
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [r12 + 1160], rax
 jmp xchain0_n21_α
 xchain0_n17_β:
 jmp xchain0_n25_α
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string "jlab"
# IR_ASSIGN gva
 xchain0_n18_α:
 mov rax, qword ptr [r12 + 464]
 mov rdx, qword ptr [r12 + 472]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 448], rax
 mov qword ptr [r12 + 456], rdx
 jmp main_γ
 xchain0_n18_β:
 jmp main_γ
 xchain0_n19_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+560] -> [zr+512]
 mov rax, qword ptr [r12 + 560]
 mov qword ptr [r12 + 512], rax
 mov rax, qword ptr [r12 + 568]
 mov qword ptr [r12 + 520], rax
# marshal arg1 = producer-box slot [zr+736] -> [zr+528]
 mov rax, qword ptr [r12 + 736]
 mov qword ptr [r12 + 528], rax
 mov rax, qword ptr [r12 + 744]
 mov qword ptr [r12 + 536], rax
  .section .rodata
  .Lbynamefn20: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn20]
 lea rsi, [r12 + 512]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 496], rax
 mov qword ptr [r12 + 504], rdx
 cmp eax, 99
 je xchain0_n10_α
 jmp xchain0_n22_α
 xchain0_n19_β:
 jmp xchain0_n10_α
 xchain0_n20_α:
# BOX CALL arg(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+880] -> [zr+832]
 mov rax, qword ptr [r12 + 880]
 mov qword ptr [r12 + 832], rax
 mov rax, qword ptr [r12 + 888]
 mov qword ptr [r12 + 840], rax
# marshal arg1 = producer-box slot [zr+960] -> [zr+848]
 mov rax, qword ptr [r12 + 960]
 mov qword ptr [r12 + 848], rax
 mov rax, qword ptr [r12 + 968]
 mov qword ptr [r12 + 856], rax
  .section .rodata
  .Lbynamefn21: .string "arg"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn21]
 lea rsi, [r12 + 832]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 816], rax
 mov qword ptr [r12 + 824], rdx
 cmp eax, 99
 je xchain0_n17_α
 jmp xchain0_n23_α
 xchain0_n20_β:
 jmp xchain0_n17_α
 xchain0_n21_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1152] -> [zr+1120]
 mov rax, qword ptr [r12 + 1152]
 mov qword ptr [r12 + 1120], rax
 mov rax, qword ptr [r12 + 1160]
 mov qword ptr [r12 + 1128], rax
  .section .rodata
  .Lrkfn26: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn26]
 lea rsi, [r12 + 1120]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1104], rax
 mov qword ptr [r12 + 1112], rdx
 cmp eax, 99
 je xchain0_n25_α
 jmp xchain0_n24_α
 xchain0_n21_β:
 jmp xchain0_n25_α
# IR_LIT_STRING
 xchain0_n22_α:
 mov qword ptr [r12 + 784], 1
 mov rax, qword ptr [rip + .Lx27_0]
 mov qword ptr [r12 + 792], rax
 jmp xchain0_n26_α
 xchain0_n22_β:
 jmp main_γ
.Lx27_0:
 .quad .Lx27_0_s
.Lx27_0_s:
 .string "FAIL 1017/002: arg(.jlab,3) = C"
# IR_LIT_STRING
 xchain0_n23_α:
 mov qword ptr [r12 + 1008], 1
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [r12 + 1016], rax
 jmp xchain0_n27_α
 xchain0_n23_β:
 jmp main_γ
.Lx28_0:
 .quad .Lx28_0_s
.Lx28_0_s:
 .string "FAIL 1017/003: arg(.jlab,0) OOB should fail"
# IR_LIT_INTEGER
 xchain0_n24_α:
 mov qword ptr [r12 + 1184], 6
 mov rax, qword ptr [rip + .Lx29_0]
 mov qword ptr [r12 + 1192], rax
 jmp xchain0_n28_α
 xchain0_n24_β:
 jmp xchain0_n25_α
.Lx29_0:
 .quad 4
# IR_LIT_STRING
 xchain0_n25_α:
 mov qword ptr [r12 + 1440], 1
 mov rax, qword ptr [rip + .Lx30_0]
 mov qword ptr [r12 + 1448], rax
 jmp xchain0_n29_α
 xchain0_n25_β:
 jmp xchain0_n32_α
.Lx30_0:
 .quad .Lx30_0_s
.Lx30_0_s:
 .string "jlab"
# IR_ASSIGN gva
 xchain0_n26_α:
 mov rax, qword ptr [r12 + 784]
 mov rdx, qword ptr [r12 + 792]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 768], rax
 mov qword ptr [r12 + 776], rdx
 jmp main_γ
 xchain0_n26_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain0_n27_α:
 mov rax, qword ptr [r12 + 1008]
 mov rdx, qword ptr [r12 + 1016]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 992], rax
 mov qword ptr [r12 + 1000], rdx
 jmp main_γ
 xchain0_n27_β:
 jmp main_γ
 xchain0_n28_α:
# BOX CALL arg(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1104] -> [zr+1056]
 mov rax, qword ptr [r12 + 1104]
 mov qword ptr [r12 + 1056], rax
 mov rax, qword ptr [r12 + 1112]
 mov qword ptr [r12 + 1064], rax
# marshal arg1 = producer-box slot [zr+1184] -> [zr+1072]
 mov rax, qword ptr [r12 + 1184]
 mov qword ptr [r12 + 1072], rax
 mov rax, qword ptr [r12 + 1192]
 mov qword ptr [r12 + 1080], rax
  .section .rodata
  .Lbynamefn29: .string "arg"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn29]
 lea rsi, [r12 + 1056]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1040], rax
 mov qword ptr [r12 + 1048], rdx
 cmp eax, 99
 je xchain0_n25_α
 jmp xchain0_n30_α
 xchain0_n28_β:
 jmp xchain0_n25_α
 xchain0_n29_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1440] -> [zr+1408]
 mov rax, qword ptr [r12 + 1440]
 mov qword ptr [r12 + 1408], rax
 mov rax, qword ptr [r12 + 1448]
 mov qword ptr [r12 + 1416], rax
  .section .rodata
  .Lrkfn35: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn35]
 lea rsi, [r12 + 1408]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1392], rax
 mov qword ptr [r12 + 1400], rdx
 cmp eax, 99
 je xchain0_n32_α
 jmp xchain0_n31_α
 xchain0_n29_β:
 jmp xchain0_n32_α
# IR_LIT_STRING
 xchain0_n30_α:
 mov qword ptr [r12 + 1232], 1
 mov rax, qword ptr [rip + .Lx36_0]
 mov qword ptr [r12 + 1240], rax
 jmp xchain0_n33_α
 xchain0_n30_β:
 jmp main_γ
.Lx36_0:
 .quad .Lx36_0_s
.Lx36_0_s:
 .string "FAIL 1017/004: arg(.jlab,4) OOB should fail"
# IR_LIT_INTEGER
 xchain0_n31_α:
 mov qword ptr [r12 + 1472], 6
 mov rax, qword ptr [rip + .Lx37_0]
 mov qword ptr [r12 + 1480], rax
 jmp xchain0_n34_α
 xchain0_n31_β:
 jmp xchain0_n32_α
.Lx37_0:
 .quad 1
# IR_LIT_STRING
 xchain0_n32_α:
 mov qword ptr [r12 + 1760], 1
 mov rax, qword ptr [rip + .Lx38_0]
 mov qword ptr [r12 + 1768], rax
 jmp xchain0_n35_α
 xchain0_n32_β:
 jmp xchain0_n38_α
.Lx38_0:
 .quad .Lx38_0_s
.Lx38_0_s:
 .string "jlab"
# IR_ASSIGN gva
 xchain0_n33_α:
 mov rax, qword ptr [r12 + 1232]
 mov rdx, qword ptr [r12 + 1240]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1216], rax
 mov qword ptr [r12 + 1224], rdx
 jmp main_γ
 xchain0_n33_β:
 jmp main_γ
 xchain0_n34_α:
# BOX CALL local(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1392] -> [zr+1344]
 mov rax, qword ptr [r12 + 1392]
 mov qword ptr [r12 + 1344], rax
 mov rax, qword ptr [r12 + 1400]
 mov qword ptr [r12 + 1352], rax
# marshal arg1 = producer-box slot [zr+1472] -> [zr+1360]
 mov rax, qword ptr [r12 + 1472]
 mov qword ptr [r12 + 1360], rax
 mov rax, qword ptr [r12 + 1480]
 mov qword ptr [r12 + 1368], rax
  .section .rodata
  .Lbynamefn35: .string "local"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn35]
 lea rsi, [r12 + 1344]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1328], rax
 mov qword ptr [r12 + 1336], rdx
 cmp eax, 99
 je xchain0_n32_α
 jmp xchain0_n36_α
 xchain0_n34_β:
 jmp xchain0_n32_α
 xchain0_n35_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1760] -> [zr+1728]
 mov rax, qword ptr [r12 + 1760]
 mov qword ptr [r12 + 1728], rax
 mov rax, qword ptr [r12 + 1768]
 mov qword ptr [r12 + 1736], rax
  .section .rodata
  .Lrkfn42: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn42]
 lea rsi, [r12 + 1728]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1712], rax
 mov qword ptr [r12 + 1720], rdx
 cmp eax, 99
 je xchain0_n38_α
 jmp xchain0_n37_α
 xchain0_n35_β:
 jmp xchain0_n38_α
# IR_LIT_STRING
 xchain0_n36_α:
 mov qword ptr [r12 + 1504], 1
 mov rax, qword ptr [rip + .Lx43_0]
 mov qword ptr [r12 + 1512], rax
 jmp xchain0_n39_α
 xchain0_n36_β:
 jmp xchain0_n32_α
.Lx43_0:
 .quad .Lx43_0_s
.Lx43_0_s:
 .string "D"
# IR_LIT_INTEGER
 xchain0_n37_α:
 mov qword ptr [r12 + 1792], 6
 mov rax, qword ptr [rip + .Lx44_0]
 mov qword ptr [r12 + 1800], rax
 jmp xchain0_n40_α
 xchain0_n37_β:
 jmp xchain0_n38_α
.Lx44_0:
 .quad 3
# IR_LIT_STRING
 xchain0_n38_α:
 mov qword ptr [r12 + 2016], 1
 mov rax, qword ptr [rip + .Lx45_0]
 mov qword ptr [r12 + 2024], rax
 jmp xchain0_n41_α
 xchain0_n38_β:
 jmp xchain0_n45_α
.Lx45_0:
 .quad .Lx45_0_s
.Lx45_0_s:
 .string "jlab"
 xchain0_n39_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1328] -> [zr+1280]
 mov rax, qword ptr [r12 + 1328]
 mov qword ptr [r12 + 1280], rax
 mov rax, qword ptr [r12 + 1336]
 mov qword ptr [r12 + 1288], rax
# marshal arg1 = producer-box slot [zr+1504] -> [zr+1296]
 mov rax, qword ptr [r12 + 1504]
 mov qword ptr [r12 + 1296], rax
 mov rax, qword ptr [r12 + 1512]
 mov qword ptr [r12 + 1304], rax
  .section .rodata
  .Lbynamefn40: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn40]
 lea rsi, [r12 + 1280]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1264], rax
 mov qword ptr [r12 + 1272], rdx
 cmp eax, 99
 je xchain0_n32_α
 jmp xchain0_n42_α
 xchain0_n39_β:
 jmp xchain0_n32_α
 xchain0_n40_α:
# BOX CALL local(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1712] -> [zr+1664]
 mov rax, qword ptr [r12 + 1712]
 mov qword ptr [r12 + 1664], rax
 mov rax, qword ptr [r12 + 1720]
 mov qword ptr [r12 + 1672], rax
# marshal arg1 = producer-box slot [zr+1792] -> [zr+1680]
 mov rax, qword ptr [r12 + 1792]
 mov qword ptr [r12 + 1680], rax
 mov rax, qword ptr [r12 + 1800]
 mov qword ptr [r12 + 1688], rax
  .section .rodata
  .Lbynamefn41: .string "local"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn41]
 lea rsi, [r12 + 1664]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1648], rax
 mov qword ptr [r12 + 1656], rdx
 cmp eax, 99
 je xchain0_n38_α
 jmp xchain0_n43_α
 xchain0_n40_β:
 jmp xchain0_n38_α
 xchain0_n41_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2016] -> [zr+1984]
 mov rax, qword ptr [r12 + 2016]
 mov qword ptr [r12 + 1984], rax
 mov rax, qword ptr [r12 + 2024]
 mov qword ptr [r12 + 1992], rax
  .section .rodata
  .Lrkfn49: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn49]
 lea rsi, [r12 + 1984]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1968], rax
 mov qword ptr [r12 + 1976], rdx
 cmp eax, 99
 je xchain0_n45_α
 jmp xchain0_n44_α
 xchain0_n41_β:
 jmp xchain0_n45_α
# IR_LIT_STRING
 xchain0_n42_α:
 mov qword ptr [r12 + 1552], 1
 mov rax, qword ptr [rip + .Lx50_0]
 mov qword ptr [r12 + 1560], rax
 jmp xchain0_n46_α
 xchain0_n42_β:
 jmp main_γ
.Lx50_0:
 .quad .Lx50_0_s
.Lx50_0_s:
 .string "FAIL 1017/005: local(.jlab,1) = D"
# IR_LIT_STRING
 xchain0_n43_α:
 mov qword ptr [r12 + 1824], 1
 mov rax, qword ptr [rip + .Lx51_0]
 mov qword ptr [r12 + 1832], rax
 jmp xchain0_n47_α
 xchain0_n43_β:
 jmp xchain0_n38_α
.Lx51_0:
 .quad .Lx51_0_s
.Lx51_0_s:
 .string "F"
# IR_LIT_INTEGER
 xchain0_n44_α:
 mov qword ptr [r12 + 2048], 6
 mov rax, qword ptr [rip + .Lx52_0]
 mov qword ptr [r12 + 2056], rax
 jmp xchain0_n48_α
 xchain0_n44_β:
 jmp xchain0_n45_α
.Lx52_0:
 .quad 0
# IR_LIT_STRING
 xchain0_n45_α:
 mov qword ptr [r12 + 2240], 1
 mov rax, qword ptr [rip + .Lx53_0]
 mov qword ptr [r12 + 2248], rax
 jmp xchain0_n49_α
 xchain0_n45_β:
 jmp xchain0_n53_α
.Lx53_0:
 .quad .Lx53_0_s
.Lx53_0_s:
 .string "jlab"
# IR_ASSIGN gva
 xchain0_n46_α:
 mov rax, qword ptr [r12 + 1552]
 mov rdx, qword ptr [r12 + 1560]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1536], rax
 mov qword ptr [r12 + 1544], rdx
 jmp main_γ
 xchain0_n46_β:
 jmp main_γ
 xchain0_n47_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1648] -> [zr+1600]
 mov rax, qword ptr [r12 + 1648]
 mov qword ptr [r12 + 1600], rax
 mov rax, qword ptr [r12 + 1656]
 mov qword ptr [r12 + 1608], rax
# marshal arg1 = producer-box slot [zr+1824] -> [zr+1616]
 mov rax, qword ptr [r12 + 1824]
 mov qword ptr [r12 + 1616], rax
 mov rax, qword ptr [r12 + 1832]
 mov qword ptr [r12 + 1624], rax
  .section .rodata
  .Lbynamefn48: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn48]
 lea rsi, [r12 + 1600]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1584], rax
 mov qword ptr [r12 + 1592], rdx
 cmp eax, 99
 je xchain0_n38_α
 jmp xchain0_n50_α
 xchain0_n47_β:
 jmp xchain0_n38_α
 xchain0_n48_α:
# BOX CALL local(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1968] -> [zr+1920]
 mov rax, qword ptr [r12 + 1968]
 mov qword ptr [r12 + 1920], rax
 mov rax, qword ptr [r12 + 1976]
 mov qword ptr [r12 + 1928], rax
# marshal arg1 = producer-box slot [zr+2048] -> [zr+1936]
 mov rax, qword ptr [r12 + 2048]
 mov qword ptr [r12 + 1936], rax
 mov rax, qword ptr [r12 + 2056]
 mov qword ptr [r12 + 1944], rax
  .section .rodata
  .Lbynamefn49: .string "local"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn49]
 lea rsi, [r12 + 1920]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1904], rax
 mov qword ptr [r12 + 1912], rdx
 cmp eax, 99
 je xchain0_n45_α
 jmp xchain0_n51_α
 xchain0_n48_β:
 jmp xchain0_n45_α
 xchain0_n49_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2240] -> [zr+2208]
 mov rax, qword ptr [r12 + 2240]
 mov qword ptr [r12 + 2208], rax
 mov rax, qword ptr [r12 + 2248]
 mov qword ptr [r12 + 2216], rax
  .section .rodata
  .Lrkfn58: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn58]
 lea rsi, [r12 + 2208]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2192], rax
 mov qword ptr [r12 + 2200], rdx
 cmp eax, 99
 je xchain0_n53_α
 jmp xchain0_n52_α
 xchain0_n49_β:
 jmp xchain0_n53_α
# IR_LIT_STRING
 xchain0_n50_α:
 mov qword ptr [r12 + 1872], 1
 mov rax, qword ptr [rip + .Lx59_0]
 mov qword ptr [r12 + 1880], rax
 jmp xchain0_n54_α
 xchain0_n50_β:
 jmp main_γ
.Lx59_0:
 .quad .Lx59_0_s
.Lx59_0_s:
 .string "FAIL 1017/006: local(.jlab,3) = F"
# IR_LIT_STRING
 xchain0_n51_α:
 mov qword ptr [r12 + 2096], 1
 mov rax, qword ptr [rip + .Lx60_0]
 mov qword ptr [r12 + 2104], rax
 jmp xchain0_n55_α
 xchain0_n51_β:
 jmp main_γ
.Lx60_0:
 .quad .Lx60_0_s
.Lx60_0_s:
 .string "FAIL 1017/007: local(.jlab,0) OOB should fail"
# IR_LIT_INTEGER
 xchain0_n52_α:
 mov qword ptr [r12 + 2272], 6
 mov rax, qword ptr [rip + .Lx61_0]
 mov qword ptr [r12 + 2280], rax
 jmp xchain0_n56_α
 xchain0_n52_β:
 jmp xchain0_n53_α
.Lx61_0:
 .quad 4
# IR_LIT_STRING
 xchain0_n53_α:
 mov qword ptr [r12 + 2368], 1
 mov rax, qword ptr [rip + .Lx62_0]
 mov qword ptr [r12 + 2376], rax
 jmp xchain0_n57_α
 xchain0_n53_β:
 jmp main_γ
.Lx62_0:
 .quad .Lx62_0_s
.Lx62_0_s:
 .string "PASS 1017_arg_local (8/8)"
# IR_ASSIGN gva
 xchain0_n54_α:
 mov rax, qword ptr [r12 + 1872]
 mov rdx, qword ptr [r12 + 1880]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1856], rax
 mov qword ptr [r12 + 1864], rdx
 jmp main_γ
 xchain0_n54_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain0_n55_α:
 mov rax, qword ptr [r12 + 2096]
 mov rdx, qword ptr [r12 + 2104]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 2080], rax
 mov qword ptr [r12 + 2088], rdx
 jmp main_γ
 xchain0_n55_β:
 jmp main_γ
 xchain0_n56_α:
# BOX CALL local(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2192] -> [zr+2144]
 mov rax, qword ptr [r12 + 2192]
 mov qword ptr [r12 + 2144], rax
 mov rax, qword ptr [r12 + 2200]
 mov qword ptr [r12 + 2152], rax
# marshal arg1 = producer-box slot [zr+2272] -> [zr+2160]
 mov rax, qword ptr [r12 + 2272]
 mov qword ptr [r12 + 2160], rax
 mov rax, qword ptr [r12 + 2280]
 mov qword ptr [r12 + 2168], rax
  .section .rodata
  .Lbynamefn57: .string "local"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn57]
 lea rsi, [r12 + 2144]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2128], rax
 mov qword ptr [r12 + 2136], rdx
 cmp eax, 99
 je xchain0_n53_α
 jmp xchain0_n58_α
 xchain0_n56_β:
 jmp xchain0_n53_α
# IR_ASSIGN gva
 xchain0_n57_α:
 mov rax, qword ptr [r12 + 2368]
 mov rdx, qword ptr [r12 + 2376]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 2352], rax
 mov qword ptr [r12 + 2360], rdx
 jmp main_γ
 xchain0_n57_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n58_α:
 mov qword ptr [r12 + 2320], 1
 mov rax, qword ptr [rip + .Lx67_0]
 mov qword ptr [r12 + 2328], rax
 jmp xchain0_n59_α
 xchain0_n58_β:
 jmp main_γ
.Lx67_0:
 .quad .Lx67_0_s
.Lx67_0_s:
 .string "FAIL 1017/008: local(.jlab,4) OOB should fail"
# IR_ASSIGN gva
 xchain0_n59_α:
 mov rax, qword ptr [r12 + 2320]
 mov rdx, qword ptr [r12 + 2328]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 2304], rax
 mov qword ptr [r12 + 2312], rdx
 jmp main_γ
 xchain0_n59_β:
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [r12 + 2408]
pop r12
ret
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 2400]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, qword ptr [r12 + 2408]
pop r12
ret

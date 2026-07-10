  .intel_syntax noprefix
  .text
  .globl main
main:
  push rbp
  mov rbp, rsp
  push rdi
  push rsi
  call core_lib_init@PLT
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
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 1456], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [r12 + 112], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n3_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "TRIM"
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [r12 + 128], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 136], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n3_α
.Lx2_0:
 .quad 1
 xchain0_n2_α:
# BOX IR_CALL SNO$KWSET(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+112] -> [zr+80]
 mov rax, qword ptr [r12 + 112]
 mov qword ptr [r12 + 80], rax
 mov rax, qword ptr [r12 + 120]
 mov qword ptr [r12 + 88], rax
# marshal arg1 = producer-box slot [zr+128] -> [zr+96]
 mov rax, qword ptr [r12 + 128]
 mov qword ptr [r12 + 96], rax
 mov rax, qword ptr [r12 + 136]
 mov qword ptr [r12 + 104], rax
  .section .rodata
  .Lrkfn4: .string "SNO$KWSET"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn4]
 lea rsi, [r12 + 80]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n3_α
# IR_LIT_STRING
 xchain0_n3_α:
 mov qword ptr [r12 + 224], 1
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [r12 + 232], rax
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n8_α
.Lx5_0:
 .quad .Lx5_0_s
.Lx5_0_s:
 .string "hi"
# IR_LIT_INTEGER
 xchain0_n4_α:
 mov qword ptr [r12 + 240], 6
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 248], rax
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n8_α
.Lx6_0:
 .quad 6
# IR_LIT_STRING
 xchain0_n5_α:
 mov qword ptr [r12 + 256], 1
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [r12 + 264], rax
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n8_α
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string "*"
 xchain0_n6_α:
# BOX IR_CALL pad_left(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+224] -> [zr+176]
 mov rax, qword ptr [r12 + 224]
 mov qword ptr [r12 + 176], rax
 mov rax, qword ptr [r12 + 232]
 mov qword ptr [r12 + 184], rax
# marshal arg1 = producer-box slot [zr+240] -> [zr+192]
 mov rax, qword ptr [r12 + 240]
 mov qword ptr [r12 + 192], rax
 mov rax, qword ptr [r12 + 248]
 mov qword ptr [r12 + 200], rax
# marshal arg2 = producer-box slot [zr+256] -> [zr+208]
 mov rax, qword ptr [r12 + 256]
 mov qword ptr [r12 + 208], rax
 mov rax, qword ptr [r12 + 264]
 mov qword ptr [r12 + 216], rax
  .section .rodata
  .Lbynamefn7: .string "pad_left"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn7]
 lea rsi, [r12 + 176]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 cmp eax, 99
 je xchain0_n8_α
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp xchain0_n8_α
# IR_ASSIGN global
 xchain0_n7_α:
 mov rsi, qword ptr [r12 + 160]
 mov rdx, qword ptr [r12 + 168]
 mov rdi, qword ptr [rip + .Lx9_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp xchain0_n8_α
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n8_α:
 mov qword ptr [r12 + 352], 1
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [r12 + 360], rax
 jmp xchain0_n9_α
 xchain0_n8_β:
 jmp xchain0_n13_α
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "hi"
# IR_LIT_INTEGER
 xchain0_n9_α:
 mov qword ptr [r12 + 368], 6
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 376], rax
 jmp xchain0_n10_α
 xchain0_n9_β:
 jmp xchain0_n13_α
.Lx11_0:
 .quad 6
# IR_LIT_STRING
 xchain0_n10_α:
 mov qword ptr [r12 + 384], 1
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [r12 + 392], rax
 jmp xchain0_n11_α
 xchain0_n10_β:
 jmp xchain0_n13_α
.Lx12_0:
 .quad .Lx12_0_s
.Lx12_0_s:
 .string "*"
 xchain0_n11_α:
# BOX IR_CALL pad_right(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+352] -> [zr+304]
 mov rax, qword ptr [r12 + 352]
 mov qword ptr [r12 + 304], rax
 mov rax, qword ptr [r12 + 360]
 mov qword ptr [r12 + 312], rax
# marshal arg1 = producer-box slot [zr+368] -> [zr+320]
 mov rax, qword ptr [r12 + 368]
 mov qword ptr [r12 + 320], rax
 mov rax, qword ptr [r12 + 376]
 mov qword ptr [r12 + 328], rax
# marshal arg2 = producer-box slot [zr+384] -> [zr+336]
 mov rax, qword ptr [r12 + 384]
 mov qword ptr [r12 + 336], rax
 mov rax, qword ptr [r12 + 392]
 mov qword ptr [r12 + 344], rax
  .section .rodata
  .Lbynamefn12: .string "pad_right"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn12]
 lea rsi, [r12 + 304]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 288], rax
 mov qword ptr [r12 + 296], rdx
 cmp eax, 99
 je xchain0_n13_α
 jmp xchain0_n12_α
 xchain0_n11_β:
 jmp xchain0_n13_α
# IR_ASSIGN global
 xchain0_n12_α:
 mov rsi, qword ptr [r12 + 288]
 mov rdx, qword ptr [r12 + 296]
 mov rdi, qword ptr [rip + .Lx14_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 jmp xchain0_n13_α
 xchain0_n12_β:
 jmp xchain0_n13_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n13_α:
 mov qword ptr [r12 + 448], 1
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [r12 + 456], rax
 jmp xchain0_n14_α
 xchain0_n13_β:
 jmp xchain0_n16_α
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "   hello"
 xchain0_n14_α:
# BOX IR_CALL ltrim(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+448] -> [zr+432]
 mov rax, qword ptr [r12 + 448]
 mov qword ptr [r12 + 432], rax
 mov rax, qword ptr [r12 + 456]
 mov qword ptr [r12 + 440], rax
  .section .rodata
  .Lbynamefn15: .string "ltrim"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn15]
 lea rsi, [r12 + 432]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 cmp eax, 99
 je xchain0_n16_α
 jmp xchain0_n15_α
 xchain0_n14_β:
 jmp xchain0_n16_α
# IR_ASSIGN global
 xchain0_n15_α:
 mov rsi, qword ptr [r12 + 416]
 mov rdx, qword ptr [r12 + 424]
 mov rdi, qword ptr [rip + .Lx17_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 400], rax
 mov qword ptr [r12 + 408], rdx
 jmp xchain0_n16_α
 xchain0_n15_β:
 jmp xchain0_n16_α
.Lx17_0:
 .quad .Lx17_0_s
.Lx17_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n16_α:
 mov qword ptr [r12 + 512], 1
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [r12 + 520], rax
 jmp xchain0_n17_α
 xchain0_n16_β:
 jmp xchain0_n19_α
.Lx18_0:
 .quad .Lx18_0_s
.Lx18_0_s:
 .string "hello   "
 xchain0_n17_α:
# BOX IR_CALL rtrim(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+512] -> [zr+496]
 mov rax, qword ptr [r12 + 512]
 mov qword ptr [r12 + 496], rax
 mov rax, qword ptr [r12 + 520]
 mov qword ptr [r12 + 504], rax
  .section .rodata
  .Lbynamefn18: .string "rtrim"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn18]
 lea rsi, [r12 + 496]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 480], rax
 mov qword ptr [r12 + 488], rdx
 cmp eax, 99
 je xchain0_n19_α
 jmp xchain0_n18_α
 xchain0_n17_β:
 jmp xchain0_n19_α
# IR_ASSIGN global
 xchain0_n18_α:
 mov rsi, qword ptr [r12 + 480]
 mov rdx, qword ptr [r12 + 488]
 mov rdi, qword ptr [rip + .Lx20_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 464], rax
 mov qword ptr [r12 + 472], rdx
 jmp xchain0_n19_α
 xchain0_n18_β:
 jmp xchain0_n19_α
.Lx20_0:
 .quad .Lx20_0_s
.Lx20_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n19_α:
 mov qword ptr [r12 + 576], 1
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [r12 + 584], rax
 jmp xchain0_n20_α
 xchain0_n19_β:
 jmp xchain0_n22_α
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string "  hello  "
 xchain0_n20_α:
# BOX IR_CALL trimws(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+576] -> [zr+560]
 mov rax, qword ptr [r12 + 576]
 mov qword ptr [r12 + 560], rax
 mov rax, qword ptr [r12 + 584]
 mov qword ptr [r12 + 568], rax
  .section .rodata
  .Lbynamefn21: .string "trimws"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn21]
 lea rsi, [r12 + 560]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 544], rax
 mov qword ptr [r12 + 552], rdx
 cmp eax, 99
 je xchain0_n22_α
 jmp xchain0_n21_α
 xchain0_n20_β:
 jmp xchain0_n22_α
# IR_ASSIGN global
 xchain0_n21_α:
 mov rsi, qword ptr [r12 + 544]
 mov rdx, qword ptr [r12 + 552]
 mov rdi, qword ptr [rip + .Lx23_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 528], rax
 mov qword ptr [r12 + 536], rdx
 jmp xchain0_n22_α
 xchain0_n21_β:
 jmp xchain0_n22_α
.Lx23_0:
 .quad .Lx23_0_s
.Lx23_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n22_α:
 mov qword ptr [r12 + 656], 1
 mov rax, qword ptr [rip + .Lx24_0]
 mov qword ptr [r12 + 664], rax
 jmp xchain0_n23_α
 xchain0_n22_β:
 jmp xchain0_n26_α
.Lx24_0:
 .quad .Lx24_0_s
.Lx24_0_s:
 .string "hi"
# IR_LIT_INTEGER
 xchain0_n23_α:
 mov qword ptr [r12 + 672], 6
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [r12 + 680], rax
 jmp xchain0_n24_α
 xchain0_n23_β:
 jmp xchain0_n26_α
.Lx25_0:
 .quad 3
 xchain0_n24_α:
# BOX IR_CALL repeat(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+656] -> [zr+624]
 mov rax, qword ptr [r12 + 656]
 mov qword ptr [r12 + 624], rax
 mov rax, qword ptr [r12 + 664]
 mov qword ptr [r12 + 632], rax
# marshal arg1 = producer-box slot [zr+672] -> [zr+640]
 mov rax, qword ptr [r12 + 672]
 mov qword ptr [r12 + 640], rax
 mov rax, qword ptr [r12 + 680]
 mov qword ptr [r12 + 648], rax
  .section .rodata
  .Lbynamefn25: .string "repeat"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn25]
 lea rsi, [r12 + 624]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 608], rax
 mov qword ptr [r12 + 616], rdx
 cmp eax, 99
 je xchain0_n26_α
 jmp xchain0_n25_α
 xchain0_n24_β:
 jmp xchain0_n26_α
# IR_ASSIGN global
 xchain0_n25_α:
 mov rsi, qword ptr [r12 + 608]
 mov rdx, qword ptr [r12 + 616]
 mov rdi, qword ptr [rip + .Lx27_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 592], rax
 mov qword ptr [r12 + 600], rdx
 jmp xchain0_n26_α
 xchain0_n25_β:
 jmp xchain0_n26_α
.Lx27_0:
 .quad .Lx27_0_s
.Lx27_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n26_α:
 mov qword ptr [r12 + 736], 1
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [r12 + 744], rax
 jmp xchain0_n27_α
 xchain0_n26_β:
 jmp xchain0_n30_α
.Lx28_0:
 .quad .Lx28_0_s
.Lx28_0_s:
 .string "foobar"
# IR_LIT_STRING
 xchain0_n27_α:
 mov qword ptr [r12 + 752], 1
 mov rax, qword ptr [rip + .Lx29_0]
 mov qword ptr [r12 + 760], rax
 jmp xchain0_n28_α
 xchain0_n27_β:
 jmp xchain0_n30_α
.Lx29_0:
 .quad .Lx29_0_s
.Lx29_0_s:
 .string "oba"
 xchain0_n28_α:
# BOX IR_CALL contains(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+736] -> [zr+704]
 mov rax, qword ptr [r12 + 736]
 mov qword ptr [r12 + 704], rax
 mov rax, qword ptr [r12 + 744]
 mov qword ptr [r12 + 712], rax
# marshal arg1 = producer-box slot [zr+752] -> [zr+720]
 mov rax, qword ptr [r12 + 752]
 mov qword ptr [r12 + 720], rax
 mov rax, qword ptr [r12 + 760]
 mov qword ptr [r12 + 728], rax
  .section .rodata
  .Lbynamefn29: .string "contains"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn29]
 lea rsi, [r12 + 704]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 688], rax
 mov qword ptr [r12 + 696], rdx
 cmp eax, 99
 je xchain0_n30_α
 jmp xchain0_n29_α
 xchain0_n28_β:
 jmp xchain0_n30_α
# IR_LIT_STRING
 xchain0_n29_α:
 mov qword ptr [r12 + 784], 1
 mov rax, qword ptr [rip + .Lx31_0]
 mov qword ptr [r12 + 792], rax
 jmp xchain0_n31_α
 xchain0_n29_β:
 jmp xchain0_n33_α
.Lx31_0:
 .quad .Lx31_0_s
.Lx31_0_s:
 .string "contains ok"
# IR_LIT_STRING
 xchain0_n30_α:
 mov qword ptr [r12 + 816], 1
 mov rax, qword ptr [rip + .Lx32_0]
 mov qword ptr [r12 + 824], rax
 jmp xchain0_n32_α
 xchain0_n30_β:
 jmp xchain0_n33_α
.Lx32_0:
 .quad .Lx32_0_s
.Lx32_0_s:
 .string "FAIL: contains"
# IR_ASSIGN global
 xchain0_n31_α:
 mov rsi, qword ptr [r12 + 784]
 mov rdx, qword ptr [r12 + 792]
 mov rdi, qword ptr [rip + .Lx33_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 768], rax
 mov qword ptr [r12 + 776], rdx
 jmp xchain0_n33_α
 xchain0_n31_β:
 jmp xchain0_n33_α
.Lx33_0:
 .quad .Lx33_0_s
.Lx33_0_s:
 .string "OUTPUT"
# IR_ASSIGN global
 xchain0_n32_α:
 mov rsi, qword ptr [r12 + 816]
 mov rdx, qword ptr [r12 + 824]
 mov rdi, qword ptr [rip + .Lx34_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 800], rax
 mov qword ptr [r12 + 808], rdx
 jmp xchain0_n33_α
 xchain0_n32_β:
 jmp xchain0_n33_α
.Lx34_0:
 .quad .Lx34_0_s
.Lx34_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n33_α:
 mov qword ptr [r12 + 880], 1
 mov rax, qword ptr [rip + .Lx35_0]
 mov qword ptr [r12 + 888], rax
 jmp xchain0_n34_α
 xchain0_n33_β:
 jmp xchain0_n37_α
.Lx35_0:
 .quad .Lx35_0_s
.Lx35_0_s:
 .string "foobar"
# IR_LIT_STRING
 xchain0_n34_α:
 mov qword ptr [r12 + 896], 1
 mov rax, qword ptr [rip + .Lx36_0]
 mov qword ptr [r12 + 904], rax
 jmp xchain0_n35_α
 xchain0_n34_β:
 jmp xchain0_n37_α
.Lx36_0:
 .quad .Lx36_0_s
.Lx36_0_s:
 .string "foo"
 xchain0_n35_α:
# BOX IR_CALL startswith(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+880] -> [zr+848]
 mov rax, qword ptr [r12 + 880]
 mov qword ptr [r12 + 848], rax
 mov rax, qword ptr [r12 + 888]
 mov qword ptr [r12 + 856], rax
# marshal arg1 = producer-box slot [zr+896] -> [zr+864]
 mov rax, qword ptr [r12 + 896]
 mov qword ptr [r12 + 864], rax
 mov rax, qword ptr [r12 + 904]
 mov qword ptr [r12 + 872], rax
  .section .rodata
  .Lbynamefn36: .string "startswith"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn36]
 lea rsi, [r12 + 848]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 832], rax
 mov qword ptr [r12 + 840], rdx
 cmp eax, 99
 je xchain0_n37_α
 jmp xchain0_n36_α
 xchain0_n35_β:
 jmp xchain0_n37_α
# IR_LIT_STRING
 xchain0_n36_α:
 mov qword ptr [r12 + 928], 1
 mov rax, qword ptr [rip + .Lx38_0]
 mov qword ptr [r12 + 936], rax
 jmp xchain0_n38_α
 xchain0_n36_β:
 jmp xchain0_n40_α
.Lx38_0:
 .quad .Lx38_0_s
.Lx38_0_s:
 .string "startswith ok"
# IR_LIT_STRING
 xchain0_n37_α:
 mov qword ptr [r12 + 960], 1
 mov rax, qword ptr [rip + .Lx39_0]
 mov qword ptr [r12 + 968], rax
 jmp xchain0_n39_α
 xchain0_n37_β:
 jmp xchain0_n40_α
.Lx39_0:
 .quad .Lx39_0_s
.Lx39_0_s:
 .string "FAIL: startswith"
# IR_ASSIGN global
 xchain0_n38_α:
 mov rsi, qword ptr [r12 + 928]
 mov rdx, qword ptr [r12 + 936]
 mov rdi, qword ptr [rip + .Lx40_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 912], rax
 mov qword ptr [r12 + 920], rdx
 jmp xchain0_n40_α
 xchain0_n38_β:
 jmp xchain0_n40_α
.Lx40_0:
 .quad .Lx40_0_s
.Lx40_0_s:
 .string "OUTPUT"
# IR_ASSIGN global
 xchain0_n39_α:
 mov rsi, qword ptr [r12 + 960]
 mov rdx, qword ptr [r12 + 968]
 mov rdi, qword ptr [rip + .Lx41_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 944], rax
 mov qword ptr [r12 + 952], rdx
 jmp xchain0_n40_α
 xchain0_n39_β:
 jmp xchain0_n40_α
.Lx41_0:
 .quad .Lx41_0_s
.Lx41_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n40_α:
 mov qword ptr [r12 + 1024], 1
 mov rax, qword ptr [rip + .Lx42_0]
 mov qword ptr [r12 + 1032], rax
 jmp xchain0_n41_α
 xchain0_n40_β:
 jmp xchain0_n44_α
.Lx42_0:
 .quad .Lx42_0_s
.Lx42_0_s:
 .string "foobar"
# IR_LIT_STRING
 xchain0_n41_α:
 mov qword ptr [r12 + 1040], 1
 mov rax, qword ptr [rip + .Lx43_0]
 mov qword ptr [r12 + 1048], rax
 jmp xchain0_n42_α
 xchain0_n41_β:
 jmp xchain0_n44_α
.Lx43_0:
 .quad .Lx43_0_s
.Lx43_0_s:
 .string "bar"
 xchain0_n42_α:
# BOX IR_CALL endswith(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1024] -> [zr+992]
 mov rax, qword ptr [r12 + 1024]
 mov qword ptr [r12 + 992], rax
 mov rax, qword ptr [r12 + 1032]
 mov qword ptr [r12 + 1000], rax
# marshal arg1 = producer-box slot [zr+1040] -> [zr+1008]
 mov rax, qword ptr [r12 + 1040]
 mov qword ptr [r12 + 1008], rax
 mov rax, qword ptr [r12 + 1048]
 mov qword ptr [r12 + 1016], rax
  .section .rodata
  .Lbynamefn43: .string "endswith"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn43]
 lea rsi, [r12 + 992]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 976], rax
 mov qword ptr [r12 + 984], rdx
 cmp eax, 99
 je xchain0_n44_α
 jmp xchain0_n43_α
 xchain0_n42_β:
 jmp xchain0_n44_α
# IR_LIT_STRING
 xchain0_n43_α:
 mov qword ptr [r12 + 1072], 1
 mov rax, qword ptr [rip + .Lx45_0]
 mov qword ptr [r12 + 1080], rax
 jmp xchain0_n45_α
 xchain0_n43_β:
 jmp xchain0_n47_α
.Lx45_0:
 .quad .Lx45_0_s
.Lx45_0_s:
 .string "endswith ok"
# IR_LIT_STRING
 xchain0_n44_α:
 mov qword ptr [r12 + 1104], 1
 mov rax, qword ptr [rip + .Lx46_0]
 mov qword ptr [r12 + 1112], rax
 jmp xchain0_n46_α
 xchain0_n44_β:
 jmp xchain0_n47_α
.Lx46_0:
 .quad .Lx46_0_s
.Lx46_0_s:
 .string "FAIL: endswith"
# IR_ASSIGN global
 xchain0_n45_α:
 mov rsi, qword ptr [r12 + 1072]
 mov rdx, qword ptr [r12 + 1080]
 mov rdi, qword ptr [rip + .Lx47_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 1056], rax
 mov qword ptr [r12 + 1064], rdx
 jmp xchain0_n47_α
 xchain0_n45_β:
 jmp xchain0_n47_α
.Lx47_0:
 .quad .Lx47_0_s
.Lx47_0_s:
 .string "OUTPUT"
# IR_ASSIGN global
 xchain0_n46_α:
 mov rsi, qword ptr [r12 + 1104]
 mov rdx, qword ptr [r12 + 1112]
 mov rdi, qword ptr [rip + .Lx48_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 1088], rax
 mov qword ptr [r12 + 1096], rdx
 jmp xchain0_n47_α
 xchain0_n46_β:
 jmp xchain0_n47_α
.Lx48_0:
 .quad .Lx48_0_s
.Lx48_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n47_α:
 mov qword ptr [r12 + 1168], 1
 mov rax, qword ptr [rip + .Lx49_0]
 mov qword ptr [r12 + 1176], rax
 jmp xchain0_n48_α
 xchain0_n47_β:
 jmp xchain0_n51_α
.Lx49_0:
 .quad .Lx49_0_s
.Lx49_0_s:
 .string "foobar"
# IR_LIT_STRING
 xchain0_n48_α:
 mov qword ptr [r12 + 1184], 1
 mov rax, qword ptr [rip + .Lx50_0]
 mov qword ptr [r12 + 1192], rax
 jmp xchain0_n49_α
 xchain0_n48_β:
 jmp xchain0_n51_α
.Lx50_0:
 .quad .Lx50_0_s
.Lx50_0_s:
 .string "bar"
 xchain0_n49_α:
# BOX IR_CALL startswith(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1168] -> [zr+1136]
 mov rax, qword ptr [r12 + 1168]
 mov qword ptr [r12 + 1136], rax
 mov rax, qword ptr [r12 + 1176]
 mov qword ptr [r12 + 1144], rax
# marshal arg1 = producer-box slot [zr+1184] -> [zr+1152]
 mov rax, qword ptr [r12 + 1184]
 mov qword ptr [r12 + 1152], rax
 mov rax, qword ptr [r12 + 1192]
 mov qword ptr [r12 + 1160], rax
  .section .rodata
  .Lbynamefn50: .string "startswith"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn50]
 lea rsi, [r12 + 1136]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1120], rax
 mov qword ptr [r12 + 1128], rdx
 cmp eax, 99
 je xchain0_n51_α
 jmp xchain0_n50_α
 xchain0_n49_β:
 jmp xchain0_n51_α
# IR_LIT_STRING
 xchain0_n50_α:
 mov qword ptr [r12 + 1248], 1
 mov rax, qword ptr [rip + .Lx52_0]
 mov qword ptr [r12 + 1256], rax
 jmp xchain0_n52_α
 xchain0_n50_β:
 jmp xchain0_n54_α
.Lx52_0:
 .quad .Lx52_0_s
.Lx52_0_s:
 .string "FAIL: startswith matched wrong"
# IR_LIT_STRING
 xchain0_n51_α:
 mov qword ptr [r12 + 1216], 1
 mov rax, qword ptr [rip + .Lx53_0]
 mov qword ptr [r12 + 1224], rax
 jmp xchain0_n53_α
 xchain0_n51_β:
 jmp xchain0_n54_α
.Lx53_0:
 .quad .Lx53_0_s
.Lx53_0_s:
 .string "no startswith ok"
# IR_ASSIGN global
 xchain0_n52_α:
 mov rsi, qword ptr [r12 + 1248]
 mov rdx, qword ptr [r12 + 1256]
 mov rdi, qword ptr [rip + .Lx54_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 1232], rax
 mov qword ptr [r12 + 1240], rdx
 jmp xchain0_n54_α
 xchain0_n52_β:
 jmp xchain0_n54_α
.Lx54_0:
 .quad .Lx54_0_s
.Lx54_0_s:
 .string "OUTPUT"
# IR_ASSIGN global
 xchain0_n53_α:
 mov rsi, qword ptr [r12 + 1216]
 mov rdx, qword ptr [r12 + 1224]
 mov rdi, qword ptr [rip + .Lx55_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 1200], rax
 mov qword ptr [r12 + 1208], rdx
 jmp xchain0_n54_α
 xchain0_n53_β:
 jmp xchain0_n54_α
.Lx55_0:
 .quad .Lx55_0_s
.Lx55_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n54_α:
 mov qword ptr [r12 + 1328], 1
 mov rax, qword ptr [rip + .Lx56_0]
 mov qword ptr [r12 + 1336], rax
 jmp xchain0_n55_α
 xchain0_n54_β:
 jmp xchain0_n58_α
.Lx56_0:
 .quad .Lx56_0_s
.Lx56_0_s:
 .string "foobar"
# IR_LIT_STRING
 xchain0_n55_α:
 mov qword ptr [r12 + 1344], 1
 mov rax, qword ptr [rip + .Lx57_0]
 mov qword ptr [r12 + 1352], rax
 jmp xchain0_n56_α
 xchain0_n55_β:
 jmp xchain0_n58_α
.Lx57_0:
 .quad .Lx57_0_s
.Lx57_0_s:
 .string "oba"
 xchain0_n56_α:
# BOX IR_CALL index(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1328] -> [zr+1296]
 mov rax, qword ptr [r12 + 1328]
 mov qword ptr [r12 + 1296], rax
 mov rax, qword ptr [r12 + 1336]
 mov qword ptr [r12 + 1304], rax
# marshal arg1 = producer-box slot [zr+1344] -> [zr+1312]
 mov rax, qword ptr [r12 + 1344]
 mov qword ptr [r12 + 1312], rax
 mov rax, qword ptr [r12 + 1352]
 mov qword ptr [r12 + 1320], rax
  .section .rodata
  .Lbynamefn57: .string "index"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn57]
 lea rsi, [r12 + 1296]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1280], rax
 mov qword ptr [r12 + 1288], rdx
 cmp eax, 99
 je xchain0_n58_α
 jmp xchain0_n57_α
 xchain0_n56_β:
 jmp xchain0_n58_α
# IR_ASSIGN global
 xchain0_n57_α:
 mov rsi, qword ptr [r12 + 1280]
 mov rdx, qword ptr [r12 + 1288]
 mov rdi, qword ptr [rip + .Lx59_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 1264], rax
 mov qword ptr [r12 + 1272], rdx
 jmp xchain0_n58_α
 xchain0_n57_β:
 jmp xchain0_n58_α
.Lx59_0:
 .quad .Lx59_0_s
.Lx59_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n58_α:
 mov qword ptr [r12 + 1424], 1
 mov rax, qword ptr [rip + .Lx60_0]
 mov qword ptr [r12 + 1432], rax
 jmp xchain0_n59_α
 xchain0_n58_β:
 jmp main_γ
.Lx60_0:
 .quad .Lx60_0_s
.Lx60_0_s:
 .string "foobar"
# IR_LIT_STRING
 xchain0_n59_α:
 mov qword ptr [r12 + 1440], 1
 mov rax, qword ptr [rip + .Lx61_0]
 mov qword ptr [r12 + 1448], rax
 jmp xchain0_n60_α
 xchain0_n59_β:
 jmp main_γ
.Lx61_0:
 .quad .Lx61_0_s
.Lx61_0_s:
 .string "xyz"
 xchain0_n60_α:
# BOX IR_CALL index(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1424] -> [zr+1392]
 mov rax, qword ptr [r12 + 1424]
 mov qword ptr [r12 + 1392], rax
 mov rax, qword ptr [r12 + 1432]
 mov qword ptr [r12 + 1400], rax
# marshal arg1 = producer-box slot [zr+1440] -> [zr+1408]
 mov rax, qword ptr [r12 + 1440]
 mov qword ptr [r12 + 1408], rax
 mov rax, qword ptr [r12 + 1448]
 mov qword ptr [r12 + 1416], rax
  .section .rodata
  .Lbynamefn61: .string "index"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn61]
 lea rsi, [r12 + 1392]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1376], rax
 mov qword ptr [r12 + 1384], rdx
 cmp eax, 99
 je main_γ
 jmp xchain0_n61_α
 xchain0_n60_β:
 jmp main_γ
# IR_ASSIGN global
 xchain0_n61_α:
 mov rsi, qword ptr [r12 + 1376]
 mov rdx, qword ptr [r12 + 1384]
 mov rdi, qword ptr [rip + .Lx63_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 1360], rax
 mov qword ptr [r12 + 1368], rdx
 jmp main_γ
 xchain0_n61_β:
 jmp main_γ
.Lx63_0:
 .quad .Lx63_0_s
.Lx63_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 1456]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret

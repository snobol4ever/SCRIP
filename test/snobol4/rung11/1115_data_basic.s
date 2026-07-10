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
  push rdi
  push rsi
  call core_lib_init@PLT
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 3
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
 mov qword ptr [r12 + 1520], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [r12 + 96], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 104], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n2_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "node(val,lson,rson)"
 xchain0_n1_α:
# BOX IR_CALL data(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+96] -> [zr+80]
 mov rax, qword ptr [r12 + 96]
 mov qword ptr [r12 + 80], rax
 mov rax, qword ptr [r12 + 104]
 mov qword ptr [r12 + 88], rax
  .section .rodata
  .Lbynamefn3: .string "data"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn3]
 lea rsi, [r12 + 80]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
 cmp eax, 99
 je xchain0_n2_α
 jmp xchain0_n2_α
xchain0_n1_β:
 jmp xchain0_n2_α
# IR_LIT_STRING
 xchain0_n2_α:
 mov qword ptr [r12 + 192], 1
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [r12 + 200], rax
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n7_α
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "x"
# IR_LIT_STRING
 xchain0_n3_α:
 mov qword ptr [r12 + 208], 1
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [r12 + 216], rax
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n7_α
.Lx5_0:
 .quad .Lx5_0_s
.Lx5_0_s:
 .string "y"
# IR_LIT_STRING
 xchain0_n4_α:
 mov qword ptr [r12 + 224], 1
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 232], rax
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n7_α
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "z"
 xchain0_n5_α:
# BOX IR_CALL node(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+192] -> [zr+144]
 mov rax, qword ptr [r12 + 192]
 mov qword ptr [r12 + 144], rax
 mov rax, qword ptr [r12 + 200]
 mov qword ptr [r12 + 152], rax
# marshal arg1 = producer-box slot [zr+208] -> [zr+160]
 mov rax, qword ptr [r12 + 208]
 mov qword ptr [r12 + 160], rax
 mov rax, qword ptr [r12 + 216]
 mov qword ptr [r12 + 168], rax
# marshal arg2 = producer-box slot [zr+224] -> [zr+176]
 mov rax, qword ptr [r12 + 224]
 mov qword ptr [r12 + 176], rax
 mov rax, qword ptr [r12 + 232]
 mov qword ptr [r12 + 184], rax
  .section .rodata
  .Lbynamefn8: .string "node"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn8]
 lea rsi, [r12 + 144]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
 cmp eax, 99
 je xchain0_n7_α
 jmp xchain0_n6_α
xchain0_n5_β:
 jmp xchain0_n7_α
# IR_ASSIGN gva
 xchain0_n6_α:
 mov rax, qword ptr [r12 + 128]
 mov rdx, qword ptr [r12 + 136]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp xchain0_n7_α
# IR_VAR
 xchain0_n7_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp xchain0_n9_α
 xchain0_n8_α:
# BOX IR_CALL datatype(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+384] -> [zr+368]
 mov rax, qword ptr [r12 + 384]
 mov qword ptr [r12 + 368], rax
 mov rax, qword ptr [r12 + 392]
 mov qword ptr [r12 + 376], rax
  .section .rodata
  .Lbynamefn12: .string "datatype"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn12]
 lea rsi, [r12 + 368]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 352], rax
 mov qword ptr [r12 + 360], rdx
 cmp eax, 99
 je xchain0_n9_α
 jmp xchain0_n10_α
xchain0_n8_β:
 jmp xchain0_n9_α
# IR_VAR
 xchain0_n9_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 560], rax
 mov qword ptr [r12 + 568], rdx
 jmp xchain0_n11_α
 xchain0_n9_β:
 jmp xchain0_n12_α
# IR_KEYWORD_SNOBOL4_read
 xchain0_n10_α:
 mov rdi, qword ptr [rip + .Lx14_0]
 call rt_keyword_read_snobol4@PLT
 mov qword ptr [r12 + 400], rax
 mov qword ptr [r12 + 408], rdx
 jmp xchain0_n13_α
 xchain0_n10_β:
 jmp xchain0_n9_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "lcase"
 xchain0_n11_α:
# BOX IR_CALL val(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+560] -> [zr+544]
 mov rax, qword ptr [r12 + 560]
 mov qword ptr [r12 + 544], rax
 mov rax, qword ptr [r12 + 568]
 mov qword ptr [r12 + 552], rax
  .section .rodata
  .Lbynamefn16: .string "val"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn16]
 lea rsi, [r12 + 544]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 528], rax
 mov qword ptr [r12 + 536], rdx
 cmp eax, 99
 je xchain0_n12_α
 jmp xchain0_n14_α
xchain0_n11_β:
 jmp xchain0_n12_α
 xchain0_n12_α:
# BOX IR_CALL node(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn18: .string "node"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn18]
 lea rsi, [r12 + 656]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 640], rax
 mov qword ptr [r12 + 648], rdx
 cmp eax, 99
 je xchain0_n16_α
 jmp xchain0_n15_α
xchain0_n12_β:
 jmp xchain0_n16_α
# IR_KEYWORD_SNOBOL4_read
 xchain0_n13_α:
 mov rdi, qword ptr [rip + .Lx19_0]
 call rt_keyword_read_snobol4@PLT
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 jmp xchain0_n17_α
 xchain0_n13_β:
 jmp xchain0_n9_α
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "ucase"
# IR_LIT_STRING
 xchain0_n14_α:
 mov qword ptr [r12 + 576], 1
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [r12 + 584], rax
 jmp xchain0_n18_α
 xchain0_n14_β:
 jmp xchain0_n12_α
.Lx20_0:
 .quad .Lx20_0_s
.Lx20_0_s:
 .string "x"
# IR_ASSIGN gva
 xchain0_n15_α:
 mov rax, qword ptr [r12 + 640]
 mov rdx, qword ptr [r12 + 648]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [r12 + 624], rax
 mov qword ptr [r12 + 632], rdx
 jmp xchain0_n16_α
 xchain0_n15_β:
 jmp xchain0_n16_α
# IR_VAR
 xchain0_n16_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 720], rax
 mov qword ptr [r12 + 728], rdx
 jmp xchain0_n19_α
 xchain0_n16_β:
 jmp xchain0_n20_α
 xchain0_n17_α:
# BOX IR_CALL replace(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+352] -> [zr+304]
 mov rax, qword ptr [r12 + 352]
 mov qword ptr [r12 + 304], rax
 mov rax, qword ptr [r12 + 360]
 mov qword ptr [r12 + 312], rax
# marshal arg1 = producer-box slot [zr+400] -> [zr+320]
 mov rax, qword ptr [r12 + 400]
 mov qword ptr [r12 + 320], rax
 mov rax, qword ptr [r12 + 408]
 mov qword ptr [r12 + 328], rax
# marshal arg2 = producer-box slot [zr+416] -> [zr+336]
 mov rax, qword ptr [r12 + 416]
 mov qword ptr [r12 + 336], rax
 mov rax, qword ptr [r12 + 424]
 mov qword ptr [r12 + 344], rax
  .section .rodata
  .Lbynamefn24: .string "replace"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn24]
 lea rsi, [r12 + 304]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 288], rax
 mov qword ptr [r12 + 296], rdx
 cmp eax, 99
 je xchain0_n9_α
 jmp xchain0_n21_α
xchain0_n17_β:
 jmp xchain0_n9_α
 xchain0_n18_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+528] -> [zr+496]
 mov rax, qword ptr [r12 + 528]
 mov qword ptr [r12 + 496], rax
 mov rax, qword ptr [r12 + 536]
 mov qword ptr [r12 + 504], rax
# marshal arg1 = producer-box slot [zr+576] -> [zr+512]
 mov rax, qword ptr [r12 + 576]
 mov qword ptr [r12 + 512], rax
 mov rax, qword ptr [r12 + 584]
 mov qword ptr [r12 + 520], rax
  .section .rodata
  .Lbynamefn26: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn26]
 lea rsi, [r12 + 496]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 480], rax
 mov qword ptr [r12 + 488], rdx
 cmp eax, 99
 je xchain0_n12_α
 jmp xchain0_n22_α
xchain0_n18_β:
 jmp xchain0_n12_α
 xchain0_n19_α:
# BOX IR_CALL rson(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+720] -> [zr+704]
 mov rax, qword ptr [r12 + 720]
 mov qword ptr [r12 + 704], rax
 mov rax, qword ptr [r12 + 728]
 mov qword ptr [r12 + 712], rax
  .section .rodata
  .Lbynamefn28: .string "rson"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn28]
 lea rsi, [r12 + 704]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 688], rax
 mov qword ptr [r12 + 696], rdx
 cmp eax, 99
 je xchain0_n20_α
 jmp xchain0_n23_α
xchain0_n19_β:
 jmp xchain0_n20_α
# IR_LIT_STRING
 xchain0_n20_α:
 mov qword ptr [r12 + 768], 1
 mov rax, qword ptr [rip + .Lx29_0]
 mov qword ptr [r12 + 776], rax
 jmp xchain0_n24_α
 xchain0_n20_β:
 jmp xchain0_n29_α
.Lx29_0:
 .quad .Lx29_0_s
.Lx29_0_s:
 .string ""
# IR_LIT_STRING
 xchain0_n21_α:
 mov qword ptr [r12 + 432], 1
 mov rax, qword ptr [rip + .Lx30_0]
 mov qword ptr [r12 + 440], rax
 jmp xchain0_n25_α
 xchain0_n21_β:
 jmp xchain0_n9_α
.Lx30_0:
 .quad .Lx30_0_s
.Lx30_0_s:
 .string "NODE"
# IR_LIT_STRING
 xchain0_n22_α:
 mov qword ptr [r12 + 608], 1
 mov rax, qword ptr [rip + .Lx31_0]
 mov qword ptr [r12 + 616], rax
 jmp xchain0_n26_α
 xchain0_n22_β:
 jmp main_γ
.Lx31_0:
 .quad .Lx31_0_s
.Lx31_0_s:
 .string "FAIL 1115/002: field accessor val"
 xchain0_n23_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+688] -> [zr+672]
 mov rax, qword ptr [r12 + 688]
 mov qword ptr [r12 + 672], rax
 mov rax, qword ptr [r12 + 696]
 mov qword ptr [r12 + 680], rax
  .section .rodata
  .Lbynamefn33: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn33]
 lea rsi, [r12 + 672]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 656], rax
 mov qword ptr [r12 + 664], rdx
 cmp eax, 99
 je xchain0_n20_α
 jmp xchain0_n27_α
xchain0_n23_β:
 jmp xchain0_n20_α
 xchain0_n24_α:
# BOX IR_CALL SNO$WANTNM(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+768] -> [zr+800]
 mov rax, qword ptr [r12 + 768]
 mov qword ptr [r12 + 800], rax
 mov rax, qword ptr [r12 + 776]
 mov qword ptr [r12 + 808], rax
  .section .rodata
  .Lrkfn35: .string "SNO$WANTNM"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn35]
 lea rsi, [r12 + 800]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 784], rax
 mov qword ptr [r12 + 792], rdx
 cmp eax, 99
 je xchain0_n29_α
 jmp xchain0_n28_α
 xchain0_n24_β:
 jmp xchain0_n29_α
 xchain0_n25_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+288] -> [zr+256]
 mov rax, qword ptr [r12 + 288]
 mov qword ptr [r12 + 256], rax
 mov rax, qword ptr [r12 + 296]
 mov qword ptr [r12 + 264], rax
# marshal arg1 = producer-box slot [zr+432] -> [zr+272]
 mov rax, qword ptr [r12 + 432]
 mov qword ptr [r12 + 272], rax
 mov rax, qword ptr [r12 + 440]
 mov qword ptr [r12 + 280], rax
  .section .rodata
  .Lbynamefn37: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn37]
 lea rsi, [r12 + 256]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 cmp eax, 99
 je xchain0_n9_α
 jmp xchain0_n30_α
xchain0_n25_β:
 jmp xchain0_n9_α
# IR_ASSIGN gva
 xchain0_n26_α:
 mov rax, qword ptr [r12 + 608]
 mov rdx, qword ptr [r12 + 616]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 592], rax
 mov qword ptr [r12 + 600], rdx
 jmp main_γ
 xchain0_n26_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n27_α:
 mov qword ptr [r12 + 752], 1
 mov rax, qword ptr [rip + .Lx39_0]
 mov qword ptr [r12 + 760], rax
 jmp xchain0_n31_α
 xchain0_n27_β:
 jmp main_γ
.Lx39_0:
 .quad .Lx39_0_s
.Lx39_0_s:
 .string "FAIL 1115/003: unset field is null"
# IR_VAR
 xchain0_n28_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 848], rax
 mov qword ptr [r12 + 856], rdx
 jmp xchain0_n32_α
 xchain0_n28_β:
 jmp xchain0_n29_α
# IR_VAR
 xchain0_n29_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 1008], rax
 mov qword ptr [r12 + 1016], rdx
 jmp xchain0_n33_α
 xchain0_n29_β:
 jmp xchain0_n34_α
# IR_LIT_STRING
 xchain0_n30_α:
 mov qword ptr [r12 + 464], 1
 mov rax, qword ptr [rip + .Lx42_0]
 mov qword ptr [r12 + 472], rax
 jmp xchain0_n35_α
 xchain0_n30_β:
 jmp main_γ
.Lx42_0:
 .quad .Lx42_0_s
.Lx42_0_s:
 .string "FAIL 1115/001: datatype of node"
# IR_ASSIGN gva
 xchain0_n31_α:
 mov rax, qword ptr [r12 + 752]
 mov rdx, qword ptr [r12 + 760]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 736], rax
 mov qword ptr [r12 + 744], rdx
 jmp main_γ
 xchain0_n31_β:
 jmp main_γ
 xchain0_n32_α:
# BOX IR_CALL lson(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+848] -> [zr+832]
 mov rax, qword ptr [r12 + 848]
 mov qword ptr [r12 + 832], rax
 mov rax, qword ptr [r12 + 856]
 mov qword ptr [r12 + 840], rax
  .section .rodata
  .Lbynamefn45: .string "lson"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn45]
 lea rsi, [r12 + 832]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 816], rax
 mov qword ptr [r12 + 824], rdx
 cmp eax, 99
 je xchain0_n29_α
 jmp xchain0_n36_α
xchain0_n32_β:
 jmp xchain0_n29_α
 xchain0_n33_α:
# BOX IR_CALL lson(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1008] -> [zr+992]
 mov rax, qword ptr [r12 + 1008]
 mov qword ptr [r12 + 992], rax
 mov rax, qword ptr [r12 + 1016]
 mov qword ptr [r12 + 1000], rax
  .section .rodata
  .Lbynamefn47: .string "lson"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn47]
 lea rsi, [r12 + 992]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 976], rax
 mov qword ptr [r12 + 984], rdx
 cmp eax, 99
 je xchain0_n34_α
 jmp xchain0_n37_α
xchain0_n33_β:
 jmp xchain0_n34_α
# IR_LIT_STRING
 xchain0_n34_α:
 mov qword ptr [r12 + 1152], 1
 mov rax, qword ptr [rip + .Lx48_0]
 mov qword ptr [r12 + 1160], rax
 jmp xchain0_n38_α
 xchain0_n34_β:
 jmp xchain0_n42_α
.Lx48_0:
 .quad .Lx48_0_s
.Lx48_0_s:
 .string "b"
# IR_ASSIGN gva
 xchain0_n35_α:
 mov rax, qword ptr [r12 + 464]
 mov rdx, qword ptr [r12 + 472]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 448], rax
 mov qword ptr [r12 + 456], rdx
 jmp main_γ
 xchain0_n35_β:
 jmp main_γ
# IR_VAR
 xchain0_n36_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 864], rax
 mov qword ptr [r12 + 872], rdx
 jmp xchain0_n39_α
 xchain0_n36_β:
 jmp xchain0_n29_α
 xchain0_n37_α:
# BOX IR_CALL rson(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+976] -> [zr+960]
 mov rax, qword ptr [r12 + 976]
 mov qword ptr [r12 + 960], rax
 mov rax, qword ptr [r12 + 984]
 mov qword ptr [r12 + 968], rax
  .section .rodata
  .Lbynamefn52: .string "rson"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn52]
 lea rsi, [r12 + 960]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 944], rax
 mov qword ptr [r12 + 952], rdx
 cmp eax, 99
 je xchain0_n34_α
 jmp xchain0_n40_α
xchain0_n37_β:
 jmp xchain0_n34_α
 xchain0_n38_α:
# BOX IR_CALL value(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1152] -> [zr+1136]
 mov rax, qword ptr [r12 + 1152]
 mov qword ptr [r12 + 1136], rax
 mov rax, qword ptr [r12 + 1160]
 mov qword ptr [r12 + 1144], rax
  .section .rodata
  .Lbynamefn54: .string "value"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn54]
 lea rsi, [r12 + 1136]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1120], rax
 mov qword ptr [r12 + 1128], rdx
 cmp eax, 99
 je xchain0_n42_α
 jmp xchain0_n41_α
xchain0_n38_β:
 jmp xchain0_n42_α
# IR_ASSIGN_VAR
 xchain0_n39_α:
 mov rdi, qword ptr [r12 + 816]
 mov rsi, qword ptr [r12 + 824]
 mov rdx, qword ptr [r12 + 864]
 mov rcx, qword ptr [r12 + 872]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n29_α
 mov qword ptr [r12 + 880], rax
 mov qword ptr [r12 + 888], rdx
 jmp xchain0_n29_α
 xchain0_n39_β:
 jmp xchain0_n29_α
# IR_LIT_STRING
 xchain0_n40_α:
 mov qword ptr [r12 + 1024], 1
 mov rax, qword ptr [rip + .Lx56_0]
 mov qword ptr [r12 + 1032], rax
 jmp xchain0_n43_α
 xchain0_n40_β:
 jmp xchain0_n34_α
.Lx56_0:
 .quad .Lx56_0_s
.Lx56_0_s:
 .string "z"
# IR_VAR
 xchain0_n41_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 1168], rax
 mov qword ptr [r12 + 1176], rdx
 jmp xchain0_n44_α
 xchain0_n41_β:
 jmp xchain0_n42_α
# IR_LIT_STRING
 xchain0_n42_α:
 mov qword ptr [r12 + 1216], 1
 mov rax, qword ptr [rip + .Lx58_0]
 mov qword ptr [r12 + 1224], rax
 jmp xchain0_n45_α
 xchain0_n42_β:
 jmp xchain0_n49_α
.Lx58_0:
 .quad .Lx58_0_s
.Lx58_0_s:
 .string ""
 xchain0_n43_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+944] -> [zr+912]
 mov rax, qword ptr [r12 + 944]
 mov qword ptr [r12 + 912], rax
 mov rax, qword ptr [r12 + 952]
 mov qword ptr [r12 + 920], rax
# marshal arg1 = producer-box slot [zr+1024] -> [zr+928]
 mov rax, qword ptr [r12 + 1024]
 mov qword ptr [r12 + 928], rax
 mov rax, qword ptr [r12 + 1032]
 mov qword ptr [r12 + 936], rax
  .section .rodata
  .Lbynamefn60: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn60]
 lea rsi, [r12 + 912]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 896], rax
 mov qword ptr [r12 + 904], rdx
 cmp eax, 99
 je xchain0_n34_α
 jmp xchain0_n46_α
xchain0_n43_β:
 jmp xchain0_n34_α
 xchain0_n44_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1120] -> [zr+1088]
 mov rax, qword ptr [r12 + 1120]
 mov qword ptr [r12 + 1088], rax
 mov rax, qword ptr [r12 + 1128]
 mov qword ptr [r12 + 1096], rax
# marshal arg1 = producer-box slot [zr+1168] -> [zr+1104]
 mov rax, qword ptr [r12 + 1168]
 mov qword ptr [r12 + 1104], rax
 mov rax, qword ptr [r12 + 1176]
 mov qword ptr [r12 + 1112], rax
  .section .rodata
  .Lbynamefn62: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn62]
 lea rsi, [r12 + 1088]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1072], rax
 mov qword ptr [r12 + 1080], rdx
 cmp eax, 99
 je xchain0_n42_α
 jmp xchain0_n47_α
xchain0_n44_β:
 jmp xchain0_n42_α
 xchain0_n45_α:
# BOX IR_CALL SNO$WANTNM(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1216] -> [zr+1248]
 mov rax, qword ptr [r12 + 1216]
 mov qword ptr [r12 + 1248], rax
 mov rax, qword ptr [r12 + 1224]
 mov qword ptr [r12 + 1256], rax
  .section .rodata
  .Lrkfn64: .string "SNO$WANTNM"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn64]
 lea rsi, [r12 + 1248]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1232], rax
 mov qword ptr [r12 + 1240], rdx
 cmp eax, 99
 je xchain0_n49_α
 jmp xchain0_n48_α
 xchain0_n45_β:
 jmp xchain0_n49_α
# IR_LIT_STRING
 xchain0_n46_α:
 mov qword ptr [r12 + 1056], 1
 mov rax, qword ptr [rip + .Lx65_0]
 mov qword ptr [r12 + 1064], rax
 jmp xchain0_n50_α
 xchain0_n46_β:
 jmp main_γ
.Lx65_0:
 .quad .Lx65_0_s
.Lx65_0_s:
 .string "FAIL 1115/004: nested accessor after mutate"
# IR_LIT_STRING
 xchain0_n47_α:
 mov qword ptr [r12 + 1200], 1
 mov rax, qword ptr [rip + .Lx66_0]
 mov qword ptr [r12 + 1208], rax
 jmp xchain0_n51_α
 xchain0_n47_β:
 jmp main_γ
.Lx66_0:
 .quad .Lx66_0_s
.Lx66_0_s:
 .string "FAIL 1115/005: value() by variable name"
# IR_VAR
 xchain0_n48_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 1296], rax
 mov qword ptr [r12 + 1304], rdx
 jmp xchain0_n52_α
 xchain0_n48_β:
 jmp xchain0_n49_α
# IR_VAR
 xchain0_n49_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 1424], rax
 mov qword ptr [r12 + 1432], rdx
 jmp xchain0_n53_α
 xchain0_n49_β:
 jmp xchain0_n54_α
# IR_ASSIGN gva
 xchain0_n50_α:
 mov rax, qword ptr [r12 + 1056]
 mov rdx, qword ptr [r12 + 1064]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1040], rax
 mov qword ptr [r12 + 1048], rdx
 jmp main_γ
 xchain0_n50_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain0_n51_α:
 mov rax, qword ptr [r12 + 1200]
 mov rdx, qword ptr [r12 + 1208]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1184], rax
 mov qword ptr [r12 + 1192], rdx
 jmp main_γ
 xchain0_n51_β:
 jmp main_γ
 xchain0_n52_α:
# BOX IR_CALL val(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1296] -> [zr+1280]
 mov rax, qword ptr [r12 + 1296]
 mov qword ptr [r12 + 1280], rax
 mov rax, qword ptr [r12 + 1304]
 mov qword ptr [r12 + 1288], rax
  .section .rodata
  .Lbynamefn72: .string "val"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn72]
 lea rsi, [r12 + 1280]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1264], rax
 mov qword ptr [r12 + 1272], rdx
 cmp eax, 99
 je xchain0_n49_α
 jmp xchain0_n55_α
xchain0_n52_β:
 jmp xchain0_n49_α
 xchain0_n53_α:
# BOX IR_CALL val(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1424] -> [zr+1408]
 mov rax, qword ptr [r12 + 1424]
 mov qword ptr [r12 + 1408], rax
 mov rax, qword ptr [r12 + 1432]
 mov qword ptr [r12 + 1416], rax
  .section .rodata
  .Lbynamefn74: .string "val"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn74]
 lea rsi, [r12 + 1408]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1392], rax
 mov qword ptr [r12 + 1400], rdx
 cmp eax, 99
 je xchain0_n54_α
 jmp xchain0_n56_α
xchain0_n53_β:
 jmp xchain0_n54_α
# IR_LIT_STRING
 xchain0_n54_α:
 mov qword ptr [r12 + 1504], 1
 mov rax, qword ptr [rip + .Lx75_0]
 mov qword ptr [r12 + 1512], rax
 jmp xchain0_n57_α
 xchain0_n54_β:
 jmp main_γ
.Lx75_0:
 .quad .Lx75_0_s
.Lx75_0_s:
 .string "PASS 1115_data_basic (6/6)"
# IR_LIT_STRING
 xchain0_n55_α:
 mov qword ptr [r12 + 1312], 1
 mov rax, qword ptr [rip + .Lx76_0]
 mov qword ptr [r12 + 1320], rax
 jmp xchain0_n58_α
 xchain0_n55_β:
 jmp xchain0_n49_α
.Lx76_0:
 .quad .Lx76_0_s
.Lx76_0_s:
 .string "new"
# IR_LIT_STRING
 xchain0_n56_α:
 mov qword ptr [r12 + 1440], 1
 mov rax, qword ptr [rip + .Lx77_0]
 mov qword ptr [r12 + 1448], rax
 jmp xchain0_n59_α
 xchain0_n56_β:
 jmp xchain0_n54_α
.Lx77_0:
 .quad .Lx77_0_s
.Lx77_0_s:
 .string "new"
# IR_ASSIGN gva
 xchain0_n57_α:
 mov rax, qword ptr [r12 + 1504]
 mov rdx, qword ptr [r12 + 1512]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1488], rax
 mov qword ptr [r12 + 1496], rdx
 jmp main_γ
 xchain0_n57_β:
 jmp main_γ
# IR_ASSIGN_VAR
 xchain0_n58_α:
 mov rdi, qword ptr [r12 + 1264]
 mov rsi, qword ptr [r12 + 1272]
 mov rdx, qword ptr [r12 + 1312]
 mov rcx, qword ptr [r12 + 1320]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n49_α
 mov qword ptr [r12 + 1328], rax
 mov qword ptr [r12 + 1336], rdx
 jmp xchain0_n49_α
 xchain0_n58_β:
 jmp xchain0_n49_α
 xchain0_n59_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1392] -> [zr+1360]
 mov rax, qword ptr [r12 + 1392]
 mov qword ptr [r12 + 1360], rax
 mov rax, qword ptr [r12 + 1400]
 mov qword ptr [r12 + 1368], rax
# marshal arg1 = producer-box slot [zr+1440] -> [zr+1376]
 mov rax, qword ptr [r12 + 1440]
 mov qword ptr [r12 + 1376], rax
 mov rax, qword ptr [r12 + 1448]
 mov qword ptr [r12 + 1384], rax
  .section .rodata
  .Lbynamefn81: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn81]
 lea rsi, [r12 + 1360]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1344], rax
 mov qword ptr [r12 + 1352], rdx
 cmp eax, 99
 je xchain0_n54_α
 jmp xchain0_n60_α
xchain0_n59_β:
 jmp xchain0_n54_α
# IR_LIT_STRING
 xchain0_n60_α:
 mov qword ptr [r12 + 1472], 1
 mov rax, qword ptr [rip + .Lx82_0]
 mov qword ptr [r12 + 1480], rax
 jmp xchain0_n61_α
 xchain0_n60_β:
 jmp main_γ
.Lx82_0:
 .quad .Lx82_0_s
.Lx82_0_s:
 .string "FAIL 1115/006: mutate field and read back"
# IR_ASSIGN gva
 xchain0_n61_α:
 mov rax, qword ptr [r12 + 1472]
 mov rdx, qword ptr [r12 + 1480]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1456], rax
 mov qword ptr [r12 + 1464], rdx
 jmp main_γ
 xchain0_n61_β:
 jmp main_γ
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
 mov rdi, qword ptr [r12 + 1520]
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
